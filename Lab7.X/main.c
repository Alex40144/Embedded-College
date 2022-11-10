

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
#define _XTAL_FREQ 4000000

void LCD_Initialise();
void LCD_Instruction(int instruction);
void LCD_Char(char character);

int counter = 0;
int threshold = 255;

void main(void){
    TRISD = 0b00000000;
    LATD = 0x00; //reset
    LATD = 0x01; //enable 1,2
    
    TRISA = 0b00000100;
    ANSELA = 0b00000100;
    
    TRISE = 0x00;
    LATE = 0x00;
    
    TRISA = 0b00000100;
    ANSELA = 0b00000100;
    ADPCH = 2;
    FVRCON = 0x80;
    while(!FVRCON & 0x40);
    FVRCON |= 0x01;
    ADREF |= 0x03;
    ADCON0 |= 0x40;
    ADCON0 |= 0x04;
    ADCON0 |= 0x80;
    ADCON0 |= 0x01;
    
    int result = 0;
    
    LCD_Initialise();
    LCD_Char('T');
    LCD_Char('E');
    LCD_Char('M');
    LCD_Char('P');
    LCD_Char(':');
    LCD_Instruction(0xC0);
    LCD_Char('R');
    LCD_Char('P');
    LCD_Char('M');
    LCD_Char(' ');
    LCD_Char(':');
    

   while(1){
       result = (ADRESH << 8);
       result |= ADRESL;
       counter++;
       if (counter > result){
           //motor off
           LATD = 0x01;
       } else {
           //motor forwards
           LATD = 0x05;
       }
       if (counter > 1024) {
           counter = 0;
       }
       __delay_us(10);
    }
}

void LCD_Instruction(int instruction){
    //send high half
        //set data
    LATD &= ~0xF0;
    LATD |= (instruction & 0xF0);
    //register select low
    LATE &= ~0x01;
    LATE |= 0x04; //enable high
    __delay_ms(5);
    LATE &= ~0x04; //enable high
    
    LATD &= ~0xF0;
    LATD |= (instruction << 4);
    //register select low
    LATE &= 0x01;
    LATE |= 0x04; //enable high
    __delay_ms(5);
    LATE &= ~0x04; //enable high
}

void LCD_Char(char character){
    LATD &= ~0xF0;
    LATD |= (character & 0xF0);
    //register select low
    LATE |= 0x01;
    LATE |= 0x04; //enable high
    __delay_ms(1);
    LATE &= ~0x04; //enable high
    
    LATD &= ~0xF0;
    LATD |= (character << 4);

    LATE |= 0x04; //enable high
    __delay_ms(1);
    LATE &= ~0x04; //enable high
}

void LCD_Initialise(){
    LCD_Instruction(0x02); //set 4 bit address
    __delay_ms(10);
    LCD_Instruction(0x28); //set data length and number of lines
    __delay_ms(10);
    LCD_Instruction(0x0C); //display on, cursor off
    __delay_ms(10);
    LCD_Instruction(0x06); //set cursor increment
    __delay_ms(10);
    LCD_Instruction(0x01); //display clear
}