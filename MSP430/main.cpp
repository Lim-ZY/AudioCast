    #include "driverlib.h"
    #include "ggwave/ggwave.h"
    #include <string.h>
    #include "ggwave/pstr.h"

    #define sPerFrame 128
    #define sRate     6000
    #define TX_PROT   GGWAVE_PROTOCOL_MT_FASTEST

    // Global GGwave instance
    GGWave ggwave;
    char txt[64];

    #define PSTR(s) (s)
    #define strcpy_P strcpy
    #define P(str) (strcpy_P(txt, PSTR(str)), txt)

    // FUNCTIONS======================================================================
    void ser_print(const char* message) {
        while (*message != '\0') {
            while (!(UCA0IFG & UCTXIFG));          // Wait for UART TX buffer to be ready
            UCA0TXBUF = *message++;                // Send character and advance pointer
            __delay_cycles(1000);          // Small delay
        }
    }

    void delay(unsigned int len) {
        while (len) {
            __delay_cycles(1000);
            len--;
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
        //Stop WDT
        WDT_A_hold(WDT_A_BASE);

        //--setup UART A0
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

        // Configure GPIO
        P2SEL1 |= BIT0 | BIT1;                    // USCI_A0 UART operation   
        P2SEL0 &= ~(BIT0 | BIT1);                 // (&= ~ clear the bit)
        //--P1.2 as PWM output (for generate_tone())
        GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P1,
            GPIO_PIN2,
            GPIO_PRIMARY_MODULE_FUNCTION
            );

        // Startup clock system with max DCO setting ~8MHz (for UART)
        CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
        CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
        CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
        CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
        CSCTL0_H = 0;                             // Lock CS registers

        UCA0CTLW0 &= ~UCSWRST;                    // put eUSCI out of reset

        PM5CTL0 &= ~LOCKLPM5;                     // turn on I/O
    }

    void init_ggwave(void) {
        //initialise ggwave
        ggwave.setLogFile(nullptr);

        auto p = GGWave::getDefaultParameters();

        // Adjust the "ggwave" parameters to your needs.
        // Make sure that the "payloadLength" parameter matches the one used on the transmitting side.
        p.payloadLength   = 8; //16
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
        
        // print ggwave.heapSize()
        char buf[16];
        sprintf(buf, "Heap size: %d\n", ggwave.heapSize());
        ser_print(buf);

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

        char buf[32];
        // CHECK TONES PRESENT
        // sprintf(buf, "No. of tones: %d\n", tones.size());
        // ser_print(buf);

        for (auto & curTone : tones) {
            const auto freq_hz = (protocol.freqStart + curTone)*ggwave.hzPerSample();
            sprintf(buf, "Playing tone: %f\n", freq_hz);
            ser_print(buf);
            generate_tone(freq_hz);
            delay(duration_ms);
        }
        Timer_A_stop(TIMER_A1_BASE);
    }

    void main (void)
    {    
        init_msp();

        init_ggwave();

        while (1) {
            send_text(ggwave, P("Hellooo"), TX_PROT);
            __delay_cycles(1000000);
        }
        
        // generate_tone(440);

        // changed heap size from 160 to 320
    }

