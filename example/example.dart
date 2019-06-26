import 'package:rpi_serial/rpi_serial.dart';

import 'basic_device.dart';

main() async {
  final serial_connection = RpiSerial();
  await testSensor(BasicSerialDevice(serial_connection));
  serial_connection.dispose();
}

testSensor(BasicSerialDevice arduino) {
  print(
      "Entering endless loop mode. Please connect a cable between the rx and tx pins.");
  while (true) {
    //TODO: add input here, then send that
    print(arduino.readChar());
  }
}
