#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QLineEdit;

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

    QLineEdit* m_lEsource;
    QLineEdit* m_lEDest;

private slots:
    void onChangeSourcePath();

    void onChangeDestPath();
};

#endif // WIDGET_H
