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

#include "include/dart_api.h"
#include "include/dart_native_api.h"

Dart_Handle HandleError(Dart_Handle handle) {
  if (Dart_IsError(handle)) {
    Dart_PropagateError(handle);
  }
  return handle;
}

//TODO: maybe include errno and this
/*
// The errno from the last I2C command
static volatile int64_t lastErrno = 0;

// Return the errno from the last I2C command
//int _lastError() native "lastError";
void lastError(Dart_NativeArguments arguments) {
  Dart_EnterScope();

  Dart_SetIntegerReturnValue(arguments, lastErrno);
  Dart_ExitScope();
}
*/

/*
 * serialOpen:
 *	Open and initialise the serial port, setting all the right
 *	port parameters - or as many as are required - hopefully!
 *********************************************************************************
 */

//TODO: const char *device, const int baud
int serialOpen(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));
  Dart_Handle arg2 = HandleError(Dart_GetNativeArgument(arguments, 2));

  const char *device;
  int64_t baud;
  HandleError(Dart_StringToCString(arg1, &device));
  HandleError(Dart_IntegerToInt64(arg2, &baud));

  int64_t result;

  struct termios options;
  speed_t myBaud;
  int status, fd;

  switch (baud)
  {
    case      50:	myBaud =      B50 ; break ;
    case      75:	myBaud =      B75 ; break ;
    case     110:	myBaud =     B110 ; break ;
    case     134:	myBaud =     B134 ; break ;
    case     150:	myBaud =     B150 ; break ;
    case     200:	myBaud =     B200 ; break ;
    case     300:	myBaud =     B300 ; break ;
    case     600:	myBaud =     B600 ; break ;
    case    1200:	myBaud =    B1200 ; break ;
    case    1800:	myBaud =    B1800 ; break ;
    case    2400:	myBaud =    B2400 ; break ;
    case    4800:	myBaud =    B4800 ; break ;
    case    9600:	myBaud =    B9600 ; break ;
    case   19200:	myBaud =   B19200 ; break ;
    case   38400:	myBaud =   B38400 ; break ;
    case   57600:	myBaud =   B57600 ; break ;
    case  115200:	myBaud =  B115200 ; break ;
    case  230400:	myBaud =  B230400 ; break ;
    case  460800:	myBaud =  B460800 ; break ;
    case  500000:	myBaud =  B500000 ; break ;
    case  576000:	myBaud =  B576000 ; break ;
    case  921600:	myBaud =  B921600 ; break ;
    case 1000000:	myBaud = B1000000 ; break ;
    case 1152000:	myBaud = B1152000 ; break ;
    case 1500000:	myBaud = B1500000 ; break ;
    case 2000000:	myBaud = B2000000 ; break ;
    case 2500000:	myBaud = B2500000 ; break ;
    case 3000000:	myBaud = B3000000 ; break ;
    case 3500000:	myBaud = B3500000 ; break ;
    case 4000000:	myBaud = B4000000 ; break ;

    default:
      //TODO: break out
      Dart_SetIntegerReturnValue(arguments, -2);
      Dart_ExitScope();
  }

  if ((fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
    //TODO: break out
    Dart_SetIntegerReturnValue(arguments, -1);
    Dart_ExitScope();

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

  Dart_SetIntegerReturnValue(arguments, fd);
  Dart_ExitScope();
}

/*
 * serialFlush:
 *	Flush the serial buffers (both tx & rx)
 *********************************************************************************
 */

void serialFlush(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t fd;
  HandleError(Dart_IntegerToInt64(arg1, &fd));

  tcflush(fd, TCIOFLUSH);

  //return 1 for now TODO: don't return anything?
  Dart_SetIntegerReturnValue(arguments, 1);
  Dart_ExitScope();
}

/*
 * serialClose:
 *	Release the serial port
 *********************************************************************************
 */

void serialClose(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t fd;
  HandleError(Dart_IntegerToInt64(arg1, &fd));

  close(fd);

  //return 1 for now TODO: don't return anything?
  Dart_SetIntegerReturnValue(arguments, 1);
  Dart_ExitScope();
}

/*
 * serialPutchar:
 *	Send a single character to the serial port
 *********************************************************************************
 */

//TODO: (const int fd, const unsigned char c)
void serialPutchar(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));
  Dart_Handle arg2 = HandleError(Dart_GetNativeArgument(arguments, 2));

  int64_t fd;
  //set to 1 for now, it appears to be a number (it's unsigned)
  unsigned char c = 1;
  int64_t tempInt;
  HandleError(Dart_IntegerToInt64(arg1, &fd));
  //TODO: find the right dart/c conversion method, then change to c
  HandleError(Dart_IntegerToInt64(arg2, &tempInt));

  write(fd, &c, 1);

  //return 1 for now TODO: don't return anything?
  Dart_SetIntegerReturnValue(arguments, 1);
  Dart_ExitScope();
}

/*
 * serialPuts:
 *	Send a string to the serial port
 *********************************************************************************
 */

//TODO: (const int fd, const char *s)
void serialPuts(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));
  Dart_Handle arg2 = HandleError(Dart_GetNativeArgument(arguments, 2));

  int64_t fd;
  char *s;
  HandleError(Dart_IntegerToInt64(arg1, &fd));
  //TODO: check that this is the right dart/c conversion method
  HandleError(Dart_StringToCString(arg2, s));

  write(fd, s, strlen(s));

  //return 1 for now TODO: don't return anything?
  Dart_SetIntegerReturnValue(arguments, 1);
  Dart_ExitScope();
}

//TODO: implement?
/*
 * serialPrintf:
 *	Printf over Serial
 *********************************************************************************
 */

/*
 * serialDataAvail:
 *	Return the number of bytes of data avalable to be read in the serial port
 *********************************************************************************
 */

int serialDataAvail(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t fd;
  HandleError(Dart_IntegerToInt64(arg1, &fd));

  //TODO: make it int64_t?
  int result;

  if (ioctl(fd, FIONREAD, &result) == -1)
    result = -1;

  //check that this return works
  Dart_SetIntegerReturnValue(arguments, result);
  Dart_ExitScope();
}

/*
 * serialGetchar:
 *	Get a single character from the serial device.
 *	Note: Zero is a valid character and this function will time-out after
 *	10 seconds.
 *********************************************************************************
 */

int serialGetchar(Dart_NativeArguments arguments)
{
  Dart_EnterScope();
  Dart_Handle arg1 = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t fd;
  HandleError(Dart_IntegerToInt64(arg1, &fd));

  int64_t result = 0;

  uint8_t x;

  if (read(fd, &x, 1) != 1)
    result = -1;

  result = ((int)x) & 0xFF;

  //check that this return works
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
    {"serialOpen", serialOpen},
    {"serialFlush", serialFlush},
    {"serialClose", serialClose},
    {"serialPutchar", serialPutchar},
    {"serialPuts", serialPuts},
    //TODO: {"serialPrintf", serialPrintf},
    {"serialDataAvail", serialDataAvail},
    {"serialGetchar", serialGetchar},
    {NULL, NULL}};

FunctionLookup no_scope_function_list[] = {
    {NULL, NULL}};

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
DART_EXPORT Dart_Handle rpi_i2c_ext_Init(Dart_Handle parent_library)
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
