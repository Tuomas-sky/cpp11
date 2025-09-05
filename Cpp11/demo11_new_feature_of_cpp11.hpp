#include<iostream>
#include<functional>
#include<future>
#include<thread>
#include<mutex>
#include<condition_variable>
using namespace std;

//可调用对象：函数，函数指针，lambda表达式，bind创建的对象、仿函数
//1、function 
//function遵循的规则：转换后的function对象的参数能够转换为可调用对象的参数；可调用对象的返回值能够转换为function对象的返回值
#if 1
//普通函数
void testFunc(int a, int b)
{
	cout << "testFunc() a=" << a << ",b=" << b << endl;
}
//Lambda表达式
auto lam = [](int a, int b) {
	cout << "Lambda a=" << a << ",b=" << b << endl;
	};
//仿函数
class Functor
{
	public:
	void operator()(int a, int b)
	{
		cout << "Functor a=" << a << ",b=" << b << endl;
	}
};

//类的成员函数
//类的静态成员函数
class TestClass {
	public:
	static void staticFunc(int a, int b)
	{
		cout << "staticFunc a=" << a << ",b=" << b << endl;
	}
	void memberFunc(int a, int b)
	{
		cout << "memberFunc a=" << a << ",b=" << b << endl;
	}
};
#endif

//2、bind
//bind:是一个函数模板，能绑定现有函数，生成新的函数，可以将可调用对象和参数一起绑定，绑定的结果使用function进行保存。
//bind的两大作用：
// ①将可调用对象与参数一起绑定为另一个function以供调用；
// ②将n元可调用对象转换为m元可调用对象(m<n)，绑定一部分参数，需要使用占位符std::placeholders::_1, _2等来表示
template<class T>
void TestBindFunc(T a, T b)
{
	cout << "TestBindFunc a=" << a << ",b=" << b << endl;
}

//3、Lambda表达式
//Lambda表达式的基本语法：[捕获列表](参数列表) -> 返回值类型 {函数体}
//闭包是一个函数内部的函数，可以读取他所关联函数的局部变量
void testLambda()
{
	int x = 10, y = 20;
	//[]不捕获任何变量
	auto f1 = []() {cout << "f1() hello world" << endl; };
	f1();
	//按值捕获外部变量
	auto f2 = [x, y]() {cout << "f2() x=" << x << ",y=" << y << endl; };
	f2();
	//按引用捕获外部变量
	auto f3 = [&x, &y]() {x += 10; y += 20; cout << "f3() x=" << x << ",y=" << y << endl; };
	f3();
	cout << "after f3() x=" << x << ",y=" << y << endl;
	//按值捕获外部变量，并且指定某个变量按引用捕获
	auto f4 = [x, &y]() mutable {y += 20; cout << "f4() x=" << x << ",y=" << y << endl; x += 10; };
	f4();
	cout << "after f4() x=" << x << ",y=" << y << endl;
	//按引用捕获外部变量，并且指定某个变量按值捕获
	auto f5 = [&x, y]() {x += 10; cout << "f5() x=" << x << ",y=" << y << endl; };
	f5();
	cout << "after f5() x=" << x << ",y=" << y << endl;
	//全部按值捕获
	auto f6 = [=]() {cout << "f6() x=" << x << ",y=" << y << endl; };
	f6();
	//全部按引用捕获
	auto f7 = [&]() {x += 10; y += 20; cout << "f7() x=" << x << ",y=" << y << endl; };
	f7();
	cout << "after f7() x=" << x << ",y=" << y << endl;
}


