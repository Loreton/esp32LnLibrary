/*
// updated by ...: Loreto Notarantonio
// Date .........: 08-09-2025 18.01.55
*/


#pragma once

// #define LN_TIME_CLASS_HOURS_VECTOR


// ORE
private:
    int8_t m_last_hour = 99;
    int8_t m_at_last_hour = -1;
    std::map<uint8_t, int8_t> m_at_last_hour_map;


public:
    bool onHour();
    bool atHour(uint8_t hour);


#ifdef LN_TIME_CLASS_HOURS_VECTOR
    private:
        std::vector<uint8_t> m_at_hours_to_monitor;
        std::map<uint8_t, bool> m_at_hour_flags;
        std::map<uint8_t, int8_t> m_at_last_hour_vector_map;
        int8_t m_at_last_hour_vector = -1;

    public:
        bool atHourFlag(uint8_t hour);
        bool getAtHourFlag(uint8_t hour);
        void setAtHourFlag(const std::vector<uint8_t>& hours);
        void updateAtHourFlags(void);
#endif