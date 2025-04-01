#include "inventory.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

/**
 * @brief Constructs the InventoryPage widget.
 * @param parent Pointer to the parent QWidget (default nullptr).
 *
 * Initializes the InventoryPage instance and prepares it for UI setup.
 */
InventoryPage::InventoryPage(QWidget* parent) : QWidget(parent) {}

/**
 * @brief Sets the tab widget to enable navigation between tabs.
 * @param tabs Pointer to the QTabWidget.
 *
 * Allows the class to be able to integrate with the tabbed interface.
 */
void InventoryPage::setTabWidget(QTabWidget* tabs) {
    tabWidget = tabs;
}

/**
 * @brief Sets up the UI components for inventory management.
 *
 * Initializes the layout of the inventory and input fields for the user to interact with.
 * Also includes action buttons and integrates the shared inventory display table.
 */
void InventoryPage::setupUI() {
    // Title label for the main layout of the inventory section
    QVBoxLayout* mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    // Input fields layout
    QLabel* titleLabel = new QLabel("Inventory Management", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 21px; font-weight: 600;");
    mainLayout->addSpacing(8);
    mainLayout->addWidget(titleLabel);

    // Input field for the Item Name
    QHBoxLayout* inputLayout = new QHBoxLayout();
    itemNameInput = new QLineEdit(this);
    itemNameInput->setPlaceholderText("Enter item name");

    // Input field for the Quantity Input
    itemQuantityInput = new QLineEdit(this);
    itemQuantityInput->setPlaceholderText("Enter quantity");

    // Input field for the Price Input
    priceInput = new QLineEdit(this);
    priceInput->setPlaceholderText("Enter price (e.g. 9.99)");

    // adds the input widget layouts
    inputLayout->addWidget(itemNameInput);
    inputLayout->addWidget(itemQuantityInput);
    inputLayout->addWidget(priceInput);
    mainLayout->addLayout(inputLayout);

    // Action buttons for the operations of the inventory
    addItemButton = new QPushButton("Add Item", this);
    removeItemButton = new QPushButton("Remove Selected Item", this);
    updateItemButton = new QPushButton("Update Selected Item", this);
    backButton = new QPushButton("Back to Dashboard", this);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addItemButton);
    buttonLayout->addWidget(removeItemButton);
    buttonLayout->addWidget(updateItemButton);
    buttonLayout->addWidget(backButton);
    mainLayout->addLayout(buttonLayout);

    // Inventory table integration for the widget
    if (sharedInventoryTable) {
        sharedInventoryTable->setHorizontalHeaderLabels({"Item Name", "Quantity", "Price"});
        mainLayout->addWidget(sharedInventoryTable);
    }

    // Connecting buttons to slots
    connect(addItemButton, &QPushButton::clicked, this, &InventoryPage::addItem);
    connect(removeItemButton, &QPushButton::clicked, this, &InventoryPage::removeItem);
    connect(updateItemButton, &QPushButton::clicked, this, &InventoryPage::updateItem);
    connect(backButton, &QPushButton::clicked, [this]() {
        if (tabWidget) {
            tabWidget->setCurrentIndex(1); // Back to Dashboard
        }
    });
}

/**
 * @brief Adds a new item to the inventory table.
 *
 * Validates the user input and then updates the table accordingly.
 * Clears the input fields after successful addition, and then displays a warning if not complete.
 */
void InventoryPage::addItem() {
    qDebug() << "Add item clicked!";
    QString name = itemNameInput->text();
    QString quantity = itemQuantityInput->text();
    QString price = priceInput->text();

    // Validate the input fields
    if (name.isEmpty() || quantity.isEmpty() || price.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    if (!sharedInventoryTable) return;

    // Add new row to the inventory table
    int row = sharedInventoryTable->rowCount();
    sharedInventoryTable->insertRow(row);

    // Populate the table with the new data
    sharedInventoryTable->setItem(row, 0, new QTableWidgetItem(name));
    sharedInventoryTable->setItem(row, 1, new QTableWidgetItem(quantity));
    sharedInventoryTable->setItem(row, 2, new QTableWidgetItem("$" + price));

    // Clear the input fields after item is added
    itemNameInput->clear();
    itemQuantityInput->clear();
    priceInput->clear();

    saveInventoryData(); // Save after adding
}

/**
 * @brief Removes the selected item from the inventory table.
 *
 * Ensures that an item is selected before removal and then will alert the user with a warning if there is no selection.
 */
void InventoryPage::removeItem() {
    qDebug() << "Remove item clicked!";
    if (!sharedInventoryTable) return;
    int currentRow = sharedInventoryTable->currentRow();
    if (currentRow >= 0) {
        sharedInventoryTable->removeRow(currentRow);
        saveInventoryData(); // Save after removing
    } else {
        QMessageBox::warning(this, "Selection Error", "Please select an item to remove.");
    }
}

/**
 * @brief Updates the selected inventory item's details.
 *
 * Checks for selection validity and updates the selected item's details if inputs are provided.
 * Clears the input fields post-update and alerts if no item is selected.
 */
void InventoryPage::updateItem() {
    qDebug() << "Update item clicked!";
    if (!sharedInventoryTable) return;
    int currentRow = sharedInventoryTable->currentRow();

    if (currentRow >= 0) {
        QString name = itemNameInput->text();
        QString quantity = itemQuantityInput->text();
        QString price = priceInput->text();
        if (!name.isEmpty())
            sharedInventoryTable->setItem(currentRow, 0, new QTableWidgetItem(name));
        if (!quantity.isEmpty())
            sharedInventoryTable->setItem(currentRow, 1, new QTableWidgetItem(quantity));
        if (!price.isEmpty())
            sharedInventoryTable->setItem(currentRow, 2, new QTableWidgetItem("$" + price));

        // Clear input fields after updating
        itemNameInput->clear();
        itemQuantityInput->clear();
        priceInput->clear();

        saveInventoryData(); // Save after updating
    } else {
        QMessageBox::warning(this, "Selection Error", "Please select an item to update.");
    }
}

/**
 * @brief Sets the page stack for navigation.
 * @param stack Pointer to QStackedWidget used to manage the multiple views.
 *
 * Allows the class to navigate between different views within the application.
 */
void InventoryPage::setPageStack(QStackedWidget* stack) {
    pageStack = stack;
}

/**
 * @brief Sets the shared inventory table.
 * @param table Pointer to the QTableWidget used for displaying inventory.
 *
 * Initializes the inventory table and sets up the UI components accordingly.
 */
void InventoryPage::setInventoryTable(QTableWidget* table) {
    if (!sharedInventoryTable) {
        sharedInventoryTable = table;
        setupUI();
    }
}

/**
 * @brief Sets the current user ID for inventory data management.
 * @param id The user ID as a QString.
 */
void InventoryPage::setCurrentUserId(const QString& id) {
    currentUserId = id;
}

/**
 * @brief Saves the current inventory data to a JSON file.
 *
 * Stores the inventory data in a user-specific file in a standard location.
 */
void InventoryPage::saveInventoryData() {
    if (currentUserId.isEmpty() || !sharedInventoryTable) return;

    QJsonArray inventoryArray;
    for (int row = 0; row < sharedInventoryTable->rowCount(); ++row) {
        QJsonObject obj;
        obj["name"] = sharedInventoryTable->item(row, 0)->text();
        obj["quantity"] = sharedInventoryTable->item(row, 1)->text();
        obj["price"] = sharedInventoryTable->item(row, 2)->text();
        inventoryArray.append(obj);
    }

    QJsonObject root;
    root["inventory"] = inventoryArray;

    // Use a consistent location across computers
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                      "/BusinessManagementSystem/data";
    
    QDir dir;
    if (!dir.exists(dataPath)) {
        dir.mkpath(dataPath);
    }

    QString filePath = dataPath + "/" + currentUserId + "_inventory.json";
    QFile file(filePath);
    
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
        qDebug() << "Inventory saved to:" << filePath;
    } else {
        qDebug() << "Failed to save inventory:" << file.errorString();
    }
}

/**
 * @brief Loads inventory data from a JSON file for the specified user.
 * @param userId The ID of the user whose inventory data should be loaded.
 */
void InventoryPage::loadInventoryData(const QString& userId) {
    currentUserId = userId;
    
    if (!sharedInventoryTable) {
        qDebug() << "sharedInventoryTable is null!";
        return;
    }

    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                      "/BusinessManagementSystem/data";
    QString filePath = dataPath + "/" + userId + "_inventory.json";
    QFile file(filePath);

    if (!file.exists()) {
        qDebug() << "No inventory file found for user:" << userId;
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open inventory file:" << file.errorString();
        return;
    }

    sharedInventoryTable->setRowCount(0);
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray inventoryArray = doc["inventory"].toArray();
    for (const QJsonValue& val : inventoryArray) {
        QJsonObject obj = val.toObject();
        int row = sharedInventoryTable->rowCount();
        sharedInventoryTable->insertRow(row);
        sharedInventoryTable->setItem(row, 0, new QTableWidgetItem(obj["name"].toString()));
        sharedInventoryTable->setItem(row, 1, new QTableWidgetItem(obj["quantity"].toString()));
        sharedInventoryTable->setItem(row, 2, new QTableWidgetItem(obj["price"].toString()));
    }
    qDebug() << "Loaded" << inventoryArray.size() << "inventory items";
}

/**
 * @brief Destructor to clean up allocated resources.
 *
 * Ensures all dynamically allocated components are cleaned up to avoid memory leaks.
 */
InventoryPage::~InventoryPage() {}
