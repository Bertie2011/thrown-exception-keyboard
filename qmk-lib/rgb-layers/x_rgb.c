#include "x_rgb.h"
#include <rgb_matrix.h>
#include <action_layer.h>

void x_rgb_set_white(void) {
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv(0, 0, rgb_matrix_get_val());
}

void x_rgb_set_cycle_left_right(void) {
    rgb_matrix_mode(RGB_MATRIX_CYCLE_LEFT_RIGHT);
    rgb_matrix_sethsv(0, 255, rgb_matrix_get_val());
}

void x_rgb_set_layer(void) {
    rgb_matrix_mode(RGB_MATRIX_CUSTOM_X_LAYER_EFFECT);
}

typedef struct RgbArgs {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RgbArgs;

const uint32_t PROGMEM rgbmaps[MAX_LAYER][MATRIX_ROWS][MATRIX_COLS];
uint8_t led_index_to_matrix_pos[RGB_MATRIX_LED_COUNT][2];
bool xRgbInit = false;

void x_rgb_init(void) {
    if (xRgbInit) return;
    xRgbInit = true;

    for (uint8_t index = 0; index < RGB_MATRIX_LED_COUNT; index++) {
        led_index_to_matrix_pos[index][0] = led_index_to_matrix_pos[index][1] = NO_LED;
    }

    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t index = g_led_config.matrix_co[row][col];
            if (index == NO_LED) continue;
            led_index_to_matrix_pos[index][0] = row;
            led_index_to_matrix_pos[index][1] = col;
        }
    }
}

#ifndef X_LAYER_EFFECT_DEFAULT
    #define X_LAYER_EFFECT_DEFAULT XXXXXXXX
#endif

__attribute__((weak)) uint32_t x_rgb_get_default_color_user(led_data* data) {
    return ________;
}
__attribute__((weak)) uint32_t x_rgb_get_override_color_user(led_data* data) {
    return ________;
}

uint32_t find_rgb_in_map(uint8_t row, uint8_t col) {
    layer_state_t layers = layer_state | (default_layer_state == 0 ? 1 : default_layer_state);
    for (int8_t i = MAX_LAYER - 1; i >= 0; i--) {
        layer_state_t i_bit = (layer_state_t)1 << i;
        if ((layers & i_bit) != i_bit) continue;

        uint32_t rgb = rgbmaps[i][row][col];
        if (rgb != ________) return rgb;
    }
    return XXXXXXXX;
}

RgbArgs convert_bits_to_rgb(uint32_t rgb) {
    uint8_t r = (rgb >> 16) & 0xFF;
    uint8_t g = (rgb >> 8) & 0xFF;
    uint8_t b = (rgb >> 0) & 0xFF;

    float brightness = ((float)rgb_matrix_get_val()) / 255;
    r *= brightness;
    g *= brightness;
    b *= brightness;

    return (RgbArgs) { r = r, g = g, b = b };
}

bool X_LAYER_EFFECT(effect_params_t* params) {
    x_rgb_init();
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; i++) {
        uint8_t row = led_index_to_matrix_pos[i][0];
        uint8_t col = led_index_to_matrix_pos[i][1];

        led_data data = {
            .row = row,
            .col = col,
            .index = i,
            .x = g_led_config.point[i].x,
            .y = g_led_config.point[i].y,
            .flags = g_led_config.flags[i]
        };
        uint32_t rgb = x_rgb_get_override_color_user(&data);
        if (rgb == ________ && row != NO_LED) rgb = find_rgb_in_map(row, col);
        if (rgb == ________) rgb = x_rgb_get_default_color_user(&data);
        if (rgb == ________) rgb = X_LAYER_EFFECT_DEFAULT;
        if (rgb == ________) rgb = XXXXXXXX;

        RgbArgs rgb_obj = convert_bits_to_rgb(rgb);
        rgb_matrix_set_color(i, rgb_obj.r, rgb_obj.g, rgb_obj.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}
