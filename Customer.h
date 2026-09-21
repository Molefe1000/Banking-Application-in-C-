#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

enum AccountType {
    SAVINGS = 0,
    CHEQUE = 1,
    FIXED_DEPOSIT = 2,
    STUDENT = 3
};

struct CustomerRecord {
    char accountNumber[30];   // ACC-BR001-00001
    char firstName[50];
    char lastName[50];
    char idNumber[15];        // 13 digits SA ID
    char contactNumber[15];
    char email[100];
    char address[150];
    char dateOfBirth[15];     // DD/MM/YYYY
    int accountType;          // AccountType enum
    double balance;
    char encryptedPIN[20];    // Hashed PIN
    char branchCode[10];
    char createdDate[15];
    bool isActive;
    int failedLoginAttempts;
    bool isLocked;
    
    CustomerRecord() {
        accountNumber[0] = '\0';
        firstName[0] = '\0';
        lastName[0] = '\0';
        idNumber[0] = '\0';
        contactNumber[0] = '\0';
        email[0] = '\0';
        address[0] = '\0';
        dateOfBirth[0] = '\0';
        accountType = SAVINGS;
        balance = 0.0;
        encryptedPIN[0] = '\0';
        branchCode[0] = '\0';
        createdDate[0] = '\0';
        isActive = true;
        failedLoginAttempts = 0;
        isLocked = false;
    }
};

#endif