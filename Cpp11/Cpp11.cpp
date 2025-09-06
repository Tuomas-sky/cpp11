#define _CRT_SECURE_NO_WARNINGS 
//#include "demo01_base.h"
//#include "demo02_read_write_file.h"
//#include"demo03.hpp"
//#include "demo04_stl.hpp"
//#include "demo05_thread.hpp"
//#include"demo07_NetProgram.hpp"
//#include "demo08_callback.hpp"
//#include "demo09_designModel.hpp"
//#include"demo10_log.hpp"
//using namespace utility;

#include "demo11_new_feature_of_cpp11.hpp"
using namespace std;
//
//union Ending {
//    uint32_t i;
//    uint8_t c[4];
//}test;
//#include <iostream>//引入标准输入输出流库
//#include <string>//引入字符串库
//#include<winsock2.h>// 引入Winsock2库，用于网络编程
//#include <ws2tcpip.h> // 引入Windows Sockets 2 TCP/IP协议的扩展库
//#pragma comment(lib,"ws2_32.lib")

int main()
{
  
	//new feature of cpp11
#if 1
	////1、function
	////普通函数
	//std::function<void(int, int)> func1 = testFunc;
	//func1(10, 20);
	////Lambda表达式
	//std::function<void(int, int)> func2 = lam;
	//func2(10, 20);
	////仿函数
	//std::function<void(int, int)> func3 = Functor();
	//func3(10, 20);
	////类的静态成员函数
	//std::function<void(int, int)>func4 = TestClass::staticFunc;
	//func4(10, 20);
	////类的成员函数
	//TestClass obj;
	//std::function<void(int, int)> func5 = std::bind(&TestClass::memberFunc, &obj, std::placeholders::_1, std::placeholders::_2);
	//func5(10, 20);

	////2、bind
	////绑定普通函数
	//std::function<void(int,int)> f1 = std::bind(&testFunc, placeholders::_1, placeholders::_2);
	//f1(100, 200);
	////绑定lambda表达式
	//auto lambda = std::bind(lam, placeholders::_1, placeholders::_2);
	//lambda(100, 200);
	////绑定仿函数
	//Functor functor;
	//auto f2 = std::bind(functor, placeholders::_1, placeholders::_2);
	//f2(100, 200);
	////绑定类的静态成员函数
	//TestClass tc;
 //   auto f3 = std::bind(&TestClass::staticFunc, 100,std::placeholders::_1);
	//f3(200);
	////绑定类的成员函数
 //   auto f4 = std::bind(&TestClass::memberFunc, &tc, std::placeholders::_1, std::placeholders::_2);
 //   f4(100,200);
	////绑定模板函数
	//auto f5 = std::bind(&TestBindFunc<string>, std::placeholders::_1, std::placeholders::_2);
	//f5("hello", " world");

	//3、Lambda表达式
	//testLambda();

	//4.1、智能指针 shred_ptr 
    shared_ptr<Test_SharedPtr> sp1 = make_shared<Test_SharedPtr>(); //make_shared<T>(args)
	cout << "1 sp1 use_count=" << sp1.use_count() << endl;
    {
        shared_ptr<Test_SharedPtr> sp2 = sp1;//引用计数+1
        cout << "2 sp1 use_count=" << sp1.use_count() << endl;
	}//sp2出作用域，引用计数-1
	cout << "3 sp1 use_count=" << sp1.use_count() << endl;
    cout << "-------------------------------------------\n";
	//4.2 、智能指针 weak_ptr
	////循环引用问题
	//shared_ptr<Parent> parent = make_shared<Parent>(); //parent.use_count=1
	//shared_ptr<Child> child = make_shared<Child>();  //child.use_count=1
	//parent->child = child;  //parent.use_count=1 ,child.use_count=2
	//child->parent = parent; //parent.use_count=2 ,child.use_count=2

	//解决循环引用问题
    shared_ptr<Parent_Weak> parent = make_shared<Parent_Weak>(); //parent.use_count=1
    shared_ptr<Child_Weak> child = make_shared<Child_Weak>();  //child.use_count=1
	parent->child_ = child;   //parent.use_count=1 ,child.use_count=1
	child->parent_ = parent;  //parent.use_count=1 ,child.use_count=1
	cout << "parent use_count=" << parent.use_count() << ",child use_count=" << child.use_count() << endl;

	//把weak_ptr转换为shared_ptr
    shared_ptr<Child_Weak>tmp = parent.get()->child_.lock();
    tmp->testWork();
	cout << "tmp_ref_count=" << tmp.use_count() << endl;  //tmp_ref_count=2

    cout << "-------------------------------------------\n";
	//4.3、智能指针 unique_ptr
    unique_ptr<int> up1(new int(10));
	cout << "*up1=" << *up1 << endl;
    //使用案例
    unique_ptr<string> p1(new string("test unique_ptr"));
    unique_ptr<string> p2 = move(p1); //将p1的所有权转移给p2，p1变为空
    //unique_ptr<string> p3 = p1; //错误，不能拷贝
    unique_ptr<string> p4(p2.release()); //将p2的所有权转移给p4，p2变为空
	cout << "*p4=" << *p4 << endl;

    //调用 release 会切断unique_ptr和它原来管理的对象间的联系，如果我们不用另一个智能指针来保存 release 返回的指针，
// 我们的程序就要负责资源的释放：
	unique_ptr<string> p5(new string("hello unique_ptr"));
   // p5.release(); // 错误：p2不会释放内存，而且我们丢失了指针
    auto p = p5.release(); // 正确，但我们必须记得 delete(p)
    delete(p);
    cout << "-------------------------------------------\n";



#endif 


    //Logger
#if 0
    Logger::getInstance()->Open("./test.log");
    
    //Logger::getInstance()->Log(Logger::DEBUG, __FILE__, __LINE__, "hello world");
    //Logger::getInstance()->Log(Logger::DEBUG, __FILE__, __LINE__, "name is %s,age is %d","tom",20);
    Logger::getInstance()->max_len(1024);
    Logger::getInstance()->level(Logger::DEBUG);
    debug("debug write log");
    info("info write log");
    warn("warn write log");
    error("error write log");
    fatal("fatal write log");

#endif 


#if 0   
    //TaskQueue* task = TaskQueue::getInstance();
    //task->print();
    ////双重检查锁定 vs 原子操作 vs 静态局部变量
    //TaskQueueLazy* lazy = TaskQueueLazy::getInstance();
    //lazy->print();

    TestSingleInstance* task = TestSingleInstance::getInstance();
    thread t1([=]() {
        for (int i = 0; i < 10; i++)
        {
            task->pushData(i + 1);
            cout << " +++push data: " << i + 1 << ", thread id=" << this_thread::get_id() << endl;
            this_thread::sleep_for(chrono::microseconds(500));
        }
        });
    thread t2([=]() {

        });


#endif

    //callback
#if 0      
    //callback
    // 普通函数
    //call_func_cb(func_cb, 100);
    //CallBackHandler obj;
    // 类成员函数
    //call_CallBackHanlder(&obj, &CallBackHandler::memberCallBack, 200);
    //call_StaticCallBackHanlder(CallBackHandler::staticMemberCallBack, 300);
    ////function与bind
    //executeCallback(OrdinaryFunc, 400);
    //executeCallback([](int val) {
    //    cout << "Lambda 作为回调函数,value=" << val << endl;
    //    },500);
    ////传递成员函数,需要传入绑定后的函数
    //CallBackHandler hanlder;
    //executeCallback(bind(&CallBackHandler::memberCallBack, &hanlder, placeholders::_1), 600);
    ////TestCallBack(MyClass());
    //A a;
    //callCB(bind(&A::CB, &a, placeholders::_1), 800);

    async_make_pizza(5, []() {
        cout << "finished make pizza!\n";
        });
    Sleep(7000);
    cout << "main process other task...\n";


#endif

    //net
#if 0
    ////大端存储vs小端存储
    //test.i = 0x12345678;
    //if (test.c[0] == 0x12) {
    //    cout << "大端存储\n";
    //}
    //else if (test.c[0] == 0x78) {
    //    cout << "小端存储\n";
    //}
    //cout << "--------------\n";
    //int x = 1;
    //char* cc = reinterpret_cast<char*>(&x);
    //if (*cc == 1) {
    //    cout << "小端存储\n";
    //}
    //else {
    //    cout << "大端存储\n";
    //}
 
    //服务端
    //scoket()->bind()->listent->accept()->read()->write()->read()->close()
    //客户端
    //scoket()->connect()->wirte()->read()->close()

    WSADATA wsaData;
    sockaddr_in server;
    SOCKET sock = INVALID_SOCKET;
    int result;
    //初始化Winsock库
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "wSAStartup失败，错误码：" << result << std::endl;
        return 1;//初始化失败，返回1
    }
    //创建一个流式套接字
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cout << "套接字创建失败，错误码：" << WSAGetLastError() << std::endl;
        WSACleanup();//清理winsock资源
        return 1;
    }
    //设置服务器的地址和端口
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    if (inet_pton(AF_INET, "10.119.123.25", &server.sin_addr) <= 0) {
        std::cout << "无效地址\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    //连到服务器
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cout << "连接失败\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    //发送消息到服务器
    std::string message = "Hello form client";
    send(sock, message.c_str(), message.size(), 0);
    //接受服务器的相应
    char buffer[1024] = { 0 };
    int byteRead = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (byteRead > 0) {
        std::cout << "服务器：" << buffer << std::endl;
    }
    else if (byteRead == 0) {
        std::cout << "连接被对方关闭" << std::endl;
    }
    else {
        std::cout << "接受失败\n";
    }
    //关闭套接字
    closesocket(sock);
    WSACleanup();
    return 0;

#endif

    //thread_v1
#if 0

   // //future
   //future<int> f= async(add_async, 10, 20);
   //cout << "f=" << f.get() << endl;

    ////promise
    //promise<int> prom;
    //future<int> fut = prom.get_future();
    //thread t(async_task, move(prom));
    //cout << "async_task result=" << fut.get() << endl;
    //t.join();

    ////packaged_task
    //packaged_task<int(int, int)> task([](int a, int b) {//创建任务
    //    return a + b;
    //    });
    //future<int> fut = task.get_future();//获取future
    //task(10, 20);//执行任务
    //cout << "packaged_task=" << fut.get() << endl;//获取结果

    ////thread pool
    //ThreadPool mypool(4);
    //for (int i = 0; i < 10; i++)
    //{
    //    auto res = mypool.enques([](int a, int b)->int {
    //        cout << "thread id = " << this_thread::get_id() << endl;
    //        return a + b;
    //        },10*i,10*i);
    //    cout << "thread res=" << res.get() << endl;
    //}



    //信号量
    //vector<thread> box;
    //for (int i = 0; i < 5; i++)
    //{
    //    box.emplace_back(semaphore_woker, i + 1);
    //}
    //for (thread& th : box) {
    //    th.join();
    //}

    //thread th1(prepare);
    //thread th2(work);
    //th1.join();
    //th2.join();

    ////栅栏
    ////死锁，下述代码产生死锁，因为task中获取锁的顺序的问题
    //thread t1(task1);
    //thread t2(task2);
    //t1.join();
    //t2.join();



#endif 

    //thread_v2
#if 0
    //5.1 一次调用
    //cout << "main thread begin,id=" << this_thread::get_id() << endl;
    //{
    //    thread th1(SubThread);
    //    this_thread::sleep_for(1s);
    //    is_exit = true;
    //    cout << "main阻塞，等待子线程" << endl;
    //    th1.join();
    //    cout << "子线程执行结束\n";
    //}
    //for (int i = 0; i < 3; i++)//callonce 只进入一次
    //{
    //    thread th(CallOnce);
    //    th.join();
    //}

    ////5.2 test 互斥锁+list实现模拟线程通信
    //MsgServer server;
    //server.Start();
    //for (int i = 0; i < 10; i++)
    //{
    //    stringstream ss;
    //    ss << "MSG: " << i + 1;
    //    server.SendMsg(ss.str());
    //    this_thread::sleep_for(500ms);
    //}
    //Sleep(1500);
    //server.Stop();

    //5.3
    //thread th1(ThreadWrite);
    //th1.detach();
    //for (int i = 0; i < 3; i++)
    //{
    //    thread th2(ThreadRead, i + 1);
    //    th2.detach();
    //}
    //getchar();

    ////5.4 使用条件变量 test 互斥锁+list实现模拟线程通信
    //    MsgServer_V2 server;
    //    server.Start();
    //    for (int i = 0; i < 3; i++)
    //    {
    //        stringstream ss;
    //        ss << "MSG " << i + 1;
    //        server.SendMsg(ss.str());
    //        this_thread::sleep_for(500ms);
    //    }
    //    Sleep(2000);
    //    server.Stop();
    //    cout << "Thread Stop\n";

    ////5.5 线程池
    //class MyTask :public XTask {
    //public:
    //    int Run() {
    //        std::cout << "==============================\n";
    //        cout <<this_thread::get_id()<< " MyTask " << name << endl;
    //        std::cout << "==============================\n";
    //        for (size_t i = 0; i < 10; i++)
    //        {
    //            if (is_exit()) break;//通过函数指针检测是否退出
    //            cout << "." << flush;
    //            this_thread::sleep_for(500ms);
    //        }
    //
    //        return 10;
    //    }
    //    string name = "";
    //};
    ////①创建线程池
    //XThreadPool pool;
    //pool.Init(4);
    ////②启动线程
    //pool.Start();
    ////③添加任务
    ////MyTask task1;
    ////task1.name = "Test name 001";
    ////pool.AddTask(&task1);
    ////MyTask task2;
    ////task1.name = "Test name 002";
    ////pool.AddTask(&task2);
    ////this_thread::sleep_for(100ms);
    //
    ////智能指针
    //{
    //    auto task3 = make_shared<MyTask>();
    //    task3->name = "Test name 003";
    //    pool.AddTask(task3);
    //    auto task4 = make_shared<MyTask>();
    //    task4->name = "Test name 004";
    //    pool.AddTask(task4);
    //    cout <<"task4->GetReturn()="<< task4->GetReturn() << endl;
    //}
    //
    //cout << "task run count= " << pool.task_run_count() << endl;
    //
    ////④1s后线程退出
    //this_thread::sleep_for(1s);
    //pool.Stop();
    //cout << "task run count= " << pool.task_run_count() << endl;
    //
    //getchar();

    

#endif 

//stl
#if 0

   // stl_vecotr();
   // stl_deque();
   // vector_vs_deque;
   // stl_list();
   // stl_map();
   // stl_set();
    //stl_stack();
    //stl_queue();
    //fang_func();
    //adapter();


#endif

#if 0
   //demo03.hpp--template
    
   // writefile();
   // readfile();
   // offset_seek();
   // read_picture();
   // printTimes<3>("hello");
   // cout << add(1, 2) << endl;
   //cout << add(1.1, 2.2) << endl;

    //Stu s1("Tom",20,99);
    //s1.Show();
    //Me me;
    //MyClass<Me> myclass;
    //myclass.showInfo(me);

    //Stu2<string, int> s1("Tom", 99);
    //func1(s1);
    //func2(s1);
    //func3(s1);

    //Stu3<string, int> s3("Tom", 99);
    //PrintS(s3);
    //Stu3<string, int> s4("Dan", 99);
    //PrintS(s4);
#endif

#if 0

    //std::cout << "Please, enter an integer (I'll be printing dots): \n";
    //std::thread th(read_value);
    //std::mutex mtx;
    //std::unique_lock<std::mutex> lck(mtx);
    //while (cv.wait_for(lck, std::chrono::seconds(1)) == std::cv_status::timeout)
    //{
    //    std::cout << '.' << std::endl;
    //}
    //std::cout << "You entered: " << value << '\n';
    //th.join();

    //std::thread consumer_thread(consume, 10);
    //for (int i = 0; i < 10; ++i)
    //{
    //    //每次cargo每次为0才运行。
    //    while (shipment_available())  std::this_thread::yield();
    //    std::unique_lock<std::mutex> lck(mtx);
    //    cargo = i + 1;
    //    cv.notify_one();
    //}
    //consumer_thread.join();

    //Box acc1(100);
    //Box acc2(50);
    //std::thread t1(transfer, std::ref(acc1), std::ref(acc2), 10);
    //std::thread t2(transfer, std::ref(acc2), std::ref(acc1), 5);
    //t1.join();
    //t2.join();
    //std::cout << "acc1 num_things: " << acc1.num_things << std::endl;
    //std::cout << "acc2 num_things: " << acc2.num_things << std::endl;

    //std::thread th1(print_block_1, 1000, '*');
    //std::thread th2(print_block_2, 1000, '$');
    //th1.join();
    //th2.join();

    //thread first(thread_1);  // 开启线程，调用：thread_1()
    //thread second(thread_2, 100); // 开启线程，调用：thread_2(100)
    //first.detach();
    //second.detach();
    //for (int i = 0; i < 20; i++)
    //{
    //    std::cout << "主线程\n";
    //}

   // //unique_ptr<AA> pa(new AA("Tom"));
   // //unique_ptr<AA> pa2 = make_unique<AA>("Dan");
   // //shared_ptr<AA> sa(new AA("TOM"));
   // //cout << "sa.use_count()=" << sa.use_count() << endl;
   // //cout << "sa.name=" << sa->name << endl;
   // unique_ptr<AA, decltype(deletefunc)*> up(new AA("Tom"), deletefunc);
   // unique_ptr<AA, decltype(lambdafunc)> up2(new AA("Tom2"), lambdafunc);

   // //cout << "---------------\n";
   // bar<int, float, char> b;
   // b.foo3(1, 3.14, 'a');
   // cout << 1.0_mm << endl;

   // //cout << "---------------\n";

   // char* ch1 = nullptr;
   // char* ch2 = 0;
   // assert(ch1 == 0);
   // assert(ch1 == nullptr);
   // assert(!ch1);
   // assert(ch2 == nullptr);
   // assert(nullptr == 0);


   // //cout << "---------------\n";

   // int_map1<std::string> int2str;
   // int2str[1] = "lucy";
   // cout << int2str[1] << endl;
   // int_map2<string>::type int2str2;
   // int2str2[2] = "Tom";
   // cout << int2str2[2] << endl;


   // X1 *x1 = new X1;
   // X2* x2 = new X2;
   //// PRINT_NOEXCEPT(noexcept(x1->~X1()));
   // PRINT_NOEXCEPT(noexcept(x2->~X2()));

   // //auto [x, y,z] = return_value();
   // //cout << x << " " << y <<" "<<z << endl;

   // //X&& x2 = make_x();
   // //x2.show();
   // //cout << "---------------\n";
   // //int a = 1, b = 2;
   // //int&& a2 = static_cast<int&&>(a);
   // //int&& b2 = move(b);
   // //cout << a << " " << a2 << " " << b << " " << b2 << endl;

#endif 
return 0;
}
