#include "BankSystem.h"
#include "Encryption.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <cstring>

// ============================================================================
// CONSTRUCTOR / DESTRUCTOR
// ============================================================================

BankSystem::BankSystem() : currentTeller(nullptr), currentCustomer(nullptr) {
    fileManager.writeDefaultConfigIfMissing();
    initializeDefaultData();
    loadAllData();
}

BankSystem::~BankSystem() {
    saveAllData();
}

// ============================================================================
// INITIALIZATION
// ============================================================================

void BankSystem::initializeDefaultData() {
    // The creation of 3 Bank Branches (Bank Code, Name, Address and Branch Contact)
    branches = fileManager.loadBranches();
    if (branches.empty()) {
        Branch b1;
        std::strcpy(b1.code, "BR001");
        std::strcpy(b1.name, "Johannesburg Main");
        std::strcpy(b1.address, "123 Main Street, Johannesburg");
        std::strcpy(b1.phone, "0111234567");
        branches.push_back(b1);

        Branch b2;
        std::strcpy(b2.code, "BR002");
        std::strcpy(b2.name, "Cape Town Central");
        std::strcpy(b2.address, "456 Long Street, Cape Town");
        std::strcpy(b2.phone, "0211234567");
        branches.push_back(b2);

        Branch b3;
        std::strcpy(b3.code, "BR003");
        std::strcpy(b3.name, "Durban Beachfront");
        std::strcpy(b3.address, "789 Marine Drive, Durban");
        std::strcpy(b3.phone, "0311234567");
        branches.push_back(b3);

        fileManager.saveBranches(branches);
    }

    // Creation of tellers working at the 3 bank branches
    tellers = fileManager.loadTellers();
    if (tellers.empty()) {
        TellerRecord t1;
        std::strcpy(t1.tellerID, "T001");
        std::strcpy(t1.fullName, "Alice Johnson");
        std::string hashed = Encryption::hashPassword("pass123");
        std::strcpy(t1.encryptedPassword, hashed.c_str());
        std::strcpy(t1.branchCode, "BR001");
        t1.isActive = true;
        tellers.push_back(t1);

        TellerRecord t2;
        std::strcpy(t2.tellerID, "T002");
        std::strcpy(t2.fullName, "Bob Smith");
        hashed = Encryption::hashPassword("pass123");
        std::strcpy(t2.encryptedPassword, hashed.c_str());
        std::strcpy(t2.branchCode, "BR002");
        t2.isActive = true;
        tellers.push_back(t2);

        TellerRecord t3;
        std::strcpy(t3.tellerID, "T003");
        std::strcpy(t3.fullName, "Carol Williams");
        hashed = Encryption::hashPassword("pass123");
        std::strcpy(t3.encryptedPassword, hashed.c_str());
        std::strcpy(t3.branchCode, "BR003");
        t3.isActive = true;
        tellers.push_back(t3);

        fileManager.saveTellers(tellers);
    }
}

void BankSystem::loadAllData() {
    customers = fileManager.loadCustomers();
    transactions = fileManager.loadTransactions();
}

void BankSystem::saveAllData() {
    fileManager.saveCustomers(customers);
    fileManager.saveTransactions(transactions);
    fileManager.saveBranches(branches);
    fileManager.saveTellers(tellers);
}

// ============================================================================
// HELPERS
// ============================================================================

std::string BankSystem::generateAccountNumber(const std::string& branchCode) {
    int count = 1;
    for (const auto& c : customers) {
        if (std::string(c.branchCode) == branchCode) count++;
    }
    std::ostringstream oss;
    oss << "ACC-" << branchCode << "-" << std::setw(5) << std::setfill('0') << count;
    return oss.str();
}

std::string BankSystem::generatePIN() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int pin = 10000 + std::rand() % 90000;
    return std::to_string(pin);
}

std::string BankSystem::getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[30];
    std::strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

std::string BankSystem::generateTransactionId() {
    std::time_t now = std::time(nullptr);
    std::ostringstream oss;
    oss << "TXN" << now << std::rand() % 1000;
    return oss.str();
}

int BankSystem::findCustomerIndex(const std::string& accountNumber) {
    for (size_t i = 0; i < customers.size(); ++i) {
        if (std::string(customers[i].accountNumber) == accountNumber) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int BankSystem::findTellerIndex(const std::string& tellerID) {
    for (size_t i = 0; i < tellers.size(); ++i) {
        if (std::string(tellers[i].tellerID) == tellerID) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool BankSystem::isValidSAID(const std::string& id) {
    if (id.length() != 13) return false;
    for (char c : id) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

bool BankSystem::isValidEmail(const std::string& email) {
    std::size_t at = email.find('@');
    if (at == std::string::npos || at == 0) return false;
    std::size_t dot = email.find('.', at);
    return dot != std::string::npos && dot > at + 1;
}

bool BankSystem::isValidPhone(const std::string& phone) {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

bool BankSystem::isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '/' || date[5] != '/') return false;
    for (int i : {0,1,3,4,6,7,8,9}) {
        if (!std::isdigit(date[i])) return false;
    }
    return true;
}

// ===========================================================================
// CREATION OF BANKING MENUS
// ===========================================================================

// ============================================================================
// RUN - MAIN MENU
// ============================================================================

void BankSystem::run() {
    while (true) {
        showMainMenu();
    }
}

void BankSystem::showMainMenu() {
    std::cout << "\n============================================\n";
    std::cout << "  STANDARD BANK - MULTI-BRANCH SYSTEM\n";
    std::cout << "============================================\n";
    std::cout << "1. Teller Login\n";
    std::cout << "2. Customer Login\n";
    std::cout << "3. View All Branches\n";
    std::cout << "4. Exit\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
        case 1:
            if (tellerLogin()) showTellerMenu();
            break;
        case 2:
            if (customerLogin()) showCustomerMenu();
            break;
        case 3:
            viewAllBranches();
            break;
        case 4:
            std::cout << "Thank you for using Standard Bank System.\n";
            exit(0);
        default:
            std::cout << "Invalid choice. Try again.\n";
    }
}

// ============================================================================
// TELLER LOGIN & MENU
// ============================================================================

bool BankSystem::tellerLogin() {
    std::string tellerID, password;
    std::cout << "\n--- TELLER LOGIN ---\n";
    std::cout << "Teller ID: ";
    std::getline(std::cin, tellerID);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    int idx = findTellerIndex(tellerID);
    if (idx == -1) {
        std::cout << "Invalid Teller ID.\n";
        return false;
    }

    std::string hashed = Encryption::hashPassword(password);
    if (hashed != tellers[idx].encryptedPassword) {
        std::cout << "Invalid password.\n";
        return false;
    }

    currentTeller = &tellers[idx];
    std::cout << "Welcome, " << tellers[idx].fullName << "!\n";
    return true;
}

void BankSystem::showTellerMenu() {
    while (currentTeller) {
        std::cout << "\n============================================\n";
        std::cout << "  TELLER MENU - " << currentTeller->fullName
                  << " (" << currentTeller->branchCode << ")\n";
        std::cout << "============================================\n";
        std::cout << "1. Register New Customer\n";
        std::cout << "2. View Customer Details\n";
        std::cout << "3. Process Transaction\n";
        std::cout << "4. Search Customers\n";
        std::cout << "5. Apply Interest to All Accounts\n";
        std::cout << "6. Generate Branch Report\n";
        std::cout << "7. Export Customers to CSV\n";
        std::cout << "8. Backup Data\n";
        std::cout << "9. Logout\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: registerNewCustomer(); break;
            case 2: viewCustomerDetails(); break;
            case 3: processTellerTransaction(); break;
            case 4: searchCustomers(); break;
            case 5: applyInterestToAll(); break;
            case 6: generateBranchReport(); break;
            case 7: fileManager.exportCustomersToCSV(customers, "customers_export.csv");
                    std::cout << "Exported to customers_export.csv\n"; break;
            case 8: if (fileManager.backupAllData()) std::cout << "Backup complete.\n"; break;
            case 9: currentTeller = nullptr; return;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

// ============================================================================
// REGISTER NEW CUSTOMER
// ============================================================================

void BankSystem::registerNewCustomer() {
    CustomerRecord rec;

    std::cout << "\n--- NEW CUSTOMER REGISTRATION ---\n";

    std::cout << "First Name: ";
    std::cin.getline(rec.firstName, 50);
    std::cout << "Surname: ";
    std::cin.getline(rec.lastName, 50);

    // SA ID
    while (true) {
        std::cout << "SA ID Number (13 digits): ";
        std::cin.getline(rec.idNumber, 15);
        if (isValidSAID(rec.idNumber)) break;
        std::cout << "Invalid. Must be 13 digits.\n";
    }

    // Phone
    while (true) {
        std::cout << "Contact Number (10 digits): ";
        std::cin.getline(rec.contactNumber, 15);
        if (isValidPhone(rec.contactNumber)) break;
        std::cout << "Invalid. Must be 10 digits.\n";
    }

    // Email
    while (true) {
        std::cout << "Email: ";
        std::cin.getline(rec.email, 100);
        if (isValidEmail(rec.email)) break;
        std::cout << "Invalid email. Must contain @ and domain.\n";
    }

    std::cout << "Physical Address: ";
    std::cin.getline(rec.address, 150);

    // DOB
    while (true) {
        std::cout << "Date of Birth (DD/MM/YYYY): ";
        std::cin.getline(rec.dateOfBirth, 15);
        if (isValidDate(rec.dateOfBirth)) break;
        std::cout << "Invalid date format.\n";
    }

    // Account Type
    std::cout << "Account Type (0=Savings, 1=Cheque, 2=Fixed Deposit, 3=Student): ";
    std::cin >> rec.accountType;
    std::cin.ignore();

    // Minimum balance
    double minBal = 0;
    switch (rec.accountType) {
        case SAVINGS: minBal = 100.0; break;
        case CHEQUE: minBal = 500.0; break;
        case FIXED_DEPOSIT: minBal = 1000.0; break;
        case STUDENT: minBal = 0.0; break;
    }

    while (true) {
        std::cout << "Initial Deposit (min R" << minBal << "): R";
        std::cin >> rec.balance;
        std::cin.ignore();
        if (rec.balance >= minBal) break;
        std::cout << "Amount too low.\n";
    }

    // Auto-generate account number
    std::string accNum = generateAccountNumber(currentTeller->branchCode);
    std::strcpy(rec.accountNumber, accNum.c_str());
    std::strcpy(rec.branchCode, currentTeller->branchCode);
    std::strcpy(rec.createdDate, getCurrentTimestamp().c_str());
    rec.isActive = true;

    // Auto-generate PIN
    std::string pin = generatePIN();
    std::string hashedPin = Encryption::hashPassword(pin);
    std::strcpy(rec.encryptedPIN, hashedPin.c_str());

    customers.push_back(rec);
    fileManager.saveCustomers(customers);

    std::cout << "\n============================================\n";
    std::cout << "  ACCOUNT CREATED SUCCESSFULLY!\n";
    std::cout << "============================================\n";
    std::cout << "Account Number: " << rec.accountNumber << "\n";
    std::cout << "Customer: " << rec.firstName << " " << rec.lastName << "\n";
    std::cout << "PIN (SHOW ONCE): " << pin << "\n";
    std::cout << "Balance: R" << rec.balance << "\n";
    std::cout << "Branch: " << rec.branchCode << "\n";
    std::cout << "============================================\n";
}

// ============================================================================
// CUSTOMER LOGIN
// ============================================================================

bool BankSystem::customerLogin() {
    std::string accNum, pin;
    std::cout << "\n--- CUSTOMER LOGIN ---\n";
    std::cout << "Account Number: ";
    std::getline(std::cin, accNum);
    std::cout << "PIN: ";
    std::getline(std::cin, pin);

    int idx = findCustomerIndex(accNum);
    if (idx == -1) {
        std::cout << "Account not found.\n";
        return false;
    }

    if (customers[idx].isLocked) {
        std::cout << "Account is locked. Contact your branch.\n";
        return false;
    }

    std::string hashed = Encryption::hashPassword(pin);
    if (hashed != customers[idx].encryptedPIN) {
        customers[idx].failedLoginAttempts++;
        if (customers[idx].failedLoginAttempts >= 3) {
            customers[idx].isLocked = true;
            std::cout << "Too many failed attempts. Account locked.\n";
        } else {
            std::cout << "Invalid PIN. Attempts left: "
                      << (3 - customers[idx].failedLoginAttempts) << "\n";
        }
        fileManager.saveCustomers(customers);
        return false;
    }

    customers[idx].failedLoginAttempts = 0;
    currentCustomer = &customers[idx];
    std::cout << "Welcome, " << customers[idx].firstName << "!\n";
    return true;
}

// ============================================================================
// CUSTOMER MENU
// ============================================================================

void BankSystem::showCustomerMenu() {
    while (currentCustomer) {
        std::cout << "\n============================================\n";
        std::cout << "  CUSTOMER MENU - " << currentCustomer->firstName
                  << " " << currentCustomer->lastName << "\n";
        std::cout << "============================================\n";
        std::cout << "1. View Balance\n";
        std::cout << "2. Deposit\n";
        std::cout << "3. Withdraw\n";
        std::cout << "4. Transfer\n";
        std::cout << "5. View Statement\n";
        std::cout << "6. Change PIN\n";
        std::cout << "7. Logout\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: viewBalance(); break;
            case 2: case 3: case 4: performTransaction(); break;
            case 5: viewStatement(); break;
            case 6: changePIN(); break;
            case 7: currentCustomer = nullptr; return;
            default: std::cout << "Invalid.\n";
        }
    }
}

void BankSystem::viewBalance() {
    int idx = findCustomerIndex(currentCustomer->accountNumber);
    if (idx != -1) {
        std::cout << "\nBalance: R" << std::fixed << std::setprecision(2)
                  << customers[idx].balance << "\n";
    }
}

void BankSystem::performTransaction() {
    int idx = findCustomerIndex(currentCustomer->accountNumber);
    if (idx == -1) return;

    int sub;
    std::cout << "\n1. Deposit\n2. Withdraw\n3. Transfer\nChoice: ";
    std::cin >> sub;
    std::cin.ignore();

    double amount;
    std::cout << "Amount: R";
    std::cin >> amount;
    std::cin.ignore();

    try {
        if (amount <= 0) throw std::runtime_error("Amount must be positive.");

        TransactionRecord txn;
        std::strcpy(txn.transactionId, generateTransactionId().c_str());
        std::strcpy(txn.accountNumber, customers[idx].accountNumber);
        std::strcpy(txn.timestamp, getCurrentTimestamp().c_str());
        txn.amount = amount;
        txn.isAssisted = false;

        if (sub == 1) { // Deposit
            customers[idx].balance += amount;
            std::strcpy(txn.transactionType, "DEPOSIT");
        } else if (sub == 2) { // Withdraw
            if (customers[idx].balance - amount < 100.0) {
                throw std::runtime_error("Insufficient funds (min balance R100).");
            }
            customers[idx].balance -= amount;
            std::strcpy(txn.transactionType, "WITHDRAWAL");
        } else if (sub == 3) { // Transfer
            std::string target;
            std::cout << "Target Account: ";
            std::getline(std::cin, target);
            int tIdx = findCustomerIndex(target);
            if (tIdx == -1) throw std::runtime_error("Target account not found.");
            if (customers[idx].balance - amount < 100.0) {
                throw std::runtime_error("Insufficient funds.");
            }
            customers[idx].balance -= amount;
            customers[tIdx].balance += amount;
            std::strcpy(txn.transactionType, "TRANSFER");
        }

        txn.balanceAfter = customers[idx].balance;
        transactions.push_back(txn);
        fileManager.saveCustomers(customers);
        fileManager.appendTransaction(txn);

        std::cout << "Transaction successful. New balance: R"
                  << customers[idx].balance << "\n";

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << "\n";
    }
}

void BankSystem::viewStatement() {
    std::cout << "\n--- STATEMENT for "
              << currentCustomer->accountNumber << " ---\n";
    bool found = false;
    for (const auto& t : transactions) {
        if (std::string(t.accountNumber) == currentCustomer->accountNumber) {
            std::cout << t.timestamp << " | " << t.transactionType
                      << " | R" << t.amount
                      << " | Balance: R" << t.balanceAfter << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No transactions found.\n";
}

void BankSystem::changePIN() {
    std::string oldPin, newPin;
    std::cout << "Current PIN: ";
    std::getline(std::cin, oldPin);

    std::string hashedOld = Encryption::hashPassword(oldPin);
    if (hashedOld != currentCustomer->encryptedPIN) {
        std::cout << "Incorrect current PIN.\n";
        return;
    }

    std::cout << "New PIN (5 digits): ";
    std::getline(std::cin, newPin);
    if (newPin.length() != 5) {
        std::cout << "PIN must be 5 digits.\n";
        return;
    }

    int idx = findCustomerIndex(currentCustomer->accountNumber);
    std::string hashedNew = Encryption::hashPassword(newPin);
    std::strcpy(customers[idx].encryptedPIN, hashedNew.c_str());
    fileManager.saveCustomers(customers);
    std::cout << "PIN changed successfully.\n";
}

// ============================================================================
// BRANCH FUNCTIONS
// ============================================================================

void BankSystem::viewAllBranches() {
    std::cout << "\n--- ALL BRANCHES ---\n";
    for (const auto& b : branches) {
        std::cout << b.code << " | " << b.name
                  << " | " << b.address
                  << " | Tel: " << b.phone << "\n";
    }
}

void BankSystem::viewBranchDetails() {
    std::string code;
    std::cout << "Branch Code: ";
    std::getline(std::cin, code);

    for (const auto& b : branches) {
        if (std::string(b.code) == code) {
            int count = 0;
            double total = 0;
            for (const auto& c : customers) {
                if (std::string(c.branchCode) == code) {
                    count++;
                    total += c.balance;
                }
            }
            std::cout << "Name: " << b.name << "\n"
                      << "Address: " << b.address << "\n"
                      << "Phone: " << b.phone << "\n"
                      << "Accounts: " << count << "\n"
                      << "Total Deposits: R" << total << "\n";
            return;
        }
    }
    std::cout << "Branch not found.\n";
}

void BankSystem::interBranchComparison() {
    std::cout << "\n--- INTER-BRANCH COMPARISON ---\n";
    for (const auto& b : branches) {
        int count = 0;
        double total = 0;
        for (const auto& c : customers) {
            if (std::string(c.branchCode) == b.code) {
                count++;
                total += c.balance;
            }
        }
        std::cout << b.code << " (" << b.name << "): "
                  << count << " accounts, Total: R" << total << "\n";
    }
}

// ============================================================================
// SEARCH
// ============================================================================

void BankSystem::searchCustomers() {
    std::string query;
    std::cout << "Search (name/ID/account): ";
    std::getline(std::cin, query);

    bool found = false;
    for (const auto& c : customers) {
        std::string fullName = std::string(c.firstName) + " " + c.lastName;
        if (fullName.find(query) != std::string::npos ||
            std::string(c.idNumber).find(query) != std::string::npos ||
            std::string(c.accountNumber).find(query) != std::string::npos) {
            std::cout << c.accountNumber << " | " << c.firstName
                      << " " << c.lastName << " | R" << c.balance << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No matches.\n";
}

// ============================================================================
// INTEREST
// ============================================================================

void BankSystem::applyInterestToAll() {
    
    int count = 0;
    for (auto& c : customers) {
        if (c.isActive && c.accountType == SAVINGS) {
            c.balance += c.balance * 0.05;
            count++;
        } else if (c.accountType == CHEQUE) {
            c.balance += c.balance * 0.02;
            count++;
        } else if (c.accountType == FIXED_DEPOSIT) {
            c.balance += c.balance * 0.08;
            count++;
        } else if (c.accountType == STUDENT) {
            c.balance += c.balance * 0.03;
            count++;
        }
    }
    fileManager.saveCustomers(customers);
    std::cout << "Interest applied to " << count << " accounts.\n";
}

// ============================================================================
// REPORTS
// ============================================================================

void BankSystem::generateBranchReport() {
    std::string branch = currentTeller ? currentTeller->branchCode : "";
    std::cout << "\n--- BRANCH REPORT: " << branch << " ---\n";
    int count = 0;
    double total = 0;
    for (const auto& c : customers) {
        if (std::string(c.branchCode) == branch) {
            std::cout << c.accountNumber << " | "
                      << c.firstName << " " << c.lastName
                      << " | R" << c.balance << "\n";
            count++;
            total += c.balance;
        }
    }
    std::cout << "Total Accounts: " << count
              << " | Total Deposits: R" << total << "\n";
}

void BankSystem::viewCustomerDetails() {
    std::string accNum;
    std::cout << "Account Number: ";
    std::getline(std::cin, accNum);
    int idx = findCustomerIndex(accNum);
    if (idx == -1) {
        std::cout << "Not found.\n";
        return;
    }
    const auto& c = customers[idx];
    std::cout << "Account: " << c.accountNumber << "\n"
              << "Name: " << c.firstName << " " << c.lastName << "\n"
              << "ID: " << c.idNumber << "\n"
              << "Phone: " << c.contactNumber << "\n"
              << "Email: " << c.email << "\n"
              << "Balance: R" << c.balance << "\n"
              << "Branch: " << c.branchCode << "\n";
}

void BankSystem::processTellerTransaction() {
    std::string accNum;
    std::cout << "Customer Account Number: ";
    std::getline(std::cin, accNum);
    int idx = findCustomerIndex(accNum);
    if (idx == -1) { std::cout << "Not found.\n"; return; }

    std::string pin;
    std::cout << "Verify Customer PIN: ";
    std::getline(std::cin, pin);
    if (Encryption::hashPassword(pin) != customers[idx].encryptedPIN) {
        std::cout << "PIN verification failed.\n";
        return;
    }

    currentCustomer = &customers[idx];
    performTransaction();
    currentCustomer = nullptr;
}

void BankSystem::dailyTransactionReport() {
    std::cout << "\n--- DAILY TRANSACTION REPORT ---\n";
    for (const auto& t : transactions) {
        std::cout << t.timestamp << " | " << t.accountNumber
                  << " | " << t.transactionType
                  << " | R" << t.amount << "\n";
    }
}

void BankSystem::customerAccountSummary() {
    std::cout << "\n--- CUSTOMER ACCOUNT SUMMARY ---\n";
    for (const auto& c : customers) {
        std::cout << c.accountNumber << " | "
                  << c.firstName << " " << c.lastName
                  << " | R" << c.balance << "\n";
    }
}

void BankSystem::branchPerformanceReport() {
    interBranchComparison();
}