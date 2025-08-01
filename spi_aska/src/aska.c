#include "aska.h"

struct spi_dt_spec spispec = SPI_DT_SPEC_GET(DT_NODELABEL(aska), SPIOP, 0);

int ASKA_spi_setup()
{
    int err;

	err = spi_is_ready_dt(&spispec);
	if (!err) {
 		printk("Error: SPI device is not ready, err: %d", err);
 		return 0;
	}

	return 1;
}

int ASKA_spi_write_buff(void *bufferData, size_t bufferSize)
{
    int err;

	struct spi_buf	tx_spi_buf 		= {.buf = bufferData, .len = bufferSize};
	struct spi_buf_set tx_spi_buf_set	= {.buffers = &tx_spi_buf, .count = 1};

	err = spi_write_dt(&spispec, &tx_spi_buf_set);
	if (err < 0) {
 		printk("spi_write_dt() failed, err %d", err);
 		return err;
	}

	return 0;
}

void ASKA_write_reg(uint8_t IC_addr ,uint8_t add, uint32_t value)
{
    uint8_t tx_buffer[5];
	
    tx_buffer[0] = add | IC_addr;
    
    tx_buffer[1] = (uint8_t)(value >> 24) & 0xff;
    tx_buffer[2] = (uint8_t)(value >> 16) & 0xff;
    tx_buffer[3] = (uint8_t)(value >> 8) & 0xff;
    tx_buffer[4] = (uint8_t)(value >> 0) & 0xff;

    ASKA_spi_write_buff(tx_buffer,5);
	//SPI_transaction(tx_buffer,5);
    //CS_SetLow();    
    //SPI1_WriteBlock(tx_buffer,5);
    //SPI1_BufferWrite(tx_buffer,5);    
    //CS_SetHigh();

}


void ASKA_test1(uint8_t ic_add)
{
        //Disable 
        //ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
		//new program
        /*
		ASKA_write_reg(ic_add, ASKA_ELE1,0x00008000);
		ASKA_write_reg(ic_add, ASKA_ELE2,0x00004000);
		ASKA_write_reg(ic_add, ASKA_CONF0,0x19672190);
		ASKA_write_reg(ic_add, ASKA_CONF1,0x00906420);
         */
        ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
		//new program
		
		ASKA_write_reg(ic_add, ASKA_CONF0,0x0a0147d0);
		ASKA_write_reg(ic_add, ASKA_CONF1,0x00907800);
        ASKA_write_reg(ic_add, ASKA_ELE1,0x00000001);
		ASKA_write_reg(ic_add, ASKA_ELE2,0x00000002);
        
}

void ASKA_test2(uint8_t ic_add)
{
        ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
		//new program
		
		ASKA_write_reg(ic_add, ASKA_CONF0,0x0a1727d0);
		ASKA_write_reg(ic_add, ASKA_CONF1,0x009028a0);
        ASKA_write_reg(ic_add, ASKA_ELE1,0x00000001);
		ASKA_write_reg(ic_add, ASKA_ELE2,0x00000002);
}

void ASKA_test3(uint8_t ic_add)
{
        ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
		//new program
		
		ASKA_write_reg(ic_add, ASKA_CONF0,0x32cb2190);
		ASKA_write_reg(ic_add, ASKA_CONF1,0x00925810);
        ASKA_write_reg(ic_add, ASKA_ELE1,0x00000001);
		ASKA_write_reg(ic_add, ASKA_ELE2,0x00000002);
}

void ASKA_test4(uint8_t ic_add)
{
        ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
		//new program
		
		ASKA_write_reg(ic_add, ASKA_CONF0,0x32c99190);
		ASKA_write_reg(ic_add, ASKA_CONF1,0x0090c808);
        ASKA_write_reg(ic_add, ASKA_ELE1,0x00000001);
		ASKA_write_reg(ic_add, ASKA_ELE2,0x00000002);
}

void ASKA_setProgram( 
        uint8_t ic_add, // 0x00,0x40,0x80,0xc0 (0-3)
        uint32_t freq, // [Hz] 5 - 50 Hz
        uint32_t amplitude, // [mA] 0 - 50 mA
        uint32_t ON_time, // [ds] 0 - 40 ds
        uint32_t OFF_time, //[ds] 0 - 120 ds
        uint32_t ramp_time, // ramptime in [ds]
        uint32_t enable,  // 1 or 0
        uint32_t phaseDuration, // 50 us (1) - 350 us (7) 
        uint32_t ele1,
        uint32_t ele2)
{
    ASKA_NMES_struct ASKA;   
    ASKA_CONF ASIC;
   
    
    // Number of 20 kHz cycles to generate freq
    ASKA.freq = (uint16_t)(BASE_FREQ/freq); 
    
    ASKA.amplitude = amplitude;
    
    // number of ON/OFF pulses 
    ASKA.ON_time = (freq*ON_time)/10;
    
    ASKA.OFF_time = (freq*OFF_time)/10;
    
    // number of pulses during ramp UP/DOWN 
    ASKA.ramp = (freq*ramp_time)/10;
            
    // ramp_factor
    if (ASKA.ramp > 0) {
        ASKA.ramp_factor = ((amplitude*16)/ASKA.ramp);
    } else {
        ASKA.ramp_factor = 0;
    }
        
    ASKA.enable = enable;    
    ASKA.phaseDuration = phaseDuration;    
    ASKA.ele1 = ele1;    
    ASKA.ele2 = ele2;
    
    ASIC.conf0 = 0;
    ASIC.conf1 = 0;
    ASIC.ele1 = 0;
    ASIC.ele2 = 0;
    
    ASIC.conf0 |= (ASKA.freq              & 0x00000fff);
    ASIC.conf0 |= ((ASKA.amplitude << 12) & 0x0003f000);
    ASIC.conf0 |= ((ASKA.ramp << 18)      & 0x00fc0000);
    ASIC.conf0 |= ((ASKA.ON_time << 24)   & 0xff000000);
    
    ASIC.conf1 |= (ASKA.ramp_factor             & 0x000003ff);
    ASIC.conf1 |= ((ASKA.OFF_time << 10)        & 0x000ffc00);
    ASIC.conf1 |= ((ASKA.enable << 20)          & 0x00100000);
    ASIC.conf1 |= ((ASKA.phaseDuration << 21)   & 0x00e00000);
    
    ASIC.ele1 = ASKA.ele1;
    ASIC.ele2 = ASKA.ele2;
    
    
    //Reprogram asic
    ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
    
    ASKA_write_reg(ic_add, ASKA_CONF0,ASIC.conf0);
	ASKA_write_reg(ic_add, ASKA_CONF1,ASIC.conf1);
    ASKA_write_reg(ic_add, ASKA_ELE1,ASIC.ele1);
	ASKA_write_reg(ic_add, ASKA_ELE2,ASIC.ele2);
        
    
}

