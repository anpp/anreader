#ifndef LISTDEVICETYPESWIDGET_H
#define LISTDEVICETYPESWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QPointer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <memory>

#include "models/devicedescriptionlistmodel.h"
#include "delegates/comboeditdelegate.h"

class ListDeviceTypesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListDeviceTypesWidget(t_devicetypelist& datalist, QWidget *parent = nullptr);

    const QTableView& listTable() const { return *m_listTable; }

private:
    std::unique_ptr<QTableView> m_listTable = std::make_unique<QTableView>(this);
    DeviceDescriptionListModel m_model;
signals:

public slots:
    void addItem();
    void removeFocusedItem();

};

#endif // LISTDEVICETYPESWIDGET_H
