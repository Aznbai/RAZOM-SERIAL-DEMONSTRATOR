# RAZOM-SERIAL-DEMONSTRATOR
![screenshot](teleplot-screenshot-1.jpg) 

This script is designed for plotting data from an MPU6050 sensor linked to an ESP32. It's tailored to work seamlessly with the "teleplot" plotter, and you should use the included teleplot JSON profile for optimal results.

A key consideration when using this sketch is the I2C pin configuration, especially with naked baseboard PCBs. Unlike more comprehensive dev boards, barebone modules like the ESP32 don't support the default I2C pins (GPIO 22 for SCL and GPIO 21 for SDA) due to the absence of external components like resistors and capacitors. Using these default pins on a barebone setup could lead to boot-up issues.

This script has been tested successfully with both the ESP32-WROOM-DEVKIT-V1 and the RAZOM motion capture protoboard. It includes PWM control for an onboard LED and responds to push-button inputs. When uploading, press and hold the boot button while briefly pushing the enable button. For the green LED, a 150-220R resistor connected to GPIO2 is recommended.

### Read More:
- https://razom.likhol.at
- https://razom.likhol.at/make/craft-your-own-tools-razom-monitor

![screen-capture](razom-monitor-node-teleplot-output-small.gif)
