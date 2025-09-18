using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TestDllCSharp
{
  
    internal class Program
    {
        // 导入普通函数
        [DllImport("CreateDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int add(int a, int b);

        [DllImport("CreateDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetDllVersion();

        // 导入MyStringUtil的C接口
        [DllImport("CreateDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr MyStringUtil_Create([MarshalAs(UnmanagedType.LPStr)] string initStr);

        [DllImport("CreateDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MyStringUtil_Destroy(IntPtr instance);

        [DllImport("CreateDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int MyStringUtil_GetLength(IntPtr instance);

        [DllImport("CreateDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr MyStringUtil_GetString(IntPtr instance);

        [DllImport("CreateDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MyStringUtil_SetString(IntPtr instance, [MarshalAs(UnmanagedType.LPStr)] string newStr);
        static void Main(string[] args)
        {
            // 测试普通函数
            int sum = add(15, 25);
            Console.WriteLine($"15 + 25 = {sum}");

            IntPtr versionPtr = GetDllVersion();
            string version = Marshal.PtrToStringAnsi(versionPtr);
            Console.WriteLine($"DLL Version: {version}");

            // 测试MyStringUtil
            IntPtr strInstance = MyStringUtil_Create("Hello C#");
            int length = MyStringUtil_GetLength(strInstance);
            Console.WriteLine($"字符串长度: {length}");

            IntPtr strPtr = MyStringUtil_GetString(strInstance);
            string content = Marshal.PtrToStringAnsi(strPtr);
            Console.WriteLine($"字符串内容: {content}");

            MyStringUtil_SetString(strInstance, "C# Test");
            strPtr = MyStringUtil_GetString(strInstance);
            content = Marshal.PtrToStringAnsi(strPtr);
            Console.WriteLine($"更新后内容: {content}");

            // 释放资源
            MyStringUtil_Destroy(strInstance);
        }
    }
}
