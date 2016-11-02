#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

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
	std::string calculatePosition() const
	{
		size_t idx = size + 1;
		std::string result;
		for (size_t idx = size + 1; idx != 1; idx >>= 1)
		{
			result.push_back((idx & 1) ? 'r' : 'l');
		}
		std::reverse(result.begin(), result.end());
		return result;
	}

	// добавяне на елемент в "попълнено" двоично дърво
	// да не се използва в комбинация с обикновения insert!
	void specialInsert(int x)
	{
		insert(x, calculatePosition());
	}

	static void _level(const Node* ptr, size_t lvl, std::vector<int>& result)
	{
		if (ptr == nullptr) return;
		if (lvl == 0)
		{
			result.push_back(ptr->data);
			return;
		}
		_level(ptr->left, lvl - 1, result);
		_level(ptr->right, lvl - 1, result);
	}

	// не е задължително това да е const - а трябва !
	static size_t _height(const Node* ptr)
	{
		if (ptr == nullptr)
			return 0;
		return 1 + std::max(_height(ptr->left), _height(ptr->right));
	}

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

	void remove(position_t position)
	{
		Node** toRemove = navigate(position);
		if (toRemove != nullptr)
		{
			freeNode(*toRemove);
			*toRemove = nullptr;
		}
	}

	std::vector<int> getLevel(size_t lvl) const
	{
		std::vector<int> result;
		_level(root, lvl, result);
		return result;
	}

	size_t height() const
	{
		return _height(root);
	}

	size_t getMaxLevel() const
	{
		if (size == 0)
			return 0;
		return height() - 1;
	}

	friend std::ostream& operator<<(std::ostream&, const Tree&);
};

// дефиницията -> в .cpp файла, освен ако не е шаблонизирано дърво
Node** Tree::navigate(position_t position)
{
	// валидирай стринга + провери дали има такава позиция
	// при използване на specialInsert валидацията не е нужна
	if (!isValidPath(position))
		return nullptr;
	Node** result = &root;
	for (size_t i = 0; i < position.size(); i++)
	{
		if (position[i] == 'l') result = &((*result)->left);
		else result = &((*result)->right);
	}
	return result;
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
	t.insert(0, "");
	t.insert(1, "l");
	t.insert(3, "r");
	t.insert(4, "ll");
	t.insert(8, "lr");
	t.insert(12, "rr");
	t.insert(10, "lrl");

	std::cout << "Height: " << t.height() << "\n";
	std::vector<int> ints = t.getLevel(2);
	for (int x : ints) // range-based for
		std::cout << x << " ";
	std::cout << "\n";


	//std::cout << t;

	int arr[] = { 1,2,3,4 };
	for (int& x : arr)
	{
		x *= 2;
	}

	for (int x : arr) std::cout << x << " ";
}
