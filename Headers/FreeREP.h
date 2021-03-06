//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//FreeREP.h - Author Jon Pry 


#ifndef FREEREP_H
#define FREEREP_H

//TODO: what is a good value for this?
#define REALLY_BIG 10e20

#define EPSILON 10e-8 //Numeric derivitives

class FreeREP
{
public:
    static void Init(){}

    static const double CONFUSION = 10e-5;
    static const double CONFUSION2 = 10e-5 * 10e-5;
};

#define ISZERO(a) ((a) < 10e-5 && (a) > -10e-5)

#endif