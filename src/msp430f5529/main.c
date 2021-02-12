#include "main.h"

/* GLOBAL VARs */
uint8_t g_timer_1khz_flag = 0;
uint8_t g_timer_1sec_flag = 0;
uint16_t g_adc_result = 0;
uint8_t g_short_ECG_flag = 0;
uint8_t g_long_ECG_flag = 0;
uint16_t g_akku_vol = 0;
uint8_t g_tmp_return = 0;
uint16_t g_writingCyclesCnt = 0;

/* For median filter */
#define NUM_ELEMENTS    7
sMedianFilter_t medianFilter;
static sMedianNode_t medianBuffer[NUM_ELEMENTS];
/* End median filter */

STATE_MACHINE_e g_sys_state = SYS_INIT;
/* END GLOBAL VARs */

/* Function declarations */
void Init_Watchdog(void);
void EnableGlobalInterrupt(void);

/* Function definitions */
void main(void) {
    while (1) {
        switch (g_sys_state) {

        case SYS_INIT:
            /* Init MSP430 BEGIN */
            Init_Watchdog();
            Init_GPIO();
            Init_CLK();
            Init_Timers();
            Init_UART();
            Init_ADC();
            Init_SPI();
            Init_FAT();            //mount, set directory to read from, assign file
            Init_UART_BT();         //Init UART Interface for Bluetooth
            /* Init median filter */
            medianFilter.numNodes = NUM_ELEMENTS;
            medianFilter.medianBuffer = medianBuffer;
            MEDIANFILTER_Init(&medianFilter); // Init median filter
            EnableGlobalInterrupt();
            /* Init MSP430 END */
            
            g_sys_state = IDLE_STATE; // Change state
            break;

        case ECG_SHORT:
            if (g_timer_1khz_flag)
            {
                g_timer_1khz_flag = 0;
                ST_ECG();
            }
            if (g_long_ECG_flag)
            {
                g_long_ECG_flag = 0;
                Short_ECG_Error();
            }
            if (!g_short_ECG_flag)
            {
                Clear_Wave_ST();
                g_sys_state = IDLE_STATE;
            }
            break;

        case ECG_LONG:
            if (g_timer_1khz_flag)
            {
                g_timer_1khz_flag = 0;
                LT_ECG();
            }
            if (g_short_ECG_flag)
            {
                g_short_ECG_flag = 0;
                Long_ECG_Error();
            }
            if (!g_long_ECG_flag)
            {
                Clear_Wave_LT();
                g_sys_state = IDLE_STATE;
            }
            break;

        case ENERGY_SAVING_MODE:
            break;

        case IDLE_STATE:
            if (g_short_ECG_flag)
            {
                g_sys_state = ECG_SHORT;
            }
            if (g_long_ECG_flag)
            {
                g_sys_state = ECG_LONG;
            }

            if (g_timer_1sec_flag){
                g_timer_1sec_flag = 0;


                Start_ADC();
                send_bt_value(g_adc_result);

                uart_transmit_data_start("page0.akku.val=");
                uart_transmit_data_value (80);
                uart_transmit_data_end();
            }

            break;

        case SYS_ERROR:
            break;

        case SYS_DIRTY_START:
            break;

        case SYS_BAD_KEY:
            break;

        default:
            break;
        } 
    }
}

/*** Configure Watchdog Timer ***/
void Init_Watchdog(void)
{
    WDT_A_hold(WDT_A_BASE);
}

/*** Global Interrupt Enable ***/
void EnableGlobalInterrupt()
{
    // Enable global oscillator fault flag
    SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    // Enter interrupt
    __bis_SR_register(GIE);
    // For debugger
    __no_operation();
}
