#ifndef RADIO_CONTROLLER_VIRTUAL_REGISTER_CONFIG_S_H
#define RADIO_CONTROLLER_VIRTUAL_REGISTER_CONFIG_S_H

//0x00 00 00 00
#include "hardware/timer.h"
#include "hardware/dma.h"

uint *channel_buffer;

//  RF_rx as in Receiving.  The incomeing side (duh)
typedef struct transfer_byte_s {
    //  Transfer is the full word we received.
    uint32_t transfer;

    uint8_t group_one;
    uint8_t group_two;
    uint8_t group_three;
    uint8_t group_four;

}RF_rx;

typedef struct dma_cfig_s {

    uint8_t channel;
    bool write_addr_inc;
    bool read_addr_inc;
    uint pio_dreq_rate;

}dmacs;



void rx_slice(RF_rx *rx){

    rx->group_one = ((rx->transfer & 0xFF000000) >> 24);
    rx->group_two = ((rx->transfer & 0x00FF0000) >> 16);
    rx->group_three = ((rx->transfer & 0x0000FF00) >> 8);
    rx->group_four = (rx->transfer & 0x000000FF);
}


bool setup_receiver_dma(dmacs *def, void *write_addr, void *read_addr, uint trans_c){
    dma_channel_config c = dma_channel_get_default_config(def->channel);
    channel_config_set_read_increment(&c, def->read_addr_inc);
    channel_config_set_write_increment(&c, def->write_addr_inc);
    channel_config_set_dreq(&c, def->pio_dreq_rate);
    dma_channel_configure(def->channel, &c, write_addr, read_addr, trans_c, false);
}


bool dma_transfer(repeating_timer_t *rt){

    uint *chan_ptr = &channel_buffer;
    volatile void *read_adr;
    volatile void *write_adr;

    if(dma_channel_is_busy(&chan_ptr)){
        printf("DMA Busy\n");
    }else{
        dma_channel_set_read_addr(chan_ptr, read_adr, false);
        dma_channel_set_write_addr(chan_ptr, write_adr, false);
        dma_channel_set_trans_count(chan_ptr, 1, true);

        dma_channel_start(&chan_ptr);
    }
}

#endif