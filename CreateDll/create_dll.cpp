#define _CRT_SECURE_NO_WARNINGS
#include"pch.h"
#include "create_dll.h"
#include<stdexcept>

int add(int a, int b) {
	return a + b;
}

const char* GetDllVersion() {
	return "1.0.0";
}

MyMath::MyMath(int f)  {
	factor = f;
}
MyMath::~MyMath() {}
int MyMath::multiply(int a, int b) {
	return a * b * factor;
}
double MyMath::divide(int a, int b) {
	if (b==0)
	{
		throw std::invalid_argument("Division by zero");
	}
	return (a / b) * factor;
}

int MyMath::square(int a) {
	return a * a;
}

// MyStringUtil class implementation
MyStringUtil::MyStringUtil(const char* initStr) {
	if (initStr == nullptr) {
		str_ = new char[1];
		str_[0] = '\0';
	}
	else {
		size_t len = std::strlen(initStr);
		str_ = new char[len + 1];
		strcpy_s(str_,len+1, initStr);
	}
}

MyStringUtil::~MyStringUtil() {
	delete[] str_;
	str_ = nullptr;
}

int MyStringUtil::GetLength() {
	return static_cast<int>(std::strlen(str_));
}

const char* MyStringUtil::GetString() {
	return str_;
}

void MyStringUtil::SetString(const char* newStr) {
	if (newStr == nullptr) return;
	delete[] str_;
	size_t len = std::strlen(newStr);
	str_ = new char[len + 1];
	strcpy_s(str_,len+1, newStr);
}

//C-style interface implementations(C# use)
MyStringUtil* MyStringUtil_Create(const char* initStr) {
	return new MyStringUtil(initStr);
}
void MyStringUtil_Destroy(MyStringUtil* instance) {
	if(instance != nullptr) {
		delete instance;
	}
}
int MyStringUtil_GetLength(MyStringUtil* instance) {
	if (instance == nullptr) return -1;
	return instance->GetLength();
}
const char* MyStringUtil_GetString(MyStringUtil* instance) {
	if (instance == nullptr) return "";
	return instance->GetString();
}
void MyStringUtil_SetString(MyStringUtil* instance, const char* newStr) {
	if(instance!=nullptr&&newStr != nullptr) {
		instance->SetString(newStr);
	}
}
