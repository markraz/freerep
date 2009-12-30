//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//FreeREP.h - Author Jon Pry 


#ifndef FREEREP_H
#define FREEREP_H

//TODO: what is a good value for this?
#define REALLY_BIG 10e20

class FreeREP
{
public:
    static void Init(){}

    static const double CONFUSION = 10e-9;
    static const double CONFUSION2 = 10e-9 * 10e-9;
};

#endif