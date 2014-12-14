#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

/*****************************************************************************/

/* ������������ ������ ������� ����������� � ������ */
#define ADC_DATA_CNT 			800
/* ������������ �������� ��� ���������� ��������� */
#define ADC_MAX_SATUR_VAL 4095
/* �������� ����������� ������� ���������� */
#define ADC_EM_CNT 				0xFFFFFFUL
/* ����� �������� ������� ��� */
#define ADC_EXTTRIG_SEL 	4 << 24
/* �������� ������� ��� ������ ������� ��������� */
#define ADC_FLTR_VAL 			200UL

/*****************************************************************************/

enum {

    ADC_INIT_OK,
    ADC_INIT_FAIL = 16

};

enum {
	
	ADC_DIS_DMA_REQ,	
	ADC_EN_DMA_REQ
		
	
};

/*****************************************************************************/

extern uint16_t adc_data_ar[];
extern volatile uint8_t adc_fltr_end;
extern uint32_t adc_fltr_satur_val;

/*****************************************************************************/

extern uint32_t adc_init(void);
extern void adc_print_res(uint32_t adc_point_count);
extern void adc_en_dis_dma_req(uint8_t en_dis);

/*****************************************************************************/

#endif // ADC_H_INCLUDED
