import '../example/arduino_serialpassthrough.dart';
import 'package:rpi_serial/serial.dart';
import 'package:rpi_serial/rpi_serial.dart';
import 'package:test/test.dart';

import 'test_util.dart';

main() {
  final serial = new RpiSerial();
  runTests(serial);
  test('dispose', () => serial.dispose());
}

//TODO: change so that it doesn't use arduino class
runTests(Serial serial) {
  Arduino fake_arduino;

  test('instantiate once', () async {
    fake_arduino = new Arduino(serial);
    await expectThrows(() => new Arduino(serial));
  });

  test('loopback test with numbers', () async {
    final List<int> values = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
    List<int> receivedValues = [];
    for (int value in values) {
      fake_arduino.writeInt(value);
      receivedValues.add(fake_arduino.readInt());
    }

    expect(receivedValues.length, 10);
    expect(values, receivedValues);
  });
}