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

    QVector<int> result;
    result.append(1);

    if (number > 1) {

        for (int i = 2; i < number + 1; i++) {

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
                QThread::sleep(500);
            }

            if (isCancelled) {
                return;
            }
        }
    }

    emit factProgress(1, 1);
    emit factResult(result);
}

void Worker::calcPrime(int from, int to) {

    isPaused = false;
    isCancelled = false;

    QVector<int> result;

    // init array
    bool primes[to + 1];
    for (int i = 0; i < to + 1; i++) {

        primes[i] = true;

        while (isPaused) {
            QThread::sleep(500);
        }

        if (isCancelled) {
            return;
        }
    }

    // filter non-prime numbers
    int progress = 0;
    for (int i = 2; i * i < to + 1; i++) {

        if (primes[i]) {

            for (int j = i * i; j < to + 1; j += i) {

                if (primes[i]) {
                    primes[i] = false;
                    progress++;
                }
            }
        }

        emit primeProgress(progress, to);

        while (isPaused) {
            QThread::sleep(500);
        }

        if (isCancelled) {
            return;
        }
    }

    // return only prime numbers from given range
    for (int i = from; i < to + 1; i++) {

        if (primes[i]) {
            result.append(primes[i]);
            progress++;
        }

        emit primeProgress(progress, to);

        while (isPaused) {
            QThread::sleep(500);
        }

        if (isCancelled) {
            return;
        }
    }

    emit primeProgress(1, 1);
    emit primeResult(result);
}
