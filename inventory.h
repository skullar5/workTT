#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>

class InventoryPage : public QWidget {
    Q_OBJECT
public:
    explicit InventoryPage(QWidget* parent = nullptr);
    ~InventoryPage();

    void setTabWidget(QTabWidget* tabs);
    void setPageStack(QStackedWidget* stack);
    void setInventoryTable(QTableWidget* table);
    void setCurrentUserId(const QString& id);  // Add this to set user ID
    void loadInventoryData(const QString& userId);  // Make public
    void saveInventoryData();  // Make public

private:
    void setupUI();
    QTabWidget* tabWidget = nullptr;
    QStackedWidget* pageStack = nullptr;
    QTableWidget* sharedInventoryTable = nullptr;
    QLineEdit* itemNameInput;
    QLineEdit* itemQuantityInput;
    QLineEdit* priceInput;
    QPushButton* addItemButton;
    QPushButton* removeItemButton;
    QPushButton* updateItemButton;
    QPushButton* backButton;
    QString currentUserId;

private slots:
    void addItem();
    void removeItem();
    void updateItem();
};

#endif // INVENTORY_H
