#ifndef CASHFLOWTRACKING_H
#define CASHFLOWTRACKING_H

#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>  // Required for file input/output operations

/**
 * @class cashflowtracking.h
 * @brief Defines the CashflowTracking class for managing and storing transactions.
 *
 * This file contains the declaration of the CashflowTracking class, which provides
 * functionalities for adding, searching, and listing transactions.
 * Transactions stored in vector, and categories mapped to their respective transaction IDs.
 * Added data persistence capabilities to save and load transaction data between sessions.
 *
 * @author Abbygail Latimer
 */

 /**
  * @struct DataEntry
  * @brief Represents a single transaction.
  *
  * Each DataEntry struct stores details about a transaction, including transaction ID,
  * date and time, seller, buyer, merchandise details, cost, and category.
  */
struct DataEntry {
    int transid;                // Unique transaction ID.
    std::string dateTime;       // Date and time of the transaction.
    std::string seller;         // Name of the seller.
    std::string buyer;          // Name of the buyer.
    std::string merchandise;    // Description of the merchandise involved.
    double cost;                // Cost of the transaction.
    std::string currency;       // Currency cost is in.
    std::string category;       // Category of the transaction (e.g., sales, expenses).
};

/**
 * @class CashflowTracking
 * @brief Manages transactions and tracks cash flow with data persistence.
 *
 * The class provides methods to add, search, and list transactions.
 * It maintains a record of transactions and categorizes them.
 * Added functionality to save data to a file and load it when the program restarts,
 * ensuring transaction data persists between sessions.
 */
class CashflowTracking {
private:
    std::vector<DataEntry> dataEntries;                         // Stores all transaction records
    std::unordered_map<std::string, std::vector<int>> categories; // Maps categories to transaction IDs
    int dataCounter = 1;                                        // Counter for generating unique transaction IDs
    std::string dataFilePath;                                   // Path to the data storage file

public:
    /**
     * @brief Constructor that initializes the CashflowTracking object.
     * @param filePath Path to the file where data will be stored.
     *
     * Sets up the CashflowTracking object and attempts to load any existing
     * transaction data from the specified file. If no file path is provided,
     * a default file name is used.
     */
    CashflowTracking(const std::string& filePath = "cashflow_data.dat") : dataFilePath(filePath) {
        // Attempt to load existing data when the object is created
        loadDataFromFile();
    }

    /**
     * @brief Destructor that ensures data is saved when the object is destroyed.
     *
     * Automatically saves all transaction data to the file when the CashflowTracking
     * object goes out of scope or is deleted, ensuring no data is lost.
     */
    ~CashflowTracking() {
        saveDataToFile();
    }

    /**
     * @brief Adds a new transaction record to the system.
     * @param dateTime Date and time of the transaction.
     * @param seller Name of the seller.
     * @param buyer Name of the buyer.
     * @param merchandise List of purchased merchandise.
     * @param cost Cost of the transaction.
     * @param currency Currency the cost is in.
     * @param category Category to store transaction under.
     *
     * This function assigns a unique transaction ID to each entry and stores it
     * in the dataEntries vector. It also updates the category mapping.
     */
    void addData(const std::string& dateTime, const std::string& seller,
        const std::string& buyer, const std::string& merchandise,
        double cost, const std::string& currency, const std::string& category);

    /**
     * @brief Searches for a transaction by its ID.
     * @param transID Unique transaction ID to search for.
     * @return Pointer to the found DataEntry, or nullptr if not found.
     *
     * This function iterates through the stored transactions and returns
     * a pointer to the matching transaction if found.
     */
    DataEntry* searchDataEntries(int transID);

    /**
     * @brief Lists all stored transactions.
     *
     * Iterates through the transaction records and prints details.
     */
    void listDataEntries();

    /**
     * @brief Retrieves all stored transaction records.
     * @return A vector containing all DataEntry objects.
     *
     * This function provides external access to the full transaction list
     */
    std::vector<DataEntry> getAllDataEntries();

    /**
     * @brief Removes transaction from records using its ID.
     * @param transID Unique transaction ID to remove.
     *
     * This function iterates through the stored transactions and removes requested transaction.
     */
    void deleteData(int transID);

    /**
     * @brief Clears dataEntries and categories
     *
     * This function clears all entries in dataEntries and categories
     */
    void clear();

    /**
     * @brief Saves all transaction data to a file.
     * @return True if the save operation was successful, false otherwise.
     *
     * This function writes all current transaction data to the specified file
     * in binary format to ensure data integrity and efficient storage.
     * It saves transaction IDs, all transaction details, and preserves the
     * current dataCounter value to maintain ID consistency across sessions.
     */
    bool saveDataToFile();

    /**
     * @brief Loads transaction data from a file.
     * @return True if the load operation was successful, false otherwise.
     *
     * This function reads transaction data from the specified file and
     * reconstructs the dataEntries vector and categories map. It also
     * restores the dataCounter value to ensure new transactions receive
     * the correct IDs. Returns false if the file doesn't exist or can't be read.
     */
    bool loadDataFromFile();
};

#endif