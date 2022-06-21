/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * @brief    A project template for M0A21 MCU.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NuMicro.h"
#include "delay.h"

void UART_Open(UART_T *uart, uint32_t u32baudrate);

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HIRC clock (Internal RC 48MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Select HCLK clock source as HIRC and HCLK source divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable UART0 clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Switch UART0 clock source to HIRC */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /* Enable PWM0 module clock */
    CLK_EnableModuleClock(PWM0_MODULE);

    /* Reset PWM0 module */
    SYS_ResetModule(PWM0_RST);

    /* Update System Core Clock */
    SystemCoreClockUpdate();

    /* Set PB multi-function pins for UART0 RXD=PB.6 and TXD=PB.4 */
    SYS->GPB_MFP1 = (SYS->GPB_MFP1 & ~(SYS_GPB_MFP1_PB4MFP_Msk | SYS_GPB_MFP1_PB6MFP_Msk)) |        \
                    (SYS_GPB_MFP1_PB4MFP_UART0_TXD | SYS_GPB_MFP1_PB6MFP_UART0_RXD);

    /* Lock protected registers */
    SYS_LockReg();
}

/*
 * This is a template project for M0A21 series MCU. Users could based on this project to create their
 * own application without worry about the IAR/Keil project settings.
 *
 * This template application uses external crystal as HCLK source and configures UART0 to print out
 * "Hello World", users may need to do extra system configuration based on their system design.
 */

int main()
{
    SYS_Init();

    /* Init UART0 to 115200-8n1 for print message */
    UART_Open(UART0, 115200);

    /* Connect UART to PC, and open a terminal tool to receive following message */
    printf("Hello World\n");

    printf("Press any key to start.\n");
    getchar();

    /* Set PA multi-function pins for PWM0 Channel 1 */
    SYS->GPA_MFP0 = (SYS->GPA_MFP0 & ~(SYS_GPA_MFP0_PA0MFP_Msk)) |
                    (SYS_GPA_MFP0_PA0MFP_PWM0_CH1);

    /* Set PWM0 timer clock prescaler */
    PWM_SET_PRESCALER(PWM0, 1, 47); // 48MHz/(47+1)=1MHz

    /* Set counter to up counting */
    PWM0->CTL1 = (PWM0->CTL1 & ~(PWM_CTL1_CNTTYPE0_Msk)) | (PWM_UP_COUNTER << PWM_CTL1_CNTTYPE0_Pos);

    /* Set PWM0 duty */
    PWM_SET_CMR(PWM0, 1, 0);

    /* Set PWM0 period */
    PWM_SET_CNR(PWM0, 1, 99); // 1MHz/(99+1)=10KHz

    /* Set output level at zero, compare up, period(center) and compare down of specified channel */
    PWM_SET_OUTPUT_LEVEL(PWM0, BIT1, PWM_OUTPUT_HIGH, PWM_OUTPUT_LOW, PWM_OUTPUT_NOTHING, PWM_OUTPUT_NOTHING);

    /* Enable output of PWM0 channel 1 */
    PWM_EnableOutput(PWM0, BIT1);

    /* Enable PWM0 channel 1 counter */
    PWM_Start(PWM0, BIT1); // CNTEN1

    /* Spin up and down the Nidec motor */
    while(1)
    {
        for(int i=1; i<=100; i++)
        {
            PWM_SET_CMR(PWM0, 1, i);
            delay(100);
        }
        delay(2000);
        for(int i=100; i>=0; i--)
        {
            PWM_SET_CMR(PWM0, 1, i);
            delay(100);
        }
        delay(5000);
    }
}

/*** (C) COPYRIGHT 2017 Nuvoton Technology Corp. ***/
