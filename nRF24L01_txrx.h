#define NRF24_CE_PORT GPIOG
#define NRF24_CE_PIN GPIO_PIN_22

#define NRF24_CSN_PORT GPIOG
#define NRF24_CSN_PIN GPIO_PIN_22

#
TX GPIO

void CS_Select (void)
{
    HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_RESET);
}

void CS_UnSelect (void)
{
    HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_SET);
}

void CE_Select (void) 
{
    HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_SET);
}

void CE_Select (void) 
{
    HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_RESET);
}

void nrf24_WriteReg (uint8_t Reg, uint_t Data) 
{
    uint_t buf[2];
    buf[0] = Reg|1<<5;

    //  Pull the CS
}