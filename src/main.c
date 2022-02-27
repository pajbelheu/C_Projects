#include "mDef.h"
#include "mJson.h"
#include "OpcFunc.h"
#include "mSerial.h"
#include "mUlvac.h"

#include <signal.h>
#include <stdlib.h>

int main() {

    pthread_t sComThread, sOPCServer;
    int *ptr[2];                        // thread return

    pthread_mutex_init(&RGAMutex, NULL);
    if (pthread_create(&sComThread, NULL, &sComUlVac, NULL) != 0) {
        perror("Failed to create thread sCom");
    }
    if (pthread_create(&sOPCServer, NULL, &mOPCServer, NULL) != 0) {
        perror("Failed to create thread OPCServer");
    }
    if (pthread_join(sComThread, NULL) != 0) {
        perror("Failed to join thread sCom");
    }
    if (pthread_join(sOPCServer, NULL) != 0) {
        perror("Failed to join thread OPCServer");
    }

    pthread_join(sComThread, (void**)&(ptr[0]));
    pthread_join(sOPCServer, (void**)&(ptr[1]));
    printf("\n sCom thread return value is [%d]\n", *ptr[0]);
    printf("\n sOpc thread return value is [%d]\n", *ptr[1]);

    pthread_mutex_destroy(&RGAMutex);

    return 0;

}