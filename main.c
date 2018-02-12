

//Drew Robert & Joseph Niski
//ECE 2049
//Lab 2

#include <msp430.h>
#include "peripherals.h"

// Function Prototypes
void swDelay(char numLoops);
void configBoardButtons();
void configLaunchButtons();
char buttonStates();
void runtimerA2(void);
void stoptimerA2(int reset);
char launchpadButtonStates();
void decimalASCIIPrice(unsigned int input);
void decimalASCIIGallons(unsigned int gal);

// Declare globals here
int state = 0;
unsigned char pressed = 0xFF;
unsigned char pressed2 = 0xFF;
unsigned char currKey = 0;
int once = 1;
int diesel = 0;
int super = 0;
int premium = 0;
int regular = 0;
unsigned int rate = 0;
long unsigned int timer_cnt = 0, current_cnt = 0, cutoff_cnt = 0;
char tdir = 1;
unsigned int totalGallons = 0;
unsigned int totalPrice = 0;
int timer_on = 0;
unsigned char priceArray[10] = {' '};
unsigned char galArray[6] = {' '};
int i = 0, j = 0, m = 0, q = 0, z = 0, p = 0, a = 0, r = 0;
unsigned char pin1[8] = {' '};
unsigned char pin2[8] = {' '};
unsigned char pin3[8] = {' '};
unsigned char displayPin1[8] = {' '};
unsigned char displayPin2[8] = {' '};
unsigned char displayPin3[8] = {' '};
unsigned char correctPin1[8] = {0,8,6,7,5,3,0,9};
unsigned char correctPin2[8] = {3,1,4,1,5,9,2,6};
int isTrue1 = 0;
int isTrue2 = 0;


// Main
void main(void)
{

    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired
    initLeds();

    _BIS_SR(GIE);

    configDisplay();
    configKeypad();
    configBoardButtons();
    configLaunchButtons();

    // *** Intro Screen ***
    Graphics_clearDisplay(&g_sContext); // Clear the display

    while(1){

    switch (state){
    case 0: // Display Welcome Screen

        currKey = getKey();

        while(once == 1)
        {
        Graphics_clearDisplay(&g_sContext); // Clear the display
        // Write some text to the display
        Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "to", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "M-HESS-P430.", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "Press '*'", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "to Start", AUTO_STRING_LENGTH, 48, 65, TRANSPARENT_TEXT);

        // Draw a box around everything because it looks nice
        Graphics_Rectangle box = {.xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
        Graphics_drawRectangle(&g_sContext, &box);

        // Update display
        Graphics_flushBuffer(&g_sContext);
        once = 0;
        }

        if (currKey == '*'){
            state = 1;
            once = 1;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }

        break;

    case 1: // Select Grade

        currKey = getKey();

        while(once == 1)
        {
        Graphics_drawStringCentered(&g_sContext, "Select Grade", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "1 = Diesel", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "2 = Super", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "3 = Premium", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "4 = Regular", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);

        Graphics_flushBuffer(&g_sContext);
        once = 0;
        }

        pressed = buttonStates();

        if (pressed == 0x01){
            once = 1;
            state = 2;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }

        else if (pressed == 0x04){
            once = 1;
            state = 4;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }

        else if (pressed == 0x10){
            once = 1;
            state = 3;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }

        else if (pressed == 0x40){
            once = 1;
            state = 5;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }


        break;


    case 2: //Diesel

        currKey = getKey();

        while (once == 1){
        Graphics_drawStringCentered(&g_sContext, "Diesel Selected", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "Correct?", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "'*' Continue", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "'#' Restart", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);

        // Update display
        Graphics_flushBuffer(&g_sContext);
        once = 0;
        }

        if (currKey == '*')
        {
            diesel = 1;
            rate = 299;
            once = 1;
            state = 6;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }
        else if (currKey == '#')
        {
            once = 1;
            state = 1;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }

        break;



    case 3: //Super

        currKey = getKey();

        while (once == 1){
        Graphics_drawStringCentered(&g_sContext, "Super Selected", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "Correct?", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "'*' Continue", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "'#' Restart", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);

        // Update display
        Graphics_flushBuffer(&g_sContext);
        once = 0;
        }

        if (currKey == '*'){
            super = 1;
            rate = 289;
            once = 1;
            state = 6;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }
        else if (currKey == '#'){
            state = 1;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }

        break;



    case 4: //Regular

        currKey = getKey();

        while (once == 1){
        Graphics_drawStringCentered(&g_sContext, "Regular Selected", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "Correct?", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "'*' Continue", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "'#' Restart", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);

        // Update display
        Graphics_flushBuffer(&g_sContext);
        once = 0;
        }

        if (currKey == '*'){
            regular = 1;
            rate = 249;
            once = 1;
            state = 6;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }
        else if (currKey == '#'){
            state = 1;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }

        break;



    case 5: //Premium

        currKey = getKey();

        while (once == 1){
        Graphics_drawStringCentered(&g_sContext, "Premium Selected", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "Correct?", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "'*' Continue", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "'#' Restart", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);

        // Update display
        Graphics_flushBuffer(&g_sContext);
        once = 0;
        }

        if (currKey == '*'){
            premium = 1;
            rate = 269;
            once = 1;
            state = 6;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }
        else if (currKey == '#'){
            state = 1;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }

        break;



    case 6: // Pump Ready

        //while(once == 1){

        Graphics_drawStringCentered(&g_sContext, "Pump Ready.", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "Begin Fueling", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);

        // Update display
        Graphics_flushBuffer(&g_sContext);

        stoptimerA2(1);

        //once = 0;

        //}//end while once
        state = 7;
        break;


    case 7: //actual fueling

        pressed2 = launchpadButtonStates();

        while (pressed2 != 0x00)
        {
            pressed2 = launchpadButtonStates();

            if (diesel == 1 && pressed2 == 0x01)
            {

                runtimerA2();

                totalGallons = timer_cnt;

                decimalASCIIGallons(totalGallons);

                Graphics_drawStringCentered(&g_sContext, galArray, 6, 48, 45, OPAQUE_TEXT);
                //Graphics_drawStringCentered(&g_sContext, priceArray, 10, 48, 55, OPAQUE_TEXT);

                // Update display
                Graphics_flushBuffer(&g_sContext);

            }

            else if (pressed2 == 0x04 && diesel == 0)
            {
                timer_on = 1;
                runtimerA2();

                totalGallons = timer_cnt;

                decimalASCIIGallons(totalGallons);

                Graphics_drawStringCentered(&g_sContext, galArray, 6, 48, 45, OPAQUE_TEXT);
                //Graphics_drawStringCentered(&g_sContext, priceArray, 10, 48, 55, OPAQUE_TEXT);

                // Update display
                Graphics_flushBuffer(&g_sContext);
            }
            else
            {
                state = 8;
                once = 1;
                current_cnt = timer_cnt;
                cutoff_cnt = timer_cnt;
                break;
            }

        } //end while pressed loop

        break;

    case 8: //top off

        while (timer_cnt <= cutoff_cnt + 500) //allow the user to top off the tank for 5 extra seconds
        {
            pressed2 = launchpadButtonStates();

            while(once == 1)
            {
            long unsigned int time_diff = timer_cnt - current_cnt;

            current_cnt = current_cnt - time_diff;
            once = 0;
            }

            while (pressed2 != 0x00)
            {
                pressed2 = launchpadButtonStates();

                if (diesel == 1 && pressed2 == 0x01)
                {
                    //runtimerA2();

                    totalGallons = current_cnt;

                    decimalASCIIGallons(totalGallons);

                    Graphics_drawStringCentered(&g_sContext, galArray, 6, 48, 45, OPAQUE_TEXT);
                    //Graphics_drawStringCentered(&g_sContext, priceArray, 10, 48, 55, OPAQUE_TEXT);

                    // Update display
                    Graphics_flushBuffer(&g_sContext);

                }

                else if (pressed2 == 0x04 && diesel == 0)
                {
                    timer_on = 1;
                    //runtimerA2();

                    totalGallons = current_cnt;

                    decimalASCIIGallons(totalGallons);

                    Graphics_drawStringCentered(&g_sContext, galArray, 6, 48, 45, OPAQUE_TEXT);
                    //Graphics_drawStringCentered(&g_sContext, priceArray, 10, 48, 55, OPAQUE_TEXT);

                    // Update display
                    Graphics_flushBuffer(&g_sContext);
                }

            } //end while pressed loop
        }

        if(pressed2 == 0x00)
        {
            stoptimerA2(1);
            totalPrice = (totalGallons/10) * rate;
            decimalASCIIPrice(totalPrice);
            state = 10;
            once = 1;
            Graphics_clearDisplay(&g_sContext); // Clear the display
            break;
        }
        else
        {
            pressed2 = launchpadButtonStates();
        }

    /*case 9: //calculate price

        while(once == 1)
        {
            Graphics_clearDisplay(&g_sContext); // Clear the display
            once = 0;
        }

        // Update display
        Graphics_flushBuffer(&g_sContext);

        totalPrice = current_cnt * rate;

        decimalASCIIPrice(totalPrice);

        swDelay(5);

        break;*/

    case 10: // Display price and pay

        currKey = getKey();

        runtimerA2();

        while (once == 1)
        {

            Graphics_drawStringCentered(&g_sContext, "Total Cost:", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, priceArray, 10, 48, 25, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Enter PIN.", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "'*' to Finish", AUTO_STRING_LENGTH, 48, 65, TRANSPARENT_TEXT);

            // Update display
            Graphics_flushBuffer(&g_sContext);

            pin1[0] = ' ';
            pin1[1] = ' ';
            pin1[2] = ' ';
            pin1[3] = ' ';
            pin1[4] = ' ';
            pin1[5] = ' ';
            pin1[6] = ' ';
            pin1[7] = ' ';

            displayPin1[0] = ' ';
            displayPin1[1] = ' ';
            displayPin1[2] = ' ';
            displayPin1[3] = ' ';
            displayPin1[4] = ' ';
            displayPin1[5] = ' ';
            displayPin1[6] = ' ';
            displayPin1[7] = ' ';

            q = 0;

            isTrue1 = 0;

            once = 0;
        }

        stoptimerA2(1);

        while (q < 8)
        {
            runtimerA2();

            if(timer_cnt > 1500)
           {
               state = 12;
               once = 1;
               Graphics_clearDisplay(&g_sContext); // Clear the display
               break;
           }

            currKey = getKey();

            if ((currKey >= '0') && (currKey <= '9'))
            {
                pin1[q] = currKey - 0x30;
                displayPin1[q] = '#';
                q++;
                stoptimerA2(1);
                swDelay(0.45);
            } //end if statement

            Graphics_drawStringCentered(&g_sContext, displayPin1, 8, 48, 45, OPAQUE_TEXT);

            // Update display
            Graphics_flushBuffer(&g_sContext);

        } //end while loop

        if (currKey == '*')
        {

            for (i = 0; i < 8; i++)
            {
                if (pin1[i] != correctPin1[i] && pin1[i] != correctPin2[i]) //incorrect pin is entered
                {
                    isTrue1 = 1;
                }

            }

            if (isTrue1 == 1){ //incorrect pin entered
                once = 1;
                state = 11;
                Graphics_clearDisplay(&g_sContext); // Clear the display
            }

            else if (isTrue1 == 0){ //correct pin entered
                currKey = getKey();
                once = 1;
                state = 13;
                swDelay(1);
                Graphics_clearDisplay(&g_sContext); // Clear the display
            }

        }

        break;


    case 11: //Incorrect Pin

        while (once == 1){
            Graphics_drawStringCentered(&g_sContext, "Incorrect Pin", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);

            // Update display
            Graphics_flushBuffer(&g_sContext);

            p++;
            once = 0;
        }

        swDelay(5);

        if (p < 3){
            isTrue1 = 0;
            once = 1;
            state = 10;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }

        else if (p == 3){
            once = 1;
            state = 12;
            Graphics_clearDisplay(&g_sContext); // Clear the display
        }

        break;

    case 12: //Alarm

        currKey = getKey();

        while (once == 1){
            Graphics_drawStringCentered(&g_sContext, "ALARM!!!", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Enter PIN", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "to Stop.", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "'*' to Finish", AUTO_STRING_LENGTH, 48, 75, TRANSPARENT_TEXT);

            // Update display
            Graphics_flushBuffer(&g_sContext);

            pin3[0] = ' ';
            pin3[1] = ' ';
            pin3[2] = ' ';
            pin3[3] = ' ';
            pin3[4] = ' ';
            pin3[5] = ' ';
            pin3[6] = ' ';
            pin3[7] = ' ';

            displayPin3[0] = ' ';
            displayPin3[1] = ' ';
            displayPin3[2] = ' ';
            displayPin3[3] = ' ';
            displayPin3[4] = ' ';
            displayPin3[5] = ' ';
            displayPin3[6] = ' ';
            displayPin3[7] = ' ';

            a = 0;

            BuzzerOn();

            setLeds(0x31);
            swDelay(1);
            setLeds(0);
            swDelay(1);
            setLeds(0x32);
            swDelay(1);
            setLeds(0);
            swDelay(1);
            setLeds(0x34);
            swDelay(1);
            setLeds(0);
            swDelay(1);
            setLeds(0x38);
            swDelay(1);
            setLeds(0);
            swDelay(1);
            setLeds((10 + 0x30) + (5 + 0x30));
            swDelay(1);

            isTrue2 = 0;

            once = 0;

        }



        while (a < 8)
        {
            currKey = getKey();

            if ((currKey >= '0') && (currKey <= '9'))
            {
                pin3[a] = currKey - 0x30;
                displayPin3[a] = '#';
                a++;
                swDelay(1);
            } //end if statement

            Graphics_drawStringCentered(&g_sContext, displayPin3, 8, 48, 55, OPAQUE_TEXT);

            // Update display
            Graphics_flushBuffer(&g_sContext);

        } //end while loop

        currKey = getKey();

        if (currKey == '*')
        {

            for (i = 0; i < 8; i++)
            {
                if (pin3[i] != correctPin1[i] && pin3[i] != correctPin2[i]) //incorrect pin is entered
                {
                    isTrue2 = 1;
                }
            }
            }

            if (isTrue2 == 1){//incorrect pin entered
                Graphics_clearDisplay(&g_sContext); // Clear the display

                Graphics_drawStringCentered(&g_sContext, "WRONG PIN!", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "TRY AGAIN!", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);

                // Update display
                Graphics_flushBuffer(&g_sContext);

                swDelay(2);

                a = 0;
                once = 1;
                state = 9;
                Graphics_clearDisplay(&g_sContext); // Clear the display
            }

            else if (isTrue2 == 0){ //correct pin is entered
                BuzzerOff();
                setLeds(0);
                once = 1;
                state = 13;
                swDelay(1);
                Graphics_clearDisplay(&g_sContext); // Clear the display
            }


        break;

    case 13: //thank you!!

        while(once == 1){
        Graphics_drawStringCentered(&g_sContext, "Thank you!", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "Come again!", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);

        // Update display
        Graphics_flushBuffer(&g_sContext);

        swDelay(5);

        once = 0;
        }

        if(once == 0)
        {
            state = 0;
            once = 1;
            break;
        }

    }//end the switch state
    }//end the while loop

}


void swDelay(char numLoops)
{
    // This function is a software delay. It performs
    // useless loops to waste a bit of time
    //
    // Input: numLoops = number of delay loops to execute
    // Output: none
    //
    // smj, ECE2049, 25 Aug 2013

    volatile unsigned int i,j;  // volatile to prevent removal in optimization
                                // by compiler. Functionally this is useless code

    for (j=0; j<numLoops; j++)
    {
        i = 50000 ;                 // SW Delay
        while (i > 0)               // could also have used while (i)
           i--;
    }
}

void configBoardButtons(){
    //4 Board Buttons: P7.0, P3.6, P2.2, P7.4

    P7SEL &= (BIT7|BIT6|BIT5|BIT3|BIT2|BIT1); //xxx0 xxx0
    P3SEL &= (BIT7|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0); //x0xx xxxx
    P2SEL &= (BIT7|BIT6|BIT5|BIT4|BIT3|BIT1|BIT0); //xxxx x0xx

    P7DIR &= (BIT7|BIT6|BIT5|BIT3|BIT2|BIT1); //xxx0 xxx0
    P3DIR &= (BIT7|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0); //x0xx xxxx
    P2DIR &= (BIT7|BIT6|BIT5|BIT4|BIT3|BIT1|BIT0); //xxxx x0xx

    P7REN |= (BIT0|BIT4);
    P3REN |= (BIT6);
    P2REN |= (BIT2);

    P7OUT |= (BIT0|BIT4);
    P3OUT |= (BIT6);
    P2OUT |= (BIT2);
}

void configLaunchButtons(){
    //2 Launchpad Buttons: P1.1, P2.1

    P2SEL &= (BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT0); //xxxx xx0x
    P1SEL &= (BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT0); //xxxx xx0x

    P2DIR &= (BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT0); //xxxx xx0x
    P1DIR &= (BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT0); //xxxx xx0x
}

char buttonStates(){
    //1 is not pressed
    //0 is pressed

    char inBits7, inBits3, inBits2;
    char out72=0, out71=0, out3=0, out2=0, out=0;

    inBits7 = P7IN & (BIT0|BIT4); //000x 000x, keep bits 0 and 4
    inBits3 = P3IN & (BIT6); //0x00 0000, keep bit 6
    inBits2 = P2IN & (BIT2); //0000 0x00, keep bit 2

    if (!(inBits7 & BIT0))
        out71 = 0x01; //0000 0001 //deisel
    else if (!(inBits7 & BIT4))
        out72 = 0x04; //Super
    else if (!(inBits3 & BIT6))
        out3 = 0x10; //Premium
    else if (!(inBits2 & BIT2))
        out2 = 0x40; //Regular

    out = (out71|out72|out3|out2);

    return out;
}

char launchpadButtonStates(){
    //1 is not pressed
    //0 is pressed

    char inBits11, inBits21;
    char out11=0, out21=0, out=0;

    inBits21 = P2IN & (BIT1); //0000 00x0, keep bits 1
    inBits11 = P1IN & (BIT1); //0000 00x0, keep bit 1

    if (!(inBits21 & BIT1))
        out21 = 0x01; //0000 0001
    else if (!(inBits11 & BIT1))
        out11 = 0x04; //0000 0100
    else
        out21 = 0x00;

    out = (out21|out11);

    return out;
}


void runtimerA2(void)
{
// This function configures and starts Timer A2
// Timer is counting ~0.01 seconds
//
// Input: none, Output: none

// Use ACLK, 16 Bit, up mode, 1 divider
    TA2CTL = TASSEL_1 + MC_1 + ID_0;
    TA2CCR0 = 327; // 327+1 = 328 ACLK tics = ~1/100 seconds
    TA2CCTL0 = CCIE; // TA2CCR0 interrupt enabled
}

void stoptimerA2(int reset)
{
// This function stops Timer A2 and resets the global time variable
// if input reset = 1
//
// Input: reset, Output: none

    TA2CTL = MC_0; // stop timer
    TA2CCTL0 &= ~CCIE; // TA2CCR0 interrupt disabled
    if (reset)
        timer_cnt = 0;
}

// Timer A2 interrupt service routine
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR(void)
{
    if(state == 8)
    {
       current_cnt++;
    }
    else
    {
        current_cnt = current_cnt;
    }
    timer_cnt++;

    pressed2 = launchpadButtonStates();
}

void decimalASCIIPrice(unsigned int input)
{

    for (m = 9; m >= 0; m--)
    {
        if (m == 5)
        {
            priceArray[m] = '.';
        }
        else if (m == 0)
        {
            priceArray[m] = '$';
        }
        else
        {
            priceArray[m] = ((input % 10) + 0x30);
            input = input / 10;
        }
    }

}

void decimalASCIIGallons(unsigned int gal)
{
    gal = gal / 10;
    for (j = 5; j >= 0; j--)
    {
        if (j == 3)
        {
            galArray[j] = '.';
        }
        else
        {
            galArray[j] = ((gal % 10) + 0x30);
            gal = gal / 10;
        }
    }
}


