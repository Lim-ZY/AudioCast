/* #############################################################################################
* Authors     : Lim Zi Yang, Yao Hejun
* Emails      : e1398962@u.nus.edu, e1113781@u.nus.edu
################################################################################################
ADAPTED FROM AUDIOCAST FOR MSP430
* Baseband Audio Generator for AudioCast Transmitter*

* Description : This sketch encodes sensor / fixed data in the form of audio signals which serves 
as a baseband signal for AudioCast transmitter. 
* Additional  : It builds on the ggwave library by ggerganov: https://github.com/ggerganov/ggwave/tree/master/examples/arduino-tx 
* Framework   : Platform IO
* Build Target: This sketch currently comples on ESP32-WROOM-32 (Devkit and NodeMCU), Check "platform.ini".
    - However, it works with other platformio compatible platforms like - Arduino Uno R3, Arduino Nano RP2040 Connect, etc.

* Receiver    : AudioCast uses a spectrum analyser to listen to the data encoded audio broadcasted by AudioCast.
                Further, the freely available "waver" app can be used to decode the data by listening to these transmissions
    - Web:     https://waver.ggerganov.com
    - Android: https://play.google.com/store/apps/details?id=com.ggerganov.Waver
    - iOS:     https://apps.apple.com/us/app/waver-data-over-sound/id1543607865
* Make sure to enable the "Fixed-length" option in "Waver"'s settings and set the number of bytes to 
be equal to "payloadLength" used in the sketch.
################################################################################################*/

#include "driverlib.h"
#include <string.h>
#include "ggwave/ggwave.h"
#include "ggwave/pstr.h"
#include "SI4713/Adafruit_Si4713.h"

// ggwave params
#define sPerFrame 128
#define sRate     6000
#define TX_PROT   GGWAVE_PROTOCOL_MT_FASTEST
#define PL        8

// ggwave functions
#define PSTR(s) (s)
#define strcpy_P strcpy
#define P(str) (strcpy_P(txt, PSTR(str)), txt)

// SI4713 params
#define _BV(n) (1 << n)
#define RESETPIN BIT3
#define FMSTATION 10230                // 10230 == 102.30 MHz

// MSP430 I2C params
#define SLAVE_ADDRESS SI4710_ADDR1

// Global instances
GGWave ggwave;
char txt[64];
Adafruit_Si4713 radio = Adafruit_Si4713(RESETPIN);

// FUNCTIONS======================================================================
void ser_print(const char* message) {
    while (*message != '\0') {
        while (!(UCA0IFG & UCTXIFG));          // Wait for UART TX buffer to be ready
        UCA0TXBUF = *message++;                // Send character and advance pointer
        __delay_cycles(1000);          // Small delay
    }
}

void ser_printa(uint8_t* array, int array_len) {
    char buf[16];
    int i;
    for (i = 0; i < array_len; i++) {
        sprintf(buf, "%x ", array[i]);
        ser_print(buf);
    }
    ser_print("\n");
}

void delay_ms(unsigned int ms) {
    while (ms) {
        __delay_cycles(8000);
        ms--;
    }
}

void generate_tone(uint16_t freq_hz) {   
    // Set frequency
    const uint32_t SMCLK_FREQ = 8000000; // CLK adjusted to 8MHz in init_msp()
    const uint16_t timer_period = (SMCLK_FREQ + (freq_hz/2)) / freq_hz - 1; //(SMCLK_FREQ / freq_hz) - 1;
    const uint16_t duty_cycle = timer_period / 2;

    //Generate PWM - Timer runs in Up-Down mode
    Timer_A_outputPWMParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = timer_period; //changed
    param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    param.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    param.dutyCycle = duty_cycle; //changed
    Timer_A_outputPWM(TIMER_A1_BASE, &param);
}

void init_msp(void) {
    //--Stop WDT
    WDT_A_hold(WDT_A_BASE);

    //--Setup UART A0
    UCA0CTLW0 |= UCSWRST;                     // put eUSCI into reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // BRCLK = SMCLK
    // Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 21-4: UCBRSx = 0x04
    // UCBRFx = int ( (52.083-52)*16) = 1
    UCA0BR0 = 52;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1;            // OS16 and modulation

    //--Configure GPIO
    P2SEL1 |= BIT0 | BIT1;                    // USCI_A0 UART operation   
    P2SEL0 &= ~(BIT0 | BIT1);                 // (&= ~ clear the bit)
    //--P1.2 as PWM output (for generate_tone())
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN2,
        GPIO_PRIMARY_MODULE_FUNCTION
        );

    //--Startup clock system with max DCO setting ~8MHz (for UART)
    CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL0_H = 0;                             // Lock CS registers

    UCA0CTLW0 &= ~UCSWRST;                    // put eUSCI out of reset

    //--Setup I2C
    P1SEL1 |= BIT6 | BIT7;                    // I2C pins in secondary module function
    P1DIR |= RESETPIN;                        // RST pin for SI4713
    UCB0CTLW0 = UCSWRST;                      // Enable SW reset
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK | UCSYNC; // I2C master mode, SMCLK
    UCB0BRW = 80;                            // fSCL = SMCLK/160 = ~100kHz
    UCB0I2CSA = SLAVE_ADDRESS;                   // Slave Address
    UCB0CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
    UCB0IE |= UCNACKIE;

    PM5CTL0 &= ~LOCKLPM5;                     // turn on I/O
    // char buf[32];
    // sprintf(buf, "%lu", CS_getSMCLK());
    // ser_print(buf);
    ser_print("MSP INIT SUCCESS\n");
}

void init_radio(void) {
    char buf[16];

    ser_print("Radio Begin=======\n");

    if (! radio.begin()) {  // begin with address 0x63 (CS high default)
        ser_print("Couldn't find radio?\n");
        while (1);
    }

    ser_print("\nSet TX power");
    radio.setTXpower(115);  // dBuV, 88-115 max

    ser_print("\nTuning into ");
    sprintf(buf, "%d.", FMSTATION/100);
    ser_print(buf);
    sprintf(buf, "%d\n", FMSTATION % 100);
    ser_print(buf);
    radio.tuneFM(FMSTATION); // 102.3 mhz

    // This will tell you the status in case you want to read it from the chip
    radio.readTuneStatus();
    ser_print("\tCurr freq: ");
    sprintf(buf, "%d.", radio.currFreq); 
    ser_print(buf);
    ser_print("\tCurr freqdBuV:"); 
    sprintf(buf, "%d.", radio.currdBuV); 
    ser_print(buf);
    ser_print("\tCurr ANTcap:"); 
    sprintf(buf, "%d. \n", radio.currAntCap); 
    ser_print(buf);

    radio.setGPIOctrl(_BV(1) | _BV(2));  // set GP1 and GP2 to output
}

void init_ggwave(void) {
    //initialise ggwave
    ggwave.setLogFile(nullptr);

    auto p = GGWave::getDefaultParameters();

    // Adjust the "ggwave" parameters to your needs.
    // Make sure that the "payloadLength" parameter matches the one used on the transmitting side.
    p.payloadLength   = PL;
    p.sampleRateInp   = sRate;
    p.sampleRateOut   = sRate;
    p.sampleRate      = sRate;
    p.samplesPerFrame = sPerFrame;
    p.sampleFormatInp = GGWAVE_SAMPLE_FORMAT_I16;
    p.sampleFormatOut = GGWAVE_SAMPLE_FORMAT_U8;
    p.operatingMode   = GGWAVE_OPERATING_MODE_TX | GGWAVE_OPERATING_MODE_TX_ONLY_TONES | GGWAVE_OPERATING_MODE_USE_DSS;

    // Protocols to use for TX
    GGWave::Protocols::tx().only(TX_PROT);

    // Sometimes, the speaker might not be able to produce frequencies in the Mono-tone range of 1-2 kHz.
    // In such cases, you can shift the base frequency up by changing the "freqStart" parameter of the protocol.
    // Make sure that in the receiver (for example, the "Waver" app) the base frequency is shifted by the same amount.
    // Here we shift the frequency by +48 bins. Each bin is equal to 48000/1024 = 46.875 Hz.
    // So the base frequency is shifted by +2250 Hz.
    // GGWave::Protocols::tx()[GGWAVE_PROTOCOL_MT_FASTEST].freqStart += 48;

    // Initialize the ggwave instance and print the memory usage
    if (!ggwave.prepare(p)) {
        ser_print("PREPARE FAILED\n");
    } else {
        ser_print("PREPARE SUCCESS\n");
    }

    ser_print("Instance initialized successfully!\n");
}

void send_text(GGWave &ggwave, const char * text, GGWave::TxProtocolId protocolId) {
    ser_print("Sending text...\n");
    if (!ggwave.init(text, protocolId)) {
        ser_print("INIT FAILED\n");
    } else {
        ser_print("INIT SUCCESS\n");
    }

    if (!ggwave.encode()) {
        ser_print("ENCODE FAILED\n");
    } else {
        ser_print("ENCODE SUCCESS\n");
    }

    const auto & protocol = GGWave::Protocols::tx()[protocolId];
    const auto tones = ggwave.txTones();
    const auto duration_ms = protocol.txDuration_ms(sPerFrame, sRate);

    for (auto & curTone : tones) {
        const auto freq_hz = (protocol.freqStart + curTone)*ggwave.hzPerSample();
        generate_tone(freq_hz);
        delay_ms(duration_ms);
    }
    Timer_A_stop(TIMER_A1_BASE);
}

void main (void)
{    
    init_msp();

    init_radio();

    init_ggwave();

    int i = 0;
    char tmp[8];
    while (true) {
        sprintf(tmp, "%03d%s", i, "ABCD");
        ser_print(tmp);
        send_text(ggwave, P(tmp), TX_PROT);
        __delay_cycles(8000000);
        i++;
    }

    // changed heap size from 160 to 352 and stack size to 240
    // changed printf-support to full
}
