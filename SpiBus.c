#include "SpiBus.h"
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/atomic.h>

static USB_XpadRumble_Data_t *lastRumble;
static USB_XpadReport_Data_t *lastReport;


static uint8_t currentIndex = 0;
static uint8_t rx[20];
static uint8_t tx[20];

static const uint8_t INIT_TRANSFER_COMMAND = 0x14;

#define SPI_SS_PIN PORTB0
#define SPI_SCK_PIN PORTB1
#define SPI_MOSI_PIN PORTB2
#define SPI_MISO_PIN PORTB3

// SPI clock modes
#define SPI_MODE_0 0x00 /* Sample (Rising) Setup (Falling) CPOL=0, CPHA=0 */
#define SPI_MODE_1 0x01 /* Setup (Rising) Sample (Falling) CPOL=0, CPHA=1 */
#define SPI_MODE_2 0x02 /* Sample (Falling) Setup (Rising) CPOL=1, CPHA=0 */
#define SPI_MODE_3 0x03 /* Setup (Falling) Sample (Rising) CPOL=1, CPHA=1 */

// data direction
#define SPI_LSB 1 /* send least significant bit (bit 0) first */
#define SPI_MSB 0 /* send most significant bit (bit 7) first */

#define SPI_NO_INTERRUPT 0
#define SPI_INTERRUPT 1

// slave or master with clock diviser
#define SPI_SLAVE 0xF0

void setup_spi(uint8_t mode, int dord, int interrupt, uint8_t clock)
{
    DDRB &= ~(1<<SPI_MOSI_PIN); // input
    DDRB |= (1<<SPI_MISO_PIN); // output
    DDRB &= ~(1<<SPI_SS_PIN); // input
    DDRB &= ~(1<<SPI_SCK_PIN);// input

    SPCR = ((interrupt ? 1 : 0)<<SPIE) 
    | (1<<SPE) // enable SPI
    | (dord<<DORD) // LSB or MSB
    | (((clock != SPI_SLAVE) ? 1 : 0) <<MSTR) // Slave or Master
    | (((mode & 0x02) == 2) << CPOL) // clock timing mode CPOL
    | (((mode & 0x01)) << CPHA) // clock timing mode CPHA
    | (((clock & 0x02) == 2) << SPR1) // cpu clock divisor SPR1
    | ((clock & 0x01) << SPR0); // cpu clock divisor SPR0
  
  SPSR = (((clock & 0x04) == 4) << SPI2X); // clock divisor SPI2X
}


void SPI_init(void) {
    DDRB = (1 << PB2); // MISO
    SPCR = (1 << SPE) | (1 << SPIE);

    setup_spi(SPI_MODE_0, SPI_MSB, SPI_INTERRUPT, SPI_SLAVE);

    for(int i=0; i<20; i++) {
        tx[i]=0x14;
    }
}

void SPI_sendRumble(USB_XpadRumble_Data_t *rumble) {
    lastRumble = rumble;
    if(currentIndex < 0) {
        SPDR = 0;
    }
}

void SPI_setLastReport(USB_XpadReport_Data_t *report) {
    lastReport = report;
}

ISR(SPI_STC_vect) {
    PORTC |= 1 << 7;

    if(currentIndex < 0) {
        SPDR = 0xFF;
        uint8_t data = SPDR;
        if(data == INIT_TRANSFER_COMMAND) {
            currentIndex = 2;
        }
        return;
    }

    SPDR = tx[currentIndex];
    rx[currentIndex] = SPDR;

    PORTC = 0;
    currentIndex++;

    if(currentIndex == 20) {
        memcpy(lastReport, &rx, sizeof(rx));
        memcpy(tx, &lastRumble, sizeof(lastRumble));
        
        currentIndex = -1;
    }
}