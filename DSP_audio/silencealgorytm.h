#ifndef SILENCEALGORYTM_H
#define SILENCEALGORYTM_H

#include <QObject>

#include <QMap>
#include <QMutex>
#include <QString>


class SilenceAlgorytm : public QObject
{
    Q_OBJECT

public:
    SilenceAlgorytm();

public slots:
    void onSetSrcPath( const QString& path );

    void onSetDestPath( const QString& path);

    void onSetThreshold( int value );

    void onSetPostSilenceSamples(int time );

    void onRunAlgorytm();

signals:
    void algorytmFinished();

private:
    bool startAlgorytm(const QString &filename);

    bool isSilence(qint16 amplitude);

    bool readFile( const QString &filename );

    bool writeToFile( const QString &filename );

private:
    enum {SIGNED, UNSIGNED};

    //! Данные из файла
    QList<qint16> m_dataResult;

    QMutex m_mutex;

    QString m_destPath;
    QString m_sourcePath;

    uint m_channelCount;
    uint m_sampleRate;
    uint m_sampleSize;
    uint m_sampleType;

    //! Количество сэмплов за время в течении которого уровень ниже порога
    uint m_postSilenceSamples;

    int m_thresholdValue;

    //! Даные для записи в файл-результат
    QMap<uint, uint> m_results;
};

#endif // SILENCEALGORYTM_H
