#include "adc.h"

AdcAvgResult_t adc_avgval;

//初始化ADC															   
void Adc_Init()
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

  //先初始化ADC1通道0 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA0 通道0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
	
	  //再初始化ADC1通道1 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PA1 通道1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 2;//2个转换在规则序列中 转换规则序列1和2 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度		
	
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
}				  
//获得ADC值
void Get_Adc()   
{
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//等待转换结束
	adc_avgval.adc_val_pa0 = ADC_GetConversionValue(ADC1);
	TRACE_WP("adc_val_pa0 = %d\r\n",adc_avgval.adc_val_pa0);	
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//等待转换结束
	adc_avgval.adc_val_pa1 = ADC_GetConversionValue(ADC1);
	TRACE_WP("adc_val_pa1 = %d\r\n\r\n",adc_avgval.adc_val_pa1);	
}
//获取times次,然后平均
//times:获取的平均次数
//offset:前n次测试结果不计入
void Get_Adc_Average(u8 times, u8 offset)
{
	u32 temp0_val=0;
	u32 temp1_val=0;
	u8 t;
	
	for(t = 0; t < (times + offset); t++)
	{
		Get_Adc();
		if(t >= offset)
		{
			TRACE_WP("----- %d -----\r\n", (t+1-offset));
			temp0_val += adc_avgval.adc_val_pa0;
			temp1_val += adc_avgval.adc_val_pa1;
			vTimeDelayMs(5);
		}
		adc_avgval.adc_val_pa0 = 0;
		adc_avgval.adc_val_pa1 = 0;
		vTimeDelayMs(5);
	}
	
	adc_avgval.average0_val = temp0_val/times;
	TRACE_WP("average0_val = %d\r\n",adc_avgval.average0_val);
	adc_avgval.average1_val = temp1_val/times;
	TRACE_WP("average1_val = %d\r\n\r\n",adc_avgval.average1_val);
	
	return ;
} 
	 
void get_DP_and_DM_voltage_event(float threshold, float redundancy)
{
	float ptm_typec_dm_voltage;
	float ptm_typec_dp_voltage;
	float upper_limit;
	float lower_limit;
	
	Get_Adc_Average(20,6);
	
	upper_limit = threshold + redundancy;
	lower_limit = threshold - redundancy;
	TRACE_WP("lower_limit = %0.2f\r\n",lower_limit);
	TRACE_WP("upper_limit = %0.2f\r\n\r\n",upper_limit);
	
	ptm_typec_dm_voltage = (adc_avgval.average0_val * 3.3)/4096;
	TRACE_WP("ptm_typec_dm_voltage = %0.2f\r\n",ptm_typec_dm_voltage);
	ptm_typec_dp_voltage = (adc_avgval.average1_val * 3.3)/4096;
	TRACE_WP("ptm_typec_dp_voltage = %0.2f\r\n\r\n",ptm_typec_dp_voltage);
	
	if(((ptm_typec_dm_voltage > lower_limit) && (ptm_typec_dm_voltage < upper_limit)) && ((ptm_typec_dp_voltage > lower_limit) && (ptm_typec_dp_voltage < upper_limit)))
	{
		TRACE_WP("[PASS] DM DP voltage test success\r\n\r\n");
		LED_G_ON;
	}
	else
	{
		TRACE_WP("[FAIL] DM DP voltage test failure\r\n\r\n");
		LED_R_ON;
	}
	
	if(ptm_typec_dm_voltage > upper_limit)
	{
		TRACE_WP("DM Voltage Above threshold\r\n");
	}
	if(ptm_typec_dm_voltage < lower_limit)
	{
		TRACE_WP("DM Voltage Below threshold\r\n");
	}
	if(ptm_typec_dp_voltage > upper_limit)
	{
		TRACE_WP("DP Voltage Above threshold\r\n");
	}
	if(ptm_typec_dp_voltage < lower_limit)
	{
		TRACE_WP("DP Voltage Below threshold\r\n");
	}
}








