#ifndef TEST_003_CPP
#define TEST_003_CPP

#include "../libs/json.hpp"
#include "test_mock.hpp"
#include <gtest/gtest.h>
#include <opencv2/homebot.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::homebot;
using json = nlohmann::json;

// 格式异常，非 json string
TEST(test_carpet_004, DataValid1) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetBoundingRects("", cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}
// 数据异常，宽高为空
TEST(test_carpet_004, DataValid2) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetBoundingRects("{\"data\": [1,1,1]}",
                                                      cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，宽高格式异常
TEST(test_carpet_004, DataValid3) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetBoundingRects(
      "{\"width\": \"3\", \"height\": 3, \"data\": [1,1,1,1,1,1,1,1,1]}",
      cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，data为空
TEST(test_carpet_004, DataValid4) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetBoundingRects(
      "{\"width\": 10, \"height\": 10}", cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，data格式异常
TEST(test_carpet_004, DataValid5) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetBoundingRects(
      "{\"width\": 3, \"height\": 3,\"data\": [1,1,1,1,1,\"1\",1,1,1]}",
      cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，data格式异常
TEST(test_carpet_004, DataValid6) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetBoundingRects(
      "{\"width\": 3, \"height\": 3, \"data\": [1,1,1,1,1,\"1\",1,1,1]}",
      cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，宽高为空
TEST(test_carpet_004, DataValid7) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetBoundingRects(
      "{\"width\": 0, \"height\": 0, \"data\": []}", cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，地毯原点超过地图
TEST(test_carpet_004, DataValid8) {
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
  string result = cv::homebot::getCarpetBoundingRects(mock.dump(), rect);
  // cout << result << endl;
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，融合原点超过地图
TEST(test_carpet_004, DataValid9) {
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
  string result = cv::homebot::getCarpetBoundingRects(mock.dump(), rect);
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据正常，地图相交
TEST(test_carpet_004, DataValid10) {
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
  string result = cv::homebot::getCarpetBoundingRects(mock.dump(), rect);
  string mock_result = "[{\"boundingBox\":{\"height\":9,\"width\":9,\"x\":91,"
                       "\"y\":0},\"id\":\"31\"}]";
  EXPECT_EQ(result, mock_result);
}

// 数据正常，地毯地图包含融合地图
TEST(test_carpet_004, DataValid11) {
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
  string result = cv::homebot::getCarpetBoundingRects(mock.dump(), rect);
  string mock_result = "[{\"boundingBox\":{\"height\":10,\"width\":10,\"x\":0,"
                       "\"y\":0},\"id\":\"31\"}]";
  EXPECT_EQ(result, mock_result);
}

// 数据正常，融合地图包含地毯地图
TEST(test_carpet_004, DataValid12) {
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
  string result = cv::homebot::getCarpetBoundingRects(mock.dump(), rect);
  string mock_result = "[{\"boundingBox\":{\"height\":10,\"width\":10,\"x\":44,"
                       "\"y\":46},\"id\":\"31\"}]";
  EXPECT_EQ(result, mock_result);
}

// 数据正常，真实数据测试
TEST(test_carpet_004, DataValid13) {
  // Expect two strings not to be equal.
  string mock = test_mock::mock_string();
  // 融合地图原点是左下角
  int fusion_width = 233;
  int fusion_height = 262;
  cv::Rect rect(103, 34, fusion_width, fusion_height);
  string result = cv::homebot::getCarpetBoundingRects(mock, rect);
  string mock_result =
      "[{\"boundingBox\":{\"height\":96,\"width\":60,\"x\":165,\"y\":158},"
      "\"id\":\"34\"},{\"boundingBox\":{\"height\":136,\"width\":76,\"x\":7,"
      "\"y\":114},\"id\":\"32\"},{\"boundingBox\":{\"height\":181,\"width\":"
      "110,\"x\":63,\"y\":68},\"id\":\"31\"},{\"boundingBox\":{\"height\":32,"
      "\"width\":48,\"x\":11,\"y\":118},\"id\":\"37\"},{\"boundingBox\":{"
      "\"height\":34,\"width\":32,\"x\":31,\"y\":71},\"id\":\"39\"},{"
      "\"boundingBox\":{\"height\":46,\"width\":51,\"x\":148,\"y\":25},\"id\":"
      "\"36\"},{\"boundingBox\":{\"height\":61,\"width\":55,\"x\":32,\"y\":7},"
      "\"id\":\"35\"},{\"boundingBox\":{\"height\":47,\"width\":26,\"x\":104,"
      "\"y\":21},\"id\":\"38\"}]";
  EXPECT_EQ(result, mock_result);
}

// 数据正常，真实数据测试
TEST(test_carpet_004, DataValid14) {
  // Expect two strings not to be equal.
  string mock = test_mock::mock_string1();
  // 融合地图原点是左下角
  int fusion_width = 87;
  int fusion_height = 78;
  cv::Rect rect(22, 34, fusion_width, fusion_height);
  string result = cv::homebot::getCarpetBoundingRects(mock, rect);
  string mock_result = "[{\"boundingBox\":{\"height\":19,\"width\":33,\"x\":26,"
                       "\"y\":33},\"id\":\"12\"},{\"boundingBox\":{\"height\":"
                       "20,\"width\":17,\"x\":0,\"y\":16},\"id\":\"11\"}]";
  EXPECT_EQ(result, mock_result);
}

// 数据正常，融合地图包含地毯地图
TEST(test_carpet_004, DataValid15) {
  // Expect two strings not to be equal.
  json mock = json::object();
  int size = 100;                  // 数组大小
  std::vector<int> data(size, 31); // 使用构造函数初始化
  mock["data"] = data;
  mock["width"] = 56;
  mock["height"] = 43;
  mock["resolution"] = 0.05;
  json position = json::object();
  position["x"] = 28 / 0.05;
  position["y"] = -40 / 0.05;
  json origin = json::object();
  origin["position"] = position;
  mock["origin"] = origin;
  // 融合地图原点是左下角
  cv::Rect rect(34, 77, 101, 142);
  string result = cv::homebot::getCarpetBoundingRects(mock.dump(), rect);
  string mock_result = "[]";
  EXPECT_EQ(result, mock_result);
}

#endif