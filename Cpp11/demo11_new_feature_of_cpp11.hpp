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

//4、智能指针
// 4.1 shared_ptr<>:
// 多个shared_ptr可以指向同一个对象，使用引用计数的方式来管理资源，当最后一个shared_ptr被销毁时，资源才会被释放
//最安全的分配和使用动态内存的方法是使用make_shared函数，该函数会在动态内存中分配对象，并返回一个shared_ptr指向该对象
//可以使用new返回的指针来初始化shared_ptr，由于智能指针的构造函数是explicit类型的，因此必须显示的进行转换，不能隐式转换
//shared_ptr<int> p0 = new int(10); //错误
// shared_ptr<int> p1(new int(10)); //正确
//使用案例
class Test_SharedPtr
{
public:
	Test_SharedPtr() {
		cout << "Test_SharedPtr()" << endl;
	}
	~Test_SharedPtr() {
		cout << "~Test_SharedPtr()" << endl;
	}
};

// 4.2 weak_ptr<>:
//shared_ptr最大的问题是会引起循环引用，会导致堆内存无法释放，导致内存泄漏。
//循环引用案例
class Parent;
class Child {
	public:
	Child() {
		cout << "Child()" << endl;
	}
	~Child() {
		cout << "~Child()" << endl;
	}
	std::shared_ptr<Parent> parent;
};
class Parent {
	public:
	Parent() {
		cout << "Parent()" << endl;
	}
	~Parent() {
		cout << "~Parent()" << endl;
	}
	std::shared_ptr<Child> child;
};
//weak_ptr如何解决循环引用问题？
//weak_ptr是一个弱引用的智能指针，只有观测权，不控制所指对象的生命周期，不会影响引用计数，不能直接访问所指对象，
// 必须通过lock()函数转换为shared_ptr才能访问
//weak_ptr的创建：
//①通过shared_ptr创建 : auto p = make_shared<int>(100); weak_ptr<int> wp(p);
//②通过weak_ptr创建: weak_ptr<int> wp1(wp);
//③通过赋值操作创建 : weak_ptr<int> wp2 = wp1;
class Parent_Weak;
class Child_Weak {
public:
	Child_Weak() {
		cout << "Child_Weak()" << endl;
	}
	~Child_Weak() {
		cout << "~Child_Weak()" << endl;
	}
	void testWork() {
		cout << "testWork()" << endl;
	}

	std::weak_ptr<Parent_Weak> parent_;
};
class Parent_Weak {
	public:
	Parent_Weak() {
		cout << "Parent_Weak()" << endl;
	}
	~Parent_Weak() {
		cout << "~Parent_Weak()" << endl;
	}
	std::weak_ptr<Child_Weak> child_;
};

//4.3 unique_ptr
//独占式智能指针，不允许其他的智能指针共享其内部的指针，更像原生指针，不可以拷贝和赋值，可以移动
//虽然不能拷贝或赋值，但是可通过调用release 或 reset将指针的所有权从一个（非const）unique_ptr转移给另一个unique_ptr

//传递unique_ptr参数和返回unique_ptr
//不能拷贝 unique_ptr 的规则有一个例外：我们可以拷贝或赋值一个将要被销毁的 unique_ptr。最常见的例子是从函数返回一个unique_ptr：
unique_ptr<string> CreateString()
{
	unique_ptr<string> p(new string("hello unique_ptr"));
	return p; //返回时会调用移动构造函数
}


