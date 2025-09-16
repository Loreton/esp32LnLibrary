//
// updated by ...: Loreto Notarantonio
// Date .........: 07-09-2025 20.06.06
//

// LnTime_Class.h
#pragma once


public:
    const char* msecToHMS(char *buffer, uint8_t buffer_len, uint32_t millisec=0, bool addMilliSec=false, bool stripHeader=false); // msec from boot (or millisec) HH:MM:SS.msec
    const char* msecToHMS(uint32_t millisec, bool withMilliSec=false, bool stripHours=false); // usa un buffer interno
    const char* secToHMS(uint32_t millisec, bool stripHours=false); // usa un buffer interno
    int8_t secondsToMinute(); // Restituisce i secondi mancanti al prossimo minuto completo
    uint32_t millisecOfDay(int offset = 0);
    uint32_t secondsOfDay(int offset = 0);
    uint32_t minutesOfDay(int offset = 0);
    uint32_t getEpoch(unsigned long offset = 0);

