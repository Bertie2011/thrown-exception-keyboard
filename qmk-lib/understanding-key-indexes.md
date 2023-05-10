# Understanding Key Indexes

For some of the features you'll need to understand how the position of a keycode in the keymap relates to row/column indexes. The easiest way to figure this out is to open any of the `.h` files in the keyboard folder and find the `LAYOUT(...)` macro definition that you have been using in your keymap.

For example, with this definition:
```C
#define LAYOUT(                                         \
  k00, k01, k02, k03, k04,     k44, k43, k42, k41, k40, \
  k10, k11, k12, k13, k14,     k54, k53, k52, k51, k50, \
  k20, k21, k22, k23, k24,     k64, k63, k62, k61, k60, \
            k32, k30, k31,     k71, k70, k72            \
)                                                       \
{                                                       \
  {   k00,   k01,   k02,   k03,   k04 },                \
  {   k10,   k11,   k12,   k13,   k14 },                \
  {   k20,   k21,   k22,   k23,   k24 },                \
  {   k30,   k31,   k32, KC_NO, KC_NO },                \
  {   k40,   k41,   k42,   k43,   k44 },                \
  {   k50,   k51,   k52,   k53,   k54 },                \
  {   k60,   k61,   k62,   k63,   k64 },                \
  {   k70,   k71,   k72, KC_NO, KC_NO },                \
}
```
We can see that the top-right key of the left half (`k04`) maps to column `4` and row `0` (start counting from zero for both values). From there you can get an understanding of the other keycode positions and start findinig patterns (e.g. "row < 4" means the left side).