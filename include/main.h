#ifndef MAIN_H
#define MAIN_H

#include <urc/common/logger.hpp>
#include <urc/common/sleep.hpp>
#include <urc/create.hpp>
#include <urc/wombat/Imu.hpp>
#include <urc/wombat/Servo.hpp>
#include <urc/wombat/Wombat.hpp>

using namespace urc::literals;

enum class Direction {
  Left,
  Right
};

class Bot {
  static constexpr auto servo90Time = 500ms;
  urc::wombat::Connection wombat_conn;
  urc::create::Connection create_conn;

  urc::create::Create board;
  urc::wombat::Imu imu;

  urc::create::Motor leftMotor;
  urc::create::Motor rightMotor;

  urc::wombat::Servo armServo;
  urc::wombat::Servo clawServo;

public:
  urc::logger_ptr log;

  explicit Bot();
  ~Bot();

  void acquireConns();
  void turnDeg(int degrees, float speed = 1.0F, Direction d = Direction::Right);
  void moveServo(urc::wombat::Servo servo, urc::Rotation degrees);

  urc::wombat::Servo getArmServo();
  urc::wombat::Servo getClawServo();

  void setMotors(float speed);
  void stop();

  void setLeftMotor(float speed);
  void setRightMotor(float speed);
};

#endif // MAIN_H
