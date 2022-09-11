#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData){
    char name[25], line[26];
    int nameLen, maxLen=24, minLen=20;

    printf("Please Enter your Name:");
    fgets(line, sizeof(line), stdin);
    sscanf(line,"%99[^\n]",&name);

    nameLen = strlen(name);
    if((nameLen>maxLen)||(nameLen<minLen)){
        printf("error %d, %s, %d, %d \n", nameLen, name, maxLen, minLen);
        return WRONG_NAME;
    }
    printf("correct %d, %s, %d, %d \n", nameLen, name, maxLen, minLen);

    memcpy(cardData->cardHolderName,name,nameLen);
    return OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData){
    char expDate[5], line[7];
    int expLen, expStrictLen=5, expWrongFormat=0;

    printf("Please Enter your Card's Expiry Date: ");
    fgets(line, sizeof(line), stdin);
    sscanf(line,"%s",&expDate);
    expLen = strlen(expDate);

    if(expLen != expStrictLen){
        return WRONG_EXP_DATE;
    }
    for(int i=0;i<expStrictLen;i++){
        if(i==2){
            if(expDate[i]!='/'){
                return WRONG_EXP_DATE;
            }
        }else if (expDate[i]>'9' || expDate[i]<'0'){
            return WRONG_EXP_DATE;
        }
    }
    memcpy(cardData->cardExpirationDate,expDate,expLen);
    return OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData){
    char pan[20], line[21];
    int panLen, maxLen=19, minLen=17;

    printf("Please Enter your Account Number: ");
    fgets(line, sizeof(line), stdin);
    sscanf(line,"%s",&pan);
    panLen = strlen(pan);
    if((panLen>maxLen)||(panLen<minLen)){
        return WRONG_PAN;
    }
    for(int i=0;i<panLen-1;i++){
        if((pan[i]>'9' || pan[i]<'0') && ((pan[i]<'a' || pan[i]>'z')&&(pan[i]<'A' || pan[i]>'z'))){
            return WRONG_PAN;
        }
    }
    strcpy(cardData->primaryAccountNumber,pan);
    return OK;
}

