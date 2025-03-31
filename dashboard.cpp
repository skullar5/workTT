/**
 * @file dashboard.cpp
 * @brief Implementation of the Dashboard class for business analytics visualization where users can view their monthly revenue,
 * access the inventory table page, view inflows and outflows, and see their revenue projection.
 * @author Kareena Sen
 * @date March 30th, 2025
 */

#include "dashboard.h"
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

 /**
  * @brief Constructs a Dashboard widget
  * @param parent The parent QWidget (optional)
  *
  * Initializes the dashboard with:
  * - Financial data displays
  * - Inventory management
  * - Monthly revenue tracking
  * - Interactive charts (bar, line, and pie)
  */
Dashboard::Dashboard(QWidget* parent) : QWidget(parent)
{
    setWindowTitle("Business Dashboard");

    // Main layout
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    // Left side layout (tables)
    QVBoxLayout* leftLayout = new QVBoxLayout();

    // Financial Data
    financialsLabel = new QLabel("Financials: Cash Inflows and Outflows", this);
    financialsLabel->setAlignment(Qt::AlignCenter);
    cashInflowsLabel = new QLabel("Inflows: $5000", this);
    cashOutflowsLabel = new QLabel("Outflows: $3000", this);

    QVBoxLayout* financialsLayout = new QVBoxLayout();
    financialsLayout->addWidget(cashInflowsLabel);
    financialsLayout->addWidget(cashOutflowsLabel);
    leftLayout->addWidget(financialsLabel);
    leftLayout->addLayout(financialsLayout);

    // Inventory Data
    inventoryLabel = new QLabel("Inventory Data", this);
    inventoryLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(inventoryLabel);

    inventoryTable = nullptr;
    editInventoryButton = new QPushButton("Edit Inventory", this);
    leftLayout->addWidget(editInventoryButton);

    connect(editInventoryButton, &QPushButton::clicked, [this]() {
        if (dashboardTabWidget) {
            dashboardTabWidget->setCurrentIndex(5);
        }
        });

    // Monthly Reports - Initialize with all 12 months
    monthlyReportsLabel = new QLabel("Monthly Revenue", this);
    monthlyReportsLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(monthlyReportsLabel);

    monthlyReportsTable = new QTableWidget(12, 2, this);
    monthlyReportsTable->setHorizontalHeaderLabels({ "Month", "Revenue" });
    monthlyReportsTable->setEditTriggers(QAbstractItemView::DoubleClicked);
    monthlyReportsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Initialize months
    QStringList months = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    for (int i = 0; i < 12; ++i) {
        monthlyReportsTable->setItem(i, 0, new QTableWidgetItem(months[i]));
        monthlyReportsTable->setItem(i, 1, new QTableWidgetItem("$")); // Default value
    }

    leftLayout->addWidget(monthlyReportsTable);
    loadMonthlyData(); // Load saved data

    // Right side layout (charts)
    rightLayout = new QVBoxLayout();

    // Bar Chart
    barChart = new QChart();
    barChart->setTitle("Monthly Revenue");
    barSeries = new QBarSeries();
    barChart->addSeries(barSeries);

    QStringList categories;
    for (const QString& month : months) {
        categories << month;
    }

    QBarCategoryAxis* barAxisX = new QBarCategoryAxis();
    barAxisX->append(categories);
    barChart->addAxis(barAxisX, Qt::AlignBottom);
    barSeries->attachAxis(barAxisX);

    QValueAxis* barAxisY = new QValueAxis();
    barAxisY->setLabelFormat("$%.0f");
    barChart->addAxis(barAxisY, Qt::AlignLeft);
    barSeries->attachAxis(barAxisY);

    barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(barChartView);

    // Line Chart
    lineChart = new QChart();
    lineChart->setTitle("Revenue Trend");
    lineSeries = new QLineSeries();
    lineChart->addSeries(lineSeries);

    QValueAxis* lineAxisX = new QValueAxis();
    lineAxisX->setTitleText("Month");
    QValueAxis* lineAxisY = new QValueAxis();
    lineAxisY->setLabelFormat("$%.0f");
    lineChart->addAxis(lineAxisX, Qt::AlignBottom);
    lineChart->addAxis(lineAxisY, Qt::AlignLeft);
    lineSeries->attachAxis(lineAxisX);
    lineSeries->attachAxis(lineAxisY);

    lineChartView = new QChartView(lineChart);
    lineChartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(lineChartView);

    // Pie Chart
    setupPieChart();

    // Add the layouts to main layout
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 2);

    // Update Button
    updateButton = new QPushButton("Update Dashboard", this);
    leftLayout->addWidget(updateButton);

    // Connect signals
    connect(updateButton, &QPushButton::clicked, this, &Dashboard::onUpdateDashboardClicked);

    updateCharts();
}

/**
 * @brief Shows update notification with total revenue
 *
 * Displays a message box with the sum of all monthly revenues
 */
void Dashboard::showUpdateNotification()
{
    double totalRevenue = 0;
    for (int i = 0; i < 12; i++) {
        if (monthlyReportsTable->item(i, 1)) {
            QString val = monthlyReportsTable->item(i, 1)->text().remove('$');
            totalRevenue += val.toDouble();
        }
    }

    QMessageBox::information(this, "Dashboard Updated",
        QString("All data has been successfully updated!\n"
            "Total Revenue: $%1")
        .arg(totalRevenue, 0, 'f', 2));
}

/**
 * @brief Updates all chart visualizations
 *
 * Refreshes bar chart, line chart, and pie chart with current data
 */
void Dashboard::updateCharts()
{
    // Clear existing data
    barSeries->clear();
    lineSeries->clear();

    // Create a single bar set for monthly revenue
    QBarSet* revenueSet = new QBarSet("Revenue");
    double maxRevenue = 0;

    // Process all 12 months
    for (int row = 0; row < 12; ++row) {
        QTableWidgetItem* monthItem = monthlyReportsTable->item(row, 0);
        QTableWidgetItem* revenueItem = monthlyReportsTable->item(row, 1);

        if (!monthItem || !revenueItem) continue;

        QString revenueStr = revenueItem->text().remove('$').remove(',');
        bool ok;
        double revenue = revenueStr.toDouble(&ok);

        if (!ok) revenue = 0.0;

        // Add to bar chart
        *revenueSet << revenue;

        // Update line chart
        lineSeries->append(row, revenue);

        // Track max revenue
        if (revenue > maxRevenue) maxRevenue = revenue;
    }

    // Add the revenue set to the bar series
    barSeries->append(revenueSet);

    // Update y-axis range for both charts
    QValueAxis* barAxisY = qobject_cast<QValueAxis*>(barChart->axes(Qt::Vertical).first());
    if (barAxisY) barAxisY->setRange(0, maxRevenue * 1.2);

    QValueAxis* lineAxisX = qobject_cast<QValueAxis*>(lineChart->axes(Qt::Horizontal).first());
    if (lineAxisX) lineAxisX->setRange(0, 11); // 0-11 for 12 months

    QValueAxis* lineAxisY = qobject_cast<QValueAxis*>(lineChart->axes(Qt::Vertical).first());
    if (lineAxisY) lineAxisY->setRange(0, maxRevenue * 1.2);

    // Update pie chart with current inflows and outflows
    updatePieChart();

    // Save and notify
    saveMonthlyData();
}

/**
 * @brief Initializes the pie chart visualization
 *
 * Sets up the pie chart with initial inflow/outflow data
 */
void Dashboard::setupPieChart()
{
    pieChart = new QChart();
    pieChart->setTitle("Financial Breakdown");
    pieSeries = new QPieSeries();

    // Extract numeric values from labels
    double inflows = cashInflowsLabel->text().split('$').last().toDouble();
    double outflows = cashOutflowsLabel->text().split('$').last().toDouble();

    // Add slices
    pieSeries->append("Inflows", inflows);
    pieSeries->append("Outflows", outflows);

    // Make slices more visible
    pieSeries->slices().at(0)->setLabelVisible(true);
    pieSeries->slices().at(0)->setBrush(QColor(0, 128, 0)); // Green for inflows
    pieSeries->slices().at(1)->setLabelVisible(true);
    pieSeries->slices().at(1)->setBrush(QColor(255, 0, 0)); // Red for outflows

    pieChart->addSeries(pieSeries);
    pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(pieChartView);
}

/**
 * @brief Updates the pie chart visualization
 *
 * Refreshes pie chart with current inflow/outflow data
 */
void Dashboard::updatePieChart()
{
    // Clear existing series
    pieChart->removeAllSeries();

    // Create new series with updated data
    pieSeries = new QPieSeries();

    // Extract numeric values from labels (removing $ and any commas)
    double inflows = cashInflowsLabel->text().split('$').last().remove(',').toDouble();
    double outflows = cashOutflowsLabel->text().split('$').last().remove(',').toDouble();

    // Add slices
    pieSeries->append("Inflows", inflows);
    pieSeries->append("Outflows", outflows);

    // Make slices more visible
    pieSeries->slices().at(0)->setLabelVisible(true);
    pieSeries->slices().at(0)->setBrush(QColor(0, 128, 0)); // Green for inflows
    pieSeries->slices().at(1)->setLabelVisible(true);
    pieSeries->slices().at(1)->setBrush(QColor(255, 0, 0)); // Red for outflows

    pieChart->addSeries(pieSeries);
}

/**
 * @brief Updates financial data displays
 *
 * Refreshes the inflow/outflow labels (example implementation)
 */
void Dashboard::updateFinancials()
{
    // Example update - in real app you would get these from actual data
    cashInflowsLabel->setText("Inflows: $5500");
    cashOutflowsLabel->setText("Outflows: $3200");
}

/**
 * @brief Updates inventory data display
 *
 * Refreshes the inventory table (example implementation)
 */
void Dashboard::updateInventory()
{
    if (inventoryTable && inventoryTable->rowCount() > 0) {
        inventoryTable->setItem(0, 1, new QTableWidgetItem("120"));
    }
}

/**
 * @brief Updates monthly reports data
 *
 * Handles saving of monthly revenue data for the current user
 */
void Dashboard::updateMonthlyReports()
{
    // Data is now handled directly through the table
    if (!currentUserId.isEmpty()) {
        saveMonthlyRevenueData(currentUserId);
    }
}

/**
 * @brief Sets reference to the main tab widget
 * @param tabs Pointer to the QTabWidget
 *
 * Allows navigation to other tabs from the dashboard
 */
void Dashboard::setTabWidget(QTabWidget* tabs)
{
    dashboardTabWidget = tabs;
}

/**
 * @brief Sets the shared inventory table
 * @param sharedTable Pointer to the inventory QTableWidget
 *
 * Connects the dashboard to the shared inventory data table
 */
void Dashboard::setInventoryTable(QTableWidget* sharedTable)
{
    inventoryTable = sharedTable;
    inventoryTable->setParent(this);
    inventoryTable->setHorizontalHeaderLabels({ "Item", "Quantity", "Price" });
    inventoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if (inventoryTable->rowCount() == 0) {
        inventoryTable->insertRow(0);
        inventoryTable->setItem(0, 0, new QTableWidgetItem("Item 1"));
        inventoryTable->setItem(0, 1, new QTableWidgetItem("100"));
        inventoryTable->setItem(0, 2, new QTableWidgetItem("$10"));

        inventoryTable->insertRow(1);
        inventoryTable->setItem(1, 0, new QTableWidgetItem("Item 2"));
        inventoryTable->setItem(1, 1, new QTableWidgetItem("200"));
        inventoryTable->setItem(1, 2, new QTableWidgetItem("$5"));
    }

    QVBoxLayout* leftLayout = qobject_cast<QVBoxLayout*>(layout()->itemAt(0)->layout());
    if (leftLayout) leftLayout->insertWidget(3, inventoryTable);
}

/**
 * @brief Gets the inventory table
 * @return Pointer to the inventory QTableWidget
 */
QTableWidget* Dashboard::getInventoryTable() const
{
    return inventoryTable;
}

/**
 * @brief Dashboard destructor
 *
 * Cleans up all dynamically allocated chart resources
 */
Dashboard::~Dashboard()
{
    delete barChart;
    delete lineChart;
    delete pieChart;
    delete barSeries;
    delete lineSeries;
    delete pieSeries;
    delete barChartView;
    delete lineChartView;
    delete pieChartView;
}

/**
 * @brief Sets the current user ID
 * @param userId The user identifier string
 */
void Dashboard::setCurrentUserId(const QString& userId) {
    currentUserId = userId;
}

/**
 * @brief Gets the current user ID
 * @return The user identifier string
 */
QString Dashboard::getCurrentUserId() const {
    return currentUserId;
}

/**
 * @brief Loads monthly revenue data for a specific user
 * @param userId The user identifier string
 */
void Dashboard::loadMonthlyRevenueData(const QString& userId) {
    currentUserId = userId;
    QDir().mkpath("data");
    QString fileName = "data/" + userId + "_monthly_revenue.json";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject obj = doc.object();
    for (int i = 0; i < 12; ++i) {
        QString month = monthlyReportsTable->item(i, 0)->text();
        if (obj.contains(month)) {
            double value = obj[month].toDouble();
            monthlyReportsTable->item(i, 1)->setText(QString("$%1").arg(value, 0, 'f', 2));
        }
    }
}

/**
 * @brief Saves monthly revenue data for a specific user
 * @param userId The user identifier string
 */
void Dashboard::saveMonthlyRevenueData(const QString& userId) {
    QJsonObject obj;
    for (int i = 0; i < 12; ++i) {
        QString month = monthlyReportsTable->item(i, 0)->text();
        QString revenueStr = monthlyReportsTable->item(i, 1)->text().remove('$').remove(',');
        obj[month] = revenueStr.toDouble();
    }
    QDir().mkpath("data");
    QString fileName = "data/" + userId + "_monthly_revenue.json";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(obj).toJson());
        file.close();
    }
}

void Dashboard::loadMonthlyData()
{
    if (currentUserId.isEmpty()) return;
    QString fileName = "data/" + currentUserId + "_dashboard.json";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc(QJsonDocument::fromJson(file.readAll()));
    file.close();

    QJsonObject json = doc.object();
    for (int i = 0; i < 12; ++i) {
        QString month = monthlyReportsTable->item(i, 0)->text();
        if (json.contains(month)) {
            double value = json[month].toDouble();
            monthlyReportsTable->item(i, 1)->setText(QString("$%1").arg(value, 0, 'f', 2));
        }
    }
}

void Dashboard::saveMonthlyData()
{
    if (currentUserId.isEmpty()) return;
    QJsonObject json;
    for (int i = 0; i < 12; ++i) {
        QString month = monthlyReportsTable->item(i, 0)->text();
        QString revenue = monthlyReportsTable->item(i, 1)->text().remove('$');
        json[month] = revenue.toDouble();
    }

    QDir().mkpath("data");
    QString fileName = "data/" + currentUserId + "_dashboard.json";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(json);
        file.write(doc.toJson());
        file.close();
    }
}


/**
 * @brief Handles update button click
 *
 * Coordinates updating all dashboard components and showing notification
 */
void Dashboard::onUpdateDashboardClicked()
{
    updateFinancials();
    updateInventory();
    updateMonthlyReports();
    updateCharts(); // charts update without showing message box
    showUpdateNotification(); // show the message here only!
}
