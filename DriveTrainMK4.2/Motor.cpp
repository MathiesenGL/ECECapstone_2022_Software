# include "Motor.h"

// Constuctor
Motor::Motor(uint16_t *_OCRnX, uint8_t *_PORTX, uint8_t _bitMask, uint8_t _minSpeed, uint8_t _baseSpeed, uint8_t _maxSpeed):
  OCRnX(_OCRnX), PORTX(_PORTX), bitMask(_bitMask), minSpeed(_minSpeed), baseSpeed(_baseSpeed), maxSpeed(_maxSpeed), currSpeed(0x00), dir(0x00)
{
  // intentionally blank
}
void Motor::accelerate() {

  if (currSpeed < minSpeed)
    currSpeed = minSpeed;
  else if (currSpeed >= maxSpeed)
    currSpeed = maxSpeed;
  else
    currSpeed++;


  *OCRnX = currSpeed;
}

void Motor::decelerate() {

  if (currSpeed > maxSpeed)
    currSpeed = maxSpeed;
  else if (currSpeed <= minSpeed)
    currSpeed = minSpeed;
  else
    currSpeed--;


  *OCRnX = currSpeed;
}

void Motor::initSpeed(uint8_t newSpeed) {
  if (newSpeed > maxSpeed)
    currSpeed = maxSpeed;
  else if (newSpeed < minSpeed)
    currSpeed = minSpeed;
  else
    currSpeed = newSpeed;

  *OCRnX = currSpeed;
}

void Motor::turnSpeed(uint8_t newSpeed) {
  currSpeed = newSpeed;

  *OCRnX = currSpeed;

}

void Motor::changeDir() {
  *PORTX ^= bitMask;
}

void Motor::brake() {
  currSpeed = 0x00;
  *OCRnX = currSpeed;
}

int Motor::getOCRnX() {
  return *OCRnX;
}

int Motor::getPORTX() {
  return *PORTX;
}

int Motor::getSpeed() {
  return currSpeed;
}
