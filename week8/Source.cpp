#include "BST.h"

int main()
{
    BST<int> tr;
    for (int i = 15; i > 5; --i)
        tr.insert(i);

    for (const int& x : tr)
        std::cout << x << " "; // your code here
    std::cout << "\n";

    // аналогично на:
    for (auto it = tr.begin(); it != tr.end(); ++it)
    {
        const int& x = *it;     // има значение инициализацията дали е референция или копие
        std::cout << x << " ";  // your code here
    }
    std::cout << "\n";
}
