
#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>
#include <leds.h>
#include <main.h>
#include <camera/po8030.h>
#include "constants.h"
#include "color.h"
#include "spi_comm.h"

// semaphore
static semaphore_t color_sem;
uint8_t r = 0,g = 0, b = 0;
/*
 *  Returns the line's width extracted from the image buffer given
 *  Returns 0 if line not found
 */
color_t scanColor(void)
{
    color_t c;
    chSemWait(&color_sem);
    if((r>g) | (b>g))
    {       
         r = r - g;
         b = b - g; 
    }
    if(r>b)
    {
        c=RED;
    }
    if(r<b)
    {
        c=BLUE;
    }
    chSemSignal(&color_sem);
    return c;
}

uint8_t mean_center(uint8_t *buffer)
{
    uint32_t mean = 0;
    for (uint16_t i = 0; i < IMAGE_BUFFER_SIZE / 5; i++)
    {
        if (buffer[i])
        {
            mean += buffer[i];
        }
    }
    mean /= (IMAGE_BUFFER_SIZE / 5);
    return mean;
}

static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg)
{

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    // Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
    po8030_advanced_config(PO8030_FORMAT_RGB565, 0, 10, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
    dcmi_enable_double_buffering();
    dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
    dcmi_prepare();

    while (1)
    {
        // starts a capture
        dcmi_capture_start();
        // waits for the capture to be done
        wait_image_ready();
        // signals an image has been captured
        //chBSemSignal(&image_ready_sem);
        //TODO: SUPER MEGA WARNIING, I commented line above
    }
}

static THD_WORKING_AREA(waProcessImage, 1024);
static THD_FUNCTION(ProcessImage, arg)
{
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    uint8_t *img_buff_ptr;
    uint8_t image_red[IMAGE_BUFFER_SIZE / 5] = {0};
    uint8_t image_blue[IMAGE_BUFFER_SIZE / 5] = {0};
    uint8_t image_green[IMAGE_BUFFER_SIZE / 5] = {0};
    clear_leds();
    while (1)
    {

        // waits until an image has been captured
        //chBSemWait(&image_ready_sem);
        // set_front_led(1);
        // gets the pointer to the array filled with the last image in RGB565
        img_buff_ptr = dcmi_get_last_image_ptr();
        for (uint8_t i; i < IMAGE_BUFFER_SIZE / 5; i += 1)
        {
            image_red[i] = 0;
            image_blue[i] = 0;
            image_green[i] = 0;
        }

        uint16_t j = 0;
        // Extracts only the center pixels
        for (uint16_t i = (IMAGE_BUFFER_SIZE * 0.8); i < (IMAGE_BUFFER_SIZE * 1.2); i += 2)
        {
            // extracts first 5bits of the first byte
            // extracts lasts 5 bits of the second byte
            image_red[j / 2] = (uint16_t)img_buff_ptr[i] & 0xF8;
            image_green[j / 2] = (int)(img_buff_ptr[i] & 0x07) << 5 | (img_buff_ptr[i + 1] & 0xE0) >> 3;
            image_blue[j / 2] = (uint16_t)(img_buff_ptr[i + 1] & 0x1F) << 3;
            j += 2;
        }
        chSemWait(&color_sem);
        r = mean_center(image_red);
        b = mean_center(image_blue);
        g = mean_center(image_green);
        chSemSignal(&color_sem);
        chThdSleepMilliseconds(100);
  /*      
        if (r >= b)

        {
            clear_leds();
            set_rgb_led(0, 10, 0, 0);
            set_rgb_led(1, 10, 0, 0);
            set_rgb_led(2, 10, 0, 0);
            set_rgb_led(3, 10, 0, 0);
        }
        else
        {
            clear_leds();
            set_rgb_led(0, 0, 0, 10);
            set_rgb_led(1, 0, 0, 10);
            set_rgb_led(2, 0, 0, 10);
            set_rgb_led(3, 0, 0, 10);
        }*/

        dcmi_reset_error();
    }
}

void process_image_start(void)
{
    /*chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
    chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL); */
    chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
    chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}