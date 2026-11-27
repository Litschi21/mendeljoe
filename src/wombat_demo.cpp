#include "wombat_demo.hpp"

#include <urc/common/sleep.hpp>

using namespace urc::literals; // 20ms and the other chrono durations

namespace demo {

void log_status(
    const urc::wombat::Wombat &wombat, const urc::wombat::Imu &imu, urc::logger_ptr log
) {
  // A urc::Scaling prints itself as an unrounded percentage (59.3091%), so the level goes
  // through a double to come out whole. urc::Rotation prints itself fine as it is.
  const auto level = static_cast<double>(wombat.get_battery_level()) * 100.0;
  LOG_INFO(log, "Battery: {:.2f} V, {:.0f}% charged", wombat.get_battery_voltage(), level);
  LOG_INFO(log, "Heading: {}", imu.get_heading());
}

void wait_for_button(const urc::wombat::Wombat &wombat, urc::logger_ptr log) {
  LOG_INFO(log, "Press the push button to start.");
  while (!wombat.get_push_button()) {
    urc::sleep(20ms);
  }

  // Wait for the release too - otherwise a button still held would satisfy the next wait
  // immediately, and one press would start two things.
  while (wombat.get_push_button()) {
    urc::sleep(20ms);
  }
}

} // namespace demo
