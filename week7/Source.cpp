#include "BST.h"

template<class T>
struct digit_less
{
    constexpr bool operator()(const T& lhs, const T& rhs) const
    {
        return ((lhs % 10) < (rhs % 10));
    }
};

int main()
{
    BST<int, digit_less<int>> tree;
    for (int i = 0; i < 20; i++)
        tree.insert(30 - i);
    for (int i = 11; i < 40; i++)
        tree.remove(i);

    std::cout << std::boolalpha;
    std::cout << " size: " << tree.size() << "\n";
    std::cout << "empty: " << tree.empty() << "\n";
    std::cout << "valid: " << tree.validate() << "\n";
    std::cout << tree << "\n";
}
