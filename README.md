# rpi_i2c.dart

rpi_serial is a Dart package for using serial communication on the Raspberry Pi.

## Overview

 * The [__serial__](lib/serial.dart) library provides the API for accessing devices
   using the [serial protocol](https://en.wikipedia.org/wiki/Serial_communication)

 * The [__RpiSerial__](lib/rpi_serial.dart) library provides implementation of
   the serial protocol on the Raspberry Pi derived from the [WiringPi](http://wiringpi.com/) library.

## Setup

Be sure to enable serial on the Raspberry Pi using
```
    sudo raspi-config
```
Also make sure to disable the console on the serial port.

[__RpiSerial__](lib/rpi_serial.dart) uses a native library written in C.
For security reasons, authors cannot publish binary content
to [pub.dev](https://pub.dev/), so there are some extra
steps necessary to compile the native library on the RPi before this package
can be used. These two steps must be performed when you install and each time
you upgrade the rpi_serial package.

1) Activate the rpi_serial package using the
[pub global](https://dart.dev/tools/pub/cmd/pub-global) command.
```
    pub global activate rpi_serial
```

2) From your application directory (the application that references
the rpi_i2c package) run the following command to build the native library
```
    pub global run rpi_serial:build_lib
```

3) Deactivate the rpi_serial package.
```
    pub global deactivate rpi_serial
```

[pub global activate](https://dart.dev/tools/pub/cmd/pub-global#activating-a-package)
makes the Dart scripts in the rpi_serial/bin directory runnable
from the command line.
[pub global run](https://dart.dev/tools/pub/cmd/pub-global#running-a-script)
rpi_serial:build_lib runs the [rpi_serial/bin/build_lib.dart](bin/build_lib.dart)
program which in turn calls the [build_lib](lib/src/native/build_lib) script
to compile the native librpi_serial_ext.so library for the rpi_i2c package.

## Connecting

Refer to [GPIO](https://www.raspberrypi.org/documentation/usage/gpio/) for connecting your serial device to the Raspberry Pi.
By default pins 8 is TX and 10 is RX on the GPIO header.

Note that the Raspberry Pi actually has two UARTs.
On the versions with Bluetooth (Raspberry Pi 3 and Raspberry Pi Zero W), the UART exposed to the GPIO pins is __"/dev/ttyS0"__, while it is __"/dev/ttyAMA0"__ on the rest of the boards.
This can be changed (see [Rapsberry Pi UART configuration](https://www.raspberrypi.org/documentation/configuration/uart.md)).
On the compute module, you can supposedly expose both serial interfaces on the GPIO.


## Examples

Info about examples is coming soon...
