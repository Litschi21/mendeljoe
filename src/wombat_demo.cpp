#include "wombat_demo.hpp"

#include <urc/common/sleep.hpp>

using namespace urc::literals; // 100ms and the other chrono durations

namespace demo {

    void log_status(const urc::wombat::Wombat& wombat, const urc::wombat::Imu& imu, urc::logger_ptr log) {
        LOG_INFO(log, "Battery voltage: {:.2f} V", wombat.get_battery_voltage());
        LOG_INFO(log, "Push button:     {}", wombat.get_push_button());
        LOG_INFO(log, "IMU temperature: {:.1f} C", imu.get_imu_temperature());
    }

    float average_imu_temperature(const urc::wombat::Imu& imu, int samples) {
        if (samples <= 0) {
            return 0.0F;
        }

        float sum = 0.0F;
        for (int i = 0; i < samples; ++i) {
            sum += imu.get_imu_temperature();
            urc::sleep(100ms);
        }
        return sum / static_cast<float>(samples);
    }

} // namespace demo
