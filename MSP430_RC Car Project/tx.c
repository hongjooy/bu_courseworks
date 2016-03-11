#include <msp430g2553.h>
#include  "API.h"         // nRF24L01's API
#include  "SPI.h"         // nRF24L01's SPI


volatile unsigned char msg[TX_PLOAD_WIDTH];
char isit;
#define BUTTON_GO 0x02 // P2.1 button 1
#define BUTTON_LEFT 0x04 // P2.2 button 2
#define BUTTON_RIGHT 0x08 // P2.3 button 3
#define BUTTON_BACK 0x10 // P2.4 button 4
#define BUTTON_ALL 0x1E

volatile unsigned int counter;
volatile unsigned int wdt_counter;


void init_button(void);

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

    init_button();

    IO_initial();
    init_NRF24L01();


    counter = 0;
    wdt_counter = 0;
    _bis_SR_register(GIE+LPM0_bits);
}

void init_button(){
	P2DIR &= ~ BUTTON_ALL;
	P2OUT |= BUTTON_ALL; // pull up
	P2REN |= BUTTON_ALL; // enable resistor
	
	counter++;
}

void interrupt button_handler(){

}
ISR_VECTOR(button_handler,".int02")


interrupt void WDT_interval_handler(){
	unsigned char b_left;
	unsigned char b_right;
	unsigned char b_back;
	unsigned char b_go;


	b_go = (P2IN & BUTTON_GO);
	b_left = (P2IN & BUTTON_LEFT);
	b_right = (P2IN & BUTTON_RIGHT);
	b_back = (P2IN & BUTTON_BACK);

	if (b_go == 0){
			msg[0]='G';
			msg[1]='O';
			msg[2]='O';
			msg[3]='O';
			nRF24L01_TxPacket(msg);
			SPI_RW_Reg(WRITE_REG+STATUS,0xff);	// clear interrupt flag(TX_DS)
		
	}
	else if (b_left == 0){
				msg[0]='L';
				msg[1]='E';
				msg[2]='F';
				msg[3]='T';
				nRF24L01_TxPacket(msg);
				SPI_RW_Reg(WRITE_REG+STATUS,0xff);	// clear interrupt flag(TX_DS)
		
			

	}
	else if (b_right == 0){
				msg[0]='R';
				msg[1]='A';
				msg[2]='I';
				msg[3]='T';
				nRF24L01_TxPacket(msg);
				SPI_RW_Reg(WRITE_REG+STATUS,0xff);	// clear interrupt flag(TX_DS)
	
	}
	else if (b_back == 0){
				msg[0]='B';
				msg[1]='A';
				msg[2]='C';
				msg[3]='K';
				nRF24L01_TxPacket(msg);
				SPI_RW_Reg(WRITE_REG+STATUS,0xff);	// clear interrupt flag(TX_DS)
			
	}
	else{
		msg[0]='N';
		msg[1]='U';
		msg[2]='L';
		msg[3]='L';
		nRF24L01_TxPacket(msg);
		SPI_RW_Reg(WRITE_REG+STATUS,0xff);	// clear interrupt flag(TX_DS)

	}


wdt_counter++;

}
ISR_VECTOR(WDT_interval_handler, ".int10")
