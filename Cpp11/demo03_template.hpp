#pragma once
#include <vector>
#include<iostream>
using namespace std;
//template函数模板
//非类型参数
template <int N>
void printTimes(std::string msg) {
    for (int i = 0; i < N; i++)
    {
        cout << msg << " " << i + 1 << endl;
    }
}

template <typename T>
T add(T a, T b) {
    return a + b;
}
//模板特化
template<>
int add(int a, int b) {
    return a + b + 100;
}

//类模板
template<class T1, class T2>
class Stu {
private:
    string name;
    T1 age;
    T2 score;
public:
    Stu(string n, T1 a, T2 s) :name(n), age(a), score(s) {}
    void Show() {
        cout << this->name << " " << this->age << " " << this->score << endl;
    }
};
//////////////////////
class Me {
public:
    void show() {
        std::cout << "Me::show is called!\n";
    }
};
template <typename T>
class MyClass {
public:
    void showInfo(T t);
};
template <typename T>
void MyClass<T>::showInfo(T t) {
   t.show();
}

//2 类模板对象作为参数传递
template<typename T1,typename T2>
class Stu2 {
private :
    T1 name;
    T2 score;
public:
    Stu2(T1 n, T2 s) :name(n), score(s) {}
    void ShowInfo() {
        cout << name << " " << score << endl;
    }
};
//2.1 确定具体类型传入
void func1(Stu2<string, int>& p) {
    p.ShowInfo();
}
//2.2参数模板化
template <typename T1,typename T2>
void func2(Stu2<T1, T2>& p) {
    p.ShowInfo();
}
//2.3将整个类模板化
template<typename T>
void func3(T& t) {
    t.ShowInfo();
}

//3.模板类继承
//3.1父类是模板类，子类是普通类
//解决方法：确定父类的泛型类型
template <typename T>
class Base {
public:
    T var1;
};
class Derived1 :public Base<int> {
};
//3.2 父类是模板类，子类是模板类
//解决办法：通过模板的方式继承
template <typename T,typename U>
class Derived2 :public Base<T> {
public:
    U var2;
};

//4.友元函数和模板类
template <typename T1,typename T2>
class Stu3 {
    //4.1友元函数类内实现
    friend void PrintS(Stu3<T1, T2>& p) {
        cout << p.name << " " << p.score << endl;
        p.showme();
    }
public:
    Stu3(T1 n, T2 s) :name(n), score(s) {}
    void showme() {
        cout << "我是: " << this->name << ",分值: " << this->score << endl;
    }
private:
    T1 name;
    T2 score;
};

//4.2友元函数类内实现

//4.2.3 类前声明
template<typename T1,typename T2>
class Stu4;
template <typename T1,typename T2>
void PrintS(Stu4<T1, T2>& p);

template <typename T1,typename T2>
class Stu4 {
    //4.2.1 类内声明，带<>
    friend void PrintS<>(Stu4<T1, T2>& p);
private:
    T1 name;
    T2 age;
public:
    Stu4(T1 n, T2 a) :name(n), age(a) {}
    void showme() {
        cout << "我是: " << this->name << ",分值: " << this->score << endl;
    }
};
//4.2.2类外实现
template<typename T1,typename T2>
void PrintS(Stu4<T1, T2>& p) {
    cout << p.name << " " << p.score << endl;
    p.showme();
}