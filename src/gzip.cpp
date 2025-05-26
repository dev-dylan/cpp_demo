#include "../libs/base64.h"
#include "../libs/utils.hpp"
#include <iostream>
#include <zlib.h>

// ChatGPT 方案
namespace GZip {
/**
 * 使用gzip算法对输入的字符串进行解码。
 *
 * @param input 需要被解码的gzip压缩过的字符串。
 * @return 如果解码成功，返回解码后的字符串；如果解码失败，返回空字符串。
 */
std::string gzip_decode(const std::string &input) {
  /**
   * 这段代码的主要功能是解压缩输入的字符串。
  首先，定义了一个z_stream类型的变量strm，并初始化其成员变量。其中，zalloc和zfree被设置为Z_NULL，表示使用默认的内存分配函数；opaque被设置为Z_NULL，表示不传递任何应用特定的数据；avail_in被设置为input.size()，表示输入数据的大小；next_in被设置为input.data()，表示输入数据的起始地址。
  然后，调用inflateInit2函数初始化解压缩器，参数15 +
  32表示窗口大小和哈希表大小。如果返回值不是Z_OK，说明初始化失败，直接返回空字符串。
  接着，定义一个输出字符串output和一个缓冲区buffer，然后在一个do-while循环中进行解压缩操作。每次循环开始时，设置strm.avail_out为buffer的大小，设置strm.next_out为buffer的地址，然后调用inflate函数进行解压缩。如果解压缩成功，将解压缩的数据追加到output中。循环直到inflate函数返回值不为Z_OK。
  最后，调用inflateEnd函数结束解压缩，并检查返回值。如果返回值不是Z_STREAM_END，说明解压缩过程出现错误，直接返回空字符串。否则，返回解压缩后的字符串output。
  */
  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  strm.avail_in = input.size();
  strm.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input.data()));
  int ret = inflateInit2(&strm, 15 + 32);
  if (ret != Z_OK)
    return "";
  std::string output;
  char buffer[32768];
  do {
    strm.avail_out = sizeof(buffer);
    strm.next_out = reinterpret_cast<Bytef *>(buffer);
    ret = inflate(&strm, Z_NO_FLUSH);
    if (output.size() < strm.total_out) {
      output.append(buffer, strm.total_out - output.size());
    }
  } while (ret == Z_OK);
  inflateEnd(&strm);
  if (ret != Z_STREAM_END) {
    return "";
  }
  return output;
}

/**
 * 使用gzip算法对输入的字符串进行编码。
 *
 * @param input 需要编码码的原始字符串。
 * @return 如果编码成功，返回编码后的字符串；如果解码失败，返回空字符串。
 */
std::string gzip_encode(const std::string &data) {
  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;
  deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 31, 8,
               Z_DEFAULT_STRATEGY);
  stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(data.data()));
  stream.avail_in = data.size();
  const int CHUNK_SIZE = 1024;
  std::vector<char> buffer(CHUNK_SIZE);
  std::string compressedData;
  do {
    stream.next_out = reinterpret_cast<Bytef *>(buffer.data());
    stream.avail_out = buffer.size();
    deflate(&stream, Z_FINISH);
    compressedData.append(buffer.data(), buffer.size() - stream.avail_out);
  } while (stream.avail_out == 0);
  deflateEnd(&stream);
  return compressedData;
}
std::string compress(const std::string origin_data) {
  // 先把原始数据做 GZIP 压缩处理
  std::string gzip_encoded = gzip_encode(origin_data);
  // 再把 GZIP 压缩数据做 base64 编码
  std::string result = base64_encode(gzip_encoded);
  return result;
}
std::string decompress(const std::string gziped_data) {
  std::string copy(gziped_data);
  // 移除压缩数据中的 \r 换行符
  copy.erase(std::remove(copy.begin(), copy.end(), '\r'), copy.end());
  // 移除压缩数据中的 \n 换行符
  copy.erase(std::remove(copy.begin(), copy.end(), '\n'), copy.end());
  // base64 解码, remove_linebreaks 参数无法移除 \r，因此都手动移除换行符
  std::string decoded_base64 = base64_decode(copy);
  // gzip 解码
  std::string decoded_gzip = gzip_decode(decoded_base64);
  return decoded_gzip;
}
} // namespace GZip