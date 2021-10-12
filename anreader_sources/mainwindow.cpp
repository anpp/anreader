#include "mainwindow.h"
#include "csvparser.h"

//----------------------------------------------------------------------------------------------------------------------
StatusFrame::StatusFrame(QWidget *parent) : QWidget (parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(&m_status);
    layout->addWidget(&m_progress);

    m_progress.setFixedHeight(12);
}

void StatusFrame::setStatusText(const QString &text)
{
    m_status.setText(text);
}



//============================================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    this->resize(MainWindow_defs::BaseWidht, MainWindow_defs::BaseHeight);    
    setWindowIcon(QIcon(":/images/icons/main/anreader.ico"));
    //QApplication::setStyle("fusion");
    initMainWindow();
    createActions();    

    settings.load();

    timer_open_file.setInterval(100);
    connect(&timer_open_file, &QTimer::timeout, this, &MainWindow::onCreate);
    timer_open_file.start();
}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{    
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::onCreate()
{
    timer_open_file.stop();
    disconnect(&timer_open_file, &QTimer::timeout, this, &MainWindow::onCreate);

    //для некоторых оконных систем
    #ifndef Q_OS_WIN64
      #ifndef Q_OS_MACOS
       settings.loadSettingsScreen();
      #endif
    #endif

    setCurrentFile(settings.getSetting("current_file").toString());
    if(!settings.getSetting("current_file").toString().isEmpty())
    {
        openFromCSV(settings.getSetting("current_file").toString(), jumps_model, delimiterCSV());
        prepareTableAfterLoad(*jtable);
    }    
    setCurrentFile(current_file);    
    selectionChanged();
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(saveQuestion())
    {
        settings.save();
        event->accept();
    } else
    {
        event->ignore();
    }
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::initMainWindow()
{
    auto layout = new QHBoxLayout;
    auto *center = new QWidget(this);

    jtable = new JumpsTable();
    jtable->setModel(&jumps_model);

    connect(&jumps_model, &JumpsTableModel::dataChanged, this, &MainWindow::documentWasModified);
    connect(&jumps_model, &JumpsTableModel::rowsInserted, this, &MainWindow::documentWasModified);
    connect(&jumps_model, &JumpsTableModel::rowsRemoved, this, &MainWindow::documentWasModified);    
    connect(jtable->selectionModel(), &QItemSelectionModel::selectionChanged,  this, &MainWindow::selectionChanged);

    createDevicesWidget();    
    createLogWidget();

    //layout->addWidget(devices_window);
    layout->addWidget(jtable);

    layout->setMargin(0);
    layout->setSpacing(0);
    center->setLayout(layout);
    this->setCentralWidget(center);
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QMenu *windowsMenu = menuBar()->addMenu(tr("&Windows"));

    const QIcon newIcon = QIcon(":/images/icons/toolbar/new.png");
    m_newAct = new QAction(newIcon, tr("&New"), this);
    m_newAct->setShortcuts(QKeySequence::New);
    m_newAct->setToolTip(tr("Create a new file"));
    connect(m_newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(m_newAct);

    const QIcon openIcon = QIcon(":/images/icons/toolbar/open.png");
    m_openAct = new QAction(openIcon, tr("&Open..."), this);
    m_openAct->setShortcuts(QKeySequence::Open);
    m_openAct->setToolTip(tr("Open an existing file"));
    connect(m_openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(m_openAct);

    const QIcon saveIcon = QIcon(":/images/icons/toolbar/save.png");
    m_saveAct = new QAction(saveIcon, tr("&Save..."), this);
    m_saveAct->setShortcuts(QKeySequence::Save);
    m_saveAct->setToolTip(tr("Save the document to disk"));
    connect(m_saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(m_saveAct);

    m_saveAsAct = new QAction(saveIcon, tr("Save &As..."), this);
    m_saveAsAct->setShortcuts(QKeySequence::SaveAs);
    m_saveAsAct->setToolTip(tr("Save the document under a new name"));
    connect(m_saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    fileMenu->addAction(m_saveAsAct);

    const QIcon editIcon = QIcon(":/images/icons/toolbar/edit.png");
    m_editAct = new QAction(editIcon, tr("&Edit"), this);
    m_editAct->setShortcuts(QKeySequence::UnknownKey);
    m_editAct->setToolTip(tr("Edit selected jump..."));
    connect(m_editAct, &QAction::triggered, this, &MainWindow::edit_selected);
    editMenu->addAction(m_editAct);

    const QIcon deleteIcon = QIcon(":/images/icons/toolbar/delete.png");
    m_deleteAct = new QAction(deleteIcon, tr("&Delete"), this);
    m_deleteAct->setShortcuts(QKeySequence::Delete);
    m_deleteAct->setToolTip(tr("Delete selected jumps"));
    connect(m_deleteAct, &QAction::triggered, this, &MainWindow::delete_selected);
    editMenu->addAction(m_deleteAct);


    QToolBar *mainToolBar = addToolBar(tr("File"));
    mainToolBar->setIconSize(QSize(40, 40));
    mainToolBar->setObjectName("file toolbar");
    mainToolBar->addAction(m_newAct);
    mainToolBar->addAction(m_openAct);
    mainToolBar->addAction(m_saveAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(m_editAct);
    mainToolBar->addAction(m_deleteAct);

    if(m_toggleDevices)
        windowsMenu->addAction(m_toggleDevices);
    if(m_toggleLog)
        windowsMenu->addAction(m_toggleLog);

        //QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    //aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::createDevicesWidget()
{
    QDockWidget *dock = new QDockWidget(tr("Devices"), this);
    dock->setObjectName("devices_widget_dock");
    //dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_toggleDevices = dock->toggleViewAction();
    m_toggleDevices->setShortcut(QKeySequence("F11"));

    devices_window = new DevicesWidget(dock);
    dock->setWidget(devices_window);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    connect(devices_window, &DevicesWidget::createStatusFrame, this, &MainWindow::createStatusFrame);
    connect(devices_window, &DevicesWidget::newTextOfState, this, &MainWindow::setStatusText);
    connect(devices_window, &DevicesWidget::setProgress, this, &MainWindow::initProgress);
    connect(devices_window, &DevicesWidget::stepProgress, this, &MainWindow::stepProgress);
    connect(devices_window, &DevicesWidget::receivedData, this, &MainWindow::finish);
    connect(devices_window, &DevicesWidget::controls_is_enabled, this, &MainWindow::enableActions);
    connect(devices_window, &DevicesWidget::log, this, &MainWindow::log);
    connect(devices_window, &DevicesWidget::giveLastJump, &jumps_model, &JumpsTableModel::takeLastJump);

    devices_window->createDefaults();
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::createLogWidget()
{
    QDockWidget *dock = new QDockWidget(tr("Log"), this);
    dock->setObjectName("log_widget_dock");
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    dock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    m_toggleLog = dock->toggleViewAction();
    m_toggleLog->setShortcut(QKeySequence("F12"));

    log_widget = new LogWidget(dock);
    log_widget->setMinimumHeight(LogWidget_defs::MinimumHeight);
    dock->setWidget(log_widget);

    addDockWidget(Qt::BottomDockWidgetArea, dock);
}

//----------------------------------------------------------------------------------------------------------------------
bool MainWindow::saveAsCSV(const QString& filename, const JumpsTableModel& jm, const QString delimiter)
{
    settings.setSetting("current_file_delimiter", delimiter);

    QString owner;
    if(QFile(filename).exists())
    {
        QFileInfo fileinfo(filename);
        owner = fileinfo.owner();
    }

    QFile f(filename);
    if(f.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&f );
        QStringList csv_string;

        //csv_string << "\" \"";        
        for(auto i = 0; i < jm.columnCount(QModelIndex()); ++i)
            csv_string << "\"" + jm.headerData(i, Qt::Horizontal).toString()+ "\"";
        ts << csv_string.join(delimiter) + "\n";

        for(auto i = 0; i < jm.rowCount(QModelIndex()); ++i )
        {
            csv_string.clear();
            //csv_string << "\"" + tv.model()->headerData(i, Qt::Vertical).toString()+ "\"";
            for(auto j = 0; j < jm.columnCount(QModelIndex()); ++j)
            {                
                QString csv_field;
                switch(j)
                {
                case CustomJumpNames::JumpDate:
                    if(jm.data(jm.index(i, j), Qt::DisplayRole).canConvert(QMetaType::QDateTime))
                            csv_string << "\"" + jm.data(jm.index(i, j), Qt::DisplayRole).toDateTime().toString(dateFormat) + "\"";
                    break;
                case N3JumpNames::Deleted:
                    csv_field = "";
                    if(jm.data(jm.index(i, j), Qt::CheckStateRole).canConvert(QMetaType::Bool))
                        csv_field = jm.data(jm.index(i, j), Qt::CheckStateRole).toBool() ? "1": "0";
                     csv_string << ("\"" + csv_field + "\"");
                    break;
                default:
                    csv_field = jm.data(jm.index(i, j), Qt::DisplayRole).toString();
                    csv_field = csv_field.replace("\"", "\"\"");
                    csv_string << "\"" + csv_field + "\"";
                    break;
                }

            }
            ts << csv_string.join(delimiter) + "\n";
        }
        f.close();
        return true;
    }
    else
    {
        log(f.errorString() + " Error - cannot write file: " + filename + (owner.isEmpty() ? "" : ". File owner is " + owner));
        return false;
    }
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::openFromCSV(const QString &filename, JumpsTableModel& jm, const QString delimiter)
{
    settings.setSetting("current_file_delimiter", delimiter);

    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        QString header = in.readLine();
        if(header == ""){
            log("Error - file format: " + filename);
            return;
        }

        QStringList field_names;
        foreach(const QString& item, *CSVParser::csvToken(header, delimiter)){
            field_names << item;
        }

        if(field_names.count() < 3){
            log("Error - file format: " + filename);
            return;
        }


        std::unique_ptr<t_rows> jumps = std::make_unique<t_rows>();
        while (!in.atEnd())
        {
            QString line = in.readLine();
            t_jump_attribute jump_data;
            std::shared_ptr<CustomJump> jump = std::make_shared<N3Jump>();

            int index = 0;
            auto checkFormat { [&] (bool greater) -> bool
                {
                    if((index >= field_names.count() && greater) || (index < field_names.count() && !greater)){
                        log("Error - file format: " + filename);
                        return false;
                    }
                    return true;
                }};

            foreach(const QString& item, *CSVParser::csvToken(line, delimiter)){
                if(!checkFormat(true))return;

                if(CustomJumpNames::JumpDate == index){
                    QDateTime dt = QDateTime::fromString(item, dateFormat);
                    jump_data.push_back(std::make_pair(field_names[index++], dt));
                }
                else
                    jump_data.push_back(std::make_pair(field_names[index++], item));
            }
            if(!checkFormat(false))return;

            jump->setPairs(jump_data);
            jumps->push_back(jump);
        }
        if(!jm.moveItems(jumps))
        {
            log("Error - cannot read file: " + filename);
            return;
        }

        file.close();
        log("File loaded: " + filename);
    }
    else
        log(file.errorString() + " Error - cannot read file: " + filename);
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::setCurrentFile(const QString &fileName)
{
    current_file = fileName;

    QString shownName = current_file;
    if(current_file.isEmpty() || current_file == "")
        shownName = tr("untitled");

    settings.setSetting("current_file", current_file);
    setWindowFilePath(shownName + " - anreader");

    fileWasModified(false);
}

//----------------------------------------------------------------------------------------------------------------------
bool MainWindow::saveQuestion()
{
    if(!file_was_modified)
        return true;

    const QMessageBox::StandardButton mb
        = QMessageBox::warning(this, tr("anreader"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch(mb) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::fileWasModified(bool value)
{
    file_was_modified = value;
    setWindowModified(file_was_modified);
}

//----------------------------------------------------------------------------------------------------------------------
QPointer<StatusFrame> MainWindow::getStatusFrame(const int id)
{
    if(map_status_frames.contains(id))
        return map_status_frames[id];
    return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::createStatusFrame(const int id)
{
    StatusFrame *status_frame;
    if(!map_status_frames.contains(id))
    {
        status_frame = new StatusFrame(this);

        statusBar()->addWidget(status_frame);
        map_status_frames[id] = status_frame;
    }
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::initProgress(const int id, unsigned int value)
{
    QPointer<StatusFrame> status_frame = getStatusFrame(id);
    if(status_frame)
    {
        status_frame->setMinimum(0);
        status_frame->setMaximum(static_cast<int>(value));
        status_frame->setValue(0);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::setStatusText(const int id, const QString &text)
{
    QPointer<StatusFrame> status_frame = getStatusFrame(id);
    if(status_frame)
        status_frame->setStatusText(text);
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::stepProgress(const int id)
{
    QPointer<StatusFrame> status_frame = getStatusFrame(id);
    if(status_frame)
        status_frame->setValue(status_frame->getValue() + 1);
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::finish(const DWidget& widget)
{
    std::unique_ptr<t_rows> jumps = std::make_unique<t_rows>();
    foreach(auto& jump, widget.device().jumps())
       jumps->push_back(jump);

    if(jumps_model.rowCount(QModelIndex()) == 0)
        jumps_model.moveItems(jumps);
    else
        jumps_model.addItems(*jumps);

    prepareTableAfterLoad(*jtable);
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::prepareTableAfterLoad(JumpsTable &table)
{
    table.horizontalHeader()->setStretchLastSection(false);
    table.resizeColumnsToContents();
    table.resizeRowsToContents();
    table.horizontalHeader()->setStretchLastSection(true);
    table.scrollToBottom();
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::newFile()
{
    if(saveQuestion())
    {
        jumps_model.clear();
        setCurrentFile(QString());
    }
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::open()
{
    if(!saveQuestion()) return;

    QString filename;
    QStringList filters;
    QString selectedFilter = defaultFilter;

    filters << commaFilter << defaultFilter;

    QFileDialog fd(this, QObject::tr("Open file..."), settings.getSetting("directory_for_save").toString(), filters.join(";;"));
    fd.selectNameFilter(defaultFilter);

    connect(&fd, &QFileDialog::filterSelected, this, [&selectedFilter](const QString &filter) {selectedFilter = filter; });
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setAcceptMode(QFileDialog::AcceptOpen);

    if(fd.exec())
    {
        filename = fd.selectedFiles().at(0);
        jumps_model.clear();
        openFromCSV(filename, jumps_model, selectedFilter == defaultFilter ? ";" : ",");
        prepareTableAfterLoad(*jtable);
        setCurrentFile(filename);
    }
}

//----------------------------------------------------------------------------------------------------------------------
bool MainWindow::save()
{
    if(jtable->model()->rowCount() > 0)
    {
        if (current_file.isEmpty()) {
            return saveAs();
        }
        else
        {
            if(saveAsCSV(current_file, jumps_model, delimiterCSV()))
            {
                setCurrentFile(current_file);
                return true;
            }
            return false;
        }
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
bool MainWindow::saveAs()
{
    QString filename;
    QStringList filters;
    QString selectedFilter = defaultFilter;

    filters << commaFilter << defaultFilter;

    QFileDialog fd(this, QObject::tr("Save jumps to file..."), settings.getSetting("directory_for_save").toString(), filters.join(";;"));
    fd.selectNameFilter(defaultFilter);

    connect(&fd, &QFileDialog::filterSelected, this, [&selectedFilter](const QString &filter) {selectedFilter = filter; });
    fd.setFileMode(QFileDialog::AnyFile);
    fd.setAcceptMode(QFileDialog::AcceptSave);

    bool fd_result = fd.exec();
    if(fd_result)
    {
        filename = fd.selectedFiles().at(0);
        settings.setSetting("directory_for_save", filename);
        if(saveAsCSV(filename, jumps_model, selectedFilter == defaultFilter ? ";" : ","))
            setCurrentFile(filename);
    }
    return fd_result;
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::delete_selected()
{
    if(jtable)
    {
        while(jtable->selectionModel()->selectedRows().count())
            jumps_model.removeItem(jtable->selectionModel()->selectedRows().at(0).row());
    }
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::edit_selected()
{

}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::about()
{

}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::documentWasModified()
{    
    fileWasModified(true);
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::enableActions(const bool enable)
{
    m_newAct->setEnabled(enable);
    m_saveAct->setEnabled(enable);
    m_openAct->setEnabled(enable);

    int selection_size = (jtable ? jtable->selectionModel()->selectedRows().size() : 0);
    m_editAct->setEnabled(enable && selection_size == 1);
    m_deleteAct->setEnabled(enable && selection_size > 0);
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::log(const QString& value)
{
    qDebug() << value;
    if(log_widget)
        log_widget->add(value);
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::selectionChanged()
{
    int selection_size = (jtable ? jtable->selectionModel()->selectedRows().size() : 0);    
    m_editAct->setEnabled(selection_size == 1);
    m_deleteAct->setEnabled(selection_size > 0);
}





