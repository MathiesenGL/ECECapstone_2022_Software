#pragma once

#include "Config.h"

// Initializes PWM on Timer1
void initPWM() {

  // Set all bits in TCCRnX registers to 0
  TCCR1A &= ~0xFF;
  TCCR1B &= ~0xDF;

  // Set the COMnX bits in the TCCRnA registers
  TCCR1A |= 0xA0;

  // Set the WGM bits in the TCCRnA registers
  TCCR1A |= 0x01;

  // Set the CS bits in TCCRnB for a prescaler of 1
  TCCR1B |= 0x01;

  // Set the last WGM bits in TCCRnB for fast PWM, 8-bit
  TCCR1B |= 0x08;
}

// Initializes interrupt pins
void initISR() {
  PCICR &= ~0x07;
  PCMSK0 &= ~0xFF;

  PCICR |= 0x01;
  PCMSK0 |= 0x09;
}

// Checks the compared results from the two distance sensors
// sets the turnCond boolean true if within range of the wall
// range set with a potentiomete on the breakout board
void getDistance() {

  if ((PINK & 0x03) == 0x03)
    IRvalue++;
  else
    IRvalue = 0;

  if (IRvalue == 3)
    turnCond = true;
  else
    turnCond = false;
}

// Reads the compared results from the line sensor array
// threshold is set by a potentiometer on the breakout board
void getLine(int delayDur = 5) {

  uint8_t compValue = ~PINF;
  uint8_t lineAverage = 0x00;

  for (int i = 0; i < 3; i++) {
    lineAverage ^= ~PINF;
    delay(delayDur);
  }

  if (compValue == lineAverage)
    lineValue = lineAverage;

  rValue = lineValue & 0x0F;
  lValue = (lineValue >> 4) & 0x0F;

  /*lineValue = ~PINF;

    rValue = lineValue & 0x0F;
    lValue = (lineValue >> 4) & 0x0F;*/
}

// Recieves control signal from the PID controller class
// seeks to drive the system to be centered on the line
// and moving forward
void trackLine() {
  //getDistance();
  getLine();

  if ((lValue == 0x0F) || (rValue == 0x0F))
    return;

  int diff = Pid.controlFunc(lValue - rValue);

  if (diff < 0) {
    lMotor.initSpeed(baseSpeed + diff);
    rMotor.initSpeed(baseSpeed - diff);
  }
  else if (diff > 0) {
    rMotor.initSpeed(baseSpeed + diff);
    lMotor.initSpeed(baseSpeed - diff);
  }

}


// Sets the motor speed to account for drift in the
// reverse state, tracking the line is unreliable due to
// sensor array being on the opposite end of travel
void reverse() {
  getLine();
  if ((lValue == 0x00) && (rValue = 0x00)) {
    lMotor.initSpeed(baseSpeed + 4);
    rMotor.initSpeed(baseSpeed);
  }

  else {
    int diff = Pid.controlFunc(lValue - rValue);

    if (diff > 0) {
      lSpeed += diff;
      rSpeed -= diff;
    }
    else if (diff < 0) {
      rSpeed += diff;
      lSpeed -= diff;
    }
    rMotor.initSpeed(rSpeed);
    lMotor.initSpeed(lSpeed);
  }
}

// function has multiple while loops designed to
// gate the function from exiting until the sensor array
// fully leaves the line and returns, on the return it
// looks for the robot to be centered enough for the PID
// to take over and stabalize
void turnAround() {

  lMotor.initSpeed(0x4E);
  rMotor.initSpeed(0x4E);

  rMotor.changeDir();

  getLine();
  while (lineValue > 0x00)
    getLine();
  while (rValue < 0x07)
    getLine();
  while ((lValue & 0x01) != 0x01)
    getLine();
}

//
void turnLeft() {
  lMotor.changeDir();
  lMotor.initSpeed(0x4E);
  rMotor.initSpeed(0x4E);

  /*while (lineValue = 0x00)
    getLine();
    while (lValue < 0x07)
    getLine();*/

  delay(500);
  getLine();

  lMotor.changeDir();

}

//
void turnRight() {
  rMotor.changeDir();
  lMotor.initSpeed(0x4E);
  rMotor.initSpeed(0x4E);

  /*while (lineValue = 0x00)
    getLine();
    while (rValue < 0x07)
    getLine();*/

  delay(500);
  getLine();
  rMotor.changeDir();


}

//
void brake() {
  lMotor.brake();
  rMotor.brake();
}

//
void nextState() {
  switch (state) {

    case 0: // stall State
      if ((PINL & 0x80) == 0x80) {
        state = 1;
        lastState = 0;
      }

      break;

    case 1: // start State

      if ((lValue == 0x0F) && (rValue = 0x0F))
        inStart = false;

      if (inStart == false)
        if ((rValue & 0x08) != 0x08) {
          state = 2;
          lastState = 1;
          returnState = state;
          breakAway = false;
        }

      break;

    case 2: // trackLine State
      getDistance();
      if (rValue == 0x0F) {
        breakAway = true;
      }

      else
        breakAway = false;

      if (turnCond == true) {
        state = 3;
        lastState = 2;
        returnState = state;
      }
      break;

    case 3: // turnRight State

      state = 4;
      lastState = 3;
      returnState = 4;

      break;

    case 4: // down State

      if (turnCond == true) {
        state = 12;
        lastState = 4;
        returnState = state;
      }

      break;

    case 5: // turnAround State

      if (lastState == 4)
        state = 12;

      else if (lastState == 8) {
        state = 2;
        breakAway = false;
      }
      else if (lastState == 12)
        state = 12;

      else {
        state = 2;
        breakAway = false;
      }

      lastState = 5;
      returnState = state;
      rMotor.changeDir();


      break;

    case 6: // back State

      if (lValue == 0x0F) {
        breakAway = true;
      }

      else
        breakAway = false;

      if (turnCond == true) {
        state = 7;
        lastState = 6;
        returnState = state;
      }

      break;

    case 7: // turnLeft State

      state = 8;
      lastState = 7;
      returnState = state;

      break;

    case 8: // end State

      if (lineValue == 0xFF) {
        delay(150);
        state = 5;
        lastState = 8;
        returnState = state;
      }

      break;

    case 9: // reverse State

      if (lineValue == 0xFF) {
        state = 5;
        lastState = 9;
        Pid.changeGain(1.00);
        ;        PORTA ^= 0x03;
      }

      break;

    case 10: // brake State

      break;

    case 11: // return State

      if (lastState == 4)
        state = 5;

      else
        state = returnState;

      break;

    case 12:

      if (lastState == 4)
        state = 5;

      else if (lastState == 5) {

        if ((PINK & 0x03) != 0x03) {
          state = 6;
          breakAway = false;
        }

        else
          state = 5;
      }
      lastState = 12;

      returnState = state;

      break;

    default:
      state = 0;
      break;
  }
}
