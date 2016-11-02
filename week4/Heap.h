#pragma once
#include<vector>

template<class T>
class Heap
{
	std::vector<T> data;

	static size_t getLeftChild(size_t);
	static size_t getRightChild(size_t);
	static size_t getParent(size_t);
public:
	Heap(const std::vector<T>&);
	Heap() = default;

	void push(const T&);
	T extractMin();
	const T& peek() const;
};

template<class T>
size_t Heap<T>::getLeftChild(size_t idx)
{
	return 2 * idx + 1;
}

template<class T>
size_t Heap<T>::getRightChild(size_t idx)
{
	return 2 * idx + 2;
}

template<class T>
size_t Heap<T>::getParent(size_t idx)
{
	return (idx - 1) / 2;
}

template<class T>
const T& Heap<T>::peek() const
{
	return data[0];
}
