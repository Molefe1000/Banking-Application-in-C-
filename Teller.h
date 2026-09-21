#ifndef TELLER_H
#define TELLER_H

#include <string>

struct TellerRecord {
    char tellerID[10];       // e.g., "T001"
    char fullName[50];
    char encryptedPassword[100];
    char branchCode[10];     // Restricts to this branch
    bool isActive;
    
    TellerRecord() {
        tellerID[0] = '\0';
        fullName[0] = '\0';
        encryptedPassword[0] = '\0';
        branchCode[0] = '\0';
        isActive = true;
    }
};

#endif