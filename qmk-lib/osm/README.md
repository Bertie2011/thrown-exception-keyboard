# One Shot Modifiers
At the time of writing, QMK one shot modifiers had some weird quirks. My one shot mod implementation has the following improvements:

* I didn't like the locking/timeout behaviour, because I wanted to be fully in control.
  * I did reintroduce the "skip one shot behaviour when held" feature, which comes in handy when using the mouse for example.
* Press a specific keycode (e.g. Escape) that would clear all active modifiers.
  * Hold modifiers to send a modified keycode (e.g. CTRL + Esc).
* Cancel a modifier by tapping it again (toggle behavior).
* Tap multiple modifiers and hold just the last one to apply them to multiple key presses after, instead of having to hold all the modifiers at once.
* Fixed a weird bug that was observed in the native QMK implementation where rolling modifiers could cause one of them to get stuck.
* This implementation uses "real" modifiers, which are registered by the operating system as soon as you activate them. (As opposed to QMK "weak" modifiers that are registered virtually and only sent to the system with the next key press.)

## How to use
* Start by adding the `x_osm.c` and `x_osm.h` files to your keymap folder.
* Add `SRC += x_osm.c` to your `rules.mk` file
* In `config.h`, add `#define TAPPING_TERM <value>` which indicates the amount of milliseconds after which a key press is considered a hold. The default value is 200, although I've been using 250 myself.
* In `keymap.c`, make sure to add:
    ```C
    #include "x_osm.h"

    bool process_record_user(uint16_t keycode, keyrecord_t *record) {
        bool pressed = record->event.pressed;

        // Replace KC_ESC by any keycode of your choice.
        // Pressing it will clear all modifiers and ignore the keycode,
        // unless the modifiers are held.
        if (!x_osm(keycode, pressed, record->event.time, KC_ESC)) return false;

        return true;
    }
    ```

* Use the `OSM(...)` keycode with [mod mask codes](https://docs.qmk.fm/#/feature_advanced_keycodes?id=checking-modifier-state) in your keymap to trigger the one shot modifiers.
* Use the `x_osm_clear()` function anywhere (e.g. in macros) to clear the one shot modifiers state at any time.
* To receive debug messages in the QMK console, add `CONSOLE_ENABLE = yes` in `rules.mk` and uncomment the `#define DEBUG_X_OSM` at the top of `x_osm.c`.