#pragma once
#include<iostream>

template<class T>
class BST
{
    class Node // вложеният клас има пълен достъп до private частите на външния..
    {
        friend class BST<T>; // ...но обратното не е вярно!
        T value;
        Node* parent;
        Node* left;
        Node* right;
        Node(const T& _val, Node* _parent) : value(_val), parent(_parent),
            left(nullptr), right(nullptr) {}
    };

    Node* root;
    size_t count;

    // помощни рекурсивни функции - по-рекурсивна структура от дървото здраве му кажи
    static void freeNode(const Node*);
    Node* insertNode(const T&, Node*, Node*);
    bool validateNode(Node*) const;
    static void printNode(std::ostream&, const Node*);
public:
    BST();
    //BST(const BST&); // to-do
    //BST& operator=(const BST&); // to-do
    ~BST();

    void insert(const T&);
    void remove(const T&); // to-do
    bool find(const T&) const; // to-do

    size_t size() const;
    bool empty() const;
    bool validate() const;

    friend std::ostream& operator<<(std::ostream& os, const BST& t)
    {
        if (t.empty())
            return os << "Empty tree!\n";
        printNode(os, t.root);
        return os << "\n";
    }
};

template<class T>
void BST<T>::freeNode(const Node* ptr)
{
    if (ptr)
    {
        freeNode(ptr->left);
        freeNode(ptr->right);
        delete ptr;
    }
}

template<class T>
typename BST<T>::Node* // налага се изрично да споменем, че BST<T>::Node е име на тип...
    BST<T>::insertNode(const T& _val, Node* location, Node* parent)
{
    // във всеки един момент parent е родителя на location
    if (!location)
        location = new Node(_val, parent);
    else if (_val < location->value)
        // вмъкваме вляво и "актуализираме" цялото ляво поддърво
        location->left = insertNode(_val, location->left, location);
    else
        // вмъкваме вдясно и "актуализираме" цялото дясно поддърво
        location->right = insertNode(_val, location->right, location);
    // така "актуализираме" цялото дърво с корен location
    return location;
}

template<class T>
bool BST<T>::validateNode(Node* location) const
{
    // нулевия указател очевидно е указател към валиден Node
    if (!location)
        return true;
    if (location->left)
    {
        // ако има ляво дете, проверяваме стойностите и връзката дете-родител
        if (!((location->left->value < location->value) &&
            (location->left->parent == location)))
            return false;
        // след това проверяваме и цялото ляво поддърво
        if(!validateNode(location->left))
            return false;
    }
    if (location->right)
    {
        // аналогично, ако има дясно дете, проверяваме стойностите и връзката дете-родител
        if (!((location->right->value >= location->value) &&
            (location->right->parent == location)))
            return false;
        // след това проверявам и цялото дясно поддърво
        if (!validateNode(location->right))
            return false;
    }
    // ако всички проверки са минали, връщаме истина
    return true;

    /* // същата проверка в един израз
    return !location ||
        ((!location->left) || (location->left->value < location->value) &&
                              (location->left->parent == location) &&
                              validateNode(location->left)) &&
        ((!location->right) || (location->right->value >= location->value) &&
                               (location->right->parent == location) &&
                               validateNode(location->right));
    */
}

template<class T>
void BST<T>::printNode(std::ostream& os, const Node* ptr)
{
    if (!ptr)
        return;
    // отпечатване ляво-корен-дясно, тоест в ненамаляващ ред
    printNode(os, ptr->left);
    os << ptr->value << " ";
    printNode(os, ptr->right);
}

template<class T>
BST<T>::BST() : root(nullptr), count(0) {} // стандартни

template<class T>
BST<T>::~BST()
{
    // също стандартно
    freeNode(root);
}

template<class T>
void BST<T>::insert(const T& _val)
{
    // цялата магия се случва в рекурсивната insertNode и това как се извиква!
    root = insertNode(_val, root, nullptr);
    ++count; // това го бяхме забравили !!!
}

template<class T>
size_t BST<T>::size() const
{
    return count;
}

template<class T>
bool BST<T>::empty() const
{
    // няколко начина:
    return (root == nullptr);
    // return (size() == 0);
    // return (count == 0);
    // return !root;
    // return !size();
    // return !count;
}

template<class T>
bool BST<T>::validate() const
{
    return validateNode(root);
}
