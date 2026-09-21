============================================================
  STANDARD BANK - MULTI-BRANCH BANKING MANAGEMENT SYSTEM
============================================================

1. COMPILATION
--------------
Navigate to the project directory and run:

    g++ -std=c++17 -Wall -o bankingsystem main.cpp BankSystem.cpp \
        Account.cpp Encryption.cpp FileManager.cpp Transaction.cpp -lstdc++fs

2. RUNNING (from the Terminal run:)
----------
    
    .\bankingsystem.exe      (Windows)

3. DEFAULT TELLER CREDENTIALS
------------------------------
Teller ID  | Password | Branch
-----------|----------|------------------
T001       | pass123  | BR001 (Johannesburg)
T002       | pass123  | BR002 (Cape Town)
T003       | pass123  | BR003 (Durban)

4. CREATING A CUSTOMER ACCOUNT
------------------------------
1. Login as a teller (e.g., T001 / pass123).
2. Choose option 1: Register New Customer.
3. Fill in the required fields.
4. The system will display:
   - Auto-generated Account Number (e.g., ACC-BR001-00001)
   - Auto-generated 5-digit PIN (SHOWN ONLY ONCE - WRITE IT DOWN!) 
   - keep account PIN Safe for later use.
	

5. CUSTOMER LOGIN AND TRANSACTIONS
----------------------------------
1. From the main menu, choose option 2: Customer Login.
2. Enter the Account Number and the 5-digit PIN.
3. Once logged in, customers can:
   - View Balance
   - Deposit / Withdraw / Transfer
   - View Statement
   - Change PIN

6. DATA FILES
-------------
All data is stored in the /data folder:
  - customers.dat      (binary - customer accounts)
  - transactions.dat   (binary - transaction history)
  - branches.dat       (binary - branch info)
  - tellers.dat        (binary - teller credentials, hashed)
  - system_config.txt  (text  - system settings)

7. BACKUP & RECOVERY
--------------------
- Teller menu option 8 creates a backup in /data/backup.
- To restore, use the FileManager::restoreFromBackup() function.

8. EXPORT
---------
- Teller menu option 7 exports all customers to customers_export.csv.

9. SECURITY NOTES
-----------------
- All PINs and passwords are stored hashed (not plain text).
- 3 failed login attempts lock the account.
- Tellers are restricted to their assigned branch.

10. MODULES
-----------
- main.cpp            - Entry point
- BankSystem.cpp/.h   - Core business logic
- Account.cpp/.h      - Base + derived account classes (OOP inheritance)
- Encryption.cpp/.h   - Password hashing + XOR encryption
- FileManager.cpp/.h  - Binary file I/O, backup, CSV export
- Transaction.h       - Transaction record struct
- Customer.h          - Customer record struct
- Teller.h            - Teller record struct
- Branch.h            - Branch struct
============================================================