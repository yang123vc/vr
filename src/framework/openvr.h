#pragma once

#include "framework/config.h"

#ifdef FRAMEWORK_SUPPORTS_OPENVR

#include <openvr.h>

#ifdef _WIN32
#pragma comment(lib, "openvr_api")
#endif

#include "framework/glm.h"

namespace framework {

  namespace openvr {
    using namespace vr;

    typedef uint64_t process_id;

    enum device_id : uint32_t {
      hmd = vr::k_unTrackedDeviceIndex_Hmd,
      max = vr::k_unMaxTrackedDeviceCount
    };

    static inline glm::mat4 hmd_mat4(const vr::HmdMatrix44_t & m) noexcept {
      return glm::make_mat4((float*)&m.m);
    }

    static inline glm::mat4 hmd_mat3x4(const vr::HmdMatrix34_t & m) noexcept {
      return glm::mat4(
        m.m[0][0], m.m[1][0], m.m[2][0], 0.0,
        m.m[0][1], m.m[1][1], m.m[2][1], 0.0,
        m.m[0][2], m.m[1][2], m.m[2][2], 0.0,
        m.m[0][3], m.m[1][3], m.m[2][3], 1.0f
      );
    }

    // static utilities
    const char * show_tracked_device_class(TrackedDeviceClass c) noexcept;
    const char * show_event_type(int e) noexcept;
    const char * show_compositor_error(EVRCompositorError e) noexcept;

    static inline EVREye eye(int i) noexcept {
      return (i == 0) ? vr::Eye_Left : vr::Eye_Right;
    }

    static inline const char * show_eye(int i) noexcept {
      switch (i) {
        case 0: return "left";
        case 1: return "right";
        default: return "unknown";
      }
    }
  }
}

#endif