import 'package:rpi_serial/serial.dart';
import 'package:rpi_serial/rpi_serial.dart';
import 'package:test/test.dart';

import 'test_util.dart';

main() {
  final serial = RpiSerial();
  runTests(serial);
  test('dispose', () => serial.dispose());
}

runTests(Serial serial) {
  test('exceptions', () async {
    // Only one instance of Serial
    await expectThrows(() => RpiSerial());
  });
}
