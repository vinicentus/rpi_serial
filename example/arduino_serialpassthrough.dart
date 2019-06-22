//TODO: make it easier to communicate, in fact, let's make it so easy that we don't need to have a device class for simple serial devices, but make it so that they can be used for more complex ones, such as gps receivers etc.

/// Use the SerialPassthrough sketch on any arduino with the right voltage level.abstract
/// Connect it up and get going. TODO: write more.

import 'package:rpi_serial/serial.dart';

/// Any Arduino, set up to forward serial messages from pc to raspberry pi and vice versa
class Arduino {
  final SerialDevice device;

  //TODO: change to basic device
  Arduino(Serial serial)
      //https://spellfoundry.com/2016/05/29/configuring-gpio-serial-port-raspbian-jessie-including-pi-3/
      : device = serial.device(/* port */ "/dev/ttyS0", /* baudrate */ 9600) {
    //say hello
    //device.serialPuts("Hello World!");
  }

  //TODO: maybe wait for data available
  String readChar() {
    return device.serialGetchar();
  }

  writeChar(String char) {
    device.serialPutchar(char);
  }

  flush() {
    device.serialFlush();
  }

  int dataAvail() {
    return device.serialDataAvail();
  }
}
