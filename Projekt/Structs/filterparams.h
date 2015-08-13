#ifndef FILTERPARAMS_H
#define FILTERPARAMS_H

#include <QObject>

struct FilterParams
{
    int kernel;

    FilterParams() {kernel = 1;}
    FilterParams(int kernel) : kernel(kernel) {}
};

Q_DECLARE_METATYPE(FilterParams)

#endif // FILTERPARAMS_H
