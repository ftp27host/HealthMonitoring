
#include "common.h"
#include "adc.h"
#include "tim.h"

/*****************************************************************************/

/* ������ ����������� �������������� */
uint16_t adc_data_ar[ADC_DATA_CNT];
/* ���� ����������  �������� ��������� */
volatile uint8_t adc_fltr_end;
/* ����������� �������� ��� �������� ��������� */
volatile uint32_t adc_last_val;
/* ���������� ������� ��� ���������� ��������� */
uint32_t adc_fltr_satur_val;

/*****************************************************************************/

uint32_t adc_init(void){
    /* ������� ���������� ������ ��� ���������� */
    uint32_t adc_em_cntr = ADC_EM_CNT;
    /* ���������������� ������ PA7 ��� ����������� ����� ��� ������ � ADC */

    GPIOA->MODER |= GPIO_MODER_MODER7;  //������������� 11 - ���������� ����/����� ��� PA5
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR7; //������� ���� 00 - ���������� ����/����� ��� �������� ��� PA5

    //GPIOA->CRL &= ~GPIO_CRL_CNF1; // Analog mode
    //GPIOA->CRL &= ~GPIO_CRL_MODE1; // General purpose output push-pull
    /* ��������� �������� ��� ������� ADC */
     //RCC->CFGR &= ~RCC_CFGR_ADCPRE;

    RCC->CR |= RCC_CR_HSION; //�������� ���������� ��������� HSI - 16���
    while(!(RCC->CR&RCC_CR_HSIRDY)); //���� ��� ������������

    /* ���������� ������������ ADC */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;


    /* ������ �������������� ����� 1 PA1 */
    ADC1->SQR5 |=  0x7;//ADC_SQR3_SQ1_0;
    //ADC1->SMPR5 &= ~0x7;
    ADC1->SMPR0 &= ~0xE00000;

    //ADC1->SQR5 |= 0x7;//ADC_SQR5_SQ1_0;
    /* �������� �������������� �� 1 ������ 1.5 ����� */
    /* ��������� scan mode */
    ADC1->CR1 |= ADC_CR1_SCAN;
    ADC1->CR1 &= ~ADC_CR1_RES;
    ADC1->CR2 &= ~0x70; // DELS == 000

    /* ���������� ������ �� �������� �������� */

   ADC1->CR2 |= ADC_CR2_EXTEN |
    		       (ADC_EXTTRIG_SEL & ADC_CR2_EXTSEL);


	/* ���������� ������� */
	ADC1->CR2 |= ADC_CR2_ADON;
    /* ����� ���������� ��� */
    //ADC1->CR2 |= ADC_CR2_RSTCAL;
		/* ��������� ���������� � ����������� ���������� */
    NVIC_EnableIRQ(ADC1_IRQn);
		/* ����� ����������
    while(ADC1->CR2 & ADC_CR2_RSTCAL){

        adc_em_cntr--;
        if(adc_em_cntr == 0)
            return (ADC_INIT_FAIL);

    }
    adc_em_cntr = ADC_EM_CNT;*/
    /* ����������

    ADC1->CR2 |= ADC_CR2_CAL;
    while(ADC1->CR2 & ADC_CR2_CAL){

		adc_em_cntr--;
        if(adc_em_cntr == 0)
			return (ADC_INIT_FAIL);

    } */
	// ������������� �����
	adc_fltr_end = 0;
	adc_last_val = 0UL;
	adc_fltr_satur_val = ADC_FLTR_VAL;
		
    return (ADC_INIT_OK);
}

/*****************************************************************************/

void adc_print_res(uint32_t adc_point_count){
    /* ������ � ������� ��������� �������������� */
    uint32_t adc_res_cntr;

    for(adc_res_cntr = 0; 
		adc_res_cntr < adc_point_count; 
		adc_res_cntr++){
		/* ������ ���������� �� 1 � ������ */
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
