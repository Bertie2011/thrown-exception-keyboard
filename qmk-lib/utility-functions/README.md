# Utility Functions

## Layer Detection Functions
The base layer is set in the `default_layer_state` variable, which is a bit mask. However, the default value is `0` instead of `1` (= the zero-th bit). In order to detect layers correctly (including base layers), new functions have to be defined that can handle this inconsistency.
```C
bool layer_enabled(uint8_t layer) {
    return (((default_layer_state == 0 ? 1 : default_layer_state) | layer_state) & ((layer_state_t)1 << layer)) != 0;
}
```

```C
bool layer_mask_enabled_any(uint8_t layer_mask) {
    return (((default_layer_state == 0 ? 1 : default_layer_state) | layer_state) & layer_mask) != 0;
}
```

```C
bool any_layer_enabled(uint8_t layer1, uint8_t layer2) {
    return layer_enabled(layer1) || layer_enabled(layer2);
}
```