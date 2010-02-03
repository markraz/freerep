//This code is an adaptation of matrix.cpp from
//the HeeksCAD project as of 1/30/10. And is covered
//under the BSD license
//Any modifications from the original work are
//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions

#include "Geom_Matrix.h"

#include <stdlib.h>
#include <math.h>

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
double Geom_Matrix::GetElement(unsigned row, unsigned col) const
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

Geom_Matrix Geom_Matrix::operator* (Geom_Matrix m) const
{
	return Multiplied(&m);	
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

Geom_Matrix Geom_Matrix::Multiplied(Geom_Matrix* mat) const
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

Geom_Matrix Geom_Matrix::XRotation(double t)
{
	Geom_Matrix m;
	m(0,0) = 1;
	m(0,1) = 0;
	m(0,2) = 0;
	m(1,0) = 0;
	m(1,1) = cos(t);
	m(1,2) = -sin(t);
	m(2,0) = 0;
	m(2,1) = sin(t);
	m(2,2) = cos(t);
	
	return m;
}

Geom_Matrix Geom_Matrix::YRotation(double t)
{
	Geom_Matrix m;
	m(0,0) = cos(t);
	m(0,1) = 0;
	m(0,2) = sin(t);
	m(1,0) = 0;
	m(1,1) = 1;
	m(1,2) = 0;
	m(2,0) = -sin(t);
	m(2,1) = 0;
	m(2,2) = cos(t);
	
	return m;
}

Geom_Matrix Geom_Matrix::ZRotation(double t)
{
	Geom_Matrix m;
	m(0,0) = cos(t);
	m(0,1) = -sin(t);
	m(0,2) = 0;
	m(1,0) = sin(t);
	m(1,1) = cos(t);
	m(1,2) = 0;
	m(2,0) = 0;
	m(2,1) = 0;
	m(2,2) = 1;
	
	return m;
}

Geom_Matrix Geom_Matrix::XToXZ(Geom_Vec3 v)
{
	Geom_Matrix m;
	m(0,0) = v.m_x / sqrt(v.m_x * v.m_x + v.m_y * v.m_y);
	m(0,1) = v.m_y / sqrt(v.m_x * v.m_x + v.m_y * v.m_y);
	m(0,2) = 0;
	m(1,0) = -v.m_y / sqrt(v.m_x * v.m_x + v.m_y * v.m_y);
	m(1,1) = v.m_x / sqrt(v.m_x * v.m_x + v.m_y * v.m_y);
	m(1,2) = 0;
	m(2,0) = 0;
	m(2,1) = 0;
	m(2,2) = 1;
	
	return m;
}

Geom_Matrix Geom_Matrix::XZToZ(Geom_Vec3 v)
{
	Geom_Matrix m;
	m(0,0) = v.m_z/v.Norm();
	m(0,1) = 0;
	m(0,2) = -sqrt(v.m_x * v.m_x + v.m_y * v.m_y)/v.Norm();
	m(1,0) = 0;
	m(1,1) = 1;
	m(1,2) = 0;
	m(2,0) = -m(0,2);
	m(2,1) = 0;
	m(2,2) = m(0,0);
	
	return m;
}

Geom_Matrix Geom_Matrix::RotateAround(Geom_Vec3 v, double t)
{
	Geom_Matrix Rxz = XToXZ(v);
	Geom_Matrix RxzT = Rxz.Transposed();
	Geom_Matrix Rxz2z = XZToZ(v);
	Geom_Matrix Rxz2zT = Rxz2z.Transposed();
	Geom_Matrix R = ZRotation(t);
	
	return RxzT * Rxz2zT * R * Rxz2z * Rxz;
}