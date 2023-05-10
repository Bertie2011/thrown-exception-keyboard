//#define DEBUG_X_OSM
#include "x_osm.h"

#ifdef CONSOLE_X_OSM
    #include "print.h"

    #define X_OSM_PRINT(text, value) {\
        print(text);\
        print(": ");\
        print_bin8(value);\
        print("\n");\
    }

#else
    #define X_OSM_PRINT(text, value)

#endif


uint8_t x_osm_state_pressed = 0; // Bit flag for Gui, Alt, Shift, Control, repeated for right, left.
uint8_t x_osm_state_sticky = 0;
uint16_t x_osm_last_press = 0;


bool x_osm(uint16_t keycode, bool pressed, uint16_t time, uint16_t reset_key) {
    bool is_one_shot = IS_QK_ONE_SHOT_MOD(keycode);
    bool is_trigger = keycode == reset_key || IS_QK_BASIC(keycode) || IS_QK_MODS(keycode);

    // If the 5th bit indicates left (0), use 4 bits as-is, otherwise move them to the left by 4 spaces.
    uint8_t mod_mask = (uint8_t) ((keycode >> 4) & 1) == 0 ? keycode & 0b1111 : (keycode & 0b1111) << 4;

    #ifdef CONSOLE_X_OSM
    if (is_one_shot) X_OSM_PRINT("Got one shot, with mod mask", mod_mask);
    #endif

    if (is_one_shot && pressed) {
        // Toggle sticky-ness and make sure to make the active mods mirror that.
        x_osm_last_press = time;
        x_osm_state_pressed |= mod_mask;
        x_osm_state_sticky ^= mod_mask;
        register_mods(mod_mask & x_osm_state_sticky);
        X_OSM_PRINT("Marked as Pressed", x_osm_state_pressed);
        X_OSM_PRINT("Marked as Sticky", x_osm_state_sticky);
        X_OSM_PRINT("Registered", mod_mask & x_osm_state_sticky);
        return false;
    } else if (is_one_shot && !pressed) {
        // Mark mods as unstuck if time has expired
        x_osm_state_pressed &= ~mod_mask;
        if (time - x_osm_last_press > TAPPING_TERM) x_osm_state_sticky &= ~mod_mask;

        // Deactivate mods that are not sticky (because they were marked as unstuck due to delay or trigger key, but not deactivated yet)
        unregister_mods(mod_mask & ~x_osm_state_sticky);
        X_OSM_PRINT("Marked as Pressed", x_osm_state_pressed);
        X_OSM_PRINT("Marked as Sticky", x_osm_state_sticky);
        X_OSM_PRINT("Unregistered", mod_mask & ~x_osm_state_sticky);
        return false;
    } else if (is_trigger) {
        bool discard_event = keycode == reset_key && x_osm_state_sticky != 0 && x_osm_state_pressed == 0;

        if (!pressed) {
            x_osm_clear();
        }
        return !discard_event;
    }

    return true;
}

void x_osm_clear(void) {
    // Remove sticky-ness and deactivate mods that are not pressed (those categorized as one-shot)
    X_OSM_PRINT("Marked as Pressed", x_osm_state_pressed);
    X_OSM_PRINT("No longer marked as Sticky", x_osm_state_sticky);
    X_OSM_PRINT("Unregistered", x_osm_state_sticky & ~x_osm_state_pressed);
    #ifdef CONSOLE_X_OSM
        xprintf("Key event discarded: %u\n", discard_event);
    #endif

    unregister_mods(x_osm_state_sticky & ~x_osm_state_pressed);
    x_osm_state_sticky = 0;
}
