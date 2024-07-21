#ifndef KEY_FORMATS_H
#define KEY_FORMATS_H

typedef struct {
    char* format_name; 

    double first_pin_inch;
    double last_pin_inch;
    double pin_increment_inch;
    int pin_num;
    double pin_width_inch;
    double drill_angle;

    double uncut_depth_inch;
    double deepest_depth_inch;
    double depth_step_inch;
    int start_depth_ind;
    int max_depth_ind;

    int macs;
} KeyFormat;

extern const KeyFormat all_formats[];

#endif // KEY_FORMATS_H
