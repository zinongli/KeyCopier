#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/widget.h>
#include <gui/modules/variable_item_list.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>
#include "key_maker_icons.h"

#define TAG "KeyMaker"

#define INCHES_PER_PIXEL 0.00978

#define START_INCH 0.247
#define END_INCH 0.997
#define STEP_SIZE_INCH 0.15

#define UNCUT_DEPTH_INCH 0.329
#define DEEPEST_DEPTH_INCH 0.191
#define DEPTH_STEP_INCH 0.023
#define MAX_DEPTH_IND ((UNCUT_DEPTH_INCH - DEEPEST_DEPTH_INCH) / DEPTH_STEP_INCH)
#define PIN_WIDTH_INCH 0.084
#define MAX_PIN 6

// Change this to BACKLIGHT_AUTO if you don't want the backlight to be continuously on.
#define BACKLIGHT_ON 1

// Our application menu has 3 items.  You can add more items if you want.
typedef enum {
    KeyMakerSubmenuIndexConfigure,
    KeyMakerSubmenuIndexGame,
    KeyMakerSubmenuIndexAbout,
} KeyMakerSubmenuIndex;

// Each view is a screen we show the user.
typedef enum {
    KeyMakerViewSubmenu, // The menu when the app starts
    KeyMakerViewTextInput, // Input for configuring text settings
    KeyMakerViewConfigure, // The configuration screen
    KeyMakerViewGame, // The main screen
    KeyMakerViewAbout, // The about screen with directions, link to social channel, etc.
} KeyMakerView;

typedef enum {
    KeyMakerEventIdRedrawScreen = 0, // Custom event to redraw the screen
    KeyMakerEventIdOkPressed = 42, // Custom event to process OK button getting pressed down
} KeyMakerEventId;

typedef struct {
    ViewDispatcher* view_dispatcher; // Switches between our views
    NotificationApp* notifications; // Used for controlling the backlight
    Submenu* submenu; // The application menu
    TextInput* text_input; // The text input screen
    VariableItemList* variable_item_list_config; // The configuration screen
    View* view_game; // The main screen
    Widget* widget_about; // The about screen

    VariableItem* key_name_item; // The name setting item (so we can update the text)
    char* temp_buffer; // Temporary buffer for text input
    uint32_t temp_buffer_size; // Size of temporary buffer

    FuriTimer* timer; // Timer for redrawing the screen
} KeyMakerApp;

typedef struct {
    uint32_t total_pin_index; // The index for total number of pins
    FuriString* key_name_name; // The name setting
    uint8_t pin_slc; // The pin that is being adjusted
    uint8_t total_pin; // The total number of pins we are adjusting
    uint8_t depth[]; // The cutting depth
} KeyMakerGameModel;

void initialize_depths(KeyMakerGameModel* model) {
    for (uint8_t i = 0; i <= model->total_pin; i++) {
        model->depth[i] = 0;
    }
}

/**
 * @brief      Callback for exiting the application.
 * @details    This function is called when user press back button.  We return VIEW_NONE to
 *            indicate that we want to exit the application.
 * @param      _context  The context - unused
 * @return     next view id
*/
static uint32_t key_maker_navigation_exit_callback(void* _context) {
    UNUSED(_context);
    return VIEW_NONE;
}

/**
 * @brief      Callback for returning to submenu.
 * @details    This function is called when user press back button.  We return VIEW_NONE to
 *            indicate that we want to navigate to the submenu.
 * @param      _context  The context - unused
 * @return     next view id
*/
static uint32_t key_maker_navigation_submenu_callback(void* _context) {
    UNUSED(_context);
    return KeyMakerViewSubmenu;
}

/**
 * @brief      Callback for returning to configure screen.
 * @details    This function is called when user press back button.  We return VIEW_NONE to
 *            indicate that we want to navigate to the configure screen.
 * @param      _context  The context - unused
 * @return     next view id
*/
static uint32_t key_maker_navigation_configure_callback(void* _context) {
    UNUSED(_context);
    return KeyMakerViewConfigure;
}

/**
 * @brief      Handle submenu item selection.
 * @details    This function is called when user selects an item from the submenu.
 * @param      context  The context - KeyMakerApp object.
 * @param      index     The KeyMakerSubmenuIndex item that was clicked.
*/
static void key_maker_submenu_callback(void* context, uint32_t index) {
    KeyMakerApp* app = (KeyMakerApp*)context;
    switch(index) {
    case KeyMakerSubmenuIndexConfigure:
        view_dispatcher_switch_to_view(app->view_dispatcher, KeyMakerViewConfigure);
        break;
    case KeyMakerSubmenuIndexGame:
        view_dispatcher_switch_to_view(app->view_dispatcher, KeyMakerViewGame);
        break;
    case KeyMakerSubmenuIndexAbout:
        view_dispatcher_switch_to_view(app->view_dispatcher, KeyMakerViewAbout);
        break;
    default:
        break;
    }
}

/**
 * Our 1st sample setting is a team color.  We have 3 options: red, green, and blue.
*/
static const char* total_pin_config_label = "Num of Pins";
static uint8_t total_pin_values[] = {5, 6};
static char* total_pin_names[] = {"5 (WIP)", "6"};
static void key_maker_total_pin_change(VariableItem* item) {
    KeyMakerApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, total_pin_names[index]);
    KeyMakerGameModel* model = view_get_model(app->view_game);
    model->total_pin_index = index;
    model->total_pin = total_pin_values[index];
}

/**
 * Our 2nd sample setting is a text field.  When the user clicks OK on the configuration 
 * setting we use a text input screen to allow the user to enter a name.  This function is
 * called when the user clicks OK on the text input screen.
*/
static const char* key_name_config_label = "Key Name";
static const char* key_name_entry_text = "Enter name";
static const char* key_name_default_value = "Key 1";
static void key_maker_key_name_text_updated(void* context) {
    KeyMakerApp* app = (KeyMakerApp*)context;
    bool redraw = true;
    with_view_model(
        app->view_game,
        KeyMakerGameModel * model,
        {
            furi_string_set(model->key_name_name, app->temp_buffer);
            variable_item_set_current_value_text(
                app->key_name_item, furi_string_get_cstr(model->key_name_name));
        },
        redraw);
    view_dispatcher_switch_to_view(app->view_dispatcher, KeyMakerViewConfigure);
}

/**
 * @brief      Callback when item in configuration screen is clicked.
 * @details    This function is called when user clicks OK on an item in the configuration screen.
 *            If the item clicked is our text field then we switch to the text input screen.
 * @param      context  The context - KeyMakerApp object.
 * @param      index - The index of the item that was clicked.
*/
static void key_maker_setting_item_clicked(void* context, uint32_t index) {
    KeyMakerApp* app = (KeyMakerApp*)context;
    index++; // The index starts at zero, but we want to start at 1.

    // Our configuration UI has the 2nd item as a text field.
    if(index == 2) {
        // Header to display on the text input screen.
        text_input_set_header_text(app->text_input, key_name_entry_text);

        // Copy the current name into the temporary buffer.
        bool redraw = false;
        with_view_model(
            app->view_game,
            KeyMakerGameModel * model,
            {
                strncpy(
                    app->temp_buffer,
                    furi_string_get_cstr(model->key_name_name),
                    app->temp_buffer_size);
            },
            redraw);

        // Configure the text input.  When user enters text and clicks OK, key_maker_setting_text_updated be called.
        bool clear_previous_text = false;
        text_input_set_result_callback(
            app->text_input,
            key_maker_key_name_text_updated,
            app,
            app->temp_buffer,
            app->temp_buffer_size,
            clear_previous_text);

        // Pressing the BACK button will reload the configure screen.
        view_set_previous_callback(
            text_input_get_view(app->text_input), key_maker_navigation_configure_callback);

        // Show text input dialog.
        view_dispatcher_switch_to_view(app->view_dispatcher, KeyMakerViewTextInput);
    }
}
static inline int min(int a, int b) {
    return (a < b) ? a : b;
}
/**
 * @brief      Callback for drawing the game screen.
 * @details    This function is called when the screen needs to be redrawn, like when the model gets updated.
 * @param      canvas  The canvas to draw on.
 * @param      model   The model - MyModel object.
*/
static double start_inch = START_INCH;
static double end_inch = END_INCH;
static double step_size_inch = STEP_SIZE_INCH;
static double inches_per_pixel = INCHES_PER_PIXEL;
static double uncut_depth_inch = UNCUT_DEPTH_INCH;
static double depth_step_inch = DEPTH_STEP_INCH;
static int pin_half_width_pixel;
static int pin_step_pixel;
static void key_maker_view_game_draw_callback(Canvas* canvas, void* model) {
    KeyMakerGameModel* my_model = (KeyMakerGameModel*)model;
    static bool initialized = false;
    if (!initialized) {
        pin_half_width_pixel = (int)round(PIN_WIDTH_INCH / INCHES_PER_PIXEL / 4);
        pin_step_pixel = (int)round(step_size_inch / inches_per_pixel);
        initialized = true;
    }
    int pin_step_pixel = (int)round(step_size_inch / inches_per_pixel);
    int post_extra_x_pixel = pin_step_pixel;
    for (int current_pin = 1; current_pin <= my_model->total_pin; current_pin += 1) {
        double current_value = start_inch + (current_pin - 1) * step_size_inch;
        int pin_center_pixel = (int)round(current_value / inches_per_pixel);
        //int next_pin_center_pixel = (int)round((start_inch + (current_pin) * step_size_inch) / inches_per_pixel);
        //int last_pin_center_pixel = (int)round((start_inch + (current_pin-2) * step_size_inch) / inches_per_pixel);

        int top_contour_pixel = (int)round(63 - uncut_depth_inch / inches_per_pixel);
        canvas_draw_line(canvas, pin_center_pixel, 20, pin_center_pixel, 50);
        int depth_pixel_i = (int)round(my_model->depth[current_pin - 1] * depth_step_inch / inches_per_pixel);
        canvas_draw_line(canvas, pin_center_pixel - pin_half_width_pixel, top_contour_pixel + depth_pixel_i, pin_center_pixel + pin_half_width_pixel, top_contour_pixel + depth_pixel_i);
        int last_depth = my_model->depth[current_pin - 2];
        int next_depth = my_model->depth[current_pin];        
        if(current_pin == 1){
            canvas_draw_line(canvas, 0, top_contour_pixel, pin_center_pixel - pin_half_width_pixel - depth_pixel_i, top_contour_pixel);
            last_depth = 0;
        } 
        if(current_pin == my_model->total_pin) {
            next_depth = MAX_DEPTH_IND;
            double numerator = (double)my_model->depth[current_pin - 1];
            double denominator = (double)(my_model->depth[current_pin - 1] + next_depth);
            double fraction = numerator / denominator;
            double product = fraction * pin_step_pixel;   
            int extra_x_pixel = (int)round(product); 
            canvas_draw_line(
                canvas,
                pin_center_pixel + extra_x_pixel,
                top_contour_pixel + depth_pixel_i - (extra_x_pixel - pin_half_width_pixel),
                128,
                top_contour_pixel + (128 - (pin_center_pixel + pin_half_width_pixel))
                );
        }
        if ((last_depth + my_model->depth[current_pin - 1]) > 4) { //yes intersection  
            int pre_extra_x_pixel = pin_step_pixel - post_extra_x_pixel;
            canvas_draw_line(
                canvas,
                pin_center_pixel - pre_extra_x_pixel,
                top_contour_pixel + depth_pixel_i - (pre_extra_x_pixel - pin_half_width_pixel),
                pin_center_pixel - pin_half_width_pixel,
                top_contour_pixel + depth_pixel_i
                );
        } else {
            canvas_draw_line(
                canvas,
                pin_center_pixel - pin_half_width_pixel - depth_pixel_i,
                top_contour_pixel,
                pin_center_pixel - pin_half_width_pixel,
                top_contour_pixel + depth_pixel_i
                );
        }
        if ((my_model->depth[current_pin - 1] + next_depth) > 4) { //yes intersection
            double numerator = (double)my_model->depth[current_pin - 1];
            double denominator = (double)(my_model->depth[current_pin - 1] + next_depth);
            double fraction = numerator / denominator;
            double product = fraction * pin_step_pixel;   
            post_extra_x_pixel = (int)round(product);         
            canvas_draw_line(
                canvas,
                pin_center_pixel + pin_half_width_pixel,
                top_contour_pixel + depth_pixel_i,
                pin_center_pixel + post_extra_x_pixel,
                top_contour_pixel + depth_pixel_i - (post_extra_x_pixel - pin_half_width_pixel)
                );
        } else { // no intersection
            canvas_draw_line(
                canvas,
                pin_center_pixel + pin_half_width_pixel,
                top_contour_pixel + depth_pixel_i,
                pin_center_pixel + pin_half_width_pixel + depth_pixel_i,
                top_contour_pixel
                );
        }
    }

    int level_contour_pixel = (int)round((end_inch + step_size_inch) / inches_per_pixel - 4);
    canvas_draw_line(canvas, 0, 63, level_contour_pixel, 63);
    int step_pixel = (int)round(step_size_inch / inches_per_pixel);
    canvas_draw_line(canvas, level_contour_pixel, 63, level_contour_pixel+step_pixel, 63-step_pixel);

    int slc_pin_pixel = (int)round((start_inch + (my_model->pin_slc - 1) * step_size_inch)/ inches_per_pixel);
    canvas_draw_str(canvas, slc_pin_pixel-2, 18, "*");

    FuriString* xstr = furi_string_alloc();
    int buffer_size = my_model->total_pin + 1; 
    char depth_str[buffer_size];
    depth_str[0] = '\0'; // Initialize the string
    // Manual string concatenation
    char* pos = depth_str;
    for (int i = 0; i < my_model->total_pin; i++) {
        int written = snprintf(pos, buffer_size - (pos - depth_str), "%u", my_model->depth[i]);
        if (written < 0 || written >= buffer_size - (pos - depth_str)) {
            // Handle error
            break;
        }
        pos += written;
    }
    furi_string_printf(xstr, "depth: %s", depth_str);
    canvas_draw_str(canvas, 0, 10, furi_string_get_cstr(xstr));
    
    //furi_string_printf(xstr, "Num of Pins: %s", total_pin_names[my_model->total_pin_index]);
    //canvas_draw_str(canvas, 44, 24, furi_string_get_cstr(xstr));
    furi_string_free(xstr);
}

/**
 * @brief      Callback for timer elapsed.
 * @details    This function is called when the timer is elapsed.  We use this to queue a redraw event.
 * @param      context  The context - KeyMakerApp object.
*/
static void key_maker_view_game_timer_callback(void* context) {
    KeyMakerApp* app = (KeyMakerApp*)context;
    view_dispatcher_send_custom_event(app->view_dispatcher, KeyMakerEventIdRedrawScreen);
}

/**
 * @brief      Callback when the user starts the game screen.
 * @details    This function is called when the user enters the game screen.  We start a timer to
 *           redraw the screen periodically (so the random number is refreshed).
 * @param      context  The context - KeyMakerApp object.
*/
static void key_maker_view_game_enter_callback(void* context) {
    uint32_t period = furi_ms_to_ticks(200);
    KeyMakerApp* app = (KeyMakerApp*)context;
    furi_assert(app->timer == NULL);
    app->timer =
        furi_timer_alloc(key_maker_view_game_timer_callback, FuriTimerTypePeriodic, context);
    furi_timer_start(app->timer, period);
}

/**
 * @brief      Callback when the user exits the game screen.
 * @details    This function is called when the user exits the game screen.  We stop the timer.
 * @param      context  The context - KeyMakerApp object.
*/
static void key_maker_view_game_exit_callback(void* context) {
    KeyMakerApp* app = (KeyMakerApp*)context;
    furi_timer_stop(app->timer);
    furi_timer_free(app->timer);
    app->timer = NULL;
}

/**
 * @brief      Callback for custom events.
 * @details    This function is called when a custom event is sent to the view dispatcher.
 * @param      event    The event id - KeyMakerEventId value.
 * @param      context  The context - KeyMakerApp object.
*/
static bool key_maker_view_game_custom_event_callback(uint32_t event, void* context) {
    KeyMakerApp* app = (KeyMakerApp*)context;
    switch(event) {
    case KeyMakerEventIdRedrawScreen:
        // Redraw screen by passing true to last parameter of with_view_model.
        {
            bool redraw = true;
            with_view_model(
                app->view_game, KeyMakerGameModel * _model, { UNUSED(_model); }, redraw);
            return true;
        }
    case KeyMakerEventIdOkPressed:
        // Process the OK button.  We play a tone based on the x coordinate.
        return true;
    default:
        return false;
    }
}

/**
 * @brief      Callback for game screen input.
 * @details    This function is called when the user presses a button while on the game screen.
 * @param      event    The event - InputEvent object.
 * @param      context  The context - KeyMakerApp object.
 * @return     true if the event was handled, false otherwise.
*/
static bool key_maker_view_game_input_callback(InputEvent* event, void* context) {
    KeyMakerApp* app = (KeyMakerApp*)context;
    if(event->type == InputTypeShort) {
        switch(event->key) {
            case InputKeyLeft: {
                // Left button clicked, reduce x coordinate.
                bool redraw = true;
                with_view_model(
                    app->view_game,
                    KeyMakerGameModel * model,
                    {
                        if(model->pin_slc > 1) {
                            model->pin_slc--;
                        }
                    },
                    redraw);
                break;
            }
            case InputKeyRight: {
                // Left button clicked, reduce x coordinate.
                bool redraw = true;
                with_view_model(
                    app->view_game,
                    KeyMakerGameModel * model,
                    {
                        if(model->pin_slc < model->total_pin) {
                            model->pin_slc++;
                        }
                    },
                    redraw);
                break;
            }
            case InputKeyUp: {
                // Left button clicked, reduce x coordinate.
                bool redraw = true;
                with_view_model(
                    app->view_game,
                    KeyMakerGameModel * model,
                    {
                        if(model->depth[model->pin_slc-1] > 0) {
                            model->depth[model->pin_slc-1]--;
                        }
                    },
                    redraw);
                break;
            }
            case InputKeyDown: {
                // Right button clicked, increase x coordinate.
                bool redraw = true;
                with_view_model(
                    app->view_game,
                    KeyMakerGameModel * model,
                    {
                        if(model->depth[model->pin_slc-1] <= (int)round(MAX_DEPTH_IND)) {
                            model->depth[model->pin_slc-1]++;
                        }
                    },
                    redraw);
                break;
            }
            default:
                // Handle other keys or do nothing
                break;
        }
    } else if(event->type == InputTypePress) {
        if(event->key == InputKeyOk) {
            // We choose to send a custom event when user presses OK button.  key_maker_custom_event_callback will
            // handle our KeyMakerEventIdOkPressed event.  We could have just put the code from
            // key_maker_custom_event_callback here, it's a matter of preference.
            view_dispatcher_send_custom_event(app->view_dispatcher, KeyMakerEventIdOkPressed);
            return true;
        }
    }

    return false;
}

/**
 * @brief      Allocate the key_maker application.
 * @details    This function allocates the key_maker application resources.
 * @return     KeyMakerApp object.
*/
static KeyMakerApp* key_maker_app_alloc() {
    KeyMakerApp* app = (KeyMakerApp*)malloc(sizeof(KeyMakerApp));

    Gui* gui = furi_record_open(RECORD_GUI);

    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);

    app->submenu = submenu_alloc();
    submenu_add_item(
        app->submenu, "Config", KeyMakerSubmenuIndexConfigure, key_maker_submenu_callback, app);
    submenu_add_item(
        app->submenu, "Play", KeyMakerSubmenuIndexGame, key_maker_submenu_callback, app);
    submenu_add_item(
        app->submenu, "About", KeyMakerSubmenuIndexAbout, key_maker_submenu_callback, app);
    view_set_previous_callback(submenu_get_view(app->submenu), key_maker_navigation_exit_callback);
    view_dispatcher_add_view(
        app->view_dispatcher, KeyMakerViewSubmenu, submenu_get_view(app->submenu));
    view_dispatcher_switch_to_view(app->view_dispatcher, KeyMakerViewSubmenu);

    app->text_input = text_input_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, KeyMakerViewTextInput, text_input_get_view(app->text_input));
    app->temp_buffer_size = 32;
    app->temp_buffer = (char*)malloc(app->temp_buffer_size);

    app->variable_item_list_config = variable_item_list_alloc();
    variable_item_list_reset(app->variable_item_list_config);
    VariableItem* item = variable_item_list_add(
        app->variable_item_list_config,
        total_pin_config_label,
        COUNT_OF(total_pin_values),
        key_maker_total_pin_change,
        app);
    uint8_t total_pin_index = 1;
    variable_item_set_current_value_index(item, total_pin_index);
    variable_item_set_current_value_text(item, total_pin_names[total_pin_index]);

    FuriString* key_name_name = furi_string_alloc();
    furi_string_set_str(key_name_name, key_name_default_value);
    app->key_name_item = variable_item_list_add(
        app->variable_item_list_config, key_name_config_label, 1, NULL, NULL);
    variable_item_set_current_value_text(
        app->key_name_item, furi_string_get_cstr(key_name_name));
    variable_item_list_set_enter_callback(
        app->variable_item_list_config, key_maker_setting_item_clicked, app);

    view_set_previous_callback(
        variable_item_list_get_view(app->variable_item_list_config),
        key_maker_navigation_submenu_callback);
    view_dispatcher_add_view(
        app->view_dispatcher,
        KeyMakerViewConfigure,
        variable_item_list_get_view(app->variable_item_list_config));

    app->view_game = view_alloc();
    view_set_draw_callback(app->view_game, key_maker_view_game_draw_callback);
    view_set_input_callback(app->view_game, key_maker_view_game_input_callback);
    view_set_previous_callback(app->view_game, key_maker_navigation_submenu_callback);
    view_set_enter_callback(app->view_game, key_maker_view_game_enter_callback);
    view_set_exit_callback(app->view_game, key_maker_view_game_exit_callback);
    view_set_context(app->view_game, app);
    view_set_custom_callback(app->view_game, key_maker_view_game_custom_event_callback);
    view_allocate_model(app->view_game, ViewModelTypeLockFree, sizeof(KeyMakerGameModel));
    KeyMakerGameModel* model = view_get_model(app->view_game);
    model->total_pin_index = total_pin_index;
    model->key_name_name = key_name_name;
    model->pin_slc = 1;
    model->total_pin = total_pin_values[model->total_pin_index];
    initialize_depths(model);


    view_dispatcher_add_view(app->view_dispatcher, KeyMakerViewGame, app->view_game);

    app->widget_about = widget_alloc();
    widget_add_text_scroll_element(
        app->widget_about,
        0,
        0,
        128,
        64,
        "Key Maker App 0.1");
    view_set_previous_callback(
        widget_get_view(app->widget_about), key_maker_navigation_submenu_callback);
    view_dispatcher_add_view(
        app->view_dispatcher, KeyMakerViewAbout, widget_get_view(app->widget_about));

    app->notifications = furi_record_open(RECORD_NOTIFICATION);

#ifdef BACKLIGHT_ON
    notification_message(app->notifications, &sequence_display_backlight_enforce_on);
#endif

    return app;
}

/**
 * @brief      Free the key_maker application.
 * @details    This function frees the key_maker application resources.
 * @param      app  The key_maker application object.
*/
static void key_maker_app_free(KeyMakerApp* app) {
#ifdef BACKLIGHT_ON
    notification_message(app->notifications, &sequence_display_backlight_enforce_auto);
#endif
    furi_record_close(RECORD_NOTIFICATION);

    view_dispatcher_remove_view(app->view_dispatcher, KeyMakerViewTextInput);
    text_input_free(app->text_input);
    free(app->temp_buffer);
    view_dispatcher_remove_view(app->view_dispatcher, KeyMakerViewAbout);
    widget_free(app->widget_about);
    view_dispatcher_remove_view(app->view_dispatcher, KeyMakerViewGame);
    view_free(app->view_game);
    view_dispatcher_remove_view(app->view_dispatcher, KeyMakerViewConfigure);
    variable_item_list_free(app->variable_item_list_config);
    view_dispatcher_remove_view(app->view_dispatcher, KeyMakerViewSubmenu);
    submenu_free(app->submenu);
    view_dispatcher_free(app->view_dispatcher);
    furi_record_close(RECORD_GUI);

    free(app);
}

/**
 * @brief      Main function for key_maker application.
 * @details    This function is the entry point for the key_maker application.  It should be defined in
 *           application.fam as the entry_point setting.
 * @param      _p  Input parameter - unused
 * @return     0 - Success
*/
int32_t main_key_maker_app(void* _p) {
    UNUSED(_p);

    KeyMakerApp* app = key_maker_app_alloc();
    view_dispatcher_run(app->view_dispatcher);

    key_maker_app_free(app);
    return 0;
}