#ifndef __DEV_DEBUG_H__
#define __DEV_DEBUG_H__

#include "stdio.h"

#define DEV_PRINT_DEBUG
#define DEV_PRINT_WARNING_ERROR

#ifdef DEV_PRINT_DEBUG
  #define Debug(__info,...) printf("Debug : " __info,##__VA_ARGS__)
#else
  #define DEBUG(__info,...)
#endif

#ifdef DEV_PRINT_WARNING_ERROR
  #define Warn(__info,...) printf("Warning : " __info,##__VA_ARGS__)
  #define Error(__info,...) printf("Error : " __info,##__VA_ARGS__)
#else
  #define Warn(__info,...)
  #define Error(__info,...)
#endif

#define Info(__info,...) printf("Info : " __info,##__VA_ARGS__)

#define Print(__info,...) printf(__info,##__VA_ARGS__)

#endif
