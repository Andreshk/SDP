#pragma once
#include<vector>
#include<cstddef>
#include<algorithm>

// приемаме "начина" за сравнение като втори темплейтен аргумент.
// Всеки обект Heap ще съдържа обект от този тип, който ще извиква
// като функция за сравнение - например пойнтър към функция или std::less.
// Задаваме стойност по подразбиране за този тип, за да пишем просто
// Heap<int> и да знаем, че ще се сравняват с оператор <.
template<class T, class Comparator = std::less<T>>
class Heap
{
	std::vector<T> data;
	Comparator cmp;

	static size_t getLeftChild(size_t);
	static size_t getRightChild(size_t);
	static size_t getParent(size_t);

	void bubbleUp(size_t);
	void bubbleDown(size_t);
public:
	// естествено, типът по подразбиране си има и стойност по подразбиране,
	// която е - познайте - конструирана с конструктура по подразбиране
	Heap(const std::vector<T>&, Comparator = Comparator());
	Heap(Comparator = Comparator());

	void push(const T&);
	T extractMin();
	const T& peek() const;

	bool empty() const;
	size_t size() const;
	bool validate() const;
};

template<class T, class Comparator>
size_t Heap<T, Comparator>::getLeftChild(size_t idx)
{
	return 2 * idx + 1;
}

template<class T, class Comparator>
size_t Heap<T, Comparator>::getRightChild(size_t idx)
{
	return 2 * idx + 2;
}

template<class T, class Comparator>
size_t Heap<T, Comparator>::getParent(size_t idx)
{
	return (idx - 1) / 2;
}

template<class T, class Comparator>
void Heap<T, Comparator>::bubbleUp(size_t idx)
{
	/* // рекурсивен вариант
	if (idx == 0)
		return;
	size_t parentIdx = getParent(idx);
	if (data[parentIdx] <= data[idx])
		return;
	std::swap(data[parentIdx], data[idx]);
	bubbleUp(parentIdx);*/
	// итеративен вариант
	while (idx != 0 && cmp(data[idx], data[getParent(idx)]))
	{
		std::swap(data[getParent(idx)], data[idx]);
		idx = getParent(idx);
	}
}

template<class T, class Comparator>
void Heap<T, Comparator>::bubbleDown(size_t idx)
{
	size_t left = getLeftChild(idx);
	size_t right = getRightChild(idx);
	if ((left >= data.size() || !cmp(data[left], data[idx])) &&
		(right >= data.size() || !cmp(data[right], data[idx])))
		return;

	if (right >= data.size() || cmp(data[left], data[idx]))
	{
		std::swap(data[idx], data[left]);
		bubbleDown(left);
	}
	else
	{
		std::swap(data[idx], data[right]);
		bubbleDown(right);
	}
}

template<class T, class Comparator>
Heap<T, Comparator>::Heap(const std::vector<T>& _data, Comparator _cmp)
	: cmp(_cmp)
{
	// за всеки обект val, взет по константна референция - аналогично на:
	/*
	for (size_t i = 0; i < _data.size(); ++i)
	{
		const T& val = _data[i]; // <- ето ТУК има значение типа (дали е T, T& или const T&),
								// т.е. какво точно ще напишем в range-based for-a
		push(val);
	}
	*/
	for (const T& val : _data)
		push(val);
}

template<class T, class Comparator>
Heap<T, Comparator>::Heap(Comparator _cmp) : cmp(_cmp) {}

template<class T, class Comparator>
const T& Heap<T, Comparator>::peek() const
{
	return data[0];
}

template<class T, class Comparator>
bool Heap<T, Comparator>::empty() const
{
	return data.empty();
}

template<class T, class Comparator>
size_t Heap<T, Comparator>::size() const
{
	return data.size();
}

template<class T, class Comparator>
void Heap<T, Comparator>::push(const T& val)
{
	data.push_back(val);
	bubbleUp(data.size() - 1);
}

template<class T, class Comparator>
T Heap<T, Comparator>::extractMin()
{
	T result = peek();
	std::swap(data[0], data[data.size() - 1]);
	data.pop_back();
	bubbleDown(0);
	return result;
}

template<class T, class Comparator>
bool Heap<T, Comparator>::validate() const
{
	for (size_t idx = 0; idx < data.size(); idx++)
	{
		size_t left = getLeftChild(idx);
		size_t right = getRightChild(idx);
		if ((left < data.size() && cmp(data[left],data[idx])) ||
			(right < data.size() && cmp(data[right],data[idx])))
			return false;
	}
	return true;
}
