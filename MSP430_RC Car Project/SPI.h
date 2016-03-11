typedef unsigned char  uchar;
typedef unsigned  int   uint;


void IO_initial();
void Delay_us(uint n);
void GLED_on();
void GLED_off();
void GLED_ray();
void ConfigClocks(void);
uchar Get_Key(void);
uchar SPI_RW(uchar byte);
uchar SPI_RW_Reg(uchar reg, uchar value);
uchar SPI_Read(uchar reg);
uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar bytes);
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bytes);
void init_NRF24L01(void);
void SetRX_Mode(void);
uchar nRF24L01_RxPacket(uchar* rx_buf);
void nRF24L01_TxPacket(uchar * tx_buf);
void TX_Packet(uchar *tx_buf);
void RX_Mode(void);
void TX_Mode(void);


