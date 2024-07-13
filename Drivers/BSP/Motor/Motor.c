#include "Motor.h"
#include "Utilities.h"

#define Motor_A1(x) HAL_GPIO_WritePin(Motor_A1_GPIO_Port,Motor_A1_Pin,(GPIO_PinState)(x));
#define Motor_A2(x) HAL_GPIO_WritePin(Motor_A2_GPIO_Port,Motor_A2_Pin,(GPIO_PinState)(x));
#define Motor_B1(x) HAL_GPIO_WritePin(Motor_B1_GPIO_Port,Motor_B1_Pin,(GPIO_PinState)(x));
#define Motor_B2(x) HAL_GPIO_WritePin(Motor_B2_GPIO_Port,Motor_B2_Pin,(GPIO_PinState)(x));



extern ExtruderConfig Config;

uint8_t Motor_Init(void)
{
    if (Current_Config->Motor == MOTOR_TYPE_28BYJ48)
    {

    }
    else if (Current_Config->Motor == MOTOR_TYPE_N20)
    {

    }
    return 0;
}

// void Stepper_Half_Step(uint8_t Step)
// {
//     /* Last Phase Memorize */
//     if(hal_tim_IT)
//     Motor_A1(1);
// }

void Stepper_Control(uint8_t Direction, uint8_t Speed)
{
    if (Direction == MOTOR_FORWARD)
    {

    }
    if (Direction == MOTOR_REVERSE)
    {

    }
}

void Stepper_Brake(void)
{
    Motor_A1(0);
    Motor_A2(0);
    Motor_B1(0);
    Motor_B2(0);
}

// void Motor_Forward(uint8_t Speed)
// {

// }

// void Motor_Backward(uint8_t Speed)
// {

// }