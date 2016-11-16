#include "BST.h"

int main()
{
    BST<int> tree;
    for (int i = 0; i < 20; i++)
        tree.insert(30 - i);
    std::cout << std::boolalpha;
    std::cout << " size: " << tree.size() << "\n";
    std::cout << "empty: " << tree.empty() << "\n";
    std::cout << "valid: " << tree.validate() << "\n";
    std::cout << tree << "\n";
}
