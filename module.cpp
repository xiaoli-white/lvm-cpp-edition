#include "module.h"

#include <vector>

#include "vm.h"
//
// Created by XiaoLi on 25-8-14.
//


namespace lvm
{
    Module::Module(const uint8_t* text, const uint64_t textLength, const uint8_t* rodata, const uint64_t rodataLength,
                   const uint8_t* data, const uint64_t dataLength, const uint64_t bssLength,
                   const uint64_t entryPoint): text(text), textLength(textLength), rodata(rodata),
                                               rodataLength(rodataLength), data(data), dataLength(dataLength),
                                               bssLength(bssLength), entryPoint(entryPoint)
    {
    }

    Module::~Module()
    {
        delete[] text;
        delete[] rodata;
        delete[] data;
    }

    uint8_t* Module::raw() const
    {
        std::vector<uint8_t> v;
        v.push_back('l');
        v.push_back('v');
        v.push_back('m');
        v.push_back('e');
        v.push_back(ENDIAN);
        for (uint64_t i = 0; i < sizeof(LVM_VERSION); i++)v.push_back(LVM_VERSION >> (i * 8));
        for (uint64_t i = 0; i < sizeof(textLength); i++)v.push_back(textLength >> (i * 8));
        for (uint64_t i = 0; i < textLength; i++)v.push_back(text[i]);
        for (uint64_t i = 0; i < sizeof(rodataLength); i++)v.push_back(rodataLength >> (i * 8));
        for (uint64_t i = 0; i < rodataLength; i++)v.push_back(rodata[i]);
        for (uint64_t i = 0; i < sizeof(dataLength); i++)v.push_back(dataLength >> (i * 8));
        for (uint64_t i = 0; i < dataLength; i++)v.push_back(data[i]);
        for (uint64_t i = 0; i < sizeof(bssLength); i++)v.push_back(bssLength >> (i * 8));
        for (uint64_t i = 0; i < sizeof(entryPoint); i++)v.push_back(entryPoint >> (i * 8));
        auto* raw = new uint8_t[v.size()];
        for (uint64_t i = 0; i < v.size(); i++)raw[i] = v[i];
        return raw;
    }

    Module* Module::fromRaw(const uint8_t* raw)
    {
        uint64_t index = 0;
        if (raw[index++] != 'l' || raw[index++] != 'v' || raw[index++] != 'm' || raw[index++] != 'e')
        {
            throw std::runtime_error("Invalid module");
            return nullptr;
        }
        if (raw[index++] != ENDIAN)
        {
            throw std::runtime_error("Invalid module");
            return nullptr;
        }
        if ((*reinterpret_cast<const uint64_t*>(&raw[index])) != LVM_VERSION)
        {
            throw std::runtime_error("Invalid module");
            return nullptr;
        }
        index += 8;
        const uint64_t textLength = *reinterpret_cast<const uint64_t*>(&raw[index]);
        index += 8;
        auto* text = new uint8_t[textLength]{};
        for (uint64_t i = 0; i < textLength; i++)text[i] = raw[index++];
        const uint64_t rodataLength = *reinterpret_cast<const uint64_t*>(&raw[index]);
        index += 8;
        auto* rodata = new uint8_t[rodataLength]{};
        for (uint64_t i = 0; i < rodataLength; i++)rodata[i] = raw[index++];
        const uint64_t dataLength = *reinterpret_cast<const uint64_t*>(&raw[index]);
        index += 8;
        auto* data = new uint8_t[dataLength]{};
        for (uint64_t i = 0; i < dataLength; i++)data[i] = raw[index++];
        const uint64_t bssLength = *reinterpret_cast<const uint64_t*>(&raw[index]);
        index += 8;
        const uint64_t entryPoint = *reinterpret_cast<const uint64_t*>(&raw[index]);
        return new Module(text, textLength, rodata, rodataLength, data, dataLength, bssLength, entryPoint);
    }
}
