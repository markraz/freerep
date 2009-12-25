//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Test.cpp - Author Jon Pry 

#include "Geom_Vec3.h"
#include "Util_Test.h"

bool Geom_Test()
{
	return Util_Test_Run(Geom_Vec3_Test,"Geom_Vec3");
}
