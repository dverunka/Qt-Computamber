#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker: public QObject
{
    Q_OBJECT

public:
    Worker();

    bool isPaused;
    bool isCancelled;

public slots:
    void calcFact(int number);
    void calcPrime(int from, int to);

signals:
    void factResult(QVector<int> result);
    void factProgress(int progress, int max);

    void primeResult(QVector<int> result);
    void primeProgress(int progress, int max);
};

#endif // WORKER_H
