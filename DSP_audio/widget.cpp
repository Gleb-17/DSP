#include "widget.h"

#include "silencealgorytm.h"

#include <QBoxLayout>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_silenceAlgorytm( new SilenceAlgorytm )
    , m_lEsource( new QLineEdit )
    , m_lEDest( new QLineEdit )
    , m_pBStart( new QPushButton(tr("Start algorytm")) )
    , m_isAlgorytmContinued( new QLabel )
{
    setWindowTitle(QString(" "));

    m_silenceAlgorytm->moveToThread(&m_thread);
    connect(&m_thread, &QThread::finished
            , m_silenceAlgorytm, &SilenceAlgorytm::deleteLater);

    m_thread.start();

    initGui();
}

Widget::~Widget()
{
    m_thread.quit();
    m_thread.wait();
}

void Widget::initGui()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QGroupBox* gbSrc = new QGroupBox(tr("Choose source path"));
    QHBoxLayout * sourceLayout = new QHBoxLayout;
    gbSrc->setLayout( sourceLayout );

    QPushButton* pBSource = new QPushButton(tr("Choose"));

    sourceLayout->addWidget(m_lEsource);
    sourceLayout->addWidget(pBSource);


    QGroupBox* gbDest = new QGroupBox(tr("Choose destination path"));
    QHBoxLayout * destLayout = new QHBoxLayout;
    gbDest->setLayout( destLayout );

    QPushButton* pBDest = new QPushButton(tr("Choose"));

    destLayout->addWidget(m_lEDest);
    destLayout->addWidget(pBDest);


    QGroupBox* gBAlgParamas = new QGroupBox(tr("Parametres of algorytm"));
    QFormLayout* formLayout = new QFormLayout;
    gBAlgParamas->setLayout(formLayout);

    QSpinBox *sbThrshold = new QSpinBox;
    sbThrshold->setRange( -1000, 1000);
    sbThrshold->setButtonSymbols(QAbstractSpinBox::NoButtons);

    m_isAlgorytmContinued->setAlignment(Qt::AlignCenter);

    formLayout->addRow(tr("Threshold level (silence), dB"), sbThrshold);
    formLayout->addRow( m_pBStart );
    formLayout->addRow( m_isAlgorytmContinued );


    mainLayout->addWidget(gbSrc);
    mainLayout->addWidget(gbDest);
    mainLayout->addWidget(gBAlgParamas);


    connect(pBSource, &QPushButton::clicked, this, &Widget::onChangeSourcePath);
    connect(pBDest, &QPushButton::clicked, this, &Widget::onChangeDestPath);
    connect(m_pBStart, &QPushButton::clicked, this, &Widget::onAlgorytmStarted);
    connect(this, &Widget::runAlgorytm
            , m_silenceAlgorytm, &SilenceAlgorytm::onRunAlgorytm);
    connect(this, &Widget::setSrcPath
            , m_silenceAlgorytm, &SilenceAlgorytm::onSetSrcPath);
    connect(this, &Widget::setDestPath
            , m_silenceAlgorytm, &SilenceAlgorytm::onSetDestPath);
    connect( m_silenceAlgorytm, &SilenceAlgorytm::algorytmFinished
             , this, &Widget::onAlgorytmFinished);

    connect(sbThrshold, SIGNAL(valueChanged(int))
            , m_silenceAlgorytm, SLOT(onSetThreshold(int)));


    QDir dirSrc("./DATA/input");
    if( !dirSrc.exists())
        qDebug() << Q_FUNC_INFO << dirSrc.mkpath(".");

    m_lEsource->setText(dirSrc.absolutePath());
    emit setSrcPath(m_lEsource->text());

    QDir dirDest("./DATA/output");
    if( !dirDest.exists())
        qDebug() << Q_FUNC_INFO << dirDest.mkpath(".");

    m_lEDest->setText(dirDest.absolutePath());
    emit setDestPath(m_lEDest->text());

}

void Widget::onAlgorytmStarted()
{
    m_isAlgorytmContinued->setText(tr("Algorytm is started"));
    m_pBStart->setEnabled(false);
    emit runAlgorytm();
}

void Widget::onAlgorytmFinished()
{
    m_pBStart->setEnabled(true);
    m_isAlgorytmContinued->setText(QString());
}

void Widget::onChangeSourcePath()
{
    QString sourcePath = QFileDialog::getExistingDirectory(this
                                                     , tr("Choose directory")
                                                     , "."
                                                     );
    if( sourcePath.isEmpty() )
        return;

    emit setSrcPath(sourcePath);
    m_lEsource->setText(sourcePath);
}

void Widget::onChangeDestPath()
{
    QString destPath = QFileDialog::getExistingDirectory(this
                                                     , tr("Choose directory")
                                                     , "."
                                                     );
    if( destPath.isEmpty() )
        return;

    emit setDestPath(destPath);
    m_lEDest->setText(destPath);
}

