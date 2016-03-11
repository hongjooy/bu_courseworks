#include <msp430g2553.h>
#include "API.h"

typedef unsigned char uchar;
/*   _ _ _ _ _ _ _ _ _ _ _ _
 *  |1|2|  1->GND  2->VCC  |
 *  |-|-|                  |
 *  |3|4|  3->CE   4->CSN  |
 *  |-|-|                  |
 *  |5|6|  5->CLK  6->MOSI |
 *  |-|-|                  |
 *  |7|8|  7->MISO 8->IRQ  |
 *  |_|_|_ _ _ _ _ _ _ _ _ |
 */
#define BIT(x)	(1 << (x))
#define CE        7
#define CSN       0
#define CLK       5
#define MOSI      6
#define MISO      4
#define IRQ       2
#define PORT     P1OUT
#define PDIR     P1DIR
#define PIN      P1IN

#define GLED     6
#define Key      3

uchar i;
uchar TX_ADDRESS[TX_ADR_WIDTH] = { 0x34, 0x43, 0x10, 0x10, 0x01 }; // Define a static TX address ;
uchar RX_ADDRESS[RX_ADR_WIDTH] = { 0x34, 0x43, 0x10, 0x10, 0x01 }; // Define a static RX address ;

///////////////IO è…³ä½�å®šç¾©////////////////////////////
void IO_initial()
{
	PDIR |= BIT(CSN) + BIT(CE) + BIT(CLK) + BIT(MOSI);
	PDIR |= BIT(GLED);

	P1REN |= BIT(IRQ);
	P1OUT |= BIT(IRQ);

	P1REN |= BIT(Key);
	P1OUT |= BIT(Key);

	PORT &= ~BIT(GLED);
	PORT &= ~BIT(CE); //CHIP DISABLE
	PORT |= BIT(CSN); //CSN IS PULL HIGH.DISABLE THE OPERATION
	PORT &= ~BIT(CLK);//CLK IS LOW

}

//******************************************************************************************
//å»¶é�²
//******************************************************************************************
void Delay_us(unsigned int n)
{
	for (; n > 0; n--)
		;
}
//////////////////init clock
void FaultRoutine(void)
{
	while (1)
		;
}

void ConfigClocks(void)
{
	if (CALBC1_1MHZ == 0xFF || CALDCO_1MHZ == 0xFF)
		FaultRoutine(); 	// If calibration data is erased
							// run FaultRoutine()
	BCSCTL1 = CALBC1_1MHZ; 	// Set range
	DCOCTL = CALDCO_1MHZ; 	// Set DCO step + modulation
	BCSCTL3 |= LFXT1S_2; 	// LFXT1 = VLO
	IFG1 &= ~OFIFG; 		// Clear OSCFault flag
	BCSCTL2 = 0; 			// MCLK = DCO = SMCLK
}

//////////////LED Open or Close//////////////
void GLED_off()
{
	PORT &= ~BIT(GLED);
}

void GLED_on()
{
	PORT |= BIT(GLED);
}

void GLED_ray()
{
	PORT ^= BIT(GLED);
}
///////////////Get IRQ////////////////////////
uchar Get_Key(void)
{
	return (PIN & (BIT(Key)));
}
/////////////////////////SPI å¯«å…¥/////////////////////////////////////
uchar SPI_RW(uchar byte)
{
	uchar bit_ctr;
	PORT &= ~(BIT(CLK));

	for (bit_ctr = 0; bit_ctr < 8; bit_ctr++) // output 8-bit
	{

		if (byte & 0x80)
			PORT |= BIT(MOSI);
		else
		{
			PORT &= ~(BIT(MOSI));			// output 'byte', MSB to MOSI
		}

		byte = (byte << 1); 				// shift next bit into MSB..
		PORT |= BIT(CLK); 					// Set clk high..
		if (PIN & (BIT(MISO)))
			byte |= BIT0; 					// capture current MISO bit
		else
		{
			byte &= ~BIT0;
		}
		PORT &= ~BIT(CLK); 					// set  clk low
	}
	PORT &= ~(BIT(MOSI)); 					//PULL DOWN THE MOSI
	return (byte); 							// return read byte
}

/////////////////////æš«å­˜å™¨è®€å¯«/////////////////////////////////////////////
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;

	PORT &= ~BIT(CSN); 		// CSN low, init SPI transaction
	status = SPI_RW(reg); 	// select register
	SPI_RW(value); 			// ..and write value to it..
	PORT |= BIT(CSN); 		// CSN high again

	return (status); 		// return nRF24L01 status byte
}

////////////////////////å¾žnRF24L01è³‡æ–™/////////////////////////////////////
uchar SPI_Read(uchar reg)
{
	uchar reg_val;

	PORT &= ~BIT(CSN); 		// CSN low, initialize SPI communication...
	SPI_RW(reg); 			// Select register to read from..
	reg_val = SPI_RW(0); 	// ..then read registervalue
	PORT |= BIT(CSN); 		// CSN high, terminate SPI communication

	return (reg_val); 		// return register value
}

/////////////////////////read RX payload, Rx/Tx address//////////////////////////

uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar bytes)
{
	uchar status, byte_ctr;

	PORT &= ~BIT(CSN); 		// Set CSN low, init SPI tranaction
	status = SPI_RW(reg); 	// Select register to write to and read status byte

	for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
		pBuf[byte_ctr] = SPI_RW(0); // Perform SPI_RW to read byte from nRF24L01

	PORT |= BIT(CSN); 		// Set CSN high again

	return (status); 		// return nRF24L01 status byte
}

/////////////////////////write TX payload, Rx/Tx address/////////////////////////////////

uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bytes)
{
	uchar status, byte_ctr;

	PORT &= ~BIT(CSN); 		// Set CSN low, init SPI tranaction
	status = SPI_RW(reg);   // Select register to write to and read status byte
	for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
		SPI_RW(*pBuf++);
	PORT |= BIT(CSN); 		// Set CSN high again
	return (status); 		// return nRF24L01 status byte
}

//****************************************************************************************
//NRF24L01åˆ�å§‹åŒ–
//***************************************************************************************/
void init_NRF24L01(void)
{
	Delay_us(100);
	PORT &=~BIT(CE);
	PORT |= BIT(CSN);
	PORT &=~BIT(CLK);
	SPI_Write_Buf(WRITE_REG + TX_ADDR,  TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,  TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
	//SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);             // Writes data to TX payload
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);                              // ç¦�æ­¢æ‰€æœ‰ Pipe è‡ªå‹•å›žæ‡‰
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);                          // ç¦�æ­¢æ‰€æœ‰æŽ¥æ”¶é€šé�“
	//SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x00);                       // ç¦�æ­¢è‡ªå‹•é‡�ç™¼
	SPI_RW_Reg(WRITE_REG + RF_CH, 0); 								  // Select RF channel 40
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);                 // Ã¦â€°Â¢Ã§Â¦Â»Ã¨Â«â€°Ã¥Â½Â¶Ã¦ï¿½â€¦Ã¦â€œâ€šÃ©â€¦â€”Ã¥Æ’â€¦
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07); 					      // TX_PWR:-12dBm, Datarate:1Mbps, LNA:HCURR
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e); // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..
}
//****************************************************************************************************/
//å‡½æ•¸:void SetRX_Mode(void)
//åŠŸèƒ½:æ•¸æ“šæŽ¥æ”¶æ¨¡å¼�
//****************************************************************************************************/
void SetRX_Mode(void)
{
	PORT &=~BIT(CE);
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   	
	PORT |=BIT(CE);
	Delay_us(330);//ä¸�èƒ½å¤ªå°�
}

uchar nRF24L01_RxPacket(uchar* rx_buf)
{
    uchar revale=0;
    uchar sta=SPI_Read(STATUS);	    
	if(sta&0x40)                	
	{
	    PORT &=~BIT(CE);			// SPI
	    SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
	    revale =1;					
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   
	return revale;
}
//***********************************************************************************************************

//**********************************************************************************************************/
void nRF24L01_TxPacket(uchar * tx_buf)
{
	PORT &=~BIT(CE);		
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,  TX_ADDRESS, TX_ADR_WIDTH);
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			  

	PORT |=BIT(CE);		    //CE 
	Delay_us(40);
}
/////////////////////////initializes one nRF24L01 device//////////////////////////////////

void RX_Mode(void)
{

	PORT &=~BIT(CE);
        SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);                         // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..

  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,  RX_ADDRESS, RX_ADR_WIDTH); // Use the same address on the RX device as the TX device

  	//SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);                             // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);                         // Enable Pipe0
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);                               // Select RF channel 40
  	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);                // Select same RX payload width as TX Payload width
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);                          // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR

  	PORT |=BIT(CE);                                                  // Set CE pin high to enable RX device

  //  This device is now ready to receive one packet of 16 bytes payload from a TX device sending to address
  //  '3443101001', with auto acknowledgment, retransmit count of 10, RF channel 40 and datarate = 2Mbps.

}

///////////////////////TRANS FUNCTION///////////////////////////////////////////////////
void TX_Mode(void)
{


	PORT &=~BIT(CE);
        SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);                         // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..

  	SPI_Write_Buf(WRITE_REG + TX_ADDR,  TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,  TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
  	//Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);        // Writes data to TX payload

  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x00);                            
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x00);                         
  	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x00);                       
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);                               // Select RF channel 40
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x03);                          // TX_PWR:-12dBm, Datarate:1Mbps, LNA:HCURR

	PORT |=BIT(CE);                                                  
        for(i=100;i>0;i--);
        PORT &=~BIT(CE);


}

///////////////////////////////Tx_Packet////////////////////////////
void TX_Packet(uchar *tx_buf)
{

     PORT &=~BIT(CE);
     SPI_Write_Buf(WRITE_REG + TX_ADDR,  TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
     SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);              // Writes data to TX payload
     PORT |= BIT(CE);
     for(i=100;i>0;i--);
       PORT &=~BIT(CE);
}
