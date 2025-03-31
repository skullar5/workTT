#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QChartView>
#include <QBarSeries>
#include <QLineSeries>
#include <QPieSeries>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
class QTabWidget;
QT_END_NAMESPACE

/**
 * @class Dashboard
 * @brief The Dashboard class represents a widget that displays various financial and inventory data in chart and table formats.
 *
 * This class provides functionality to display and manage financial data, inventory information, and monthly reports
 * through different charts (bar, line, pie) and tables.
 */
    class Dashboard : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Dashboard widget with the given parent.
     * @param parent The parent widget (default is nullptr).
     */
    explicit Dashboard(QWidget* parent = nullptr);

    /**
     * @brief Destroys the Dashboard widget.
     */
    ~Dashboard();

    /**
     * @brief Sets the tab widget for the dashboard.
     * @param tabs Pointer to the QTabWidget to be used in the dashboard.
     */
    void setTabWidget(QTabWidget* tabs);

    /**
     * @brief Sets the inventory table to be displayed in the dashboard.
     * @param sharedTable Pointer to the QTableWidget containing inventory data.
     */
    void setInventoryTable(QTableWidget* sharedTable);

    /**
     * @brief Gets the inventory table currently displayed in the dashboard.
     * @return Pointer to the QTableWidget containing inventory data.
     */
    QTableWidget* getInventoryTable() const;

    /**
     * @brief Loads monthly data from storage.
     */
    void loadMonthlyData();

    /**
     * @brief Saves monthly data to storage.
     */
    void saveMonthlyData();

    /**
     * @brief Displays a notification about available updates.
     */
    void showUpdateNotification();

    /**
     * @brief Updates all charts in the dashboard with current data.
     */
    void updateCharts();

    /**
     * @brief Updates financial information displays.
     */
    void updateFinancials();

    /**
     * @brief Updates inventory information displays.
     */
    void updateInventory();

    /**
     * @brief Updates monthly reports displays.
     */
    void updateMonthlyReports();

    /**
     * @brief Sets the current user ID for data management.
     * @param userId The ID of the current user.
     */
    void setCurrentUserId(const QString& userId);

    /**
     * @brief Gets the current user ID.
     * @return The ID of the current user.
     */
    QString getCurrentUserId() const;

    /**
     * @brief Loads monthly revenue data for a specific user.
     * @param userId The ID of the user whose data should be loaded.
     */
    void loadMonthlyRevenueData(const QString& userId);

    /**
     * @brief Saves monthly revenue data for a specific user.
     * @param userId The ID of the user whose data should be saved.
     */
    void saveMonthlyRevenueData(const QString& userId);

    /**
     * @brief Slot called when the update dashboard button is clicked.
     */
    void onUpdateDashboardClicked();

private:
    QString m_currentUserId;          ///< Stores the current user's ID
    QString m_dataFilePath;           ///< Path to the data file

    QTabWidget* dashboardTabWidget = nullptr;  ///< Pointer to the tab widget
    QTableWidget* inventoryTable = nullptr;    ///< Pointer to the inventory table

    // Financial labels
    QLabel* cashInflowsLabel;        ///< Label for cash inflows display
    QLabel* cashOutflowsLabel;       ///< Label for cash outflows display
    QLabel* financialsLabel;         ///< Label for financial summary
    QLabel* inventoryLabel;          ///< Label for inventory summary
    QLabel* monthlyReportsLabel;     ///< Label for monthly reports

    QTableWidget* monthlyReportsTable;  ///< Table widget for monthly reports
    QPushButton* editInventoryButton;   ///< Button to edit inventory
    QPushButton* updateButton;          ///< Button to update dashboard

    QVBoxLayout* rightLayout;           ///< Layout for right side elements

    // Chart objects
    QChart* barChart;                   ///< Bar chart object
    QChart* lineChart;                  ///< Line chart object
    QChart* pieChart;                   ///< Pie chart object

    // Chart series
    QBarSeries* barSeries;              ///< Series for bar chart
    QLineSeries* lineSeries;            ///< Series for line chart
    QPieSeries* pieSeries;              ///< Series for pie chart

    // Chart views
    QChartView* barChartView;           ///< View for bar chart
    QChartView* lineChartView;          ///< View for line chart
    QChartView* pieChartView;           ///< View for pie chart

    /**
     * @brief Initializes the pie chart with default settings.
     */
    void setupPieChart();

    /**
     * @brief Updates the pie chart with current data.
     */
    void updatePieChart();

    QString currentUserId;               ///< Current user ID (duplicate member?)
    QLabel* updateNotificationLabel;     ///< Label for update notifications
};

#endif // DASHBOARD_H