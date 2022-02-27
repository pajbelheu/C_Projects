#ifndef Open62541
    #define OPen62541
    #include <open62541/plugin/log_stdout.h>
    #include <open62541/server.h>
    #include <open62541/server_config_default.h>
    /* Files namespace_foo_flt_generated.h and namespace_foo_flt_generated.c 
    are created from FooFlt.NodeSet2.xml in the src_generated directory by CMake */
    #include "massspectro_nodeids.h"
    #include "namespace_massspectro_generated.h" 
    #include "mDef.h"
    #include <math.h>
#endif

typedef struct AMU_OPCS {
    UA_NodeId Comp_Node;
    UA_NodeId Mass_Node;
    UA_NodeId Output_Node;
    UA_NodeId Units_Node;
    UA_NodeId Error_Node;
} AMUOPC;

UA_Boolean running = true;
UA_Int32 mVal = 0;

AMURef GetRef(unsigned long int pVal){

    AMURef tAMU;
    unsigned long int iVal = pVal;
    unsigned long int RGAStart, RGAStep, AMUStep;

    RGAStart = aModel[4];
    RGAStep = aModel[1];
    AMUStep = aModel[3];

    iVal -= RGAStart;
    tAMU.iRGA = (long unsigned int)floor(iVal / RGAStep)+1; 
    /*
    tAMU.iAMU = floor((iVal % RGAStep)/AMUStep)+1;
    tAMU.iField = pVal - (RGAStart+(tAMU.iRGA-1)*RGAStep+(tAMU.iAMU-1)*AMUStep);
    */
    tAMU.iAMU = (long unsigned int)floor((iVal % RGAStep)/AMUStep);
    tAMU.iField = pVal - (RGAStart+(tAMU.iRGA-1)*RGAStep+(tAMU.iAMU)*AMUStep);
    return tAMU;
}

/* To Be deleted 
static void
updateCurrentTime(UA_Server *server) {
    UA_DateTime now = UA_DateTime_now();
    UA_Variant value;
    UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_DATETIME]);
    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "current-time-value-callback");
    UA_Server_writeValue(server, currentNodeId, value);
}

static void
beforeReadTime(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeid, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data) {
    updateCurrentTime(server);
}

static void
afterWriteTime(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeId, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "The variable was updated");
}

static void
addValueCallbackToCurrentTimeVariable(UA_Server *server) {
    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "current-time-value-callback");
    UA_ValueCallback callback ;
    callback.onRead = beforeReadTime;
    callback.onWrite = afterWriteTime;
    UA_Server_setVariableNode_valueCallback(server, currentNodeId, callback);
}

static void
updateMyTime(UA_Server *server, const UA_NodeId myNodeId) {
    UA_DateTime now = UA_DateTime_now();
    UA_Variant value;
    UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_DATETIME]);
    //UA_NodeId myNodeId = UA_NODEID_STRING(1, myNodeText);
    UA_Server_writeValue(server, myNodeId, value);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "UpdateMyTime");
}

static void
beforeReadMyTime(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeid, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "BeforeReadMyTime"); 
    updateMyTime(server, *nodeid);
}

static void
addValueCallbackToMyTime(UA_Server *server, UA_NodeId currentNodeId) {
    //UA_NodeId currentNodeId = UA_NODEID_STRING(3, "MyTime001");
    //UA_NodeId currentNodeId = UA_NODEID_NUMERIC(2, 7);
    UA_String uaString = currentNodeId.identifier.string; 
    UA_ValueCallback callback ;
    callback.onRead = beforeReadMyTime;
    callback.onWrite = afterWriteTime;
    UA_Server_setVariableNode_valueCallback(server, currentNodeId, callback);

    char* convert = (char*)UA_malloc(sizeof(char)*uaString.length+1);
    memcpy(convert, uaString.data, uaString.length );
    convert[uaString.length] = '\0';
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "AddCallBack ns=%d;id=%d;txt=%s",
                    currentNodeId.namespaceIndex, currentNodeId.identifier.numeric,
                    convert);
}
*/

/* To be deleted
static void
updateAmu(UA_Server *server, const UA_NodeId myNodeId) {
    UA_Variant sValue;
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "ReadAmu");
    mVal += 1;
    UA_Variant_setScalar(&sValue, &mVal, &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_writeValue(server, myNodeId, sValue);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "WriteAmu");
}

static void
beforeReadAmu(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeid, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "BeforeReadAmu"); 
    updateAmu(server, *nodeid);
}

static void
afterWriteAmu(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeId, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "The Amu was updated");
}

static void
addValCallbackToAmu(UA_Server *server, UA_NodeId currentNodeId) {
    UA_ValueCallback callback ;

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "AddCallBack ns=%d;id=%d",
                    currentNodeId.namespaceIndex, currentNodeId.identifier.numeric);

    callback.onRead = beforeReadAmu;
    callback.onWrite = afterWriteAmu;
    UA_Server_setVariableNode_valueCallback(server, currentNodeId, callback);
}
*/

static void
updateOutput(UA_Server *server, const UA_NodeId myNodeId) {
    
    UA_Variant sValue;
    unsigned long int lNodeId;
    AMURef tAMU;
    int rc;
    char cMsg[80] = "Initialized";

    lNodeId = myNodeId.identifier.numeric;
    tAMU = GetRef(lNodeId);
    rc = pthread_mutex_lock(&RGAMutex);
    if (rc) { /* an error has occurred */
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }
    switch (tAMU.iField)
    {
    case 1: // Cmp
        UA_Variant_setScalar(&sValue, &RGA[tAMU.iRGA][tAMU.iAMU].iCmp, &UA_TYPES[UA_TYPES_INT16]);
        break;
    case 8: // Mass
        UA_Variant_setScalar(&sValue, &RGA[tAMU.iRGA][tAMU.iAMU].iMass, &UA_TYPES[UA_TYPES_INT16]);
        sprintf(cMsg, "WriteOutput Node Id %lu, value : %d\r", lNodeId, RGA[tAMU.iRGA][tAMU.iAMU].iMass);
        break;
    case 14: // Output
        UA_Variant_setScalar(&sValue, &RGA[tAMU.iRGA][tAMU.iAMU].fVal, &UA_TYPES[UA_TYPES_DOUBLE]);
        sprintf(cMsg, "WriteOutput Node Id %lu, value : %e\r", lNodeId, RGA[tAMU.iRGA][tAMU.iAMU].fVal);
        break;
    case 20: // Unit
        UA_Variant_setScalar(&sValue, &RGA[tAMU.iRGA][tAMU.iAMU].iCmp, &UA_TYPES[UA_TYPES_STRING]);;
        sprintf(cMsg, "WriteOutput Node Id %lu, value : %c\r", lNodeId, RGA[tAMU.iRGA][tAMU.iAMU].cUnit);
        break;
    case 21: // Error
        UA_Variant_setScalar(&sValue, &RGA[tAMU.iRGA][tAMU.iAMU].iErr, &UA_TYPES[UA_TYPES_INT16]);
        sprintf(cMsg, "WriteOutput Node Id %lu, value : %d\r", lNodeId, RGA[tAMU.iRGA][tAMU.iAMU].iErr);
        break;
    default:
        // Nada;
        break;
    }
    //printf("myNodeId.identifier : %d\n", myNodeId.identifier.numeric);
    //UA_Variant_setScalar(&sValue, &dVal, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Server_writeValue(server, myNodeId, sValue);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, cMsg);
    rc = pthread_mutex_unlock(&RGAMutex);
    if (rc) {
        perror("pthread_mutex_unlock");
        pthread_exit(NULL);
    }
}

static void
beforeReadOutput(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeid, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data) {
    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "BeforeReadOutput"); 
    updateOutput(server, *nodeid);
}

static void
afterWriteOutput(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeId, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data) {
    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
    //            "The Output was updated");
}

static void
addValCallbackToOutput(UA_Server *server, UA_NodeId currentNodeId) {
    UA_ValueCallback callback ;

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "AddCallBack ns=%d;id=%d",
    //                currentNodeId.namespaceIndex, currentNodeId.identifier.numeric);

    callback.onRead = beforeReadOutput;
    callback.onWrite = afterWriteOutput;
    UA_Server_setVariableNode_valueCallback(server, currentNodeId, callback);

}

static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

void* mOPCServer(){
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    
    size_t aSize; 
    unsigned long int iSpec, iAmu, iPtr;
    AMUOPC *NodeBuffer = NULL;
    
    // Read Number of node create according to ModelDesign 
    aSize = ReadModel();
    NodeBuffer = (AMUOPC *)malloc(aSize * sizeof(AMUOPC));
    if (NodeBuffer == NULL){
        //return FAIL ;
        return NULL;
    }
    
    UA_Server *server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    UA_StatusCode retVal;
    /* create nodes from nodeset */
    if(namespace_massspectro_generated(server) != UA_STATUSCODE_GOOD) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Could not add the MassSpectro nodeset. "
        "Check previous output for any error.");
        retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
    } 
    else {
        // this will just get the namespace index, since it is already added to the server
        UA_UInt16 nsIdx = UA_Server_addNamespace(server, "http://opcfoundation.org/Coater");

        // Creation of memory table & UA_NodeId
        for(iSpec = 0; iSpec < aModel[0]; iSpec ++){    // Nb Mass Spectro
            for(iAmu = 0; iAmu < aModel[2]; iAmu ++){   // Nb AmU by Mass Spectro
                /* To Be deleted
                NodeBuffer[iSpec*aModel[2]+iAmu].AMU_ID = aModel[4]+iSpec*aModel[1]+iAmu*aModel[3];
                NodeBuffer[iSpec*aModel[2]+iAmu].AMU_Node = UA_NODEID_NUMERIC(nsIdx, NodeBuffer[iSpec*aModel[2]+iAmu].AMU_ID);
                NodeBuffer[iSpec*aModel[2]+iAmu].AMU_Val = 0.0;
                NodeBuffer[iSpec*aModel[2]+iAmu].Pp_ID = aModel[5]+iSpec*aModel[1]+iAmu*aModel[3];
                NodeBuffer[iSpec*aModel[2]+iAmu].Pp_Node = UA_NODEID_NUMERIC(nsIdx, NodeBuffer[iSpec*aModel[2]+iAmu].Pp_ID);
                NodeBuffer[iSpec*aModel[2]+iAmu].Pp_Val = 0.0;
                */
                iPtr = iSpec*aModel[1]+iAmu*aModel[3];  // iterate all AMU per Mass Spectro
                //Value  OPC idx = StartRGA + iPtr + Value offset
                NodeBuffer[iSpec*aModel[2]+iAmu].Comp_Node = UA_NODEID_NUMERIC(nsIdx, aModel[4]+iPtr+aModel[5]);
                NodeBuffer[iSpec*aModel[2]+iAmu].Mass_Node = UA_NODEID_NUMERIC(nsIdx, aModel[4]+iPtr+aModel[6]);
                NodeBuffer[iSpec*aModel[2]+iAmu].Output_Node = UA_NODEID_NUMERIC(nsIdx, aModel[4]+iPtr+aModel[7]);
                NodeBuffer[iSpec*aModel[2]+iAmu].Units_Node = UA_NODEID_NUMERIC(nsIdx, aModel[4]+iPtr+aModel[8]);
                NodeBuffer[iSpec*aModel[2]+iAmu].Error_Node = UA_NODEID_NUMERIC(nsIdx, aModel[4]+iPtr+aModel[9]);
            }
        }
        
        /* To be deleted
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "MassSpec001 AMU001 output has ns=%d;id=%d",
                    NodeBuffer[0].AMU_Node.namespaceIndex, NodeBuffer[0].AMU_Node.identifier.numeric);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "MassSpec001 AMU002 output has ns=%d;id=%d",
                    NodeBuffer[1].AMU_Node.namespaceIndex, NodeBuffer[1].AMU_Node.identifier.numeric);
        */

        // Do some additional stuff with the nodes
        //addValueCallbackToMyTime(server, TestNodeId1);
        
        for(iSpec = 0; iSpec < aModel[0]; iSpec ++){
            for(iAmu = 0; iAmu < aModel[2]; iAmu ++){
                //addValCallbackToOutput(server, NodeBuffer[iSpec*aModel[2]+iAmu].Comp_Node );
                addValCallbackToOutput(server, NodeBuffer[iSpec*aModel[2]+iAmu].Mass_Node );
                addValCallbackToOutput(server, NodeBuffer[iSpec*aModel[2]+iAmu].Output_Node );
                //addValCallbackToOutput(server, NodeBuffer[iSpec*aModel[2]+iAmu].Units_Node );
                //addValCallbackToOutput(server, NodeBuffer[iSpec*aModel[2]+iAmu].Error_Node );
            }
        }
        
        retVal = UA_Server_run(server, &running);
    }

    // free allocated memory
    free(NodeBuffer);
    UA_Server_delete(server);
    
    printf("\n OPC thread processing done\n");
    ret1  = 100;
    pthread_exit(&ret1);
    
    //return retVal == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
    return NULL;
}