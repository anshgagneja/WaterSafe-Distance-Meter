#include <stdio.h>
#include <LPC17xx.h>
#include <math.h>
#include<string.h>
#define PRESCALE 29999999
#define PINSEL_LED 0xff    // P0.4-0.11
#define RS_CTRL 0x08000000 // P0.27 To inform whether it is command or data
#define EN_CTRL 0x10000000 // P0.28 Enable Pin first goes high then low
#define DT_CTRL 0x07800000 // P0.23 to P0.26 data lines
#define ULTRA_TRIGGER (1 << 15)     // P0.15
#define ULTRA_ECHO (1 << 16)     // P0.16
 
char RESULT[20] = "";
int TEMPORARY, TEMPORARY1, TEMPORARY2 = 0;
int flag = 0, flag1,flag2;
int x,row,key,col,i, j, k, l, r, ULTRA_ECHOTime = 5000;
float distance = 0;
float dis = 20.0;
 
void PORT_REFRESH(void);
void WRITETOLCD(void);
void WRITETOPORT(void);
void DISPLAY_LCD(unsigned char *buf1);
void delay(unsigned int r1);
void clearDisplay(void);
void startTimer0(void);
float stopTimer0(void);
void initTimer0(void);
void delayUS(unsigned int microseconds);
void delayMS(unsigned int milliseconds);
 
void delayUS(unsigned int microseconds) // Using Timer0
{
    LPC_SC->PCLKSEL0 &= ~(0x3 << 2);   // Set PCLK_TIMER0 to divide by 1
    LPC_TIM0->TCR = 0x02;               // Reset timer
    LPC_TIM0->PR = 0;                   // Set prescaler to 0
    LPC_TIM0->MR0 = microseconds - 1;  // Set match register for 10us
    LPC_TIM0->MCR = 0x01;               // Interrupt on match
    LPC_TIM0->TCR = 0x01;               // Enable timer
 
    while ((LPC_TIM0->IR & 0x01) == 0);  // Wait for interrupt flag
 
    LPC_TIM0->TCR = 0x00;               // Disable timer
    LPC_TIM0->IR = 0x01;
}
 
void delayMS(unsigned int milliseconds) // Using Timer0
{
    delayUS(milliseconds * 1000);
}
 
void initTimer0(void)
{
    // Timer for distance
    LPC_TIM0->CTCR = 0x0;
    LPC_TIM0->PR = 11999999;
    LPC_TIM0->TCR = 0x02;    // Reset Timer
}
 
void startTimer0()
{
    LPC_TIM0->TCR = 0x02; // Reset Timer
    LPC_TIM0->TCR = 0x01; // Enable timer
}
 
float stopTimer0()
{
    LPC_TIM0->TCR = 0x0;
    return LPC_TIM0->TC;
}
 
void delay(unsigned int r1)
{
    for (r = 0; r < r1; r++)
        ;
}
 
void PORT_REFRESH(void)
{
    /* Clearing the lines at power on */
    LPC_GPIO0->FIOCLR = DT_CTRL; // Clearing data lines
    LPC_GPIO0->FIOCLR = RS_CTRL; // Clearing RS line
    LPC_GPIO0->FIOCLR = EN_CTRL; // Clearing Enable line
    delay(3200);
    return;
}
 
void WRITETOPORT()
{
    int j;
    LPC_GPIO0->FIOPIN = TEMPORARY2 << 23;
    if (flag1 == 0)
    {
        LPC_GPIO0->FIOCLR = 1 << 27;
    }
    else
    {
        LPC_GPIO0->FIOSET = 1 << 27;
    }
    LPC_GPIO0->FIOSET = 1 << 28;
    for (j = 0; j < 50; j++)
        ;
    LPC_GPIO0->FIOCLR = 1 << 28;
    for (j = 0; j < 10000; j++);
}
 
void WRITETOLCD()
{
    TEMPORARY2 = (TEMPORARY1 >> 4) & 0xF;
    WRITETOPORT();
    TEMPORARY2 = TEMPORARY1 & 0xF;
    WRITETOPORT();
}
 
int main()
{
    int command[] = {3, 3, 3, 2, 2, 0x01, 0x06, 0x0C, 0x80};
    SystemInit();
    SystemCoreClockUpdate();
    initTimer0();
    LPC_PINCON->PINSEL0 &= 0xfffff00f;    // Interface LEDs P0.4-P0.11
    LPC_PINCON->PINSEL0 &= 0x3fffffff;    // Interface ULTRA_TRIGGER P0.15
    LPC_PINCON->PINSEL1 &= 0xfffffff0;
// Interface ULTRA_ECHO P0.16

LPC_PINCON->PINSEL3 &= 0;
LPC_PINCON->PINSEL4 &= 0;
    LPC_GPIO0->FIODIR |= ULTRA_TRIGGER | 1<<17;            // Direction for ULTRA_TRIGGERGER pin
    LPC_GPIO0->FIODIR |= 0 << 16 | 0<<23;         // Direction for ULTRA_ECHO PIN
    LPC_GPIO0->FIODIR |= PINSEL_LED << 4; // Direction for LED
LPC_GPIO2->FIODIR |= 0xf<<10;
 
    LPC_PINCON->PINSEL1 |= 0;
    LPC_GPIO0->FIODIR |= 0XF << 23 | 1 << 27 | 1 << 28;
    flag1 = 0;
    for (i = 0; i < 9; i++)
    {
        TEMPORARY1 = command[i];
        WRITETOLCD();
            for(j=0;j<30000;j++);
    }
    flag1 = 1;
    i = 0;
 
    flag = 1;
    LPC_GPIO0->FIOCLR |= ULTRA_TRIGGER;
    while (1)
    {
        LPC_GPIO0->FIOSET = 0x00000800;
        // Output 10us HIGH on ULTRA_TRIGGER pin
        LPC_GPIO0->FIOMASK = 0xFFFF7FFF;
        LPC_GPIO0->FIOPIN |= ULTRA_TRIGGER;
        delayUS(10);
        LPC_GPIO0->FIOCLR |= ULTRA_TRIGGER;
        LPC_GPIO0->FIOMASK = 0x0;
 
        while (!(LPC_GPIO0->FIOPIN & ULTRA_ECHO)){   // Wait for a HIGH on ULTRA_ECHO pin
        }          
        startTimer0();
        while (LPC_GPIO0->FIOPIN & ULTRA_ECHO); // Wait for a LOW on ULTRA_ECHO pin
 
        ULTRA_ECHOTime = stopTimer0(); // Stop Counting
        distance = (0.0343 * ULTRA_ECHOTime) / 40;

        sprintf(RESULT, " Proximity: %.2f", distance);
        flag1 = 1;
        i = 0;
        flag1 = 0;
        TEMPORARY1 = 0x01;
        WRITETOLCD();
        flag1 = 1;
 
       
        while (RESULT[i] != '\0')
        {
            TEMPORARY1 = RESULT[i];
            WRITETOLCD();
            for(j=0;j<30000;j++);
            i++;
        }
if (distance < 2.5) {
LPC_GPIO2->FIOPIN |= 1<<13;
LPC_GPIO0->FIOSET = 0xff << 4;
LPC_GPIO0->FIOSET = 1 << 17;
}
else if(distance <5){
LPC_GPIO2->FIOCLR = 1<<13;
LPC_GPIO0->FIOSET = 0xFE << 4;
LPC_GPIO0->FIOSET = 1 << 17;
}
else if(distance <7.5){
LPC_GPIO2->FIOCLR = 1<<13;
LPC_GPIO0->FIOSET = 0xFC << 4;
LPC_GPIO0->FIOSET = 1 << 17;
}
else if(distance <10){
LPC_GPIO2->FIOCLR = 1<<13;
LPC_GPIO0->FIOSET = 0xF8 << 4;
LPC_GPIO0->FIOSET = 1 << 17;
}
else if(distance <12){
LPC_GPIO2->FIOCLR = 1<<13;
LPC_GPIO0->FIOSET = 0xF0 << 4;
LPC_GPIO0->FIOSET = 1 << 17;
}
else if(distance <15){
LPC_GPIO2->FIOCLR = 1<<13;
LPC_GPIO0->FIOSET = 0xE0 << 4;
LPC_GPIO0->FIOSET = 1 << 17;
}
else if(distance <18){
LPC_GPIO2->FIOCLR = 1<<13;
LPC_GPIO0->FIOSET = 0xc0 << 4;
LPC_GPIO0->FIOSET = 1 << 17;
}

else if(distance <20){
LPC_GPIO2->FIOCLR = 1<<13;
LPC_GPIO0->FIOSET = 0x80 << 4;
LPC_GPIO0->FIOSET = 1 << 17;
}
else {
LPC_GPIO2->FIOCLR = 1<<13;
//LPC_GPIO0->FIOCLR = LED_Pinsel << 4;
LPC_GPIO0->FIOPIN = 0x00 << 4;
LPC_GPIO0->FIOCLR = 1 << 17;
}
delay(88000);
}
}