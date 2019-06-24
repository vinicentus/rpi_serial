import 'package:rpi_serial/rpi_serial.dart';

import 'arduino_serialpassthrough.dart';

main() async {
  final serial_connection = new RpiSerial();
  await testSensor(new Arduino(serial_connection));
  serial_connection.dispose();
}

testSensor(Arduino arduino) {
  print(
      "Entering endless loop mode. Please connect a cable between the rx and tx pins.");
  while (true) {
    //TODO: add input here, then send that
    print(arduino.readChar());
  }
}
