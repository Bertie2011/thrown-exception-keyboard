#include <stdint.h>
#include <stdbool.h>
#include "keycodes.h"
#include "action.h"

#if defined(CONSOLE_ENABLE) && defined(DEBUG_X_OSM)
    #define CONSOLE_X_OSM
#endif

bool x_osm(uint16_t keycode, bool pressed, uint16_t time, uint16_t ignore_key);
void x_osm_clear(void);
