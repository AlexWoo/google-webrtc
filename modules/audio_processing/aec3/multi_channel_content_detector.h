/*
 *  Copyright (c) 2022 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_PROCESSING_AEC3_MULTI_CHANNEL_CONTENT_DETECTOR_H_
#define MODULES_AUDIO_PROCESSING_AEC3_MULTI_CHANNEL_CONTENT_DETECTOR_H_

#include <stddef.h>

#include <vector>

#include "absl/types/optional.h"

namespace webrtc {

// Analyzes audio content to determine whether the contained audio is proper
// multichannel, or only upmixed mono. To allow for differences introduced by
// hardware drivers, a threshold `detection_threshold` is used for the
// detection.
class MultiChannelContentDetector {
 public:
  // If |stereo_detection_timeout_threshold_seconds| <= 0, no timeout is
  // applied: Once multichannel is detected, the detector remains in that state
  // for its lifetime.
  MultiChannelContentDetector(bool detect_stereo_content,
                              int num_render_input_channels,
                              float detection_threshold,
                              int stereo_detection_timeout_threshold_seconds,
                              float stereo_detection_hysteresis_seconds);

  // Compares the left and right channels in the render `frame` to determine
  // whether the signal is a proper multichannel signal. Returns a bool
  // indicating whether a change in the proper multichannel content was
  // detected.
  bool UpdateDetection(
      const std::vector<std::vector<std::vector<float>>>& frame);

  bool IsProperMultiChannelContentDetected() const {
    return persistent_multichannel_content_detected_;
  }

  bool IsTemporaryMultiChannelContentDetected() const {
    return temporary_multichannel_content_detected_;
  }

 private:
  const bool detect_stereo_content_;
  const float detection_threshold_;
  const absl::optional<int> detection_timeout_threshold_frames_;
  const int stereo_detection_hysteresis_frames_;
  bool persistent_multichannel_content_detected_;
  bool temporary_multichannel_content_detected_ = false;
  int64_t frames_since_stereo_detected_last_ = 0;
  int64_t consecutive_frames_with_stereo_ = 0;
};

}  // namespace webrtc

#endif  // MODULES_AUDIO_PROCESSING_AEC3_MULTI_CHANNEL_CONTENT_DETECTOR_H_
