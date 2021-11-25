#pragma once


#ifdef _WIN32
//Include httplib.h before Windows.h or include Windows.h by defining WIN32_LEAN_AND_MEAN beforehand.
#include <httplib.h>
#include <Windows.h>
#else // not _WIN32
#include <httplib.h>
#endif

//git clone https://github.com/yhirose/cpp-httplib.git cpp-httplib.github
