#ifndef N3NAMESDELEGAT_H
#define N3NAMESDELEGAT_H

#include <QItemDelegate>

class N3NamesDelegate : public QItemDelegate
{
    Q_OBJECT

    enum class ModelType: unsigned {Unk = 0, N3Names = 1, N3AlarmsSettings = 2};

    const QString button_add = tr("Add");
public:
    N3NamesDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    QWidget *createEditorN3Names(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditorN3AlarmsSettings(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorDataN3Names(QWidget *editor, const QModelIndex &index) const;
    void setEditorDataN3AlarmsSettings(QWidget *editor, const QModelIndex &index) const;
    void setModelDataN3Names(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setModelDataN3AlarmsSettings(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paintN3Names(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paintN3AlarmsSettings(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void drawPrimitive(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, QStyle::PrimitiveElement pe) const;
    QRect calcRect(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    ModelType modelType(const QAbstractItemModel& model) const;

private slots:
    void radio_toggled(bool value);
    void check_toggled(bool value);
    void combo_changed(const QString& value);
    void add();
    void del();
};

#endif // N3NAMESDELEGAT_H
