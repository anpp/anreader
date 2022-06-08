#ifndef N3_NAMES_DIALOG_H
#define N3_NAMES_DIALOG_H

#include <QDialog>
#include <memory>

namespace Ui {
class N3NamesDialog;
}

class N3Names;

class N3NamesDialog : public QDialog
{
    Q_OBJECT

    const N3Names& m_n3names;
    std::unique_ptr<N3Names> m_new_n3names;
    QByteArray raw_names;
    QString m_title;
    int m_active_index = -1;
    int m_new_active_index = -1;

    int m_active_index2 = -1; //for signals - two active (freefall and swoop)
    int m_new_active_index2 = -1;

public:
    explicit N3NamesDialog(const QString& title, const N3Names& names, int aindex, int aindex2,  QWidget *parent = nullptr);
    ~N3NamesDialog();

private:
    Ui::N3NamesDialog *ui;
};

#endif // N3_NAMES_DIALOG_H
