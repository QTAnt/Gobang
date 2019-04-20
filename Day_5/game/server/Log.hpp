#pragma once

#include <iostream>
#include <vector>
#include <string>

#define INFO 0
#define WARNING 1
#define FATAL 2

using namespace std;

vector g_error_level={
    "INFO",
    "WARNING",
    "FATAL",//最后一行可以不加‘,’一般习惯加上
};

void Log(int level, string message, string file, int line) //级别(宏)、信息、文件位置、行
{
    cout << "[" << g_error_level[level] << "][" << message << "]:" << file << ":" << line << endl;
}

#define LOG(level, message) Log(level, message, __FILE__, __LINE__)
