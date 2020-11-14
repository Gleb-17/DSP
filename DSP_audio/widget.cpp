#include "widget.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QDataStream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_lEsource( new QLineEdit )
    , m_lEDest( new QLineEdit )
{
    setWindowTitle(QString(" "));

    QFile file("D:/Qt_projects/DSP/DSP_audio/input/1-10.bin");

    if (!file.open(QIODevice::ReadOnly) )
        return;

    qint64 filesize = file.size();

    QByteArray buffer;
    buffer.resize(static_cast<int>(filesize));
    QDataStream stream(&file);


    stream.readRawData( buffer.data(), static_cast<int>(filesize));
    //buffer[1] = 34;
    qDebug() << Q_FUNC_INFO << buffer.count();

    //Assign sound samples to short array
    qint16* resultingData = reinterpret_cast<qint16*>(buffer.data());

    QList<qint16> result;
    for ( int i=0; i < buffer.count() / 2; i++ )
    {
        //qDebug() << Q_FUNC_INFO << i;
        result.append( *(resultingData + i) );
    }
    qDebug() << Q_FUNC_INFO << result.count();

    int zeroOccurs = 0;
    for( qint16 value : result )
    {
        if( !value)
        {
            zeroOccurs++;
//            printf("HEY ");
        }
        else
        {
            if(zeroOccurs)
            {
//                if( zeroOccurs > 10 )
//                    printf("%d ", zeroOccurs);
                zeroOccurs = 0;
            }
        }
        //printf("%d ", value);
    }

    initGui();
}

Widget::~Widget()
{

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


    mainLayout->addWidget(gbSrc);
    mainLayout->addWidget(gbDest);



    QDir dir("./DATA/input");
    if( !dir.exists())
        qDebug() << Q_FUNC_INFO << dir.mkpath(".");

    m_lEsource->setText(dir.absolutePath());
    m_sourcePath = m_lEsource->text();


    connect(pBSource, &QPushButton::clicked, this, &Widget::onChangeSourcePath);
    connect(pBDest, &QPushButton::clicked, this, &Widget::onChangeDestPath);

}

void Widget::onChangeSourcePath()
{
    QString sourcePath = QFileDialog::getExistingDirectory(this
                                                     , tr("Choose directory")
                                                     , "."
                                                     );
    if( sourcePath.isEmpty() )
        return;

    m_sourcePath = sourcePath;
    m_lEsource->setText(m_sourcePath);
}

void Widget::onChangeDestPath()
{
    QString destPath = QFileDialog::getExistingDirectory(this
                                                     , tr("Choose directory")
                                                     , "."
                                                     );
    if( destPath.isEmpty() )
        return;

    m_destPath = destPath;
    m_lEDest->setText(m_destPath);
}
