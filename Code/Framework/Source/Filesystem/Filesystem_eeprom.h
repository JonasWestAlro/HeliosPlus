#ifndef FILESYSTEM_EEPROM
#define FILESYSTEM_EEPROM

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void eeprom_write(void* offset, void* data, uint32_t size);
void eeprom_read(void* buffer,  void* offset, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
