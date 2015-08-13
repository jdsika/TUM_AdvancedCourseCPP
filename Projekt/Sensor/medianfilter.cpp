#include "medianfilter.h"

#include <algorithm>
#include <QDebug>

/**
* @brief MedianFilter::filter fuehrt einen rolling median mit bestimmter kernelsize ueber dem eingangsvektor aus
* @param in Eingangsvektor
* @param kernelsize Aus wievielen Werte soll der Median einer Stelle im Vektor berechnet werden. Muss ungerade und >= 3 sein
* @return Median-gefilterter Vektor
*/
std::vector<double> MedianFilter::filter(const std::vector<double> &in, const int kernelsize)
{
    int len = in.size();
    int halfsize = (kernelsize-1)/2;

    if(kernelsize % 2 == 0 || kernelsize < 3 || len < kernelsize){
//        qWarning() << "kernelsize=" <<kernelsize << "oder in.size()=" << len << "falsch";
        return in;
    }

    std::vector<double> out = in;

    // normaler rolling median
    for(int i=0; i<len; ++i){
        auto left  = in.begin() + i - halfsize;
        auto right = in.begin() + i + halfsize + 1;
        if(left  < in.begin()) left  = in.begin(); // carlo disagrees about the use of <
        if(right > in.end()  ) right = in.end();   // laurenz says this is fine

        std::vector<double> sortVect(left, right); // copy the in vector's contents from <left> to <right> to this new vector

        std::nth_element(sortVect.begin(), sortVect.begin()+sortVect.size()/2, sortVect.end()); // sort the vector

        out[i] = sortVect[sortVect.size()/2]; // sortVect.size()%2==1 --> takes the median value and saves it to the output vector
    }

    return out;
}
