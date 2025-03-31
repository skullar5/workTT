#ifndef INVOICE_H
#define INVOICE_H

#include <QString>
#include <QDate>
#include <QJsonObject>

/**
 * @class Invoice
 * @brief Handles the invoice and holding data for invoice
 *
 * This class is used to hold the variables and data of an invoice
 * It includes getters and setters for the invoice as well as being able to create JSON file
 *
 * @author Maneet Chahal
 */
class Invoice {
public:
    Invoice();
    /**
     * @brief Default constructor.
     *
     * Initializes a new invoice with a unique ID, today's date as due date,
     * amount set to 0.0, and status set to "Pending".
     */

    Invoice(const QString& customerName, const QString& customerAddress, double amount, const QDate& dueDate);

    /**
     * @brief Gets the customer's name.
     * @return Customer name.
     */
    QString getCustomerName() const;

    /**
     * @brief Gets the customer's address.
     * @return Customer address.
     */
    QString getCustomerAddress() const;

    /**
     * @brief Gets the invoice amount.
     * @return Amount due.
     */
    double getAmount() const;

    /**
     * @brief Gets the due date of the invoice.
     * @return Due date.
     */
    QDate getDueDate() const;

    /**
     * @brief Gets the current status of the invoice.
     * @return Status string (e.g., "Pending", "Paid").
     */
    QString getStatus() const;

    /**
     * @brief Gets the unique invoice ID.
     * @return Invoice ID.
     */
    int getInvoiceID() const;

    /**
     * @brief Sets the customer's name.
     * @param name New customer name.
     */
    void setCustomerName(const QString& name);

    /**
     * @brief Sets the customer's address.
     * @param address New customer address.
     */
    void setCustomerAddress(const QString& address);

    /**
     * @brief Sets the invoice amount.
     * @param amount New amount due.
     */
    void setAmount(double amount);

    /**
     * @brief Sets the invoice due date.
     * @param dueDate New due date.
     */
    void setDueDate(const QDate& dueDate);

    /**
     * @brief Sets the invoice status.
     * @param status New status (e.g., "Pending", "Completed").
     */
    void setStatus(const QString& status);

    /**
     * @brief Serializes the invoice data to a JSON object.
     * @return JSON representation of the invoice.
     */
    QJsonObject toJson() const;

    /**
     * @brief Creates an Invoice object from a JSON object.
     * @param json JSON object containing invoice data.
     * @return Invoice instance.
     */
    static Invoice fromJson(const QJsonObject& json);

private:
    static int invoiceCounter;  // Counter for generating unique invoice IDs
    int invoiceID;              // Unique invoice ID
    QString customerName;       // Name of the customer
    QString customerAddress;    // Customer's billing address
    double amount;              // Total amount due
    QDate dueDate;              // Date payment is due
    QString status;             // Current status of the invoice
};

#endif // INVOICE_H