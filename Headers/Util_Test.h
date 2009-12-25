//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Util_Test.h - Author Jon Pry 

#ifndef UTIL_TEST_H
#define UTIL_TEST_H

bool RunAllTests();

bool Util_Test_Run(bool (*)(),const char *msg);

//Package Toplevel Test Decelerations
bool Geom_Test();

#endif 
