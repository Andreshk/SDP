#pragma once
#include <vector>
#include <cstddef>   // size_t
#include <algorithm> // std::swap

template<class T>
class PairingHeap
{
    struct Node
    {
        T value;
        Node* leftChild;
        Node* rightSibling;
        Node* predecessor;
        Node(const T& _val) : value(_val), leftChild(nullptr),
            rightSibling(nullptr), predecessor(nullptr) {}
    };

    Node* root;
    size_t count;

    // помощни функции за копиране/изтриване
    static Node* copyNode(const Node*, Node*);
    static void freeNode(const Node*);
    void copyFrom(const PairingHeap&);

    // Помощен конструктор, който конструира пирамида по нейния корен
    // (получен напр. от някой наследник), но може да не знае размера й!
    // Използва се и за конструиране на пирамида с един възел (по дадена стойност)
    explicit PairingHeap(Node*, size_t = 0);
public:
    class proxy
    {
        friend class PairingHeap<T>;
        Node* ptr;
        explicit proxy(Node*);
    public:
        const T& operator*() const;
        const T* operator->() const;
        operator bool() const;
        friend bool operator==(const proxy& lhs, const proxy& rhs) { return lhs.ptr == rhs.ptr; }
        friend bool operator!=(const proxy& lhs, const proxy& rhs) { return !(lhs == rhs); }
    };

    // стандартна голяма четворка
    PairingHeap();
    PairingHeap(const PairingHeap&);
    PairingHeap& operator=(const PairingHeap&);
    ~PairingHeap();
    
    // ако имаме пирамидите x и y, то x.merge(y) ВИНАГИ оставя y празна (!),
    // независимо кой от корените е по-малкият; x после съдържа всички елементи на двете.
    void merge(PairingHeap&);

    // вмъкване - създаваме нова пирамида с една стойност и сливаме с дадената
    proxy insert(const T&);

    // класическа операция; взимаме стойността в корена (ако няма такъв - undefined behaviour)
    const T& peek() const;

    // най-тежката операция: премахваме корена, получаваме списък от всичките му деца
    // и лека-полека ги сливаме до получаването на една остатъчна пирамида
    T extractMin();

    // специално (!)
    proxy decreaseKey(proxy, const T&);

    // отново стандартни методи
    size_t size() const;
    bool empty() const;
    // clear() освобождава цялата памет, заета от пирамидата, и я връща във валидно празно състояние
    void clear();

    // бонус - удобна функцийка
    friend void swap(PairingHeap& lhs, PairingHeap& rhs)
    {
        std::swap(lhs.root, rhs.root);
        std::swap(lhs.count, rhs.count);
    }
};

template<class T>
PairingHeap<T>::proxy::proxy(Node* _ptr) : ptr(_ptr) {}

template<class T>
const T& PairingHeap<T>::proxy::operator*() const
{
    return ptr->value;
}

template<class T>
const T* PairingHeap<T>::proxy::operator->() const
{
    return &ptr->value;
}

template<class T>
PairingHeap<T>::proxy::operator bool() const
{
    return bool(ptr);
}

template<class T>
auto PairingHeap<T>::copyNode(const Node* ptr, Node* _pred) -> Node*
{
    if (!ptr)
        return nullptr;
    Node* tmp = new Node(ptr->value);
    tmp->leftChild = copyNode(ptr->leftChild, tmp);
    tmp->rightSibling = copyNode(ptr->rightSibling, tmp);
    tmp->predecessor = _pred;
    return tmp;
}

template<class T>
void PairingHeap<T>::freeNode(const Node* ptr)
{
    if (ptr)
    {
        freeNode(ptr->leftChild);
        freeNode(ptr->rightSibling);
        delete ptr;
    }
}

template<class T>
void PairingHeap<T>::copyFrom(const PairingHeap& other)
{
    root = copyNode(other.root, nullptr);
    count = other.count;
}

template<class T>
PairingHeap<T>::PairingHeap(Node* _root, size_t _count) : root(_root), count(_count) {}

template<class T>
PairingHeap<T>::PairingHeap() : PairingHeap(nullptr, 0) {}

template<class T>
PairingHeap<T>::PairingHeap(const PairingHeap& other)
{
    copyFrom(other);
}

template<class T>
PairingHeap<T>& PairingHeap<T>::operator=(const PairingHeap& other)
{
    if (this != &other)
    {
        clear();
        copyFrom(other);
    }
    return *this;
}

template<class T>
PairingHeap<T>::~PairingHeap()
{
    clear();
}

template<class T>
void PairingHeap<T>::merge(PairingHeap<T>& other)
{
    if (this == &other || other.empty())
        return;
    if (empty())
    {
        swap(*this, other);
        return;
    }
    if (other.root->value < root->value)
        swap(*this, other);
    other.root->rightSibling = root->leftChild;
    if (root->leftChild)                            //!
        root->leftChild->predecessor = other.root;  //!
    root->leftChild = other.root;
    root->leftChild->predecessor = root;            //!
    count += other.count;
    other.root = nullptr;
    other.count = 0;
}

template<class T>
auto PairingHeap<T>::insert(const T& _val) -> proxy
{
    // простичко
    Node* res = new Node(_val);
    merge(PairingHeap(res, 1));
    return proxy(res);
}

template<class T>
const T& PairingHeap<T>::peek() const
{
    return root->value;
}

template<class T>
T PairingHeap<T>::extractMin()
{
    // преди да освободим паметта за корена, взимаме неговите value и leftChild
    T result = peek();
    Node* nextChild = root->leftChild;
    delete root;
    root = nullptr;
    // за да не го изгубим по-нататък
    size_t oldCount = count;
    // масивът от децата на стария корен - също отделни пирамиди
    std::vector<PairingHeap<T>> children;
    // ако има деца, всички ги изолираме и добавяме във вектора
    while (nextChild)
    {
        Node* curr = nextChild;
        nextChild = nextChild->rightSibling;
        curr->rightSibling = curr->predecessor = nullptr;
        children.push_back(PairingHeap(curr));
    }
    // отново, ако има деца, всички ги сливаме към първото в масива
    // и после бързо разменяме *this с получената пирамида.
    if (!children.empty())
    {
        for (size_t i = 1; i < children.size(); i++)
            children[0].merge(children[i]);
        swap(*this, children[0]);
    }
    // намаляваме бройката (тя се губи при последния swap) и връщаме резултата
    count = oldCount - 1;
    return result;
}

template<class T>
auto PairingHeap<T>::decreaseKey(proxy it, const T& newKey) -> proxy
{
    // в случай на невалиден вход
    if (*it < newKey)
        return it;
    // актуализирай стойността
    Node* location = it.ptr;
    location->value = newKey;
    // ако е корен (<=> няма ляв брат), не променяй нищо повече
    if (root == location)
        return it;
    // съобщи на левия си брат, че има нов десен брат/наследник
    if (location == location->predecessor->rightSibling)
        location->predecessor->rightSibling = location->rightSibling;
    else
        location->predecessor->leftChild = location->rightSibling;
    // съобщи и на десния си брат (ако има такъв), че вече не си му ти отляво
    if (location->rightSibling)
        location->rightSibling->predecessor = location->predecessor;
    // изолирай върха да бъде корен на собствеа пирамида
    location->rightSibling = location->predecessor = nullptr;
    // построй пирамида по изолирания връх
    PairingHeap tmp(location);
    // запази стария размер на пирамидата
    size_t oldCount = count;
    // слей новата пирамида с текущата
    merge(tmp);
    // възстанови размера
    count = oldCount;
    // върни същия итератор (добри практики)
    return it;
}

template<class T>
size_t PairingHeap<T>::size() const
{
    return count;
}

template<class T>
bool PairingHeap<T>::empty() const
{
    return (root == nullptr);
}

template<class T>
void PairingHeap<T>::clear()
{
    freeNode(root);
    root = nullptr;
    count = 0;
}
