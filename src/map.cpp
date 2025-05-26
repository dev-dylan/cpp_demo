#include "map.hpp"
#include "../libs/json.hpp"
#include "../libs/utils.hpp"
#include "gzip.hpp"
#include <any>
#include <bitset>
#include <cwchar>
#include <filesystem>
#include <iostream>
#include <map>
#include <opencv2/freetype.hpp>
#include <opencv2/homebot.hpp>
#include <opencv2/opencv.hpp>
#include <optional>
#include <string>
#include <vector>

using namespace std;
using namespace cv;
using namespace std::chrono;
using json = nlohmann::json;

namespace map_p {

string filePath() {
  std::filesystem::path currentPath(__FILE__);
  std::filesystem::path projectDir = currentPath.parent_path();
  return projectDir.string() + "/";
}

void writeImage(Mat mat, string name) {
  string output_filename = filePath() + "../save/" + name + ".png";
  cout << "save image to " << output_filename << endl;
  bool success = imwrite(output_filename, mat);
}
cv::Mat carpetMat() {
  string imagePath =
      "/Users/admin/Documents/cpp_demo/resource/homebot_carpet.png";
  Mat temp = imread(imagePath, IMREAD_UNCHANGED);
  Mat carpet;
  cvtColor(temp, carpet, COLOR_BGRA2RGBA);
  return carpet;
}
void bidirectionalErgodic(vector<int> data, int start, int end, int width,
                          Mat mat) {
  for (int i = start; i < end; i++) {
    int x = i % width; // 余数是 x 坐标
    int y = i / width; // 除数是 y 坐标
    if (data.size() <= i) {
      continue;
    }
    int value = data[i];
    mat.at<Vec4b>(y, x) = value;
  }
}

/// @brief 手动生成房间图片并添加对应房间名称
void semanticImages() {
  string json_result = utils_p::readJsonFile("../mock/mock1.json");
  json data = json::parse(json_result);
  int image_count = 0;
  // 加载 FreeType 字体
  cv::Ptr<cv::freetype::FreeType2> ft2;
  ft2 = cv::freetype::createFreeType2();
  ft2->loadFontData("/System/Library/Fonts/PingFang.ttc",
                    0); // 使用 PingFang 字体

  for (const auto &item : data) {
    image_count++;
    string map_data = item["map_modification"].get<string>();
    string gzip_json = GZip::decompress(map_data);

    json json_data = json::parse(gzip_json);
    int width = json_data["width"];
    int height = json_data["height"];
    vector<int> data = json_data["data"];

    vector<int> room_ids;
    map<int, string> room_names;
    map<int, string> room_class;

    json partition_list = item["partitionList"].get<json>();
    for (const auto &par_item : partition_list) {
      int partition_id = std::stoi(par_item["partition_id"].get<string>());
      string partioton_name = par_item["room_name"].get<string>();
      string partioton_class = par_item["partition_class"].get<string>();
      room_ids.push_back(partition_id);
      room_names[partition_id] = partioton_name;
      room_class[partition_id] = partioton_class;
    }
    json room_relations = item["room_relations"].get<json>();
    Mat mat1 = cv::homebot::drawFullMap(gzip_json, room_relations.dump());
    json partition_info = item["partitionList"].get<json>();
    string rinfo = cv::homebot::getRoomsInfo(gzip_json, partition_info.dump());
    json room_info = json::parse(rinfo);
    int count = 0;
    Mat resizeMat;
    int scale = 5;
    cv::resize(mat1, resizeMat, Size(), scale, scale);
    for (const auto &index : room_info) {
      // // 定义文本内容和位置
      json point = index["point"].get<json>();
      int r_i = index["index"].get<int>();
      string room_name = room_names[r_i];
      string room_name_value = "";
      for (char c : room_name) {
        if (!isdigit(c) && !isspace(c)) {
          room_name_value += c;
        }
      }

      string room_id_value = to_string(r_i);

      int x = point["x"].get<int>();
      int y = point["y"].get<int>();
      Point name_center(x * scale, (y - 5) * scale);

      Point class_center(x * scale, (y + 5) * scale);
      // 定义文本大小和颜色
      int fontHeight = 30;          // 字体高度
      cv::Scalar color(0, 0, 0, 1); // 黑色

      int baseline = 0;
      cv::Size nameSize =
          ft2->getTextSize(room_name_value, fontHeight, -1, &baseline);
      cv::Point org(name_center.x - nameSize.width / 2,
                    name_center.y + nameSize.height / 2);

      cv::Size classSize =
          ft2->getTextSize(room_id_value, fontHeight, -1, &baseline);
      cv::Point org1(class_center.x - classSize.width / 2,
                     class_center.y + classSize.height / 2);

      // 添加中文文字到图像
      ft2->putText(resizeMat, room_name_value, org, fontHeight, color, -1,
                   cv::LINE_AA, false);
      ft2->putText(resizeMat, room_id_value, org1, fontHeight, color, -1,
                   cv::LINE_AA, false);
      count++;
    }
    Mat rgb_image;
    cv::cvtColor(resizeMat, rgb_image, cv::COLOR_BGR2RGB);
    writeImage(rgb_image, "image_" + std::to_string(image_count) + "_width_" +
                              to_string(width) + "_height_" +
                              to_string(height));
  }
}
void testFuncInput() {

  std::string content = GZip::decompress(
      "eJzt1kFKw0AYgNGrlFmnJakVMVcRF4GGZkAaaSOtlt7dGBVXFcWirfPgzZAwPwyBb5FdmFdd"
      "\r\nFcqbUZ4BAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAL+"
      "hOLDgfzhUuPI5d99t\r\nW++"
      "cJ6WTBqWTDKGTCp2TDoUDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r"
      "\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMDXFVc/X3/+EfC5Y2QudU7dsTr/"
      "aF31nDSp\r\nkw6lkwx/"
      "6iRL3AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAqRrdZqGp46LpQjnN\r\niyy0"
      "q7iIy1DuXp7qZVd1sR1eN6Hsj7ehzLPwOOxP/b7Pwn27ju9D/"
      "fF4OpyPZ5Prt5F+ZlWv\r\n27uH16l8kl9mYRPnXdPfObvYPwPXG8dV");

  // std::string content = utils_p::readJsonFile("../mock/mock1.json");
  std::string materials = "{\"31\":6,\"32\":6,\"33\":8,\"34\":6}";
  Mat res = homebot::drawMaterialMap(content, materials);

  std::string content1 = GZip::decompress(
      "eJzt3N1uE1cUgNFXQb6OUUiCVOVVql4ggSBSRSpIRVvEuzc16qAEQ2zPnNnn7L0uvhEVporP"
      "\r\nrD3O/CSfN69f3b3aXP/6bPviTJIkSZIkSZIkFe7F+XmBTfQqq/"
      "92UC5fjhXdOii6lTe6lTe6\r\nlTe6lTe6lTe6lTe6lTe6lTe6lbdHt7ej3fKtZg0jnG4d3T"
      "C6J+GY6+CG4u0grqOiW3mjW3mj\r\nW3mjW3kbTvck3I+pNbGQ/"
      "WcDhwTvUN9EweXhDbPSfIvvQRpm1XuL7yEaZtV762jfk/"
      "HBftUH\r\n7jqR+5ya76nxnigEfPFd//"
      "jZ0hWBr4c82iTjUcbX9sw333zzzXcO39sJecuzT8ir1gXyCfq+"
      "\r\nzWK7EfKqdYOcccYf7612m2jpT8iHvi76aIrrMCec8KwBDnj6+"
      "OY7cyfvwb2nGx2QRBzxJYlH\r\n++Obb7755vv7d3/"
      "wZdAOHHLO+anOo231EN+jxzffmeOb78zxzXfm+OY7c3zznTm++c4c33xn\r\njm++"
      "M8c335njm+/M8c135viu63v25oe/vGG1jTEwBlHjk0M/"
      "+eSTTz755OOPfkX6D95ntD7y\r\nO4r8IpFPfs3IJ79m5JNfM/"
      "LJrxn55NeMfPJrRj75NSOf/JqRT37NyCe/"
      "ZuSTXzPyya8Z+eTX\r\njHzya0Y++WO3XW7z+EeyO+"
      "BoFIzC2mOwbyqiLUZXahaGHwjDYBgWGIYH7zfatFnoMrMwUGbB\r\nLJgFs2AWzIJZMAtmob"
      "V/42AchhqH1UbBPJiHAfrGdLVNtMfoss/Dvn0ezfyYIIV/"
      "Hv5oweR3\r\nGPkdRz755It88kU++SKf/DK1u9y7FMY16he86+/"
      "t+Ufro90hmm++xTff4pvvIvHNd+b45jtz\r\nfPOduVK+n7heTHjGygh/Ug/"
      "fGeOb78zxzXfm2j2gMtxTLyZAPUwj30oa3/"
      "H7QHzzLb75Ft98\r\n14nv+H0gvvkW33yLb77rxHf8PlBj341v42OuzptlnG91Ht/KHN/"
      "KHN/K1N5TzKvTiU/"
      "MnziL\r\nvYqN82J1oY5u0U236KZbdCt5dCtvdCtvdCtvdCtvdGsFYZ1s0so/"
      "eRW0ou7t5f2/"
      "WKu84CGO\r\ngr4DfOgGdNAHDHTQS9Qv9Al7Pw8Ckj5wfUvf1QFNzAfveOYR6jvQSfrgzZa+"
      "DvcOeKI+eKij\r\nXiTUUS8S6qHc3SkNIj/"
      "r5j3yDUcimkf1vl7lppvujNFNd97opjtvdNOdt5N0//AcFXHE12N7\r\n/"
      "OYk6MePTQfywrS7jthIe6tvPX5u/ODJ6sBfuP1oJ6MX4/"
      "zwr6wDZ3wP3CK+G963K698ku7m\r\naDz2ZquM+SEjEO2n9/getoNW/"
      "tTHNDqgiboOpX70CqZUTviwNbtMlfwDAHmdOm7RdvlWS99D\r\nHfBBV+"
      "uB4JvvpHX0bT7pJRrmIYomnCAv0Xd7qcvVbXm0B71Ee/dSs4uSIe+GdM2z0cFXP/"
      "9d\r\nLL0BvvPmffSfuOnyDfXY7NVaZoqjkc55t+"
      "FWiF9ttWatxDDS95929nEDZ5XDCvDFwY8Q4fEr\r\n0YXw7fQ2Ik+"
      "aVtnJXZ4utvlKZ307fXF6Y8/Dt1mI9lqx2fjWaD7wPs55t7xHl9/7/"
      "NWZrRtx\r\nxHshvuRxfIt5P2H+/"
      "Wo0PS2N3uEVg3xx5Cx3FN98Z45vvjPHN9+Z45vvzPHNd+"
      "b45jtzfPOd\r\nuSF8T8bb3yshPFvDCF/"
      "laM53tvjmO3N88505vvnOHN98Z24435PxVs+gMp6tIY03O67znS2+\r\n+c4c33xnjm++"
      "M7X3zCzaJ+Na2vgO9aPN6DGup4y321BPfT31YfSxF/"
      "YzFw37gcMe+4Jhj33B\r\nsMe+YJnY//DZL8+D6Sd7MAH/"
      "VRFhP3rYY18w7LEvGPbYFywL+4l++18SA36GMsFveux/NFVz\r\n0T/A3/"
      "TCk57YqfQfuFBLmfdhgD322GOPPfbYY69GexP7AxcK+ySNyH71h3Hwz9po/"
      "MNQYJ8p\r\n7LEvGPbYF2w09hP9gNub5GdqRPkhHwDYZwp77AuGPfYFwx77gmGPfcGwx75g2"
      "GNfsNHZT/TX\r\nelwH/0xl4L/"
      "KJwP2mcIe+"
      "4Jhj33BsD9moeI30V6yZx5ULuhVrij0UQNjAPSgEQdg1hSYAD1o\r\nxAlwQqlWDTYP/"
      "GrR+Ffl+Ffl+Ffl+Ffl+"
      "Ffl4h9vcf1SkiRJkiSt3bPfzjbv3ty8fXe3ub54\r\neXG2uf1w8/bm/eb6839/"
      "evP+7tXdze3uPz9trl+cbf7aXJ+fbf7ebf+533452/xx+/Hm/xfd\r\n//X26vnV7hVf/"
      "7B70f2rPrz5ePv7n19fd/78/OXZ5tPN67t39//TXy6//At09Q+7");

  Mat ress = homebot::drawFullMap(content1, "{}");
  // Mat carpet = carpetMat();
  // cv::Rect rect(40, 98, 243, 201);
  // Mat res = cv::homebot::drawCarpetMap(
  //     content, rect, {carpet},
  //     "{}", false);
  cout << "ssss" << ress.rows << ":" << ress.cols << endl;
  writeImage(ress, "material_mapxxx");
}

} // namespace map_p