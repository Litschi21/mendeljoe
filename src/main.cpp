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
    : board(create_conn, "create"), imu(wombat_conn, "imu"), left{board.left_wheel()},
      right{board.right_wheel()}, servo1(wombat_conn, "servo1", urc::wombat::ServoPort::p0),
      servo2(wombat_conn, "servo2", urc::wombat::ServoPort::p1), log(urc::get_logger("LOG")) {
  acquireConns();
}

Bot::~Bot() {
  servo1.disable();
  servo2.disable();

  wombat_conn.release();
  create_conn.release();
}

void Bot::acquireConns() {
  wombat_conn.acquire();
  create_conn.acquire();
}

void Bot::turnDeg(int degrees, float speed) {
  LOG_INFO(log, "Turning {} deg at {}% speed", degrees, speed * 100);
  if (degrees == 0.0F) {
    return;
  }

  float turned{};
  auto prev = imu.get_heading().degrees();

  left.set_speed(speed);
  right.set_speed(-speed);

  while (turned < degrees) {
    auto curr = imu.get_heading().degrees();
    turned += std::remainder(curr - prev, 360.0F);
    prev = curr;

    if (degrees - turned <= 20.0) {
      setLeftMotor(0.05F);
      setRightMotor(-0.05F);
    }

    urc::sleep(1ms);
  }

  setMotors(speed);
}

void Bot::moveServo(int servoNum, urc::Rotation degrees) {
  if (servoNum == 1) {
    servo1.set_position(degrees);
  }

  if (servoNum == 2) {
    servo2.set_position(degrees);
  }

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

void Bot::setLeftMotor(float speed) { left.set_speed(speed); }
void Bot::setRightMotor(float speed) { right.set_speed(speed); }

int main() {
  Bot bot{};

  bot.setMotors(0.5F);
  urc::sleep(1000ms);

  bot.turnDeg(720, 0.5F);
  urc::sleep(1000ms);

  bot.turnDeg(180, 0.5F);
  urc::sleep(1000ms);

  bot.stop();
  return 0;
}
