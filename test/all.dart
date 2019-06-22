import 'package:rpi_serial/rpi_serial.dart';
import 'package:test/test.dart';

import 'basic_test.dart' as basic;
import 'loopback_test.dart' as loopback;

main() {
  final serial = new RpiSerial();
  group('basic', () => basic.runTests(serial));
  group('deviceTests', () => loopback.runTests(serial));
  test('dispose', () => serial.dispose());
}
