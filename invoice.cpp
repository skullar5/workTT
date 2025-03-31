#include "invoice.h"

int Invoice::invoiceCounter = 1;

/**
 * @brief Default constructor.
 *
 * Initializes the invoice with default values:
 * - invoice ID auto-incremented from static counter
 * - amount set to 0.0
 * - due date set to current date
 * - status set to "Pending"
 */
Invoice::Invoice()
    : invoiceID(invoiceCounter++), amount(0.0), dueDate(QDate::currentDate()), status("Pending") {
}

/**
 * @brief Parameterized constructor.
 *
 * Initializes an invoice with specified values:
 * - assigns unique invoice ID
 * - sets customer name, address, amount, and due date
 * - status defaults to "Pending"
 *
 * @param customerName The name of the customer.
 * @param customerAddress The billing address of the customer.
 * @param amount The total invoice amount.
 * @param dueDate The payment due date.
 */
Invoice::Invoice(const QString& customerName, const QString& customerAddress, double amount, const QDate& dueDate)
    : invoiceID(invoiceCounter++), customerName(customerName), customerAddress(customerAddress), amount(amount), dueDate(dueDate), status("Pending") {
}


/** @return The name of the customer. */
QString Invoice::getCustomerName() const { return customerName; }

/** @return The billing address of the customer. */
QString Invoice::getCustomerAddress() const { return customerAddress; }

/** @return The amount due on the invoice. */
double Invoice::getAmount() const { return amount; }

/** @return The date by which payment is due. */
QDate Invoice::getDueDate() const { return dueDate; }

/** @return The current payment status (e.g., Pending, Paid). */
QString Invoice::getStatus() const { return status; }

/** @return The unique invoice identifier. */
int Invoice::getInvoiceID() const { return invoiceID; }

/** @param name New customer name. */
void Invoice::setCustomerName(const QString& name) { customerName = name; }

/** @param address New customer address. */
void Invoice::setCustomerAddress(const QString& address) { customerAddress = address; }

/** @param amt New invoice amount. */
void Invoice::setAmount(double amt) { amount = amt; }

/** @param date New due date. */
void Invoice::setDueDate(const QDate& date) { dueDate = date; }

/** @param stat New payment status. */
void Invoice::setStatus(const QString& stat) { status = stat; }

/**
 * @brief Converts the invoice into a JSON object.
 *
 * Serializes all invoice fields including:
 * - invoiceID
 * - customerName
 * - customerAddress
 * - amount
 * - dueDate
 * - status
 *
 * @return A QJsonObject representing the invoice.
 */
QJsonObject Invoice::toJson() const {
    QJsonObject json;
    json["invoiceID"] = invoiceID;
    json["customerName"] = customerName;
    json["customerAddress"] = customerAddress;
    json["amount"] = amount;
    json["dueDate"] = dueDate.toString(Qt::ISODate);
    json["status"] = status;
    return json;
}

/**
 * @brief Reconstructs an Invoice object from a JSON object.
 *
 * Parses the necessary fields from the JSON and assigns them to a new Invoice instance.
 *
 * @param json The JSON object containing invoice data.
 * @return The corresponding Invoice object.
 */
Invoice Invoice::fromJson(const QJsonObject& json) {
    Invoice invoice;
    invoice.invoiceID = json["invoiceID"].toInt();
    invoice.customerName = json["customerName"].toString();
    invoice.customerAddress = json["customerAddress"].toString();
    invoice.amount = json["amount"].toDouble();
    invoice.dueDate = QDate::fromString(json["dueDate"].toString(), Qt::ISODate);
    invoice.status = json["status"].toString();
    return invoice;
}