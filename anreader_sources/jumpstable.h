#ifndef JUMPSTABLE_H
#define JUMPSTABLE_H

#include <QTableView>


class JumpsTable : public QTableView
{
    QString m_NoDataToDisplayInfoText = tr("No data to display");
protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent* event) override;
public:
    JumpsTable();

    void setNoDataToDisplayInfoText(const QString &value) { m_NoDataToDisplayInfoText = value; }
};

#endif // JUMPSTABLE_H
