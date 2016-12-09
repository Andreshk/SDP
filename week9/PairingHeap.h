#pragma once
#include <vector>
#include <cstddef>   // std::size_t
#include <algorithm> // std::swap

template<class T>
class PH
{
    struct Node
    {
        T value;
        Node* leftChild;
        Node* rightSibling;
        Node(const T& _val) : value(_val), leftChild(nullptr), rightSibling(nullptr) {}
    };

    Node* root;

    // Помощен конструктор, който конструира пирамида по нейния корен (получен напр. от някой наследник)
    // Използва се и за конструиране на пирамида с един възел (по дадена стойност)
    PH(Node*);

    void copyFrom(const PH&);           // използва се от копи-конструктора и оператор=
    static Node* copyNode(const Node*); // използва се от copyFrom и извиква себе си рекурсивно
    static void freeNode(const Node*);  // използва се от clear(), което се използва от деструктора
public:
    class proxy
    {
        friend class PH<T>;
        Node* ptr;

        proxy(Node* _p) : ptr(_p) {}
    public:
        // през това "прокси", което е нищо повече от "внимателен" указател,
        // ще достъпваме само стойността в съответния възел - и то без да я променяме!
        const T& operator*() const
        {
            return ptr->value;
        }
        const T* operator->() const
        {
            return &ptr->value;
        }
        friend bool operator==(const proxy& lhs, const proxy& rhs)
        {
            return lhs.ptr == rhs.ptr;
        }
        friend bool operator!=(const proxy& lhs, const proxy& rhs)
        {
            return !(lhs == rhs);
        }
    };

    // конструкторът по подразбиране е безинтересен
    PH();
    PH(const PH&);
    PH& operator=(const PH&);
    ~PH();

    // ако имаме пирамидите x и y, то x.merge(y) ВИНАГИ оставя y празна (!)
    // съобразете как да комбинирате този факт с проверката кой е по-малкият от двата корена
    void merge(PH&);

    // вмъкване - създаваме нова пирамида с една стойност и сливаме с дадената
    proxy insert(const T&);

    // класическа операция; нищо специално
    const T& peek() const;

    // тук става интересно (!)
    T extractMin();

    // разменяме съдържанието на две пирамиди
    friend void swap(PH<T>& p1, PH<T>& p2)
    {
        std::swap(p1.root, p2.root);
    }

    // стандартни
    bool empty() const;
    void clear();
};

template<class T>
void PH<T>::copyFrom(const PH& other)
{
    // преди това със сигурност root==nullptr, .т.е. пирамидата е празна
    root = copyNode(other.root);
}

template<class T>
auto PH<T>::copyNode(const Node* location) -> Node*
{
    if (!location)
        return nullptr;
    Node* result = new Node(location->value);
    result->leftChild = copyNode(location->leftChild);
    result->rightSibling = copyNode(location->rightSibling);
    return result;
}

template<class T>
void PH<T>::freeNode(const Node* location)
{
    if (!location)
        return;
    freeNode(location->leftChild);
    freeNode(location->rightSibling);
    delete location;
}

template<class T>
PH<T>::PH(Node* _ptr) : root(_ptr) {}

// голямата четворка се имплементира по "шаблонен" (pun intended) начин
template<class T>
PH<T>::PH() : root(nullptr) {}

template<class T>
PH<T>::PH(const PH& other) : PH()
{
    copyFrom(other);
}

template<class T>
PH<T>& PH<T>::operator=(const PH& other)
{
    if (this != &other)
    {
        clear();
        copyFrom(other);
    }
    return *this;
}

template<class T>
PH<T>::~PH()
{
    clear();
}

template<class T>
auto PH<T>::insert(const T& _val) -> proxy
{
    // създаваме нова кутийка и по нейния адрес конструираме
    // не само новата пирамидка, ами и проксито, което връщаме
    Node* location = new Node(_val);
    PH tmp(location);
    merge(tmp);
    return proxy(location);
}

template<class T>
const T& PH<T>::peek() const
{
    return root->value;
}

template<class T>
void PH<T>::merge(PH& other)
{
    if (this == &other || other.empty())
        return;
    if (empty())
    {
        swap(*this, other);
        return;
    }
    // с тази проверка си гарантираме, че винаги *this е с по-малък корен,
    // т.е. other ще бъде "погълната" в *this.
    if (other.root->value < root->value)
        swap(*this, other);
    other.root->rightSibling = root->leftChild;
    root->leftChild = other.root;
    other.root = nullptr;
}

template<class T>
T PH<T>::extractMin()
{
    // запази стойността в корена
    T result = peek();
    // запази всички деца на корена
    Node* nextChild = root->leftChild;
    std::vector<PH> children;
    while (nextChild)
    {
        Node* tmp = nextChild->rightSibling;
        // изолирай пирамидата с корен nextChild и я добави в масива
        nextChild->rightSibling = nullptr;
        children.push_back(PH(nextChild));
        // премини към следвщаото дете
        nextChild = tmp;
    }
    // освободи паметта за корена
    delete root;
    root = nullptr;
    // слей всичките деца - внимавайте дали въобще има такива !
    if (!children.empty())
    {
        // за да променим метода на сливане, трябва да променим само този цикъл
        for (size_t i = 1; i < children.size(); i++)
            children[0].merge(children[i]);
        // пренасочи корена към корена на тази получена пирамида
        swap(*this, children[0]);
    }
    // в този момент всички пирамиди в children са празни
    // при излизане от фунцкията ще им се извикат деструкторите - няма да загубим данни
    return result;
}

template<class T>
bool PH<T>::empty() const
{
    return (root == nullptr);
}

template<class T>
void PH<T>::clear()
{
    freeNode(root);
    root = nullptr;
}

// за да си спестим писане по-горе, но все пак "потребителите" да ползват пълното име
template<class T>
using PairingHeap = PH<T>;
