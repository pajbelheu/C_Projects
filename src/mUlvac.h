#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GetVal(char* pBuf){
    
    printf("Enter name: ");
    scanf("%17s", pBuf);
}

void SubString(char* pBuf, char *sStr, int vPos){

    int c = 0;
    int vLen = strlen(sStr);

    while (c < vLen){
      sStr[c] = pBuf[vPos+c-1];
      c++;
    }
    sStr[c] = '\0';
}

void sClcChkSum(char* pBuf, char* sChk){

    int i, vSum;
    int iLen = strlen(pBuf);
    BYTE a, b;    // to extract 2 first bytes 
    BYTE bSum;    // sum of 2 first bytes

    vSum = 0;
    for(i=0; i<iLen; i++){
        vSum += pBuf[i];
    }
    a = (vSum & 0xFF);                  //extract first byte
    b = ((vSum >> 8) & 0xFF);           //extract second byte
    bSum = a + b;
    a = (bSum & 0xFF);                  //extract first byte
    sprintf(sChk, "%x", a);             //Byte to Char[]
    if(sChk[0] > 96 && sChk[0] < 123){
        sChk[0] -= 32;                  //Upper case 
    }
    if(sChk[1] > 96 && sChk[1] < 123){
        sChk[1] -= 32;                  //Upper case 
    }
    //printf("sChk= %s\n",sChk);
}

int TestChkSum(char *pBuf){
    
    int bRet = 0;
    int iLen = strlen(pBuf);
    char vStr[iLen], tChk[2], sChk[2];
    
    strcpy(vStr, pBuf);   
    vStr[iLen-2] = '\0';
    sClcChkSum(vStr, tChk);
 
    sChk[0]= pBuf[iLen-2];
    sChk[1]= pBuf[iLen-1];

    bRet = (sChk[0] == tChk[0])*(sChk[1] == tChk[1]);

    return bRet;
}

void ParseBuf(char *sBuffer){
    
    int iLen = strlen(sBuffer);
    int iTer = (iLen - 5)/18;
    int iRGA, iPtr, rc;
    char *ePtr;

    char sRGA[] = "0";
    char sUnit[] = "A";
    char sStr[] = "000,0.00E-00,0000";
    char sMass[] = "000";
    char sVal[] = "0.00E-00";
    char sErr[] = "0000";
    
    SubString(sBuffer, sRGA, 3);
    iRGA = atoi(sRGA);
    //printf("RGA n° : %d\n", iRGA);
    SubString(sBuffer, sUnit, 4);
    rc = pthread_mutex_lock(&RGAMutex);
    if (rc) { /* an error has occurred */
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }
    for (iPtr = 0; iPtr <= iTer; iPtr++){
        SubString(sBuffer, sStr, 6+iPtr*18);
        SubString(sStr, sMass, 1);
        RGA[iRGA][iPtr].iMass = atoi(sMass);
        SubString(sStr, sVal, 5);
        //RGA[iRGA][iPtr].fVal = atof(sVal);
        RGA[iRGA][iPtr].fVal = strtod(sVal, &ePtr);
        SubString(sStr, sErr, 14);
        RGA[iRGA][iPtr].iErr = atoi(sErr);
        RGA[iRGA][iPtr].cUnit = sUnit[0];
        /*
        printf("Substring %s at pos %d\n", sStr, iPtr);
        printf("iMass : %d, fVal  : %e, iErr  : %d, cUnit : %c\n", 
            RGA[iRGA][iPtr].iMass,
            RGA[iRGA][iPtr].fVal,
            RGA[iRGA][iPtr].iErr,
            RGA[iRGA][iPtr].cUnit
            );
        */
    }
    rc = pthread_mutex_unlock(&RGAMutex);
    if (rc) {
        perror("pthread_mutex_unlock");
        pthread_exit(NULL);
    }
}

void* sComUlVac(){

    int fd;                             // File Descriptor 
    char sBuffer[iSize];

    char* eBuffer = malloc(sizeof(char[2]));
    int iVal; 
    
    //Open Serial com
    fd = oPenCom(portname);

    /* Cyclic call */
    signal(SIGINT, intHandler);
    while (keepRunning){
        for (iVal = 0; iVal < eRGANber; iVal++){
            gEtDataCom(fd, sCall[iVal], sBuffer, iSize);
            if (TestChkSum(sBuffer) == 1){
                //printf("TestChkSum is TRUE\n");
                ParseBuf(sBuffer);
            }
            else{
                printf("Rcv String : %s\n", sBuffer);
                printf("TestChkSum is FALSE\n");
            }
        }
        iVal = 0;
    } 
    
    //Close Com
    cLoseCom(fd);

    //Free Memory
    free(eBuffer);

    printf("\n Ulvac thread processing done\n");
    ret2  = 200;
    pthread_exit(&ret2);

    return NULL;

}