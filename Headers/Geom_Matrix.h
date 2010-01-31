//This code is an adaptation of matrix.h from
//the HeeksCAD project as of 1/30/10. And is covered
//under the BSD license
//Any modifications from the original work are
//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions

#ifndef GEOM_MATRIX_H
#define GEOM_MATRIX_H

#include "Geom_Vec3.h"

#include <vector>

class Geom_Matrix
{
        std::vector<double> m_data;
public:
        Geom_Matrix(double init=0);
        ~Geom_Matrix();
        double& operator() (unsigned row, unsigned col);
        double  operator() (unsigned row, unsigned col) const;
        //double Geom_Matrix::operator() (unsigned row, unsigned col);
        double GetElement(unsigned row, unsigned col);
        Geom_Matrix Multiplied(Geom_Matrix* mat);
        void Multiply(Geom_Matrix* mat);
        Geom_Vec3 Multiply(Geom_Vec3 vec);
        Geom_Matrix Multiplied(double s);
        void Multiply(double s);
        Geom_Matrix Added(double s);
        Geom_Matrix Added(Geom_Matrix *mat);
        void Add(double s);
        void Add(Geom_Matrix *mat);
        Geom_Matrix Subtracted(double s);
        Geom_Matrix Subtracted(Geom_Matrix *mat);
        void Subtract(Geom_Matrix* mat);
        void Subtract(double s);
        void Divide(double s);
        Geom_Matrix Divided(double s);
        void Transpose();
        Geom_Matrix Transposed();
        void Randomize();
};


#endif 
