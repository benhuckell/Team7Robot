#ifndef HARDWARETIMER_H_
#define HARDWARETIMER_H_

#include "Arduino.h"
#include "stm32_gpio_af.h"

typedef enum {
    //libmaple:                             // HAL compatible
    TIMER_DISABLED,
    TIMER_PWM,                              // == TIM_OCMODE_PWM1
    TIMER_OUTPUT_COMPARE,                   // == TIM_OCMODE_TIMING           no output, useful for only-interrupt

    //other:
    TIMER_OUTPUT_COMPARE_ACTIVE,            // == TIM_OCMODE_ACTIVE           pin is set high when counter == channel compare
    TIMER_OUTPUT_COMPARE_INACTIVE,          // == TIM_OCMODE_INACTIVE         pin is set low when counter == channel compare
    TIMER_OUTPUT_COMPARE_TOGGLE,            // == TIM_OCMODE_TOGGLE           pin toggles when counter == channel compare
    TIMER_OUTPUT_COMPARE_PWM1,              // == TIM_OCMODE_PWM1             pin high when counter < channel compare, low otherwise
    TIMER_OUTPUT_COMPARE_PWM2,              // == TIM_OCMODE_PWM2             pin low when counter < channel compare, high otherwise
    TIMER_OUTPUT_COMPARE_FORCED_ACTIVE,     // == TIM_OCMODE_FORCED_ACTIVE    pin always high
    TIMER_OUTPUT_COMPARE_FORCED_INACTIVE,   // == TIM_OCMODE_FORCED_INACTIVE  pin always low

    //Input capture
    TIMER_INPUT_CAPTURE_RISING,          // == TIM_INPUTCHANNELPOLARITY_RISING
    TIMER_INPUT_CAPTURE_FALLING,         // == TIM_INPUTCHANNELPOLARITY_FALLING

    //PWM input capture on channel 1 + channel 2
    //TIMER_INPUT_CAPTURE_PWM,             // == TIM_INPUTCHANNELPOLARITY_RISING (channel 1) + TIM_INPUTCHANNELPOLARITY_FALLING (channel 2)

    //Encoder mode
    //TIMER_ENCODER                        // == TIM_ENCODERMODE_TI1
} TIMER_MODES;

#define TIMER_DEFAULT_PIN 0xFF

class HardwareTimer {
public:
    HardwareTimer(TIM_TypeDef *instance, const stm32_tim_pin_list_type *pin_list, int pin_list_size);

    void pause(void);

    void resume(void);

    uint32_t getPrescaleFactor();

    void setPrescaleFactor(uint32_t factor);

    uint32_t getOverflow();

    void setOverflow(uint32_t val);

    uint32_t getCount(void);

    void setCount(uint32_t val);

    uint32_t setPeriod(uint32_t microseconds);

    void setMode(int channel, TIMER_MODES mode, uint8_t pin = TIMER_DEFAULT_PIN);

    uint32_t getCompare(int channel);

    void setCompare(int channel, uint32_t compare);

    //Add interrupt to period update
    void attachInterrupt(void (*handler)(void));

    void detachInterrupt();

    //Add interrupt to channel
    void attachInterrupt(int channel, void (*handler)(void));

    void detachInterrupt(int channel);

    void refresh(void);

    uint32_t getBaseFrequency();

    TIM_HandleTypeDef handle = {0};

    TIM_OC_InitTypeDef channelOC[4];

    TIM_IC_InitTypeDef channelIC[4];

    //Callbacks: 0 for update, 1-4 for channels
    void (*callbacks[5])(void);

    const stm32_tim_pin_list_type *tim_pin_list;

    int tim_pin_list_size;

private:
    void resumeChannel(int channel, int timChannel);

};
extern HardwareTimer Timer1;
#ifdef TIM1
    
#endif

#ifdef TIM2
    extern HardwareTimer Timer2;
#endif

#ifdef TIM3
    extern HardwareTimer Timer3;
#endif

#ifdef TIM4
    extern HardwareTimer Timer4;
#endif

#endif