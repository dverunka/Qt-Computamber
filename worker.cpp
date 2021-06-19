#include "worker.h"

#include <QThread>

Worker::Worker()
{
    isPaused = false;
    isCancelled = false;
}

void Worker::calcFact(int number) {

    isPaused = false;
    isCancelled = false;
    emit factProgress(0, 100);

    QVector<int> result;
    result.append(1);

    if (number > 1) {

        for (int i = 2; i <= number; i++) {

            int carry = 0;
            for (int j = 0; j < result.size(); j++) {

                int temp = result[j] * i + carry;
                result[j] = temp % 10;
                carry = temp / 10;
            }

            while (carry > 0) {
                result.append(carry % 10);
                carry = carry / 10;
            }

            emit factProgress(i, number);

            while (isPaused) {
                QThread::msleep(500);
            }

            if (isCancelled) {
                return;
            }
        }
    }

    emit factProgress(100, 100);
    emit factResult(result);
}

void Worker::calcPrime(int from, int to) {

    isPaused = false;
    isCancelled = false;
    emit primeProgress(0, 100);

    QVector<int> result;

    // allocate memory for array, otherwise it could overflow
    bool *primes = (bool*) malloc((to + 1) * sizeof (bool));
    for (int i = 0; i <= to; i++) {

        primes[i] = true;

        while (isPaused) {
            QThread::msleep(500);
        }

        if (isCancelled) {
            free(primes);
            return;
        }
    }

    // filter non-prime numbers
    int progress = 0;
    for (int i = 2; i * i <= to; i++) {

        if (primes[i]) {

            for (int j = i * i; j <= to; j += i) {

                if (primes[j]) {
                    primes[j] = false;
                    progress++;
                }
            }
        }

        emit primeProgress(progress, to);

        while (isPaused) {
            QThread::msleep(500);
        }

        if (isCancelled) {
            free(primes);
            return;
        }
    }

    // return only prime numbers from given range
    for (int i = from; i <= to; i++) {

        if (primes[i]) {

            result.append(i);
            progress++;
        }

        while (isPaused) {
            QThread::msleep(500);
        }

        if (isCancelled) {
            free(primes);
            return;
        }
    }

    // clean memory
    free(primes);

    emit primeProgress(100, 100);
    emit primeResult(result);
}
