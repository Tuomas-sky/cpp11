/*
需求
1、日志存储：文本文件
2、日志内容：时间、级别、文件、行号、内容
3、日志级别：debug<info<warn<error<fatal
4、日志翻滚：设置日志的大小
*/
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<ctime>//提供了处理时间和日期的函数与类型主要用于获取系统时间、转换时间格式等操作
#include<cstdarg>//包含va_list、va_start、va_arg、va_end等宏,常用于需要灵活参数列表的函数实现
#include<stdexcept>
using namespace std;

namespace utility {
#define debug(fmt,...) Logger::getInstance()->Log(Logger::DEBUG,__FILE__,__LINE__,fmt,##__VA_ARGS__);
#define info(fmt,...) Logger::getInstance()->Log(Logger::INFO,__FILE__,__LINE__,fmt,##__VA_ARGS__);
#define warn(fmt,...) Logger::getInstance()->Log(Logger::WARN,__FILE__,__LINE__,fmt,##__VA_ARGS__);
#define error(fmt,...) Logger::getInstance()->Log(Logger::ERROR,__FILE__,__LINE__,fmt,##__VA_ARGS__);
#define fatal(fmt,...) Logger::getInstance()->Log(Logger::FATAL,__FILE__,__LINE__,fmt,##__VA_ARGS__);

	class Logger {
	public:
		//日志级别
		enum Level {
			DEBUG=0,
			INFO,
			WARN,
			ERROR,
			FATAL,
			LEVEL_COUNT,
		};
		//open file
		void Open(const string& filename) {
			filename_ = filename;
			fout_.open(filename,ios::app);
			if (fout_.fail())
				throw logic_error("open file error " + filename);

			fout_.seekp(0, ios::end);
			local_len_ = fout_.tellp();
		}
		//close file
		void Close() {
			fout_.close();
		}
		static Logger* getInstance() {
			if (log_instance_ == nullptr)
				log_instance_ = new Logger();
			return log_instance_;
		}
		//日志级别
		void level(Level level) {
			m_level_ = level;
		}
		//日志回滚
		void max_len(int bytes) {
			max_length_ = bytes;
		}

		//记录Log信息
		void Log(Level level,const char* filename,int line,const char* format,...) {
			//指定什么级别的日志不输出
			if (m_level_ > level)
				return;
			
			if (fout_.fail())
				throw logic_error("open file failed " + filename_);

			//构造时间戳
			time_t tick = time(nullptr);
			struct tm ptm = {};
			localtime_s(&ptm, &tick);
			char timestamp[32];
			memset(timestamp, 0, sizeof(timestamp));
			strftime(timestamp, sizeof(timestamp), "%Y/%m/%d %H:%M:%S", &ptm);
			const char* fmt = "%s %s %s:%d";
			int size = snprintf(nullptr, 0, fmt,timestamp,level_[level],filename,line);
			if (size > 0) {
				char* buffer = new char[size + 1];
				snprintf(buffer, size + 1, fmt,timestamp,level_[level], filename, line);
				buffer[size] = 0;
				cout << buffer << endl;
				fout_ << buffer << " ";

				local_len_ += line;
				delete buffer;
			}
			//输出可变参数
			va_list args_ptr;
			va_start(args_ptr, format);
			size = vsnprintf(nullptr, 0, format, args_ptr);
			va_end(args_ptr);
			if (size > 0) {
				char* buffer = new char[size + 1];
				va_start(args_ptr, format);
				vsnprintf(buffer, size + 1, format, args_ptr);
				va_end(args_ptr);
				cout << buffer << endl;
				fout_ << buffer;

				local_len_ += line;
				delete buffer;
			}
			fout_ << "\n";
			fout_.flush();

			if (local_len_ >= max_length_ && max_length_ > 0) {
				rotate();
			}
		}

	private:
		void rotate() {
			Close();
			time_t tick = time(nullptr);
			struct tm ptm{};
			localtime_s(&ptm, &tick);
			char timestamp[32];
			memset(timestamp, 0, sizeof(timestamp));
			strftime(timestamp, sizeof(timestamp), ".%Y-%m-%d_%H-%M-%S", &ptm);
			string filename = filename_ + timestamp;
			if (rename(filename_.c_str(), filename.c_str())!=0) {
				throw logic_error("rename log file error ");
			}
			Open(filename_);
		}

		Logger():m_level_(DEBUG),max_length_(0),local_len_(0){}
		~Logger() {
			Close();
		}
	private:
		fstream fout_;
		string filename_;
		static const char* level_[LEVEL_COUNT];
		//单例模式
		static Logger* log_instance_;
		//打印日志级别
		Level m_level_;
		//日志回滚
		int max_length_;//最大长度
		int local_len_;//file 当前长度
	};
	//静态变量初始化
	const char* Logger::level_[]{
		"DEBUG",
		"INFO",
		"WARN,"
		"ERROR",
		"FATAL",
	};
	Logger* Logger::log_instance_ = nullptr;
}
