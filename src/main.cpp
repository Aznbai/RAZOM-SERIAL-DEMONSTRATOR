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
  mpu.calcOffsets(); // gyro and accelero
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
  byte status = mpu.begin();
  Serial.print("MPU6050 status: ");
  Serial.println(status);
  while (status != 0)
  {
  } // stop everything if could not connect to MPU6050
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
  int currTemp = mpu.getTemp();

  int currAngleX = round(mpu.getAngleX()); // angle x;
  int currAngleY = round(mpu.getAngleY()); // angle y;
  int currAngleZ = round(mpu.getAngleZ()); // angle z;

  int currAccX = round(mpu.getAccX() * 100); // acceleration x
  int currAccY = round(mpu.getAccY() * 100); // acceleration y
  int currAccZ = round(mpu.getAccZ() * 100); // acceleration z

  int currGyroX = round(mpu.getGyroX() * 10); // gyro x
  int currGyroY = round(mpu.getGyroY() * 10); // gyro y
  int currGyroZ = round(mpu.getGyroZ() * 10); // gyro z

  int currAccAngleX = round(mpu.getAccAngleX() * 10); // acc angle x
  int currAccAngleY = round(mpu.getAccAngleY() * 10); // acc angle y

  if ((millis() - timer) > 10)
  { // print data every 10ms
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