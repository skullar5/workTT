#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QMessageBox>
#include <QHeaderView>
#include <QDialog>
#include <QFormLayout>
#include <QFileDialog>
#include <QDateTime>
#include <QTextStream>
#include <QStackedWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QGroupBox>
#include <QInputDialog>
#include <QTextEdit>
#include "loginpage.h"
#include "authenticatesystem.h"
#include "financialreport.h"
#include "cashflowtracking.h"
#include "budgetpage.h"
#include "dashboard.h"
#include "inventory.h"
#include "mainwindow.h"
#include "invoicemanager.h"
#include <QTextBrowser>
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDebug>

/**
 * @brief Constructor for MainWindow.
 *
 * Initializes the main application window, sets up the UI components, and connects signals
 * and slots for navigation and functionality. It also configures the tab widget and integrates
 * various pages such as Dashboard, Financials, Budget, and Cashflow Tracking.
 *
 * @param parent The parent widget, default is nullptr.
 *
 * @author Simran Kullar
  *         Abbygail Latimer
  *         Kareena Sen
  *         Maneet Chahal
  *         Katherine R
 */
MainWindow::MainWindow(AuthenticateSystem* authSystem, QWidget* parent)
    : QMainWindow(parent), authSystem(authSystem) {
    setWindowTitle("Business Management System"); // Set window title and size
    resize(900, 650);

    QWidget* centralWidget = new QWidget(this); // Create a central widget for the main window
    setCentralWidget(centralWidget); // Set the central widget for the main window

    QTabWidget* tabWidget = new QTabWidget(centralWidget); // Create a tab widget for navigation
    QTableWidget* sharedInventoryTable = new QTableWidget(0, 3); // Create a shared inventory table with 0 rows and 3 columns

    dashboardPage = new Dashboard();  // Create the dashboard widget
    QWidget* financialsPage = new QWidget(); // Create the financials page widget
    BudgetPage* budgetPage = new BudgetPage(); // Create the budget page widget
    QWidget* cashflowTrackingPage = new QWidget(); // Create the cashflow tracking page widget
    InvoiceManager* billingPage = new InvoiceManager();

    QWidget* guidancePage = new QWidget();
    QVBoxLayout* guidanceLayout = new QVBoxLayout(guidancePage);

    dashboardPage->setCurrentUserId(currentUserId); // assuming it was set before launching MainWindow
    dashboardPage->loadMonthlyRevenueData(currentUserId);


    dashboardPage->setInventoryTable(sharedInventoryTable); // Set the shared inventory table for the dashboard


    inventoryPage = new InventoryPage(); // Create the inventory page widget
    inventoryPage->setTabWidget(tabWidget); // Set the tab widget for the inventory page
    inventoryPage->setInventoryTable(sharedInventoryTable); // Set the shared inventory table for the inventory page
    //inventoryPage->setCurrentUserId(currentUserId);
    //inventoryPage->loadInventoryData(currentUserId);

    // Let Dashboard switch tabs
    dashboardPage->setTabWidget(tabWidget); // Allow the dashboard to control tab navigation

    tabWidget->setCurrentIndex(0); // Set the default tab to the first one (Dashboard)
    tabWidget->setMovable(false); // Disable the ability to move tabs
    tabWidget->setTabsClosable(false); // Disable the ability to close tabs

    // Add tabs to the tab widget with appropriate labels
    // Step 2: Add to tab widget
    tabWidget->addTab(guidancePage, "Getting Started");
    tabWidget->addTab(dashboardPage, "Dashboard Overview"); // Add the Dashboard tab
    tabWidget->addTab(financialsPage, "Financials"); // Add the Financials tab
    tabWidget->addTab(budgetPage, "Budget Planner"); // Add the Budget tab
    tabWidget->addTab(cashflowTrackingPage, "Cashflow Tracking"); // Add the Cashflow Tracking tab
    tabWidget->addTab(inventoryPage, "Inventory"); // Add the Inventory tab
    tabWidget->addTab(billingPage, "Billing and Invoice");

    // Create a vertical layout for the main window
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(tabWidget); // Add the tab widget to the layout
    centralWidget->setLayout(layout); // Set the layout for the central widget


    dashboardPage->setTabWidget(tabWidget);  // Pass the tab widget to the dashboard for navigation control

    // Billing & Invoice Tab

    /*QVBoxLayout* billingLayout = new QVBoxLayout(billingPage); // Main vertical layout

    QLabel* titleBillingLabel = new QLabel("Billing and Invoice Management", this);
    titleBillingLabel->setAlignment(Qt::AlignCenter);
    titleBillingLabel->setStyleSheet("font-size: 21px; font-weight: 600;");
    billingLayout->addSpacing(8);
    billingLayout->addWidget(titleBillingLabel);*/

    // Financials tab
    QVBoxLayout* financialsLayout = new QVBoxLayout(financialsPage); // Main vertical layout

    // Title
    QLabel* financialsLabel = new QLabel("Financial Reports", financialsPage);
    financialsLabel->setAlignment(Qt::AlignCenter);
    financialsLabel->setStyleSheet("font-size: 21px; font-weight: 600;");
    financialsLayout->addSpacing(8);
    financialsLayout->addWidget(financialsLabel);
    financialsLayout->addSpacing(25);

    // Buttons (in a row)
    QPushButton* profitLossButton = new QPushButton("Generate Profit & Loss Report", financialsPage);
    QPushButton* expenseBreakdownButton = new QPushButton("Generate Expense Breakdown", financialsPage);
    QPushButton* balanceSheetButton = new QPushButton("Generate Balance Sheet", financialsPage);

    profitLossButton->setFixedWidth(220);
    expenseBreakdownButton->setFixedWidth(220);
    balanceSheetButton->setFixedWidth(220);


    QHBoxLayout* buttonRow = new QHBoxLayout();
    buttonRow->addStretch(); // optional: push buttons to center
    buttonRow->addWidget(profitLossButton);
    buttonRow->addSpacing(20); // space between buttons
    buttonRow->addWidget(expenseBreakdownButton);
    buttonRow->addSpacing(20);
    buttonRow->addWidget(balanceSheetButton);
    buttonRow->addStretch();

    financialsLayout->addLayout(buttonRow);

    // Add spacer to push everything up
    financialsLayout->addStretch();

    financialsPage->setLayout(financialsLayout);


    // Create a button in the top-left corner
    logoutButton = new QPushButton("Logout", this);

    // Position it in the top-right corner
    int buttonWidth = 100;
    int buttonHeight = 25;
    int margin = 10;
    logoutButton->setGeometry(width() - buttonWidth - margin, margin - 5, buttonWidth, buttonHeight);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::handleLogout);


    // Connect the profit/loss button to generate and display a profit/loss report
    connect(profitLossButton, &QPushButton::clicked, this, [this]() {
        try {
            if (reportGenerator.loadCSV("business_financials.csv")) {
                reportGenerator.generateProfitLossStatement("profit_loss_statement.csv");
                QMessageBox::information(this, "Success", "Profit & Loss statement generated");
            }
            else {
                QMessageBox::warning(this, "Error", "Failed to load financial data");
            }
        }
        catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
        }
        });

    // Connect the expense breakdown button to generate and display a expense breakdown report
    connect(expenseBreakdownButton, &QPushButton::clicked, this, [this]() {
        try {
            if (reportGenerator.loadCSV("business_financials.csv")) {
                reportGenerator.generateExpenseBreakdownReport("expense_breakdown_report.csv");
                QMessageBox::information(this, "Success", "Expense breakdown report generated");
            }
            else {
                QMessageBox::warning(this, "Error", "Failed to load financial data");
            }
        }
        catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
        }
        });

    // Connect the balance sheet button to generate and display a balance sheet report
    connect(balanceSheetButton, &QPushButton::clicked, this, [this]() {
        try {
            if (reportGenerator.loadCSV("business_financials.csv")) {
                reportGenerator.generateBalanceSheetReport("balance_sheet_report.csv");
                QMessageBox::information(this, "Success", "Balance sheet report generated");
            }
            else {
                QMessageBox::warning(this, "Error", "Failed to load financial data");
            }
        }
        catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
        }
        });

    // Connect tab changes to update the budget page when the Budget tab is selected
    connect(tabWidget, &QTabWidget::currentChanged, this, [=](int index) {
        qDebug() << "Switched to tab:" << index;
        if (index == 3) {  // Budget tab
            budgetPage->update();
            budgetPage->repaint();
            budgetPage->layout()->update();
        }
        });
    // Step 1: Create the page and main layout
    //QWidget* guidancePage = new QWidget(this);
    //QVBoxLayout* guidanceLayout = new QVBoxLayout(guidancePage); // Main vertical layout

    // ─── Tutorial GroupBox ─────────────────────
    QGroupBox* tutorialBox = new QGroupBox("Tutorial");
    QVBoxLayout* tutorialLayout = new QVBoxLayout(tutorialBox);

    QPushButton* replayButton = new QPushButton("🔄 Replay");
    tutorialLayout->addWidget(replayButton);
    tutorialLayout->addStretch();

    connect(replayButton, &QPushButton::clicked, this, &MainWindow::showTutorialDialog);


    // ─── Help GroupBox (contains FAQ + User Guide) ────────────
    QGroupBox* helpBox = new QGroupBox("❓Help");
    QVBoxLayout* helpLayout = new QVBoxLayout(helpBox);

    // FAQ Section
    QGroupBox* faqBox = new QGroupBox("📠 FAQ");
    QVBoxLayout* faqLayout = new QVBoxLayout(faqBox);
    QTextEdit* faqText = new QTextEdit();
    faqText->setReadOnly(true);
    faqText->setText(
        "Q: How do I access the dashboard?\n"
        "A: Log in and click the 'Dashboard' tab in the main menu.\n\n"

        "Q: How do I generate financial reports?\n"
        "A: Go to the 'Financials' tab, choose a report type, set the date range, and click 'Generate.'\n\n"

        "Q: How do I add a new transaction?\n"
        "A: Go to the 'Cashflow Tracking' tab and click 'Add Transaction.'\n\n"

        "Q: How do I create a budget?\n"
        "A: Go to the 'Budget Planner' tab, click 'New Budget', choose a period, and set your category and amount.\n\n"

        "Q: How do I update inventory?\n"
        "A: Go to the 'Inventory' tab, select an item to edit its quantity or click 'Add Item' to add a new product.\n\n"

        "Q: How do I manage invoices?\n"
        "A: Click the 'Billing and Invoice' tab, then use 'New Invoice' to create one, or update/delete existing invoices.\n\n"

        "Q: Can I use different currencies?\n"
        "A: Yes! Choose your preferred currency during transaction entry. Real-time rates are applied automatically.\n"
    );

    faqLayout->addWidget(faqText);

    // User Guide Section
    QGroupBox* userGuideBox = new QGroupBox("📘 User Guide");
    QVBoxLayout* userGuideLayout = new QVBoxLayout();

    QString userGuideText = R"(
<b>Dashboard Overview</b><br>
<i>Introduction:</i> View a summary of financials, inventory, and monthly reports.<br>
<i>How to Use:</i><br>
• Go to the 'Dashboard' tab.<br>
• View the financial graph and inventory table.<br>
• Click a revenue cell to edit, then click 'Update Dashboard'.<br><br>

<b>Budgeting Page</b><br>
<i>Introduction:</i> Manage budgets and control overspending.<br>
<i>How to Use:</i><br>
• Click 'New Budget'.<br>
• Enter category, period, and amount.<br>
• Add expenses and receive warnings if over budget.<br><br>

<b>Financials Page</b><br>
<i>Introduction:</i> Generate financial reports with one click.<br>
<i>How to Use:</i><br>
• Select a report type.<br>
• Click 'Generate' to export a CSV report.<br><br>

<b>Inventory Page</b><br>
<i>Introduction:</i> Track product stock and get low-stock alerts.<br>
<i>How to Use:</i><br>
• Click 'Add Item'.<br>
• Update or delete items as needed.<br><br>

<b>Billing & Invoicing</b><br>
<i>Introduction:</i> Manage invoices and client payments.<br>
<i>How to Use:</i><br>
• Click 'New Invoice'.<br>
• Enter customer and amount.<br>
• Mark invoices as paid or delete if needed.<br><br>

<b>Multi-Currency Support</b><br>
<i>Introduction:</i> Convert and view values in various currencies.<br>
<i>How to Use:</i><br>
• Use the currency converter in Cashflow Tracking.<br>
• Select the currency and convert in real-time.<br>
)";

    QTextBrowser* userGuideBrowser = new QTextBrowser();
    userGuideBrowser->setHtml(userGuideText);
    userGuideBrowser->setMinimumHeight(400);  // Optional: Adjust height as needed

    userGuideLayout->addWidget(userGuideBrowser);
    userGuideBox->setLayout(userGuideLayout);


    // Add FAQ and User Guide into Help
    helpLayout->addWidget(faqBox);
    helpLayout->addWidget(userGuideBox);

    // ─── Assemble Final Layout ───────────────────
    guidanceLayout->addWidget(tutorialBox);
    guidanceLayout->addWidget(helpBox);
    guidancePage->setLayout(guidanceLayout);


    QGroupBox* externalLinksBox = new QGroupBox("🌐 External Resources");
    QVBoxLayout* externalLayout = new QVBoxLayout();

    QString externalLinksHtml = R"(
<b>💸 Cashflow Tracking:</b><br>
• <a href='https://www.investopedia.com/terms/c/cash-flow.asp'>What is Cash Flow? – Investopedia</a><br>
• <a href='https://quickbooks.intuit.com/ca/resources/cash-flow/'>Cash Flow Management – QuickBooks Canada</a><br><br>

<b>📊 Budgeting:</b><br>
• <a href='https://www.canada.ca/en/financial-consumer-agency/services/budget.html'>Government of Canada: Budgeting Tools</a><br>
• <a href='https://www.mint.com/how-to-budget'>How to Create a Budget – Mint</a><br><br>

<b>📈 Financial Reports:</b><br>
• <a href='https://www.investopedia.com/terms/f/financial-statements.asp'>Types of Financial Reports – Investopedia</a><br>
• <a href='https://corporatefinanceinstitute.com/resources/accounting/balance-sheet/'>Understanding Balance Sheets – CFI</a><br><br>

<b>🧾 Billing & Invoicing:</b><br>
• <a href='https://quickbooks.intuit.com/r/invoicing/what-is-an-invoice'>What is an Invoice? – QuickBooks</a><br>
• <a href='https://www.freshbooks.com/en-ca/invoice-templates'>Free Invoice Templates – FreshBooks</a><br><br>

<b>💱 Transactions & Currency:</b><br>
• <a href='https://www.oanda.com/currency-converter/'>Live Currency Converter – OANDA</a><br>
• <a href='https://www.xe.com/currencycharts/'>Currency Charts & History – XE</a><br>
)";

    QTextBrowser* externalBrowser = new QTextBrowser();
    externalBrowser->setOpenExternalLinks(true);
    externalBrowser->setHtml(externalLinksHtml);
    externalBrowser->setMinimumHeight(300);

    externalLayout->addWidget(externalBrowser);
    externalLinksBox->setLayout(externalLayout);


    // Add to help layout
    helpLayout->addWidget(externalLinksBox);

    // Cashflow Tracking tab
    //create cashflow tracking tab and table to store all transactions
    QVBoxLayout* cashflowLayout = new QVBoxLayout(cashflowTrackingPage);

    //two buttons that run the addtransaction and searchtransaction functions and export button for exportinh
    QPushButton* addButton = new QPushButton("Add Transaction");
    QPushButton* searchButton = new QPushButton("Search Transaction");
    QPushButton* exportButton = new QPushButton("Generate Report");
    QPushButton* deleteTransactionButton = new QPushButton("Delete Transaction", this);

    QLabel* titleLabel = new QLabel("Cashflow Tracking", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 21px; font-weight: 600;");
    cashflowLayout->addSpacing(8);
    cashflowLayout->addWidget(titleLabel);

    tableWidget = new QTableWidget(cashflowTrackingPage);
    tableWidget->setColumnCount(8);
    tableWidget->setHorizontalHeaderLabels({ "ID", "Date/Time", "Seller", "Buyer", "Merchandise", "Cost", "Currency", "Category" });
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    // transaction input fields
    dateTimeEdit = new QLineEdit();
    sellerEdit = new QLineEdit();
    buyerEdit = new QLineEdit();
    merchandiseEdit = new QLineEdit();
    costEdit = new QLineEdit();
    currencyEdit = new QLineEdit();
    categoryEdit = new QLineEdit();
    searchEdit = new QLineEdit();

    //input widgets and layout
    // Create a horizontal layout for buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(exportButton);
    buttonLayout->addWidget(deleteTransactionButton);

    // Add the button layout to the main vertical layout
    cashflowLayout->addLayout(buttonLayout);

    cashflowLayout->addWidget(tableWidget);
    cashflowTrackingPage->setLayout(cashflowLayout);

    //connect buttons
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addTransaction);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchTransaction);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::generateReport);
    connect(deleteTransactionButton, &QPushButton::clicked, this, [this]() {
        bool ok;
        int transID = QInputDialog::getInt(this, "Delete Transaction", "Enter Transaction ID:", 1, 1, 10000, 1, &ok);
        if (ok) {
            if (cashflowTracking.searchDataEntries(transID)) {
                cashflowTracking.deleteData(transID);
                QMessageBox::information(this, "Success", "Transaction deleted successfully.");
                updateTransactionTable(); // Refresh the table
            }
            else {
                QMessageBox::warning(this, "Error", "Transaction ID not found.");
            }
        }
        });



    //connect(qApp, &QCoreApplication::aboutToQuit, inventoryPage, &InventoryPage::saveInventoryData);

}

/**
 * @brief Destructor for MainWindow.
 *
 * Cleans up resources used by the MainWindow instance. Qt automatically handles the deletion
 * of child widgets, so no manual cleanup is required.
 *
 * @author Simran Kullar
 */
MainWindow::~MainWindow() {
    // Nothing needs to be deleted manually because Qt handles child QObjects
}

void MainWindow::handleLogout() {
    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Logout", "Are you sure you want to logout?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (confirm == QMessageBox::Yes) {
        this->hide();  // 👈 don't close MainWindow

        LoginPage* loginPage = new LoginPage(authSystem, this); // 👈 pass THIS main window back
        loginPage->show();
    }
}

void MainWindow::handleTableEdit(int row, int column) {
    if (row < 0 || row >= tableWidget->rowCount()) return;

    // Clear previous data and rebuild from table
    cashflowTracking.clear();  // You’ll need to add this method (see below)

    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        int id = tableWidget->item(i, 0)->text().toInt();
        QString dateTime = tableWidget->item(i, 1)->text();
        QString seller = tableWidget->item(i, 2)->text();
        QString buyer = tableWidget->item(i, 3)->text();
        QString merchandise = tableWidget->item(i, 4)->text();
        QString costStr = tableWidget->item(i, 5)->text();
        QString currency = tableWidget->item(i, 6)->text();
        QString category = tableWidget->item(i, 7)->text();

        costStr.remove('$');
        costStr.remove(',');
        double cost = costStr.toDouble();

        cashflowTracking.addData(dateTime.toStdString(), seller.toStdString(), buyer.toStdString(),
            merchandise.toStdString(), cost, currency.toStdString(), category.toStdString());

    }

    saveTransactions();  // persist updates
}


void MainWindow::showTutorialDialog() {
    QDialog* tutorialDialog = new QDialog(this);
    tutorialDialog->setWindowTitle("Welcome Tutorial");
    tutorialDialog->resize(650, 500);

    QVBoxLayout* layout = new QVBoxLayout(tutorialDialog);

    QString tutorialText = R"(
<pre>
R"(Business Management System Tutorial

Dashboard Overview
Introduction
The dashboard provides a well-organized summary of financials, budgeting, and inventory data. It allows users to gain quick insights into their business operations.
How to Use
1. Accessing the Dashboard
    * Log in to your account.
    * Navigate to the "Dashboard" tab in the main menu.
2. Key Features
    * Financial Overview: Displays cash inflows and outflows.
    * Inventory Status: Provides table to itemized inventory entries
    * Monthly Reports: View generated reports for a selected month in the form of a bar graph or line chart that shows revenue projection.
3. Dynamic Updates
    * Transactions and inventory changes reflect in real time.
    * Click the table to change revenue inputs for each month and deselect the table and hit “Update Dashboard” to view all the updates in bar graph or line chart form.

Budgeting Page
Introduction
The budgeting page allows users to create and manage budgets for different expense categories.
How to Use
1. Creating a Budget
    * Click on "New Budget."
    * Choose budgeting period.
    * Enter a category name and budget amount.
    * Save the entry.
2. Managing Budgets
    * Click on an existing budget to update or delete it.
    * Add expenses under each category.
    * A warning will appear if an expense exceeds the set budget.

Financials Page
Introduction
The financials page enables users to generate financial reports such as profit and loss statements and balance sheets.
How to Use
1. Generating Reports
    * Select a report type (Profit & Loss, Expense Breakdown, Balance Sheet).
    * Set the date range and click "Generate."

Inventory Page
Introduction
The inventory page helps track and manage stock levels, product categories, and low-stock alerts.
How to Use
1. Adding Inventory Items
    * Click "Add Item."
    * Enter product details (name, quantity, category).
    * Save the item.
2. Managing Inventory
    * Update stock levels by selecting an item and editing its quantity.
    * Remove an item when it is no longer needed.

Billing and Invoicing Management
Introduction
The billing module allows users to create, manage, and store invoices while tracking payments.
How to Use
1. Creating Invoices
    * Click "New Invoice."
    * Enter customer details, amount, and due date.
    * Save the invoice.
2. Managing Invoices
    * Update existing invoices when needed.
    * Mark invoices as paid to update their status.
    * Delete invoices that are no longer required.

Extra Available Features:
Multi-Currency Support
Introduction
The system allows users to conduct transactions in multiple currencies with real-time exchange rates.
How to Use
1. Selecting a Currency
    * Choose your preferred currency before making a transaction.
2. Real-Time Exchange Rates
    * The system automatically updates rates periodically. </pre>
)";

    QLabel* label = new QLabel(tutorialText);
    label->setTextFormat(Qt::RichText);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignTop);

    QScrollArea* scroll = new QScrollArea();
    scroll->setWidget(label);
    scroll->setWidgetResizable(true);
    layout->addWidget(scroll);

    QPushButton* continueBtn = new QPushButton("Continue");
    layout->addWidget(continueBtn);
    connect(continueBtn, &QPushButton::clicked, tutorialDialog, &QDialog::accept);

    tutorialDialog->exec();
}


/**
 * @brief Adds a new transaction to the system.
 *
 * Opens a dialog where users can enter transaction details. If all required fields are filled,
 * the transaction is stored in CashflowTracking and the transaction table is updated.
 *
 * @author Abbygail Latimer
 */
void MainWindow::addTransaction() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add Transaction");
    QFormLayout form(&dialog);

    QLineEdit dateTimeEdit, sellerEdit, buyerEdit, merchandiseEdit, costEdit, categoryEdit;
    QComboBox* currencyBox = new QComboBox();
    currencyBox->addItems({ "USD", "CAD", "EUR", "GBP", "AUD" });

    form.addRow("Enter date and time (yyyy-mm-dd, hh:mm):", &dateTimeEdit);
    form.addRow("Enter seller:", &sellerEdit);
    form.addRow("Enter buyer:", &buyerEdit);
    form.addRow("Enter merchandise:", &merchandiseEdit);

    QHBoxLayout* costLayout = new QHBoxLayout();
    costEdit.setPlaceholderText("Enter amount");
    costLayout->addWidget(&costEdit);
    costLayout->addWidget(currencyBox);
    QWidget* costWidget = new QWidget();
    costWidget->setLayout(costLayout);
    form.addRow("Enter cost:", costWidget);

    form.addRow("Enter category:", &categoryEdit);

    QGroupBox converterBox("Currency Converter");
    QVBoxLayout* converterLayout = new QVBoxLayout();
    QHBoxLayout* conversionLayout = new QHBoxLayout();
    QLineEdit amountEdit;
    QComboBox fromCurrency, toCurrency;
    QLabel resultLabel("Converted Amount: ");

    fromCurrency.addItems({ "USD", "CAD", "EUR", "GBP", "AUD" });
    toCurrency.addItems({ "USD", "CAD", "EUR", "GBP", "AUD" });

    QPushButton* convertButton = new QPushButton("Convert");
    conversionLayout->addWidget(&amountEdit);
    conversionLayout->addWidget(&fromCurrency);
    conversionLayout->addWidget(&toCurrency);
    conversionLayout->addWidget(convertButton);

    converterLayout->addLayout(conversionLayout);
    converterLayout->addWidget(&resultLabel);
    converterBox.setLayout(converterLayout);
    form.addRow(&converterBox);

    connect(convertButton, &QPushButton::clicked, [&]() {
        double amount = amountEdit.text().toDouble();
        if (amount <= 0) {
            QMessageBox::warning(&dialog, "Conversion Error", "Enter a valid amount.");
            return;
        }
        QString from = fromCurrency.currentText();
        QString to = toCurrency.currentText();
        double convertedAmount = currencyConvert(from, to, amount);
        resultLabel.setText("Converted Amount: " + QString::number(convertedAmount, 'f', 2) + " " + to);
        });

    QPushButton* submitButton = new QPushButton("Add", &dialog);
    form.addRow(submitButton);

    connect(submitButton, &QPushButton::clicked, [&]() {
        QString dateTime = dateTimeEdit.text().trimmed();
        QString seller = sellerEdit.text().trimmed();
        QString buyer = buyerEdit.text().trimmed();
        QString merchandise = merchandiseEdit.text().trimmed();
        QString currency = currencyBox->currentText();
        QString category = categoryEdit.text().trimmed();
        QString costStr = costEdit.text().trimmed();

        bool ok;
        double cost = costStr.toDouble(&ok);

        if (!ok || cost <= 0 || dateTime.isEmpty() || seller.isEmpty() || buyer.isEmpty() || merchandise.isEmpty() || category.isEmpty()) {
            QMessageBox::warning(&dialog, "Input Error", "All fields must be filled with valid data.");
            return;
        }

        QRegularExpression regex("^\\d{4}-\\d{2}-\\d{2}, \\d{2}:\\d{2}$");
        if (!regex.match(dateTime).hasMatch()) {
            QMessageBox::warning(&dialog, "Date and Time Format Error", "Your date and time must be in the format 'yyyy-mm-dd, hh:mm'.");
            return;
        }

        cashflowTracking.addData(
            dateTime.toStdString(),
            seller.toStdString(),
            buyer.toStdString(),
            merchandise.toStdString(),
            cost,
            currency.toStdString(),
            category.toStdString()
        );

        updateTransactionTable();
        dialog.accept();
        });

    dialog.exec();
}


/**
 * @brief Updates the transaction table with the latest data.
 *
 * Retrieves all transactions from CashflowTracking and populates the table widget
 * with the transaction details.
 *
 * @author Abbygail Latimer
 */
void MainWindow::updateTransactionTable() {
    // Populate the transaction table with data from CashflowTracking
    tableWidget->setRowCount(0); // Clear the table by setting the row count to 0

    // Retrieve all data entries from the CashflowTracking object
    std::vector<DataEntry> transactions = cashflowTracking.getAllDataEntries();

    // Iterate through each transaction entry
    for (const auto& entry : transactions) {
        // Get the current row count and insert a new row at the end
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);

        // Populate the table with data from the current entry
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(entry.transid))); // Transaction ID
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(entry.dateTime))); // Date and Time
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(entry.seller))); // Seller
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(entry.buyer))); // Buyer
        tableWidget->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(entry.merchandise))); // Merchandise
        tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(entry.cost, 'f', 2))); // Cost (formatted to 2 decimal places)
        tableWidget->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(entry.currency)));
        tableWidget->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(entry.category))); // Category
    }
    saveTransactions();

}

double MainWindow::currencyConvert(QString from, QString to, double amount) {
    double rate = 1.0;

    if (from == to) {
        rate = 1.0;
    }
    else if (from == "USD") {
        if (to == "CAD") rate = 1.43;
        else if (to == "EUR") rate = 0.92;
        else if (to == "GBP") rate = 0.77;
        else if (to == "AUD") rate = 1.59;
    }
    else if (from == "CAD") {
        if (to == "USD") rate = 0.7;
        else if (to == "EUR") rate = 0.65;
        else if (to == "GBP") rate = 0.54;
        else if (to == "AUD") rate = 1.11;
    }
    else if (from == "EUR") {
        if (to == "USD") rate = 1.08;
        else if (to == "CAD") rate = 1.55;
        else if (to == "GBP") rate = 0.84;
        else if (to == "AUD") rate = 1.72;
    }
    else if (from == "GBP") {
        if (to == "USD") rate = 1.29;
        else if (to == "CAD") rate = 1.85;
        else if (to == "EUR") rate = 1.2;
        else if (to == "AUD") rate = 2.06;
    }
    else if (from == "AUD") {
        if (to == "USD") rate = 0.63;
        else if (to == "CAD") rate = 0.9;
        else if (to == "EUR") rate = 0.58;
        else if (to == "GBP") rate = 0.49;
    }

    return amount * rate;
}
void MainWindow::setCurrentUserId(const QString& id) {
    currentUserId = id;

    dashboardPage->setCurrentUserId(id);
    dashboardPage->loadMonthlyRevenueData(id);

    inventoryPage->setCurrentUserId(id);    // Set the user ID for inventory
    inventoryPage->loadInventoryData(id);   // Load inventory data for this user

    loadTransactions(id);  // Load transactions
}
/**
 * @brief Searches for a transaction by ID.
 *
 * Opens a dialog where users can input a transaction ID to search for a specific transaction.
 * If found, the transaction details are displayed in a message box.
 *
 * @author Abbygail Latimer
 */
void MainWindow::searchTransaction() {
    // Create a QDialog window with 'this' as its parent
    QDialog dialog(this);
    dialog.setWindowTitle("Search Transaction"); // Set the title of the dialog window

    // Create a form layout to organize the widgets in the dialog
    QFormLayout form(&dialog);

    // Create a QLineEdit widget for entering the transaction ID
    QLineEdit searchEdit;
    form.addRow("Transaction ID:", &searchEdit); // Add the QLineEdit to the form with a label

    // Create a QPushButton for initiating the search
    QPushButton searchButton("Search", &dialog);
    form.addRow(&searchButton); // Add the search button to the form

    // Connect the search button's clicked signal to a lambda function
    connect(&searchButton, &QPushButton::clicked, [&]() {
        // Retrieve the text from the QLineEdit and convert it to an integer (transaction ID)
        int searchID = searchEdit.text().toInt();

        // Search for the transaction in the CashflowTracking object using the entered ID
        DataEntry* transaction = cashflowTracking.searchDataEntries(searchID);

        // Check if the transaction was found
        if (!transaction) {
            // If not found, show a warning message box
            QMessageBox::warning(&dialog, "Search Error", "Transaction not found!");
        }
        else {
            // If found, format the transaction details into a QString
            QString details = QString("ID: %1\nDate: %2\nSeller: %3\nBuyer: %4\nMerchandise: %5\nCost: %6\nCategory: %7")
                .arg(transaction->transid) // Transaction ID
                .arg(QString::fromStdString(transaction->dateTime)) // Date and Time
                .arg(QString::fromStdString(transaction->seller)) // Seller
                .arg(QString::fromStdString(transaction->buyer)) // Buyer
                .arg(QString::fromStdString(transaction->merchandise)) // Merchandise
                .arg(transaction->cost, 0, 'f', 2) // Cost (formatted to 2 decimal places)
                .arg(QString::fromStdString(transaction->category)); // Category

            // Show the transaction details in an information message box
            QMessageBox::information(&dialog, "Transaction Found", details);
        }

        // Close the dialog after the search is complete
        dialog.accept();
        });

    // Display the dialog and wait for user interaction
    dialog.exec();
}

/**
* @brief Generates a financial report for cashflow tracking.
*
* Allows users to select a date range and category, then exports transactions within that range
* to a CSV file. The exported file contains details such as ID, date/time, seller, buyer,
* merchandise, cost, and category.
*
* @author Abbygail Latimer
*/
void MainWindow::generateReport() {
    // Create a QDialog window with 'this' as its parent
    QDialog dialog(this);
    dialog.setWindowTitle("Generate Report"); // Set the title of the dialog window

    // Create a form layout to organize the widgets in the dialog
    QFormLayout form(&dialog);

    // Create QLineEdit widgets for start date, end date, and category filter
    QLineEdit startDateEdit, endDateEdit, categoryEdit;

    // Set placeholder text for the input fields
    startDateEdit.setPlaceholderText("YYYY-MM-DD"); // Hint for start date format
    endDateEdit.setPlaceholderText("YYYY-MM-DD");   // Hint for end date format
    categoryEdit.setPlaceholderText("(Optional)");  // Hint that category is optional

    // Add the input fields to the form with labels
    form.addRow("Start Date:", &startDateEdit);
    form.addRow("End Date:", &endDateEdit);
    form.addRow("Category:", &categoryEdit);

    // Create a QPushButton for exporting the report
    QPushButton exportButton("Export", &dialog);
    form.addRow(&exportButton); // Add the export button to the form

    // Connect the export button's clicked signal to a lambda function
    connect(&exportButton, &QPushButton::clicked, [&]() {
        // Retrieve and trim the input values
        QString startDate = startDateEdit.text().trimmed(); // Start date input
        QString endDate = endDateEdit.text().trimmed();     // End date input
        QString categoryFilter = categoryEdit.text().trimmed().toLower(); // Category filter (optional)

        // Validate that both date fields are filled
        if (startDate.isEmpty() || endDate.isEmpty()) {
            QMessageBox::warning(&dialog, "Input Error", "Both date fields must be filled!");
            return; // Exit the function if validation fails
        }

        // Parse the start and end dates from the input strings
        QDate start = QDate::fromString(startDate, "yyyy-MM-dd");
        QDate end = QDate::fromString(endDate, "yyyy-MM-dd");

        // Validate the parsed dates
        if (!start.isValid() || !end.isValid() || start > end) {
            QMessageBox::warning(&dialog, "Date Error", "Invalid date range!");
            return; // Exit the function if the date range is invalid
        }

        // Open a file dialog to let the user choose where to save the CSV file
        QString filePath = QFileDialog::getSaveFileName(this, "Save CSV", "", "CSV Files (*.csv)");
        if (filePath.isEmpty()) return; // Exit if the user cancels the file dialog

        // Open the selected file for writing
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(&dialog, "File Error", "Unable to open file for writing.");
            return; // Exit if the file cannot be opened
        }

        // Create a QTextStream to write to the file
        QTextStream out(&file);
        out << "ID,Date/Time,Seller,Buyer,Merchandise,Cost,Category\n"; // Write the CSV header

        // Retrieve all transactions from the CashflowTracking object
        std::vector<DataEntry> transactions = cashflowTracking.getAllDataEntries();

        // Iterate through each transaction
        for (const auto& entry : transactions) {
            // Extract the date from the transaction's dateTime string
            QDate entryDate = QDate::fromString(QString::fromStdString(entry.dateTime).split(",")[0], "yyyy-MM-dd");
            // Convert the transaction's category to lowercase for comparison
            QString entryCategory = QString::fromStdString(entry.category).toLower();

            // Check if the transaction falls within the specified date range
            bool inDateRange = entryDate.isValid() && entryDate >= start && entryDate <= end;
            // Check if the transaction matches the category filter (if provided)
            bool categoryMatches = categoryFilter.isEmpty() || entryCategory == categoryFilter;

            // If both conditions are met, write the transaction to the CSV file
            if (inDateRange && categoryMatches) {
                out << entry.transid << ","
                    << QString::fromStdString(entry.dateTime) << ","
                    << QString::fromStdString(entry.seller) << ","
                    << QString::fromStdString(entry.buyer) << ","
                    << QString::fromStdString(entry.merchandise) << ","
                    << entry.cost << ","
                    << QString::fromStdString(entry.category) << "\n";
            }
        }

        // Close the file after writing
        file.close();

        // Show a success message
        QMessageBox::information(&dialog, "Success", "CSV file saved successfully!");

        // Close the dialog
        dialog.accept();
        });

    // Display the dialog and wait for user interaction
    dialog.exec();
}


void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event); // Call base class implementation

    int buttonWidth = 100; // Button width
    int buttonHeight = 26; // Button height
    int margin = 10;       // Margin from the top and right

    // Reposition the button dynamically when resizing
    logoutButton->setGeometry(width() - buttonWidth - margin, margin - 7, buttonWidth, buttonHeight);
}

void MainWindow::saveTransactions() {
    if (currentUserId.isEmpty()) return;

    // Create JSON data
    QJsonArray transactionsArray;
    std::vector<DataEntry> transactions = cashflowTracking.getAllDataEntries();
    for (const auto& entry : transactions) {
        QJsonObject obj;
        obj["id"] = QString::number(entry.transid);
        obj["datetime"] = QString::fromStdString(entry.dateTime);
        obj["seller"] = QString::fromStdString(entry.seller);
        obj["buyer"] = QString::fromStdString(entry.buyer);
        obj["merchandise"] = QString::fromStdString(entry.merchandise);
        obj["cost"] = QString::number(entry.cost, 'f', 2);
        obj["currency"] = QString::fromStdString(entry.currency);
        obj["category"] = QString::fromStdString(entry.category);
        transactionsArray.append(obj);
    }

    QJsonObject root;
    root["transactions"] = transactionsArray;

    // Use a consistent location across computers - AppDataLocation is platform-specific
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/BusinessManagementSystem/data";

    // Ensure data directory exists
    QDir dir;
    if (!dir.exists(dataPath)) {
        bool created = dir.mkpath(dataPath);
        if (!created) {
            qDebug() << "Failed to create data directory at:" << dataPath;
            return;
        }
    }

    // Save the file
    QString filePath = dataPath + "/" + currentUserId + "_transactions.json";
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
        qDebug() << "Successfully saved" << transactions.size() << "transactions to:" << filePath;
    }
    else {
        qDebug() << "Failed to save transactions. Cannot open file:" << filePath;
        qDebug() << "Error:" << file.errorString();
    }
}

void MainWindow::loadTransactions(const QString& userId) {
    // Use the same location as in saveTransactions
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/BusinessManagementSystem/data";

    QString filePath = dataPath + "/" + userId + "_transactions.json";
    QFile file(filePath);

    qDebug() << "Attempting to load transactions from:" << filePath;

    if (!file.exists()) {
        qDebug() << "Transaction file does not exist for user:" << userId;
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open transaction file:" << file.errorString();
        return;
    }

    // Clear existing data first to prevent duplication
    tableWidget->setRowCount(0);
    cashflowTracking.clear();

    QByteArray fileData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(fileData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error:" << parseError.errorString();
        return;
    }

    QJsonArray transactionsArray = doc["transactions"].toArray();
    qDebug() << "Loaded" << transactionsArray.size() << "transactions";

    for (const QJsonValue& val : transactionsArray) {
        QJsonObject obj = val.toObject();
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(obj["id"].toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(obj["datetime"].toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(obj["seller"].toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(obj["buyer"].toString()));
        tableWidget->setItem(row, 4, new QTableWidgetItem(obj["merchandise"].toString()));
        tableWidget->setItem(row, 5, new QTableWidgetItem(obj["cost"].toString()));
        tableWidget->setItem(row, 6, new QTableWidgetItem(obj["currency"].toString()));
        tableWidget->setItem(row, 7, new QTableWidgetItem(obj["category"].toString()));

        // Add to the CashflowTracking object
        cashflowTracking.addData(
            obj["datetime"].toString().toStdString(),
            obj["seller"].toString().toStdString(),
            obj["buyer"].toString().toStdString(),
            obj["merchandise"].toString().toStdString(),
            obj["cost"].toString().toDouble(),
            obj["currency"].toString().toStdString(),
            obj["category"].toString().toStdString()
        );
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    saveTransactions();         // Save transaction data
    inventoryPage->saveInventoryData();  // Save inventory data
    QMainWindow::closeEvent(event);
}




/**
 * @brief Entry point of the application.
 *
 * This function initializes the Qt application, creates an instance of the authentication system,
 * the main window, and the login page. It then displays the login page and starts the application
 * event loop.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int The exit code of the application.
 *
 * @author Simran Kullar
 */
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    AuthenticateSystem authSystem;
    authSystem.loadUsersFromFile();

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&authSystem]() {
        authSystem.saveUsersToFile();
    });

    MainWindow mainWindow(&authSystem);
    LoginPage loginPage(&authSystem, &mainWindow);

    loginPage.show();

    return app.exec();
}
