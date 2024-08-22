#pragma once

#include "CommonDef.h"
#include <stdio.h>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <fstream>

#define Accuracy_corr 16.0 / 15.9687500

struct YUVBuffer_16bit
{
  std::unique_ptr<uint16_t[]> yBuffer;
  std::unique_ptr<uint16_t[]> uBuffer;
  std::unique_ptr<uint16_t[]> vBuffer;
};
//分配内存
void allocate_YUV444_buffers(YUVBuffer_16bit &buffer, int width, int height);
//读取数据
void read_YUV444_frame(const std::string &yuvFilePath, int frameIndex, int width, int height, YUVBuffer_16bit &buffer);
//坐标是否有效
bool validate_coordinate(int PosX, int PosY, int width, int height);
// 访问16bit的数据
std::array<uint16_t, 3> get_pixel_YUV_16bit(YUVBuffer_16bit &buffer, int PosX, int PosY, int width, int height);
//访问数据
std::array<float, 2> get_piexl_RGMV(YUVBuffer_16bit &buffer, int PosX, int PosY, int width, int height);