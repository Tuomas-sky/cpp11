#include<iostream>
#include<thread>
#include<mutex>
#include<vector>
#include<condition_variable>
#include<semaphore>
#include<future>
#include<queue>
#include<list>

#include<sstream>
#include<Windows.h>
#include <chrono>
using namespace std;


#if 0

using namespace std;
mutex mtx1;
recursive_mutex rmtx1;
timed_mutex tmtx1;

class MyClass {
public:
	void Func() {
		cout << "Member function\n";
	}
};

int sum = 0;
void thread_add(int times) {
	/*
thread
互斥量和锁mutex
条件变量
多线程异步编程
原子操作：atomic
其他同步原语
局部存储：thread_local
	*/
	for (int i = 0; i < times; i++)
	{
		mtx1.lock();
		sum++;
		mtx1.unlock();
	}
}

void re_mutex() {

	rmtx1.lock();
	cout << this_thread::get_id() << " recursive mutex is locked!\n";

	rmtx1.lock();
	cout << this_thread::get_id() << " recursive mutex  is locked again! \n";

	rmtx1.unlock();
	cout << this_thread::get_id() << " recursive mutex is unlocked!\n";

	rmtx1.unlock();
	cout << this_thread::get_id() << " recursive mutex is unlocked again!\n";

}

void time_mutex() {
	auto start = std::chrono::high_resolution_clock::now();
	//尝试在指定时间内获取锁
	if (tmtx1.try_lock_for(std::chrono::seconds(2))) {
		cout << "Thread " << this_thread::get_id() << " locked mutex.\n";
		this_thread::sleep_for(3s);
		tmtx1.unlock();
		cout << "Thread " << this_thread::get_id() << " unlocked mutex.\n";
	}
	else {
		cout << "Thread " << this_thread::get_id() << " locked failed.\n";
	}
	auto  end = std::chrono::high_resolution_clock::now();
	chrono::duration<double> diff = end - start;
	cout << "Thread " << this_thread::get_id() << " took " << diff.count() << " s\n";

}

//1、lock_guard vs unique_lock  RAII (Recource Acquisition Is Initialization)
//lock_guard
/*
问题描述细节
锁的粒度问题: std:lock_guard的锁作用域与对象的作用域相同，可能导致锁的粒度过大，影响
并发性能。
无法中途解锁: std:lock_guard对象构造，锁将被自动获取，并在对象析构时自动释放，无
法中途手动解锁。
不支持条件变量：std::lock_guard通常与互斥锁一起使用，但不支持与条件变量（如
std:condition_variable）配合使用。
不支持递归锁: 虽然可以与std::recursive_mutex一起使用，但std:lock_guard本身不提供对递
归锁的特殊支持。
无法指定锁的尝试:  std:lock_guard总是尝试获取锁，不支持非阻塞（尝试）获取锁。
无法与超时机制结合: std:lock_guard不提供超时机制，无法设置获取锁的等待时间。
*/

int num = 0;
void add() {

	lock_guard<mutex> lock(mtx1);
	for (int i = 0; i < 10000; i++)
	{
		num++;//相对而言，加锁越细，效率越低,lock_guard不支持手动解锁
	}
}
void print_id(int id) {
	lock_guard<mutex> lock(mtx1);
	cout << "# " << id << ",Thread id=" << this_thread::get_id() << endl;
}

//unique_lock
void unique_lock_defer() {
	unique_lock<mutex> lock(mtx1, defer_lock);
	lock.lock();//自己上锁
	cout << "Thread id=" << this_thread::get_id() << " ,defer_lock!" << endl;
	lock.unlock();
}

void unique_lock_try() {
	unique_lock<mutex> lock(mtx1, try_to_lock);
	lock.unlock();
	if (lock.owns_lock()) {
		cout << "锁上了\n";
	}
	else {
		cout << "没锁上\n";
	}
}

void unique_lock_adopt() {
	mtx1.lock();//先锁定互斥锁
	unique_lock<mutex> lock(mtx1, adopt_lock);//接管已锁定的互斥锁
	cout << "call unique_lock adopt\n";
}


//2、线程间的同步
 /*
 互斥锁：用于保护共享资源，防止多个线程同时访问同一个资源
读写锁：读写锁允许多个线程同时读取，但只允许一个线程写入
条件变量：条件变量用于阻塞一个或多个线程，直到某个条件为真
信号量：信号量是一种更高级的同步机制，控制对共享资源的访问
原子操作：原子操作用于对单个变量的无锁保护，常用于计数器等简单的数据
同步
栅栏：用于协调多个线程的执行，使得它们在某个同步点等待，直到所有线程
都到达该点《C++20》
 */


 /*
等待和通知机制：线程可以等待某个条件的改变，而不需要一直占用CPU资源。
与互斥锁配合使用：通常与互斥锁一起使用，以保护共享数据的访问。
线程通信：实现线程之间的高效通信和协调

wait thread 与 notify thread
 */

 //2、条件变量
mutex mtx;
condition_variable cv;
bool flag = false;

void printInfo(int i) {
	//cout << "call printInfo\n";
	unique_lock<mutex> lock(mtx);
	/* while (!flag) {
		 cv.wait(lock);
	 }*/
	cv.wait(lock, []() {return flag; });//Pred返回bool，表示线程是否应该继续等待
	cout << this_thread::get_id() << " - " << i << endl;
}
void update_flag() {
	cout << "call updata" << endl;
	this_thread::sleep_for(3s);
	unique_lock<mutex> lock(mtx);
	flag = true;
	cv.notify_all();
}
//调用wait的工作原理
/*
ii．工作原理
1．释放锁：当线程调用wait方法时，它会首先释放与std:unique_lock关联的互斥锁。这是为了允许其他线程可
以访问和修改与条件变量相关的共享数据。
2．进入等待队列：释放锁后，线程会进入条件变量的等待队列中，进入阻塞状态。此时线程不再消耗CPU时
间，直到被唤醒。
3．等待条件或通知：线程在等待队列中等待，直到两个条件之一发生：
。另一个线程调用了同一个条件变量的notify_one或notify_all方法，并且该线程是等待队列中的第一个线
程（对于notify_one）或等待队列中的所有线程（对于notify_all）。
。谓词函数 pred 返回 true。
4．重新获取锁：当线程被唤醒后（无论是由于收到通知还是条件成立），它会尝试重新获取之前释放的互斥锁。
如果此时锁已经被其他线程持有，则该线程会阻塞在互斥锁上，直到获得锁。
5．检查条件：获得锁后，线程会再次调用谓词函数pred来检查条件是否成立。如果条件不成立（即pred 返回
false），则线程会重新进入等待队列，并释放锁，继续等待。这个过程会不断重复，直到条件成立。
6．继续执行：如果条件成立（即pred 返回 true），则线程会退出wait 方法，并继续执行后续的代码。
 */

int value = 0;
bool turn = false;
void increment(int id) {
	for (int i = 0; i < 10; i++)
	{
		//等待轮到自己执行
		unique_lock<mutex> lock(mtx);
		cv.wait(lock, [&]() {return (id == 1) ? !turn : turn; });

		//执行操作
		++value;
		cout << "thread #" << id << " increse " << value << endl;

		//切换标志
		turn = !turn;
		cv.notify_all();
	}
}

//3、读写锁
/*
1.共享锁（Shared Lock）:允许多个线程同时持有。多个线程可以并发地读取共享资源。不能与独占锁同时持有。

2.独占锁（ExclusiveLock）：只能有一个线程持有。持有独占锁的线程可以写入共享资源。
当持有独占锁时，其他线程不能持有共享锁或独占锁。

3.读写分离：
读操作和写操作分开处理，提高并发性能。。适用于读多写少的场景。
 */

////4.1、seamphore - 互斥
//counting_semaphore<1> sem(1);
//void semaphore_woker(int id) {
//	sem.acquire();//p操作
//	cout << "Woker id=" << id << " is working...\n";
//	this_thread::sleep_for(1s);
//	cout << "Woker id=" << id << " is done...\n";
//	sem.release();//v 操作
//}
////4.2、seamphore - 同步
//
//counting_semaphore<1> ready(0);
//counting_semaphore<1> done(1);
//
//void prepare() {
//	done.acquire();
//	cout << "Prapare..." << endl;
//	this_thread::sleep_for(2s);
//	cout << "Prepare done...\n";
//	ready.release();
//}
//void work() {
//	ready.acquire();
//	cout << "Work...\n";
//	this_thread::sleep_for(1s);
//	cout << "Work done...\n";
//	done.release();
//}

//5、栅栏barrier
/*
栅栏的作用和特点
特点
·同步点：栅栏用于创建一个同步点，确保多个线程在同一时刻同步。
·计数器：栅栏内部维护一个计数器，记录到达栅栏的线程数量。当计数器达
到预设值时，所有等待的线程被同时唤醒。
●可重用性：C++20中的std:barrier是可重用的，线程可以反复使用同一个栅栏对象进行同步。
作用
1.阶段同步：确保多个线程在某一阶段完成后再进入下一阶段。例如，在并
行计算中，各个线程可能需要同步它们的计算结果，然后再继续下一步计算。
2.批处理：多个线程可能需要在某些关键时刻汇聚数据或状态，然后再继续
各自的任务
*/

//6、死锁
/*
死锁通常由以下四个必要条件引起，这四个条件同时满足时，系统就会产生死锁：
1．互斥条件（Mutual Exclusion）:
:资源一次只能被一个线程占用。
2．请求和保持条件（Hold and Wait）:
。线程已经持有至少一个资源，同时又申请新的资源，而新资源被其他线程占有。
3.不剥夺条件（NoPreemption）:
。已获得的资源在未使用完之前，不能被强行剥夺，只能在使用完毕后由线程自己释放。
4.循环等待条件（CircularWait）：
。存在一个线程循环等待的链，链中的每个线程都持有下一个线程需要的资源。
*/
mutex m1;
mutex m2;
void task1() {
	unique_lock<mutex> lock1(m1);
	this_thread::sleep_for(1s);
	unique_lock<mutex> lock2(m2);
	cout << "task1 finished" << endl;
}
void task2() {
	unique_lock<mutex> lock1(m2);
	this_thread::sleep_for(1s);
	unique_lock<mutex> lock2(m1);
	cout << "task2 finished" << endl;
}

/*
如何解决死锁？
解决死锁的方法主要有以下几种：
1．避免循环等待条件：
。避免形成资源循环等待链。例如，可以通过对所有资源进行排序，并按顺序申请资源，确保不会形成循环等待。
2．破坏请求和保持条件：
。在一个线程申请资源时，不允许它同时持有其他资源。可以使用一次性分配所有资源的方法，即线程一次
性申请它所需要的所有资源，只有在所有资源都满足的情况下才开始执行，否则就释放已经获得的资源重
新申请。
：3．破坏不剥夺条件：
。如果一个线程申请新的资源时已持有的资源被占用，可以强制释放它已占有的资源。尽管这种方法在实际
系统中难以实现，但在某些情况下是可行的。
*/

//7、异步编程 feture 与 async
/*
* 异步编程的优点:
提高性能：通过并发执行多个任务，异步编程可以更高效地利用CPU资源。
提高响应速度：异步编程可以使程序在等待某些操作完成时继续响应用户输入，提高用户体验。
简化I/O 操作：异步编程特别适合处理I/O 密集型操作，如文件读取、网络请求等。
*/
//async
/*
std::async：是一个函数模板，用于启动一个异步任务。它接受一个可调用的对象（如函数、Lambda表达
式、函数对象等）作为参数，并在一个单独的线程上异步执行该对象。std:async自动管理异步任务的生命周
期，并返回一个std:future对象，该对象可用于获取异步操作的结果。
*/
int add_async(int a, int b) {
	cout << "add_async is called!" << endl;
	return a + b;
}
//future
/*
std:future：是一个模板类，用于表示异步操作的结果。它允许开发者在将来的某个时刻查询异步操作的状
态、等待操作完成或获取操作的结果。通常，std:future对象不是直接创建的，而是与std:async、
std::packaged_task或std:promise配合使用.
*/

//promise
void async_task(promise<int> prom) {
	this_thread::sleep_for(1s);
	prom.set_value(100);
}

//packaged_task
/*
* 
packaged_task的工作流程执行任务：
1、创建packaged_task：在创建packaged_task对象时，你需要传递一个可调用对象（如函数、lambda表达式等）给它。这个对象会被封装在
packaged_task中，成为一个异步任务。
2、获取std:future：使用get_future()方法可以获取与packaged_task关联的 std:future 对象，后者可以用于获取异步任务的结果。
3、执行任务：任务可以通过operator() 或者在一个新线程中调用std::thread来执行。
注意，这里的packaged_task 对象必须通过 std:move 传递，因为packaged_task 不允许复制。
4、获取结果：通过调用std:future的getO方法，调用线程可以等待异步任务
完成并获取其返回值。如果任务尚未完成，getO将阻塞直到结果可用。
*/

//Thread Pool

class ThreadPool {
public:
	ThreadPool(int threadnums);

	//添加任务
	template<typename F,typename ...Arg>
	auto enques(F&& f, Arg&& ...args) -> future<typename result_of<F(Arg...)>::type>;

	~ThreadPool();

private:
	void worker();

	bool isstop;
	mutex mtx;
	condition_variable cv;
	vector<thread> workers;
	std::queue<function<void()>> myque;
};

ThreadPool::ThreadPool(int nums) :isstop(false) {
	for (int i = 0; i < nums; i++)
	{
		workers.emplace_back([this]() {
			this->worker();
			});
	}
}
ThreadPool::~ThreadPool() {
	{
		unique_lock<mutex> lock(mtx);
		isstop = true;
	}
	cv.notify_all();
	for (thread& th : workers) {
		th.join();
	}
}

template<typename F,typename ...Args>
auto ThreadPool::enques(F&& f, Args&&...args)->future<typename result_of<F(Args...)>::type> {
	using functype = typename result_of<F(Args...)>::type;
	//获取一个智能指针，指向一个被包装为function<void()>类型的task
	auto task = make_shared<packaged_task<functype()>>(
		bind(std::forward<F>(f), std::forward<Args>(args)...)
	);

	//获取future
	future<functype> fures = task->get_future();

	//将任务添加到队列
	{
		unique_lock<mutex> lock(this->mtx);
		if (isstop) {
			throw std::runtime_error("error:线程池通停止了");
		}

		myque.emplace([task]() {
			(*task)();//调用task
			}); 
	}

	cv.notify_one();
	return fures;
}

void ThreadPool::worker() {
	while (true) {
		//定义任务
		function<void()> task;
		//从队列中取得的一个任务
		{
			unique_lock<mutex> lock(mtx);
			cv.wait(lock, [this]() {
				return this->isstop || !this->myque.empty();//为true往下执行
				});
			if (isstop && myque.empty()) return;
			task = move(this->myque.front());
			myque.pop();
		}
		//执行任务
		task();
	}
}
#endif
///////////////////////////////////////////////////////////

#if 1
//5.1 一次调用
bool is_exit = false;

void SubThread() {
	cout << "SubThread begin ,id=" << this_thread::get_id() << endl;
	for (int i = 0; i < 5; i++)
	{
		if (!is_exit) {
			cout << "id=" << i + 1 << endl;
			this_thread::sleep_for(chrono::seconds(1));
		}
	}
	cout << "SubThread end ,id=" << this_thread::get_id() << endl;
}

void SystemInit() {
	cout << "System Init\n";
}
void CallOnce() {
	static once_flag flag;
	call_once(flag, SystemInit);
}

//5.2互斥锁+list实现模拟线程通信
class XThread {
public:
	//线程开始
	virtual void Start() {
		is_exit_ = false;
		th_ = thread(&XThread::Main, this);
	}
	//设置线程退出标志并等待
	virtual void Stop() {
		is_exit_ = true;
		Wait();
	}
	//等待线程退出 （阻塞）
	virtual void Wait() {
		if (th_.joinable())
			th_.join();
	}
	//判断是否退出
	bool is_exit() { return is_exit_; }

protected:
	bool is_exit_ = false;
private:
	//线程入口
	virtual void Main() = 0;

	thread th_;
};

class MsgServer:public XThread {
public:
	//给当前线程发消息
	void SendMsg(string msg) {
		unique_lock<mutex> lock(mtx_);
		msgs_.push_back(msg);
	}

private:
	//线程入口函数
	void Main() override {
		while (!is_exit()) {
			this_thread::sleep_for(10ms);
			unique_lock<mutex> lock(mtx_);
			if (msgs_.empty())
				continue;
			while (!msgs_.empty()) {
				cout << "recv : " << msgs_.front() << endl;
				msgs_.pop_front();
			}
		}
	}
	//消息缓冲队列
	list<string> msgs_;
	mutex mtx_;

};


//5.3 使用条件变量cv测试
mutex cv_mtx;
list<string> msg_;
condition_variable cv_;
void ThreadWrite() {
	for (int i = 0; ; i++)
	{
		stringstream ss;
		ss << "Write msg" << i;
		unique_lock<mutex> lock(cv_mtx);
		msg_.push_back(ss.str());
		lock.unlock();
		cv_.notify_one();
		this_thread::sleep_for(1s);
	}
}

void ThreadRead(int i) {
	for (;;) {
		cout << "Read msg" << endl;
		unique_lock<mutex> lock(cv_mtx);
		//cv_.wait(lock);//解锁、阻塞等待信号
		cv_.wait(lock, [i] {
			cout << i << " wait\n";
			return !msg_.empty();
			});//返回true就继续往下走，返回false则继续wait
		if (msg_.empty()) return;
		//获取信号后锁定
		while (!msg_.empty()) {
			cout <<i<<" read "<< msg_.front() << endl;
			msg_.pop_front();
		}
	}
}

//5.4 使用条件变量cv测试
class MsgServer_V2 :public XThread {
public:
	//给当前线程发消息
	void SendMsg(string msg) {
		unique_lock<mutex> lock(mtx_);
		msgs_.push_back(msg);
		//通知Main
		lock.unlock();
		cv_.notify_one();
	}
	void Stop() override {
		is_exit_ = true;
		cv_.notify_all();
		Wait();
	}

private:
	//线程入口函数
	void Main() override {
		while (!is_exit()) {
			//this_thread::sleep_for(10ms);
			unique_lock<mutex> lock(mtx_);
			cv_.wait(lock, [this] {//注意线程退出
				cout << "wait cv \n";
				if (is_exit()) return true;
				return !msgs_.empty();
				});
			
			while (!msgs_.empty()) {
				cout << "recv : " << msgs_.front() << endl;
				msgs_.pop_front();
			}
		}
	}

	//消息缓冲队列
	list<string> msgs_;
	mutex mtx_;
	condition_variable cv_;

};

//5.5.1 普通函数版本 Thread Pool
#if 0
//5.5 ThreadPool
//任务的入口函数
class XTask {
public:
	virtual int  Run() = 0;
	std::function<bool()> is_exit = nullptr;
};

class XThreadPool {
public:
	//初始化线程池的线程数量
	void Init(int num) {
		unique_lock<mutex> lock(mtx_);
		this->thread_num_ = num;
		cout << "ThreadPool Init " << num << endl;
	}
	//启动所有线程，需要先Init
	void Start() {
		unique_lock<mutex> lock(mtx_);
		if (thread_num_ <= 0) {
			cerr << "Please Init XThreadPool" << endl;
		}
		if (!threads_.empty()) {
			cerr << "Thread pool has start!" << endl;
		}
		//start thread
		for (int i = 0; i < thread_num_; i++)
		{
			auto th = new thread(&XThreadPool::Run, this);
			threads_.push_back(th);
		}


	}

	//添加任务
	void AddTask(XTask* task) {
		unique_lock<mutex> lock(mtx_);
		tasks_.push_back(task);
		task->is_exit = [this] {return is_exit();};

		lock.unlock();
		cv_.notify_one();
	}

	//获取任务
	XTask* getTask() {
		unique_lock<mutex> lock(mtx_);
		if (tasks_.empty()) {
			cv_.wait(lock);
		}
		if (is_exit()) {
			return nullptr;//没有task了
		}
		if (tasks_.empty()) {
			return nullptr;//防止多次通知
		}
		auto task = tasks_.front();
		tasks_.pop_front();
		return task;
	}

	//线程池退出
	void Stop() {
		is_exit_ = true;
		cv_.notify_all();
		for (auto& th : threads_)
			th->join();
		//clear thread
		unique_lock<mutex> lock(mtx_);
		threads_.clear();
	}

	bool is_exit() { return is_exit_; }

	int task_run_count() { return task_run_count_; }

private:
	//线程池线程的入口函数
	void Run() {
		cout << "begin ThreadPool Run " << this_thread::get_id() << endl;
		while (!is_exit()) {
			auto task = getTask();
			if (!task) continue;
			++task_run_count_;
			try
			{
				task->Run();
			}
			catch (const std::exception&)
			{
				cerr << "exception" << endl;
			}
			--task_run_count_;
		}

		cout << "end ThreadPool Run " << this_thread::get_id() << endl;
	}

	int thread_num_ = 0;
	mutex mtx_;
	vector<std::thread*> threads_;//存放线程
	list<XTask*> tasks_;//任务列表
	condition_variable cv_;//消息通知
	bool is_exit_ = false;//线程池退出
	//正在运行的任务数量,线程安全
	std::atomic<int> task_run_count_ = { 0 };
};
#endif 

//5.5.2 智能指针版本 Thread Pool
#if 0
//5.5 ThreadPool
//任务的入口函数
class XTask {
public:
	virtual int  Run() = 0;
	std::function<bool()> is_exit = nullptr;
};

class XThreadPool {
public:
	//初始化线程池的线程数量
	void Init(int num) {
		unique_lock<mutex> lock(mtx_);
		this->thread_num_ = num;
		cout << "ThreadPool Init " << num << endl;
	}
	//启动所有线程，需要先Init
	void Start() {
		unique_lock<mutex> lock(mtx_);
		if (thread_num_ <= 0) {
			cerr << "Please Init XThreadPool" << endl;
		}
		if (!threads_.empty()) {
			cerr << "Thread pool has start!" << endl;
		}
		//start thread
		for (int i = 0; i < thread_num_; i++)
		{
			//auto th = new thread(&XThreadPool::Run, this);
			//智能指针
			auto th = make_shared<std::thread>(&XThreadPool::Run, this);
			threads_.push_back(th);
		}


	}

	//添加任务
	void AddTask(std::shared_ptr<XTask> task) {
		unique_lock<mutex> lock(mtx_);
		tasks_.push_back(task);
		task->is_exit = [this] {return is_exit();};

		lock.unlock();
		cv_.notify_one();
	}

	//获取任务
	std::shared_ptr<XTask> getTask() {
		unique_lock<mutex> lock(mtx_);
		if (tasks_.empty()) {
			cv_.wait(lock);
		}
		if (is_exit()) {
			return nullptr;//没有task了
		}
		if (tasks_.empty()) {
			return nullptr;//防止多次通知
		}
		auto task = tasks_.front();
		tasks_.pop_front();
		return task;
	}

	//线程池退出
	void Stop() {
		is_exit_ = true;
		cv_.notify_all();
		for (auto& th : threads_)
			th->join();
		//clear thread
		unique_lock<mutex> lock(mtx_);
		threads_.clear();
	}

	bool is_exit() { return is_exit_; }

	int task_run_count() { return task_run_count_; }

private:
	//线程池线程的入口函数
	void Run() {
		cout << "begin ThreadPool Run " << this_thread::get_id() << endl;
		while (!is_exit()) {
			auto task = getTask();
			if (!task) continue;
			++task_run_count_;
			try
			{
				task->Run();
			}
			catch (const std::exception&)
			{
				cerr << "exception" << endl;
			}
			--task_run_count_;
		}

		cout << "end ThreadPool Run " << this_thread::get_id() << endl;
	}

	int thread_num_ = 0;
	mutex mtx_;
	//智能指针版本
	vector<std::shared_ptr<std::thread>> threads_;//存放线程
	list<std::shared_ptr<XTask>> tasks_;//任务列表
	condition_variable cv_;//消息通知
	bool is_exit_ = false;//线程池退出
	//正在运行的任务数量,线程安全
	std::atomic<int> task_run_count_ = { 0 };
};
#endif 

//5.5.3 异步获取任务
#if 1
//5.5 ThreadPool
//任务的入口函数
class XTask {
public:
	virtual int  Run() = 0;
	std::function<bool()> is_exit = nullptr;
	void SetValue(int v) {
		p_.set_value(v);
	}
	auto GetReturn() {
		//阻塞等待set_value,只能调用一次
		return p_.get_future().get();
	}

private:
	//用来接收Run()的返回值
	promise<int> p_;
};

class XThreadPool {
public:
	//初始化线程池的线程数量
	void Init(int num) {
		unique_lock<mutex> lock(mtx_);
		this->thread_num_ = num;
		cout << "ThreadPool Init " << num << endl;
	}
	//启动所有线程，需要先Init
	void Start() {
		unique_lock<mutex> lock(mtx_);
		if (thread_num_ <= 0) {
			cerr << "Please Init XThreadPool" << endl;
		}
		if (!threads_.empty()) {
			cerr << "Thread pool has start!" << endl;
		}
		//start thread
		for (int i = 0; i < thread_num_; i++)
		{
			//auto th = new thread(&XThreadPool::Run, this);
			//智能指针
			auto th = make_shared<std::thread>(&XThreadPool::Run, this);
			threads_.push_back(th);
		}


	}

	//添加任务
	void AddTask(std::shared_ptr<XTask> task) {
		unique_lock<mutex> lock(mtx_);
		tasks_.push_back(task);
		task->is_exit = [this] {return is_exit();};

		lock.unlock();
		cv_.notify_one();
	}

	//获取任务
	std::shared_ptr<XTask> getTask() {
		unique_lock<mutex> lock(mtx_);
		if (tasks_.empty()) {
			cv_.wait(lock);
		}
		if (is_exit()) {
			return nullptr;//没有task了
		}
		if (tasks_.empty()) {
			return nullptr;//防止多次通知
		}
		auto task = tasks_.front();
		tasks_.pop_front();
		return task;
	}

	//线程池退出
	void Stop() {
		is_exit_ = true;
		cv_.notify_all();
		for (auto& th : threads_)
			th->join();
		//clear thread
		unique_lock<mutex> lock(mtx_);
		threads_.clear();
	}

	bool is_exit() { return is_exit_; }

	int task_run_count() { return task_run_count_; }

private:
	//线程池线程的入口函数
	void Run() {
		cout << "begin ThreadPool Run " << this_thread::get_id() << endl;
		while (!is_exit()) {
			auto task = getTask();
			if (!task) continue;
			++task_run_count_;
			try
			{
				//设置值
				auto re = task->Run();
				task->SetValue(re);
			}
			catch (const std::exception&)
			{
				cerr << "exception" << endl;
			}
			--task_run_count_;
		}

		cout << "end ThreadPool Run " << this_thread::get_id() << endl;
	}

	int thread_num_ = 0;
	mutex mtx_;
	//智能指针版本
	vector<std::shared_ptr<std::thread>> threads_;//存放线程
	list<std::shared_ptr<XTask>> tasks_;//任务列表
	condition_variable cv_;//消息通知
	bool is_exit_ = false;//线程池退出
	//正在运行的任务数量,线程安全
	std::atomic<int> task_run_count_ = { 0 };
};
#endif 

#endif // 1




