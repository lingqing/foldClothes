/********************************************************************
 * File Name: FoldClothes
 * Description: 鍙犺。鏉跨▼搴忥紝瀹屾垚鍙犺。鏈嶇殑宸ヤ綔
 * Author:  AndyLiang
 * Date :   2016-6-16 
 *******************************************************************/
 /* ---------- user define ------------- */  
// servo
#define SERVO_POS_INIT 1500
#define SERVO_POS_LEFT 700
#define SERVO_POS_RIGHT 2000
// #define SERVO_POS_LEFT 1000
#define SERVO_POS_BACK 2100

/* ---------- include ------------- */ 
#include <Servo.h>  // 鑸垫満搴�
#include "UserMotor.h"  // 鑷畾涔夌數鏈洪┍鍔�
#include "ColorSensor.h"  // 棰滆壊浼犳劅鍣�
#include "MusicPlayer.h"
#include "UserServo.h"

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

// #define SPEAK
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
  UserServo myServo;
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
 
  for(int i=0; i<3; i++){
    colorClothes = i + 1;
    Serial.println(i+1);
      whirlPlate();
  }
}
/**
 * setup for initial
 */
 void setup() {
  // put your setup code here, to run once:
  // 电机 舵机 初始化  
  // servoPlate.attach(PLATE_PIN);   // 舵机 ---> PORT 9
  // servoPlate.write(SERVO_POS_INIT);
  myServo.attach(PLATE_PIN);
  myServo.write(SERVO_POS_INIT);

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
  state = READY;
}
/**
 * loop main
 */
 void loop() {
  // put your main code here, to run repeatedly:
  
  // 鐘舵�佹満杞崲
  // Serial.println(state);
  switch(state)
  {
    case TEST:
    test();
    break;
    case INIT:
    #ifdef SPEAK
    player.playToEnd(1, 5000);
    player.playToEnd(20, 5000);
    #endif
    state = READY;
    break;
    case READY:
    if(waitForPutClothes()) state = COLOR;
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
  #ifdef SPEAK
  player.playToEnd(20, 15000);        // 开始叠衣服
  #endif
  // 
  motor1.runToTime(FORWARD, 600);
  delay(1100);
  motor1.runToLimit(BACKWARD, 800);
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
  if(HIGH == digitalRead(DETECT_PIN))
  {
    #ifdef SPEAK
    player.playToEnd(10, 15000);     // 没有检测到衣服
    #endif
    return 0;
  } 
  else
  {
    colorClothes = getColor();
    #ifdef SPEAK
    switch (colorClothes) {
      case 0:
      player.playToEnd(14, 5000);     // 颜色无法识别
      break;
      case 1:
      player.playToEnd(11, 5000);
      break;
      case 2:
      player.playToEnd(12, 5000);
      break;
      case 3:
      player.playToEnd(13, 5000);
      break;
      default:
        // do something
        break;
    }
    #endif
    return colorClothes;
  } 
}
/**
 * 鏃嬭浆鏀惰。杞洏
 */
 void whirlPlate()
 {
  #ifdef SPEAK
  player.play(18);     //     归类衣服
  delay(5000);
  #endif;  
  
  // static int i = 1;
  // if(i++ >=3) i = 1;
  // colorClothes = i;
  Serial.print("color is :");
  Serial.println(colorClothes);

  switch (colorClothes) {
    case 1:
    Serial.println("case 1");
    myServo.write(SERVO_POS_LEFT);
    delay(1500);
    break;
    case 2:
    Serial.println("case 2");
    myServo.write(SERVO_POS_RIGHT);
    delay(1500);
    break;
    case 3:
    Serial.println("case 3");
    myServo.write(SERVO_POS_BACK);
    delay(2500);
    break;
    default:
    Serial.println("case default");
    myServo.write(SERVO_POS_INIT);
    delay(1500);
    return;      
  }  
}
/**
 * 
 */
 void whirlPlateToInit()
 {
  #ifdef SPEAK
  player.playToEnd(19, 10000);
  #endif;
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
    #ifdef SPEAK
    player.play(17);    // 检测到衣服，开始识别颜色
    #endif 
    return true;
  }
  else
  {
    return false;
  }
}
