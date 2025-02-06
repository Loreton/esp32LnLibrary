/*
// updated by ...: Loreto Notarantonio
// Date .........: 05-02-2025 20.02.20
*/

#ifndef __INPUT_PIN_FUNCTIONS_H__
    #define __INPUT_PIN_FUNCTIONS_H__


    void pinDisplayState(io_input_pin_struct_t *pin);
    void resetInputPin(io_input_pin_struct_t *p);
    bool readInputPin(io_input_pin_struct_t *p, uint8_t signal_pin=99, bool is_buzzer=false);




#endif