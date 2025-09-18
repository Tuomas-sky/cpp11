#pragma once	
#ifdef CREATE_DLL_EXPORTS
#define CREATE_DLL_API __declspec(dllexport)
#else
#define CREATE_DLL_API __declspec(dllimport)
#endif

//������ͨ����
extern "C" CREATE_DLL_API int add(int a, int b);
extern "C" CREATE_DLL_API const char* GetDllVersion();
//������
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

//������
class CREATE_DLL_API MyStringUtil {
private:
	char* str_;
public:
	MyStringUtil(const char* initStr);
	~MyStringUtil();
	int GetLength();//��ȡ�ַ�������
	const char* GetString();//��ȡ�ַ�������
	void SetString(const char* newStr);//�����µ��ַ���
};
//�������ṩ��C#���õĽӿڣ�C#����ֱ��ʹ�õ����࣬��Ҫ��������ĵ�������������װ��C���Ľӿ�
extern "C" {
	//����ʵ��
	CREATE_DLL_API MyStringUtil* MyStringUtil_Create(const char* initStr);
	//����ʵ��
	CREATE_DLL_API void MyStringUtil_Destroy(MyStringUtil* instance);
	//��ȡ�ַ�������
	CREATE_DLL_API int MyStringUtil_GetLength(MyStringUtil* instance);
	//��ȡ�ַ�������
	CREATE_DLL_API const char* MyStringUtil_GetString(MyStringUtil* instance);
	//�����µ��ַ���
	CREATE_DLL_API void MyStringUtil_SetString(MyStringUtil* instance, const char* newStr);
}


