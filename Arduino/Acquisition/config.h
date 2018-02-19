#ifndef HEADER_CONFIG
#define HEADER_CONFIG

#include <DueFlashStorage.h>
#include <efc.h>
#include <flash_efc.h>

struct configuration
{
  uint8_t check = 1;
  float slot_step = 10.;
};

configuration readConfiguration()
{
  DueFlashStorage flash;
  byte bytes[sizeof(configuration)];
  for (int i = 0; i < sizeof(configuration); ++i)
    bytes[i] = flash.read(i);
  configuration c;
  memcpy(&c, bytes, sizeof(configuration));
  return c;
}

void write_configuration(configuration c)
{
  DueFlashStorage flash;
  byte bytes[sizeof(configuration)];
  memcpy(bytes, &c, sizeof(configuration));
  flash.write(0, bytes, sizeof(configuration));
}

#endif
