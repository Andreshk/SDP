#include <iostream>
#include "PairingHeap.h"

template<class T>
void heapTest(std::vector<T> values)
{
    PairingHeap<T> ph;
    std::vector<typename PairingHeap<T>::proxy> pxs;

    // не просто добавяме елементите в пирамидата,
    // ами и запазваме "проксита" към всеки един от тях
    for (const T& x : values)
        pxs.push_back(ph.insert(x));

    // очакваме независимо от структурата на пирамидата
    // стойностите в прокситата да са в същия ред, в който сме ги добавили
    std::cout << "Proxies: ";
    for (const typename PairingHeap<T>::proxy& p : pxs)
        std::cout << *p << " ";
    std::cout << "\n Values: ";
    for (const T& x : values)
        std::cout << x << " ";
    std::cout << "\n";

    // да си тестваме копиконструктора
    PairingHeap<T> ph2 = ph;
    ph.clear();
    // махаме елементите във вектора и ги добавяме в сортиран ред
    values.clear();
    while (!ph2.empty())
        values.push_back(ph2.extractMin());

    std::cout << " Sorted: ";
    for (const T& x : values)
        std::cout << x << " ";
    std::cout << "\n\n";
}

int main()
{
    std::vector<int> v{ 1,3,5,2,4,6,7,9,8,10 };
    heapTest(v);
}
