#include "mainwindow.h"
#include "csvparser.h"

const static QString sDataList_Titles[] = {QObject::tr("Aircrafts"), QObject::tr("Dropzones"), QObject::tr("Canopies"), QObject::tr("Jump types")};

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
    : QMainWindow(parent),
      jumps_model(dl)
{

    this->resize(MainWindow_defs::BaseWidht, MainWindow_defs::BaseHeight);    
    setWindowIcon(QIcon(":/images/icons/main/anreader.ico"));
    //QApplication::setStyle("fusion");
    initMainWindow();
    createActions();    

    settings.load();
    dl.load();

    timer_open_file.setInterval(100);
    connect(&timer_open_file, &QTimer::timeout, this, &MainWindow::onCreate);
    timer_open_file.start();
}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{    
    for(auto& act: m_actions)
        delete act;
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::onCreate()
{
    timer_open_file.stop();
    disconnect(&timer_open_file, &QTimer::timeout, this, &MainWindow::onCreate);

    //для некоторых оконных систем
    #ifndef Q_OS_WIN64
        #ifndef Q_OS_WIN32
            #ifndef Q_OS_MACOS
                settings.loadSettingsScreen();
            #endif
        #endif
    #endif

    setCurrentFile(settings.getSetting("current_file").toString());
    if(!settings.getSetting("current_file").toString().isEmpty())
    {
        if(openFromCSV(settings.getSetting("current_file").toString(), jumps_model, delimiterCSV()))
        {
            prepareTableAfterLoad(*jtable);
            setCurrentFile(current_file);
        }
        else
            setCurrentFile("");
    }
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

    jtable = std::make_unique<JumpsTable>();
    jtable->setModel(&jumps_model);

    connect(&jumps_model, &JumpsTableModel::dataChanged, this, &MainWindow::documentWasModified);
    connect(&jumps_model, &JumpsTableModel::rowsInserted, this, &MainWindow::documentWasModified);
    connect(&jumps_model, &JumpsTableModel::rowsRemoved, this, &MainWindow::documentWasModified);    
    connect(jtable->selectionModel(), &QItemSelectionModel::selectionChanged,  this, &MainWindow::selectionChanged);
    connect(jtable.get(), &QTableView::doubleClicked, this, &MainWindow::table_doubleClicked);

    createDevicesWidget();    
    createLogWidget();

    layout->addWidget(jtable.get());

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
    QMenu *registerMenu = menuBar()->addMenu(tr("&Registry"));
    QMenu *windowsMenu = menuBar()->addMenu(tr("&Windows"));

    const QIcon newIcon = QIcon(":/images/icons/toolbar/new.png");
    m_newAct = new QAction(newIcon, tr("&New"), this);
    m_actions.push_back(m_newAct);
    m_newAct->setShortcuts(QKeySequence::New);
    m_newAct->setToolTip(tr("Create a new file"));
    connect(m_newAct, &QAction::triggered, this, &MainWindow::newFile);

    const QIcon openIcon = QIcon(":/images/icons/toolbar/open.png");
    m_openAct = new QAction(openIcon, tr("&Open..."), this);
    m_actions.push_back(m_openAct);
    m_openAct->setShortcuts(QKeySequence::Open);
    m_openAct->setToolTip(tr("Open an existing file"));
    connect(m_openAct, &QAction::triggered, this, &MainWindow::open);    

    const QIcon saveIcon = QIcon(":/images/icons/toolbar/save.png");
    m_saveAct = new QAction(saveIcon, tr("&Save..."), this);
    m_actions.push_back(m_saveAct);
    m_saveAct->setShortcuts(QKeySequence::Save);
    m_saveAct->setToolTip(tr("Save the document to disk"));
    connect(m_saveAct, &QAction::triggered, this, &MainWindow::save);

    m_saveAsAct = new QAction(saveIcon, tr("Save &As..."), this);
    m_actions.push_back(m_saveAsAct);
    m_saveAsAct->setShortcuts(QKeySequence::SaveAs);
    m_saveAsAct->setToolTip(tr("Save the document under a new name"));
    connect(m_saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);

    const QIcon copyIcon = QIcon(":/images/icons/toolbar/copy.png");
    m_copyAct = new QAction(copyIcon, tr("&Copy"), this);
    m_actions.push_back(m_copyAct);
    m_copyAct->setShortcuts(QKeySequence::Copy);
    m_copyAct->setToolTip(tr("Copy selected jumps"));
    //m_copyAct->setShortcutVisibleInContextMenu(true);
    connect(m_copyAct, &QAction::triggered, this, &MainWindow::copy_selected);

    const QIcon editIcon = QIcon(":/images/icons/toolbar/edit.png");
    m_editAct = new QAction(editIcon, tr("&Edit"), this);
    m_actions.push_back(m_editAct);
    m_editAct->setShortcuts(QKeySequence::UnknownKey);
    m_editAct->setToolTip(tr("Edit selected jump..."));
    connect(m_editAct, &QAction::triggered, this, &MainWindow::edit_selected);   

    const QIcon deleteIcon = QIcon(":/images/icons/toolbar/delete.png");
    m_deleteAct = new QAction(deleteIcon, tr("&Delete"), this);
    m_actions.push_back(m_deleteAct);
    m_deleteAct->setShortcuts(QKeySequence::Delete);
    m_deleteAct->setToolTip(tr("Delete selected jumps"));
    connect(m_deleteAct, &QAction::triggered, this, &MainWindow::delete_selected);

    const QIcon aircraftsIcon = QIcon(":/images/icons/menu/airplane.png");
    m_registerAicraftsAct = new QAction(aircraftsIcon, tr("Aircrafts"), this);
    m_actions.push_back(m_registerAicraftsAct);
    m_registerAicraftsAct->setToolTip(tr("Aircrafts..."));
    connect(m_registerAicraftsAct, &QAction::triggered, this, &MainWindow::aicrafts_list);

    const QIcon dropzonesIcon = QIcon(":/images/icons/menu/dz.png");
    m_registerDropZonesAct = new QAction(dropzonesIcon, tr("Dropzones"), this);
    m_actions.push_back(m_registerDropZonesAct);
    m_registerDropZonesAct->setToolTip(tr("Dropzones..."));
    connect(m_registerDropZonesAct, &QAction::triggered, this, &MainWindow::dropzones_list);

    const QIcon canopiesIcon = QIcon(":/images/icons/menu/canopy.png");
    m_registerCanopiesAct = new QAction(canopiesIcon, tr("Canopies"), this);
    m_actions.push_back(m_registerCanopiesAct);
    m_registerCanopiesAct->setToolTip(tr("Canopies..."));
    connect(m_registerCanopiesAct, &QAction::triggered, this, &MainWindow::canopies_list);    

    const QIcon jumptypesIcon = QIcon(":/images/icons/menu/jump.png");
    m_registerJumpTypesAct = new QAction(jumptypesIcon, tr("Jump types"), this);
    m_actions.push_back(m_registerJumpTypesAct);
    m_registerJumpTypesAct->setToolTip(tr("Jump types..."));
    connect(m_registerJumpTypesAct, &QAction::triggered, this, &MainWindow::jumptypes_list);


    QToolBar *mainToolBar = addToolBar(tr("File"));
    mainToolBar->setIconSize(QSize(40, 40));
    mainToolBar->setObjectName("file toolbar");
    mainToolBar->addAction(m_newAct);
    mainToolBar->addAction(m_openAct);
    mainToolBar->addAction(m_saveAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(m_copyAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(m_editAct);
    mainToolBar->addAction(m_deleteAct);

    fileMenu->addAction(m_newAct);
    fileMenu->addAction(m_openAct);
    fileMenu->addAction(m_saveAct);
    fileMenu->addAction(m_saveAsAct);

    editMenu->addAction(m_copyAct);
    editMenu->addSeparator();
    editMenu->addAction(m_editAct);
    editMenu->addAction(m_deleteAct);

    registerMenu->addAction(m_registerAicraftsAct);
    registerMenu->addAction(m_registerDropZonesAct);
    registerMenu->addAction(m_registerCanopiesAct);
    registerMenu->addAction(m_registerJumpTypesAct);

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
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_toggleDevices = dock->toggleViewAction();
#ifndef Q_OS_MACOS
    m_toggleDevices->setShortcut(QKeySequence("F11"));
#else
    m_toggleDevices->setShortcut(QKeySequence("Ctrl+F11"));
#endif

    devices_window = std::make_unique<DevicesWidget>(dock);
    dock->setWidget(devices_window.get());
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    connect(devices_window.get(), &DevicesWidget::createStatusFrame, this, &MainWindow::createStatusFrame);
    connect(devices_window.get(), &DevicesWidget::newTextOfState, this, &MainWindow::setStatusText);
    connect(devices_window.get(), &DevicesWidget::setProgress, this, &MainWindow::initProgress);
    connect(devices_window.get(), &DevicesWidget::stepProgress, this, &MainWindow::stepProgress);
    connect(devices_window.get(), &DevicesWidget::receivedData, this, &MainWindow::finish);
    connect(devices_window.get(), &DevicesWidget::afterConnect, this, &MainWindow::afterConnect);
    connect(devices_window.get(), &DevicesWidget::controls_is_enabled, this, &MainWindow::enableActions);
    connect(devices_window.get(), &DevicesWidget::log, this, &MainWindow::log);
    connect(devices_window.get(), &DevicesWidget::giveLastJump, &jumps_model, &JumpsTableModel::takeLastJump);

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

    log_widget = std::make_unique<LogWidget>(dock);
    log_widget->setMinimumHeight(LogWidget_defs::MinimumHeight);
    dock->setWidget(log_widget.get());

    addDockWidget(Qt::BottomDockWidgetArea, dock);
}

//----------------------------------------------------------------------------------------------------------------------
bool MainWindow::saveAsCSV(const QString& filename, const JumpsTableModel& jm, const QString delimiter)
{
    settings.setSetting("current_file_delimiter", delimiter);

    const auto& data = jm.items();
    if(!data.size()) return false;

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
        std::unique_ptr<t_jump_attribute> j_atr;
        QStringList field_names;

        j_atr = data[0]->getPairs(); //из первой записи считаем названия полей        
        for(const auto& fields: *j_atr)
        {
            field_names << fields.first;
            csv_string << "\"" + fields.first+ "\"";
        }

        ts << csv_string.join(delimiter) + "\n";

        for(const auto& jump: data)
        {
            csv_string.clear();
            j_atr = jump->getPairs();

            for(uint i = 0; i < j_atr->size(); ++i)
            {
                QString csv_field;
                int inner_index = N3Jump::index(field_names[i]);
                switch(inner_index)
                {
                case CustomJumpNames::JumpDate:
                    csv_string << "\"" + jump->getJumpDate().toString(dateFormat) + "\"";
                    break;
                case CustomJumpNames::Deleted:
                    csv_field = "";
                    csv_field = jump->isDeleted() ? "1": "0";
                    csv_string << ("\"" + csv_field + "\"");
                    break;
                default:
                    csv_field = j_atr->at(i).second.toString();
                    csv_field = csv_field.replace("\"", "\"\"");
                    csv_field = csv_field.replace("\n", "\t"); //переводы строк заменяются на табы
                    csv_string << "\"" + csv_field + "\"";
                    break;
                }
            }
            ts << csv_string.join(delimiter) + "\n";
        }
        f.close();
        log("File saved: " + filename);
        return true;
    }
    else
    {
        log(f.errorString() + " Error - cannot write file: " + filename + (owner.isEmpty() ? "" : ". File owner is " + owner));
        return false;
    }
}


//----------------------------------------------------------------------------------------------------------------------
bool MainWindow::openFromCSV(const QString &filename, JumpsTableModel& jm, const QString delimiter)
{
    std::unique_ptr<QStringList> tokens;

    settings.setSetting("current_file_delimiter", delimiter);

    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        QString header = in.readLine();
        if(header == ""){
            log("Error - file format: " + filename);
            return false;
        }

        QStringList field_names;
        tokens = CSVParser::csvToken(header, delimiter);
        for(const auto& item: qAsConst(*tokens)){
            field_names << item;
        }

        if(field_names.count() < 3){
            log("Error - file format: " + filename);
            return false;
        }


        std::unique_ptr<t_rows> jumps = std::make_unique<t_rows>();
        while (!in.atEnd())
        {
            QString line = in.readLine();

            //чтение ячеек с переводами строк. Пока не нужно, вместо переводов табы
            //while(line.size() > 0 && '"' != line[line.size() - 1])
            //    line += '\n' + in.readLine();

            t_jump_attribute jump_data;
            std::shared_ptr<CustomJump> jump = std::make_shared<N3Jump>();

            int index = 0;
            int inner_index = 0;
            auto checkFormat { [&] (bool greater) -> bool
                {
                    if((index >= field_names.count() && greater) || (index < field_names.count() && !greater)){
                        log("Error - file format: " + filename);
                        return false;
                    }
                    return true;
                }};

            tokens = CSVParser::csvToken(line, delimiter);
            for(const auto& item: qAsConst(*tokens))
            {
                if(!checkFormat(true)) return false;
                inner_index = N3Jump::index(field_names[index]);

                QDateTime dt;
                QString note;

                switch (inner_index)
                {
                case CustomJumpNames::JumpDate:
                    dt = QDateTime::fromString(item, dateFormat);
                    if(dt.isNull() || !dt.isValid())
                        dt = QDateTime::fromString(item, dateFormat_excel);
                    jump_data.push_back(std::make_pair(field_names[index], dt));
                    break;
                case CustomJumpNames::Note:
                    note = item;
                    jump_data.push_back(std::make_pair(field_names[index], note.replace("\t", "\n")));
                    break;
                default:
                    jump_data.push_back(std::make_pair(field_names[index], item.trimmed()));
                    break;
                }
                index++;
            }
            if(!checkFormat(false)) return false;

            jump->setPairs(jump_data);
            jumps->push_back(jump);
            initDataInLists(jump->getAC(), jump->getDZ(), jump->getCanopy());
        }
        if(!jm.moveItems(jumps))
        {
            log("Error - cannot read file: " + filename);
            return false;
        }

        file.close();
        log("File loaded: " + filename);        
        return true;
    }
    else
        log(file.errorString() + " Error - cannot read file: " + filename);
    return false;
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
void MainWindow::open_DataListDialog(const datakind dlk, map_DataList &data)
{
    QString def_sting;

    auto jump_property = [&def_sting](const CustomJump& jump, const datakind dk) -> const QString&
    {
        switch (static_cast<int>(dk)) {
        case (static_cast<int>(datakind::aircrafts)): return jump.getAC();
        case (static_cast<int>(datakind::dropzones)): return jump.getDZ();
        case (static_cast<int>(datakind::canopies)): return jump.getCanopy();
        case (static_cast<int>(datakind::jump_types)): return jump.getType();
        default:
            return def_sting;

        }
    };

    t_datalist datalist;

    for(const auto& item: data)
        datalist.push_back(std::make_tuple(false, item.first, item.second));

    for(auto& item: datalist)
        std::get<DataListModel_defs::Used>(item) =  (std::find_if(jumps_model.items().begin(), jumps_model.items().end(), [&] (const ptr_jump& jump)
        { return jump_property(*jump, dlk) == std::get<DataListModel_defs::Key>(item); }) != jumps_model.items().end() ||  std::get<DataListModel_defs::Key>(item) == "");


    std::unique_ptr<DataList_Dialog> dl_dialog = std::make_unique<DataList_Dialog>(sDataList_Titles[static_cast<uint>(dlk)], datalist, this);

    if(dl_dialog->exec())
    {
        data.clear();
        for(const auto& item: datalist)
            data[std::get<DataListModel_defs::Key>(item).trimmed()] = std::get<DataListModel_defs::Value>(item);
        dl.save();
        prepareTableAfterEdit(*jtable);
    }
}


//----------------------------------------------------------------------------------------------------------------------
QPointer<StatusFrame> MainWindow::getStatusFrame(const int id)
{
    if(map_status_frames.find(id) != map_status_frames.end())
        return map_status_frames[id].get();
    return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::createStatusFrame(const int id)
{
    std::unique_ptr<StatusFrame> status_frame;
    if(map_status_frames.find(id) == map_status_frames.end())
    {
        status_frame = std::make_unique<StatusFrame>(this);
        statusBar()->addWidget(status_frame.get());
        map_status_frames[id] = std::move(status_frame);
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
    for(const auto& jump: widget.device().jumps())
    {
       jumps->push_back(jump);
    }

    if(jumps_model.rowCount() == 0)
        jumps_model.moveItems(jumps);
    else
        jumps_model.addItems(*jumps);

    jumps_model.sort_jumps();
    prepareTableAfterLoad(*jtable);
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::afterConnect(const DWidget &widget)
{
    for(auto& ap: widget.device().airplanes().Names())
        initDataInLists(ap, "", "");

    for(auto& dz: widget.device().dropzones().Names())
        initDataInLists("", dz, "");

    dl.save();
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::prepareTableAfterEdit(JumpsTable &table) const
{ 
    table.horizontalHeader()->setStretchLastSection(false);
    table.resizeColumnsToContents();
    table.resizeRowsToContents();
    table.horizontalHeader()->setStretchLastSection(true);
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::prepareTableAfterLoad(JumpsTable &table) const
{
    prepareTableAfterEdit(table);
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
        if(openFromCSV(filename, jumps_model, selectedFilter == defaultFilter ? ";" : ","))
        {
            prepareTableAfterLoad(*jtable);
            setCurrentFile(filename);
        }
        else
            setCurrentFile("");
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
void MainWindow::copy_selected()
{
    QClipboard *clipboard = QApplication::clipboard();
    QString rows = "";

    const auto& data = jumps_model.items();
    if(!data.size()) return;

    uint selection_size = (jtable ? jtable->selectionModel()->selectedRows().size() : 0);
    if(selection_size > 0)
    {
        std::unique_ptr<t_jump_attribute> j_atr;
        j_atr = data[0]->getPairs(); //из первой записи считаем названия полей

        QStringList field_names;
        for(const auto& fields: *j_atr)
            field_names << fields.first;

        uint n_first_item = jtable->selectionModel()->selectedRows().at(0).row();

        for(uint i = n_first_item; i < selection_size + n_first_item; ++i)
        {
            const auto& jump = data[i];
            {
                j_atr = jump->getPairs();

                for(size_t j = 0; j < j_atr->size(); ++j)
                {
                    rows += (j > 0)? "\t": "";

                    int inner_index = N3Jump::index(field_names[j]);
                    const datakind dk = (CustomJumpNames::AC == inner_index ? datakind::aircrafts : (CustomJumpNames::DZ == inner_index ? datakind::dropzones : datakind::canopies));
                    QString map_value;

                    switch(inner_index)
                    {
                    case CustomJumpNames::JumpDate:
                        rows += jump->getJumpDate().toString(dateFormat);
                        break;
                    case CustomJumpNames::Deleted:
                        rows += jump->isDeleted() ? "1": "0";
                        break;
                    case CustomJumpNames::AC:
                    case CustomJumpNames::Canopy:
                    case CustomJumpNames::DZ:
                        map_value = dl.mappedValue(dk, j_atr->at(j).second.toString());
                        rows += map_value;
                        break;

                    default:
                        rows += "\"" + j_atr->at(j).second.toString() + "\"";
                        break;
                    }
                }
            }
            rows += "\n";
        }
    }
    if(!rows.isEmpty())
        clipboard->setText(rows);
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::edit_selected()
{
    if(!jtable) return;

    if(jtable->selectionModel()->selectedRows().count() == 1)
    {
        auto currentRow  = jtable->currentIndex().row();
        editJump(currentRow);
    }
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
    for(auto& action: m_actions)
        action->setEnabled(enable);

    if(jtable) jtable->setEnabled(enable);

    selection_changed(enable);
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::log(const QString& value)
{
    qDebug() << value;
    if(log_widget)
        log_widget->add(value);
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::selection_changed(bool enable)
{
    int selection_size = (jtable ? jtable->selectionModel()->selectedRows().size() : 0);
    m_editAct->setEnabled(enable && selection_size == 1);
    m_deleteAct->setEnabled(enable && selection_size > 0);
    m_copyAct->setEnabled(enable && selection_size > 0);
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::editJump(uint row_index)
{
    std::shared_ptr<N3Jump> edit_jump = std::dynamic_pointer_cast<N3Jump>(jumps_model.getItem(row_index));
    if(edit_jump)
    {
        std::unique_ptr<N3JumpEditor> n3_jump_editor = std::make_unique<N3JumpEditor>(this, *edit_jump, dl);
        //n3_jump_editor->setAttribute(Qt::WA_DeleteOnClose);
        if(n3_jump_editor->exec() == QDialog::Accepted && n3_jump_editor->isModified())
        {
            prepareTableAfterEdit(*jtable);
            documentWasModified();
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::initDataInLists(const QString &aircraft, const QString &dropzone, const QString &canopy)
{
    if(aircraft != "" && dl.aircrafts().find(aircraft) == dl.aircrafts().end())
        dl.aircrafts()[aircraft] = "";

    if(dropzone != "" && dl.dropzones().find(dropzone) == dl.dropzones().end())
        dl.dropzones()[dropzone] = "";

    if(canopy != "" && dl.canopies().find(canopy) == dl.canopies().end())
        dl.canopies()[canopy] = "";

}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::selectionChanged()
{
    selection_changed(true);
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::table_doubleClicked(const QModelIndex& index)
{
    if(index.isValid())
        editJump(index.row());
}





