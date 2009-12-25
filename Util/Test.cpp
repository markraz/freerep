//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Test.cpp - Author Jon Pry 

#include "Util_Test.h"

#include <stdio.h>

bool Util_Test_Run(bool (*pRun)(),const char *msg)
{
	if(!pRun())
	{
		printf("Error: %s\n", msg);
		return false;
	}
	return true;
}

bool RunAllTests()
{
	return Util_Test_Run(Geom_Test,"Geom_Test");	
}