

// PIC18F45K40 Configuration Bit Settings



// 'C' source line config statements



// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = HFINTOSC_1MHZ// Power-up default value for COSC bits (HFINTOSC with HFFRQ = 4 MHz and CDIV = 4:1)



// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)



// CONFIG2L
#pragma config MCLRE = EXTMCLR  // Master Clear Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (Power up timer disabled)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)



// CONFIG2H
#pragma config BORV = VBOR_2P45 // Brown Out Reset Voltage selection bits (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)



// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled)



// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)



// CONFIG4L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (006000-007FFFh) not write-protected)



// CONFIG4H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-30000Bh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config SCANE = ON       // Scanner Enable bit (Scanner module is available for use, SCANMD bit can control the module)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)



// CONFIG5L
#pragma config CP = OFF         // UserNVM Program Memory Code Protection bit (UserNVM code protection disabled)
#pragma config CPD = OFF        // DataNVM Memory Code Protection bit (DataNVM code protection disabled)



// CONFIG5H



// CONFIG6L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)



// CONFIG6H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)



// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



#include <xc.h>



#include <pic18f45k40.h>
#include <stdint.h>
#define _XTAL_FREQ 6000000

void displayDigit(int digit);
void Digits(int val);
void Bar(int val);
void Top(int val);
int state = 1;

__interrupt(high_priority) void isr (void){
    
    if (PIR0 & 0x01){
        
        state += 1;
        if (state > 2){
            state = 0;
        }
        PIR0 &= ~0x01;
        INTCON |= 0b11000000;
    }
}

void main(void){
    TRISB = 0b00001111;
    ANSELB = 0;
    TRISC = 0;
    TRISA = 0b00000001;
    TRISD = 0x00;
    ANSELA = 0b00000001;
    LATD = 0x00;
    ADCON0 |= 0x40;
    ADCON0 |= 0x04;
    ADCON0 |= 0x80;
    ADCON0 |= 0x01;
    
    
    PIE0 = 0b00000001; // enable int 0 interrupt
    INTCON = 0b01000000;
    INTCON |= 0b11000000;
        
    uint16_t result = 0;
    while(1){
        //__delay_ms(50);
        result = (ADRESH << 8);
        result |= ADRESL;
        if (state == 0){
            Top(result);
        } else if (state == 1){
            Digits(result);
        } else if (state == 2){
            Bar(result);
        }
        result = 0;
    }
}

void Top(int val){
    if (val > 1000){
        LATC = 0x80;
    }
    else if (val > 900){
        LATC = 0x40;
    }
    else if (val > 800){
        LATC = 0x20;
    }
    else if (val > 700){
        LATC = 0x10;
    }
    else if (val > 600){
        LATC = 0x08;
    }
    else if (val > 500){
        LATC = 0x04;
    }
    else if (val > 400){
        LATC = 0x02;
    }
    else if (val > 300){
        LATC = 0x01;
    }
    else{
        LATC = 0x01;
    }
}

void Bar(int val){
    if (val > 1000){
        LATC = 0xff;
    }
    else if (val > 900){
        LATC = 0x7f;
    }
    else if (val > 800){
        LATC = 0x3f;
    }
    else if (val > 700){
        LATC = 0x1f;
    }
    else if (val > 600){
        LATC = 0x0f;
    }
    else if (val > 500){
        LATC = 0x07;
    }
    else if (val > 400){
        LATC = 0x03;
    }
    else if (val > 300){
        LATC = 0x01;
    }
    else{
        LATC = 0x01;
    }
}

void Digits(int val){
    int max100 = val / 10;
    int tens = max100 / 10;
    int units = max100 % 10;
        
    for (int i = 0; i < 10; i++){
        displayDigit(tens);
        LATB = 0b10000000;
        __delay_ms(1);
        LATB = 0b11000000;
        displayDigit(units);
        LATB = 0b01000000;
        __delay_ms(1);
        LATB = 0b11000000;
        __delay_ms(1);
    }
}

void displayDigit(int digit){
    switch(digit){
        case 0:
            LATD = 0b11111100;
            break;
        case 1:
            LATD = 0b01100000;
            break;
        case 2:
            LATD = 0b11011010;
            break;
        case 3:
            LATD = 0b11110010;
            break;
        case 4:
            LATD = 0b01100110;
            break;
        case 5:
            LATD = 0b10110110;
            break;
        case 6:
            LATD = 0b10111110;
            break;
        case 7:
            LATD = 0b11100000;
            break;
        case 8:
            LATD = 0b11111110;
            break;
        case 9:
            LATD = 0b11110110;
            break;
    }
}