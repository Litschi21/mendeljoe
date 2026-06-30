// Minimal Wombat API tutorial.
//
// Connects to the Wombat daemon and prints a few read-only sensor values.
// Reading sensors does NOT require acquire_control() — that is only needed when
// you want to drive motors/servos (which take the hardware lock).
//
// Build & deploy:  scripts/deploy.sh   (build -> upload -> run)

#include "wombat_demo.hpp"

int main() {
    // get_logger() lazily starts the quill logging backend on first call.
    auto* log = urc::get_logger("tutorial");

    try {
        // Connects to Connection::SYSTEMD_SOCKET_PATH (/run/wombat/api.sock) and
        // performs the GetInfo handshake. Pass Connection::TEMP_SOCKET_PATH as the
        // third argument to talk to a development socket instead.
        urc::wombat::Connection connection;
        urc::wombat::Wombat wombat(connection, "wombat");

        demo::log_status(wombat, log);

        const float avg = demo::average_imu_temperature(wombat, 5);
        LOG_INFO(log, "Average IMU temperature over 5 samples: {:.2f} C", avg);

        // Next steps — drive a motor (needs the hardware lock):
        //   connection.acquire_control();
        //   urc::wombat::Motor motor(connection, "motor", urc::wombat::MotorPort::p0, 1.0);
        //   motor.set_speed(0.5f);   // [-1.0 .. 1.0]
        //   ...
        //   motor.off();
    } catch (const urc::wombat::WombatException& e) {
        LOG_ERROR(log, "Wombat error: {}", e.what());
        return 1;
    }

    return 0;
}
