void x_rgb_set_white(void);
void x_rgb_set_cycle_left_right(void);
void x_rgb_set_layer(void);

#define XXXXXXXX 0x000000 // black
#define OOOOOOOO 0xFFFFFF // white
#define ________ 0x000001 // transparent (use the color on the layer below)

typedef struct {
    uint8_t row; // key position matrix indexes
    uint8_t col;
    uint8_t index; // led identifier
    uint8_t x; // physical position on the keyboard
    uint8_t y;
    uint8_t flags; // See https://docs.qmk.fm/#/feature_rgb_matrix?id=flags
} led_data;

uint32_t x_rgb_get_default_color_user(led_data* data);
uint32_t x_rgb_get_override_color_user(led_data* data);
