// this sketch plots the data delivered by mpu6050 sensor to the ESP32
// serial output is combinable with "teleplot" plotter
// use included teleplot json profile
//
// avoid using ESP32 default I2C pins with naked baseboard pcbs.
// unlike bigger dev boards, barebone modules do not support i2c on default pins, because they lack external periphery (resistors an capacitors).
// if default I2C pinout of ESP32: GPIO 22 - SCL // GPIO 21 - SDA is used in barebone configuration boot up problems may occur.
// tested with ESP32-WROOM-DEVKIT-V1 as well as RAZOM motion capture protoboard
// pwm controll on board led
// response to push buttons
// press and hold boot button while momentarly pushing enable during upload
// green led is connected via 150-220R resistor to GPIO2

#include <Arduino.h>
#include "Wire.h"
#include <MPU6050_light.h>

#define LED_BUILTIN 2

// PWM LED MODE PARAMETERS
const int PWMfreq = 5000;
const int PWMledChannel = 0;
const int PWMresolution = 8;

// PARAMETERS
int delayMillis = 10;
int angleMapMax = 500;
int angleZeroRange = 5;

MPU6050 mpu(Wire);
unsigned long timer = 0;
void recalibrate()
{
  Serial.println("DO NOT MOVE!");
  Serial.println("RECALIBRATION AFTER 2 SECONDS");
  for (byte i = 0; i < 255; i++)
  {
    ledcWrite(PWMledChannel, i);
    delay(2);
  }
  for (byte i = 255; i > 0; i--)
  {
    ledcWrite(PWMledChannel, i);
    delay(2);
  }
  mpu.calcOffsets(true, true); // gyro and accelero
  Serial.println("RECALIBRATION DONE");
}
void setup()
{
  Serial.begin(115200);
  delay(1000);

  // PWM LED INIT
  ledcSetup(PWMledChannel, PWMfreq, PWMresolution); // configure LED PWM functionalitites
  ledcAttachPin(LED_BUILTIN, PWMledChannel);        // attach the channel to the GPIO to be
  // ACTIVATE PULLUPS AT YET UNUSED BUTTONS OF RAZOM PROJECTBOARDSFOR SAFETY PURPOSES
  pinMode(4, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);

  Wire.begin();
  // Wire.begin(18, 19); //UNCOMMENT IF  SECONDARY I2C PINS ARE USED. (SDA GPIO18 // SCL GPIO19)
  byte status = mpu.begin(0, 0);
  Serial.print("MPU6050 status: ");
  Serial.println(status);
  while (status != 0)
  {
  } // stop everything if could not connect to MPU6050
  recalibrate();
  delay(250);
  mpu.update();
  delay(250);
  recalibrate();
}

void loop()
{
  // BUTTON TEST
  if (digitalRead(4) == LOW)
  {
    Serial.println("BUTTON USER_0 WAS PUSHED");
    for (byte i = 0; i < 10; i++)
    {
      ledcWrite(PWMledChannel, 255);
      delay(20);
      ledcWrite(PWMledChannel, 0);
      delay(20);
    }
    recalibrate();
  }
  if (digitalRead(19) == LOW)
  {
    Serial.println("BUTTON USER_1 WAS PUSHED");

    for (byte i = 0; i < 2; i++)
    {
      ledcWrite(PWMledChannel, 255);
      delay(250);
      ledcWrite(PWMledChannel, 0);
      delay(250);
    }
    recalibrate();
  }
  mpu.update();

  if ((millis() - timer) > delayMillis)
  { // print data every 10ms

    int currTemp = mpu.getTemp();

    int currAngleX = constrain(round(mpu.getAngleX() * 12), -2000, 2000); // angle x;
    int currAngleY = constrain(round(mpu.getAngleY() * 12), -2000, 2000); // angle y;
    int currAngleZ = round(mpu.getAngleZ());                              // angle z;

    // int currAccX = round(mpu.getAccX() * 325); // acceleration x factors: default mpu.begin values
    // int currAccY = round(mpu.getAccY() * 325); // acceleration y factors: default mpu.begin values
    // int currAccZ = round(mpu.getAccZ() * 227); // acceleration z factors: default mpu.begin values
    int currAccX = constrain((mpu.getAccX() * 250), -1000, 1000); // acceleration x
    int currAccY = constrain((mpu.getAccY() * 250), -1000, 1000); // acceleration y
    int currAccZ = constrain((mpu.getAccZ() * 250), -1000, 1000); // acceleration z

    int currGyroX = constrain(round(mpu.getGyroX() * 3.74), -1000, 1000); // gyro x
    int currGyroY = constrain(round(mpu.getGyroY() * 3.74), -1000, 1000); // gyro y
    int currGyroZ = constrain(round(mpu.getGyroZ() * 3.74), -1000, 1000); // gyro z

    int currAccAngleX = constrain(round(mpu.getAccAngleX() * 5.5), -1000, 1000);  // acc angle x aka ROLL
    int currAccAngleY = constrain(round(mpu.getAccAngleY() * 11.4), -1000, 1000); // acc angle y aka PITCH

    Serial.print(">AngleX: ");
    Serial.print(currAngleX);
    Serial.println();
    Serial.print(">AngleY: ");
    Serial.print(currAngleY);
    Serial.println();
    Serial.print(">AngleZ: ");
    Serial.println(currAngleZ);
    Serial.println();

    Serial.print(">AccX: ");
    Serial.print(currAccX);
    Serial.println();
    Serial.print(">AccY: ");
    Serial.print(currAccY);
    Serial.println();
    Serial.print(">AccZ: ");
    Serial.println(currAccZ);
    Serial.println();

    Serial.print(">GyroX: ");
    Serial.print(currGyroX);
    Serial.println();
    Serial.print(">GyroY: ");
    Serial.print(currGyroY);
    Serial.println();
    Serial.print(">GyroZ: ");
    Serial.println(currGyroZ);
    Serial.println();

    Serial.print(">Roll: ");
    Serial.print(currAccAngleX);
    Serial.println();
    Serial.print(">Pitch: ");
    Serial.print(currAccAngleY);
    Serial.println();

    timer = millis();
  }
}