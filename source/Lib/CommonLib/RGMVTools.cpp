#include "RGMVTools.h"

void allocate_YUV444_buffers(YUVBuffer_16bit &buffer, int width, int height)
{
  // size_t yuvTotalSize = width * height * 3 * 2;
  buffer.yBuffer = std::make_unique<uint16_t[]>(width * height * 2);
  buffer.uBuffer = std::make_unique<uint16_t[]>(width * height * 2);
  buffer.vBuffer = std::make_unique<uint16_t[]>(width * height * 2);
}

void read_YUV444_frame(const std::string &yuvFilePath, int frameIndex, int width, int height, YUVBuffer_16bit &buffer)
{
  std::ifstream file(yuvFilePath, std::ios::binary);
  if (!file.is_open())
  {
    throw std::runtime_error("无法打开YUV文件");
  }

  file.seekg(frameIndex * width * height * 6, std::ios::beg);

  // 分配内存
  allocate_YUV444_buffers(buffer, width, height);

  // 读取帧数据
  file.read(reinterpret_cast<char *>(buffer.yBuffer.get()), width * height * 2);
  file.read(reinterpret_cast<char *>(buffer.uBuffer.get()), width * height * 2);
  file.read(reinterpret_cast<char *>(buffer.vBuffer.get()), width * height * 2);
  file.close();
}
bool validate_coordinate(int PosX, int PosY, int width, int height)
{
  if (PosX >= 0 && PosX < width && PosY >= 0 && PosY < height)
  {
    return true;
  }
  else
  {
    return false;
  }
}
// 访问16bit的数据
std::array<uint16_t, 3> get_pixel_YUV_16bit(YUVBuffer_16bit &buffer, int PosX, int PosY, int width, int height)
{
  if (!validate_coordinate(PosX, PosY, width, height))
  {
    printf("Pos:(%d,%d) is out of the bounds\n", PosX, PosY);
    int x = PosX < 0 ? 0 : PosX > width - 1 ? width - 1 : PosX;
    int y = PosY < 0 ? 0 : PosY > height - 1 ? height - 1 : PosY;

    uint16_t pixelY = buffer.yBuffer.get()[y * width + x];
    uint16_t pixelU = buffer.uBuffer.get()[y * width + x];
    uint16_t pixelV = buffer.vBuffer.get()[y * width + x];

    std::array<uint16_t, 3> centerPixelYUV = { pixelY, pixelU, pixelV };
    return centerPixelYUV;
  }
  else
  {
    uint16_t pixelY = buffer.yBuffer.get()[PosY * width + PosX];
    uint16_t pixelU = buffer.uBuffer.get()[PosY * width + PosX];
    uint16_t pixelV = buffer.vBuffer.get()[PosY * width + PosX];

    std::array<uint16_t, 3> centerPixelYUV = { pixelY, pixelU, pixelV };
    return centerPixelYUV;
  }
}

// 访问RG-MV数据
std::array<float, 2> get_piexl_RGMV(YUVBuffer_16bit &buffer, int PosX, int PosY, int width, int height)
{
  std::array<uint16_t, 3> DepthMv_UINT16 = get_pixel_YUV_16bit(buffer, PosX, PosY, width, height);
  float ScreenMv_U = Accuracy_corr * (((float(DepthMv_UINT16[1]) - 32768.0f) * 2.0f) / 65536.0f) * (width - 1);
  float ScreenMv_V = -1.0f * Accuracy_corr * (((float(DepthMv_UINT16[2]) - 32768.0f) * 2.0f) / 65536.0f) * (height - 1);
  std::array<float, 2> RGMV = { ScreenMv_U, ScreenMv_V };
  return RGMV;
}