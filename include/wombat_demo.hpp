#pragma once

#include <urc/common/logger.hpp>
#include <urc/wombat.hpp>

namespace demo {

    /// Logs a one-line snapshot of the most common Wombat sensors.
    void log_status(const urc::wombat::Wombat& wombat, const urc::wombat::Imu& imu, urc::logger_ptr log);

    /// Reads the IMU die temperature `samples` times (100 ms apart) and returns
    /// the mean in degrees Celsius.
    [[nodiscard]] float average_imu_temperature(const urc::wombat::Imu& imu, int samples);

} // namespace demo
