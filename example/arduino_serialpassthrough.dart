//TODO: make it easier to communicate, in fact, let's make it so easy that we don't need to have a device class for simple serial devices, but make it so that they can be used for more complex ones, such as gps receivers etc.

/// Use the SerialPassthrough sketch on any arduino with the right voltage level.
/// Connect it up and get going. TODO: write more.

import 'dart:convert';

import 'package:rpi_serial/serial.dart';

/// Any Arduino, set up to forward serial messages from pc to raspberry pi and vice versa
class Arduino {
  //TODO: change to RpiSerialDevice?
  final SerialDevice device;

  //TODO: change to basic device
  Arduino(Serial serial, [int baudRate = 9600])
      //https://spellfoundry.com/2016/05/29/configuring-gpio-serial-port-raspbian-jessie-including-pi-3/
      : device =
            serial.device(/* port */ "/dev/ttyS0", /* baudrate */ baudRate) {
    //say hello
    //device.serialPuts("Hello World!");
  }

  //TODO: maybe wait for data available
  /// Returns a single ASCII character of data
  String readChar() {
    return AsciiDecoder().convert([device.serialGetchar()]);
  }

  /// Returns a single Byte of data, represented as an [int]
  int readByte() {
    return device.serialGetchar();
  }

  //TODO: add tests for writeChar and writeByte
  /// Writes a single ASCII character of data
  writeChar(String char) {
    device.serialPutchar(AsciiEncoder().convert(char)[0]);
  }

  /// Writes a single Byte of data, represented as an [int]
  writeByte(int byte) {
    device.serialPutchar(byte);
  }

  flush() {
    device.serialFlush();
  }

  int dataAvail() {
    return device.serialDataAvail();
  }
}
