#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include "ui_app/ui.h"

#define DISP_BUF_SIZE (128 * 1024)

// void button_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
//     static uint32_t last_btn = 0;   /*Store the last pressed button*/
//     int btn_pr = my_btn_read();     /*Get the ID (0,1,2...) of the pressed button*/
//     if(btn_pr >= 0) {               /*Is there a button press? (E.g. -1 indicated no button was pressed)*/
//        last_btn = btn_pr;           /*Save the ID of the pressed button*/
//        data->state = LV_INDEV_STATE_PRESSED;  /*Set the pressed state*/
//     } else {
//        data->state = LV_INDEV_STATE_RELEASED; /*Set the released state*/
//     }

//     data->btn = last_btn;            /*Save the last button*/
// }

int main(void)
{
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

//    /*A small buffer for LittlevGL to draw the screen's content*/
//    static lv_color_t buf[DISP_BUF_SIZE];

//    /*Initialize a descriptor for the buffer*/
//    static lv_disp_draw_buf_t disp_buf;
//    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    static lv_color_t buf[DISP_BUF_SIZE],buf1[DISP_BUF_SIZE];
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, buf1, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 240;
    disp_drv.ver_res    = 240;
    lv_disp_drv_register(&disp_drv);

    //libinput_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER ;
//    /*This function will be called periodically (by the library) to get the mouse position and state*/
    // indev_drv_1.read_cb = button_read;
    // lv_indev_t *key_indev = lv_indev_drv_register(&indev_drv_1);


    /*Set a cursor for the mouse*/
//    LV_IMG_DECLARE(mouse_cursor_icon)
//    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
//    lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
//    lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/


    /*Create a Demo*/
//    lv_demo_widgets();
    ui_init();

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
