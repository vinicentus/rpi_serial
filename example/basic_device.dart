import 'dart:convert';
import 'package:rpi_serial/serial.dart';

/// Any basic serial device
class BasicSerialDevice {
  //TODO: change to RpiSerialDevice?
  final SerialDevice device;

  //initialize device and serial
  BasicSerialDevice(Serial serial, [int baudRate = 9600])
      //Set the port and baudrate (check README.md if you are unsure about this).
      : device = serial.device("/dev/ttyS0", baudRate);

  /// Returns a single ASCII character of data
  String readChar() {
    return AsciiDecoder().convert([device.serialGetchar()]);
  }

  /// Returns a single Byte of data, represented as an [int]
  int readByte() {
    return device.serialGetchar();
  }

  /// Writes a single ASCII character of data
  writeChar(String char) {
    device.serialPutchar(AsciiEncoder().convert(char)[0]);
  }

  /// Writes a single Byte of data, represented as an [int]
  writeByte(int byte) {
    device.serialPutchar(byte);
  }

  void sendMultipleBytes(List<int> list) {
    for (int byte in list) {
      device.serialPutchar(byte);
    }
  }

  /// Clears both the TX and RX buffers
  flush() {
    device.serialFlush();
  }

  /// Returns the number of bytes available to read on the serial port
  int dataAvail() {
    return device.serialDataAvail();
  }
}
