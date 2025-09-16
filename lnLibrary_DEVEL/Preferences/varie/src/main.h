#ifndef MAIN_H
    #define MAIN_H

    #ifdef MAIN_CPP
        bool RO_MODE = true;
        bool RW_MODE = false;

    #else
        extern bool RO_MODE;
        extern bool RW_MODE;

    #endif

#endif