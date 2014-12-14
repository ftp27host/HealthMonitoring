

uint32_t i2c_init(void){
	/* разрешение тактирования  I2C1 */
	RCC->APB1ENR |=  RCC_APB1ENR_I2C1EN;
}
