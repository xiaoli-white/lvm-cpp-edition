//
// Created by XiaoLi on 25-8-14.
//

#ifndef VM_H
#define VM_H
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include "memory.h"
#include "module.h"

#ifdef __WIN32
#include <Windows.h>
#else
#include <signal.h>
#include <sys/mman.h>
#include <setjmp.h>
#include <errno.h>
#ifdef __APPLE__
#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)
#else
#define PAGE_SIZE sysconf(_SC_PAGESIZE)
#endif
#endif


namespace lvm
{
    constexpr const char* VERSION_STRING = "0.2.1";
    constexpr uint64_t DEFAULT_STACK_SIZE = 4 * 1024 * 1024;
    constexpr uint64_t DEFAULT_MEMORY_SIZE = 1024 * 1024 * 1024;
    constexpr uint64_t LVM_VERSION = 0;
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    constexpr uint8_t ENDIAN = 0;
#else
    constexpr uint8_t ENDIAN = 1;
#endif

    class VirtualMachine;
    class ThreadHandle;
    class ExecutionUnit;
    class FileHandle;
    class Memory;
    class FreeMemory;

    inline VirtualMachine* currentVirtualMachine;
    inline thread_local ExecutionUnit* currentExecutionUnit;

    class VirtualMachine
    {
    public:
        const uint64_t stackSize;
        Memory* memory;
        std::map<uint64_t, ThreadHandle*> threadID2Handle;
        uint64_t entryPoint = 0;

        VirtualMachine(uint64_t heapSize, uint64_t stackSize);
        int init(const Module* module);
        void destroy();
        int run();
        uint64_t createThread(ThreadHandle* threadHandle, uint64_t entryPoint);
        inline uint64_t open(const char* path, uint32_t flags, uint32_t mode);
        inline uint64_t close(uint64_t fd);
        inline uint32_t read(uint64_t fd, uint8_t* buffer, uint32_t count);
        inline uint32_t write(uint64_t fd, const uint8_t* buffer, uint32_t count);
        void exit(uint64_t status);

    private:
        bool running = false;
        std::map<uint64_t, FileHandle*> fd2FileHandle;
        uint64_t lastFd = 0;
        uint64_t lastThreadID = 0;
        std::recursive_mutex _mutex;

        ExecutionUnit* createExecutionUnit(ThreadHandle* threadHandle, uint64_t entryPoint);
        void destroyThread(const ThreadHandle* threadHandle);
        uint64_t getThreadID();
        uint64_t getFd();
    };

    class ThreadHandle
    {
    public:
        const uint64_t threadID;
        ExecutionUnit* executionUnit;
        std::thread* _thread = nullptr;
        ThreadHandle(uint64_t threadID, ExecutionUnit* executionUnit);
        ~ThreadHandle();
        void start();

    private:
        std::mutex _mutex;
    };

    class ExecutionUnit
    {
    public:
        uint64_t* registers = nullptr;

        explicit ExecutionUnit(VirtualMachine* virtualMachine);
        void init(uint64_t stackBase, uint64_t entryPoint);
        void setThreadHandle(ThreadHandle* threadHandle);
        void execute();
        void interrupt(uint8_t interruptNumber) const;
        void destroy();

    private:
        VirtualMachine* virtualMachine;
        ThreadHandle* threadHandle = nullptr;
        std::mutex _mutex;
    };

    class FileHandle
    {
    public:
        static constexpr uint32_t FH_READ = 1;
        static constexpr uint32_t FH_WRITE = 1 << 1;

        FileHandle(std::string path, uint32_t flags, uint32_t mode, FILE* input, FILE* output);
        FileHandle(std::string path, uint32_t flags, uint32_t mode);
        ~FileHandle();
        inline uint32_t _read(uint8_t* buffer, uint32_t count) const;
        inline uint32_t _write(const uint8_t* buffer, uint32_t count) const;

    private:
        static constexpr uint32_t FH_PREOPEN = 1 << 2;
        const std::string path;
        const uint32_t flags;
        const uint32_t mode;
        FILE* input;
        FILE* output;
    };
#ifdef __WIN32
    LONG WINAPI pageFaultHandler(PEXCEPTION_POINTERS ExceptionInfo);
#else
    inline sigjmp_buf env;
    void PageFaultHandler(int sig, siginfo_t* info, void* context);
#endif
    void InstallPageFaultHandler();


    class Memory
    {
    public:
        FreeMemory* freeMemoryList = nullptr;
        uint64_t heapSize;
        void* heap;
#ifdef  __WIN32
#else
        void* metadata;
#endif


        explicit Memory(uint64_t heapSize);
        void init(const uint8_t* text, uint64_t textLength, const uint8_t* rodata, uint64_t rodataLength,
                  const uint8_t* data, uint64_t dataLength, uint64_t bssLength);
        void lock();
        void unlock();
        uint64_t allocateMemory(ThreadHandle* threadHandle, uint64_t size);
        uint64_t reallocateMemory(ThreadHandle* threadHandle, uint64_t address, uint64_t newSize);
        void freeMemory(ThreadHandle* threadHandle, uint64_t address);
        uint64_t allocateMemoryWithoutHead(ThreadHandle* threadHandle, uint64_t size);
        static bool setReadonly(uint64_t address, uint64_t size);
        static bool setReadwrite(uint64_t address, uint64_t size);

    private:
        std::recursive_mutex _mutex;
        std::recursive_mutex _lock;
    };

    class FreeMemory
    {
    public:
        uint64_t start;
        uint64_t end;
        FreeMemory* next = nullptr;

        FreeMemory(uint64_t start, uint64_t end);
        ~FreeMemory();
    };
}
#endif //VM_H
