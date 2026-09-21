#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>

// Base class
class Account {
protected:
    std::string accountNumber;
    double balance;
    std::string accountTypeName;
    
public:
    Account(const std::string& accNum, double initialBalance)
        : accountNumber(accNum), balance(initialBalance) {}
    
    virtual ~Account() {}
    
    virtual double getMinimumBalance() const = 0;
    virtual double getInterestRate() const = 0;
    virtual double getWithdrawalLimit() const = 0;
    virtual std::string getAccountTypeName() const = 0;
    
    double getBalance() const { return balance; }
    std::string getAccountNumber() const { return accountNumber; }
    
    bool deposit(double amount) {
        if (amount <= 0) return false;
        balance += amount;
        return true;
    }
    
    virtual bool withdraw(double amount) {
        if (amount <= 0) return false;
        if (balance - amount < getMinimumBalance()) return false;
        balance -= amount;
        return true;
    }
    
    virtual void applyInterest() {
        balance += balance * getInterestRate();
    }
    
    void display() const {
        std::cout << "Account: " << accountNumber
                  << " | Type: " << getAccountTypeName()
                  << " | Balance: R" << balance << std::endl;
    }
};

// Savings Account
class SavingsAccount : public Account {
public:
    SavingsAccount(const std::string& accNum, double initialBalance)
        : Account(accNum, initialBalance) {}
    
    double getMinimumBalance() const override { return 100.0; }
    double getInterestRate() const override { return 0.05; }  // 5%
    double getWithdrawalLimit() const override { return 5000.0; }
    std::string getAccountTypeName() const override { return "Savings"; }
};

// Cheque Account
class ChequeAccount : public Account {
public:
    ChequeAccount(const std::string& accNum, double initialBalance)
        : Account(accNum, initialBalance) {}
    
    double getMinimumBalance() const override { return 500.0; }
    double getInterestRate() const override { return 0.02; }  // 2%
    double getWithdrawalLimit() const override { return 10000.0; }
    std::string getAccountTypeName() const override { return "Cheque"; }
};

// Fixed Deposit Account
class FixedDepositAccount : public Account {
public:
    FixedDepositAccount(const std::string& accNum, double initialBalance)
        : Account(accNum, initialBalance) {}
    
    double getMinimumBalance() const override { return 1000.0; }
    double getInterestRate() const override { return 0.08; }  // 8%
    double getWithdrawalLimit() const override { return 0.0; } // No withdrawals
    std::string getAccountTypeName() const override { return "Fixed Deposit"; }
    
    bool withdraw(double amount) override {
        return false; // Fixed deposits cannot be withdrawn
    }
};

// Student Account
class StudentAccount : public Account {
public:
    StudentAccount(const std::string& accNum, double initialBalance)
        : Account(accNum, initialBalance) {}
    
    double getMinimumBalance() const override { return 0.0; }
    double getInterestRate() const override { return 0.03; }  // 3%
    double getWithdrawalLimit() const override { return 2000.0; }
    std::string getAccountTypeName() const override { return "Student"; }
};

#endif