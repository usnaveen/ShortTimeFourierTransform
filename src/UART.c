void initUART(void);
void sendUART(unsigned char mydata);
unsigned char receiveUART(void);
void printUART(unsigned char mydata[]);

void initUART(void)
{
initPLL();
PINSEL0 = 0x05; // UART0 TXD - P0.0, RXD - P0.1
U0LCR = 0x83; // DLAB=1 , Allowing Baud rate change
U0DLL = 0x97; // 0x0097 for 9600 (97)
U0DLM = 0x00; // 
U0LCR = 0x03; // DLAB=0 , Allowing Tx Rx and stopping Baud rate change
}

void sendUART(float data)
{
char str[];
sprintf(str, "%d", floorf(data));
int i=0;
while(str[i]!="NULL"){
  U0THR = str[i];
  while((U0LSR & (0x01<< 5 ))==0); // THREmpty
}
}

unsigned char receiveUART(void)
{
unsigned char mydata;
while ((U0LSR & 0x01)== 0);
mydata = U0RBR;
return mydata;
}

void printUART(CMPLX* freq_components)
{
int i=0;
while(*freq_components!='\0')
{
sendUART(cabsf(*freq_components));
freq_components++;
}
}
