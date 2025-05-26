
#ifndef TEST_002_CPP
#define TEST_002_CPP

#include "../libs/json.hpp"
#include "../libs/utils.hpp"
#include "../src/map.hpp"
#include "test_mock.hpp"
#include <gtest/gtest.h>
#include <opencv2/homebot.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace utils_p;
using namespace cv::homebot;
using json = nlohmann::json;

cv::Mat carpetMat() {
  string imagePath =
      "/Users/admin/Documents/cpp_demo/resource/homebot_carpet.png";
  Mat temp = imread(imagePath, IMREAD_UNCHANGED);
  Mat carpet;
  cvtColor(temp, carpet, COLOR_BGRA2RGBA);
  return carpet;
}

bool test002_areMatsEqual(const cv::Mat &mat1, const cv::Mat &mat2) {
  if (mat1.size() != mat2.size() || mat1.type() != mat2.type()) {
    // 大小或类型不匹配，直接返回 false
    return false;
  }
  std::vector<cv::Mat> channels1;
  std::vector<cv::Mat> channels2;
  cv::split(mat1, channels1);
  cv::split(mat2, channels2);
  for (int i = 0; i < channels1.size(); ++i) {
    cv::Mat diff;
    cv::compare(channels1[i], channels2[i], diff, cv::CMP_NE);
    if (cv::countNonZero(diff) != 0) {
      return false;
    }
  }
  return true;
}
// 格式异常，非 json string
TEST(test_carpet_002, DataValid1) {
  // Expect two strings not to be equal.
  Mat carpet = carpetMat();
  Mat result =
      cv::homebot::drawCarpetMap("", cv::Rect(0, 0, 0, 0), {carpet}, "", true);

  // Expect equality.
  EXPECT_EQ(result.rows, 0);
  EXPECT_EQ(result.cols, 0);
}
// 数据异常，宽高为空
TEST(test_carpet_002, DataValid2) {
  // Expect two strings not to be equal.
  Mat carpet = carpetMat();
  Mat result = cv::homebot::drawCarpetMap(
      "{\"data\": [1,1,1]}", cv::Rect(0, 0, 0, 0), {carpet}, "", true);
  // Expect equality.
  EXPECT_EQ(result.rows, 0);
  EXPECT_EQ(result.cols, 0);
}

// 数据异常，宽高格式异常
TEST(test_carpet_002, DataValid3) {
  // Expect two strings not to be equal.
  Mat carpet = carpetMat();
  Mat result = cv::homebot::drawCarpetMap(
      "{\"width\": \"3\", \"height\": 3, \"data\": [1,1,1,1,1,1,1,1,1]}",
      cv::Rect(0, 0, 0, 0), {carpet}, "", true);

  // Expect equality.
  EXPECT_EQ(result.rows, 0);
  EXPECT_EQ(result.cols, 0);
}

// 数据异常，data为空
TEST(test_carpet_002, DataValid4) {
  // Expect two strings not to be equal.
  Mat carpet = carpetMat();
  Mat result =
      cv::homebot::drawCarpetMap("{\"width\": 10, \"height\": 10}",
                                 cv::Rect(0, 0, 0, 0), {carpet}, "", true);

  // Expect equality.
  EXPECT_EQ(result.rows, 0);
  EXPECT_EQ(result.cols, 0);
}

// 数据异常，data格式异常
TEST(test_carpet_002, DataValid5) {
  // Expect two strings not to be equal.
  Mat carpet = carpetMat();
  Mat result = cv::homebot::drawCarpetMap(
      "{\"width\": 3, \"height\": 3,\"data\": [1,1,1,1,1,\"1\",1,1,1]}",
      cv::Rect(0, 0, 0, 0), {carpet}, "", true);

  // Expect equality.
  EXPECT_EQ(result.rows, 0);
  EXPECT_EQ(result.cols, 0);
}

// 数据异常，data格式异常
TEST(test_carpet_002, DataValid6) {
  // Expect two strings not to be equal.
  Mat carpet = carpetMat();
  Mat result = cv::homebot::drawCarpetMap(
      "{\"width\": 3, \"height\": 3, \"data\": [1,1,1,1,1,\"1\",1,1,1]}",
      cv::Rect(0, 0, 0, 0), {carpet}, "", true);

  // Expect equality.
  EXPECT_EQ(result.rows, 0);
  EXPECT_EQ(result.cols, 0);
}

// 数据异常，宽高为空
TEST(test_carpet_002, DataValid7) {
  // Expect two strings not to be equal.
  Mat carpet = carpetMat();
  Mat result =
      cv::homebot::drawCarpetMap("{\"width\": 0, \"height\": 0, \"data\": []}",
                                 cv::Rect(0, 0, 0, 0), {carpet}, "", true);

  // Expect equality.
  EXPECT_EQ(result.rows, 0);
  EXPECT_EQ(result.cols, 0);
}

// 数据异常，地毯原点超过地图
TEST(test_carpet_002, DataValid8) {
  // Expect two strings not to be equal.
  json mock = json::object();
  int size = 100;                  // 数组大小
  std::vector<int> data(size, 31); // 使用构造函数初始化
  mock["data"] = data;
  mock["width"] = 10;
  mock["height"] = 10;
  mock["resolution"] = 0.05;
  json position = json::object();
  position["x"] = 0.6;
  position["y"] = 0.6;
  json origin = json::object();
  origin["position"] = position;
  mock["origin"] = origin;
  // 融合地图原点是左下角
  cv::Rect rect(0.0, 0.0, 100, 100);
  Mat carpet = carpetMat();
  Mat result =
      cv::homebot::drawCarpetMap(mock.dump(), rect, {carpet}, "", true);
  // cout << result << endl;

  // Expect equality.
  EXPECT_EQ(result.rows, 0);
  EXPECT_EQ(result.cols, 0);
}

// 数据异常，融合原点超过地图
TEST(test_carpet_002, DataValid9) {
  // Expect two strings not to be equal.
  json mock = json::object();
  int size = 100;                  // 数组大小
  std::vector<int> data(size, 31); // 使用构造函数初始化
  mock["data"] = data;
  mock["width"] = 10;
  mock["height"] = 10;
  mock["resolution"] = 0.05;
  json position = json::object();
  position["x"] = 0.4;
  position["y"] = 0.4;
  json origin = json::object();
  origin["position"] = position;
  mock["origin"] = origin;
  // 融合地图原点是左下角
  cv::Rect rect(101, 101, 100, 100);
  Mat carpet = carpetMat();
  Mat result =
      cv::homebot::drawCarpetMap(mock.dump(), rect, {carpet}, "", true);

  // Expect equality.
  EXPECT_EQ(result.rows, 0);
  EXPECT_EQ(result.cols, 0);
}

// 数据正常，地图相交
TEST(test_carpet_002, DataValid10) {
  // Expect two strings not to be equal.
  json mock = json::object();
  int size = 100;                  // 数组大小
  std::vector<int> data(size, 31); // 使用构造函数初始化
  mock["data"] = data;
  mock["width"] = 10;
  mock["height"] = 10;
  mock["resolution"] = 0.05;
  json position = json::object();
  position["x"] = 0.4;
  position["y"] = 0.4;
  json origin = json::object();
  origin["position"] = position;
  mock["origin"] = origin;
  // 融合地图原点是左下角
  cv::Rect rect(99, 99, 100, 100);
  Mat carpet = carpetMat();
  Mat result = cv::homebot::drawCarpetMap(
      mock.dump(), rect, {carpet},
      "[{\"carpetEvadeId\":\"31\",\"carpetEvadeSwitch\":0}]", true);
  // Expect equality.
  // 生成的 Mat 会在融合地图的基础上扩大两倍
  EXPECT_EQ(result.rows, 100 * 2);
  EXPECT_EQ(result.cols, 100 * 2);

  string imagePath =
      "/Users/admin/Documents/cpp_demo/tests/test_002/DataValid10.png";
  Mat temp = imread(imagePath, IMREAD_UNCHANGED);

  bool isEqual = test002_areMatsEqual(temp, result);
  ASSERT_TRUE(isEqual); // 使用 ASSERT_TRUE 断言
}

// 数据正常，地毯地图包含融合地图
TEST(test_carpet_002, DataValid11) {
  // Expect two strings not to be equal.
  json mock = json::object();
  int size = 100 * 100;            // 数组大小
  std::vector<int> data(size, 31); // 使用构造函数初始化
  mock["data"] = data;
  mock["width"] = 100;
  mock["height"] = 100;
  mock["resolution"] = 0.05;
  json position = json::object();
  position["x"] = 2.5;
  position["y"] = 2.5;
  json origin = json::object();
  origin["position"] = position;
  mock["origin"] = origin;
  // 融合地图原点是左下角
  cv::Rect rect(8, 8, 10, 10);
  Mat carpet = carpetMat();
  Mat result = cv::homebot::drawCarpetMap(
      mock.dump(), rect, {carpet},
      "[{\"carpetEvadeId\":\"31\",\"carpetEvadeSwitch\":0}]", true);
  // Expect equality.
  // 生成的 Mat 会在融合地图的基础上扩大两倍
  EXPECT_EQ(result.rows, 10 * 2);
  EXPECT_EQ(result.cols, 10 * 2);

  string imagePath =
      "/Users/admin/Documents/cpp_demo/tests/test_002/DataValid11.png";
  Mat temp = imread(imagePath, IMREAD_UNCHANGED);
  // 和本地保存的图片做对比，检查是否异常
  bool isEqual = test002_areMatsEqual(temp, result);
  ASSERT_TRUE(isEqual); // 使用 ASSERT_TRUE 断言
}

// 数据正常，融合地图包含地毯地图
TEST(test_carpet_002, DataValid12) {
  // Expect two strings not to be equal.
  json mock = json::object();
  int size = 100;                  // 数组大小
  std::vector<int> data(size, 31); // 使用构造函数初始化
  mock["data"] = data;
  mock["width"] = 10;
  mock["height"] = 10;
  mock["resolution"] = 0.05;
  json position = json::object();
  position["x"] = 0.3;
  position["y"] = 0.3;
  json origin = json::object();
  origin["position"] = position;
  mock["origin"] = origin;
  // 融合地图原点是左下角
  cv::Rect rect(50, 50, 100, 100);
  Mat carpet = carpetMat();
  Mat result = cv::homebot::drawCarpetMap(
      mock.dump(), rect, {carpet},
      "[{\"carpetEvadeId\":\"31\",\"carpetEvadeSwitch\":0}]", true);
  // Expect equality.
  // 生成的 Mat 会在融合地图的基础上扩大两倍
  EXPECT_EQ(result.rows, 100 * 2);
  EXPECT_EQ(result.cols, 100 * 2);

  string imagePath =
      "/Users/admin/Documents/cpp_demo/tests/test_002/DataValid12.png";
  Mat temp = imread(imagePath, IMREAD_UNCHANGED);
  // 和本地保存的图片做对比，检查是否异常
  bool isEqual = test002_areMatsEqual(temp, result);
  ASSERT_TRUE(isEqual); // 使用 ASSERT_TRUE 断言
}

// 数据正常，真实数据测试
TEST(test_carpet_002, DataValid13) {
  // Expect two strings not to be equal.
  string mock = test_mock::mock_string();
  // 融合地图原点是左下角
  int fusion_width = 233;
  int fusion_height = 262;
  cv::Rect rect(103, 34, fusion_width, fusion_height);
  Mat carpet = carpetMat();
  Mat result =
      cv::homebot::drawCarpetMap(mock, rect, {carpet, carpet}, "[]", true);
  // Expect equality.
  // 生成的 Mat 会在融合地图的基础上扩大两倍
  EXPECT_EQ(result.rows, fusion_height * 2);
  EXPECT_EQ(result.cols, fusion_width * 2);

  string imagePath =
      "/Users/admin/Documents/cpp_demo/tests/test_002/DataValid13.png";
  Mat temp = imread(imagePath, IMREAD_UNCHANGED);

  // 和本地保存的图片做对比，检查是否异常
  bool isEqual = test002_areMatsEqual(temp, result);
  ASSERT_TRUE(isEqual); // 使用 ASSERT_TRUE 断言
}

// 数据正常，融合地图包含地毯地图，长宽不一致
TEST(test_carpet_002, DataValid14) {
  // Expect two strings not to be equal.
  json mock = json::object();
  int width = 13;
  int height = 18;
  int size = width * height;       // 数组大小
  std::vector<int> data(size, 31); // 使用构造函数初始化
  mock["data"] = data;
  mock["width"] = width;
  mock["height"] = height;
  mock["resolution"] = 0.05;
  json position = json::object();
  position["x"] = 0.3;
  position["y"] = 0.15;
  json origin = json::object();
  origin["position"] = position;
  mock["origin"] = origin;
  // 融合地图原点是左下角
  int row = 134;
  int col = 110;
  cv::Rect rect(32, 45, col, row);
  Mat carpet = carpetMat();
  string carpet_options = "[{\"carpetEvadeId\":\"31\",\"carpetEvadeSwitch\":0},"
                          "{\"carpetEvadeId\":\"32\",\"carpetEvadeSwitch\":0},{"
                          "\"carpetEvadeId\":\"33\",\"carpetEvadeSwitch\":0},{"
                          "\"carpetEvadeId\":\"34\",\"carpetEvadeSwitch\":0},{"
                          "\"carpetEvadeId\":\"35\",\"carpetEvadeSwitch\":0},{"
                          "\"carpetEvadeId\":\"36\",\"carpetEvadeSwitch\":0},{"
                          "\"carpetEvadeId\":\"37\",\"carpetEvadeSwitch\":0},{"
                          "\"carpetEvadeId\":\"38\",\"carpetEvadeSwitch\":0},{"
                          "\"carpetEvadeId\":\"39\",\"carpetEvadeSwitch\":0}]";
  Mat result = cv::homebot::drawCarpetMap(mock.dump(), rect, {carpet},
                                          carpet_options, true);
  // Expect equality.
  // 生成的 Mat 会在融合地图的基础上扩大两倍
  EXPECT_EQ(result.rows, row * 2);
  EXPECT_EQ(result.cols, col * 2);

  string imagePath =
      "/Users/admin/Documents/cpp_demo/tests/test_002/DataValid14.png";
  Mat temp = imread(imagePath, IMREAD_UNCHANGED);
  // 和本地保存的图片做对比，检查是否异常
  bool isEqual = test002_areMatsEqual(temp, result);
  ASSERT_TRUE(isEqual); // 使用 ASSERT_TRUE 断言
}

TEST(test_carpet_002, DataValid15) {
  string json_result = utils_p::readJsonFile(
      "/Users/admin/Documents/cpp_demo/tests/test_002/DataValid15.json");
  Mat carpet = carpetMat();
  cv::Rect rect(24, 30, 84, 75);
  Mat result = cv::homebot::drawCarpetMap(
      json_result, rect, {carpet},
      "[{\"carpetEvadeId\":\"13\",\"carpetEvadeSwitch\":0}]", true);
  string imagePath =
      "/Users/admin/Documents/cpp_demo/tests/test_002/DataValid15.png";
  Mat temp = imread(imagePath, IMREAD_UNCHANGED);
  // 和本地保存的图片做对比，检查是否异常
  bool isEqual = test002_areMatsEqual(temp, result);
  ASSERT_TRUE(isEqual); // 使用 ASSERT_TRUE 断言
}

TEST(test_carpet_002, DataValid16) {
  string json_result = utils_p::readJsonFile(
      "/Users/admin/Documents/cpp_demo/tests/test_002/DataValid15.json");
  Mat carpet = carpetMat();
  cv::Rect rect(24, 30, 84, 75);
  Mat result = cv::homebot::drawCarpetMap(
      json_result, rect, {carpet},
      "{}", true);
  string imagePath =
      "/Users/admin/Documents/cpp_demo/tests/test_002/DataValid15.png";
  Mat temp = imread(imagePath, IMREAD_UNCHANGED);
  // 和本地保存的图片做对比，检查是否异常
  bool isEqual = test002_areMatsEqual(temp, result);
  ASSERT_TRUE(isEqual); // 使用 ASSERT_TRUE 断言
}

#endif