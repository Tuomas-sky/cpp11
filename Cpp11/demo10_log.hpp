/*
����
1����־�洢���ı��ļ�
2����־���ݣ�ʱ�䡢�����ļ����кš�����
3����־����debug<info<warn<error<fatal
4����־������������־�Ĵ�С
*/
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<ctime>//�ṩ�˴���ʱ������ڵĺ�����������Ҫ���ڻ�ȡϵͳʱ�䡢ת��ʱ���ʽ�Ȳ���
#include<cstdarg>//����va_list��va_start��va_arg��va_end�Ⱥ�,��������Ҫ�������б�ĺ���ʵ��
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
		//��־����
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
		//��־����
		void level(Level level) {
			m_level_ = level;
		}
		//��־�ع�
		void max_len(int bytes) {
			max_length_ = bytes;
		}

		//��¼Log��Ϣ
		void Log(Level level,const char* filename,int line,const char* format,...) {
			//ָ��ʲô�������־�����
			if (m_level_ > level)
				return;
			
			if (fout_.fail())
				throw logic_error("open file failed " + filename_);

			//����ʱ���
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
			//����ɱ����
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
		//����ģʽ
		static Logger* log_instance_;
		//��ӡ��־����
		Level m_level_;
		//��־�ع�
		int max_length_;//��󳤶�
		int local_len_;//file ��ǰ����
	};
	//��̬������ʼ��
	const char* Logger::level_[]{
		"DEBUG",
		"INFO",
		"WARN,"
		"ERROR",
		"FATAL",
	};
	Logger* Logger::log_instance_ = nullptr;
}
