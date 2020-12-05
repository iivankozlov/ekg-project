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
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    // Configure buzzer
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
    // Configure LED2 on DevKit
    //GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);

    // Configure button on DevKit
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);

    /*** LEAVE COMMENTED OUT BEGIN ***/
    // What edge transition will trigger an interrupt
    //(low-to-high or high-to-low)
    //GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1,
        //GPIO_LOW_TO_HIGH_TRANSITION);
    // Clears the IFG bit associated with P1.1
    //GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    /*** LEAVE COMMENTED OUT END ***/

    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
}

// Activate buzzer
void GPIO_Buzzer_Single_Beep(void) {
    // Buzzer on
    GPIO_setOutputHighOnPin(GPIO_PORT_P1,
                            GPIO_PIN6);
    DELAY500K;
    // Buzzer off
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,
                            GPIO_PIN6);
}

/* Interrupt Service Routines */
#pragma vector = PORT1_VECTOR
__interrupt void pushbutton_ISR(void) {
    switch(__even_in_range(P1IV, 0x10)) {
        case 0x00: break;   // None
        case 0x02: break;   // Pin 0
        case 0x04:          // Pin 1
            // Toggle LED
            GPIO_toggleOutputOnPin(GPIO_PORT_P1,
                                   GPIO_PIN0);
            GPIO_Buzzer_Single_Beep();
            Test_UART();
            break;
        case 0x06: break;   // Pin 2
        case 0x08: break;   // Pin 3
        case 0x0A: break;   // Pin 4
        case 0x0C: break;   // Pin 5
        case 0x0E: break;   // Pin 6
        case 0x10: break;   // Pin 7
        default: _never_executed();
    }
}
