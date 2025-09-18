#pragma once	
#ifdef CREATE_DLL_EXPORTS
#define CREATE_DLL_API __declspec(dllexport)
#else
#define CREATE_DLL_API __declspec(dllimport)
#endif

//导出普通函数
extern "C" CREATE_DLL_API int add(int a, int b);
extern "C" CREATE_DLL_API const char* GetDllVersion();
//导出类
class CREATE_DLL_API MyMath {
	public:
		MyMath(int f=1);
		~MyMath();
	int multiply(int a, int b);
	double divide(int a, int b);
	static int square(int a);

public:
	int factor;
};

//导出类
class CREATE_DLL_API MyStringUtil {
private:
	char* str_;
public:
	MyStringUtil(const char* initStr);
	~MyStringUtil();
	int GetLength();//获取字符串长度
	const char* GetString();//获取字符串内容
	void SetString(const char* newStr);//设置新的字符串
};
//以下是提供给C#调用的接口，C#不能直接使用导出类，需要将导出类的导出函数单独封装成C风格的接口
extern "C" {
	//创建实例
	CREATE_DLL_API MyStringUtil* MyStringUtil_Create(const char* initStr);
	//销毁实例
	CREATE_DLL_API void MyStringUtil_Destroy(MyStringUtil* instance);
	//获取字符串长度
	CREATE_DLL_API int MyStringUtil_GetLength(MyStringUtil* instance);
	//获取字符串内容
	CREATE_DLL_API const char* MyStringUtil_GetString(MyStringUtil* instance);
	//设置新的字符串
	CREATE_DLL_API void MyStringUtil_SetString(MyStringUtil* instance, const char* newStr);
}


