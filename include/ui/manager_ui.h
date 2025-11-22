#pragma once

#include <QMainWindow>
#include <QTableView>
#include <QTimer>
#include "core/worker_interface.h"

namespace hyperfarm {

// Forward declaration
class WorkerManager;

class ManagerUI : public QMainWindow {
    Q_OBJECT
    
public:
    explicit ManagerUI(WorkerManager* manager, QWidget* parent = nullptr);
    virtual ~ManagerUI();
    
private slots:
    void onWorkerStatusChanged(WorkerId id);
    void onStartWorkerClicked();
    void onStopWorkerClicked();
    void onPauseWorkerClicked();
    void updateUI();
    
private:
    void setupUI();
    void updateWorkerTable();
    
    WorkerManager* manager_;
    QTableView* worker_table_;
    QTimer* update_timer_;
};

} // namespace hyperfarm
