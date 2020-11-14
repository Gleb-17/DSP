#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void initGui();

private:
    QString m_sourcePath;

    QString m_destPath;
};

#endif // WIDGET_H
