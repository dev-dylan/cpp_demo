#ifndef TEST_001_CPP
#define TEST_001_CPP

#include "../libs/json.hpp"
#include <gtest/gtest.h>
#include <opencv2/homebot.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv::homebot;
using json = nlohmann::json;

// 格式异常，非 json string
TEST(test_carpet_001, DataValid1) {
  // Expect two strings not to be equal.
  std::string result =
      cv::homebot::checkCarpetValidation("", cv::Rect(0, 0, 0, 0));
  json res = json::parse(result);
  // Expect equality.
  EXPECT_EQ(res["code"], 1001);
}
// 数据异常，宽高为空
TEST(test_carpet_001, DataValid2) {
  // Expect two strings not to be equal.
  std::string result = cv::homebot::checkCarpetValidation("{\"data\": [1,1,1]}",
                                                          cv::Rect(0, 0, 0, 0));
  json res = json::parse(result);
  // Expect equality.
  EXPECT_EQ(res["code"], 1001);
}

// 数据异常，宽高格式异常
TEST(test_carpet_001, DataValid3) {
  // Expect two strings not to be equal.
  std::string result = cv::homebot::checkCarpetValidation(
      "{\"width\": \"3\", \"height\": 3, \"data\": [1,1,1,1,1,1,1,1,1]}",
      cv::Rect(0, 0, 0, 0));
  json res = json::parse(result);
  // Expect equality.
  EXPECT_EQ(res["code"], 1001);
}

// 数据异常，data为空
TEST(test_carpet_001, DataValid4) {
  // Expect two strings not to be equal.
  std::string result = cv::homebot::checkCarpetValidation(
      "{\"width\": 10, \"height\": 10}", cv::Rect(0, 0, 0, 0));
  json res = json::parse(result);
  // Expect equality.
  EXPECT_EQ(res["code"], 1001);
}

// 数据异常，data格式异常
TEST(test_carpet_001, DataValid5) {
  // Expect two strings not to be equal.
  std::string result = cv::homebot::checkCarpetValidation(
      "{\"width\": 3, \"height\": 3,\"data\": [1,1,1,1,1,\"1\",1,1,1]}",
      cv::Rect(0, 0, 0, 0));
  json res = json::parse(result);
  // Expect equality.
  EXPECT_EQ(res["code"], 1001);
}

// 数据异常，data格式异常
TEST(test_carpet_001, DataValid6) {
  // Expect two strings not to be equal.
  std::string result = cv::homebot::checkCarpetValidation(
      "{\"width\": 3, \"height\": 3, \"data\": [1,1,1,1,1,\"1\",1,1,1]}",
      cv::Rect(0, 0, 0, 0));
  json res = json::parse(result);
  // Expect equality.
  EXPECT_EQ(res["code"], 1001);
}

// 数据异常，宽高为空
TEST(test_carpet_001, DataValid7) {
  // Expect two strings not to be equal.
  std::string result = cv::homebot::checkCarpetValidation(
      "{\"width\": 0, \"height\": 0, \"data\": []}", cv::Rect(0, 0, 0, 0));
  json res = json::parse(result);
  // Expect equality.
  EXPECT_EQ(res["code"], 1001);
}

// 数据异常，原点无法对齐
TEST(test_carpet_001, DataValid8) {
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
  std::string result = cv::homebot::checkCarpetValidation(mock.dump(), rect);
  // cout << result << endl;
  json res = json::parse(result);
  // Expect equality.
  EXPECT_EQ(res["code"], 1002);
}

// 数据异常，原点无法对齐
TEST(test_carpet_001, DataValid9) {
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
  std::string result = cv::homebot::checkCarpetValidation(mock.dump(), rect);
  json res = json::parse(result);
  // Expect equality.
  EXPECT_EQ(res["code"], 1002);
}

// 数据异常，原点无法对齐
TEST(test_carpet_001, DataValid10) {
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
  std::string result = cv::homebot::checkCarpetValidation(mock.dump(), rect);
  json res = json::parse(result);
  // Expect equality.
  EXPECT_EQ(res["code"], 1000);
}

// 数据异常，原点无法对齐
// TEST(test_carpet_001, DataValid11)
// {
//   // Expect two strings not to be equal.
//   cv::Rect rect(99, 99, 100, 100);
//   std::string result = cv::homebot::checkCarpetValidation(NULL, rect);
//   cout << "sss" << result << endl;
//   json res = json::parse(result);
//   // Expect equality.
//   EXPECT_EQ(res["code"], 1000);
// }

#endif