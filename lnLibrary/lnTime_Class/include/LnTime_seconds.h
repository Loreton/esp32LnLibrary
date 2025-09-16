/*
// updated by ...: Loreto Notarantonio
// Date .........: 08-09-2025 18.01.27
*/


#pragma once

// #define LN_TIME_CLASS_SECONDS_VECTOR


// SECONDI
private:
    int8_t m_last_second = -1;
    int8_t m_at_last_second = -1;
    std::map<uint32_t, uint32_t> m_last_epoch_seconds_map;
    std::map<uint8_t,  uint8_t>  m_at_last_second_map;
    // bool onSecondModulo(uint32_t modulo, bool trueOnFirstRun=false);

public:
    bool atSecondFlag(uint8_t second);
    bool onSecond(void);
    bool atSecond(uint8_t second);
    bool onModulo(uint32_t seconds, bool trueOnFirstRun);
    bool onModulo(uint8_t h, uint8_t m, uint8_t s);



#ifdef LN_TIME_CLASS_SECONDS_VECTOR // # defined in -DFlags
    private:
        std::vector<uint8_t> m_at_seconds_to_monitor;
        std::map<uint8_t, bool> m_at_second_flags;
        std::map<uint8_t, int8_t> m_at_last_second_vector_map;
        int8_t m_at_last_second_vector = -1;

    public:
        bool getAtSecondFlag(uint8_t second);
        void setAtSecondFlag(const std::vector<uint8_t>& seconds);
        void updateAtSecondFlags(void);
#endif