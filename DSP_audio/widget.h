#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QThread>

class SilenceAlgorytm;

class QLabel;
class QLineEdit;
class QPushButton;
class QThread;


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

signals:
    void setSrcPath( const QString& path );

    void setDestPath( const QString& path);

    void runAlgorytm();

private:
    void initGui();

private:
    QThread m_thread;
    SilenceAlgorytm* m_silenceAlgorytm;


    QLineEdit* m_lEsource;
    QLineEdit* m_lEDest;

    QPushButton* m_pBStart;

    QLabel* m_isAlgorytmContinued;

private slots:
    void onAlgorytmStarted();

    void onAlgorytmFinished();

    void onChangeSourcePath();

    void onChangeDestPath();

};

#endif // WIDGET_H
