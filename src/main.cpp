// Minimal Wombat API tutorial.
//
// Connects to the Wombat daemon, prints a few read-only sensor values, then
// briefly drives a motor and a servo. Reading sensors needs no lock; driving
// actuators requires acquire_control() (the hardware lock).
//
// Build & deploy:  scripts/deploy.sh   (build -> upload -> run)

#include "wombat_demo.hpp"

#include <chrono>
#include <thread>

using namespace std::chrono_literals;
using namespace urc::literals; // _deg / _rad / _rot -> urc::Rotation

int main() {
    // get_logger() lazily starts the quill logging backend on first call.
    auto* log = urc::get_logger("tutorial");

    try {
        // Connects to Connection::SYSTEMD_SOCKET_PATH (/run/wombat/api.sock) and
        // performs the GetInfo handshake. Pass Connection::TEMP_SOCKET_PATH as the
        // socket argument to talk to a development socket instead.
        urc::wombat::Connection connection;
        urc::wombat::Wombat wombat(connection, "wombat");

        demo::log_status(wombat, log);

        const float avg = demo::average_imu_temperature(wombat, 5);
        LOG_INFO(log, "Average IMU temperature over 5 samples: {:.2f} C", avg);

        // Driving motors/servos takes the hardware lock, held until
        // release_control() (or the Connection is destroyed).
        connection.acquire_control();

        // Motor speed is a urc::Scaling, normalised to [-1.0 .. 1.0].
        urc::wombat::Motor motor(connection, "motor", urc::wombat::MotorPort::p0, 1.0);
        motor.set_speed(0.5); // half speed forward
        std::this_thread::sleep_for(500ms);
        motor.off();

        // Servo angle is a urc::Rotation — the urc::literals keep it readable.
        urc::wombat::Servo servo(connection, "servo", urc::wombat::ServoPort::p0);
        servo.set_position(90_deg);
        std::this_thread::sleep_for(500ms);
        servo.disable();

        connection.release_control();
    } catch (const urc::wombat::WombatException& e) {
        LOG_ERROR(log, "Wombat error: {}", e.what());
        return 1;
    }

    return 0;
}
