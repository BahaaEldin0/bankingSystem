#include "app.h"

void appStart(){
    ST_cardData_t cardData;
    ST_terminalData_t termData;
    ST_transaction_t transData;
    EN_terminalError_t ret_term;
    EN_serverError_t initDatabase;
    EN_cardError_t ret_card;
    printf("System Starting ...\n");
    printf("Retrieving Database...\n");
    initDatabase = initializeServer();

    if(initDatabase!=ok){
        printf("Database Error error code: %d\n",initDatabase);
    }else{
        printf("Database Retrieved successfully\n");
    }

    ret_card = getCardHolderName(&cardData);
    while (ret_card == WRONG_NAME) {
        printf("\n\t--===WRONG_NAME===--\n");
        printf("Re-enter:\n");
        ret_card = getCardHolderName(&cardData);
    }
    ret_card = getCardExpiryDate(&cardData);
    while (ret_card == WRONG_EXP_DATE) {
        printf("\n\t--===WRONG_EXP_DATE===--\n");
        printf("Re-enter:\n");
        ret_card = getCardExpiryDate(&cardData);
    }
    ret_card = getCardPAN(&cardData);
    while (ret_card == WRONG_PAN) {
        printf("\n\t--===WRONG_PAN===--\n");
        printf("Re-enter:\n");
        ret_card = getCardPAN(&cardData);
    }
    ret_term = setMaxAmount(&termData);
    while (ret_term == INVALID_MAX_AMOUNT) {
        printf("\n\t--===INVALID AMOUNT===--\n");
        printf("Re-enter:\n");
        ret_term = setMaxAmount(&termData);
    }
    ret_term = getTransactionDate(&termData);
    while (ret_term == WRONG_DATE) {
        printf("\n\t--===WRONG_DATE===--\n");
        printf("Re-enter:\n");
        ret_term = getTransactionDate(&termData);
    }
    ret_term = isCardExpired(cardData, termData);
    if (ret_term == EXPIRED_CARD) {
        printf("\n\n\t--===EXPIRED_CARD===--\n");
        return;
    }
    ret_term = getTransactionAmount(&termData);
    while (ret_term == INVALID_AMOUNT) {
        printf("\n\t--===INVALID_AMOUNT===--\n");
        printf("Re-enter:\n");
        ret_term = getTransactionAmount(&termData);
    }
    ret_term = isBelowMaxAmount(&termData);
    if (ret_term == EXCEED_MAX_AMOUNT){
        printf("\n\t--===DECLINED EXCEED_MAX_AMOUNT===--\n");
        return;
    }
    transData.cardHolderData = cardData;
    transData.terminalData = termData;
    EN_transState_t transSt = recieveTransactionData(&transData);
    printf("--%s--\n",transData.cardHolderData.primaryAccountNumber);

    if (transSt == DECLINED_STOLEN_CARD) {
        printf("\n\t--===DECLINED STOLEN CARD/INVALID ACCOUNT===--\n");
    }
    else if (transSt== DECLINED_INSUFFECIENT_FUND) {
        printf("\n\t--===DECLINED_INSUFFECIENT_FUND===--\n");
    }
    else if (transSt == APPROVED) {
        printf("\n\t-=APPROVED\n");
        printf("\t-=UPDATING ACCOUNTS DB...\n");
        updateAccountDB();
        printf("\t-=SAVING TRANSACTION...\n");
        saveTransaction(&transData);
    }

}