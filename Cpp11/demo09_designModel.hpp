#pragma
#include <iostream>
#include<mutex>
#include<queue>
#include<thread>
using namespace std;

//1、单例模式-常用在任务队列
//饿汉模式（提前创建唯一实例对象，会占用一定的内存空间，但是访问实例对象时线程安全，但是访问内部数据的时候，多线程的情况下也会有线程安全问题）
//懒汉模式（在用到的时候才会创建对象，不会提前占用内存空间，但是会有线程安全的问题，可以使用双重检查锁定或静态局部对象的方式避免）
//1.1饿汉模式

class TaskQueue {
public:
    TaskQueue(const TaskQueue& t) = delete;
    TaskQueue& operator=(const TaskQueue& t) = delete;

    static TaskQueue* getInstance() {
        return m_taskQ;
    }

    void print() {
        cout << "饿汉式-单例对象的一个成员函数\n";
    }
private:
    TaskQueue() = default;
    static TaskQueue* m_taskQ;
};
TaskQueue* TaskQueue::m_taskQ = new TaskQueue;

//1.2懒汉模式
class TaskQueueLazy {
public:
    TaskQueueLazy(const TaskQueueLazy& t) = delete;
    TaskQueueLazy& operator=(const TaskQueueLazy& t) = delete;

    ////双重检查锁定
    //static TaskQueueLazy* getInstance() {
    //    if (m_taskQ == nullptr) {
    //        m_mutex.lock();
    //        if (m_taskQ == nullptr) {
    //            m_taskQ = new TaskQueueLazy();
    //        }
    //        m_mutex.unlock();
    //    }
    //    return m_taskQ;
    //}
    
    ////原子变量+双重检查锁定
    //static TaskQueueLazy* getInstance() {
    // //从原子变量取出数据
    //    TaskQueueLazy* queue = m_atomic_taskQ.load();
    //    if (queue == nullptr) {
    //        queue = m_atomic_taskQ.load();
    //        lock_guard<mutex> lock(m_mutex);
    //        if (queue == nullptr) {
    //            queue = new TaskQueueLazy();
    //            m_atomic_taskQ.store(queue);
    //        }
    //    }
    //    return queue;
    //}

    //静态局部变量
    static TaskQueueLazy* getInstance() {
        static TaskQueueLazy taskQ;
        return &taskQ;
    }


    void print() {
        cout<<"饿汉式-单例对象的一个成员函数\n";
    }
private:
    TaskQueueLazy() = default;
    static TaskQueueLazy* m_taskQ;

    static mutex m_mutex;
    //原子变量
    static atomic<TaskQueueLazy*> m_atomic_taskQ;
};
TaskQueueLazy* TaskQueueLazy::m_taskQ = nullptr;
mutex TaskQueueLazy::m_mutex;
atomic<TaskQueueLazy*> TaskQueueLazy::m_atomic_taskQ;

class TestSingleInstance {
public:
    TestSingleInstance(const TestSingleInstance& t) = delete;
    TestSingleInstance& operator=(const TestSingleInstance* t) = delete;
    static TestSingleInstance* getInstance() {
        return &obj;
    }

    bool isEmpty() {
        lock_guard<mutex> lock(m_mtx);
        if (!task.empty()) {
            return true;
        }
        return false;
    }

    bool popData() {
        lock_guard<mutex> lock(m_mtx);
        if (!task.empty()) {
            task.pop();
            return true;
        }
        return false;
    }
    void pushData(int data) {
        lock_guard<mutex> lock(m_mtx);
        task.push(data);
    }
    int getData() {
        lock_guard<mutex> lock(m_mtx);
        if (!task.empty()) {
           return task.front();
        }
        return -1;
    }

private:
    TestSingleInstance() = default;
    mutex m_mtx;
    queue<int> task;
    static TestSingleInstance obj;
};
TestSingleInstance TestSingleInstance::obj;