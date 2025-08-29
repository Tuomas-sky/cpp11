#include <iostream>
#include<vector>
#include <deque>
#include<list>
#include<map>
#include<stack>
#include<queue>
#include<algorithm>
#include<functional>

using namespace std;

void stl_vecotr() {
    vector<int> v{ 1,3,5,7,2 };
    v.push_back(100);
    v.push_back(200);
    v.push_back(300);
    for (auto it = v.begin(); it != v.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;
    v.pop_back();
    for (auto it : v) {
        cout << it << " ";
    }
    cout << endl;

    v.insert(v.begin(), { 10,20 });
    v.erase(v.end() - 2);
    for (auto it : v) {
        cout << it << " ";
    }
    cout << endl;

    auto it = find(v.begin(), v.end(), 7);
    cout << "find(v.begin(), v.end(), 7)=" << *it << endl;
    
}

void stl_deque() {//两端操作数据进行快速插入和删除，双端开口连续线性存储结构
    //deque分段连续空间的数据结构实现的
    //容易扩展，支持随机访问
    deque<int> dq1;
    dq1.assign(5, 10);
    dq1.assign({ 100,200 });
    for (auto d : dq1) {
        cout << d << " ";
    }
    cout << endl;
    vector<int> v2 = { 1,2,3 };
    deque<int> dq2(v2.begin(), v2.end());
    for (auto d : dq2) {
        cout << d << " ";
    }
    cout << endl;
}

void vector_vs_deque() {
    cout << R"(
4.1内部实现
1.vector:
。底层通过动态数组实现。
。内部分配一块连续的内存区域，因此支持随机访问。
。在末尾进行插入和删除操作效率较高，但在中间或头部插入 / 删除操作效率较低。
2. deque:
。底层通过一系列固定大小的数组块实现。
。允许在两端（头部和尾部）进行高效的插入和删除操作。
·支持随机访问，但不像vector那样连续内存，因此在插入和删除操作上可能更加高效。

4.2．性能特征：
1．随机访问：
。对于随机访问，vector的性能优于deque，因为vector具有连续的内存存储，因此访问元素的时间复杂度
为O(1)，而deque的时间复杂度为O(1)或更高。
2．插入和删除操作：
 在末尾进行插入和删除操作时，vector和deque的性能相似，都为常数时间复杂度（摊销情况下）。
 在中间或头部进行插入和删除操作时，deque的性能通常优于vector，因为deque允许在两端进行高效
操作，而vector需要移动大量元素。
3．内存分配：
 vector在需要扩展容量时，可能需要重新分配内存并复制元素，这可能导致一些性能开销。
 deque的每个块都是固定大小的，因此它可能会频繁地分配和释放内存块，这可能会增加一些额外的开
销。


4.3.选择使用场景：
·如果需要高效的随机访问，并且主要操作是在末尾进行插入和删除操作，那么vector可能更适合。
·如果需要在两端进行高效的插入和删除操作，或者在中间进行频繁的插入和删除操作，那么deque可能更适
合。deque在处理大量数据时，可能比vector更具性能优势。

)" << endl;

           
}

void stl_list() {
    string s = R"(
list的特点
1．动态大小：list的大小可以在运行时动态改变，无需预先分配固定大小的内存。
2.双向选代：可以通过迭代器在list中向前或向后遍历元素。
3．高效插入和删除：在list中的任何位置插入或删除元素都只需要改变相邻元素的指针，时间复杂度为O(1)。
4．非连续内存：list中的元素不存储在连续的内存中，这有助于在内存碎片较多的环境中更有效地管理内存。

list与vector的区别
1．内存布局：Vector的元素存储在连续的内存空间中，而list的元素则分散存储。
2.插入和删除操作：在vector的中间位置插入或删除元素需要移动大量的元素，因此时间复杂度较高
(O(n)）。而在list中，这些操作只需要改变相邻节点的指针，时间复杂度为O(1)。
3．访问元素：由于vector的元素存储在连续的内存中，因此可以通过下标直接访问任何元素，时间复杂度为0(1)。
而在list中，访问元素需要通过迭代器逐步遍历，时间复杂度为O(n)。
4．空间效率：vector在动态调整大小时可能会进行内存分配和释放，这可能导致一定的空间浪费。而list不需
要预先分配固定大小的内存，因此空间效率更高。
)";
    list<int> l = { 1,2,4,5 };
    auto it = l.begin();
    std::advance(it, 2);
    l.insert(it, 100);
    for (auto v : l) {
        cout << v << " ";
    }
    cout << endl;



}

void stl_map() {
    /*
    那么，有了平衡二叉树为什么还要红黑树呢？这主要源于红黑树的以下优势：
实现简单性：红黑树的平衡性要求相对宽松，这使得其实现比平衡二叉树更简单。在实践中，红黑树的代码通
常更短、更易于理解和维护。
性能稳定性：虽然平衡二叉树在理论上具有更严格的平衡性，但在实际应用中，红黑树的性能通常更为稳定。
红黑树的平衡性调整策略使其在插入和删除操作中能够保持相对稳定的性能。
应用场景广泛性：红黑树由于其简单性和性能稳定性，在多种场合得到了广泛应用。例如，在C++的STL库
中，map和set容器的底层实现通常使用红黑树。此外，红黑树也常用于内核模块、数据库索引等场景。
    */

    map<string, int> m;
    m["Tom1"] = 1;
    m.insert(std::pair<string, int>("Tom2", 2));
    m.insert(std::make_pair<string, int>("Tom3", 3));
    for (auto& val : m) {
        cout << val.first << " ： " << val.second << endl;
    }
    m.erase("Tom1");
    if (m.empty()) {
        cout << "map is empty!\n";
    }
    else {
        cout << "map is not empty!\n";
    }
    cout << "map element=" << m.size() << endl;

}

void stl_set() {

}

void stl_stack() {
    //stack使用deque/list作为其底层的实现，不支持迭代器访问，是一种受限的数据结构
}

void stl_queue() {
    //queue更多作为适配器，底层使用deque/list实现
    queue<int> q;
    q.push(1);
    q.push(3);
    q.push(5);
    q.push(7);
    cout << "q.size()=" << q.size() << endl;
    while (!q.empty()) {
        cout << q.front() << endl;
        q.pop();
    }
    cout << "q.size()=" << q.size() << endl;
}

//仿函数
struct CompareInt {
    bool operator()(int a, int b) {
        return a < b;
    }
};
class MyFunc {
public:
    int operator()(int val) {
        return 2 * val;
    }
};
void callFunc(int val, MyFunc& myfunc) {
    cout << "调用仿函数后,数值改变：" << myfunc(val)  << endl;
}

void fang_func() {
    vector<int> v{ 2,5,1,4,7 };
    cout << "排序前: \n";
    for (auto val : v) {
        cout << val << " ";
    }
    cout << endl;
    sort(v.begin(), v.end(), CompareInt());
    cout << "排序后: \n";
    for (auto val : v) {
        cout << val << " ";
    }
    cout << endl;

    MyFunc func;
    callFunc(10, func);

}

//算法algorithem


//函数适配器
//绑定适配器/成员函数适配器/谓词适配器
bool lesszero(int a, int b) {
    return a < 0;
}
struct Foo {
    void print_sum(int a, int b) {
        cout << a + b << endl;
    }
};

void adapter() {
    vector<int> v{ 1,-3,5,-6,-3,7 };
    //1.bind操作
    auto myfunc = bind(lesszero, placeholders::_1, 0);
    auto val = count_if(v.begin(), v.end(), myfunc);
    cout << val << endl;
    cout << "-------------------------\n";

    //2.mem_fn()-->成员函数
    Foo foo;
    auto foo_bind = std::mem_fn(&Foo::print_sum);
    foo_bind(foo, 3, 5);

    //3.谓词适配器
    vector<int> vec{ 1,3,5,2,6 };
    auto is_even = [](int a) {return a % 2 == 0;};
    auto is_odd = std::not1(std::function<bool(int)>(is_even));
    auto result = find_if(vec.begin(), vec.end(), is_odd);
    cout << "*result=" << *result << endl;



}

