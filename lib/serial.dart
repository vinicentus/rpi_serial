//port == device?

/// Base Serial interface supported by all Serial implementations.
abstract class Serial {
  final _allocatedAddresses = <String>[];

  /// Check that the port can be used for Serial
  /// and that is has not already been allocated.
  /// This should be called by subclasses not clients.
  void allocatePort(String port) {
    if (_allocatedAddresses.contains(port)) {
      throw new SerialException('Already allocated', port);
    }
    _allocatedAddresses.add(port);
  }

  /// Return the [SerialDevice] for communicating with the device at [port].
  SerialDevice device(String port, int baud);

  /// Call dispose before exiting your application to cleanup native resources.
  void dispose();
}

/// A Serial Device
abstract class SerialDevice {
  final String port;
  final int baud;

  SerialDevice(this.port, this.baud);

  /// Returns the number of characters available for reading.
  int serialDataAvail();

  /// Returns the next character available on the serial device. This call will block for up to 10 seconds if no data is available
  int serialGetchar();

  /// Sends the single byte [char] to the serial device identified by the given file descriptor.
  void serialPutchar(int char);

  /// Sends the nul-terminated string [s] to the serial device identified by the given file descriptor.
  void serialPuts(String s);

  //TODO: implement
  //void  serialPrintf (String message);

  void serialFlush ();
}

/// Exceptions thrown by Serial.
class SerialException {
  final String message;
  final String port;
  final int errorNumber;

  SerialException(this.message, [this.port, this.errorNumber]);

  @override
  String toString() {
    String msg = message;
    if (port != null) msg = '$msg, port: $port';
    if (errorNumber != null) msg = '$msg, error: $errorNumber';
    return 'SerialException: $msg';
  }
}
