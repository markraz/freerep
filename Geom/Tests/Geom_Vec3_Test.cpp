//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Vec3_Test.cpp - Author Jon Pry 

#include "Geom_Vec3.h"
#include "Util_Test.h"

bool Geom_Vec3_Test_Norm()
{
	Geom_Vec3 vec1(1,0,0);	
	Geom_Vec3 vec2(0,1,0);
	Geom_Vec3 vec3(0,0,1);
	Geom_Vec3 vec4(2,0,0);
	Geom_Vec3 vec5(-2,0,0);
	
	return vec1.Norm() == 1.0 && 
			vec2.Norm() == 1.0 &&
			vec3.Norm() == 1.0 &&
			vec4.Norm() == 2.0 &&
			vec5.Norm() == 2.0;
}

bool Geom_Vec3_Test_Add()
{
	Geom_Vec3 vec1(1,0,0);	
	Geom_Vec3 vec2(0,1,0);
	Geom_Vec3 vec3(0,0,1);
	
	Geom_Vec3 vec4 = vec1 + vec2 + vec3;
	
	if(vec4.m_x != 1.0 || vec4.m_y != 1.0 || vec4.m_z != 1.0)
		return false;
		
	vec4 = vec1.Added(vec2.Added(vec3));	

	if(vec4.m_x != 1.0 || vec4.m_y != 1.0 || vec4.m_z != 1.0)
		return false;
	
	return true;
}

bool Geom_Vec3_Test_Subtract()
{
	Geom_Vec3 vec1(1,0,0);	
	Geom_Vec3 vec2(0,1,0);
	Geom_Vec3 vec3(0,0,1);
	
	Geom_Vec3 vec4 = vec1 - vec2 - vec3;
	
	if(vec4.m_x != 1.0 || vec4.m_y != -1.0 || vec4.m_z != -1.0)
		return false;
		
	vec4 = vec1.Subtracted(vec2).Subtracted(vec3);	

	if(vec4.m_x != 1.0 || vec4.m_y != -1.0 || vec4.m_z != -1.0)
		return false;
	
	return true;
}

bool Geom_Vec3_Test_Multiply()
{
	Geom_Vec3 vec1(1,2,3);
	
	Geom_Vec3 vec2 = vec1 * 2.0;
	
	if(vec2.m_x != 2.0 || vec2.m_y != 4.0 || vec2.m_z != 6.0)
		return false;
	
	vec2 = vec1.Multiplied(2.0);
	
	if(vec2.m_x != 2.0 || vec2.m_y != 4.0 || vec2.m_z != 6.0)
		return false;

	return true;
}

bool Geom_Vec3_Test()
{
	return Util_Test_Run(Geom_Vec3_Test_Norm,"Geom_Vec3_Norm") &&
			Util_Test_Run(Geom_Vec3_Test_Add,"Geom_Vec3_Add") &&
			Util_Test_Run(Geom_Vec3_Test_Subtract,"Geom_Vec3_Subtract") &&
			Util_Test_Run(Geom_Vec3_Test_Multiply,"Geom_Vec3_Multiply");
			
	//TODO: dot and cross 
}