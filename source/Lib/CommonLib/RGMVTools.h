#pragma once

#include "CommonDef.h"
#include <stdio.h>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <fstream>

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