//
// Created by XiaoLi on 25-8-14.
//

#include <iostream>

#include "bytecode.h"
#include "exception.h"
#include "vm.h"
#ifdef  _WIN32
#include <windows.h>
#else
#include <signal.h>
#include <sys/mman.h>
#include <setjmp.h>
#include <errno.h>
#endif


namespace lvm
{
#ifdef  __WIN32
    LONG WINAPI pageFaultHandler(PEXCEPTION_POINTERS ExceptionInfo)
    {
        if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
        {
            auto faultAddress = reinterpret_cast<void*>(ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);

            Memory* memory = currentVirtualMachine->memory;

            if (faultAddress >= memory->heap &&
                faultAddress < reinterpret_cast<void*>(reinterpret_cast<int64_t>(memory->heap) + memory->heapSize))
            {
                MEMORY_BASIC_INFORMATION mbi;
                if (VirtualQuery(faultAddress, &mbi, sizeof(mbi)))
                {
                    if (mbi.State == MEM_RESERVE)
                    {
                        SYSTEM_INFO sysInfo;
                        GetSystemInfo(&sysInfo);
                        size_t pageSize = sysInfo.dwPageSize;

                        auto pageBase = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(faultAddress) & ~(pageSize -
                            1));

                        if (VirtualAlloc(pageBase,
                                         pageSize,
                                         MEM_COMMIT,
                                         PAGE_READWRITE))
                        {
                            memset(pageBase, 0, pageSize);
                            return EXCEPTION_CONTINUE_EXECUTION;
                        }
                    }
                    else if (mbi.State == MEM_COMMIT)
                    {
                        // std::cout << "Page is committed but access violation occurred (permissions issue?)" <<
                        // std::endl;
                    }
                    else if (mbi.State == MEM_FREE)
                    {
                        // std::cout << "Page is free (not reserved)" << std::endl;
                    }
                }
            }
            else
            {
                // std::cout << "Access violation outside managed memory range" << std::endl;
                // std::cout << "Faulting address: " << faultAddress << std::endl;
                // std::cout << "Range: " << memory->heap << " - " << memory->heap + memory->heapSize << std::endl;
            }
        }

        return EXCEPTION_CONTINUE_SEARCH;
    }

    void InstallPageFaultHandler()
    {
        AddVectoredExceptionHandler(1, pageFaultHandler);
    }

    Memory::Memory(const uint64_t heapSize) : heapSize(heapSize)
    {
        heap = VirtualAlloc(nullptr, heapSize, MEM_RESERVE, PAGE_NOACCESS);
        if (!heap)
        {
            printf("Failed to reserve memory space\n");
            exit(1);
        }
        auto* freeMemory = new FreeMemory(0, 0);
        freeMemory->next = new FreeMemory(0, heapSize);
        freeMemoryList = freeMemory;
    }

    bool Memory::setReadonly(uint64_t address, uint64_t size)
    {
        DWORD oldProtection;
        return VirtualProtect(reinterpret_cast<void*>(address), size + 8, PAGE_READONLY, &oldProtection);
    }

    bool Memory::setReadwrite(uint64_t address, uint64_t size)
    {
        DWORD oldProtection;
        return VirtualProtect(reinterpret_cast<void*>(address), size + 8, PAGE_READWRITE, &oldProtection);
    }
#else
    void PageFaultHandler(int sig, siginfo_t* info, void* context)
    {
        if (sig == SIGSEGV)
        {
            void* faultAddress = info->si_addr;

            if (const Memory* memory = currentVirtualMachine->memory; faultAddress >= memory->heap &&
                faultAddress < reinterpret_cast<void*>(reinterpret_cast<int64_t>(memory->heap) + memory->heapSize))
            {
                auto pageBase = reinterpret_cast<void*>(reinterpret_cast<size_t>(faultAddress) & ~(PAGE_SIZE - 1));
                size_t pageIndex = (static_cast<char*>(pageBase) - static_cast<char*>(memory->heap)) / PAGE_SIZE;
                if (!static_cast<bool*>(memory->metadata)[pageIndex])
                {
                    if (mprotect(pageBase, PAGE_SIZE, PROT_READ | PROT_WRITE) == 0)
                    {
                        static_cast<bool*>(memory->metadata)[pageIndex] = true;
                        memset(pageBase, 0, PAGE_SIZE);
                        return;
                    }
                }
            }
        }

        signal(sig, SIG_DFL);
        raise(sig);
    }

    void InstallPageFaultHandler()
    {
        struct sigaction sa{};
        sa.sa_sigaction = PageFaultHandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_SIGINFO;

        if (sigaction(SIGSEGV, &sa, nullptr) == -1)
        {
            perror("Failed to install signal handler");
            exit(EXIT_FAILURE);
        }
    }

    Memory::Memory(uint64_t heapSize) : heapSize(heapSize)
    {
        heap = mmap(nullptr, heapSize,PROT_NONE,MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);

        if (heap == MAP_FAILED)
        {
            perror("Failed to reserve memory space");
            exit(EXIT_FAILURE);
        }

        this->metadata = calloc(heapSize / PAGE_SIZE, sizeof(uint8_t));
        if (!this->metadata)
        {
            perror("Failed to allocate metadata");
            exit(EXIT_FAILURE);
        }

        auto* freeMemory = new FreeMemory(0, 0);
        freeMemory->next = new FreeMemory(0, heapSize);
        freeMemoryList = freeMemory;
    }

    bool Memory::setReadonly(uint64_t address, uint64_t size)
    {
        size = (size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
        auto pageBase = reinterpret_cast<void*>(address & ~(PAGE_SIZE - 1));

        if (mprotect(pageBase, size, PROT_READ) == -1)
        {
            perror("mprotect failed");
            return false;
        }
        return true;
    }

    bool Memory::setReadwrite(uint64_t address, uint64_t size)
    {
        size = (size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
        auto pageBase = reinterpret_cast<void*>(address & ~(PAGE_SIZE - 1));

        if (mprotect(pageBase, size, PROT_READ | PROT_WRITE) == -1)
        {
            perror("mprotect failed");
            return false;
        }
        return true;
    }

#endif


    void Memory::init(const uint8_t* text, const uint64_t textLength, const uint8_t* rodata,
                      const uint64_t rodataLength, const uint8_t* data, const uint64_t dataLength,
                      const uint64_t bssLength)
    {
        const uint64_t textPtr = reinterpret_cast<uint64_t>(heap) + allocateMemoryWithoutHead(nullptr, textLength);
        memcpy(reinterpret_cast<void*>(textPtr), text, textLength);

        const uint64_t rodataPtr = reinterpret_cast<uint64_t>(heap) + allocateMemoryWithoutHead(nullptr, rodataLength);
        memcpy(reinterpret_cast<void*>(rodataPtr), rodata, rodataLength);

        const uint64_t dataPtr = reinterpret_cast<uint64_t>(heap) + allocateMemoryWithoutHead(nullptr, dataLength);
        memcpy(reinterpret_cast<void*>(dataPtr), data, dataLength);

        const uint64_t bssPtr = reinterpret_cast<uint64_t>(heap) + allocateMemoryWithoutHead(nullptr, bssLength);

        setReadonly(textPtr, textLength);
        setReadonly(rodataPtr, rodataLength);
        setReadwrite(dataPtr, dataLength);
        setReadwrite(bssPtr, bssLength);
    }


    void Memory::lock()
    {
        this->_lock.lock();
    }

    void Memory::unlock()
    {
        this->_lock.unlock();
    }

    uint64_t Memory::allocateMemory(ThreadHandle* threadHandle, const uint64_t size)
    {
        std::lock_guard lock(_mutex);
        const uint64_t length = size + 8;
        FreeMemory* freeMemory = this->freeMemoryList->next;
        while (freeMemory != nullptr)
        {
            if (freeMemory->end - freeMemory->start >= length)
            {
                const uint64_t start = freeMemory->start;
                freeMemory->start += length;
                const uint64_t ptr = reinterpret_cast<uint64_t>(heap) + start;
                setReadwrite(ptr, length);
                *reinterpret_cast<uint64_t*>(ptr) = size;
                return start + 8;
            }
            freeMemory = freeMemory->next;
        }
        throw VMException("Out of memory");
    }

    uint64_t Memory::reallocateMemory(ThreadHandle* threadHandle, uint64_t address, uint64_t newSize)
    {
        std::lock_guard lock(_mutex);
        const auto heap = reinterpret_cast<uint64_t>(this->heap);
        const uint64_t oldSize = *reinterpret_cast<uint64_t*>(heap + address - 8);
        auto* bytes = new uint8_t[oldSize];
        memcpy(bytes, reinterpret_cast<void*>(heap + address), oldSize);
        this->freeMemory(threadHandle, address);
        const uint64_t newAddress = this->allocateMemory(threadHandle, newSize);
        memcpy(reinterpret_cast<void*>(heap + newAddress), bytes, std::min(oldSize, newSize));
        delete[] bytes;
        return newAddress;
    }

    void Memory::freeMemory(ThreadHandle* threadHandle, uint64_t address)
    {
        std::lock_guard lock(_mutex);
        address -= 8;
        const uint64_t size = *reinterpret_cast<uint64_t*>(reinterpret_cast<uint64_t>(heap) + address) + 8;
        FreeMemory* freeMemory = this->freeMemoryList->next;
        while (freeMemory->next != nullptr)
        {
            if (freeMemory->end == address)
            {
                freeMemory->end += size;
                break;
            }
            if (freeMemory->start == address + size)
            {
                freeMemory->start -= size;
                break;
            }
            if (freeMemory->end < address && freeMemory->next->start > address + size)
            {
                FreeMemory* next = freeMemory->next;
                freeMemory->next = new FreeMemory(address, address + size);
                freeMemory->next->next = next;
                break;
            }
            freeMemory = freeMemory->next;
        }
        if (freeMemory->end < address && freeMemory->next == nullptr)
        {
            freeMemory->next = new FreeMemory(address, address + size);
        }
    }

    uint64_t Memory::allocateMemoryWithoutHead(ThreadHandle* threadHandle, uint64_t size)
    {
        std::lock_guard lock(_mutex);
        FreeMemory* freeMemory = this->freeMemoryList->next;
        while (freeMemory != nullptr)
        {
            if (freeMemory->end - freeMemory->start >= size)
            {
                const uint64_t start = freeMemory->start;
                freeMemory->start += size;
                return start;
            }
            freeMemory = freeMemory->next;
        }
        throw VMException("Out of memory");
    }


    FreeMemory::FreeMemory(uint64_t start, uint64_t end) : start(start), end(end)
    {
    }

    FreeMemory::~FreeMemory()
    {
        delete this->next;
    }
}
