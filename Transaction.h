#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

struct TransactionRecord {
    char transactionId[20];
    char accountNumber[30];
    char transactionType[20];  // DEPOSIT, WITHDRAWAL, TRANSFER
    double amount;
    double balanceAfter;
    char timestamp[30];        // DD/MM/YYYY HH:MM:SS
    char description[100];
    char tellerID[10];         // If assisted
    bool isAssisted;
    
    TransactionRecord() {
        transactionId[0] = '\0';
        accountNumber[0] = '\0';
        transactionType[0] = '\0';
        amount = 0.0;
        balanceAfter = 0.0;
        timestamp[0] = '\0';
        description[0] = '\0';
        tellerID[0] = '\0';
        isAssisted = false;
    }
};

#endif