#ifdef IOP_POSIX_MOCK
#include "driver/cpp17_mock.hpp"
#elif defined(IOP_ESP8266)
#include "driver/arduino.hpp"
#elif defined(IOP_ESP32)
#include "driver/arduino.hpp"
#elif defined(IOP_NOOP)
#include "driver/noop.hpp"
#else
#error "Target not supported"
#endif