#include"Heap.h"
#include<iostream>
#include<functional>

// сравнение по последна цифра
bool compareLastDigit(int a, int b)
{
	return (a % 10) < (b % 10);
}

// примерно извикване:
Heap<int, bool(*)(int,int)> hp1(compareLastDigit);
// за ентусиатите:
Heap<int, std::function<bool(int, int)>> hp2(compareLastDigit);
// ламбда изрази. Да, правилно прочетохте - анонимна ламбда функция :D
Heap<int, std::function<bool(int, int)>> hp3([](int a, int b) { return a < b; });

void heapSort(std::vector<int>& ints)
{
	// в случая сортираме в намаляващ ред
	Heap<int, std::greater<int>> hp(ints);
	ints.clear();
	while (!hp.empty())
		ints.push_back(hp.extractMin());
}

int someEvilFunction()
{
	throw 42;
}

int main()
{
	std::vector<int> ints = { 18,26,32,4,35,83,77,101 };
	heapSort(ints);

	for (int x : ints)
		std::cout << x << " ";
	std::cout << "\n";

	try
	{
		// do stuff here
		someEvilFunction();
		// това няма да се изпълни, ако someEvilFuntion хвърли
	}
	catch (int x)
	{
		std::cout << "caught int x=" << x << "\n";
		// оправяме се по някакъв начин
		//throw; // така хвърляме наново същото изключение, което сме хванали
		// то ще напусне този catch блок и няма да се хване в долните
		// те "покриват" само try блока по-горе (!)
	}
	catch (double d)
	{
		std::cout << "caught double d=" << d << "\n";
		// оправяме се по някакъв друг начин - можем да "различаваме"
		// какви са ни проблемите по типа на хвърленото изключение
		// примерно неуспешна алокация на памет; непозволен достъп до паметта
		// и т.н.
	}
	catch (...)
	{
		std::cout << "caught something, don't know what...\n";
		// some stuff here
	}

	std::cout << "all ok here\n";
}
