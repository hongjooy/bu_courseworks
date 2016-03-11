#include <msp430g2553.h>
#include  "API.h"         // nRF24L01's API
#include  "SPI.h"         // nRF24L01's SPI


uchar *end = "\r\n";
uchar RxBuf[TX_PLOAD_WIDTH];

#define H1 0x02 // P2.1 button 1 // for motor 1
#define H2 0x04 // P2.2 button 2 // for motor 1
#define H3 0x08 // P2.3 button 3 // for motor 2
#define H4 0x10 // P2.4 button 4 // for motor 2
#define H_ALL 0x1E

unsigned int counter;

// define direction register, output register, and select registers
#define TA_BIT1 0x08 //pin 1.3
#define TA_BIT2 0x02 //pin 1.1

// define the bit mask (within the port) corresponding to output TA1

void init_timerA0(void);


unsigned int counter = 0;
void main()
{

	 BCSCTL1 = CALBC1_8MHZ;
		 DCOCTL = CALDCO_8MHZ;
	  WDTCTL =(WDTPW + // (bits 15-8) password
		                   // bit 7=0 => watchdog timer on
		                   // bit 6=0 => NMI on rising edge (not used here)
		                   // bit 5=0 => RST/NMI pin does a reset (not used here)
		           WDTTMSEL + // (bit 4) select interval timer mode
		           WDTCNTCL +  // (bit 3) clear watchdog timer counter
		  		          0 // bit 2=0 => SMCLK is the source
		  		          +1 // bits 1-0 = 01 => source/8K
		  		   );
		  IE1 |= WDTIE;		// enable the WDT interrupt (in the system interrupt register IE1)


	IO_initial();
	uart_io_set();
	init_NRF24L01();
	init_timerA0();
	counter = 0;
//	P2OUT |= H1 + H3;
//	P2OUT &= ~ (H2 + H4) ;
	P2DIR |= H_ALL;
	P2OUT |= H1 + H3;
	P2OUT &= ~ (H2 + H4) ;

	_bis_SR_register(GIE+LPM0_bits);  // enable interrupts and also turn the CPU off!

 }

void init_timerA0(){


		TACTL = TACLR;					// reset clock
		TACTL = TASSEL_2+ID_3;			// clock source = SMCLK
										// clock divider=8
										// (clock still off)

		TACCTL0=0;						// Nothing on CCR0
		TACCTL1=OUTMOD_7;				// reset/set mode
		TACCR0 = 999;					// period-1 in CCR0
		TACCR1 =  50;                  // duty cycle in CCR1
		//P1SEL|=TA_BIT1 + TA_BIT2;				// connect timer 1 output to pin 2
		P1DIR |= TA_BIT1 + TA_BIT2;
		P1OUT |= TA_BIT1 + TA_BIT2;
		TACTL |= MC_1;					// timer on in up mode



}

interrupt void WDT_interval_handler(){
	SetRX_Mode();
	nRF24L01_RxPacket(RxBuf);

	if(RxBuf[0] == 'G' || RxBuf[1] == 'G' || RxBuf[2] == 'G' || RxBuf[3] == 'G'){

		P2OUT |= H1 + H3;
		P2OUT &= ~ (H2 + H4) ;
	}
	else if (RxBuf[0] == 'N'|| RxBuf[1] == 'N'||RxBuf[2] == 'N'|| RxBuf[3] == 'N'){
		P2OUT &= ~ (H1 + H2 + H3 + H4);

	}
	else if (RxBuf[0] == 'R'|| RxBuf[1] == 'R' || RxBuf[2] == 'R' || RxBuf[3] == 'R'){
		P2OUT |= H2;
		P2OUT &= ~ (H1 + H3 + H4);
	}
	else if (RxBuf[0] == 'L'||RxBuf[1] == 'L'||RxBuf[2] == 'L'||RxBuf[3] == 'L'){
		P2OUT |= H3;
		P2OUT &= ~ (H1 + H2 + H4);
	}
	else if (RxBuf[0] == 'B'||RxBuf[1] == 'B'||RxBuf[2] == 'B'||RxBuf[3] == 'B'){

		P2OUT |= H2 + H4;
		P2OUT &= ~ (H1 + H3) ;
	}
	else{
		P2OUT &= ~ (H1 + H2 + H3 + H4);

	}





}
ISR_VECTOR(WDT_interval_handler, ".int10")
































/*
interrupt void WDT_interval_handler(){
	//SetRX_Mode();
	//nRF24L01_RxPacket(RxBuf);

	counter++;
}
ISR_VECTOR(WDT_interval_handler, ".int10")

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{

	send_IRQ();
}
*/
