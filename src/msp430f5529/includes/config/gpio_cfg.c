/*
 * gpio_cfg.c
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#include <includes/config/gpio_cfg.h>


// Configure GPIO ports/pins
void Init_GPIO(void) {
    // Configure LED1 on DevKit
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN3);
    
    // Configure buzzer
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN1);
    
    // Configure LED2 on DevKit
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);

    // Configure 5V DC/DC Enaeble
	GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);
	GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);

    // Configure button on PCB
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN0);

    // Configure Pin attached to button DONT USE
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN2);


    /********************************************/
    /********* BEGIN Configure SPI BLOCK ********/
    /********************************************/
    // SPI_MOSI_SDCARD - P3.0
    // SPI_MISO_SDCARD - P3.1
    // SPI_CLK_SDCARD - P3.2
//    GPIO_setAsPeripheralModuleFunctionInputPin(
//        GPIO_PORT_P3,
//        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2
//        );

    // SPI_CS_SDCARD - P2.7
//    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);
//    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
    
    // TODO CARD DETECTION TO BE TESTED
    
    // !!!TEMPORARY!!! SPI_CD_SDCARD - P2.5 (GPIO, Card Select Pin)
    
    /* CD pin shorts to the ground when a card in NOT inserted.
     * Connect a pull up resistor (10K or so) and wire this to another
     * pin if you want to detect a card is inserted */

    /********* !!! IMPORTANT NOTE !!! ********/
    /* TEST WITH INTERNAL PULL UP RESISTOR ONLY 
     * WITHOUT EXTERNAL PULL UP RESISTOR CIRCUIT!
     * OTHERWISE CHANGE TO GPIO_setAsInputPin(..)
     * OR CALCULATE TOTAL RESISTANCE OF BOTH PULL 
     * UPS TO AVOID VOLTAGE DROP */
    /*****************************************/
//    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN5);
    /********************************************/
    /********* END Configure SPI BLOCK **********/
    /********************************************/
}

// Activate buzzer
//void GPIO_Buzzer_Single_Beep(void) {
//    // Buzzer on
//    GPIO_setOutputHighOnPin(GPIO_PORT_P1,
//                            GPIO_PIN6);
//    DELAY500K;
//    // Buzzer off
//    GPIO_setOutputLowOnPin(GPIO_PORT_P1,
//                            GPIO_PIN6);
//}

/* Interrupt Service Routines */
#pragma vector = PORT1_VECTOR
__interrupt void pushbutton_ISR(void) {
    switch(__even_in_range(P1IV, 0x10)) {
        case 0x00: break;   // None
        case 0x02:          // Pin 0

            GPIO_toggleOutputOnPin(GPIO_PORT_P6,GPIO_PIN6);
            GPIO_toggleOutputOnPin(GPIO_PORT_P2,GPIO_PIN4);

            break;
        case 0x04: break;   // Pin 1
        case 0x06: break;   // Pin 2
        case 0x08: break;   // Pin 3
        case 0x0A: break;   // Pin 4
        case 0x0C: break;   // Pin 5
        case 0x0E: break;   // Pin 6
        case 0x10: break;   // Pin 7
        default: _never_executed();
    }
}
