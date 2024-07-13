// #include "volt.h"
// #include "adc.h"

// uint32_t Battery_Voltage;

// uint8_t ReadBatVolt(uint32_t* Volt)
// {
//     if (HAL_ADC_Start(&hadc1) != HAL_OK) return 1;
//     if (HAL_ADC_PollForConversion(&hadc1, 1) != HAL_OK) return 2;
//     *Volt = HAL_ADC_GetValue(&hadc1);
//     *Volt = *Volt * 3300 >> 12;
//     if (HAL_ADC_Stop(&hadc1) != HAL_OK) return 3;
//     return 0;
// }