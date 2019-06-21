import 'dart:async';

import 'package:rpi_serial/rpi_serial.dart';

import 'mpl3115a2.dart';

main() async {
  final i2c = new RpiI2C();
  await readSensor(new Mpl3115a2(i2c));
  i2c.dispose();
}

Future readSensor(Mpl3115a2 mpl3115a2) async {
  Mpl3115a2Data result;

  print('Pressure and temperature:');
  result = await mpl3115a2.read();
  print('  pressure: ${result.pressure} pascals');
  print('  temperature: ${result.temperature} celsius');

  print('Altitude and temperature:');
  result = await mpl3115a2.read(altitude: true);
  print('  altitude: ${result.altitude} meters');
  print('  temperature: ${result.temperature} celsius');
}
