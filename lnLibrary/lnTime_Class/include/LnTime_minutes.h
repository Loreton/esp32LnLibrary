/*
// updated by ...: Loreto Notarantonio
// Date .........: 08-09-2025 18.01.54
*/


#pragma once

// #define LN_TIME_CLASS_MINUTES_VECTOR

private:
    int8_t m_last_minute = -1;
    int8_t m_at_last_minute = -1;
    std::map<uint32_t, uint32_t> m_last_epoch_minutes_map;
    std::map<uint8_t,  uint8_t>  m_at_last_minute_map;

public:
    bool onMinute();
    bool atMinute(uint8_t minute);
    // bool onMinuteModulo(uint32_t modulo, bool trueOnFirstRun=false);




#ifdef LN_TIME_CLASS_MINUTES_VECTOR
    private:
        std::vector<uint8_t> m_at_minutes_to_monitor;
        std::map<uint8_t, bool> m_at_minute_flags;
        std::map<uint8_t, int8_t> m_at_last_minute_vector_map;
        int8_t m_at_last_minute_vector = -1;

    public:
        bool atMinuteFlag(uint8_t minute);
        bool getAtMinuteFlag(uint8_t minute);
        void setAtMinuteFlag(const std::vector<uint8_t>& minutes);
        void updateAtMinuteFlags(void);
#endif