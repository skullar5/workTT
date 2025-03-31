#include "cashflowtracking.h"
#include <iostream>
#include <fstream>
#include <algorithm>

/**
 * @class cashflowtracking.cpp
 * @brief Implements the CashflowTracking class for managing financial transactions with data persistence.
 *
 * This file defines the methods for adding, searching, listing, and retrieving
 * transaction data. The class helps store transaction records and categorizes
 * them based on different attributes.
 * Enhanced with data persistence features to save data between sessions.
 *
 * @author Abbygail Latimer
 */

 /**
  * @brief Adds a new transaction record to the system.
  * @param dateTime Date and time of the transaction.
  * @param seller Name of the seller.
  * @param buyer Name of the buyer.
  * @param merchandise List of purchased merchandise.
  * @param cost Cost of the transaction.
  * @param currency Currency the cost is in
  * @param category Category to store transaction under.
  */
void CashflowTracking::addData(const std::string& dateTime, const std::string& seller,
    const std::string& buyer, const std::string& merchandise,
    double cost, const std::string& currency, const std::string& category) {
    // Create a new DataEntry object and add it to the dataEntries vector
    DataEntry data = { dataCounter++, dateTime, seller, buyer, merchandise, cost, currency, category };
    dataEntries.push_back(data);
    categories[category].push_back(data.transid); // Add the transaction ID to the category's list
}

/**
 * @brief Searches for a transaction by its ID.
 * @param transID Unique transaction ID to search for.
 * @return Pointer to the found DataEntry, or nullptr if not found.
 */
DataEntry* CashflowTracking::searchDataEntries(int transID) {
    for (auto& data : dataEntries) {
        if (data.transid == transID) { // Check if the current entry matches the transaction ID
            return &data; // Return a pointer to the matching entry
        }
    }
    return nullptr; // Return nullptr if no matching entry is found
}

/**
 * @brief Lists all stored transactions.
 */
void CashflowTracking::listDataEntries() {
    for (const auto& data : dataEntries) {
        // Print details of each transaction in the dataEntries vector
        std::cout << "ID: " << data.transid << ", Date/Time: " << data.dateTime
            << ", Buyer: " << data.buyer << ", Seller: " << data.seller
            << ", Merchandise: " << data.merchandise << ", Cost: " << data.cost
            << ", Currency: " << data.currency << ", Category: " << data.category << "\n";
    }
}

/**
 * @brief Retrieves all stored transaction records.
 * @return A vector containing all DataEntry objects.
 */
std::vector<DataEntry> CashflowTracking::getAllDataEntries() {
    return dataEntries;
}

/**
 * @brief Removes transaction from records using its ID.
 * @param transID Unique transaction ID to remove.
 */
void CashflowTracking::deleteData(int transID) {
    for (auto it = dataEntries.begin(); it != dataEntries.end(); ++it) {
        if (it->transid == transID) {
            // Remove the transaction ID from its category
            categories[it->category].erase(
                std::remove(categories[it->category].begin(), categories[it->category].end(), transID),
                categories[it->category].end()
            );
            // Remove the transaction from dataEntries
            dataEntries.erase(it);
            return;
        }
    }
}

/**
 * @brief Clears dataEntries and categories
 */
void CashflowTracking::clear() {
    dataEntries.clear();
    categories.clear();
    dataCounter = 1;
}

/**
 * @brief Saves all transaction data to a file.
 * @return True if the save operation was successful, false otherwise.
 *
 * This function serializes all transaction data to a binary file.
 * The file format is structured as follows:
 * 1. dataCounter value (to maintain ID consistency)
 * 2. Number of entries
 * 3. For each entry:
 *    - Fixed-size data (transid, cost)
 *    - String lengths followed by string contents for all string fields
 */
bool CashflowTracking::saveDataToFile() {
    std::ofstream outFile(dataFilePath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << dataFilePath << std::endl;
        return false;
    }

    // Save the current data counter to ensure ID consistency across sessions
    outFile.write(reinterpret_cast<const char*>(&dataCounter), sizeof(dataCounter));

    // Save the number of entries
    size_t numEntries = dataEntries.size();
    outFile.write(reinterpret_cast<const char*>(&numEntries), sizeof(numEntries));

    // Save each data entry
    for (const auto& entry : dataEntries) {
        // Write fixed-size members
        outFile.write(reinterpret_cast<const char*>(&entry.transid), sizeof(entry.transid));
        outFile.write(reinterpret_cast<const char*>(&entry.cost), sizeof(entry.cost));

        // Write string lengths and then the strings
        size_t len;

        // DateTime
        len = entry.dateTime.size();
        outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
        outFile.write(entry.dateTime.c_str(), len);

        // Seller
        len = entry.seller.size();
        outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
        outFile.write(entry.seller.c_str(), len);

        // Buyer
        len = entry.buyer.size();
        outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
        outFile.write(entry.buyer.c_str(), len);

        // Merchandise
        len = entry.merchandise.size();
        outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
        outFile.write(entry.merchandise.c_str(), len);

        // Currency
        len = entry.currency.size();
        outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
        outFile.write(entry.currency.c_str(), len);

        // Category
        len = entry.category.size();
        outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
        outFile.write(entry.category.c_str(), len);
    }

    outFile.close();
    return true;
}

/**
 * @brief Loads transaction data from a file.
 * @return True if the load operation was successful, false otherwise.
 *
 * This function deserializes transaction data from a binary file.
 * It reads the file according to the format written by saveDataToFile():
 * 1. dataCounter value
 * 2. Number of entries
 * 3. For each entry:
 *    - Fixed-size data (transid, cost)
 *    - String lengths followed by string contents for all string fields
 *
 * The function also rebuilds the categories map based on the loaded entries.
 */
bool CashflowTracking::loadDataFromFile() {
    std::ifstream inFile(dataFilePath, std::ios::binary);
    if (!inFile.is_open()) {
        // File might not exist yet, which is not an error for a new user
        return false;
    }

    // Clear existing data
    dataEntries.clear();
    categories.clear();

    // Read the data counter
    inFile.read(reinterpret_cast<char*>(&dataCounter), sizeof(dataCounter));

    // Read the number of entries
    size_t numEntries;
    inFile.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries));

    // Read each data entry
    for (size_t i = 0; i < numEntries; ++i) {
        DataEntry entry;

        // Read fixed-size members
        inFile.read(reinterpret_cast<char*>(&entry.transid), sizeof(entry.transid));
        inFile.read(reinterpret_cast<char*>(&entry.cost), sizeof(entry.cost));

        // Read string lengths and then the strings
        size_t len;
        std::vector<char> buffer;

        // DateTime
        inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer.resize(len);
        inFile.read(buffer.data(), len);
        entry.dateTime.assign(buffer.data(), len);

        // Seller
        inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer.resize(len);
        inFile.read(buffer.data(), len);
        entry.seller.assign(buffer.data(), len);

        // Buyer
        inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer.resize(len);
        inFile.read(buffer.data(), len);
        entry.buyer.assign(buffer.data(), len);

        // Merchandise
        inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer.resize(len);
        inFile.read(buffer.data(), len);
        entry.merchandise.assign(buffer.data(), len);

        // Currency
        inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer.resize(len);
        inFile.read(buffer.data(), len);
        entry.currency.assign(buffer.data(), len);

        // Category
        inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer.resize(len);
        inFile.read(buffer.data(), len);
        entry.category.assign(buffer.data(), len);

        // Add the entry to our collections
        dataEntries.push_back(entry);
        categories[entry.category].push_back(entry.transid);
    }

    inFile.close();
    return true;
}