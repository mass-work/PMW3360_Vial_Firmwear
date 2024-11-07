

#include QMK_KEYBOARD_H
#include "config.h"

#ifdef POINTING_DEVICE_ENABLE

    void pointing_device_init_kb(void) {
        pmw33xx_init(0);         // index 1 is the fast device.
        pmw33xx_set_cpi(0, 1000); // applies to first sensor
        pointing_device_init_user();
    }

    report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
        pmw33xx_report_t report = pmw33xx_read_burst(0);
        if (!report.motion.b.is_lifted && report.motion.b.is_motion) {
            // From quantum/pointing_device_drivers.c
            #define constrain_hid(amt) ((amt) < -127 ? -127 : ((amt) > 127 ? 127 : (amt)))
            mouse_report.x = constrain_hid(mouse_report.x + report.delta_x);
            mouse_report.y = constrain_hid(mouse_report.y + report.delta_y);
        }
        return pointing_device_task_user(mouse_report);
    }
#endif
