#pragma once

#include <urc/common/logger.hpp>
#include <urc/wombat.hpp>

namespace demo {

/// Logs what is worth knowing before a run starts: battery charge and which way the robot faces.
void log_status(
    const urc::wombat::Wombat &wombat, const urc::wombat::Imu &imu, urc::logger_ptr log
);

/// Blocks until the Wombat's push button has been pressed and let go again.
void wait_for_button(const urc::wombat::Wombat &wombat, urc::logger_ptr log);

} // namespace demo
