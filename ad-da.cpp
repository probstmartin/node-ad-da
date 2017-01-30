#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <bcm2835.h>
#include <unistd.h>

#define BCM2708_PERI_BASE   0x20000000
#define GPIO_BASE           (BCM2708_PERI_BASE + 0x200000)
#define MAXTIMINGS          100
#define DHT11               11
#define DHT22               22
#define AM2302              22

int initialized = 0;
unsigned long long last_read[32] = {};
float last_temperature[32] = {};
float last_humidity[32] = {};

unsigned long long getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long long time = (unsigned long long)(tv.tv_sec) * 1000 +
                              (unsigned long long)(tv.tv_usec) / 1000;
    return time;
}

long readADC()
{
    int32_t adc[8];
    int32_t volt[8];
    uint8_t i;
    uint8_t ch_num;
    int32_t iTemp;
    uint8_t buf[3];

    ADS1256_StartScan(1);
    ch_num = 4;

    while((ADS1256_Scan() == 0));
    for (i = 0; i < ch_num; i++)
    {
        adc[i] = ADS1256_GetAdc(i);
        volt[i] = (adc[i] * 100) / 167;    
    }
    
    for (i = 0; i < ch_num; i++)
    {
        buf[0] = ((uint32_t)adc[i] >> 16) & 0xFF;
        buf[1] = ((uint32_t)adc[i] >> 8) & 0xFF;
        buf[2] = ((uint32_t)adc[i] >> 0) & 0xFF;
        printf("%d=%02X%02X%02X, %8ld", (int)i, (int)buf[0], (int)buf[1], (int)buf[2], (long)adc[i]);                

        iTemp = volt[i];    /* uV  */
        if (iTemp < 0)
        {
            iTemp = -iTemp;
            printf(" (-%ld.%03ld %03ld V) \r\n", iTemp /1000000, (iTemp%1000000)/1000, iTemp%1000);
        }
        else
        {
            printf(" ( %ld.%03ld %03ld V) \r\n", iTemp /1000000, (iTemp%1000000)/1000, iTemp%1000);                    
        }
                
    }
    printf("\33[%dA", (int)ch_num);  
    bsp_DelayUS(100000);      

    return 0;
}

int initialize()
{

    uint8_t id;

    if (!bcm2835_init())
    {
        #ifdef VERBOSE
        puts("BCM2835 initialization failed.");
        #endif
        return 1;
    }
    else
    {
        #ifdef VERBOSE
        puts("BCM2835 initialized.");
        #endif
        initialized = 1;        

        bcm2835_spi_begin();
        bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST );      // The default
        bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                   // The default
        bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8192); // The default
        bcm2835_gpio_fsel(SPICS, BCM2835_GPIO_FSEL_OUTP);//
        bcm2835_gpio_write(SPICS, HIGH);
        bcm2835_gpio_fsel(DRDY, BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(DRDY, BCM2835_GPIO_PUD_UP);

        id = ADS1256_ReadChipID();
        #ifdef VERBOSE
        printf("\r\n");
        printf("ID=\r\n");
        #endif
        if (id != 3)
        {
            printf("Error, ASD1256 Chip ID = 0x%d\r\n", (int)id);
        }
        else
        {
            printf("Ok, ASD1256 Chip ID = 0x%d\r\n", (int)id);
        }

        ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_15SPS);

        return 0;
    }
}
