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
TEST(test_carpet_003, DataValid1) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetContours("", cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}
// 数据异常，宽高为空
TEST(test_carpet_003, DataValid2) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetContours("{\"data\": [1,1,1]}",
                                                 cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，宽高格式异常
TEST(test_carpet_003, DataValid3) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetContours(
      "{\"width\": \"3\", \"height\": 3, \"data\": [1,1,1,1,1,1,1,1,1]}",
      cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，data为空
TEST(test_carpet_003, DataValid4) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetContours(
      "{\"width\": 10, \"height\": 10}", cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，data格式异常
TEST(test_carpet_003, DataValid5) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetContours(
      "{\"width\": 3, \"height\": 3,\"data\": [1,1,1,1,1,\"1\",1,1,1]}",
      cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，data格式异常
TEST(test_carpet_003, DataValid6) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetContours(
      "{\"width\": 3, \"height\": 3, \"data\": [1,1,1,1,1,\"1\",1,1,1]}",
      cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，宽高为空
TEST(test_carpet_003, DataValid7) {
  // Expect two strings not to be equal.
  string result = cv::homebot::getCarpetContours(
      "{\"width\": 0, \"height\": 0, \"data\": []}", cv::Rect(0, 0, 0, 0));
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，地毯原点超过地图
TEST(test_carpet_003, DataValid8) {
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
  string result = cv::homebot::getCarpetContours(mock.dump(), rect);
  // cout << result << endl;
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据异常，融合原点超过地图
TEST(test_carpet_003, DataValid9) {
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
  string result = cv::homebot::getCarpetContours(mock.dump(), rect);
  // Expect equality.
  EXPECT_EQ(result, "[]");
}

// 数据正常，地图相交
TEST(test_carpet_003, DataValid10) {
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
  string result = cv::homebot::getCarpetContours(mock.dump(), rect);
  string mock_result =
      "[{\"id\":31,\"points\":[[{\"x\":91,\"y\":0},{\"x\":91,\"y\":8},{\"x\":"
      "99,\"y\":8},{\"x\":99,\"y\":0}]],\"type\":\"carpet\"}]";
  EXPECT_EQ(result, mock_result);
}

// 数据正常，地毯地图包含融合地图
TEST(test_carpet_003, DataValid11) {
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
  string result = cv::homebot::getCarpetContours(mock.dump(), rect);
  string mock_result =
      "[{\"id\":31,\"points\":[[{\"x\":0,\"y\":0},{\"x\":0,\"y\":9},{\"x\":9,"
      "\"y\":9},{\"x\":9,\"y\":0}]],\"type\":\"carpet\"}]";
  EXPECT_EQ(result, mock_result);
}

// 数据正常，融合地图包含地毯地图
TEST(test_carpet_003, DataValid12) {
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
  string result = cv::homebot::getCarpetContours(mock.dump(), rect);
  string mock_result =
      "[{\"id\":31,\"points\":[[{\"x\":44,\"y\":46},{\"x\":44,\"y\":55},{\"x\":"
      "53,\"y\":55},{\"x\":53,\"y\":46}]],\"type\":\"carpet\"}]";
  EXPECT_EQ(result, mock_result);
}

// 数据正常，真实数据测试
TEST(test_carpet_003, DataValid13) {
  // Expect two strings not to be equal.
  string mock = test_mock::mock_string();
  // 融合地图原点是左下角
  int fusion_width = 233;
  int fusion_height = 262;
  cv::Rect rect(103, 34, fusion_width, fusion_height);
  string result = cv::homebot::getCarpetContours(mock, rect);
  string mock_result =
      "[{\"id\":34,\"points\":[[{\"x\":165,\"y\":158},{\"x\":165,\"y\":175},{"
      "\"x\":169,\"y\":175},{\"x\":171,\"y\":177},{\"x\":171,\"y\":211},{\"x\":"
      "169,\"y\":213},{\"x\":169,\"y\":247},{\"x\":173,\"y\":247},{\"x\":175,"
      "\"y\":249},{\"x\":175,\"y\":253},{\"x\":222,\"y\":253},{\"x\":222,\"y\":"
      "219},{\"x\":224,\"y\":217},{\"x\":224,\"y\":171},{\"x\":200,\"y\":171},{"
      "\"x\":198,\"y\":169},{\"x\":198,\"y\":163},{\"x\":180,\"y\":163},{\"x\":"
      "178,\"y\":161},{\"x\":169,\"y\":161},{\"x\":167,\"y\":159},{\"x\":167,"
      "\"y\":158}]],\"type\":\"carpet\"},{\"id\":32,\"points\":[[{\"x\":63,"
      "\"y\":114},{\"x\":63,\"y\":151},{\"x\":61,\"y\":153},{\"x\":47,\"y\":"
      "153},{\"x\":45,\"y\":151},{\"x\":45,\"y\":150},{\"x\":32,\"y\":150},{"
      "\"x\":30,\"y\":152},{\"x\":29,\"y\":152},{\"x\":29,\"y\":161},{\"x\":27,"
      "\"y\":163},{\"x\":10,\"y\":163},{\"x\":10,\"y\":177},{\"x\":13,\"y\":"
      "177},{\"x\":15,\"y\":179},{\"x\":15,\"y\":184},{\"x\":8,\"y\":191},{"
      "\"x\":8,\"y\":228},{\"x\":7,\"y\":229},{\"x\":7,\"y\":247},{\"x\":16,"
      "\"y\":247},{\"x\":18,\"y\":245},{\"x\":20,\"y\":245},{\"x\":22,\"y\":"
      "247},{\"x\":59,\"y\":247},{\"x\":61,\"y\":249},{\"x\":74,\"y\":249},{"
      "\"x\":78,\"y\":245},{\"x\":78,\"y\":215},{\"x\":80,\"y\":213},{\"x\":80,"
      "\"y\":161},{\"x\":82,\"y\":159},{\"x\":82,\"y\":136},{\"x\":79,\"y\":"
      "136},{\"x\":77,\"y\":134},{\"x\":77,\"y\":115},{\"x\":78,\"y\":114}]],"
      "\"type\":\"carpet\"},{\"id\":31,\"points\":[[{\"x\":69,\"y\":68},{\"x\":"
      "69,\"y\":69},{\"x\":67,\"y\":71},{\"x\":66,\"y\":71},{\"x\":66,\"y\":87}"
      ",{\"x\":64,\"y\":89},{\"x\":63,\"y\":89},{\"x\":63,\"y\":100},{\"x\":65,"
      "\"y\":102},{\"x\":65,\"y\":105},{\"x\":63,\"y\":107},{\"x\":63,\"y\":"
      "113},{\"x\":87,\"y\":113},{\"x\":89,\"y\":115},{\"x\":89,\"y\":116},{"
      "\"x\":93,\"y\":116},{\"x\":95,\"y\":118},{\"x\":95,\"y\":128},{\"x\":93,"
      "\"y\":130},{\"x\":89,\"y\":130},{\"x\":89,\"y\":159},{\"x\":94,\"y\":"
      "159},{\"x\":96,\"y\":161},{\"x\":96,\"y\":165},{\"x\":94,\"y\":167},{"
      "\"x\":89,\"y\":167},{\"x\":89,\"y\":177},{\"x\":87,\"y\":179},{\"x\":87,"
      "\"y\":212},{\"x\":91,\"y\":212},{\"x\":93,\"y\":214},{\"x\":93,\"y\":"
      "223},{\"x\":91,\"y\":225},{\"x\":91,\"y\":231},{\"x\":89,\"y\":233},{"
      "\"x\":86,\"y\":233},{\"x\":86,\"y\":239},{\"x\":93,\"y\":239},{\"x\":95,"
      "\"y\":241},{\"x\":95,\"y\":246},{\"x\":127,\"y\":246},{\"x\":129,\"y\":"
      "248},{\"x\":162,\"y\":248},{\"x\":162,\"y\":215},{\"x\":164,\"y\":213},{"
      "\"x\":164,\"y\":177},{\"x\":165,\"y\":176},{\"x\":164,\"y\":175},{\"x\":"
      "164,\"y\":158},{\"x\":161,\"y\":158},{\"x\":159,\"y\":156},{\"x\":159,"
      "\"y\":148},{\"x\":166,\"y\":141},{\"x\":166,\"y\":125},{\"x\":163,\"y\":"
      "125},{\"x\":161,\"y\":123},{\"x\":161,\"y\":112},{\"x\":163,\"y\":110},{"
      "\"x\":167,\"y\":110},{\"x\":167,\"y\":108},{\"x\":169,\"y\":106},{\"x\":"
      "172,\"y\":106},{\"x\":172,\"y\":88},{\"x\":168,\"y\":84},{\"x\":160,"
      "\"y\":84},{\"x\":158,\"y\":82},{\"x\":158,\"y\":76},{\"x\":160,\"y\":74}"
      ",{\"x\":162,\"y\":74},{\"x\":162,\"y\":71},{\"x\":149,\"y\":71},{\"x\":"
      "149,\"y\":72},{\"x\":147,\"y\":74},{\"x\":138,\"y\":74},{\"x\":138,"
      "\"y\":76},{\"x\":136,\"y\":78},{\"x\":125,\"y\":78},{\"x\":123,\"y\":76}"
      ",{\"x\":123,\"y\":73},{\"x\":125,\"y\":71},{\"x\":127,\"y\":71},{\"x\":"
      "127,\"y\":68},{\"x\":108,\"y\":68},{\"x\":108,\"y\":70},{\"x\":106,"
      "\"y\":72},{\"x\":83,\"y\":72},{\"x\":81,\"y\":70},{\"x\":81,\"y\":68}]],"
      "\"type\":\"carpet\"},{\"id\":37,\"points\":[[{\"x\":11,\"y\":118},{"
      "\"x\":11,\"y\":147},{\"x\":23,\"y\":147},{\"x\":23,\"y\":141},{\"x\":25,"
      "\"y\":139},{\"x\":27,\"y\":139},{\"x\":29,\"y\":141},{\"x\":29,\"y\":"
      "144},{\"x\":30,\"y\":144},{\"x\":32,\"y\":146},{\"x\":32,\"y\":149},{"
      "\"x\":46,\"y\":149},{\"x\":47,\"y\":148},{\"x\":51,\"y\":148},{\"x\":51,"
      "\"y\":139},{\"x\":53,\"y\":137},{\"x\":58,\"y\":137},{\"x\":58,\"y\":"
      "119},{\"x\":42,\"y\":119},{\"x\":42,\"y\":129},{\"x\":40,\"y\":131},{"
      "\"x\":37,\"y\":131},{\"x\":35,\"y\":129},{\"x\":35,\"y\":118}]],"
      "\"type\":\"carpet\"},{\"id\":39,\"points\":[[{\"x\":31,\"y\":71},{\"x\":"
      "31,\"y\":101},{\"x\":34,\"y\":104},{\"x\":59,\"y\":104},{\"x\":59,\"y\":"
      "102},{\"x\":61,\"y\":100},{\"x\":62,\"y\":100},{\"x\":62,\"y\":89},{"
      "\"x\":60,\"y\":87},{\"x\":60,\"y\":83},{\"x\":55,\"y\":83},{\"x\":49,"
      "\"y\":77},{\"x\":49,\"y\":71}]],\"type\":\"carpet\"},{\"id\":36,"
      "\"points\":[[{\"x\":163,\"y\":25},{\"x\":163,\"y\":44},{\"x\":161,\"y\":"
      "46},{\"x\":148,\"y\":46},{\"x\":148,\"y\":51},{\"x\":149,\"y\":51},{"
      "\"x\":151,\"y\":53},{\"x\":151,\"y\":57},{\"x\":149,\"y\":59},{\"x\":"
      "149,\"y\":70},{\"x\":162,\"y\":70},{\"x\":164,\"y\":68},{\"x\":174,"
      "\"y\":68},{\"x\":175,\"y\":69},{\"x\":186,\"y\":69},{\"x\":186,\"y\":65}"
      ",{\"x\":188,\"y\":63},{\"x\":195,\"y\":63},{\"x\":195,\"y\":33},{\"x\":"
      "197,\"y\":31},{\"x\":198,\"y\":31},{\"x\":198,\"y\":27},{\"x\":192,"
      "\"y\":27},{\"x\":192,\"y\":28},{\"x\":190,\"y\":30},{\"x\":189,\"y\":30}"
      ",{\"x\":187,\"y\":28},{\"x\":187,\"y\":27},{\"x\":174,\"y\":27},{\"x\":"
      "172,\"y\":25}]],\"type\":\"carpet\"},{\"id\":35,\"points\":[[{\"x\":47,"
      "\"y\":7},{\"x\":47,\"y\":8},{\"x\":45,\"y\":10},{\"x\":34,\"y\":10},{"
      "\"x\":34,\"y\":18},{\"x\":37,\"y\":21},{\"x\":32,\"y\":26},{\"x\":32,"
      "\"y\":64},{\"x\":58,\"y\":64},{\"x\":58,\"y\":57},{\"x\":60,\"y\":55},{"
      "\"x\":63,\"y\":55},{\"x\":65,\"y\":57},{\"x\":65,\"y\":64},{\"x\":67,"
      "\"y\":64},{\"x\":69,\"y\":66},{\"x\":69,\"y\":67},{\"x\":81,\"y\":67},{"
      "\"x\":81,\"y\":51},{\"x\":83,\"y\":49},{\"x\":86,\"y\":49},{\"x\":86,"
      "\"y\":10},{\"x\":79,\"y\":10},{\"x\":77,\"y\":8},{\"x\":66,\"y\":8},{"
      "\"x\":65,\"y\":7}]],\"type\":\"carpet\"},{\"id\":38,\"points\":[[{\"x\":"
      "106,\"y\":21},{\"x\":106,\"y\":27},{\"x\":104,\"y\":29},{\"x\":104,"
      "\"y\":60},{\"x\":106,\"y\":60},{\"x\":108,\"y\":62},{\"x\":108,\"y\":67}"
      ",{\"x\":127,\"y\":67},{\"x\":127,\"y\":54},{\"x\":129,\"y\":52},{\"x\":"
      "129,\"y\":21}]],\"type\":\"carpet\"}]";
  EXPECT_EQ(result, mock_result);
}

// 数据正常，真实数据测试
TEST(test_carpet_003, DataValid14) {
  // Expect two strings not to be equal.
  string mock = test_mock::mock_string1();
  // 融合地图原点是左下角
  int fusion_width = 87;
  int fusion_height = 78;
  cv::Rect rect(22, 34, fusion_width, fusion_height);
  string result = cv::homebot::getCarpetContours(mock, rect);
  string mock_result =
      "[{\"id\":12,\"points\":[[{\"x\":50,\"y\":33},{\"x\":49,\"y\":34},{\"x\":"
      "29,\"y\":34},{\"x\":28,\"y\":35},{\"x\":27,\"y\":35},{\"x\":27,\"y\":36}"
      ",{\"x\":26,\"y\":37},{\"x\":26,\"y\":40},{\"x\":27,\"y\":41},{\"x\":27,"
      "\"y\":50},{\"x\":28,\"y\":50},{\"x\":29,\"y\":51},{\"x\":33,\"y\":51},{"
      "\"x\":34,\"y\":50},{\"x\":35,\"y\":51},{\"x\":36,\"y\":50},{\"x\":37,"
      "\"y\":50},{\"x\":38,\"y\":49},{\"x\":39,\"y\":49},{\"x\":40,\"y\":50},{"
      "\"x\":42,\"y\":50},{\"x\":42,\"y\":47},{\"x\":43,\"y\":46},{\"x\":43,"
      "\"y\":44},{\"x\":40,\"y\":44},{\"x\":39,\"y\":43},{\"x\":39,\"y\":42},{"
      "\"x\":38,\"y\":42},{\"x\":37,\"y\":43},{\"x\":35,\"y\":43},{\"x\":32,"
      "\"y\":40},{\"x\":32,\"y\":39},{\"x\":33,\"y\":38},{\"x\":39,\"y\":38},{"
      "\"x\":40,\"y\":39},{\"x\":41,\"y\":39},{\"x\":42,\"y\":38},{\"x\":43,"
      "\"y\":38},{\"x\":44,\"y\":39},{\"x\":46,\"y\":39},{\"x\":47,\"y\":40},{"
      "\"x\":47,\"y\":41},{\"x\":46,\"y\":42},{\"x\":46,\"y\":43},{\"x\":47,"
      "\"y\":44},{\"x\":47,\"y\":47},{\"x\":48,\"y\":48},{\"x\":48,\"y\":49},{"
      "\"x\":49,\"y\":49},{\"x\":51,\"y\":47},{\"x\":52,\"y\":48},{\"x\":54,"
      "\"y\":48},{\"x\":55,\"y\":47},{\"x\":56,\"y\":47},{\"x\":57,\"y\":46},{"
      "\"x\":57,\"y\":45},{\"x\":58,\"y\":44},{\"x\":58,\"y\":43},{\"x\":56,"
      "\"y\":41},{\"x\":56,\"y\":40},{\"x\":55,\"y\":40},{\"x\":54,\"y\":39},{"
      "\"x\":54,\"y\":38},{\"x\":53,\"y\":37},{\"x\":54,\"y\":36},{\"x\":55,"
      "\"y\":36},{\"x\":55,\"y\":34},{\"x\":54,\"y\":34},{\"x\":53,\"y\":33}]],"
      "\"type\":\"carpet\"},{\"id\":11,\"points\":[[{\"x\":6,\"y\":16},{\"x\":"
      "5,\"y\":17},{\"x\":3,\"y\":17},{\"x\":2,\"y\":18},{\"x\":1,\"y\":18},{"
      "\"x\":1,\"y\":19},{\"x\":0,\"y\":20},{\"x\":0,\"y\":25},{\"x\":1,\"y\":"
      "26},{\"x\":1,\"y\":27},{\"x\":0,\"y\":28},{\"x\":0,\"y\":33},{\"x\":1,"
      "\"y\":34},{\"x\":2,\"y\":34},{\"x\":3,\"y\":35},{\"x\":5,\"y\":35},{"
      "\"x\":7,\"y\":33},{\"x\":9,\"y\":33},{\"x\":10,\"y\":32},{\"x\":11,"
      "\"y\":32},{\"x\":11,\"y\":31},{\"x\":12,\"y\":30},{\"x\":15,\"y\":30},{"
      "\"x\":15,\"y\":29},{\"x\":16,\"y\":28},{\"x\":16,\"y\":27},{\"x\":15,"
      "\"y\":26},{\"x\":14,\"y\":26},{\"x\":13,\"y\":25},{\"x\":13,\"y\":22},{"
      "\"x\":12,\"y\":21},{\"x\":12,\"y\":18},{\"x\":11,\"y\":17},{\"x\":8,"
      "\"y\":17},{\"x\":7,\"y\":16}]],\"type\":\"carpet\"}]";
  EXPECT_EQ(result, mock_result);
}

// 数据正常，融合地图包含地毯地图
TEST(test_carpet_003, DataValid15) {
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
  string result = cv::homebot::getCarpetContours(mock.dump(), rect);
  string mock_result = "[]";
  EXPECT_EQ(result, mock_result);
}

#endif