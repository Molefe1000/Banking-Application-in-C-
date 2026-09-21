#ifndef BRANCH_H
#define BRANCH_H

#include <string>

struct Branch {
    char code[10];          // e.g., "BR001"
    char name[50];          // e.g., "Johannesburg Main"
    char address[100];
    char phone[15];
    int totalAccounts;
    double totalDeposits;
    
    Branch() {
        code[0] = '\0';
        name[0] = '\0';
        address[0] = '\0';
        phone[0] = '\0';
        totalAccounts = 0;
        totalDeposits = 0.0;
    }
};

#endif