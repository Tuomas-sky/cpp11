#include<iostream>
#include<functional>
#include<future>
#include<thread>
#include<mutex>
#include<condition_variable>
using namespace std;

//�ɵ��ö��󣺺���������ָ�룬lambda���ʽ��bind�����Ķ��󡢷º���
//1��function 
//function��ѭ�Ĺ���ת�����function����Ĳ����ܹ�ת��Ϊ�ɵ��ö���Ĳ������ɵ��ö���ķ���ֵ�ܹ�ת��Ϊfunction����ķ���ֵ
#if 1
//��ͨ����
void testFunc(int a, int b)
{
	cout << "testFunc() a=" << a << ",b=" << b << endl;
}
//Lambda���ʽ
auto lam = [](int a, int b) {
	cout << "Lambda a=" << a << ",b=" << b << endl;
	};
//�º���
class Functor
{
	public:
	void operator()(int a, int b)
	{
		cout << "Functor a=" << a << ",b=" << b << endl;
	}
};

//��ĳ�Ա����
//��ľ�̬��Ա����
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

//2��bind
//bind:��һ������ģ�壬�ܰ����к����������µĺ��������Խ��ɵ��ö���Ͳ���һ��󶨣��󶨵Ľ��ʹ��function���б��档
//bind���������ã�
// �ٽ��ɵ��ö��������һ���Ϊ��һ��function�Թ����ã�
// �ڽ�nԪ�ɵ��ö���ת��ΪmԪ�ɵ��ö���(m<n)����һ���ֲ�������Ҫʹ��ռλ��std::placeholders::_1, _2������ʾ
template<class T>
void TestBindFunc(T a, T b)
{
	cout << "TestBindFunc a=" << a << ",b=" << b << endl;
}

//3��Lambda���ʽ
//Lambda���ʽ�Ļ����﷨��[�����б�](�����б�) -> ����ֵ���� {������}
//�հ���һ�������ڲ��ĺ��������Զ�ȡ�������������ľֲ�����
void testLambda()
{
	int x = 10, y = 20;
	//[]�������κα���
	auto f1 = []() {cout << "f1() hello world" << endl; };
	f1();
	//��ֵ�����ⲿ����
	auto f2 = [x, y]() {cout << "f2() x=" << x << ",y=" << y << endl; };
	f2();
	//�����ò����ⲿ����
	auto f3 = [&x, &y]() {x += 10; y += 20; cout << "f3() x=" << x << ",y=" << y << endl; };
	f3();
	cout << "after f3() x=" << x << ",y=" << y << endl;
	//��ֵ�����ⲿ����������ָ��ĳ�����������ò���
	auto f4 = [x, &y]() mutable {y += 20; cout << "f4() x=" << x << ",y=" << y << endl; x += 10; };
	f4();
	cout << "after f4() x=" << x << ",y=" << y << endl;
	//�����ò����ⲿ����������ָ��ĳ��������ֵ����
	auto f5 = [&x, y]() {x += 10; cout << "f5() x=" << x << ",y=" << y << endl; };
	f5();
	cout << "after f5() x=" << x << ",y=" << y << endl;
	//ȫ����ֵ����
	auto f6 = [=]() {cout << "f6() x=" << x << ",y=" << y << endl; };
	f6();
	//ȫ�������ò���
	auto f7 = [&]() {x += 10; y += 20; cout << "f7() x=" << x << ",y=" << y << endl; };
	f7();
	cout << "after f7() x=" << x << ",y=" << y << endl;
}

//4������ָ��
// 4.1 shared_ptr<>:
// ���shared_ptr����ָ��ͬһ������ʹ�����ü����ķ�ʽ��������Դ�������һ��shared_ptr������ʱ����Դ�Żᱻ�ͷ�
//�ȫ�ķ����ʹ�ö�̬�ڴ�ķ�����ʹ��make_shared�������ú������ڶ�̬�ڴ��з�����󣬲�����һ��shared_ptrָ��ö���
//����ʹ��new���ص�ָ������ʼ��shared_ptr����������ָ��Ĺ��캯����explicit���͵ģ���˱�����ʾ�Ľ���ת����������ʽת��
//shared_ptr<int> p0 = new int(10); //����
// shared_ptr<int> p1(new int(10)); //��ȷ
//ʹ�ð���
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
//shared_ptr���������ǻ�����ѭ�����ã��ᵼ�¶��ڴ��޷��ͷţ������ڴ�й©��
//ѭ�����ð���
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
//weak_ptr��ν��ѭ���������⣿
//weak_ptr��һ�������õ�����ָ�룬ֻ�й۲�Ȩ����������ָ������������ڣ�����Ӱ�����ü���������ֱ�ӷ�����ָ����
// ����ͨ��lock()����ת��Ϊshared_ptr���ܷ���
//weak_ptr�Ĵ�����
//��ͨ��shared_ptr���� : auto p = make_shared<int>(100); weak_ptr<int> wp(p);
//��ͨ��weak_ptr����: weak_ptr<int> wp1(wp);
//��ͨ����ֵ�������� : weak_ptr<int> wp2 = wp1;
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
//��ռʽ����ָ�룬����������������ָ�빲�����ڲ���ָ�룬����ԭ��ָ�룬�����Կ����͸�ֵ�������ƶ�
//��Ȼ���ܿ�����ֵ�����ǿ�ͨ������release �� reset��ָ�������Ȩ��һ������const��unique_ptrת�Ƹ���һ��unique_ptr

//����unique_ptr�����ͷ���unique_ptr
//���ܿ��� unique_ptr �Ĺ�����һ�����⣺���ǿ��Կ�����ֵһ����Ҫ�����ٵ� unique_ptr������������ǴӺ�������һ��unique_ptr��
unique_ptr<string> CreateString()
{
	unique_ptr<string> p(new string("hello unique_ptr"));
	return p; //����ʱ������ƶ����캯��
}


