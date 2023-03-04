/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

/* mbed-fest hands-on session sample 2
 * bare-metal blinky with classic style code
 */

#include "mbed.h"
#include "FXOS8700Q.h"
#include "math.h"
#include "ESP8266.h"

Serial pc(USBTX,USBRX);                     //Serial Communication with PC
ESP8266 wifi(PTC17, PTC16, 115200);         //Tx Pin:PTC17; Rx Pin:PTC16; Baud rate:115200
char snd[255],rcv[1000]; 

I2C i2c( PTE25, PTE24 );
FXOS8700QAccelerometer  acc( i2c, FXOS8700CQ_SLAVE_ADDR1 );

DigitalOut led_x( LED1 );
DigitalOut led_y( LED2 );
DigitalOut led_z( LED3 );
DigitalOut buzzer(D2);
DigitalIn vibMotor(D4); 
AnalogIn level(A0);

float lowerthreshold = 400;             // level of submersion of the sensor plate(value to be set using the sensing)
int on = 1, off = 0;

int main(void)
{
    float   x, y, z;
     pc.baud(115200); 
     pc.printf("IOT CAR Antitheft System Project\n\r");
    //Accelerometer initializations
    acc.enable();
    
    while (true) {

        //Get Data
        acc.getX( x );
        acc.getY( y );
        acc.getZ( z );

        // LED 
        if ( x > 0 )
            led_x = 1;
        else
            led_y = 0;

        if ( y > 0 )
            led_y = 1;
        else
            led_y = 0;

        if ( z <= 1){
            led_x = 1;
            led_y = 0;
            led_z = 0;
            //printf("Car is tilted");
            buzzer = on; 
            wait_us(50000); 
            buzzer = off; 
            wait_us(50000);
		strcpy(snd,"0");
		wifi.SendCMD(snd);
        	// Wait for 1 second before sending again
       	 wait(1);
          
        }
        else
            led_z = 0;

        // Output data
        printf("X:%6.1f, Y:%6.1f, Z:%6.1f\r\n", x * 90.0, y * 90.0, z * 90.0 );
        wait( 0.5 );

        if (vibMotor == 1){
            printf("Car intrusion detected");
		strcpy(snd,"1");
		wifi.SendCMD(snd);
        	// Wait for 1 second before sending again
       	 wait(1);
            buzzer = on; 
            wait_us(50000000);

            buzzer = off; 
            wait_us(50000);
        }

        if (level>= lowerthreshold)
        {
            printf("\n Alert sent! The vehicle is submerging in the water!");
            strcpy(snd,"2");
		wifi.SendCMD(snd);
        	// Wait for 1 second before sending again
       	 wait(1);
            buzzer = on; 
            wait_us(5000); 
            buzzer = off; 
            wait_us(5000);
        }

    }
}