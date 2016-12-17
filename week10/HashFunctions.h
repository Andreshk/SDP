#pragma once
#include<cstdint> // за uint64_t - името идва от unsigned int, 64-bit type
#include<string>
#include<vector>

// Три специални хеширащи функции.
// Fowler-Noll-Vo hash
uint64_t bitwiseHash1(const char* bytes, size_t size)
{
    uint64_t result = 2166136261;
    for (size_t i = 0; i < size; i++)
    {
        result = (result * 16777619) ^ bytes[i];
    }
    return result;
}

// One-At-A-Time hash by Jenkins
uint64_t bitwiseHash2(const char* bytes, size_t size)
{
    uint64_t result = 0;
    for (size_t i = 0; i < size; i++)
    {
        result += bytes[i];
        result += (result << 10);
        result ^= (result >> 6);
    }
    result += (result << 3);
    result ^= (result >> 11);
    result += (result << 15);
    return result;
}

// Shift-Add-XOR hash
uint64_t bitwiseHash3(const char* bytes, size_t size)
{
    uint64_t result = 0;
    for (size_t i = 0; i < size; i++)
    {
        result ^= (result << 5) + (result >> 2) + bytes[i];
    }
    return result;
}

// Общи функции, аналогични на тези от Source.cpp
template<class T>
uint64_t myHash1(const T& obj)
{
    return bitwiseHash1((const char*)&obj, sizeof(obj));
}
template<class T>
uint64_t myHash2(const T& obj)
{
    return bitwiseHash2((const char*)&obj, sizeof(obj));
}
template<class T>
uint64_t myHash3(const T& obj)
{
    return bitwiseHash3((const char*)&obj, sizeof(obj));
}

// Специализации за вектори. Става малко еднообразно,
// начинът за заобикаляне е мааааалко грозен. (hint: шаблони)
template<class T>
uint64_t myHash1(const std::vector<T>& obj)
{
    return bitwiseHash1((const char*)obj.data(), obj.size()*sizeof(T));
}

template<class T>
uint64_t myHash2(const std::vector<T>& obj)
{
    return bitwiseHash2((const char*)obj.data(), obj.size()*sizeof(T));
}

template<class T>
uint64_t myHash3(const std::vector<T>& obj)
{
    return bitwiseHash3((const char*)obj.data(), obj.size()*sizeof(T));
}

// и за стрингове
uint64_t myHash1(const std::string& str)
{
    return bitwiseHash1(str.data(), str.size());
}

uint64_t myHash2(const std::string& str)
{
    return bitwiseHash2(str.data(), str.size());
}

uint64_t myHash3(const std::string& str)
{
    return bitwiseHash3(str.data(), str.size());
}
