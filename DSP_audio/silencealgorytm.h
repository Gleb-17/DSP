#ifndef SILENCEALGORYTM_H
#define SILENCEALGORYTM_H

#include <QObject>

#include <QMap>
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

    void onRunAlgorytm();

signals:
    void algorytmFinished();

private:
    bool startAlgorytm(const QString &filename);

    bool isSilence(qint16 amplitude);

    bool writeToFile( const QString &filename );

private:
    enum {SIGNED, UNSIGNED};

    QString m_destPath;
    QString m_sourcePath;

    uint m_channelCount;
    uint m_sampleRate;
    uint m_sampleSize;
    uint m_sampleType;

    uint m_startSilencePos;

    int m_thresholdValue;

    QMap<uint, uint> m_results;
};

#endif // SILENCEALGORYTM_H
