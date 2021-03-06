//This code is an adaptation of Davyd Madeley GtkGLExt example 
//It is licensed under the BSD license. 
//Any modifications from the original work are
//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions

/*
 * An example of using GtkGLExt in C
 *
 * Written by Davyd Madeley <davyd@madeley.id.au> and made available under a
 * BSD license.
 * 
 * This is purely an example, it may eat your cat and you can keep both pieces.
 *
 * Compile with:
 *    gcc -o gtkglext-example `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0` gtkglext-example.c
 */
 
#define DRAWFACES
#define DRAWEDGES

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <GL/gl.h>
#include <GL/glu.h>
 
#include <math.h>

#include "Topo_Line.h"
#include "Topo_Arc.h"
#include "Topo_Edge.h"
#include "Topo_Face_Planar.h"
#include "Topo_Face_Spheric.h"
#include "Topo_Face_Toroidal.h"
#include "Topo_Solid.h"
#include "BREP_Algo_Extrude.h"
#include "BREP_Algo_Make_Sphere.h"
#include "BREP_Algo_Make_Cone.h"
#include "FreeREP.h"
#include "Util_Test.h"
#include "Format_FREP.h"
#include "Format_IGES.h"
#include "ICanTriangulate.h"

#include "../Util/Text3d.h"

#define ALPHA 0.5

void dvCall(const Geom_Vec3 &pt,const Geom_Vec3 &derivitive)
{
    glVertex3d(pt.m_x,pt.m_y,pt.m_z);
}

void vCall(const Geom_Vec3 &pt,const Geom_Vec3 &n)
{
	Geom_Vec3 norm = n;// * -1;
	
	glNormal3d(norm.m_x,norm.m_y,norm.m_z);
    glVertex3d(pt.m_x,pt.m_y,pt.m_z);
}

 GLfloat material_diffuse[] = {.25,.25,.25, 1 };

double ang = 0;
static gboolean
expose (GtkWidget *da, GdkEventExpose *event, gpointer user_data)
{
	GdkGLContext *glcontext = gtk_widget_get_gl_context (da);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (da);

	// g_print (" :: expose\n");

	if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
	{
		g_assert_not_reached ();
	}

	/* draw in here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glDepthFunc(GL_GREATER);  // The Type Of Depth Testing (Less Or Equal)
    glEnable(GL_DEPTH_TEST);  
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
    
         
	glPushMatrix();

	glRotatef (ang, 1, 0, 1);
	// glRotatef (ang, 0, 1, 0);
	// glRotatef (ang, 0, 0, 1);

	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	

/*
	glBegin (GL_LINES);
	glColor3f (1., 0., 0.);
	glVertex3f (0., 0., 0.);
	glVertex3f (1., 0., 0.);
	glEnd ();

	glBegin (GL_LINES);
	glColor3f (0., 1., 0.);
	glVertex3f (0., 0., 0.);
	glVertex3f (0., 1., 0.);
	glEnd ();

	glBegin (GL_LINES);
	glColor3f (0., 0., 1.);
	glVertex3f (0., 0., 0.);
	glVertex3f (0., 0., 1.);
	glEnd (); */

	FreeREP::Init();

    Geom_Vec3 pt1(0,0,0);
    Geom_Vec3 pt2(0,1,0);
    Geom_Vec3 pt3(1,1,0);
    Geom_Vec3 pt4(1,0,0);

    Geom_Vec3 pt5(0.5,0.5,0);
    Geom_Vec3 pt6(0.5,.75,0);
    Geom_Vec3 pt7(.75,.75,0);


    Topo_Line *l1 = new Topo_Line(pt1,pt2);
    Topo_Line *l2 = new Topo_Line(pt2,pt3);
    Topo_Line *l3 = new Topo_Line(pt3,pt4);

    Topo_Arc *a1 = new Topo_Arc(Geom_Ax2(pt4,Geom_Vec3(0,0,-1),Geom_Vec3(-1,0,0)),1,M_PI/2,0);

    Topo_Line *l4 = new Topo_Line(pt5,pt6);
    Topo_Line *l5 = new Topo_Line(pt6,pt7);
    Topo_Line *l6 = new Topo_Line(pt7,pt5);

/*
    Topo_Edge *e1 = new Topo_Edge();
    e1->Add(l1);
    e1->Add(l2);
    //e1->Add(l3);
    e1->Add(a1);
    e1->Reverse();

    Topo_Edge *e2 = new Topo_Edge();
    e2->Add(l4);
    e2->Add(l5);
    e2->Add(l6);
*/
/*
    glBegin(GL_LINE_STRIP);
    e1->GetVertices(.01,vCall);
    glEnd();

    glBegin(GL_LINE_STRIP);
    e2->GetVertices(.01,vCall);
    glEnd(); */

/*
    Topo_Face *face = new Topo_Face_Planar(Geom_Plane(Geom_Vec3(0,0,0),Geom_Vec3(0,0,-1)));
    face->Add(e1);
    face->Add(e2);*/

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_AUTO_NORMAL);
    
    //Draw a coordinate axis on the screen
    glBegin(GL_LINES);
    
    glVertex3d(0,0,0);
    glVertex3d(0,0,.5);
    
    glVertex3d(0,0,0);
    glVertex3d(0,.5,0);
    
    glVertex3d(0,0,0);
    glVertex3d(.5,0,0);
    
    glEnd();
    
    glPushMatrix();
    glScaled(.25,.25,.25);
    
    glPushMatrix();
    glTranslated(-.5,.5,0);
    t3dDraw3D("Y", 0, 0, 0.05f);
	glPopMatrix();

	glPushMatrix();
    glTranslated(.5,-.5,0); 
    glRotated(90,0,0,1);
    t3dDraw3D("X", 0, 0, 0.05f);
	glPopMatrix();
	
	glTranslated(-.5,0,0.5); 
    glRotated(90,1,0,0);
    t3dDraw3D("Z", 0, 0, 0.05f);
	
	
	glPopMatrix();

    glBegin(GL_TRIANGLES);
    //face->Triangulate(.01,vCall);
    glEnd();
    
    Topo_Face_Spheric *sphere = new Topo_Face_Spheric(Geom_Ax2(Geom_Vec3(0,0,0),Geom_Vec3(0,0,1),Geom_Vec3(1,0,0)),1);
    glBegin(GL_TRIANGLES);
    //sphere->Triangulate(.01,vCall);
    glEnd();
    
    Topo_Face_Toroidal *toroid = new Topo_Face_Toroidal(Geom_Ax2(Geom_Vec3(0,0,0),Geom_Vec3(0,0,1),Geom_Vec3(1,0,0)),.5,.125);
    glBegin(GL_TRIANGLES);
    //toroid->Triangulate(.01,vCall);
    glEnd();

    //Topo_Solid *solid = BrepAlgoExtrude(face,Geom_Vec3(0,0,.5));

    glBegin(GL_TRIANGLES); 
    //solid->Triangulate(.001,vCall); 
    glEnd(); 
    
    std::vector<Topo_Shape*> shapes = ReadIGES("Tests/Pawn.igs");// = ReadFREP("Tests/SimpleFaces.FREP");
    
//    shapes.push_back(MakeSphere(Geom_Ax2(Geom_Vec3(0,0,0),Geom_Vec3(0,0,1),Geom_Vec3(1,0,0)),1));
//    shapes.push_back(MakeSphere(Geom_Ax2(Geom_Vec3(0,0,0),Geom_Vec3(0,0,1),Geom_Vec3(1,0,0)),0.25));
//    shapes.push_back(MakeCone(Geom_Ax2(Geom_Vec3(0,0,0),Geom_Vec3(0,0,1),Geom_Vec3(1,0,0)),.5,1,1));
    
    for(int i=0; i < shapes.size(); i++)
    {
    	ICanTriangulate *obj = dynamic_cast<ICanTriangulate*>(shapes[i]);
    	if(obj)
    	{
 #ifdef DRAWFACES
    		glBegin(GL_TRIANGLES);
    		obj->Triangulate(.01,vCall);
    		glEnd();
 #endif
 #ifdef DRAWEDGES
    		Topo_Face *face = dynamic_cast<Topo_Face*>(shapes[i]);
    		if(face)
    		{
    			Topo_Edge* edge = face->GetFirstEdge();
    			while(edge)
    			{
	    			glBegin(GL_LINE_STRIP);
    				edge->GetVertices(.01,dvCall);
    				glEnd();	
    				
    				edge = face->GetNextEdge();	
    			}	
    		}

    		Topo_Solid *solid = dynamic_cast<Topo_Solid*>(shapes[i]);
    		if(solid)
    		{
    			face = solid->GetFirstFace();
    			while(face)
    			{
    				Topo_Edge* edge = face->GetFirstEdge();
    				while(edge)
    				{
		    			glBegin(GL_LINE_STRIP);
    					edge->GetVertices(.01,dvCall);
    					glEnd();	
    				
    					edge = face->GetNextEdge();	
    				}
    				face = solid->GetNextFace();
    			}	
    		}
    	
 #endif
    	}
    	else
    	{
    		Topo_Edge *edge = dynamic_cast<Topo_Edge*>(shapes[i]);
    		if(edge)
    		{
    			glBegin(GL_LINE_STRIP);
    			edge->GetVertices(.01,dvCall);
    			glEnd();	
    		}
    		
    		Topo_Wire *wire = dynamic_cast<Topo_Wire*>(shapes[i]);
    		if(wire)
    		{
    			glBegin(GL_LINE_STRIP);
    			wire->GetVertices(.01,dvCall);
    			glEnd();	
    		}	
    			
    	}
    }
    
/*    Topo_Face *tface = solid->GetFirstFace();
    while(tface)
    {
        Topo_Edge *tedge = tface->GetFirstEdge();
        while(tedge)
        {
            glBegin(GL_LINE_STRIP);
            tedge->GetVertices(.01,vCall);
            glEnd();

            tedge = tface->GetNextEdge();
        }

        tface = solid->GetNextFace();
    }*/


	glPopMatrix ();

	if (gdk_gl_drawable_is_double_buffered (gldrawable))
		gdk_gl_drawable_swap_buffers (gldrawable);

	else
		glFlush ();

	gdk_gl_drawable_gl_end (gldrawable);

	return TRUE;
}

static gboolean
configure (GtkWidget *da, GdkEventConfigure *event, gpointer user_data)
{
	GdkGLContext *glcontext = gtk_widget_get_gl_context (da);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (da);

	if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
	{
		g_assert_not_reached ();
	}

	glLoadIdentity();

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();

	glViewport (0, 0, da->allocation.width, da->allocation.height);
	glFrustum(-1,1,-1,1,100,100);

	glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
	glLoadIdentity();
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glScalef (.5,.5,.5);

	gdk_gl_drawable_gl_end (gldrawable);

	return TRUE;
}

static gboolean
rotate (gpointer user_data)
{
	GtkWidget *da = GTK_WIDGET (user_data);

	ang++;

	gdk_window_invalidate_rect (da->window, &da->allocation, FALSE);
	gdk_window_process_updates (da->window, FALSE);

	return TRUE;
}

int
main (int argc, char **argv)
{
	RunAllTests();
	
	GtkWidget *window;
	GtkWidget *da;
	GdkGLConfig *glconfig;

	gtk_init (&argc, &argv);
	gtk_gl_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
	da = gtk_drawing_area_new ();

	gtk_container_add (GTK_CONTAINER (window), da);
	g_signal_connect_swapped (window, "destroy",
			G_CALLBACK (gtk_main_quit), NULL);
	gtk_widget_set_events (da, GDK_EXPOSURE_MASK);

	gtk_widget_show (window);

	/* prepare GL */
	glconfig = gdk_gl_config_new_by_mode ((GdkGLConfigMode)
			(GDK_GL_MODE_RGB |
			GDK_GL_MODE_DEPTH |
			GDK_GL_MODE_DOUBLE));

	if (!glconfig)
	{
		g_assert_not_reached ();
	}

	if (!gtk_widget_set_gl_capability (da, glconfig, NULL, TRUE,
				GDK_GL_RGBA_TYPE))
	{
		g_assert_not_reached ();
	}

	g_signal_connect (da, "configure-event",
			G_CALLBACK (configure), NULL);
	g_signal_connect (da, "expose-event",
			G_CALLBACK (expose), NULL);

	gtk_widget_show_all (window);

	g_timeout_add (1000 / 30, rotate, da);
	
	t3dInit();
	
	gtk_main ();
}