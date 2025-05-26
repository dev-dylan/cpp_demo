#ifndef GZIP_CPP
#define GZIP_CPP
#include <iostream>

namespace GZip {
/// @brief 通过 GZIP 和 Base64 算法压缩数据
/// @param origin_data 原始数据据
/// @return gziped_data 被压缩后的数据
std::string compress(const std::string origin_data);

/// @brief 通过 GZIP 和 Base64 算法解压缩数据
/// @param gziped_data 被压缩后的数据
/// @return origin_data 原始数据
std::string decompress(const std::string gziped_data);
} // namespace GZip

#endif