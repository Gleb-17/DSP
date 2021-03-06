#include "silencealgorytm.h"


#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>

#include <cmath>


SilenceAlgorytm::SilenceAlgorytm()
    : m_channelCount( 1 )
    , m_sampleRate( 44100 )
    , m_sampleSize( 16 )
    , m_sampleType(SIGNED)
    , m_preSilenceSamples( m_sampleRate * 20 / 1000 )
    , m_postSilenceSamples( m_sampleRate * 100 / 1000 )
    , m_thresholdValue( 0 )
    , isSilenceFound( false )
    , isCheckStartSilencePosition( false )
    , isCheckStopSilencePosition( false )
{

}

void SilenceAlgorytm::onSetSrcPath(const QString &path)
{
    m_sourcePath = path;
}

void SilenceAlgorytm::onSetDestPath(const QString &path)
{
    m_destPath = path;
}

void SilenceAlgorytm::onSetThreshold(int value)
{
    m_thresholdValue = value;
}

void SilenceAlgorytm::onSetPreSilenceSamples(int time)
{
    m_preSilenceSamples = m_sampleRate * time / 1000;
}

void SilenceAlgorytm::onSetPostSilenceSamples(int time)
{
    m_postSilenceSamples = m_sampleRate * time / 1000;
}

void SilenceAlgorytm::onRunAlgorytm()
{
    if(m_sourcePath.isEmpty())
        return;

    QDir dir(m_sourcePath);

    QStringList fileList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    foreach( QString filename, fileList )
    {
        startAlgorytm(filename);
    }

    emit algorytmFinished();
}

bool SilenceAlgorytm::startAlgorytm(const QString& filename)
{
    QMutexLocker mL( &m_mutex );

    if( !readFile(filename) )
        return false;

    //qDebug() << Q_FUNC_INFO << result.count();

    uint startSilencePos = 0;
    m_results.clear();
    uint preSilenceSamples = 0;
    uint postSilenceSamples = 0;

    for( int i = 0; i < m_dataResult.count(); ++i )
    {
        if( isSilence(m_dataResult.at(i)) )
        {
            if( !isCheckStartSilencePosition && !isSilenceFound )
            {
                isCheckStartSilencePosition = true;
                preSilenceSamples = m_preSilenceSamples;
            }
            else if( isCheckStartSilencePosition )
            {
                if( preSilenceSamples )
                    --preSilenceSamples;

                if( !preSilenceSamples )
                {
                    isSilenceFound = true;
                    isCheckStartSilencePosition = false;
                    startSilencePos = i;

                    preSilenceSamples = 0;
                }
            }
            else if( isCheckStopSilencePosition )
            {
                isCheckStopSilencePosition = false;
                postSilenceSamples = 0;
            }
        }
        else
        {
            if( isSilenceFound )
            {
                if( !isCheckStopSilencePosition )
                {
                    isCheckStopSilencePosition = true;
                    postSilenceSamples = m_postSilenceSamples;
                }
                else
                {
                    if( postSilenceSamples )
                        --postSilenceSamples;

                    if( !postSilenceSamples )
                    {
                        uint silenceStart = ((double)startSilencePos / m_sampleRate) * 1000;
                        uint silenceDuration =
                                ((double)(i - startSilencePos) / m_sampleRate) * 1000;

                        m_results.insert(silenceStart, silenceDuration);

                        isSilenceFound = false;
                        isCheckStopSilencePosition = false;
                        postSilenceSamples = 0;
                    }
                }
            }
            else if( isCheckStartSilencePosition )
            {
                isCheckStartSilencePosition = false;
                preSilenceSamples = 0;
            }
        }
    }

    //last silence element check
    if( isSilenceFound )
    {
        uint silenceStart = ((double)startSilencePos / m_sampleRate) * 1000;
        uint silenceDuration =
                ((double)(m_dataResult.count() - 1 - startSilencePos) / m_sampleRate) * 1000;

        m_results.insert(silenceStart, silenceDuration);
    }

    bool result = writeToFile(filename);

    return result;
}

bool SilenceAlgorytm::isSilence(qint16 amplitude)
{
    double db = 20 * log10(abs(amplitude));
    //printf("%f ", db);
    return db < m_thresholdValue;
}

bool SilenceAlgorytm::readFile(const QString &filename)
{
    QFile file(m_sourcePath + "/" + filename);

    if (!file.open(QIODevice::ReadOnly) )
        return false;

    qint64 filesize = file.size();

    QByteArray buffer;
    buffer.resize(static_cast<int>(filesize));
    QDataStream stream(&file);


    stream.readRawData( buffer.data(), static_cast<int>(filesize));
    //qDebug() << Q_FUNC_INFO << buffer.count();

    //Assign sound samples to short array
    qint16* resultingData = reinterpret_cast<qint16*>(buffer.data());

    m_dataResult.clear();
    for ( int i=0; i < buffer.count() / 2; i++ )
    {
        m_dataResult.append( *(resultingData + i) );
        //printf("%d ", *(resultingData + i));
    }
    return true;
}

bool SilenceAlgorytm::writeToFile( const QString &filename )
{
    if( m_destPath.isEmpty() )
        return false;

    QFile file( m_destPath + "/" + filename + "_silence_data.txt");

    if (!file.open(QIODevice::WriteOnly) )
        return false;

#if defined Q_OS_WIN
    QString endOfLine = "\r\n";
#elif defined Q_OS_UNIX
    QString endOfLine = "\n";
#endif

    QTextStream stream(&file);
    stream << "start (ms)" << " " << "duration (ms)" << endOfLine;
    for( QMap<uint, uint>::iterator it = m_results.begin(); it != m_results.end(); ++it )
    {
        stream << it.key() << " " << it.value() << endOfLine;
    }
    file.close();

    return true;
}
