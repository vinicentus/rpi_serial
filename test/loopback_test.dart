import 'dart:async';

import 'package:rpi_serial/serial.dart';
import 'package:rpi_serial/rpi_serial.dart';
import 'package:test/test.dart';

import '../example/arduino_serialpassthrough.dart';

import 'test_util.dart';

main() {
  final serial = RpiSerial();
  runTests(serial);
  test('dispose', () => serial.dispose());
}

runTests(Serial serial) {
  BasicSerialDevice device;

  //TODO: add tests for writeChar and writeByte

  test('instantiate once', () async {
    device = BasicSerialDevice(serial);
    await expectThrows(() => BasicSerialDevice(serial));
  });

  test('loopback test with most of the ASCII characters', () {
    final List<String> values = [" ", "!", '"', "#", r"$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?", "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", r"\", "]", "^", "_", "`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~"];
    List<String> receivedValues = [];
    for (String value in values) {
      device.writeChar(value);
      receivedValues.add(device.readChar());
    }

    expect(receivedValues, values);
  });

  test('test flush and dataAvail', () async {
    device.flush();
    expect(device.dataAvail(), 0);

    device.writeChar("a");
    await Future.delayed(const Duration(milliseconds: 10));
    expect(device.dataAvail(), 1);
  });
}
