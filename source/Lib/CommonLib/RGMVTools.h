#pragma once

#include <stdio.h>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <numeric>
#include <cmath>
#define Accuracy_corr 16.0 / 15.9687500

const float PixelRGMVPossConfidence[40] = { 1.0,
                                            0.9999804223185569f,
                                            0.9998896138226733f,
                                            0.999404309252218f,
                                            0.9982471516213848f,
                                            0.9956574185765983f,
                                            0.9914913731978255f,
                                            0.982863233847163f,
                                            0.9727412618157837f,
                                            0.9532358476907485f,
                                            0.9300712068650722f,
                                            0.893828798938288f,
                                            0.8606706524564596f,
                                            0.808649289099526f,
                                            0.756701030927835f,
                                            0.684618222058281f,
                                            0.6179264766625362f,
                                            0.5518266779949023f,
                                            0.4768747193533902f,
                                            0.4125457875457875f,
                                            0.35260663507109f,
                                            0.2977346278317152f,
                                            0.2558674643350207f,
                                            0.20448430493273542f,
                                            0.16892502258355918f,
                                            0.1404710920770878f,
                                            0.11895754518705338f,
                                            0.09240658899156287f,
                                            0.07889467360833f,
                                            0.06424792139077853f,
                                            0.05333333333333334f,
                                            0.04337265787647467f,
                                            0.03545611015490534f,
                                            0.027281584930172135f,
                                            0.02311589613679544f,
                                            0.019085125719478947f,
                                            0.015615792575132587f,
                                            0.013191131069323604f,
                                            0.010351402887496594f,
                                            0.008937960042060988f };

struct YUVBuffer_16bit
{
  std::unique_ptr<uint16_t[]> yBuffer;
  std::unique_ptr<uint16_t[]> uBuffer;
  std::unique_ptr<uint16_t[]> vBuffer;
};
struct YBuffer_8bit
{
  std::unique_ptr<uint8_t[]> yBuffer;
};
struct Block
{
  int x;        // 横坐标
  int y;        // 纵坐标
  int width;    // 宽度
  int height;   // 高度
  int mv_x;     // 水平运动矢量
  int mv_y;     // 垂直运动矢量
};
//分配内存
void allocate_YUV444_buffers(YUVBuffer_16bit &buffer, int width, int height);
void allocate_Y_buffers(YBuffer_8bit &buffer, int width, int height);
//读取RGMV数据
void read_YUV444_frame(const std::string &yuvFilePath, int frameIndex, int width, int height, YUVBuffer_16bit &buffer);
//读取RGMV置信度
void read_Y_8bit_frame(const std::string &yuvFilePath, int frameIndex, int width, int height, YBuffer_8bit &buffer);
//坐标是否有效
bool validate_coordinate(int PosX, int PosY, int width, int height);
// 访问16bit的数据
std::array<uint16_t, 3> get_pixel_YUV_16bit(YUVBuffer_16bit &buffer, int PosX, int PosY, int width, int height);
//访问数据
std::array<float, 2> get_piexl_RGMV(YUVBuffer_16bit &buffer, int PosX, int PosY, int width, int height);

//访问RGMV 帧间预测置信度
uint8_t get_pixel_Y(YBuffer_8bit &buffer, int PosX, int PosY, int width, int height);
//读取实际编码数据
std::vector<Block> readBlocksFromFile(const std::string &filename);
// 计算平均值
template <typename T>
T calculate_mean(const std::vector<T>& values) {
  // 使用 double 类型计算均值以提高精度
  double sum = std::accumulate(values.begin(), values.end(), 0.0);
  double mean = sum / values.size();

  // 返回原始数据类型的均值
  return (int)(mean);
}

// 计算中值
template <typename T>
T calculate_median(std::vector<T> values) {
  size_t size = values.size();
  if (size == 0) return T(0);
  std::sort(values.begin(), values.end());
  if (size % 2 == 0)
    return (values[size / 2 - 1] + values[size / 2]) / 2;
  else
    return values[size / 2];
}

// 计算众数


template <typename T>
T calculate_mode(const std::vector<T>& values) {
  if (values.empty()) {
    throw std::runtime_error("Cannot calculate mode of an empty vector.");
  }

  std::unordered_map<T, int> frequency;
  for (const T& value : values) {
    frequency[value]++;
  }

  T mode = values[0];
  int max_count = 0;

  // 使用传统的迭代器访问方式
  for (auto it = frequency.begin(); it != frequency.end(); ++it) {
    const T& key = it->first;
    int count = it->second;

    if (count > max_count) {
      max_count = count;
      mode = key;
    }
  }

  return mode;
}
//计算方差
template <typename T>
int calculate_variance(const std::vector<T>& values) {
  size_t size = values.size();
  if (size == 0) {
    throw std::runtime_error("Cannot calculate variance of an empty vector.");
  }

  double mean = calculate_mean(values); // 使用 double 计算均值
  double sum_squared_diff = std::accumulate(values.begin(), values.end(), 0.0,
    [mean](double acc, T value) {
    double diff = static_cast<double>(value) - mean;
    return acc + diff * diff;
  });
  double variance = sum_squared_diff / size;

  // 将方差四舍五入为整数
  return (int)(std::round(variance));
}