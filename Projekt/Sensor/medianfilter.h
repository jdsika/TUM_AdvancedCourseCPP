#ifndef _MEDIANFILTER_H_
#define _MEDIANFILTER_H_

#include <QVector>

/**
 * @brief The MedianFilter class will filter data with an
 * median filter which will return the centered value of a given set
 */
class MedianFilter {
public:
    /**
     * @brief filter will filter a given input with an given kernelsize
     * @param[in] in (reference of std::vector<double>)
     * @param kernelsize (kernelsize with an odd number)
     * @return the filtered std::vector<double>
     */
    static std::vector<double> filter(const std::vector<double> &in, const int kernelsize = 3);
};

#endif
