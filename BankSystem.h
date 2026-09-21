#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include <vector>
#include <string>
#include <map>
#include "Customer.h"
#include "Teller.h"
#include "Branch.h"
#include "Transaction.h"
#include "Account.h"
#include "FileManager.h"

class BankSystem {
private:
    FileManager fileManager;
    std::vector<CustomerRecord> customers;
    std::vector<TellerRecord> tellers;
    std::vector<Branch> branches;
    std::vector<TransactionRecord> transactions;

    TellerRecord* currentTeller;
    CustomerRecord* currentCustomer;

    // Helpers
    std::string generateAccountNumber(const std::string& branchCode);
    std::string generatePIN();
    std::string getCurrentTimestamp();
    std::string generateTransactionId();
    int findCustomerIndex(const std::string& accountNumber);
    int findTellerIndex(const std::string& tellerID);
    bool isValidSAID(const std::string& id);
    bool isValidEmail(const std::string& email);
    bool isValidPhone(const std::string& phone);
    bool isValidDate(const std::string& date);

    // Initialization
    void initializeDefaultData();
    void loadAllData();
    void saveAllData();

    // Menus
    void showMainMenu();
    void showTellerMenu();
    void showCustomerMenu();
    void showBranchMenu();
    void showReportsMenu();

    // Teller Functions
    bool tellerLogin();
    void registerNewCustomer();
    void viewCustomerDetails();
    void processTellerTransaction();
    void generateBranchReport();

    // Customer Functions
    bool customerLogin();
    void viewBalance();
    void performTransaction();
    void viewStatement();
    void changePIN();
    void applyInterestToAll();

    // Branch Functions
    void viewAllBranches();
    void viewBranchDetails();
    void interBranchComparison();

    // Search
    void searchCustomers();

    // Reports
    void dailyTransactionReport();
    void customerAccountSummary();
    void branchPerformanceReport();

public:
    BankSystem();
    ~BankSystem();
    void run();
};

#endif