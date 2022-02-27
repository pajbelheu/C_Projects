#ifndef mDef
    #define mDef

    //#define _POSIX_C_SOURCE 199309L (real time extension)
    #define _POSIX_C_SOURCE 200112L // 

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <signal.h>
    #include <pthread.h>
    #include <unistd.h>
    #include <errno.h>

    //Defines
    enum { eRGANber=8, eRGAMass=20};
    //#define dRGAMass 40;
    //#define dRGANber 8;
    #define FAIL 1;

    // Constants 
    const int iSize = 256;
    //const unsigned long int RGAOffset = 2795;
    //const unsigned long int RGAStep = 307;
    //const unsigned long int AMUStep = 15;
    const int FALSE = 1;
    const int TRUE = 0;

    const char *sCall[8] = {"DA1B6\r\n", "DA2B7\r\n", "DA3B8\r\n", "DA4B9\r\n", 
                            "DA5BA\r\n", "DA6BB\r\n", "DA7BC\r\n", "DA8BD\r\n"};
    char *portname = "/dev/ttyUSB0";    // Serial Com

    //Typedef's
    typedef unsigned char BYTE;

    typedef struct AMUDatas
    {
        /* data */
        int iCmp, iMass, iErr;
        double fVal;
        char cUnit;
    } AMUData;

    typedef struct AMURefs
    {
        /* data */
        unsigned long int iRGA, iAMU, iField;
    } AMURef;

    // Signals to interrupt program

    static volatile int keepRunning = 1;

    void intHandler(int dummy) {
        keepRunning = 0;
    }

    //Global variables
    AMUData RGA[eRGANber][eRGAMass];
    pthread_mutex_t RGAMutex = PTHREAD_MUTEX_INITIALIZER;
    unsigned long int aModel[10]= {0,0,0,0,0,0,0,0,0,0}; // see ReadModel function definition
    int ret1, ret2;     // return of both threads

#endif
