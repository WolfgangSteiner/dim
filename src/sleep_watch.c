#include "grv/grv.h"
#include "grv/grv_util.h"

#define SOFT_CUTOFF 22.5f
#define HARD_CUTOFF 23.25f

void render_lock_image(void) {
    char* text = "TIME TO SLEEP";
    char* output_image = "/tmp/sleep_watch.png";
    int width = 1920;
    int height = 1080;
    int font_size = 72;
    char* text_color="#888888";
    char* background_color="#222222";
    char* cmd = grv_cstr_new_with_format(
        "convert -size %dx%d -background \"%s\" -fill \"%s\" -font Souvenir -pointsize 144 -gravity Center label:\"%s\" %s",
        width, height, background_color, text_color, text, output_image);
    //grv_log_info(grv_str_ref(cmd));
    system(cmd);
    free(cmd);
}

f32 unlock_interval(f32 time) {
    if (time >= HARD_CUTOFF) {
        return 1.0f/60.0f;
    } else if (time >= SOFT_CUTOFF) {
        return 15.0f/60.0f;
    } else {
        return 1.0f;
    }
}

int main(void) {
    f32 last_timestamp = -1.0f;
    while (true) {
        f32 time = grv_local_time_f32();
        if (time >= SOFT_CUTOFF || time < 5.0f) {
            if (last_timestamp < 0.0f || (last_timestamp > 0 && time - last_timestamp > unlock_interval(time))) {
                render_lock_image();
                system("i3lock --nofork --color=222222 -i /tmp/sleep_watch.png");
                last_timestamp = grv_local_time_f32();
            }
        } else {
            last_timestamp = -1.0f;
        }
        sleep(5);
    }
}
