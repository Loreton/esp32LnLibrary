## ref:

1. https://www.markdownguide.org/basic-syntax/
2. https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax
3. https://markdowntohtml.com

# esp32LnLibrary


## Structures
    struct PINpulsetime {
        bool       enabled    = false; // solo in caso di msecs_default
        bool       auto_on_off    = false; // OFF as soon pulsetime expire
        uint32_t   msecs_default = 0; // millis
        uint32_t   msecs_start   = 0; // millis()
        uint32_t   cur_duration = 0; // millis()
        char       hms_remaining[12]; // formato HH:MM:SS
        uint32_t   remaining=0;
        uint32_t   elapsed=0;
    };
