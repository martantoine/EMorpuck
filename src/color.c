#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>
#include <leds.h>
#include <main.h>
#include <camera/po8030.h>
#include "constants.h"
#include "color.h"

uint16_t r = 0, b = 0;

//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

/*
 *  Returns the line's width extracted from the image buffer given
 *  Returns 0 if line not found
 */


int mean_center (uint8_t *buffer)
{
    uint32_t mean = 0;
    for(uint16_t i=0; i < IMAGE_BUFFER_SIZE/5; i++)
    {
        mean+= buffer[i];
    }
    mean /= (IMAGE_BUFFER_SIZE/5);
    return mean; 
}

static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
	po8030_advanced_config(PO8030_FORMAT_RGB565, 0, 10, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
	dcmi_enable_double_buffering();
	dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
	dcmi_prepare();

    while(1){
        //starts a capture
		dcmi_capture_start();
		//waits for the capture to be done
		wait_image_ready();
		//signals an image has been captured
		chBSemSignal(&image_ready_sem);
    }
}


static THD_WORKING_AREA(waProcessImage, 1024);
static THD_FUNCTION(ProcessImage, arg) 
{

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	uint8_t *img_buff_ptr;
	uint8_t image_red[IMAGE_BUFFER_SIZE/5] = {0};
    uint8_t image_blue[IMAGE_BUFFER_SIZE/5] = {0};


    while(1){
    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);   //il ne sort jamais de ce wait
		//gets the pointer to the array filled with the last image in RGB565    
		img_buff_ptr = dcmi_get_last_image_ptr();


		//Extracts only the center pixels
		for(uint16_t i = (2*IMAGE_BUFFER_SIZE -2* IMAGE_BUFFER_SIZE/10) ; i < (2*IMAGE_BUFFER_SIZE + 2* IMAGE_BUFFER_SIZE/10) ; i+=2)
        {
			//extracts first 5bits of the first byte
			//extracts lasts 5 bits of the second byte
            image_red[i/2] = (int)img_buff_ptr[i]&0xF8;
			image_blue[i/2] = (int)(img_buff_ptr[i]&0x1F)<<3;
        }   
        r=mean_center(image_red);
        b=mean_center(image_blue);
       
    /*if (b>100)
        {
            clear_leds();
				set_rgb_led(0, 10, 0, 0);
				set_rgb_led(1, 10, 0, 0);
				set_rgb_led(2, 10, 0, 0);
				set_rgb_led(3, 10, 0, 0);
        }   */     
     if(true)
        {
            clear_leds();
           		set_rgb_led(0, 0, 0, 10);
				set_rgb_led(1, 0, 0, 10);
				set_rgb_led(2, 0, 0, 10);
				set_rgb_led(3, 0, 0, 10);           
                }

		}
}

void process_image_start(void){
	/*chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL); */
   chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);    
   chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
 
}