#ifndef N3_NAMES_DIALOG_H
#define N3_NAMES_DIALOG_H

#include <QDialog>
#include <memory>


namespace Ui {
class N3NamesDialog;
}

class N3Names;
class N3NamesModel;
class N3NamesDelegate;

class N3NamesDialog : public QDialog
{
    Q_OBJECT

    const N3Names& m_n3names;
    std::unique_ptr<N3Names> m_new_n3names;
    QByteArray raw_names;
    QString m_title;
    std::unique_ptr<N3NamesModel> m_model;
    std::unique_ptr<N3NamesDelegate> m_delegate;

public:
    explicit N3NamesDialog(const QString& title, const N3Names& names,  QWidget *parent = nullptr);
    ~N3NamesDialog();

private:
    Ui::N3NamesDialog *ui;

private slots:

};

#endif // N3_NAMES_DIALOG_H
