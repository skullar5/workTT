#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QtNetwork/QNetworkReply>
#include <QPushButton>
#include <QJsonObject>
#include "authenticatesystem.h"
#include "financialreport.h"
#include "cashflowtracking.h"
#include "dashboard.h"
#include "loginpage.h"
#include "inventory.h"
#include "budgetpage.h"


class QPushButton;
class QResizeEvent;
class QLineEdit;
class QTableWidget;
class AuthenticateSystem;
class LoginPage;
class FinancialReportGenerator;
class CashflowTracking;
class Dashboard;
class BudgetPage;
class InventoryPage;

/**
 * @class main.cpp
 * @brief Main application window for the Business Management System.
 *
 * This file contains the implementation of the MainWindow class, which serves as the central
 * interface for managing financial reports, cashflow tracking, budgets, and inventory.
 * It integrates various components such as FinancialReportGenerator, CashflowTracking, and
 * InventoryPage to provide a comprehensive business management solution.
 *
 */

 /**
  * @brief Main application window for the Business Management System.
  *
  * This class manages the main UI of the application, including tabs for Dashboard, Financials,
  * Budget, Cashflow Tracking, and Inventory. It integrates various components to provide
  * functionality for adding transactions, generating reports, and managing inventory.
  *
  * @author Simran Kullar
  *         Abbygail Latimer
  *         Kareena Sen
  *         Maneet Chahal
  *         Katherine R
  */
class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    FinancialReportGenerator reportGenerator; // Handles financial report generation (e.g., profit/loss, balance sheet)
    CashflowTracking cashflowTracking; // Manages cashflow transactions and data entries
    QTableWidget* tableWidget; // Displays cashflow transactions in a table format
    QLineEdit* dateTimeEdit; // Input field for transaction date and time
    QLineEdit* sellerEdit; // Input field for the seller's name
    QLineEdit* buyerEdit; // Input field for the buyer's name
    QLineEdit* merchandiseEdit; // Input field for the merchandise description
    QLineEdit* costEdit; // Input field for the transaction cost
    QLineEdit* categoryEdit; // Input field for the transaction category
    QLineEdit* searchEdit; // Input field for searching transactions by ID
    QPushButton* logoutButton; // Logout button
    AuthenticateSystem* authSystem; // Pointer to the authentication system
    LoginPage* loginPage; // Pointer to the login page
    QString currentUserId;      // Stores the currently logged-in user's unique identifier
    Dashboard* dashboardPage;   // Pointer to the dashboard UI page/component
    QJsonObject exchangeRates;  // Stores currency exchange rate data in JSON format
    QLineEdit* currencyEdit; // Input field for the transaction currency
    BudgetPage* budgetPage;      // Pointer to the budget management page, handles financial planning and tracking
    InventoryPage* inventoryPage; // Pointer to the inventory management page, tracks product/item stock levels


    /**
    * @brief Saves all current transactions to persistent storage.
    *
    * Serializes the transaction data and writes it to either:
    * - A database
    * - A file (JSON/XML/binary)
    * - Cloud storage
    *
    * @note This function should be called whenever transactions are modified
    * @warning Will overwrite existing transaction data
    * @see loadTransactions()
    */
    void saveTransactions();

    /**
     * @brief Loads transactions for a specific user from storage.
     *
     * @param userId The unique identifier of the user whose transactions to load
     *
     * Reads and deserializes transaction data from:
     * - Database (using userId as key)
     * - Local user file
     * - Cloud storage account
     *
     * @throw std::runtime_error if loading fails
     * @note Clears existing transactions before loading
     * @see saveTransactions()
     */
    void loadTransactions(const QString& userId);

    /**
     * @brief Handles window closing events.
     *
     * Performs cleanup operations when the application window is closed:
     * - Saves unsaved transactions (if any)
     * - Releases resources
     * - Logs shutdown event
     *
     * @param event The close event containing state information
     *
     * @note Automatically called by Qt framework on window close
     * @warning Any unsaved data will be lost if not handled here
     * @see QCloseEvent, QWidget::closeEvent()
     */
    void closeEvent(QCloseEvent* event);

public:
    /**
    * @brief Constructs a new MainWindow instance.
    *
    * Initializes the main application window with required dependencies.
    *
    * @param authSystem Pointer to the authentication system component (non-null)
    * @param parent Optional parent widget (defaults to nullptr)
    *
    * @note The MainWindow takes ownership of the authSystem pointer
    * @warning authSystem must be a valid pointer
    */
    explicit MainWindow(AuthenticateSystem* authSystem, QWidget* parent = nullptr);

    /**
     * @brief Destroys the MainWindow instance.
     *
     * Performs cleanup of:
     * - UI components
     * - Memory allocations
     * - Active connections
     *
     * @note Automatically called when window is closed
     */
    ~MainWindow();

    /**
     * @brief Sets the currently active user ID.
     *
     * Updates application state to reflect the authenticated user.
     *
     * @param id The user's unique identifier string
     *
     * @note Triggers UI updates and permission checks
     * @warning Does not validate the ID - must be pre-authenticated
     * @see AuthenticateSystem::verifyUser()
     */
    void setCurrentUserId(const QString& id);


private slots:
    /**
    * @brief Adds a new transaction to the system.
    *
    * Opens a dialog where users can enter transaction details. If all required fields are filled,
    * the transaction is stored in CashflowTracking and the transaction table is updated.
    *
    * @author Abbygail Latimer
    */
    void addTransaction();
    /**
    * @brief Updates the transaction table with the latest data.
    *
    * Retrieves all transactions from CashflowTracking and populates the table widget
    * with the transaction details.
    *
    * @author Abbygail Latimer
    */
    void updateTransactionTable();
    /**
    * @brief Searches for a transaction by ID.
    *
    * Opens a dialog where users can input a transaction ID to search for a specific transaction.
    * If found, the transaction details are displayed in a message box.
    *
    * @author Abbygail Latimer
    */
    void searchTransaction();
    /**
    * @brief Generates a financial report for cashflow tracking.
    *
    * Allows users to select a date range and category, then exports transactions within that range
    * to a CSV file. The exported file contains details such as ID, date/time, seller, buyer,
    * merchandise, cost, and category.
    *
    * @author Abbygail Latimer
    */
    void generateReport();

    /**
    * @brief Handles user logout process.
    *
    * Performs the following actions:
    * - Clears current user session data
    * - Resets application state
    * - Navigates back to login screen
    * - Emits logout signal for other components
    *
    * @note Automatically saves any unsaved data before logout
    * @warning Will terminate any ongoing operations
    * @see loginSuccessful(), sessionTimeout()
    */
    void handleLogout();

    /**
     * @brief Processes table cell edit events.
     *
     * Validates and applies changes made to table cells:
     * - Verifies data type compliance
     * - Checks business rules
     * - Updates model data
     * - Triggers related calculations/updates
     *
     * @param row The 0-based row index of edited cell
     * @param column The 0-based column index of edited cell
     *
     * @throw std::invalid_argument if edit violates constraints
     * @note Emits dataChanged() signal on successful edit
     * @see beginEdit(), endEdit(), modelAboutToBeReset()
     */
    void handleTableEdit(int row, int column);

    /**
    * @brief Performs currency conversion.
    * @return amount converted to new currency
    *
    * Converts the amount entered from the "from" currency to the "to" currency and returns the result.
    *
    * @author Abbygail Latimer
    */
    double currencyConvert(QString from, QString to, double amount);
    void showTutorialDialog();
protected:
    /**
    * @brief Handles window resize events.
    *
    * This function is automatically called by Qt's event system when the window is resized.
    * It performs the following operations:
    * - Updates internal layout calculations
    * - Adjusts child widget geometries
    * - Triggers content repainting if needed
    * - Maintains aspect ratio constraints (if any)
    *
    * @param event Pointer to the QResizeEvent containing old and new size information
    *
    * @note The base class implementation should be called if overridden (QWidget::resizeEvent)
    * @warning Heavy computations here may affect resize performance
    * @see QResizeEvent, QWidget::resizeEvent()
    */
    void resizeEvent(QResizeEvent* event) override;

};
#endif // MAINWINDOW_H