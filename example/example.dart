import 'package:rpi_serial/rpi_serial.dart';

import 'arduino_serialpassthrough.dart';

main() async {
  final serial_connection = new RpiSerial();
  await testSensor(new Arduino(serial_connection));
  serial_connection.dispose();
}

testSensor(Arduino arduino) {
  print("Writing 1");
  arduino.writeInt(1);

  print("Reading");
  print(arduino.readInt());
}
