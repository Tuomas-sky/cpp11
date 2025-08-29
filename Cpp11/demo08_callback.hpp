#pragma
#include<iostream>
#include<functional>
#include <mutex>
#include<thread>
#include<Windows.h>

using namespace std;

//1.普通函数
void func_cb(int val) {
    cout << "普通函数回调，value=" << val << endl;
}
void call_func_cb(void(*callback)(int), int value) {
    callback(value);
}

//2.成员函数回调,传入对象指针和成员函数指针
class CallBackHandler {
public:
    void memberCallBack(int value) {
        cout << "类普通成员函数回调，value=" << value << endl;
    }
    static void staticMemberCallBack(int value) {
        cout << "类静态成员函数回调，value=" << value << endl;
    }
};
void call_CallBackHanlder(CallBackHandler* obj, void(CallBackHandler::* callback)(int), int val) {
    (obj->*callback)(val);
}
void call_StaticCallBackHanlder(void (*callback)(int), int val) {
    callback(val);
}

//3.基于functional和bind回调
//std::function 是现代 C++ 中推荐的工具，可以存储任意可调用对象（普通函数、成员函数、Lambda 表达式等），而 std::bind 可以绑定成员函数与对象。
void OrdinaryFunc(int value) {
    cout << "普通函数回调，value=" << value << endl;
}

void executeCallback(function<void(int)> callback, int val) {
    callback(val);
}

////4.Lambda回调
//struct MyClass {
//    void operator()(int value) {
//        cout << "仿函数作为回调传递，value=" << value << endl;
//    }
//};
////typedef void(*MyCallBack)(int);
//template<typename MyClass>
//void TestCallBack(MyClass callback) {
//    int val = 1;
//    callback(val);
//}

class A {
public:
    void CB(int val) {
        cout << "call CB,value=" << val << endl;
    }
};
void callCB(function<void(int)> callback, int val) {
    callback(val);
}
class MyClass {
public:
    MyClass() : value(0) {}
    void doAsyncTask(std::function<void(int)> callback) {
        std::lock_guard<std::mutex> lock(mtx); // 加锁
        callbackFunction = callback;
        // 模拟异步任务
        std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::lock_guard<std::mutex> lock(mtx); // 回调时再加锁
            if (callbackFunction) {
                callbackFunction(value);
            }
            }).detach();
    }
    void setValue(int v) {
        std::lock_guard<std::mutex> lock(mtx); // 加锁
        value = v;
    }
    ~MyClass() {
        std::lock_guard<std::mutex> lock(mtx); // 在析构函数中加锁，避免回调访问已销毁的对象
        callbackFunction = nullptr;
    }
private:
    int value;
    std::function<void(int)> callbackFunction;
    std::mutex mtx;
};

//异步调用
void async_make_pizza(int num, function<void()>callback) {
    thread([num, callback]() {
        for (int i = 0; i < num; i++)
        {
            Sleep(1000);
        }
        cout << "async_make_pizza finished\n";
        callback();
        }).detach();
}

