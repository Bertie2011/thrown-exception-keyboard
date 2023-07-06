# Zones
While designing my keymap I quickly realized that the modifier keys won't fit on the base layer (I didn't like the timing issues of home row mods). I didn't want to move them to a separate layer either, because that would cause too much layer switching.

Introducing the concept of zones: Each zone is another layer and a group of zone layers can completely cover the keyboard (as if it was just 1 layer). Using custom code pressing a key in one zone, will turn the others off. That way each zone can be used repeatedly, while always having access to the base layer when you need it, without having to deactivate the zone layer explicitly.

Let's assume we have a nav bar zone (delete, tab, esc), a mod zone and we fill the remaining space with special characters. 
![Symbols Layer](/docs/assets/keymap/symbols-layer.png)

The zone concept makes the following example sequences work:

* Delete characters and type new text:
  * `SYM` (turns all zones on)
  * `DEL` (causes symbols and mod zones to turn off)
  * `DEL` (repeat as much as needed, the navbar zone is still available)
  * start typing (now the navbar zone turns off)
  * the base layer is completely uncovered now, all zones have been turned off
* Save all files in editor:
  * `SYM` (turns all zones on)
  * `CTRL` (causes navbar and symbols zones to turn off)
  * `SHIFT` (mod zone was still on)
  * `S` (now mod zone turns off as well)
  * the base layer is completely uncovered now, all zones have been turned off

## Implementing into your keymap
Unfortunately I haven't written a generic reusable solution for this yet, but here are some pointers to get started.

First start by defining a new layer with keycodes for each of the zones you have in mind. Make sure to fill overlapped slots with `_______` (transparent), so that the zone layers can be stacked on top of each other to form one full layer.

Make sure to copy [the layer detection](/qmk-lib/utility-functions/) functions into your keymap.

Then add a [custom keycode](https://docs.qmk.fm/#/custom_quantum_functions?id=custom-keycodes) that toggles all your layers on and off.

```C
enum keys {
    CC_LAYER_SYM = SAFE_RANGE,
};

enum layers {
    LAYER_BASE = 0,
    // ...
    LAYER_SYMBOLS,
    LAYER_NAV_BAR,
    LAYER_MODS
};

// Layers use bitflags, which is basically saying that each bit (0 or 1) in a number is used as a boolean indicator (a flag).
// The line below creates a bitflag where the bits for each of zones is turned on.
const int LAYER_SYM_MASK = (1 << LAYER_SYMBOLS) | (1 << LAYER_NAV_BAR) | (1 << LAYER_MODS);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    bool pressed = record->event.pressed;

    if (pressed && keycode == CC_LAYER_SYM) {
        if (layer_mask_enabled_any(LAYER_SYM_MASK)) {
            layer_and(~LAYER_SYM_MASK);
        }
        else layer_or(LAYER_SYM_MASK);
        return false;
    }
    
    return true;
}
```

> Now would be a good time to test!

Before continuing, make sure that you have a good understanding of how the position of a keycode in the keymap relates to the row/column indexes. Read up on it [here](/qmk-lib/understanding-key-indexes.md).

Now we can implement code to turn each of the zones off automatically. Note that deactivating a zone layer with `pressed == true` will deactivate the layer before QMK has a chance to find the keycode that belongs to the key. Deactivating with `pressed == false` (on release) would still allow that layer to be used for finding the related keycode.

```C
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    bool pressed = record->event.pressed;

    bool releaseBeforePress;
    bool releaseAfterPress;

    releaseBeforePress = ...; // Which keys will cause the layer to deactivate before being handled
    releaseAfterPress = ...; // Which keys will cause the layer to deactivate after being handled

    if (layer_enabled(<zone layer>) && keycode != CC_LTG_SYM && <key check>) {
        layer_off(<zone layer>);
    }

    // ... the custom keycode implementation should be here ...
}
```

Now replace `<key check>` with one of the following examples, or come up with your own:

Boolean Condition | Description
--:|:--
`pressed == releaseBeforePress` | Any keys matched by `releaseBeforePress` will cause the layer to deactivate before processing the keys, all others will deactivate the layer on release. This can simulate a one shot layer.
`((pressed && releaseBeforePress) \|\| (!pressed && releaseAfterPress))`| This condition will make sure that all keys detected by `releaseBeforePress` will deactivate the layer first before processing the key press. Any other keys detected by `releaseAfterPress` will deactivate the layer after processing the key press. Any key presses detected by neither of boolean value will keep the layer activated. Each of the boolean values can just be set to `false` to not match any key presses.

