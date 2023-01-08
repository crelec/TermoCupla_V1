#include "mbed.h"

SPI spi(NC,PB_14,PB_13); // mosi, miso, sclk  , MOSI=NC
DigitalOut cs(PC_4); 

//static BufferedSerial serial_port(USBTX, USBRX);
static BufferedSerial serial_port(PA_2, PA_3);

//Variable global
int dato_Entero;
int dato_decimal;
char mensaje[40]={0};

//Prototipo de funciones
void configurar(void);
void leerTemparatura(void);

// Hilos 
Thread hilo_lecturaTermocupla;

//funcion decimal entero
void Decimal_Entero(float decimal){
dato_Entero=int(decimal);
dato_decimal=(decimal-int(decimal))*100;
}

// main() runs in its own thread in the OS
int main()
{
    configurar();
    hilo_lecturaTermocupla.start(leerTemparatura);
    serial_port.write("Hola.! Arranque del programa.\n\r",31);
    
    while (true) {
    }
}

void configurar(void)
{
    cs = 1;
    spi.format(16,1); // 16 bit modo 1
    spi.frequency(4000000); //frecuencia 4Mhz
}

void leerTemparatura(void)
{
    float temp  = 0;
    long long tempByte= 0;

    while(true)
    {
    cs = 0;
    wait_us(5);    // Espero 16 flancos de reloj SPI
    tempByte  =  spi.write(0);
    cs = 1;
    tempByte=((tempByte)>>3);//Temperatura 12 bit D14=MSB a D3=LSB
    temp = tempByte*0.25;
    Decimal_Entero(temp);
    //sprintf(mensaje,"%llu\n\r",tempByte);
    //serial_port.write(mensaje,sizeof(mensaje));
    sprintf(mensaje,"Temperatura: %d.%d C\n\r",dato_Entero,dato_decimal);
    serial_port.write(mensaje,sizeof(mensaje));
    ThisThread::sleep_for(1000ms);    
    }
 } 
