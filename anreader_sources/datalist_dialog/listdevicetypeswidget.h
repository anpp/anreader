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
//#include "delegates/comboeditdelegate.h"
#include "delegates/combo_popupwidget_delegate.h"

class ListDeviceTypesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListDeviceTypesWidget(t_devicetypelist& datalist, QWidget *parent = nullptr);

    const QTableView& listTable() const { return *m_listTable; }
    void setupCurrentRow();

private:
    std::unique_ptr<QTableView> m_listTable = std::make_unique<QTableView>(this);
    DeviceDescriptionListModel m_model;
signals:
    void rowIsEnabled(bool enable);

public slots:
    void addItem();
    void removeFocusedItem();
    void rowChanged();

};

#endif // LISTDEVICETYPESWIDGET_H
