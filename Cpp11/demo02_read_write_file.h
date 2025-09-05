#pragma once
#include <fstream>
#include <vector>
#include<iostream>
#include <string>
using namespace std;
//file operate
void writefile() {
    //open
    std::fstream fs("text.txt", ios::out);
    if (!fs.is_open()) {
        cout << "fiile open failed!" << endl;
    }
    //operate
    //1.按行写入
    fs << "hello file stream" << endl;
    //2.按字符写入
    //fs.put('A');
    //char c = 65;
    //while (c <= 90) {
    //    fs.put(c);
    //    c++;
    //}
    ////3.write写入
    //char data[] = "hello world";
    //fs.write(data, sizeof(data));

    //close
    fs.close();
    cout << "finished" << endl;
}

void readfile() {
    //fstream ifs("text.txt", ios::in);
    //if (!ifs.is_open()) {
    //cout << "file open failed!" << endl;
    //}
    ////1.按行读取
    //string line;
    //while (getline(ifs, line)) {
    //    cout << line << endl;
    //}
    ////2.按词读取
    //string word;
    //while (ifs >> word) {
    //    cout << word << endl;
    //}
    ////3.get()读取
    //char c;
    //while (ifs.get(c)) {
    //    cout << c << endl;
    //}
    //4.read()读取二进制的方式读取
    fstream ifs("text.txt", ios::in|ios::binary);
    if (!ifs.is_open()) {
        cout << "file open failed!" << endl;
    }
    //获取文件大小
    ifs.seekg(0, ios::end);//文件指针（文件光标）
    streampos filesize = ifs.tellg();
    if (filesize < 0) {
        cout << "获取文件按大小失败\n";
        ifs.close();
    }
    ifs.seekg(0, ios::beg);
    cout << "fizesize=" << filesize << endl;
    const int maxbuffer = 100;
    char buffer[maxbuffer];
    while (!ifs.eof())
    {
        ifs.read(buffer, sizeof(buffer));
        for (int i = 0; i < ifs.gcount(); i++)
        {
           // cout << hex << static_cast<int>(buffer[i]) << " ";
            cout <<buffer[i]<< " ";
        }
        cout << "-----------" << endl;

    }

    ifs.close();

}

void offset_seek() {
    fstream ifs("a.txt", ios::in | ios::binary);
    if (!ifs.is_open()) {
        cout << "read file failed!" << endl;
    }
   // ifs.seekg(4, ios::beg);
    ifs.seekg(-2, ios::end);
    char c;
    while (ifs.get(c)) {
        cout << c;
    }
    ifs.close();
}

struct Pixel {
    uint8_t red;
    uint8_t greeen;
    uint8_t blue;
};
void read_picture() {
    const char* in_image = "1.jpg";
    //打开图像
    ifstream ifs(in_image, ios::binary);
    if (!ifs.is_open()) {
        cout << "open failed!" << endl;
        return;
    }
    //读取图像尺寸
    ifs.seekg(0, ios::end);
    streampos fileSize = ifs.tellg();
    cout << "fileSize=" << fileSize << endl;
    ifs.seekg(0, ios::beg);
    const int headSize = 2;
    const int channels = 3;
    if (fileSize <= headSize) {
        cout << "图像文件无效" <<in_image<< endl;
        return;
    }

    int imageH = 0;
    int imageW = 0;
    ifs.ignore(headSize);

    //读入像素
    const int pixelDataSize = static_cast<int>(fileSize - ifs.tellg());
    const int pixelCount = pixelDataSize / channels;

    vector<Pixel> pixels(pixelCount);
    ifs.read(reinterpret_cast<char*>(pixels.data()), pixelDataSize);
    ifs.close();
    //将通道数据写入txt
    for (int channel = 0; channel < channels; ++channel)
    {
        string outputFile = "channel_" + to_string(channel + 1) + ".txt";
        ofstream ofs(outputFile);
        if (!ofs.is_open()) {
            cout << "open file failed!" << endl;
            return;
        }
        //写入像素
        for (const auto& pixel : pixels) {
            uint8_t value;
            switch (channel) {
            case 0:
                value = pixel.red;
                break;
            case 1:
                value = pixel.greeen;
                break;
            case 2:
                value = pixel.blue;
                break;
            default:
                cerr << "未知通道" << endl;
                return;
            }
            ofs << static_cast<int>(value) << " ";
        }
        ofs.close();
        cout << "通道" << channel + 1 << "数据已写入文件" << outputFile << endl;
    }

}


