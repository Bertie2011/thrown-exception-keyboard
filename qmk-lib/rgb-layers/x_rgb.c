#include "x_rgb.h"
#include <rgb_matrix.h>
#include <action_layer.h>
#include <send_string.h>

hsv_t rgb_to_hsv(rgb_t rgb);

void x_rgb_set_white(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(0, 0, rgb_matrix_get_val());
}

void x_rgb_set_cycle_left_right(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_LEFT_RIGHT);
    rgb_matrix_sethsv_noeeprom(0, 255, rgb_matrix_get_val());
}

void x_rgb_set_layer(void) {
    rgb_matrix_mode(RGB_MATRIX_CUSTOM_X_LAYER_EFFECT);
}

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

        uint32_t rgb = pgm_read_dword(&rgbmaps[i][row][col]);
        if (rgb != ________) return rgb;
    }
    return XXXXXXXX;
}

rgb_t convert_bits_to_rgb(uint32_t rgb) {
    uint8_t r = (rgb >> 16) & 0xFF;
    uint8_t g = (rgb >> 8) & 0xFF;
    uint8_t b = (rgb >> 0) & 0xFF;

    rgb_t rgb_obj = { .r = r, .g = g, .b = b };
    hsv_t hsv_obj = rgb_to_hsv(rgb_obj);
    hsv_obj.v *= ((float)rgb_matrix_get_val()) / 255;

    return hsv_to_rgb(hsv_obj);
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

        rgb_t rgb_obj = convert_bits_to_rgb(rgb);
        rgb_matrix_set_color(i, rgb_obj.r, rgb_obj.g, rgb_obj.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

void x_rgb_send(void) {
    hsv_t currentHsv = rgb_matrix_get_hsv();
    currentHsv.v *= ((float)255) / RGB_MATRIX_MAXIMUM_BRIGHTNESS;
    rgb_t currentRgb = hsv_to_rgb_nocie(currentHsv);
    SEND_STRING("0x");
    send_byte(currentRgb.r);
    send_byte(currentRgb.g);
    send_byte(currentRgb.b);
}

hsv_t rgb_to_hsv(rgb_t rgb) {
    float r = (float)rgb.r / 255;
    float g = (float)rgb.g / 255;
    float b = (float)rgb.b / 255;
    float cMax = MAX(r, MAX(g, b));
    float cMin = MIN(r, MIN(g, b));
    float delta = cMax - cMin;
    hsv_t result = { .h = 0, .s = 0, .v = cMax * 255 };

    float h = 0; // Scale 0 -> 6 with looping on either side.
    if (delta == 0) {
        h = 0;
    } else if (cMax == r) {
        h = ((g - b) / delta);
    } else if (cMax == g) {
        h = (((b - r) / delta) + 2);
    } else if (cMax == b) {
        h = (((r - g) / delta) + 4);
    }
    // Cleanup, map to 0 -> 255
    result.h = (int16_t)((h * 256 / 6) + 256) % 256;

    if (cMax == 0) {
        result.s = 0;
    } else {
        result.s = (delta / cMax) * 255;
    }

    return result;
}
