# ThrownException Keymap
When doing a deep dive into small keyboard keymaps, a lot of them seem to rely on holding keys, whether that's for switching layers, using modifiers or simply fitting another special character on the same key. 

This didn't feel right for me, because when I decide I want to put something on the screen, I only want to to think about what to do and not how to do it. I want my input to be instant and not be dependent on any kind of delay. Also the effort of holding a key and pressing another seems to be more than the effort of pressing two or even three keys shortly.

## Philosophy
* Use the Colemak Mod-DH base layout for improved ergonomics
* Avoid holding keys at all costs for instant input and decreased effort
* Avoid thinking about which layers were activated earlier, only think about the next key.
  * All layers can be activated from any other layer, no need to disable a layer first
  * The [symbols layer](#symbols-layer) has a special implementation that allows repetition and base layer access at the same time. 
* Exclusive layers, only 1 extra layer can be active at a time

## Custom OSM implementation
The OSM keys let you press a modifiier (Ctrl, Shift, Alt, Meta) once to lock it into place until the next key is pressed. At the time of creating my keymap the native QMK implementation had some weird bugs going on when multiple modifiers were pressed or toggled in quick succession. To resolve this, I wrote my own implementation that also supports temporary hold and maintaining active mods across layers.

## Base Layer
The layer follows Colemak Mod-DH. The non-alpha keys have been filled by most frequently used characters. The thumb keys are occupied by a symbols layer key, space, backspace and shift. Both inner (home) thumb keys can be pressed together to activate the navigation layer. The outer thumb keys are rotary encoders with key press functionality.

![Base Layer](assets/keymap/base-layer.png)

## Symbols Layer

### Zones
The symbols layer actually consists of 3 partial layers (let's call them zones), that together fill the entire keyboard. Using custom code each zone can be turned off whenever a key outside that zone is pressed. That way each zone can be used repeatedly, while always having access to the base layer when you need it, without having to deactivate the symbols layer.

Note that ESC within the navbar zone also turns that zone off.

This makes the following example sequences work:
* Delete characters and type new text:
  * `SYM` (turns all zones on)
  * `DEL` (causes symbols and mod zones to turn off - hold or repeatedly press)
  * start typing (now the navbar zone turns off).
* Save all files in editor:
  * `SYM` (turns all zones on),
  * `CTRL` (causes navbar and symbols zones to turn off),
  * `SHIFT` (mod zone was still on),
  * `S` (now mod zone turns off as well).

### Summary
On the right there are delete, tab and escape keys. On the bottom there is ALT on the left thumb, CTRL and SHIFT on the right thumb.

![Symbols Layer](assets/keymap/symbols-layer.png)

Some insights on why I picked these positions for the symbols
* I'm a programmer, so access to brackets is important. I put the opening brackets on the home row, because editors often auto complete the closing brackets.
* Logic operators & (`and`) and | (`or`) are next to each other
* Mark down \* (`list item`) and # (`header`) are next to each other
* The second symbol column on the left contains all symbols that indicate something will follow (`and then...`)
* Quotes " and ` are next to each other
* Regex symbols ^ (`start of line`) and $ (`end of line`) are next to each other
* C# string modifiers $ (`allow variables`) and @ (`literal text with new line support`) are in the same position from each hand
* WIN modifier is on the same key as V, allowing for rapid clipboard access (WIN + V)
* The second symbol column on the right contains all horizontal line symbols
* Both slash types are next to each other

## Navigations Layer
This layer can be reached by pressing the inner thumb keys at the same time. It has arrow navigation on the left and a numpad on the right. This layer automatically deactivates when pressing space, backspace or enter. The modifiers are not one shot on this layer, since they are to be used with multiple arrow key presses.

Combos on the inner column allow opening the settings layer and starting caps word.

![Nav Layer](assets/keymap/nav-layer.png)

## Settings Layer
The settings layer only contains F-keys and some locking keys, future keys for RGB or other keyboard settings will appear here too.

![Settings Layer](assets/keymap/settings-layer.png)

## Game Layer
The outer thumb key on the symbol layer will cause the keyboard to switch to this base layer. It's a simple QWERTY base layer with the left hand shifted over to bring WASD under the correct fingers. It also has another column on the left with commonly used modifier keys and an escape on one of the thumb keys.

![Game Layer](assets/keymap/game-layer.png)