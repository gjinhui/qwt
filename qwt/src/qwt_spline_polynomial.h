/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_SPLINE_POLYNOMIAL_H
#define QWT_SPLINE_POLYNOMIAL_H 1

#include "qwt_global.h"
#include <qpoint.h>
#include <qmetatype.h>

#ifndef QT_NO_DEBUG_STREAM
#include <qdebug.h>
#endif

/*!
  \brief A cubic polynomial without constant term

  QwtSplinePolynomial is a 3rd degree polynomial 
  of the form: y = c3 * x³ + c2 * x² + c1 * x;

  QwtSplinePolynomial is usually used in combination with polygon
  interpolation, where it is not necessary to store a constant term ( c0 ),
  as the translation is known from the corresponding polygon points.
 */
class QWT_EXPORT QwtSplinePolynomial
{
public:
    QwtSplinePolynomial( double c3 = 0.0, double c2 = 0.0, double c1 = 0.0 );

    double valueAt( double x ) const;
    double slopeAt( double x ) const;
    double curvatureAt( double x ) const;

    static QwtSplinePolynomial fromSlopes( 
        const QPointF &p1, double m1, 
        const QPointF &p2, double m2 );

    static QwtSplinePolynomial fromSlopes( 
        double x, double y, double m1, double m2 );

    static QwtSplinePolynomial fromCurvatures( 
        const QPointF &p1, double cv1,
        const QPointF &p2, double cv2 );
    
    static QwtSplinePolynomial fromCurvatures(
        double dx, double dy, double cv1, double cv2 );

public:
    double c3;
    double c2;
    double c1;
};

inline QwtSplinePolynomial::QwtSplinePolynomial( double a, double b, double c ):
    c3(a),
    c2(b),
    c1(c)
{
}

inline double QwtSplinePolynomial::valueAt( double x ) const
{
    return ( ( ( c3 * x ) + c2 ) * x + c1 ) * x;
}   

inline double QwtSplinePolynomial::slopeAt( double x ) const
{   
    return ( 3.0 * c3 * x + 2.0 * c2 ) * x + c1;
}

inline double QwtSplinePolynomial::curvatureAt( double x ) const
{   
    return 6.0 * c3 * x + 2.0 * c2;
}

inline QwtSplinePolynomial QwtSplinePolynomial::fromSlopes( 
    const QPointF &p1, double m1, const QPointF &p2, double m2 )
{
    return fromSlopes( p2.x() - p1.x(), p2.y() - p1.y(), m1, m2 );
}

inline QwtSplinePolynomial QwtSplinePolynomial::fromSlopes( 
    double x, double y, double m1, double m2 )
{
    const double c2 = ( 3.0 * y / x - 2 * m1 - m2 ) / x;
    const double c3 = ( ( m2 - m1 ) / x - 2.0 * c2 ) / ( 3.0 * x );

    return QwtSplinePolynomial( c3, c2, m1 );
}

inline QwtSplinePolynomial QwtSplinePolynomial::fromCurvatures( 
    const QPointF &p1, double cv1, const QPointF &p2, double cv2 )
{
    return fromCurvatures( p2.x() - p1.x(), p2.y() - p1.y(), cv1, cv2 );
}

inline QwtSplinePolynomial QwtSplinePolynomial::fromCurvatures( 
    double x, double y, double cv1, double cv2 )
{
    const double c3 = ( cv2 - cv1 ) / ( 6.0 * x );
    const double c2 = 0.5 * cv1;
    const double c1 = y / x - ( c3 * x + c2 ) * x;

    return QwtSplinePolynomial( c3, c2, c1 );
}

Q_DECLARE_METATYPE( QwtSplinePolynomial )

#ifndef QT_NO_DEBUG_STREAM
inline QDebug operator<<( QDebug debug, const QwtSplinePolynomial &polynomial )
{
    debug.nospace() << "Polynom(" << polynomial.c3 << ", "
        << polynomial.c2 << ", " << polynomial.c1 << ")";
    return debug.space();
}
#endif

#endif
