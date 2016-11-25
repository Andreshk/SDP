#pragma once
template<class T>
class PairingHeap
{
    struct Node
    {
        T value;
        Node* leftChild;
        Node* rightSibling;
        Node(const T& _val) : value(_val), leftChild(nullptr), rightSibling(nullptr) {}
    };

    Node* root;

    // помощен конструктор, който конструира пирамидка с един възел
    PairingHeap(const T&);

public:
    // конструкторът по подразбиране е безинтересен
    PairingHeap() : root(nullptr) {}
    
    // ако имаме пирамидите x и y, то x.merge(y) ВИНАГИ оставя y празна (!)
    // съобразете как да комбинирате този факт с проверката кой е по-малкият от двата корена
    void merge(PairingHeap&);
    
    // вмъкване - създаваме нова пирамида с една стойност и сливаме с дадената
    void insert(const T&);

    // класическа операция; нищо специално
    const T& peek() const;

    // тук става интересно (!)
    T extractMin();
};
