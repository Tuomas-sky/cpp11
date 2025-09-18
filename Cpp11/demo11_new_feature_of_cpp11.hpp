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

//4.4 智能指针性能与安全
//以shared_ptr为例
//①大小是原始指针的两倍（内部有一个原始指针指向资源，还有一个指针指向引用计数）
//②增加和减少引用计数需要原子操作，性能开销较大

//5、左值引用和右值引用
//右值引用是C++11新引入的引用类型，主要用于实现移动语义（move()）和完美转发（forward()）
//左值引用：T&，可以引用一个左值,不能引用一个右值,常量左值引用：const T&，可以引用一个左值，也可以引用一个右值
//右值引用：T&&，使用到move()的时候，move()本身只做类型转换，对性能无影响， 
// 5.1 move() 移动语义：本身只做类型转换，对性能无影响；move 只能转出来右值，forward 都可以:
// T&& var1 = std::move(var); 只能转出来右值
// 
// 5.2 forward() 完美转发：将函数参数原封不动的传递给另一个函数，保持参数的左值或右值属性；不会转发，也是只会做类型转换；
//std::forward<T>(u) 有两个参数：T 与 u。
//当 T 为左值引用类型时，u 将被转换为 T 类型的左值；否则 u 将被转换为 T 类型右值。
//万能引用：T&&，可以绑定到任何类型的值（左值或右值），通常用于模板中实现完美转发（forward）

//5、静态库.lib 和 动态库.dll
//5.1 静态库介绍：
//静态库是将目标文件打包成一个文件，链接到可执行文件中，运行时不需要依赖外部库文件
//优点：不依赖外部库文件，部署简单；
//缺点：可执行文件体积大；更新库文件需要重新编译可执行文件
//5.2 动态库介绍：
//动态库是将目标文件打包成一个文件，运行时通过动态链接加载到内存中
//优点：可执行文件体积小；更新库文件不需要重新编译可执行文件；多个程序可以共享同一个动态库，节省内存
//缺点：运行时需要依赖外部库文件，部署复杂；动态链接加载有一定的性能开销
//静态库与动态库如何使用代码创建，给出示例代码：
//实例见CreateDll项目

//6.*_cast,
// 格式均为：xx_cast<type-id>(expression) ，其中type-id-要转换成什么类型，expression-被转换类型的目标变量
//6.1 static_cast
//static_cast主要用于非多态类型之间的转换，编译时进行类型检查，不能用于去除const属性
//使用场景：
//①基本数据类型之间的转换，如int转float，float转double等
//②在有类型指针与void*之间转换
//③在有继承关系的类指针或引用之间转换（向上转换和向下转换）
//上行转换（派生类->基类），是安全的
//下行转换（基类->派生类），不安全，可能会导致运行时错误
//隐式转换建议使用static_cast进行显式转换
class CBase {};
class CDerived : public CBase {};
//6.2 dynamic_cast
//使用场景：用于将基类指针或引用转换为派生类指针或引用，必须是多态类型（至少有一个虚函数）（下行转换，是安全的）
//对指针进行dynamic_cast，如果转换失败，返回nullptr,成功返回正常cast后的对象指针；对引用进行dynamic_cast，如果转换失败，抛出bad_cast异常，成功返回正常cast后的对象引用
class DynamicBase {
	//dynamic_cast再将父类cast为子类时，父类必须有虚函数
	virtual int test() { return 0; }
};

class DynamicDerived : public DynamicBase {
	public:
		int test() override { return 1; }
};
//6.3 const_cast
//使用场景：常量指针（引用）与非常量指针（引用）之间的转换；去除对象的const属性，该行为比较危险；
//6.4 reinterpret_cast
//主要用于不同类型的指针之间的转换，或者指针与整数类型之间的转换，
//reinterpret_cast不会改变指针的值，只是改变指针的类型，通常用于底层编程，如操作系统开发，驱动开发等






