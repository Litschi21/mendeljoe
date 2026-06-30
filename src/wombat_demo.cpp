#include "wombat_demo.hpp"

#include <chrono>
#include <thread>

namespace demo {

    void log_status(const urc::wombat::Wombat& wombat, urc::logger_ptr log) {
        LOG_INFO(log, "Battery voltage: {:.2f} V", wombat.get_battery_voltage());
        LOG_INFO(log, "Push button:     {}", wombat.get_push_button());
        LOG_INFO(log, "IMU temperature: {:.1f} C", wombat.get_imu_temperature());
    }

    float average_imu_temperature(const urc::wombat::Wombat& wombat, int samples) {
        if (samples <= 0) {
            return 0.0F;
        }

        float sum = 0.0F;
        for (int i = 0; i < samples; ++i) {
            sum += wombat.get_imu_temperature();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        return sum / static_cast<float>(samples);
    }

} // namespace demo
