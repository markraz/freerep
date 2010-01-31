//This code is an adaptation of matrix.cpp from
//the HeeksCAD project as of 1/30/10. And is covered
//under the BSD license
//Any modifications from the original work are
//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions

#include "Geom_Matrix.h"

#include <stdlib.h>

Geom_Matrix::Geom_Matrix(double init)
{
        m_data.resize(9);

        for(int i=0; i < 9; i++)
                m_data[i] = init;
}

Geom_Matrix::~Geom_Matrix()
{
}


inline
double Geom_Matrix::GetElement(unsigned row, unsigned col)
{
        return m_data[3*row + col];
}

inline
double& Geom_Matrix::operator() (unsigned row, unsigned col)
{
   return m_data[3*row + col];
}

inline
double Geom_Matrix::operator() (unsigned row, unsigned col) const
{
   return m_data[3*row + col];
}

void Geom_Matrix::Randomize()
{
        for(int i=0; i < 9; i++)
        {
                m_data[i] = rand() / 100.0;
        }
}

Geom_Vec3 Geom_Matrix::Multiply(Geom_Vec3 pnt)
{
	double nvec[3],ovec[3];
	ovec[0] = pnt.m_x; ovec[1] = pnt.m_y; ovec[2] = pnt.m_z;
	
    for(int i=0; i < 3; i++)
    {
     	nvec[i] = 0;
        for(int k=0; k < 3; k++)
        {
      		nvec[i] += GetElement(i,k) * ovec[k];
        }
    }
 	
 	return Geom_Vec3(nvec[0],nvec[1],nvec[2]);
}

Geom_Matrix Geom_Matrix::Multiplied(Geom_Matrix* mat)
{
        Geom_Matrix nmat;

        for(int i=0; i < 3; i++)
        {
                for(int j=0; j < 3; j++)
                {
                        nmat(i,j) = 0;
                        for(int k=0; k < 3; k++)
                        {
                                nmat(i,j) += GetElement(i,k) * mat->GetElement(k,j);
                        }
                }
        }
        return nmat;
}

Geom_Matrix Geom_Matrix::Multiplied(double s)
{
        Geom_Matrix nmat;

        for(int i=0; i < 3; i++)
        {
                for(int j=0; j < 3; j++)
                {
                        nmat(i,j) = s * GetElement(i,j);
                }
        }
        return nmat;
}

void Geom_Matrix::Multiply(Geom_Matrix *mat)
{
        Geom_Matrix m = Multiplied(mat);
        m_data = m.m_data;
}

Geom_Matrix Geom_Matrix::Transposed()
{
        Geom_Matrix nmat;
        for(int i=0; i < 3; i++)
        {
                for(int j=0; j < 3; j++)
                {
                        nmat(j,i) = GetElement(i,j);
                }
        }
        return nmat;
}

void Geom_Matrix::Transpose()
{
        Geom_Matrix m = Transposed();
        m_data = m.m_data;
}


void Geom_Matrix::Multiply(double s)
{
        Geom_Matrix m = Multiplied(s);
        m_data = m.m_data;
}

void Geom_Matrix::Add(Geom_Matrix* mat)
{
        Geom_Matrix m = Added(mat);
        m_data = m.m_data;
}

void Geom_Matrix::Subtract(Geom_Matrix* mat)
{
        Geom_Matrix m = Subtracted(mat);
        m_data = m.m_data;
}

void Geom_Matrix::Subtract(double s)
{
        Geom_Matrix m = Subtracted(s);
        m_data = m.m_data;
}

void Geom_Matrix::Add(double s)
{
        Geom_Matrix m = Added(s);
        m_data = m.m_data;
}

void Geom_Matrix::Divide(double s)
{
        Geom_Matrix m = Divided(s);
        m_data = m.m_data;
}

Geom_Matrix Geom_Matrix::Added(Geom_Matrix *mat)
{
        Geom_Matrix nmat;
        for(int i=0; i < 3; i++)
        {
                for(int j=0; j < 3; j++)
                {
                        nmat(i,j) = GetElement(i,j) + mat->GetElement(i,j);
                }
        }

        return nmat;
}

Geom_Matrix Geom_Matrix::Subtracted(Geom_Matrix *mat)
{
        Geom_Matrix nmat;
        for(int i=0; i < 3; i++)
        {
                for(int j=0; j < 3; j++)
                {
                        nmat(i,j) = GetElement(i,j) - mat->GetElement(i,j);
                }
        }

        return nmat;
}

Geom_Matrix Geom_Matrix::Subtracted(double s)
{
        Geom_Matrix nmat;
        for(int i=0; i < 3; i++)
        {
                for(int j=0; j < 3; j++)
                {
                        nmat(i,j) = GetElement(i,j) - s;
                }
        }

        return nmat;
}

Geom_Matrix Geom_Matrix::Added(double s)
{
        Geom_Matrix nmat;
        for(int i=0; i < 3; i++)
        {
                for(int j=0; j < 3; j++)
                {
                        nmat(i,j) = GetElement(i,j) + s;
                }
        }

        return nmat;
}

Geom_Matrix Geom_Matrix::Divided(double s)
{
        Geom_Matrix nmat;
        for(int i=0; i < 3; i++)
        {
                for(int j=0; j < 3; j++)
                {
                        nmat(i,j) = GetElement(i,j) / s;
                }
        }

        return nmat;
}

