void initUART(void);
void sendUART(unsigned char mydata);
unsigned char receiveUART(void);
void printUART(unsigned char mydata[]);

void initUART(void)
{
initPLL();
PINSEL0 = 0x05; // UART0 TXD - P0.0, RXD - P0.1
U0LCR = 0x83; // DLAB=1 , 11 8bit UART
U0DLL = 0x87; // 0x0187 for 9600 (391)
U0DLM = 0x01; // 
U0LCR = 0x03; // DLAB=0 , 11 8bit UART
}

void sendUART(unsigned char mydata)
{
U0THR = mydata;
while((U0LSR & (0x01<< 5 ))==0); // THREmpty
}

unsigned char receiveUART(void)
{
unsigned char mydata;
while ((U0LSR & 0x01)== 0);
mydata = U0RBR;
return mydata;
}

void printUART(unsigned char mydata[])
{
int i=0;
while(mydata[i]!='\0')
{
sendUART(mydata[i]);
i++;
}
}
