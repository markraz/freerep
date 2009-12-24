//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//ICanCopy.h - Author Jon Pry 

#ifndef ICANCOPY_H
#define ICANCOPY_H

#include "Geom_Vec3.h"

class ICanCopy
{
public:
    virtual void* MakeACopy() const = 0;
};

class ICanCopyWithTranslation
{
public:
    virtual void* MakeTranslatedCopy(Geom_Vec3 dir) const = 0;
};

#endif