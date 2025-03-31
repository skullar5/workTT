#include "loginpage.h"
#include <iostream>
#include <QDir>
#include <QDialog>
#include <QTimer>
#include <QVBoxLayout>
#include <QLabel>
#include <QObject>
#include <QScrollArea>
#include <QTextEdit>
#include <QPushButton>
#include <QTabWidget>
#include "mainwindow.h"

LoginPage::LoginPage(AuthenticateSystem* auth, QMainWindow* mainWin, QWidget* parent)
    : QMainWindow(parent), authSystem(auth), mainWindow(mainWin) {
    setWindowTitle("Login - Business Management System");
    resize(800, 600);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    QLabel* titleLabel = new QLabel("Business Management System");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    QLabel* loginLabel = new QLabel("Login");
    loginLabel->setAlignment(Qt::AlignCenter);
    loginLabel->setStyleSheet("font-size: 16px; color: gray;");

    QLabel* userIcon = new QLabel();
    QPixmap userPixmap("../../../user.png");
    if (!userPixmap.isNull()) {
        userIcon->setPixmap(userPixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        userIcon->setAlignment(Qt::AlignCenter);
    }
    else {
        userIcon->setAlignment(Qt::AlignCenter);
        userIcon->setStyleSheet("font-size: 32px;");
    }

    idInput = new QLineEdit();
    idInput->setPlaceholderText("Enter your ID");
    idInput->setFixedWidth(200);

    QHBoxLayout* idRow = new QHBoxLayout();
    idRow->addStretch();
    idRow->addWidget(idInput);
    idRow->addStretch();

    loginButton = new QPushButton("Login");
    signupButton = new QPushButton("Sign Up");
    statusLabel = new QLabel("");

    loginButton->setFixedWidth(200);
    signupButton->setFixedWidth(200);

    QLabel* noteLabel = new QLabel("Note: ID must be at least 5 characters long.");
    noteLabel->setStyleSheet("color: gray; font-size: 12px;");
    noteLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(titleLabel);
    layout->addWidget(userIcon);
    layout->addWidget(loginLabel);
    layout->addLayout(idRow);
    layout->addSpacing(25);
    layout->addWidget(loginButton);
    layout->addSpacing(2);
    layout->addWidget(signupButton);
    layout->addWidget(statusLabel);
    layout->addWidget(noteLabel);

    layout->setAlignment(loginButton, Qt::AlignHCenter);
    layout->setAlignment(signupButton, Qt::AlignHCenter);

    connect(loginButton, &QPushButton::clicked, this, &LoginPage::handleLogin);
    connect(signupButton, &QPushButton::clicked, this, &LoginPage::handleSignup);
}

void LoginPage::handleLogin() {
    QString id = idInput->text().trimmed();

    if (id.isEmpty()) {
        statusLabel->setText("❌ ID is required. Please try again.");
        statusLabel->setStyleSheet("color: red; font-size: 13px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        return;
    }

    if (id.length() < 5) {
        statusLabel->setText("❌ ID must be at least 5 characters long.");
        statusLabel->setStyleSheet("color: red; font-size: 13px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        return;
    }

    std::cout << "Attempting login for user: " << id.toStdString() << std::endl;

    bool success = authSystem->authenticateUser(id, this);

    if (authSystem->isAccountLocked(id)) {
        showLockDialog();
        return;
    }

    if (success) {
        MainWindow* realMainWindow = qobject_cast<MainWindow*>(mainWindow);
        if (realMainWindow) {
            realMainWindow->setCurrentUserId(id);
            realMainWindow->show();
            this->close();

            QDialog* tutorialDialog = new QDialog();
            tutorialDialog->setWindowTitle("Welcome Tutorial");
            tutorialDialog->resize(600, 400);

            QVBoxLayout* layout = new QVBoxLayout(tutorialDialog);

            QString tutorialText = "<pre>" R"(Business Management System Tutorial

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
    * The system automatically updates rates periodically.)" "</pre>";

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

            connect(continueBtn, &QPushButton::clicked, [=]() {
                tutorialDialog->accept();
                });

            tutorialDialog->exec();
        }
    }
    else {
        if (!authSystem->userExists(id)) {
            statusLabel->setText("❌ You are not signed up. Please sign up first.");
        }
        else {
            statusLabel->setText("❌ Incorrect login. Try again.");
        }
        statusLabel->setStyleSheet("color: red; font-size: 13px;");
        statusLabel->setAlignment(Qt::AlignCenter);
    }
}

void LoginPage::handleSignup() {
    QString id = idInput->text().trimmed();

    if (id.isEmpty()) {
        statusLabel->setText("❌ ID is required. Please try again.");
        statusLabel->setStyleSheet("color: red; font-size: 13px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        return;
    }

    if (id.length() < 5) {
        statusLabel->setText("❌ ID must be at least 5 characters long.");
        statusLabel->setStyleSheet("color: red; font-size: 13px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        return;
    }

    if (authSystem->userExists(id)) {
        statusLabel->setText("⚠️ You already signed up. Please log in.");
        statusLabel->setStyleSheet("color: red; font-size: 13px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        return;
    }

    authSystem->addUser(id, "User");
    statusLabel->setText("✅ Account created successfully! Please log in.");
    statusLabel->setStyleSheet("color: green; font-size: 13px;");
    statusLabel->setAlignment(Qt::AlignCenter);
}

void LoginPage::showLockDialog() {
    QDialog* lockDialog = new QDialog(this);
    lockDialog->setWindowTitle("🔒 Account Locked");
    lockDialog->setModal(true);
    lockDialog->setWindowFlags(lockDialog->windowFlags() & ~Qt::WindowCloseButtonHint);

    QVBoxLayout* dialogLayout = new QVBoxLayout(lockDialog);
    QLabel* countdownLabel = new QLabel("🔒 Your account is temporarily locked.\n\nPlease wait 60 seconds before trying again.");
    countdownLabel->setAlignment(Qt::AlignCenter);
    dialogLayout->addWidget(countdownLabel);

    QTimer* countdownTimer = new QTimer(lockDialog);
    int* remainingTime = new int(60);

    QObject::connect(countdownTimer, &QTimer::timeout, lockDialog, [=]() mutable {
        (*remainingTime)--;
        countdownLabel->setText(QString("🚫 ACCOUNT LOCKED\n\nTime remaining: %1 seconds").arg(*remainingTime));
        if (*remainingTime <= 0) {
            countdownTimer->stop();
            delete remainingTime;
            lockDialog->accept();
        }
        });

    countdownTimer->start(1000);
    lockDialog->exec();
}
