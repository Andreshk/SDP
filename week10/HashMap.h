#pragma once
#include"HashFunctions.h"
#include<vector>
#include<list>

// Забележително: хештаблицата ни дава O(1) амортизирано време за вмъкване, измъкване и търсене
template<class K, class V>
class HashMap
{
    // проста структура, съдържаща ключа, стойността и трите хеша на ключа
    // аналогично на KeyValuePair от лекции
    struct Tuple
    {
        const K key;
        V value;
        uint64_t hash1, hash2, hash3;

        Tuple(const K& _key, const V& _value, uint64_t h1, uint64_t h2, uint64_t h3)
            : key(_key), value(_value), hash1(h1), hash2(h2), hash3(h3) {}
    };

    // динамичен масив от bucket-и, всеки от който е свързан списък
    std::vector<std::list<Tuple>> buckets;

    // помощна функция, която връща указател към намереното value за удобство
    // приема трите хеша като допълнителни аргументи, за да не ги преизчисляваме
    V* _search(uint64_t h1, uint64_t h2, uint64_t h3, const K& _key)
    {
        // намираме bucket-а отново по първия хеш
        size_t idx = h1 % buckets.size();
        // когато търсим стойността обаче, първо сравняваме И ТРИТЕ хеша
        // и едва в случая, когато и трите съвпадат, сравняваме двата ключа
        // Повечето пъти първият хеш е достатъчен, другите са fallback.
        for (Tuple& tup : buckets[idx])
            if (tup.hash1 == h1 && tup.hash2 == h2
                && tup.hash3 == h3 && tup.key == _key)
                return &tup.value;
        return nullptr;
    }

public:
    // търсене по ключ
    bool search(const K& _key)
    {
        // генерираме трите хеша и ги предаваме на private функцията
        uint64_t h1 = myHash1(_key), h2 = myHash1(_key), h3 = myHash3(_key);
        V* location = _search(h1, h2, h3, _key);
        return (location != nullptr);
    }

    // вмъкване на нова стойност за даден ключ
    void insert(const K& _key, const V& _value)
    {
        uint64_t h1 = myHash1(_key), h2 = myHash1(_key), h3 = myHash3(_key);
        V* location = _search(h1, h2, h3, _key);
        if (location == nullptr)
        {
            // ако не сме намерили ключа, го добавяме
            size_t idx = h1 % buckets.size();
            // emplace функцията спестява едно копиране - вместо да конструираме обект
            // и да го push_back-ваме (тоест копираме), директно казваме на контейнера
            // (list-а) как да конструира обекта ни in-place
            buckets[idx].emplace_back(_key, _value, h1, h2, h3);
        }
        else // в противен случай заменяме старата стойност с новата
            *location = _value;
    }
};
