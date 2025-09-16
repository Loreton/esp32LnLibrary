/*
// updated by ...: Loreto Notarantonio
// Date .........: 08-09-2025 18.01.58
*/


#pragma once

// #define LN_TIME_CLASS_YDAYS_VECTOR


// ORE
private:
    int8_t m_last_yday = 99;
    int8_t m_at_last_yday = -1;
    std::map<uint8_t, int8_t> m_at_last_yday_map;


public:
    bool onDay();

