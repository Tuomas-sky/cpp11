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


