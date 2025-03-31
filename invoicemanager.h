#ifndef INVOICEMANAGER_H
#define INVOICEMANAGER_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "invoice.h"

QT_BEGIN_NAMESPACE
class QListWidget;
class QLineEdit;
QT_END_NAMESPACE

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
    class InvoiceManager : public QMainWindow {
    Q_OBJECT

    public:
        /**
         * @brief Constructor for the InvoiceManager class.
         *
         * Initializes the main window layout, sets up form elements, and connects all buttons to their
         * corresponding slots. Automatically loads previously saved invoices from if available.
         *
         * @param parent The parent widget (default is nullptr).
         */
        InvoiceManager(QWidget* parent = nullptr);

    private slots:
        /**
         * @brief Adds a new invoice using user-provided input.
         *
         * Validates required fields, constructs an Invoice object, appends it to the internal list,
         * saves to disk, and refreshes the invoice list view.
         */
        void addInvoice();

        /**
         * @brief Deletes the currently selected invoice from the list.
         *
         * If no invoice is selected, shows a warning message. After deletion,
         * updates the stored data and refreshes the display.
         */
        void deleteInvoice();

        /**
         * @brief Marks the selected invoice as completed.
         *
         * If the invoice is already marked completed, a notice is shown.
         * Otherwise, updates the status, saves, and reloads the list.
         */
        void markAsCompleted();

        /**
         * @brief Loads all invoices from a local JSON file into memory and updates the list view.
         *
         * Reads data from 'invoices.json', deserializes it, and populates the GUI list.
         * This is called during startup and after every edit to refresh the UI.
         */
        void loadInvoices();

        /**
         * @brief Saves the current state of all invoices to a JSON file.
         *
         * Serializes the internal invoice list into a QJsonArray and writes it to 'invoices.json'.
         * Ensures that all invoice data persists between app sessions.
         */
        void saveInvoices();

        /**
         * @brief Opens a modal dialog allowing the user to edit the selected invoice.
         *
         * Pre-fills fields with existing invoice data. After editing, updates the invoice,
         * saves the changes to disk, and refreshes the UI.
         */
        void editInvoice();

        /**
         * @brief Displays detailed, read-only information about a selected invoice.
         *
         * Triggered on double-clicking an invoice in the list. Shows a popup with full
         * customer and invoice data in a structured format.
         *
         * @param item Pointer to the clicked item in the QListWidget.
         */
        void viewInvoiceDetails(QListWidgetItem* item);

    private:
        // --- UI Elements ---

        /**
         * @brief Input field for customer's full name.
         */
        QLineEdit* customerNameEdit;

        /**
         * @brief Input field for the customer's address.
         */
        QLineEdit* customerAddressEdit;

        /**
         * @brief Input field for the invoice amount.
         */
        QLineEdit* amountEdit;

        /**
         * @brief Input field for the due date (expects yyyy-MM-dd).
         */
        QLineEdit* dueDateEdit;

        /**
         * @brief Widget displaying all invoices in a scrollable list.
         */
        QListWidget* invoiceListWidget;

        /**
         * @brief Internal container holding all created invoice objects.
         *
         * Used to build and serialize invoice data as well as manipulate entries.
         */
        QVector<Invoice> invoices;
};

#endif // INVOICEMANAGER_H