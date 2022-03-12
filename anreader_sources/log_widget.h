#ifndef LOG_WIDGET_H
#define LOG_WIDGET_H

#include <QWidget>
#include <QPointer>


class QTextEdit;

enum LogWidget_defs: int {MinimumHeight = 20};

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogWidget(QWidget *parent = nullptr);

    QSize sizeHint() const
    {
        QSize sh = QWidget::sizeHint();
        sh.setHeight(LogWidget_defs::MinimumHeight);
        return sh;
    }

private:
    QPointer<QTextEdit> m_text_log;
signals:

public slots:
    void add(const QString& value);
};

#endif // LOG_WIDGET_H
