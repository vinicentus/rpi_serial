//TODO: check dependencies
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>

#include "include/dart_api.h"
#include "include/dart_native_api.h"

Dart_Handle HandleError(Dart_Handle handle) {
  if (Dart_IsError(handle)) {
    Dart_PropagateError(handle);
  }
  return handle;
}

// The errno from the last Serial command
static volatile int64_t lastErrno = 0;

/// Return the errno from the last Serial command
void lastError(Dart_NativeArguments arguments) {
  Dart_EnterScope();

  Dart_SetIntegerReturnValue(arguments, lastErrno);
  Dart_ExitScope();
}

/// Open and initialise the serial port, setting all the right
/// port parameters - or as many as are required - hopefully!
void serialOpen(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));
  Dart_Handle arg2 = HandleError(Dart_GetNativeArgument(arguments, 2));

  const char *device;
  int64_t baud;
  HandleError(Dart_StringToCString(arg1, &device));
  HandleError(Dart_IntegerToInt64(arg2, &baud));

  bool continue1 = true;
  bool continue2 = true;
  int64_t result;

  struct termios options;
  speed_t myBaud;
  int status, fd;

  switch (baud)
  {
  case      50:	myBaud =      B50; break;
  case      75:	myBaud =      B75; break;
  case     110:	myBaud =     B110; break;
  case     134:	myBaud =     B134; break;
  case     150:	myBaud =     B150; break;
  case     200:	myBaud =     B200; break;
  case     300:	myBaud =     B300; break;
  case     600:	myBaud =     B600; break;
  case    1200:	myBaud =    B1200; break;
  case    1800:	myBaud =    B1800; break;
  case    2400:	myBaud =    B2400; break;
  case    4800:	myBaud =    B4800; break;
  case    9600:	myBaud =    B9600; break;
  case   19200:	myBaud =   B19200; break;
  case   38400:	myBaud =   B38400; break;
  case   57600:	myBaud =   B57600; break;
  case  115200:	myBaud =  B115200; break;
  case  230400:	myBaud =  B230400; break;
  case  460800:	myBaud =  B460800; break;
  case  500000:	myBaud =  B500000; break;
  case  576000:	myBaud =  B576000; break;
  case  921600:	myBaud =  B921600; break;
  case 1000000:	myBaud = B1000000; break;
  case 1152000:	myBaud = B1152000; break;
  case 1500000:	myBaud = B1500000; break;
  case 2000000:	myBaud = B2000000; break;
  case 2500000:	myBaud = B2500000; break;
  case 3000000:	myBaud = B3000000; break;
  case 3500000:	myBaud = B3500000; break;
  case 4000000:	myBaud = B4000000; break;

  default:
    result = -2;
    lastErrno = errno;
    continue1 = false;
  }

  if (((fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1) && continue1)
  {
    result = -1;
    lastErrno = errno;
    continue2 = false;
  }

  if (continue2)
  {

    fcntl(fd, F_SETFL, O_RDWR);

    // Get and modify current options:

    tcgetattr(fd, &options);

    cfmakeraw(&options);
    cfsetispeed(&options, myBaud);
    cfsetospeed(&options, myBaud);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    //uses 8 data bits
    //TODO: limit other parts of software to this, or allow customization
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;

    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 100; // Ten seconds (100 deciseconds)

    tcsetattr(fd, TCSANOW, &options);

    ioctl(fd, TIOCMGET, &status);

    status |= TIOCM_DTR;
    status |= TIOCM_RTS;

    ioctl(fd, TIOCMSET, &status);

    usleep(10000); // 10mS

    result = fd;
    lastErrno = 0;
  }

  Dart_SetIntegerReturnValue(arguments, result);
  Dart_ExitScope();
}

/// Flush the serial buffers (both tx & rx)
void serialFlush(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t fd;
  HandleError(Dart_IntegerToInt64(arg1, &fd));

  tcflush(fd, TCIOFLUSH);

  //return 0 for now
  Dart_SetIntegerReturnValue(arguments, 0);
  Dart_ExitScope();
}

/// Release the serial port
void serialClose(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t fd;
  HandleError(Dart_IntegerToInt64(arg1, &fd));

  close(fd);

  //return 0 for now
  Dart_SetIntegerReturnValue(arguments, 0);
  Dart_ExitScope();
}

/// Send a single byte to the serial port
void serialPutchar(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));
  Dart_Handle arg2 = HandleError(Dart_GetNativeArgument(arguments, 2));

  int64_t fd;
  int64_t tempC;
  HandleError(Dart_IntegerToInt64(arg1, &fd));
  HandleError(Dart_IntegerToInt64(arg2, &tempC));
  //convert int64_t to uint8_T
  uint8_t c = tempC & 0xFF;

  write(fd, &c, 1);

  //return 0 for now
  Dart_SetIntegerReturnValue(arguments, 0);
  Dart_ExitScope();
}

/// Return the number of bytes of data avalable to be read in the serial port
void serialDataAvail(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t fd;
  HandleError(Dart_IntegerToInt64(arg1, &fd));

  //This should autmatically be converted to int_64t when needed
  int result;

  if (ioctl(fd, FIONREAD, &result) == -1)
    result = -1;
    lastErrno = errno;

  //TODO: move?
  lastErrno = 0;
  Dart_SetIntegerReturnValue(arguments, result);
  Dart_ExitScope();
}

/// Get a single byte from the serial device.
/// Note: This function will time-out after 10 seconds.
void serialGetchar(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t fd;
  HandleError(Dart_IntegerToInt64(arg1, &fd));

  int64_t result = 0;

  uint8_t x;

  if (read(fd, &x, 1) != 1)
    result = -1;
    lastErrno = errno;

  result = ((int)x) & 0xFF;
  lastErrno = errno;

  Dart_SetIntegerReturnValue(arguments, result);
  Dart_ExitScope();
}

// ===== Infrastructure methods ===============================================

struct FunctionLookup
{
  const char *name;
  Dart_NativeFunction function;
};

FunctionLookup function_list[] = {
    {"lastError", lastError},
    {"serialOpen", serialOpen},
    {"serialFlush", serialFlush},
    {"serialClose", serialClose},
    {"serialPutchar", serialPutchar},
    {"serialDataAvail", serialDataAvail},
    {"serialGetchar", serialGetchar},
    {NULL, NULL},
};

FunctionLookup no_scope_function_list[] = {
    {NULL, NULL},
};

// Resolve the Dart name of the native function into a C function pointer.
// This is called once per native method.
Dart_NativeFunction ResolveName(Dart_Handle name,
                                int argc,
                                bool *auto_setup_scope)
{
  if (!Dart_IsString(name))
  {
    return NULL;
  }
  Dart_NativeFunction result = NULL;
  if (auto_setup_scope == NULL)
  {
    return NULL;
  }

  Dart_EnterScope();
  const char *cname;
  HandleError(Dart_StringToCString(name, &cname));

  for (int i = 0; function_list[i].name != NULL; ++i)
  {
    if (strcmp(function_list[i].name, cname) == 0)
    {
      *auto_setup_scope = true;
      result = function_list[i].function;
      break;
    }
  }

  if (result != NULL)
  {
    Dart_ExitScope();
    return result;
  }

  for (int i = 0; no_scope_function_list[i].name != NULL; ++i)
  {
    if (strcmp(no_scope_function_list[i].name, cname) == 0)
    {
      *auto_setup_scope = false;
      result = no_scope_function_list[i].function;
      break;
    }
  }

  Dart_ExitScope();
  return result;
}

// Initialize the native library.
// This is called once when the native library is loaded.
DART_EXPORT Dart_Handle rpi_serial_ext_Init(Dart_Handle parent_library)
{
  if (Dart_IsError(parent_library))
  {
    return parent_library;
  }
  Dart_Handle result_code =
      Dart_SetNativeResolver(parent_library, ResolveName, NULL);
  if (Dart_IsError(result_code))
  {
    return result_code;
  }
  return Dart_Null();
}
