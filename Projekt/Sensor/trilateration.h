#ifndef TRILATERATION_H
#define TRILATERATION_H

namespace trilateration
{
/**
     * @brief circle_circle_intersection: Determine the points where 2 circles in a common plane intersect.
     * @param[in] x0 (double)
     * @param[in] y0 (double)
     * @param[in] r0 (double)
     * @param[in] x1 (double)
     * @param[in] y1 (double)
     * @param[in] r1 (double)
     * @param[in] xi (double*)
     * @param[in] yi (double*)
     * @param[in] xi_prime (double*)
     * @param[in] yi_prime (double*)
     * @return of the error value
     */
    int circle_circle_intersection(double x0, double y0, double r0,
                                   double x1, double y1, double r1,
                                   double *xi, double *yi,
                                   double *xi_prime, double *yi_prime);
}


#endif // TRILATERATION_H
