import 'package:rpi_serial/serial.dart';
import 'package:test/test.dart';

expectThrows(f()) async {
  try {
    await f();
    fail('expected exception');
  } on I2CException {
    // Expected... fall through
  }
}
