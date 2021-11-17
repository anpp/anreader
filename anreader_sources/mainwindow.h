#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QStatusBar>
#include <QProgressBar>
#include <QDateTime>
#include <QMap>
#include <QPointer>
#include <QMenuBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QClipboard>
#include <QApplication>
#include <tuple>


#include "common.h"
#include "device/neptune_hil.h"
#include "jumpstable.h"
#include "jumpstablemodel.h"
#include "devices_list_widget.h"
#include "settings.h"
#include "log_widget.h"
#include "n3jumpeditor.h"
#include "datalists.h"
#include "datalist_dialog/datalist_dialog.h"

enum MainWindow_defs: int {BaseWidht = 1100, BaseHeight = 500, StatusStateLabelWidth = 250};


class StatusFrame : public QWidget
{
    Q_OBJECT
public:
    explicit StatusFrame(QWidget *parent = nullptr);
    ~StatusFrame() {}

    void setMinimum(int value) {m_progress.setMinimum(value); }
    void setMaximum(int value) {m_progress.setMaximum(value); }
    void setValue(int value) {m_progress.setValue(value); }
    int getValue() const {return m_progress.value(); }

private:
    QLabel m_status;
    QProgressBar m_progress;

protected:

signals:

public slots:
    void setStatusText(const QString& text);
};



//================================================================================================================================================


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:    
    void initMainWindow();
    void createActions();
    void createDevicesWidget();
    void createLogWidget();
    bool saveAsCSV(const QString& filename, const JumpsTableModel& jm, const QString delimiter = ";");
    bool openFromCSV(const QString& filename, JumpsTableModel& jm, const QString delimiter = ";");
    void setCurrentFile(const QString &fileName);
    bool saveQuestion();
    void fileWasModified(bool value);
    QString delimiterCSV() { return settings.getSetting("current_file_delimiter").toString(); }
    void open_DataListDialog(const datakind dlk, map_DataList& data);
    void selection_changed(bool enable = true);
    void editJump(uint row_index);
    void initDataInLists(const QString& aircraft, const QString& dropzone, const QString& canopy);

    QPointer<StatusFrame> getStatusFrame(const int id);

    JumpsTableModel jumps_model;
    std::unique_ptr<JumpsTable> jtable;
    std::unique_ptr<DevicesWidget> devices_window;
    std::unique_ptr<LogWidget> log_widget;

    std::map<int, std::unique_ptr<StatusFrame>> map_status_frames;

    QPointer<QAction> m_newAct;
    QPointer<QAction> m_openAct;
    QPointer<QAction> m_saveAct;
    QPointer<QAction> m_saveAsAct;
    QPointer<QAction> m_copyAct;
    //QPointer<QAction> m_pasteAct;
    QPointer<QAction> m_editAct;
    QPointer<QAction> m_deleteAct;
    QPointer<QAction> m_registerAicraftsAct;
    QPointer<QAction> m_registerDropZonesAct;
    QPointer<QAction> m_registerCanopiesAct;
    QVector<QPointer<QAction>> m_actions;

    QPointer<QAction> m_toggleDevices;
    QPointer<QAction> m_toggleLog;

    QString defaultFilter = QObject::tr("CSV file with semicolon delimiters (*.csv)");
    QString commaFilter = QObject::tr("CSV file with comma delimiters (*.csv)");
    QString dateFormat = "dd.MM.yyyy hh:mm";
    QString dateFormat_excel = "dd.MM.yyyy h:mm";

    QString current_file;
    bool file_was_modified = false;
    QTimer timer_open_file;

    Settings settings{this, "anreader", "anreader"};
    DataLists dl{"anreader", "anreader"};

//signals:

public slots:
    void createStatusFrame(const int id);
    void initProgress(const int id, unsigned int value);
    void setStatusText(const int id, const QString& text);
    void stepProgress(const int id);
    void finish(const DWidget& widget);
    void afterConnect(const DWidget& widget);
    void prepareTableAfterEdit(JumpsTable& table) const;
    void prepareTableAfterLoad(JumpsTable& table) const;
    void onCreate();

    void newFile();
    void open();
    bool save();
    bool saveAs();
    void delete_selected();
    void copy_selected();
    void edit_selected();
    void aicrafts_list()  {open_DataListDialog(datakind::aircrafts, dl.aircrafts()); }
    void dropzones_list() {open_DataListDialog(datakind::dropzones, dl.dropzones()); }
    void canopies_list()  {open_DataListDialog(datakind::canopies, dl.canopies()); }
    void about();
    void documentWasModified();
    void enableActions(const bool enable);
    void log(const QString& value);
    void selectionChanged();
    void table_doubleClicked(const QModelIndex& index);
};
#endif // MAINWINDOW_H
