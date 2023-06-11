#pragma once

#include <cassert>
#include <cstdint>
#include <thread>

#include "cereal/messaging/messaging.h"
#include "cereal/visionipc/visionipc.h"
#include "common/queue.h"
#include "system/camerad/cameras/camera_common.h"

#define V4L2_BUF_FLAG_KEYFRAME 8

class VideoEncoder {
public:
  VideoEncoder(const char* filename, CameraType type, int in_width, int in_height, int fps,
              int bitrate, cereal::EncodeIndex::Type codec, int out_width, int out_height)
  : filename(filename), type(type), in_width(in_width), in_height(in_height), fps(fps),
    bitrate(bitrate), codec(codec), out_width(out_width), out_height(out_height) { }
  virtual ~VideoEncoder();
  virtual int encode_frame(VisionBuf* buf, VisionIpcBufExtra *extra) = 0;
  virtual void encoder_open(const char* path) = 0;
  virtual void encoder_close() = 0;

  void publisher_init();
  static void publisher_publish(VideoEncoder *e, int segment_num, uint32_t idx, VisionIpcBufExtra &extra, unsigned int flags, kj::ArrayPtr<capnp::byte> header, kj::ArrayPtr<capnp::byte> dat);


protected:
  const char* filename;
  int in_width, in_height;
  int out_width, out_height, fps;
  int bitrate;
  cereal::EncodeIndex::Type codec;
  CameraType type;

private:
  // total frames encoded
  int cnt = 0;

  // publishing
  std::unique_ptr<PubMaster> pm;
  const char *service_name;
};