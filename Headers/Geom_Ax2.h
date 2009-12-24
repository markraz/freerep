//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Ax2.h - Author Jon Pry 


#ifndef GEOM_AX2_H
#define GEOM_AX2_H

#include "Geom_Vec3.h"

class Geom_Ax2
{
    Geom_Vec3 m_loc;
    Geom_Vec3 m_zdir;
    Geom_Vec3 m_xdir;

public:
    Geom_Ax2(Geom_Vec3 loc, Geom_Vec3 zdir, Geom_Vec3 xdir):m_loc(loc),m_zdir(zdir),m_xdir(xdir){}
    Geom_Ax2(){}

    Geom_Vec3 Location() const {return m_loc;}
    Geom_Vec3 ZDir() const {return m_zdir;}
    Geom_Vec3 XDir() const {return m_xdir;}
    Geom_Vec3 YDir() const {return m_zdir ^ m_xdir;}

    void SetLocation(Geom_Vec3 p){m_loc = p;}
};

#endif