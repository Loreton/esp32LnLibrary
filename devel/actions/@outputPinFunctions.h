/*
// updated by ...: Loreto Notarantonio
// Date .........: 05-02-2025 19.55.03
*/
// #include <ESP32Time.h> // per definire ESP32Time rtc;

#ifndef __OUTPUT_PIN_FUNCTIONS_H__
    #define __OUTPUT_PIN_FUNCTIONS_H__

    bool isTimerExpired(io_output_pin_struct_t *p);

    // void displayAllPins(void);
    void displayOutputPin(io_output_pin_struct_t *pin);
    void pulseSound(uint8_t pin, int16_t frequency, uint32_t duration);
    void buzzerScaleUp(io_output_pin_struct_t *p);
    void buzzerScaleDown(io_output_pin_struct_t *p);

    void pinPulse(io_output_pin_struct_t *p, uint16_t delaymS, uint8_t times=1);
    void pinLowPulse(uint8_t pin, uint16_t delaymS=1, uint8_t times=1);
    void pinHighPulse(uint8_t pin, uint16_t delaymS=1, uint8_t times=1);

    uint8_t toggleOutputPin(io_output_pin_struct_t *p, int16_t pulsetime=0);
    void setOutputPinOFF(io_output_pin_struct_t *p);
    void setOutputPinON(io_output_pin_struct_t *p, int16_t pulsetime=0);

#endif