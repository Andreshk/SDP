#include"HashMap.h"
#include<iostream>

// uint64_t = unsigned int, 64-битов
// Идея: всеки обект в края на краищата е последователност от байтове, така че
// ако можем да хешираме последователности от байтове (т.е. char*), ще можем да хешираме всичко.
uint64_t bitwiseHash(const char* bytes, size_t size)
{
    size_t result = 0;
    for (size_t i = 0; i < size; i++)
        result += bytes[i];
    return result;
}

// Основната функция, която приема обекти от всякакъв тип
// и ги интерпретира като последователности от байтове.
template<class T>
uint64_t myHash(const T& obj)
{
    const char* bytes = (const char*)&obj;
    size_t size = sizeof(obj);
    return bitwiseHash(bytes, size);
}

// Проблем: искаме еднаквите обекти да имат едни и същи хешове. За векторите обаче
// под "еднакви" разбираме едно и също съдържание - а самият обект е един указател и два брояча.
// Решение: правим специалнизация на функцията
template<class T>
uint64_t myHash(const std::vector<T>& obj)
{
    const char* bytes = (const char*)obj.data();
    size_t size = obj.size()*sizeof(T);
    return bitwiseHash(bytes, size);
}

// Аналогичен проблем и за стринговете - пак правим специализация
template<>
uint64_t myHash(const std::string& str)
{
    return bitwiseHash(str.data(), str.size());
}

int main()
{
    int i = 0x207b;
    float f = 3.141592f;
    std::vector<int> v{ 1,2,3,4,5 };
    std::vector<int> v1{ 1,2,3,4,5 };
    std::string s1 = "alAbala", s2 = "alabala";

    std::cout << "myHash(" << i << ") = " << myHash(i) << "\n";
    std::cout << "myHash(" << f << ") = " << myHash(f) << "\n\n";
    // Пример за колизия на глупавата ни хешираща функция.
    std::cout << "myHash(257) = " << myHash(257) << "\n";
    std::cout << "myHash(2) = " << myHash(2) << "\n\n";
    // Пример за работата с вектори:
    std::cout << "myHash(v) = " << myHash(v) << "\n";
    std::cout << "myHash(v1) = " << myHash(v1) << "\n\n";

    HashMap<std::string, int> hm;
    std::cout << std::boolalpha << hm.search("iei") << "\n";
}
