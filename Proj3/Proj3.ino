#include <FreeRTOS.h>
#include <freertos/semphr.h>
#include <limits.h>

#include "Graphics.h"
#include "Vector2d.hpp"
#include "Ball.h"
#include "Paddle.h"
#include "Wall.h"

// Frame rate info
#define DRAW_PERIOD 20      // 20 ms = 50 fps
#define MOVE_PERIOD 5       // 5 ms

#define BTN_CNT 2
Adafruit_GFX_Button MenuButtons[BTN_CNT];
char MenuLabels[BTN_CNT][2] = {"<", ">"};

// global variables
Ball *ball = nullptr;
Paddle *pPaddle = nullptr;
Paddle *cPaddle = nullptr;
Wall *top = nullptr;
Wall *btm = nullptr;
Wall *left = nullptr;
Wall *right = nullptr;

//Timers
TimerHandle_t DrawTimer;
TimerHandle_t MoveTimer;

//#define configUSE_TASK_NOTIFICATIONS 1
TaskHandle_t xDrawGame = NULL, xMoveGame = NULL;
//SemaphoreHandle_t ballMutex = NULL;

void setup()
{
  Serial.begin(115200);

  graphicsInit();
  initializeButtons(MenuButtons, MenuLabels);

  // create the walls
  double width = screenSize().getX();
  double height = screenSize().getY();

  top = new Wall( Vector2d(0, 0), Vector2d(width - 1, 0), GREEN );
  btm = new Wall( Vector2d(width - 1, height - 1), Vector2d(0, height - 1), GREEN );
  left = new Wall( Vector2d(0, height - 1), Vector2d(0, 0), WHITE );
  right = new Wall( Vector2d(width - 1, 0), Vector2d(width - 1, height - 1), WHITE );

  // create the ball
  ball = new Ball( Vector2d(width / 2, height / 2) );

  // create the paddles
  pPaddle = new Paddle(Vector2d(width / 2, 5), Vector2d(80, 5), Vector2d(0,1));
  cPaddle = new Paddle(Vector2d(width / 2, height - 10), Vector2d(80, 5), Vector2d(0,-1));

  // timers
  DrawTimer = xTimerCreate(
                "DrawTimer",
                DRAW_PERIOD,
                pdTRUE,
                NULL,
                NotifyDraw);

  MoveTimer = xTimerCreate(
                "MoveTimer",
                MOVE_PERIOD,
                pdTRUE,
                NULL,
                NotifyMove);

  // tasks
  xTaskCreatePinnedToCore(
    DrawGame,
    "DrawGame",
    10000,
    NULL,
    2,
    &xDrawGame,
    1);

  xTaskCreatePinnedToCore(
    MoveGame,
    "MoveGame",
    10000,
    NULL,
    2,
    &xMoveGame,
    1);

  // mutexes
  xTimerStart(DrawTimer, 0);
  xTimerStart(MoveTimer, 0);
}

void loop()
{
  // put your main code here, to run repeatedly:
}

void NotifyDraw (TimerHandle_t xTimer)
{
  xTaskNotifyGive(xDrawGame);
}

void NotifyMove (TimerHandle_t xTimer)
{
  xTaskNotifyGive(xMoveGame);
}

void DrawGame (void *params)
{
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    ball->draw();

    top->draw();
    btm->draw();
    left->draw();
    right->draw();

    cPaddle->draw();
    pPaddle->draw();
  }
}

void MoveGame (void *params)
{
  for (;;) {
    int btn = buttonPressed(MenuButtons, BTN_CNT);

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    pPaddle->move(btn, MOVE_PERIOD / 1000.0);
    if (pPaddle->isCollidingWith(left) == true)
      pPaddle->handleCollision(left);
    if (pPaddle->isCollidingWith(right) == true)
      pPaddle->handleCollision(right);

    cPaddle->move(ball->getV(), MOVE_PERIOD / 5000.0);
    if (cPaddle->isCollidingWith(left) == true)
      cPaddle->handleCollision(left);
    if (cPaddle->isCollidingWith(right) == true)
      cPaddle->handleCollision(right);

    ball->move(MOVE_PERIOD / 1000.0);
    if (ball->isCollidingWith(top) == true)
      ball->handleCollision(top);
    if (ball->isCollidingWith(btm) == true)
      ball->handleCollision(btm);
    if (ball->isCollidingWith(left) == true)
      ball->handleCollision(left);
    if (ball->isCollidingWith(right) == true)
      ball->handleCollision(right);
    if (ball->isCollidingWith(pPaddle) == true)
      ball->handleCollision(pPaddle);
    if (ball->isCollidingWith(cPaddle) == true)
      ball->handleCollision(cPaddle);
  }
}
