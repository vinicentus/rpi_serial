import 'package:rpi_serial/serial.dart';
import 'package:rpi_serial/rpi_serial.dart';
import 'package:test/test.dart';

import 'test_util.dart';

main() {
  final i2c = new RpiI2C();
  runTests(i2c);
  test('dispose', () => i2c.dispose());
}

runTests(I2C i2c) {
  test('exceptions', () async {
    // Only one instance of I2C factory
    await expectThrows(() => new RpiI2C());
  });
}
