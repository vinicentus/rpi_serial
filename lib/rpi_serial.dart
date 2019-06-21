import 'package:rpi_serial/serial.dart';

//TODO: change name of external lib
import 'dart-ext:rpi_i2c_ext';

/// The [Serial] interface used for accessing Serial devices on the Raspberry Pi.
class RpiSerial extends Serial {
  static bool _instantiatedSerial = false;

  final _devices = <RpiSerialDevice>[];

  RpiSerial() {
    if (_instantiatedSerial) {
      throw new SerialException('Serial already instantiated');
    }
    _instantiatedSerial = true;
  }

  @override
  SerialDevice device(String port, int baud) {
    allocatePort(port);
    int fd = _serialOpen(port, baud);
    if (fd < 0) throw new SerialException('device init failed: $fd');
    final device = new RpiSerialDevice(port, baud, fd);
    _devices.add(device);
    return device;
  }

  @override
  void dispose() {
    while (_devices.isNotEmpty) {
      int result = _serialClose(_devices.removeLast()._fd);
      if (result != 0) throw new SerialException('dispose failed: $result');
    }
  }

  int _serialOpen(String port, int baud) native "serialOpen";
  int _serialClose(int fd) native "serialClose";
}

class RpiSerialDevice extends SerialDevice {
  final int _fd;

  RpiSerialDevice(String port, int baud, this._fd) : super(port, baud);

  @override
  int serialDataAvail() {
    return _throwIfNegative(_serialDataAvail(_fd));
  }

  @override
  int serialGetchar() => _throwIfNegative(_serialGetchar(_fd));

  @override
  void serialPutchar(int char) {
    _throwIfNegative(_serialPutchar(_fd, char));
  }

  @override
  void serialPuts(String s) {
    _throwIfNegative(_serialPuts(_fd, s));
  }

  /// Throw an exception if [value] is less than zero, else return [value].
  int _throwIfNegative(int value) {
    if (value < 0) {
      throw new SerialException(
          'operation failed: $value', port, _lastError(value));
    }
    return value;
  }

  //TODO: implement
  int _lastError(int value) => value;
  int _serialDataAvail(int fd) native "serialDataAvail";
  int _serialGetchar(int fd) native "serialGetchar";
  //TODO: find better type
  int _serialPutchar(int fd, int c) native "serialPutchar";
  int _serialPuts(int fd, String s) native "serialPuts";
}
