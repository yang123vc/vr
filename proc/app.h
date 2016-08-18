#pragma once

#include "framework.h"
#include <random>

namespace proc {

  using namespace framework;
  
  struct app {
    app() : window("proc", { 4, 5, gl::profile::core }), vr(), display(vr) {}
    ~app() {}
    void run();

    sdl::window window;
    openvr::system vr;
    openvr::display display;
    std::mt19937 rng; // for the main thread
    vr::TrackedDevicePose_t physical_pose[vr::k_unMaxTrackedDeviceCount]; // current poses
    vr::TrackedDevicePose_t predicted_pose[vr::k_unMaxTrackedDeviceCount]; // poses 2 frames out
  };

}