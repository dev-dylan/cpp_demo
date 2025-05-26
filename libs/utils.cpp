#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include "utils.hpp"

using namespace std;
using namespace std::chrono;

namespace utils_p
{

    string currentTimeString()
    {
        // 获取当前时间
        std::time_t now = std::time(0);

        // 将时间转换为 tm 结构，以便进行格式化
        std::tm *ltm = std::localtime(&now);

        // 创建一个格式化时间的字符串
        char timeString[100];
        std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", ltm);

        // 打印当前时间
        return timeString;
    }

    void currentMillSecond()
    {
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto epoch = now_ms.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
        std::time_t current_time = std::chrono::system_clock::to_time_t(now);
        std::tm *time_info = std::localtime(&current_time);
        std::cout << "当前时间： " << std::put_time(time_info, "%Y-%m-%d %H:%M:%S") << "." << value.count() % 1000 << std::endl;
    }

    long long currentTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto epoch = now_ms.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
        // std::cout << "当前时间戳（毫秒）： " << value.count() << std::endl;
        return value.count();
    }

    std::string readJsonFile(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "无法打开文件： " << filename << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    }

}