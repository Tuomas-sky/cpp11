#pragma once
#include <iostream>
#include <map>
#include <cassert>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

//别名
using uint = unsigned int;
using fptr1 = void(*)(int, int);
typedef unsigned int uint32;
typedef void(*fptr2)(int, int);
//别名模板
template<class T>
using int_map1 = map<int, T>;

template<class T>
struct int_map2 {
    typedef map<int, T> type;
};


class X {
public:
    X() { count++; cout << "ctor:" << count << endl; }
    X(const X& x) { count++;cout << "copy ctor:" << endl; }
    ~X() { cout << "~ctor" << endl; }
    void show() { cout << "count=" << count << endl; }
private:
    static int count;
};
X make_x() {
    X x1;
    return x1;
}
int X::count = 0;
class X1 {
    ~X1() {}
};
class X2 {};
#define PRINT_NOEXCEPT(x)  std::cout << #x << "=" << x << std::endl;

//cpp17结构化绑定
auto return_value() {
    return make_tuple(1, 2.3, 'a');
}


////////////////////////////////////////
class AA {
public:
    string name;
    AA() { cout << "AA()" << endl; }
    AA(string n) :name(n) { cout << "AA(string)" << endl; }
    ~AA() { cout << "~AA()" << endl; }

};

void deletefunc(AA* a) {
    cout << "全局函数deletefunc" << endl;
    delete a;
}

auto lambdafunc = [](AA* a) {
    cout << "Lambdafunc" << endl;
    delete a;
    };

struct XX {
    static int a;
};
int XX::a = 1;
struct YY {
    inline static int b = 10;
};
//自定义字面量
long double operator "" _mm(long double x) { return x / 10; };

//template<typename ...Args>
//void foo(Args... args){}
//
//template<typename ...Args>
//class T {
//public:
//    void foo2(Args... args) {
//        foo(args...);
//    }
//};

template<class T, class U>
T foo1(T t, U u) {
    cout << t << ":" << u << endl;
    return t;
}
template<class ...Args>
void foo2(Args...args) {}
template<class ...Args>
class bar {
public:
    void foo3(Args ...args) {
        foo2(foo1(&args, args)...);
        cout << "sizeof...(args)=" << sizeof...(args) << endl;

    }
};


void thread_1()
{
    while (1)
    {
        cout << "子线程1111" << endl;
    }
}

void thread_2(int x)
{
    while (1)
    {
        cout << "子线程2222" << endl;
    }
}


std::mutex mtx_1; // mutex for critical section
std::mutex mtx_2;  // mutex for critical section
int test_num = 1;

void print_block_1(int n, char c)
{
    // critical section (exclusive access to std::cout signaled by locking mtx):
    mtx_1.lock();
    for (int i = 0; i < n; ++i)
    {
        //std::cout << c;
        test_num = 1;
        std::cout << test_num << std::endl;
    }
    std::cout << '\n';
    mtx_1.unlock();
}

void print_block_2(int n, char c)
{// critical section (exclusive access to std::cout signaled by locking mtx):
    mtx_2.lock();
    test_num = 2;
    for (int i = 0; i < n; ++i)
    {
        //std::cout << c;
        test_num = 2;
        std::cout << test_num << std::endl;
    }
    mtx_2.unlock();
}

struct Box {
    explicit Box(int num) : num_things{ num } {}
    int num_things;
    std::mutex m;
};
void transfer(Box& from, Box& to, int num)
{
    // defer_lock表示暂时unlock，默认自动加锁
    std::unique_lock<std::mutex> lock1(from.m, std::defer_lock);
    std::unique_lock<std::mutex> lock2(to.m, std::defer_lock);//两个同时加锁
    std::lock(lock1, lock2);//或者使用lock1.lock()

    from.num_things -= num;
    to.num_things += num;//作用域结束自动解锁,也可以使用lock1.unlock()手动解锁
}


std::mutex mtx;
std::condition_variable cv;
int cargo = 0;
bool shipment_available()
{
    return cargo != 0;
}
void consume(int n)
{
    for (int i = 0; i < n; ++i)
    {
        std::unique_lock<std::mutex> lck(mtx);//自动上锁
        //第二个参数为false才阻塞（wait），阻塞完即unlock，给其它线程资源
        cv.wait(lck, shipment_available);// consume:
        std::cout << cargo << '\n';
        cargo = 0;
    }
}


int value;
void read_value()
{
    std::cin >> value;
    std::cout << value << endl;
    cv.notify_one();
}