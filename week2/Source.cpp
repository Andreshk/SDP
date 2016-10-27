#include <iostream>

class Point
{
	double x, y;
public:
	Point(double _x, double _y) : x(_x), y(_y) {}
	double getX() const { return x; }
	static Point getZero() // статичен метод на клас
	{
		return Point(0, 0);
	}
	void doStuff() const
	{
		Point zero = getZero();
		// Point zero = Point::getZero();
		std::cout << "stuff\n";
	}
};

class A
{
	static int arr[]; // декларация -> .h файл
public:
	static const int something = 0; // декларация+дефиниция, понеже е const
	int y;  // обикновени член-данни
	int& operator[](size_t idx) { return arr[idx]; }
};

int A::arr[] = { 1,2,3,4 }; // дефиниция/инициализация -> .cpp файла

class Singleton
{
	// задължително private, за да не създаваме обекти
	Singleton(int x, int y) : a(x), b(y) {}; // поне един конструктор

	int a, b; // може да съдържа каквото си иска, както обикновен клас
public:
	static Singleton s; //(!)
	Singleton(const Singleton&) = delete; // делете-нати, public
	Singleton& operator=(const Singleton&) = delete;

	static int getA() // препоръчително методите му да са статични
	{
		return s.a;
	}
	static void setA(int _a) // но не е задължително
	{
		s.a = _a;
	}
};

//тип	  //принадлежност
Singleton Singleton::s(1, 2); // дефиниция/инициализация

int main()
{
	//std::cout << "sizeof(A)=" << sizeof(A) << "\n";
	//A a1 = A();

	//std::cout << a1[0] << " " << "\n";
	//std::cout << A::something << " " << "\n";

	Singleton::setA(10);
	//std::cout << Singleton::getA() << "\n";
}

 iei
