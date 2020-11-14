#include "widget.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDataStream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
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

    QLineEdit* lEsource = new QLineEdit;
    QPushButton* pBSource = new QPushButton(tr("Choose"));

    sourceLayout->addWidget(lEsource);
    sourceLayout->addWidget(pBSource);


    QGroupBox* gbDest = new QGroupBox(tr("Choose destination path"));
    QHBoxLayout * destLayout = new QHBoxLayout;
    gbDest->setLayout( destLayout );

    QLineEdit* lEDest = new QLineEdit;
    QPushButton* pBDest = new QPushButton(tr("Choose"));

    destLayout->addWidget(lEDest);
    destLayout->addWidget(pBDest);


    mainLayout->addWidget(gbSrc);
    mainLayout->addWidget(gbDest);


    QDir dir("./DATA");
    if( !dir.exists())
        qDebug() << Q_FUNC_INFO << dir.mkpath(".");

    lEsource->setText(dir.absolutePath());


}
