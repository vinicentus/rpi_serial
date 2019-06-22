import 'dart:async';

import 'package:rpi_serial/serial.dart';
import 'package:rpi_serial/rpi_serial.dart';
import 'package:test/test.dart';

import '../example/arduino_serialpassthrough.dart';

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

  test('loopback test with most of the ASCII characters', () {
    final List<String> values = [" ", "!", '"', "#", r"$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?", "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", r"\", "]", "^", "_", "`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~"];
    List<String> receivedValues = [];
    for (String value in values) {
      fake_arduino.writeChar(value);
      receivedValues.add(fake_arduino.readChar());
    }

    expect(receivedValues, values);
  });

  test('string', () async {
    String startString = "This is a string... 12345";
    List<String> result = [];
    fake_arduino.writeStr(startString);
    await Future.delayed(const Duration(milliseconds: 10));
    while (fake_arduino.dataAvail() >= 1) {
      result.add(fake_arduino.readChar());
    }
    expect(result.join(), startString);
  });

  test('test flush and dataAvail', () async {
    fake_arduino.flush();
    expect(fake_arduino.dataAvail(), 0);

    fake_arduino.writeChar("a");
    await Future.delayed(const Duration(milliseconds: 10));
    expect(fake_arduino.dataAvail(), 1);
  });
}
