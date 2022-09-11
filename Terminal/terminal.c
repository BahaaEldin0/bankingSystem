#include "terminal.h"
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData){
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    printf("Current Date: %d/%02d/%02d\n",info->tm_mday, info->tm_mon + 1,info->tm_year + 1900);

    char transactionDate[10], line[12];
    int tranLen, tranStrictLen=10;

    printf("Please Enter Transaction Date: ");
    fgets(line, sizeof(line), stdin);
    sscanf(line,"%s",&transactionDate);
    tranLen = strlen(transactionDate);

    if(tranLen != tranStrictLen){
        printf("error len %d, %s", tranLen, transactionDate);
        return WRONG_DATE;
    }
    for(int i=0;i<tranStrictLen;i++){
        if(i==2||i==5){
            if(transactionDate[i]!='/'){
                printf("error format at %d, %c", i, transactionDate[i]);

                return WRONG_DATE;
            }
        }else if (transactionDate[i]>'9' || transactionDate[i]<'0'){
            printf("error format at %d, %c", i, transactionDate[i]);
            return WRONG_DATE;
        }
    }
    memcpy(termData->transactionDate,transactionDate,tranLen);
    return Ok;
}
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData){

    if((cardData.cardExpirationDate[3]<termData.transactionDate[8])||((cardData.cardExpirationDate[3]==termData.transactionDate[8])&&(cardData.cardExpirationDate[4]<termData.transactionDate[9]))){
        return EXPIRED_CARD;
    }else if((termData.transactionDate[3]>cardData.cardExpirationDate[0])||((termData.transactionDate[3]==cardData.cardExpirationDate[0])&&(termData.transactionDate[4]>cardData.cardExpirationDate[1]))){
        return EXPIRED_CARD;
    }
    return Ok;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData){
    float amount;
    char line[50];
    printf("Please Enter your transaction amount: ");
    fgets(line, sizeof(line),stdin);
    sscanf(line, "%f",&amount);
    if(amount<= 0){
        return INVALID_AMOUNT;
    }
    termData->transAmount = amount;
    return Ok;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData){
    if(termData->maxTransAmount<termData->transAmount){
        return EXCEED_MAX_AMOUNT;
    }
    return Ok;
}


EN_terminalError_t setMaxAmount(ST_terminalData_t *termData){
    float maxAmount;
    char line[50];
    printf("Please Enter your max transaction amount: ");
    fgets(line, sizeof(line),stdin);
    sscanf(line, "%f",&maxAmount);
    if(maxAmount<= 0){
        return INVALID_MAX_AMOUNT;
    }
    termData->maxTransAmount = maxAmount;
    return Ok;
}
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);
