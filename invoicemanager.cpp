#include "invoicemanager.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QListWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QLabel>


/**
 * @class InvoiceManager
 * @brief Manages the creation, editing, deletion, and display of invoices in a GUI.
 *
 * This class is the interface for managing invoices. It provides form fields
 * for user input, a list for displaying saved invoices, and buttons for modifying invoice
 * entries.
 *
 * @author Maneet Chahal
 */


 /**
  * @brief Constructor for the InvoiceManager class.
  *
  * Sets up the UI layout, initializes form fields and buttons,
  * connects UI actions to slots, and loads saved invoices from file.
  *
  * @param parent The parent widget (default is nullptr).
  */
InvoiceManager::InvoiceManager(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Invoice Manager");
    resize(500, 600);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QFormLayout* formLayout = new QFormLayout();
    customerNameEdit = new QLineEdit();
    customerAddressEdit = new QLineEdit();
    amountEdit = new QLineEdit();
    dueDateEdit = new QLineEdit();
    dueDateEdit->setPlaceholderText("YYYY-MM-DD");


    formLayout->addRow("Customer Name*", customerNameEdit);
    formLayout->addRow("Customer Address*", customerAddressEdit);
    formLayout->addRow("Amount*", amountEdit);
    formLayout->addRow("Due Date*", dueDateEdit);

    QPushButton* addInvoiceBtn = new QPushButton("➕ Create Invoice");
    formLayout->addRow(addInvoiceBtn);
    mainLayout->addLayout(formLayout);

    invoiceListWidget = new QListWidget();
    mainLayout->addWidget(invoiceListWidget);

    QPushButton* markCompletedBtn = new QPushButton("✅ Mark as Completed");
    QPushButton* editInvoiceBtn = new QPushButton("✏️ Edit Invoice");
    QPushButton* deleteInvoiceBtn = new QPushButton("🗑️ Delete Invoice");

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(markCompletedBtn);
    btnLayout->addWidget(editInvoiceBtn);
    btnLayout->addWidget(deleteInvoiceBtn);
    mainLayout->addLayout(btnLayout);

    loadInvoices();

    connect(addInvoiceBtn, &QPushButton::clicked, this, &InvoiceManager::addInvoice);
    connect(deleteInvoiceBtn, &QPushButton::clicked, this, &InvoiceManager::deleteInvoice);
    connect(markCompletedBtn, &QPushButton::clicked, this, &InvoiceManager::markAsCompleted);
    connect(editInvoiceBtn, &QPushButton::clicked, this, &InvoiceManager::editInvoice);
    connect(invoiceListWidget, &QListWidget::itemDoubleClicked, this, &InvoiceManager::viewInvoiceDetails);
}

/**
 * @brief Adds a new invoice using user-provided input.
 *
 * Validates all fields, creates an Invoice object, saves it,
 * and updates the invoice list display.
 */
void InvoiceManager::addInvoice() {
    if (customerNameEdit->text().isEmpty() ||
        customerAddressEdit->text().isEmpty() ||
        amountEdit->text().isEmpty() ||
        dueDateEdit->text().isEmpty()) {

        QMessageBox::critical(this, "⚠️ Error", "All required fields must be filled.");
        return;
    }

    Invoice inv(customerNameEdit->text(), customerAddressEdit->text(),
        amountEdit->text().toDouble(),
        QDate::fromString(dueDateEdit->text(), "yyyy-MM-dd"));

    invoices.append(inv);
    saveInvoices();
    loadInvoices();
    QMessageBox::information(this, "✅ Success", "Invoice created successfully!");
}

/**
 * @brief Deletes the currently selected invoice.
 *
 * Removes it from the internal list and file, then refreshes the display.
 */
void InvoiceManager::deleteInvoice() {
    int selectedRow = invoiceListWidget->currentRow();
    if (selectedRow < 0 || selectedRow >= invoices.size()) {
        QMessageBox::warning(this, "⚠️ Error", "Please select a valid invoice to delete.");
        return;
    }
    invoices.removeAt(selectedRow);
    saveInvoices();
    loadInvoices();
    QMessageBox::information(this, "🗑️ Deleted", "Invoice deleted successfully.");
}

/**
 * @brief Marks an invoice as completed if not already done.
 *
 * Updates the status to "Completed", saves, and reloads the list.
 */
void InvoiceManager::markAsCompleted() {
    int selectedRow = invoiceListWidget->currentRow();
    if (selectedRow < 0 || selectedRow >= invoices.size()) {
        QMessageBox::warning(this, "⚠️ Error", "Please select a valid invoice to update.");
        return;
    }

    Invoice& inv = invoices[selectedRow];
    if (inv.getStatus() == "Completed") {
        QMessageBox::information(this, "ℹ️ Info", "Invoice is already marked as completed.");
        return;
    }

    inv.setStatus("Completed");
    saveInvoices();
    loadInvoices();
    QMessageBox::information(this, "✅ Updated", "Invoice marked as completed!");
}

/**
 * @brief Loads invoices from JSON file and refreshes the list widget.
 */
void InvoiceManager::loadInvoices() {
    invoiceListWidget->clear();
    invoices.clear();

    QFile file("invoices.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonArray jsonArray = QJsonDocument::fromJson(data).array();

        for (const auto& jsonVal : jsonArray)
            invoices.append(Invoice::fromJson(jsonVal.toObject()));

        file.close();
    }

    for (const auto& inv : invoices) {
        invoiceListWidget->addItem(
            QString("#%1 | %2 | $%3 | %4 | %5")
            .arg(inv.getInvoiceID())
            .arg(inv.getCustomerName())
            .arg(inv.getAmount())
            .arg(inv.getDueDate().toString("yyyy-MM-dd"))
            .arg(inv.getStatus())
        );
    }
}

/**
 * @brief Saves the current list of invoices to a local JSON file.
 */
void InvoiceManager::saveInvoices() {
    QFile file("invoices.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonArray jsonArray;
        for (const auto& inv : invoices)
            jsonArray.append(inv.toJson());

        file.write(QJsonDocument(jsonArray).toJson());
        file.close();
    }
}

/**
 * @brief Opens a dialog allowing the user to edit an invoice.
 *
 * Pre-fills the form with existing invoice data and updates it upon confirmation.
 */
void InvoiceManager::editInvoice() {
    int selectedRow = invoiceListWidget->currentRow();
    if (selectedRow < 0 || selectedRow >= invoices.size()) {
        QMessageBox::warning(this, "⚠️ Error", "Please select a valid invoice to edit.");
        return;
    }
    Invoice& inv = invoices[selectedRow];

    QDialog editDialog(this);
    editDialog.setWindowTitle("✏️ Edit Invoice");

    QFormLayout form(&editDialog);

    QLineEdit customerNameEdit;
    customerNameEdit.setText(inv.getCustomerName());

    QLineEdit customerAddressEdit;
    customerAddressEdit.setText(inv.getCustomerAddress());

    QLineEdit amountEdit;
    amountEdit.setText(QString::number(inv.getAmount()));

    QLineEdit dueDateEdit;
    dueDateEdit.setText(inv.getDueDate().toString("yyyy-MM-dd"));

    QComboBox statusCombo;
    statusCombo.addItems({ "Pending", "Completed" });
    statusCombo.setCurrentText(inv.getStatus());

    form.addRow("Customer Name*", &customerNameEdit);
    form.addRow("Customer Address*", &customerAddressEdit);
    form.addRow("Amount*", &amountEdit);
    form.addRow("Due Date*", &dueDateEdit);
    form.addRow("Status*", &statusCombo);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, [&]() {
        if (customerNameEdit.text().isEmpty() || customerAddressEdit.text().isEmpty() ||
            amountEdit.text().isEmpty() || dueDateEdit.text().isEmpty()) {
            QMessageBox::critical(&editDialog, "⚠️ Error", "All required fields must be filled.");
            return;
        }

        inv.setCustomerName(customerNameEdit.text());
        inv.setCustomerAddress(customerAddressEdit.text());
        inv.setAmount(amountEdit.text().toDouble());
        inv.setDueDate(QDate::fromString(dueDateEdit.text(), "yyyy-MM-dd"));
        inv.setStatus(statusCombo.currentText());

        saveInvoices();
        loadInvoices();
        QMessageBox::information(this, "✅ Success", "Invoice updated successfully!");
        editDialog.accept();
        });

    connect(&buttonBox, &QDialogButtonBox::rejected, &editDialog, &QDialog::reject);

    editDialog.exec();
}

/**
 * @brief Displays a read-only dialog with full invoice details.
 *
 * Triggered by double-clicking an item in the invoice list.
 * Shows all fields in a popup window.
 *
 * @param item The selected invoice item.
 */
void InvoiceManager::viewInvoiceDetails(QListWidgetItem* item) {
    int selectedRow = invoiceListWidget->row(item);
    if (selectedRow < 0 || selectedRow >= invoices.size()) return;

    const Invoice& inv = invoices[selectedRow];

    QDialog detailsDialog(this);
    detailsDialog.setWindowTitle("📄 Invoice Details");

    QFormLayout form(&detailsDialog);

    QLabel customerName(inv.getCustomerName());
    QLabel customerAddress(inv.getCustomerAddress());
    QLabel amount(QString("$%1").arg(inv.getAmount()));
    QLabel dueDate(inv.getDueDate().toString("yyyy-MM-dd"));
    QLabel status(inv.getStatus());

    form.addRow("<b>Invoice ID:</b>", new QLabel(QString::number(inv.getInvoiceID())));
    form.addRow("<b>Customer Name:</b>", &customerName);
    form.addRow("<b>Customer Address:</b>", &customerAddress);
    form.addRow("<b>Amount:</b>", &amount);
    form.addRow("<b>Due Date:</b>", &dueDate);
    form.addRow("<b>Status:</b>", &status);

    QPushButton closeButton("Close");
    form.addRow(&closeButton);
    connect(&closeButton, &QPushButton::clicked, &detailsDialog, &QDialog::accept);

    detailsDialog.exec();
}
