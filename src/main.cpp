#include "main.h"

#include <cmath>
#include <urc/common/sleep.hpp>
#include <urc/create.hpp>
#include <urc/wombat/Imu.hpp>
#include <urc/wombat/Servo.hpp>
#include <urc/wombat/Wombat.hpp>

using namespace urc::literals; // _deg / _rad / _rot -> urc::Rotation, _pct -> urc::Scaling,
                               // plus 500ms and friends

Bot::Bot()
    : board(create_conn, "create"), imu(wombat_conn, "imu"), leftMotor{board.left_wheel()},
      rightMotor{board.right_wheel()},
      armServo(wombat_conn, "armServo", urc::wombat::ServoPort::p0),
      clawServo(wombat_conn, "clawServo", urc::wombat::ServoPort::p1), log(urc::get_logger("LOG")) {
  acquireConns();
}

Bot::~Bot() {
  armServo.disable();
  clawServo.disable();

  wombat_conn.release();
  create_conn.release();
}

void Bot::acquireConns() {
  wombat_conn.acquire();
  create_conn.acquire();
}

void Bot::turnDeg(int degrees, float speed, Direction d) {
  LOG_INFO(log, "Turning {} deg at {}% speed", degrees, speed * 100);
  if (degrees == 0) {
    return;
  }

  float turned{};
  bool left = d == Direction::Left;
  auto prev = imu.get_heading().degrees();

  leftMotor.set_speed(left ? -speed : speed);
  rightMotor.set_speed(left ? speed : -speed);

  while (left ? (turned > degrees) : (turned < degrees)) {
    auto curr = imu.get_heading().degrees();
    turned += std::remainder(curr - prev, 360.0F);
    prev = curr;

    if (degrees - std::abs(turned) <= 20.0) {
      setLeftMotor(left ? -0.05F : 0.05F);
      setRightMotor(left ? 0.05F : -0.05F);
    }

    urc::sleep(1ms);
  }

  setMotors(speed);
}

urc::wombat::Servo Bot::getArmServo() { return armServo; }
urc::wombat::Servo Bot::getClawServo() { return clawServo; }

void Bot::moveServo(urc::wombat::Servo servo, urc::Rotation degrees) {
  servo.set_position(degrees);
  urc::sleep(servo90Time);
}

void Bot::setMotors(float speed) {
  setLeftMotor(speed);
  setRightMotor(speed);
}

void Bot::stop() {
  setLeftMotor(0.0F);
  setRightMotor(0.0F);
}

void Bot::setLeftMotor(float speed) { leftMotor.set_speed(speed); }
void Bot::setRightMotor(float speed) { rightMotor.set_speed(speed); }

int main() {
  Bot bot{};

  bot.setMotors(1.0F);
  urc::sleep(500ms);

  bot.turnDeg(180_deg);
  bot.turnDeg(180_deg, 1.0F, Direction::Left);

  bot.stop();
  return 0;
}
