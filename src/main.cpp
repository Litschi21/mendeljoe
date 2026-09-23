// Minimal Wombat API tutorial.
//
// Connects to the Wombat daemon, reports what it finds, waits for the push button, then
// briefly drives a motor and a servo. Reading sensors needs no lock; driving actuators
// requires acquire() (the hardware lock).
//
// Build & deploy:  scripts/deploy.sh   (format -> build -> upload -> run)

#include "wombat_demo.hpp"

#include <urc/common/sleep.hpp>

using namespace urc::literals; // _deg / _rad / _rot -> urc::Rotation, _pct -> urc::Scaling,
                               // plus 500ms and friends

int main() {
    // get_logger() lazily starts the quill logging backend on first call.
    auto* log = urc::get_logger("tutorial");

    try {
        // Connects to Connection::SYSTEMD_SOCKET_PATH (/run/wombat/api.sock) and
        // performs the GetInfo handshake. Pass Connection::TEMP_SOCKET_PATH as the
        // socket argument to talk to a development socket instead.
        urc::wombat::Connection connection;
        urc::wombat::Wombat wombat(connection, "wombat");
        urc::wombat::Imu imu(connection, "imu");

        demo::log_status(wombat, imu, log);

        // Nothing above took the lock, so the check can happen with the robot on the table
        // and someone else still holding control.
        demo::wait_for_button(wombat, log);

        // Driving motors/servos takes the hardware lock.
        connection.acquire();

        // Motor speed is a urc::Scaling, normalised to [-1.0 .. 1.0]; _pct spells that out.
        urc::wombat::Motor motor(connection, "motor", urc::wombat::MotorPort::p0, 1.0);
        motor.set_speed(50_pct);
        urc::sleep(500ms);
        motor.off();

        // Servo angle is a urc::Rotation, the urc::literals keep it readable. It is measured
        // about the centre, so 0_deg is centre and the range is -90_deg to +90_deg.
        urc::wombat::Servo servo(connection, "servo", urc::wombat::ServoPort::p0);
        servo.set_position(45_deg);
        urc::sleep(500ms);
        servo.disable();

        // No release() needed: `connection` goes out of scope here and gives control up on
        // its own. Call it explicitly only to hand control over earlier than that.
    } catch (const urc::wombat::WombatException& e) {
        LOG_ERROR(log, "Wombat error: {}", e.what());
        return 1;
    }

    return 0;
}
