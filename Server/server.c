#include "server.h"

ST_accountsDB_t accountsDB[255];
int currAccount = 0;
ST_transaction_t transactionsDB[255];
int curtransaction = 0;

EN_serverError_t initializeServer(){
    FILE *accounts;
    char line[60] = {0};
    accounts = fopen("../accounts.txt","r+");
    if(accounts==NULL){
        return DATABASE_NOT_FOUND;
    }
    while(fgets(line,60,accounts)){
        //read data to array
        ST_accountsDB_t account1;
        char *token = strtok(line, ",");

            //printf("%s\n", token);
            if(token!=NULL){
                account1.balance = atof(token);
                printf("balance: %s\n",token);
                token = strtok(NULL, ",");
            }
            if(token!=NULL){
                memcpy(account1.primaryAccountNumber,token,19);
                printf("account number: %s------%s\n",token, account1.primaryAccountNumber);
            }


        accountsDB[currAccount] = account1;
        printf("account number:%d %s------%s\n",currAccount,accountsDB[currAccount].primaryAccountNumber, account1.primaryAccountNumber);
        currAccount++;
    }
    fclose(accounts);


    return ok;
}
EN_serverError_t updateAccountDB()
{
    FILE *accounts;
    char line[60] = {0};
    accounts = fopen("../accounts.txt","w");
    if (!accounts){
        return SAVING_FAILED;
    }
    for (int i = 0; i < 255; i++) {
        if (accountsDB[i].primaryAccountNumber[0] == '\0') {
            break;
        }
        fprintf(accounts, "%f,%s,\n", accountsDB[i].balance, accountsDB[i].primaryAccountNumber);
    }
    return ok;
}

EN_transState_t recieveTransactionData(ST_transaction_t *transData){
    if(isValidAccount(&transData->cardHolderData)==DECLINED_STOLEN_CARD){
        transData->transState = DECLINED_STOLEN_CARD;
        return DECLINED_STOLEN_CARD;
    }
    if(isAmountAvailable(&transData->terminalData,&transData->cardHolderData)==DECLINED_INSUFFECIENT_FUND){
        transData->transState = DECLINED_INSUFFECIENT_FUND;
        return DECLINED_INSUFFECIENT_FUND;
    }
    transData->transState = APPROVED;
    char pan[19] = {0};
    strcpy(pan,transData->cardHolderData.primaryAccountNumber);
    for(int i=0;i<currAccount;i++){
        if(strcmp(accountsDB[i].primaryAccountNumber,pan)==0) {
            printf("in balance %f\n",accountsDB[i].balance);
            accountsDB[i].balance = accountsDB[i].balance - transData->terminalData.transAmount;
            printf("in after balance %f\n",accountsDB[i].balance);

        }
    }
    return ok;

}
EN_serverError_t isValidAccount(ST_cardData_t *cardData){

    int len = strlen(cardData->primaryAccountNumber);
    int sum = 0, second = 0;
    for (int i = len - 1; i >= 0; i--) {

        int d = cardData->primaryAccountNumber[i] - '0';
        if (second == 1)
            d = d * 2;
        sum += d / 10;
        sum += d % 10;
        second = second == 1 ? 0 : 1;
    }
    if(sum%10!=0){
        printf("Card is not luhn %s\n",cardData->primaryAccountNumber);
        return DECLINED_STOLEN_CARD;
    }


    for(int i=0;i<currAccount;i++){


        if(strcmp(accountsDB[i].primaryAccountNumber,cardData->primaryAccountNumber)==0) {
            printf("Card is not stolen\n");
            return ok;
        }
    }
    return DECLINED_STOLEN_CARD;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData,ST_cardData_t *cardData){
    char pan[19]={0};
    strcpy(pan,cardData->primaryAccountNumber);

    for(int i=0;i<currAccount;i++){
        char panSaved[19] = {0};
        strcpy(panSaved,accountsDB[i].primaryAccountNumber);
        printf("%s %f, %s %f\n",pan,termData->transAmount,panSaved, accountsDB[i].balance);
        if(strcmp(panSaved,pan)==0) {
            if(termData->transAmount>accountsDB[i].balance){
                return LOW_BALANCE;
            }
        }
    }

    return ok;
}
EN_serverError_t saveTransaction(ST_transaction_t *transData) {
    FILE *transactions;
    char line[60] = {0};
    transactions = fopen("../transactions.txt", "r+");
    if (transactions == NULL) {
        return SAVING_FAILED;
    }
    transData->transactionSequenceNumber = curtransaction + 1;
    fprintf(transactions, "Transaction Processed\n");
    fprintf(transactions, "\t-Transaction Sequence Number: %d\n\n", transData->transactionSequenceNumber);
    fprintf(transactions, "Card Holder Name: %s\n", transData->cardHolderData.cardHolderName);
    fprintf(transactions, "-PAN: %s\n", transData->cardHolderData.primaryAccountNumber);
    fprintf(transactions, "-Expiry Date: %s\n", transData->cardHolderData.cardExpirationDate);
    fprintf(transactions, "-Amount: %f\n", transData->terminalData.transAmount);
    fprintf(transactions, "-Max Amount: %f\n", transData->terminalData.maxTransAmount);
    if (transData->transState == APPROVED) {
        fprintf(transactions, "-Transaction State: APPROVED\n");
    } else if (transData->transState == DECLINED_INSUFFECIENT_FUND) {
        fprintf(transactions, "-Transaction State: DECLINED_INSUFFECIENT_FUND\n");
    } else if (transData->transState == DECLINED_STOLEN_CARD) {
        fprintf(transactions, "-Transaction State: DECLINED_STOLEN_CARD\n");
    } else {
        fprintf(transactions, "-Transaction State: UNKNOWN_REASON\n");
    }

    fclose(transactions);
    return ok;



}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData);