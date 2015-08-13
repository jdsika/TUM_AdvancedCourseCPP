/**
 * spline.h
 *
 * simple cubic spline interpolation library without external
 * dependencies
 *
 * ---------------------------------------------------------------------
 * Copyright (C) 2011, 2014 Tino Kluge (ttk448 at gmail.com)
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * -> replace STD:: stuff with QT:: functions (Jan Ehrensperger 29.11.14)
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ---------------------------------------------------------------------
 *
 */
#ifndef SPLINEQT_H
#define SPLINEQT_H

#include <QVector>

namespace tkqt {

/**
 * @brief The band_matrix class, which is the basis for cubic hermite spline creation.
 */
class band_matrix
{
private:
    QVector< QVector<double> > m_upper;  /**< upper band QVector of QVector*/
    QVector< QVector<double> > m_lower;  /**< lower band QVector of QVector*/

public:
    /**
     * @brief band_matrix => standard constructor
     */
    band_matrix() {}
    /**
     * @brief band_matrix => overwritten constructor with dimension
     * @param[in] dim (int) dimension
     * @param[in] n_u (int)
     * @param[in] n_l (int)
     */
    band_matrix(int dim, int n_u, int n_l);
    /**
     * @brief Destructor
     */
    ~band_matrix() {}
    /**
     * @brief resize This method will intialise the band matrix
     * @param[in] dim (int)
     * @param[in] n_u (int)
     * @param[in] n_l (int)
     */
    void resize(int dim, int n_u, int n_l);       // init with dim,n_u,n_l

    /**
     * @brief dim This method will return the dimension of the matrix
     * @return The dimension of the matrix (int)
     */
    int dim() const;                              // matrix dimension

    /**
     * @brief num_upper This method will return the size of the internal matrix m_upper
     * @return the size of the m_upper matrix
     */
    int num_upper() const {
        return m_upper.size()-1;
    }
    /**
     * @brief num_lower This method will return the size of the internal matrix m_lower
     * @return the size of the m_lower matrix
     */
    int num_lower() const {
        return m_lower.size()-1;
    }
    // access operator
    double & operator () (int i, int j);            // write
    double   operator () (int i, int j) const;      // read
    // we can store an additional diogonal (in m_lower)
    double& saved_diag(int i);
    double  saved_diag(int i) const;

    /**
     * @brief lu_decompose : This method will compute a LU-Decomposition
     */
    void lu_decompose();

    /**
     * @brief r_solve : This method solves Rx=y
     * @param b Reference of QVector<double>
     * @return the QVector<double> x
     */
    QVector<double> r_solve(const QVector<double>& b) const;

    /**
     * @brief l_solve : This method solves Ly=b
     * @param b :is the result vector (QVector<double>) Reference
     * @return  return QVector<double> y
     */
    QVector<double> l_solve(const QVector<double>& b) const;

    /**
     * @brief lu_solve: This method will solve A = LU with LU-decomposite
     * @param b
     * @param is_lu_decomposed
     * @return an QVector<double>
     */
    QVector<double> lu_solve(const QVector<double>& b,
                             bool is_lu_decomposed=false);
};

/**
 * @brief The spline class => This class will create an cubic hermite spline from two given vectors.
 */
class spline
{
private:
    QVector<double> m_x,  /**<  x coordinates of points*/
                    m_y;  /**< y coordinates of points*/
    // interpolation parameters
    // f(x) = a*(x-x_i)^3 + b*(x-x_i)^2 + c*(x-x_i) + y_i

    QVector<double> m_a, /**< interpolation parameter m_a of f(x) = m_a*(x-x_i)^3 + m_b*(x-x_i)^2 + m_c*(x-x_i) + m_d*/
                    m_b, /**< interpolation parameter m_b of f(x) = m_a*(x-x_i)^3 + m_b*(x-x_i)^2 + m_c*(x-x_i) + m_d*/
                    m_c, /**< interpolation parameter m_c of f(x) = m_a*(x-x_i)^3 + m_b*(x-x_i)^2 + m_c*(x-x_i) + m_d*/
                    m_d; /**< interpolation parameter m_d of f(x) = m_a*(x-x_i)^3 + m_b*(x-x_i)^2 + m_c*(x-x_i) + m_d*/
public:
    /**
     * @brief set_points this method will compute the cubic hermite spline
     * from given QVector<double> =x and QVector<double> =y
     * @param[in] x (QVector<double>) x-points of the spline
     * @param[in] y (QVector<double>) y-points of the spline
     * @param[in] cubic_spline (bool) if the spline should be a cubic spline or not
     * @warning x,y  have to be sorted ascendingly.
     */
    void set_points(const QVector<double>& x,
                    const QVector<double>& y, bool cubic_spline=true);
    /**
     * @brief operator ()
     * @param x
     * @return
     */
    double operator() (double x) const;
};


} // namespace tkqt




#endif // SPLINEQT_H
