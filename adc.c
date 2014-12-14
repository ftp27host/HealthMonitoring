
#include "common.h"
#include "adc.h"
#include "tim.h"

/*****************************************************************************/

/* массив результатов преобразований */
uint16_t adc_data_ar[ADC_DATA_CNT];
/* флаг завершения  пропуска насыщения */
volatile uint8_t adc_fltr_end;
/* сохраненное значение для пропуска насыщения */
volatile uint32_t adc_last_val;
/* приращение функции для фильтрации насыщения */
uint32_t adc_fltr_satur_val;

/*****************************************************************************/

uint32_t adc_init(void){
    /* счетчик аварийного выхода при калибровке */
    uint32_t adc_em_cntr = ADC_EM_CNT;
    /* конфигурирование вывода PA7 как аналогового входа для работы с ADC */

    GPIOA->MODER |= GPIO_MODER_MODER7;  //Устанавливаем 11 - аналоговый вход/выход для PA5
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR7; //Очищаем быты 00 - аналоговый вход/выход без подтяжки для PA5

    //GPIOA->CRL &= ~GPIO_CRL_CNF1; // Analog mode
    //GPIOA->CRL &= ~GPIO_CRL_MODE1; // General purpose output push-pull
    /* установка делителя для частоты ADC */
     //RCC->CFGR &= ~RCC_CFGR_ADCPRE;

    RCC->CR |= RCC_CR_HSION; //Включаем внутренний генератор HSI - 16МГц
    while(!(RCC->CR&RCC_CR_HSIRDY)); //Ждем его стабилизации

    /* разрешение тактирования ADC */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;


    /* первое преобразование канал 1 PA1 */
    ADC1->SQR5 |=  0x7;//ADC_SQR3_SQ1_0;
    //ADC1->SMPR5 &= ~0x7;
    ADC1->SMPR0 &= ~0xE00000;

    //ADC1->SQR5 |= 0x7;//ADC_SQR5_SQ1_0;
    /* скорость преобразования на 1 канале 1.5 такта */
    /* включение scan mode */
    ADC1->CR1 |= ADC_CR1_SCAN;
    ADC1->CR1 &= ~ADC_CR1_RES;
    ADC1->CR2 &= ~0x70; // DELS == 000

    /* разрешение работы от внешнего триггера */

   ADC1->CR2 |= ADC_CR2_EXTEN |
    		       (ADC_EXTTRIG_SEL & ADC_CR2_EXTSEL);


	/* разрешение запуска */
	ADC1->CR2 |= ADC_CR2_ADON;
    /* сброс калибровки АЦП */
    //ADC1->CR2 |= ADC_CR2_RSTCAL;
		/* включение прерывания в контроллере прерываний */
    NVIC_EnableIRQ(ADC1_IRQn);
		/* сброс калибровки
    while(ADC1->CR2 & ADC_CR2_RSTCAL){

        adc_em_cntr--;
        if(adc_em_cntr == 0)
            return (ADC_INIT_FAIL);

    }
    adc_em_cntr = ADC_EM_CNT;*/
    /* калибровка

    ADC1->CR2 |= ADC_CR2_CAL;
    while(ADC1->CR2 & ADC_CR2_CAL){

		adc_em_cntr--;
        if(adc_em_cntr == 0)
			return (ADC_INIT_FAIL);

    } */
	// инициализация флага
	adc_fltr_end = 0;
	adc_last_val = 0UL;
	adc_fltr_satur_val = ADC_FLTR_VAL;
		
    return (ADC_INIT_OK);
}

/*****************************************************************************/

void adc_print_res(uint32_t adc_point_count){
    /* индекс в массиве результов преобразований */
    uint32_t adc_res_cntr;

    for(adc_res_cntr = 0; 
		adc_res_cntr < adc_point_count; 
		adc_res_cntr++){
		/* печать результата по 1 в строке */
		printv(adc_data_ar[adc_res_cntr]);
    	//toString(adc_data_ar[adc_res_cntr]);
    	//print();
		print(";\n\r");

    }
    print("end");
		
}

/*****************************************************************************/

void adc_en_dis_dma_req(uint8_t en_dis){

	if(en_dis == ADC_EN_DMA_REQ){
			
		ADC1->CR2 |= ADC_CR2_DMA;			
		ADC1->CR1 &= ~ADC_CR1_EOCIE;
		
	}else{
			
		adc_last_val = 0UL;
		ADC1->CR2 &= ~ADC_CR2_DMA;
		ADC1->CR1 |= ADC_CR1_EOCIE;
			
	}

}

/*****************************************************************************/

void ADC1_IRQHandler(void){
	
		//uint32_t cur_val;
		/*
		cur_val = ADC1->DR;
		if(cur_val < adc_last_val)
			//if((adc_last_val - cur_val) > adc_fltr_satur_val){
								tim3_stop();
				adc_fltr_end = 1;
			//}

		adc_last_val = cur_val;
	*/
}

/*****************************************************************************/
