#pragma once
#include<iostream>
#include<cstddef> // за size_t
#include<functional> // за std::less, std::greater и т.н.

template<class T, class Comp = std::less<int>>
class BST
{
    class Node // вложеният клас има пълен достъп до private частите на външния..
    {
        friend class BST<T, Comp>; // ...но обратното не е вярно!
        T value;
        Node* parent;
        Node* left;
        Node* right;
        Node(const T& _val, Node* _parent) : value(_val), parent(_parent),
            left(nullptr), right(nullptr) {}
    };

    Node* root;
    size_t count;
    Comp cmp;

    // помощни рекурсивни функции - по-рекурсивна структура от дървото здраве му кажи
    static void freeNode(const Node*);
    void insertNode(const T&, Node*&, Node*);
    const Node* findNode(const Node*, const T&) const;
    bool validateNode(Node*) const;
    static void printNode(std::ostream&, const Node*);
    void updateParent(const Node*, Node*);

    static void rotateLeft(Node*&);
    static void rotateRight(Node*&);
public:
    BST(const Comp& = Comp());
    //BST(const BST&); // to-do
    //BST& operator=(const BST&); // to-do
    ~BST();

    void insert(const T&);
    void remove(const T&);
    bool find(const T&) const;

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


/*
x < y  <=> operator<(x,y) <=> cmp(x,y) и заместваме навсякъде
x > y  <=>   y<x  <=>  cmp(y,x)
x <= y <=> !(x>y) <=> !cmp(y,x)
x >= y <=> !(x<y) <=> !cmp(x,y)
x == y <=> !(x<y) && !(y<x) <=> !cmp(x,y) && !cmp(y,x)
x != y <=>  (x<y) || (y<x)  <=>  cmp(x,y) || cmp(y,x) - ДеМорган хаха */

template<class T, class Comp>
void BST<T, Comp>::freeNode(const Node* ptr)
{
    if (ptr)
    {
        freeNode(ptr->left);
        freeNode(ptr->right);
        delete ptr;
    }
}

template<class T, class Comp>
void BST<T, Comp>::insertNode(const T& _val, Node*& location, Node* parent)
{
    // взимаме location с референция, т.к. искаме да напишем "x=f(x);" и го заменяме
    // само с извикването на "f(x);", което вътре променя този x - в случая указателя

    // във всеки един момент parent е родителя на location
    if (!location)
        location = new Node(_val, parent);
    else if (cmp(_val, location->value))
    {
        // вмъкваме вляво и "актуализираме" цялото ляво поддърво
        insertNode(_val, location->left, location);
        // избираме да завъртим надясно, в случай че лявото поддърво е "натежало"
        // дори не сме сигурни, че това е хубава стратегия - но можем да го тестваме
        rotateRight(location->left);
    }
    else
    {
        // вмъкваме вдясно и "актуализираме" цялото дясно поддърво
        insertNode(_val, location->right, location);
        // когато вмъкваме в дясното поддърво, после го завъртаме наляво
        rotateLeft(location->right);
    }
    // така "актуализираме" цялото дърво с корен location
}

template<class T, class Comp> // trailing return type - синтактична захар
auto BST<T, Comp>::findNode(const Node* location, const T& _val) const -> const Node*
{
    // чист, рекурсивен, прост вариант...
    /*if (!location || (!cmp(location->value, _val)) && !cmp(_val, location->value)))
        return location;
    if (cmp(_val, location->value))
        return findNode(location->left, _val);
    else
        return findNode(location->right, _val);*/
    // ...и итеративен вариант
    while (location && (cmp(_val, location->value) || cmp(location->value, _val)))
    {
        if (cmp(_val, location->value))
            location = location->left;
        else
            location = location->right;
    }
    return location;
}

template<class T, class Comp>
bool BST<T, Comp>::validateNode(Node* location) const
{
    // нулевият указател очевидно е указател към валиден Node
    if (!location)
        return true;
    if (location->left)
    {
        // ако има ляво дете, проверяваме стойностите и връзката дете-родител
        if (!(!cmp(location->value, location->left->value) &&
             (location->left->parent == location)))
            return false;
        // след това проверяваме и цялото ляво поддърво
        if (!validateNode(location->left))
            return false;
    }
    if (location->right)
    {
        // аналогично, ако има дясно дете, проверяваме стойностите и връзката дете-родител
        if (!(!cmp(location->right->value, location->value) &&
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
        ((!location->left) || (cmp(location->left->value, location->value)) &&
                              (location->left->parent == location) &&
                               validateNode(location->left)) &&
        ((!location->right) || (!cmp(location->right->value, location->value) &&
                               (location->right->parent == location) &&
                                validateNode(location->right));
    */
}

template<class T, class Comp>
void BST<T, Comp>::printNode(std::ostream& os, const Node* ptr)
{
    if (!ptr)
        return;
    // отпечатване ляво-корен-дясно, тоест в ненамаляващ ред
    printNode(os, ptr->left);
    os << ptr->value << " ";
    printNode(os, ptr->right);
}

template<class T, class Comp>
void BST<T, Comp>::updateParent(const Node* location, Node* newChild)
{
    // помощна функция, която взима възела location - такъв, който има само 1 дете
    // и казва на родителя му (ако има такъв), че вече има ново дете (newChild).
    if (location->parent)
    {
        // Трябва да проверим дали сме ляво или дясно дете на родителя си
        if (location == location->parent->left)
            location->parent->left = newChild;
        else
            location->parent->right = newChild;
    }
    else
        root = newChild; // !!!
    // грижим се за двупосочната връзка - новото дете вече си има нов родител!
    if (newChild)
        newChild->parent = location->parent;
}

template<class T, class Comp>
void BST<T, Comp>::rotateLeft(Node*& ptr)
{
    // ако дървото е празно, или няма дясно поддърво - няма как да завъртим наляво
    if (!ptr || !ptr->right)
        return;
    Node* tmp = ptr->right;
    ptr->right = tmp->left;
    if (ptr->right)
        ptr->right->parent = ptr;
    tmp->parent = ptr->parent;
    tmp->left = ptr;
    ptr->parent = tmp;
    ptr = tmp;
}

template<class T, class Comp>
void BST<T, Comp>::rotateRight(Node*& ptr)
{
    // ако дървото е празно, или няма ляво поддърво - няма как да завъртим надясно
    if (!ptr || !ptr->left)
        return;
    Node* tmp = ptr->left;
    ptr->left = tmp->right;
    if (ptr->left)
        ptr->left->parent = ptr;
    tmp->parent = ptr->parent;
    tmp->right = ptr;
    ptr->parent = tmp;
    ptr = tmp;
}

template<class T, class Comp>
BST<T, Comp>::BST(const Comp& _cmp) : root(nullptr), count(0), cmp(_cmp) {} // стандартни

template<class T, class Comp>
BST<T, Comp>::~BST()
{
    // също стандартно
    freeNode(root);
    // после се извиква деструктора на член-данната cmp и всичко е наред
}

template<class T, class Comp>
void BST<T, Comp>::insert(const T& _val)
{
    // цялата магия се случва в рекурсивната insertNode и това как се извиква!
    insertNode(_val, root, nullptr);
    ++count; // това го бяхме забравили !!!
}

template<class T, class Comp>
void BST<T, Comp>::remove(const T& _val)
{
    const Node* location = findNode(root, _val);
    if (!location)
        return;
    // След като намерим търсената стойност (location->value),
    // имаме три случая - възелът има 0, 1 или 2 наследника.
    // И в трите случая намаляваме броя стойности в дървото.
    --count;
    if (!location->left && !location->right)
    {
        // Ако възелът няма наследници, значи е листо и го "отрязваме", 
        // като единствено известяваме родителя си че вече няма дете.
        updateParent(location, nullptr);
    }
    else if (!location->left || !location->right)
    {
        // Ако има само един наследник, то родителят ни го "осиновява".
        // Проверяваме дали детето е ляво или дясно
        Node* onlyChild = ((location->left) ? location->left : location->right);
        updateParent(location, onlyChild);
    }
    else
    {
        // Намираме най-голямата стойност, по-малка от търсената.
        // Този възел "изваждаме" от дървото (аналогично на предишния случай)
        // и вмъкваме на мястото на премахваната стойност.
        Node* candidate = location->left;
        while (candidate->right)
            candidate = candidate->right;
        // знаем, че ако тази най-голяма по-малка стойност има дете, то може да е само ляво
        updateParent(candidate, candidate->left);

        // актуализираме указателите към и от децата на "кандидата"
        candidate->left = location->left;
        candidate->right = location->right;
        // (!) ако кандидатът е ляво дете на location и няма свое ляво дете,
        // то след updateParent location->left става nullptr!!!
        if(location->left)
            location->left->parent = candidate;
        // проверяваме само лявото дете, понеже търсим в лявото поддърво и дясното не се променя
        location->right->parent = candidate;

        // казваме на родителя на location, че вече candidate му е син
        // това актуализира и parent указателя на candidate
        updateParent(location, candidate);
    }
    // най-накрая освобождаваме паметта
    delete location;
}

template<class T, class Comp>
bool BST<T, Comp>::find(const T& _val) const
{
    return (findNode(root, _val) != nullptr);
    // return findNode(root, _val);
}

template<class T, class Comp>
size_t BST<T, Comp>::size() const
{
    return count;
}

template<class T, class Comp>
bool BST<T, Comp>::empty() const
{
    // няколко начина:
    return (root == nullptr);
    // return (size() == 0);
    // return (count == 0);
    // return !root;
    // return !size();
    // return !count;
}

template<class T, class Comp>
bool BST<T, Comp>::validate() const
{
    return validateNode(root);
}
