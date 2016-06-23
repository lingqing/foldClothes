/********************************************************************
 * File Name: FoldClothes
 * Description: 鍙犺。鏉跨▼搴忥紝瀹屾垚鍙犺。鏈嶇殑宸ヤ綔
 * Author:  AndyLiang
 * Date :   2016-6-16 
 *******************************************************************/
/* ---------- user define ------------- */  
// servo
#define SERVO_POS_INIT 1500
#define SERVO_POS_LEFT 1000
#define SERVO_POS_RIGHT 2000
#define SERVO_POS_LEFT 1000
#define SERVO_POS_BACK 2500

/* ---------- include ------------- */ 
#include <Servo.h>  // 鑸垫満搴�
#include "UserMotor.h"  // 鑷畾涔夌數鏈洪┍鍔�
#include "ColorSensor.h"  // 棰滆壊浼犳劅鍣�
#include "MusicPlayer.h"

/* ---------- define ------------- */ 
// define
#define MOTOR1_RUN_A 3
#define MOTOR1_DIR_B 4
#define MOTOR2_RUN_A 5
#define MOTOR2_DIR_B 6
#define MOTOR3_RUN_A 7
#define MOTOR3_DIR_B 8

#define LIMIT_LEFT    11
#define LIMIT_RIGHT   10
#define LIMIT_BACK    12

#define DETECT_PIN    13    // 衣服坚持传感器
#define PLATE_PIN     9

// #define _TEST_
/* ---------- type define ------------- */
typedef enum{
  INIT,
  READY,
  COLOR,
  FOLD,
  WHIRL,
  ENDING,
  TEST,
}MachineState;

/* ---------- global var ------------- */
Servo servoPlate;
UserMotor motor1(1);
UserMotor motor2(2);
UserMotor motor3(3);
MusicPlayer player;
//ColorSensor colorSensor;
MachineState state;
static uint8_t colorClothes = 0;

/**
 * test
 */
void test()
{
  // motor1.runToLimit(FORWARD, 2000);
  // delay(1000);
  // motor2.runToLimit(FORWARD, 2000);
  // delay(1000);
  // motor3.runToLimit(FORWARD, 2000);
  // delay(1000); 
  /*detectClothesColor(); 
  switch (colorClothes) {
      case 0:
        player.playToEnd(10, 5000);
        break;
      case 1:
        player.playToEnd(12, 5000);
        break;
      case 2:
        player.playToEnd(13, 5000);
        break;
      case 3:
        player.playToEnd(14, 5000);
        break;
      default:
        // do something
        return;
  }*/
  // int i = waitForPutClothes();
  // 
  // Serial.println(i);
  delay(1000);
  foldClothes();
}
/**
 * setup for initial
 */
void setup() {
  // put your setup code here, to run once:
  // 电机 舵机 初始化  
  servoPlate.attach(PLATE_PIN);   // 舵机 ---> PORT 9
  servoPlate.write(SERVO_POS_INIT);
  motor1.attach(MOTOR1_RUN_A, MOTOR1_DIR_B, LIMIT_LEFT );
  motor2.attach(MOTOR2_RUN_A, MOTOR2_DIR_B, LIMIT_RIGHT);
  motor3.attach(MOTOR3_RUN_A, MOTOR3_DIR_B, LIMIT_BACK );

#ifndef _TEST_
  motor1.runToLimit(BACKWARD, 3000);
  delay(1000);
  motor2.runToLimit(BACKWARD, 3000);
  delay(1000);
  motor3.runToLimit(BACKWARD, 3000);
  delay(1000);
#endif
  // MP3 初始化
  player.init();
  // 传感器初始化
  pinMode(DETECT_PIN , INPUT);

  TSC_Init();
  // TODO TSC_Close;
  delay(1000);  
  state = TEST;
}
/**
 * loop main
 */
void loop() {
  // put your main code here, to run repeatedly:
  
  // 鐘舵�佹満杞崲
  Serial.println(state);
  switch(state)
  {
    case TEST:
      test();
    break;
    case INIT:
      state = READY;
    break;
    case READY:
      if(waitForPutClothes()) state = /*COLOR*/FOLD;
    break;
    case COLOR:
      if(detectClothesColor()) state = FOLD;
      else state = READY;
    break;
    case FOLD:
      foldClothes();
      state = WHIRL;
    break;
    case WHIRL:
      whirlPlate();   // to color position
      state = ENDING;
    break;
    case ENDING:
      whirlPlateToInit();   // to init position
      state = READY;
    break;
    default:
      state = INIT;    
  } // end of switch
} // end of loop

/**
 * 鍙犺。鏈嶅彔鏉挎帶鍒剁▼搴�
 */
void foldClothes()
{
  // motor1.runToLimit(FORWARD, 1000);
  motor1.runToTime(FORWARD, 600);
  delay(1100);
  motor1.runToLimit(BACKWARD, 700);
  delay(1100);
  motor2.runToTime(FORWARD, 550);
  delay(1100);
  motor2.runToLimit(BACKWARD, 700);
  delay(1100);
  motor3.runToTime(FORWARD, 550);
  delay(1100);
  motor3.runToLimit(BACKWARD, 1000);
  delay(1100);
}
/**
 *
 */
uint8_t detectClothesColor()
{
  colorClothes = 0;
  // if(LOW == digitalRead(DETECT_PIN)) return 0;
  // else
  {
    colorClothes = getColor();
    Serial.println(colorClothes);
    return colorClothes;
  } 
}
/**
 * 鏃嬭浆鏀惰。杞洏
 */
void whirlPlate()
{
  switch (colorClothes) {
    case 1:
      servoPlate.write(SERVO_POS_LEFT);
      delay(1000);
      break;
    case 2:
      servoPlate.write(SERVO_POS_RIGHT);
      delay(1000);
      break;
    case 3:
      servoPlate.write(SERVO_POS_BACK);
      delay(2000);
      break;
    default:
      servoPlate.write(SERVO_POS_INIT);
      delay(1000);
      return;      
  }  
}
/**
 * 
 */
void whirlPlateToInit()
{
  servoPlate.write(SERVO_POS_INIT);
  switch (colorClothes) {
    case 1:      
    case 2:      
      delay(1000);
      break;
    case 3:      
      delay(2000);
      break;
    default:      
      return;      
  } 
  colorClothes = 0; // init color 
}
/**
 *
 *
 */
bool waitForPutClothes()
{
  static uint8_t waitState = 0;  // 0 -- open
                            // 1 -- wait to confirm
                            // 2 -- confirm close  
  static unsigned int _waitTime;
  switch(waitState)
  {
  case 0:
    if (LOW == digitalRead(DETECT_PIN))
    {
      _waitTime = millis();
      waitState = 1;
    }
  break;
  case 1:
    if (LOW == digitalRead(DETECT_PIN))  // 继续高位
    {
      if (millis() - _waitTime >= 20)
      {
        waitState = 2;
      }
    }
    else waitState = 0;
  break;
  case 2:      
  break;
  default:
  return false;
  }
  if(2 == waitState)
  {
    waitState = 0;
    return true;
  }
  else
  {
    return false;
  }
}
