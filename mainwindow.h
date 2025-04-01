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
class InvoiceManager;  // Added missing forward declaration

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
    FinancialReportGenerator reportGenerator; // Handles financial report generation
    CashflowTracking cashflowTracking;        // Manages cashflow transactions
    QTableWidget* tableWidget;                // Displays cashflow transactions
    QLineEdit* dateTimeEdit;                  // Input field for transaction date/time
    QLineEdit* sellerEdit;                    // Input field for seller
    QLineEdit* buyerEdit;                     // Input field for buyer
    QLineEdit* merchandiseEdit;               // Input field for merchandise
    QLineEdit* costEdit;                      // Input field for cost
    QLineEdit* categoryEdit;                  // Input field for category
    QLineEdit* searchEdit;                    // Input field for searching transactions
    QPushButton* logoutButton;                // Logout button
    AuthenticateSystem* authSystem;           // Pointer to authentication system
    LoginPage* loginPage;                     // Pointer to login page (optional, not used in constructor)
    QString currentUserId;                    // Current user ID
    Dashboard* dashboardPage;                 // Dashboard page
    QJsonObject exchangeRates;                // Currency exchange rates (unused in current code)
    QLineEdit* currencyEdit;                  // Input field for currency
    BudgetPage* budgetPage;                   // Budget management page
    InventoryPage* inventoryPage;             // Inventory management page

    void saveTransactions();
    void loadTransactions(const QString& userId);
    void closeEvent(QCloseEvent* event);

public:
    explicit MainWindow(AuthenticateSystem* authSystem, QWidget* parent = nullptr);
    ~MainWindow();
    void setCurrentUserId(const QString& id);

private slots:
    void addTransaction();
    void updateTransactionTable();
    void searchTransaction();
    void generateReport();
    void handleLogout();
    void handleTableEdit(int row, int column);
    double currencyConvert(QString from, QString to, double amount);
    void showTutorialDialog();

protected:
    void resizeEvent(QResizeEvent* event) override;
};

#endif // MAINWINDOW_H
