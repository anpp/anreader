#include "log_widget.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDateTime>

//----------------------------------------------------------------------------------------------------------------------
LogWidget::LogWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    m_text_log = new QTextEdit(this);
    main_layout->addWidget(m_text_log);
    main_layout->setMargin(0);

    m_text_log->setReadOnly(true);
}

//---------------------------------------------------------------------------------------------------------------------
void LogWidget::add(const QString &value)
{
    if(!m_text_log) return;
    QString date_color = "#0000ff";
    QString error_color = "#ff0000";
    QString warning_color = "#964b00";
    QString hint_color = "#ff00ff";
    QString color;
    QString message = value;


    if(message.contains("warning", Qt::CaseInsensitive))
        color = warning_color;
    if(message.contains("error", Qt::CaseInsensitive))
        color = error_color;
    if(message.contains("hint", Qt::CaseInsensitive))
        color = hint_color;

    if(!color.isEmpty())
        message = "<FONT color=" + color + ">" + message + "</FONT>";

    QString value_to_log = value == "" ? message :
                                         "<FONT color=" + date_color + ">" + "[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "]</FONT> " + message;

    m_text_log->append(value_to_log);
}
