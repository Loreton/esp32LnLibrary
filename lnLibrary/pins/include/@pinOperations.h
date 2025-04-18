/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-04-2025 13.00.57
*/

#ifndef __PIN_OPERATIONS_FUNCTIONS_H__
    #define __PIN_OPERATIONS_FUNCTIONS_H__
    #include "@mainStructures.h"



    // pioDebug.cpp
    void    displayPinStatus(io_input_pin_struct_t *pin);
    void    displayPinStatus(io_output_pin_struct_t *p);

    // pioOperations.cpp
    void    pulseGenerator(io_output_pin_struct_t *p, uint32_t onTime, uint32_t offTime);

    int8_t  readLongPressPin(io_input_pin_struct_t *p);

    uint8_t readInputPin(io_input_pin_struct_t *p);
    uint8_t readOutputPin(io_output_pin_struct_t *p);


    // uint8_t isActed(io_output_pin_struct_t *p);
    // #define readOutputPin isActed
    // inline uint8_t pippo(io_output_pin_struct_t *p) {return isActed(p); }
    inline uint8_t isActed(io_output_pin_struct_t *p) {return readOutputPin(p); }
    inline uint8_t readPin(io_output_pin_struct_t *p) {return readOutputPin(p); }
    inline uint8_t readPin(io_input_pin_struct_t *p) {return readInputPin(p); }

    void    pinToggle(io_output_pin_struct_t *p);
    void    pinOFF(io_output_pin_struct_t *p);
    void    pinON(io_output_pin_struct_t *p);
    void    pinPulseON(io_output_pin_struct_t *p, int32_t mSecsPulseLength=0, bool autoOff=true);






    // pinPulseTime.cpp
    void        startPulsetime(io_output_pin_struct_t *p, int32_t msec_pulseLength=0, bool auto_off=false);
    void        stopPulsetime(io_output_pin_struct_t *p);
    bool        isPulsetimeExpired(io_output_pin_struct_t *p);
    uint32_t    remainingPulseTime(io_output_pin_struct_t *p);



    // pinTimer.cpp
    void        pinTimerStart(io_input_pin_struct_t *p, int32_t msec_length=0);
    void        pinTimerStop(io_input_pin_struct_t *p);
    bool        pinTimerExpired(io_input_pin_struct_t *p);
    uint32_t    pinTimerRemaining(io_input_pin_struct_t *p);



    // activeBuzzer.cpp
    void activeBuzzerON(void);
    void activeBuzzerOFF(void);
    void activeBuzzerToggle(void);
    void activeBuzzerPulse(uint16_t ms_duration);

    // passieBuzzer.cpp
    void passiveBuzzerPulse(int16_t frequency, uint32_t duration);
    void passiveBuzzerScaleUp(void);
    void passiveBuzzerScaleDown(void);


#endif