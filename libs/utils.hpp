#ifndef UTILS_CPP
#define UTILS_CPP

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>

#endif // HOMEBOT_CPP

using namespace std;

namespace utils_p
{
    string currentTimeString();
    void currentMillSecond();
    long long currentTimestamp();
    std::string readJsonFile(const std::string &filename);
}