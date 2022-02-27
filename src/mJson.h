#ifndef mJson
    #define mJson
    #include "mDef.h"
    #include <stdio.h>
    #include <json-c/json.h>
    #include <errno.h>
#endif

long unsigned int GetIntFld(struct json_object *parsed_json, char Text[10], struct json_object *fIeld){
    
    long unsigned int iVal;

    json_object_object_get_ex(parsed_json, Text, &fIeld);
    iVal = json_object_get_int(fIeld);

    return iVal;
}

long unsigned int ReadModel(){

    size_t fSize = 1024;
    size_t n;
    FILE *fp;
    char buffer[fSize];
    struct json_object *parsed_json;
    struct json_object *NbRGA, *StepRGA, *NbAMU, *StepAMU, *StartRGA, 
                        *OffComp, *OffMass, *OffOutput, *OffUnit, *OffError;
    
    fp = fopen("../build/modelconfig.json", "r");
    if (fp == NULL) {
        perror("Failed to open modelconfig.json\n");
        return (1);
    }
    n = fread(buffer, fSize, 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);
    aModel[0] = GetIntFld(parsed_json, "NbRGA", NbRGA);
    aModel[1] = GetIntFld(parsed_json, "StepRGA", StepRGA);
    aModel[2] = GetIntFld(parsed_json, "NbAMU", NbAMU);
    aModel[3] = GetIntFld(parsed_json, "StepAMU", StepAMU);
    aModel[4] = GetIntFld(parsed_json, "StartRGA", StartRGA);
    aModel[5] = GetIntFld(parsed_json, "OffComp", OffComp);
    aModel[6] = GetIntFld(parsed_json, "OffMass", OffMass);
    aModel[7] = GetIntFld(parsed_json, "OffOutput", OffOutput);
    aModel[8] = GetIntFld(parsed_json, "OffUnit", OffUnit);
    aModel[9] = GetIntFld(parsed_json, "OffError", OffError);
    
    return (aModel[0] * aModel[2]);

}
