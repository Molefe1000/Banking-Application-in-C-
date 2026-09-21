#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <filesystem>

namespace fs = std::filesystem;

FileManager::FileManager(const std::string& directory)
    : dataDir(directory)
{
    customersFile    = dataDir + "/customers.dat";
    transactionsFile = dataDir + "/transactions.dat";
    branchesFile     = dataDir + "/branches.dat";
    tellersFile      = dataDir + "/tellers.dat";
    configFile       = dataDir + "/system_config.txt";

    ensureDataDirectoryExists();
}

void FileManager::ensureDataDirectoryExists() const {
    if (!fs::exists(dataDir)) {
        fs::create_directory(dataDir);
    }
}

// ---- Customers ----
std::vector<CustomerRecord> FileManager::loadCustomers() const {
    std::vector<CustomerRecord> customers;
    std::ifstream in(customersFile, std::ios::binary);
    if (!in) return customers;

    CustomerRecord rec;
    while (in.read(reinterpret_cast<char*>(&rec), sizeof(CustomerRecord))) {
        customers.push_back(rec);
    }
    return customers;
}

bool FileManager::saveCustomers(const std::vector<CustomerRecord>& customers) const {
    std::ofstream out(customersFile, std::ios::binary | std::ios::trunc);
    if (!out) return false;
    for (const auto& c : customers) {
        out.write(reinterpret_cast<const char*>(&c), sizeof(CustomerRecord));
    }
    return true;
}

// ---- Transactions ----
std::vector<TransactionRecord> FileManager::loadTransactions() const {
    std::vector<TransactionRecord> txns;
    std::ifstream in(transactionsFile, std::ios::binary);
    if (!in) return txns;

    TransactionRecord rec;
    while (in.read(reinterpret_cast<char*>(&rec), sizeof(TransactionRecord))) {
        txns.push_back(rec);
    }
    return txns;
}

bool FileManager::appendTransaction(const TransactionRecord& txn) const {
    std::ofstream out(transactionsFile, std::ios::binary | std::ios::app);
    if (!out) return false;
    out.write(reinterpret_cast<const char*>(&txn), sizeof(TransactionRecord));
    return true;
}

bool FileManager::saveTransactions(const std::vector<TransactionRecord>& txns) const {
    std::ofstream out(transactionsFile, std::ios::binary | std::ios::trunc);
    if (!out) return false;
    for (const auto& t : txns) {
        out.write(reinterpret_cast<const char*>(&t), sizeof(TransactionRecord));
    }
    return true;
}

// ---- Branches ----
std::vector<Branch> FileManager::loadBranches() const {
    std::vector<Branch> branches;
    std::ifstream in(branchesFile, std::ios::binary);
    if (!in) return branches;

    Branch rec;
    while (in.read(reinterpret_cast<char*>(&rec), sizeof(Branch))) {
        branches.push_back(rec);
    }
    return branches;
}

bool FileManager::saveBranches(const std::vector<Branch>& branches) const {
    std::ofstream out(branchesFile, std::ios::binary | std::ios::trunc);
    if (!out) return false;
    for (const auto& b : branches) {
        out.write(reinterpret_cast<const char*>(&b), sizeof(Branch));
    }
    return true;
}

// ---- Tellers ----
std::vector<TellerRecord> FileManager::loadTellers() const {
    std::vector<TellerRecord> tellers;
    std::ifstream in(tellersFile, std::ios::binary);
    if (!in) return tellers;

    TellerRecord rec;
    while (in.read(reinterpret_cast<char*>(&rec), sizeof(TellerRecord))) {
        tellers.push_back(rec);
    }
    return tellers;
}

bool FileManager::saveTellers(const std::vector<TellerRecord>& tellers) const {
    std::ofstream out(tellersFile, std::ios::binary | std::ios::trunc);
    if (!out) return false;
    for (const auto& t : tellers) {
        out.write(reinterpret_cast<const char*>(&t), sizeof(TellerRecord));
    }
    return true;
}

// ---- Config ----
bool FileManager::writeDefaultConfigIfMissing() const {
    std::ifstream check(configFile);
    if (check.good()) return true;

    std::ofstream out(configFile);
    if (!out) return false;

    out << "# Banking System Configuration\n";
    out << "bank_name=Standard Bank\n";
    out << "interest_rate_savings=5.0\n";
    out << "interest_rate_cheque=2.0\n";
    out << "interest_rate_fixed=8.0\n";
    out << "interest_rate_student=3.0\n";
    out << "overdraft_limit=1000.00\n";
    out << "min_balance_savings=100.00\n";
    out << "min_balance_cheque=500.00\n";
    out << "backup_folder=data/backup\n";
    out << "currency=ZAR\n";
    return true;
}

std::string FileManager::readConfigValue(const std::string& key, const std::string& fallback) const {
    std::ifstream in(configFile);
    if (!in) return fallback;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string k = line.substr(0, pos);
        std::string v = line.substr(pos + 1);
        k.erase(k.find_last_not_of(" \t\r\n") + 1);
        v.erase(0, v.find_first_not_of(" \t\r\n"));

        if (k == key) return v;
    }
    return fallback;
}

// ---- Backup / Recovery ----
bool FileManager::backupAllData(const std::string& backupFolder) const {
    if (!fs::exists(backupFolder)) {
        fs::create_directories(backupFolder);
    }

    std::vector<std::string> files = {
        customersFile, transactionsFile, branchesFile, tellersFile, configFile
    };

    for (const auto& file : files) {
        std::ifstream src(file, std::ios::binary);
        if (!src) continue;

        std::size_t pos = file.find_last_of("/\\");
        std::string filename = (pos == std::string::npos) ? file : file.substr(pos + 1);
        std::string dest = backupFolder + "/" + filename;

        std::ofstream dst(dest, std::ios::binary | std::ios::trunc);
        if (!dst) return false;
        dst << src.rdbuf();
    }
    return true;
}

bool FileManager::restoreFromBackup(const std::string& backupFolder) const {
    std::vector<std::string> files = {
        customersFile, transactionsFile, branchesFile, tellersFile, configFile
    };

    for (const auto& file : files) {
        std::size_t pos = file.find_last_of("/\\");
        std::string filename = (pos == std::string::npos) ? file : file.substr(pos + 1);
        std::string src = backupFolder + "/" + filename;

        std::ifstream in(src, std::ios::binary);
        if (!in) continue;

        std::ofstream out(file, std::ios::binary | std::ios::trunc);
        if (!out) return false;
        out << in.rdbuf();
    }
    return true;
}

// ---- Export ----
bool FileManager::exportCustomersToCSV(const std::vector<CustomerRecord>& customers,
                                        const std::string& outFile) const {
    std::ofstream out(outFile);
    if (!out) return false;

    out << "AccountNumber,FirstName,LastName,IDNumber,Contact,Email,AccountType,Balance,BranchCode\n";
    for (const auto& c : customers) {
        out << c.accountNumber << ","
            << c.firstName << ","
            << c.lastName << ","
            << c.idNumber << ","
            << c.contactNumber << ","
            << c.email << ","
            << c.accountType << ","
            << c.balance << ","
            << c.branchCode << "\n";
    }
    return true;
}

bool FileManager::exportTransactionsToCSV(const std::vector<TransactionRecord>& txns,
                                           const std::string& outFile) const {
    std::ofstream out(outFile);
    if (!out) return false;

    out << "TransactionID,AccountNumber,Type,Amount,BalanceAfter,Timestamp,Description\n";
    for (const auto& t : txns) {
        out << t.transactionId << ","
            << t.accountNumber << ","
            << t.transactionType << ","
            << t.amount << ","
            << t.balanceAfter << ","
            << t.timestamp << ","
            << t.description << "\n";
    }
    return true;
}

bool FileManager::exportReportToText(const std::string& reportText,
                                      const std::string& outFile) const {
    std::ofstream out(outFile);
    if (!out) return false;
    out << reportText;
    return true;
}