#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <vector>
#include <string>
#include "Customer.h"
#include "Teller.h"
#include "Branch.h"
#include "Transaction.h"

class FileManager {
private:
    std::string dataDir;
    std::string customersFile;
    std::string transactionsFile;
    std::string branchesFile;
    std::string tellersFile;
    std::string configFile;

public:
    explicit FileManager(const std::string& directory = "data");

    // Customers
    std::vector<CustomerRecord> loadCustomers() const;
    bool saveCustomers(const std::vector<CustomerRecord>& customers) const;

    // Transactions
    std::vector<TransactionRecord> loadTransactions() const;
    bool appendTransaction(const TransactionRecord& txn) const;
    bool saveTransactions(const std::vector<TransactionRecord>& txns) const;

    // Branches
    std::vector<Branch> loadBranches() const;
    bool saveBranches(const std::vector<Branch>& branches) const;

    // Tellers
    std::vector<TellerRecord> loadTellers() const;
    bool saveTellers(const std::vector<TellerRecord>& tellers) const;

    // Config
    bool writeDefaultConfigIfMissing() const;
    std::string readConfigValue(const std::string& key, const std::string& fallback) const;

    // Backup / recovery
    bool backupAllData(const std::string& backupFolder = "data/backup") const;
    bool restoreFromBackup(const std::string& backupFolder = "data/backup") const;

    // Export
    bool exportCustomersToCSV(const std::vector<CustomerRecord>& customers, const std::string& outFile) const;
    bool exportTransactionsToCSV(const std::vector<TransactionRecord>& txns, const std::string& outFile) const;
    bool exportReportToText(const std::string& reportText, const std::string& outFile) const;

    void ensureDataDirectoryExists() const;
};

#endif