#ifndef _SERVO_H_
#define _SERVO_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    PWM_T *pwm;
    uint32_t u32pwmChannelMask;
    volatile uint32_t *pu32pdio;
    uint32_t u32gpioMfpSave;
    uint8_t u8degree;
} STR_SERVO_T;

void servo_attach(STR_SERVO_T *pServo, PWM_T *pwm, uint32_t u32pwmChannelMask, volatile uint32_t *pu32pdio);
uint8_t servo_write(STR_SERVO_T *pServo, uint8_t u8degree);
uint8_t servo_read(STR_SERVO_T *pServo);
void servo_detach(STR_SERVO_T *pServo);

#ifdef __cplusplus
}
#endif

#endif
