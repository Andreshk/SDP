#include<iostream>
#include<string>

struct Node
{
	int data;
	Node* left;
	Node* right;
	Node(int _data, Node* _l = nullptr, Node* _r = nullptr)
		: data(_data), left(_l), right(_r) {};
};

class Tree
{
	using position_t = const std::string&; // псевдоним на тип

	Node* root;
	size_t size;

	void freeNode(Node* ptr)
	{
		if (ptr == nullptr)
			return;
		freeNode(ptr->left);
		freeNode(ptr->right);
		delete ptr;
	}
	bool isValidPath(position_t) { return true; }
	Node** navigate(position_t); // само декларация
public:
	Tree() : root(nullptr), size(0) {}
	Tree(const Tree&) = delete; //to-do later
	Tree& operator=(const Tree&) = delete; // to-do later
	~Tree()
	{
		freeNode(root);
	}

	void insert(int x, position_t position)
	{
		Node** toInsert = navigate(position);
		// винаги *toInsert е нулевия указател, който трябва да стане ненулев
		if (toInsert != nullptr) // невалидна позиция, navigate е върнал nullptr
		{
			*toInsert = new Node(x);
			++size;
		}
	}

	void specialInsert(int x)
	{
		//insert(x, calculatePosition(/*???*/));
	}

	void remove(position_t position)
	{
		Node** toRemove = navigate(position);
		if (toRemove != nullptr)
		{
			freeNode(*toRemove);
			*toRemove = nullptr;
		}
	}

	friend std::ostream& operator<<(std::ostream&, const Tree&);
};

// дефиницията -> в .cpp файла, освен ако не е шаблонизирано дърво
Node** Tree::navigate(position_t position)
{
	// валидирай стринга + провери дали има такава позиция
	if (!isValidPath(position))
		return nullptr;
	Node** toRemove = &root;
	for (size_t i = 0; i < position.size(); i++)
	{
		if (position[i] == 'l') toRemove = &((*toRemove)->left);
		else toRemove = &((*toRemove)->right);
	}
	return toRemove;
}

void printNode(std::ostream& os, Node* ptr)
{
	if (ptr == nullptr)
		return;
	os << ptr->data << " -> ";
	if (ptr->left == nullptr) os << "#";
	else os << ptr->left->data;
	os << ", ";
	if (ptr->right == nullptr) os << "#";
	else os << ptr->right->data;
	os << "\n";
	printNode(os, ptr->left);
	printNode(os, ptr->right);
}

std::ostream& operator<<(std::ostream& os, const Tree& t)
{
	printNode(os, t.root);
	return os;
}

int main()
{
	Tree t;
	t.insert(1, "");
	t.insert(2, "l");
	t.insert(3, "r");
	t.insert(5, "ll");
	std::cout << t;
}
