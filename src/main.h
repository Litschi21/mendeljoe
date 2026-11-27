#ifndef MAIN_H
#define MAIN_H

#include <urc/common/logger.hpp>
#include <urc/common/sleep.hpp>
#include <urc/create.hpp>
#include <urc/wombat/Imu.hpp>
#include <urc/wombat/Servo.hpp>
#include <urc/wombat/Wombat.hpp>

using namespace urc::literals;

class Bot {
  static constexpr auto servo90Time = 500ms;
  urc::wombat::Connection wombat_conn;
  urc::create::Connection create_conn;

  urc::create::Create board;
  urc::wombat::Imu imu;

  urc::create::Motor left;
  urc::create::Motor right;

  urc::wombat::Servo servo1;
  urc::wombat::Servo servo2;

public:
  urc::logger_ptr log;

  explicit Bot();
  ~Bot();

  void acquireConns();
  void turnDeg(int degrees, float speed = 1.0F);
  void moveServo(int servoNum, urc::Rotation degrees);

  void setBoth(float speed);
  void stop();

  void setLeft(float speed);
  void setRight(float speed);
};

#endif // MAIN_H
