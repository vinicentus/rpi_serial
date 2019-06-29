import 'dart:math';
import 'dart:async';
import 'package:rpi_serial/rpi_serial.dart';
import 'basic_device.dart';

main() async {
  final serial_connection = RpiSerial();
  await testSensor(BasicSerialDevice(serial_connection));
  serial_connection.dispose();
}

testSensor(BasicSerialDevice device) async {
  print(
      "Entering endless loop mode. Please connect a cable between the rx and tx pins.");
  while (true) {
    device.writeChar(Random().nextInt(10).toString());
    print(device.readChar());
    await Future.delayed(Duration(milliseconds: 500));
  }
}
