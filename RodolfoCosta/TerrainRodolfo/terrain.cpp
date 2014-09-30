#include <CGAL/Cartesian.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/intersections.h>

#include <GL/glut.h>
#include "GL/gl.h"
#include <fstream>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <vector>

using namespace std;

// Main CGAL types
typedef CGAL::Cartesian<double> Repr;
typedef CGAL::Projection_traits_xy_3<Repr> Gt;
typedef CGAL::Delaunay_triangulation_2<Gt> Delaunay; // A Delaunay triangulation
typedef Repr::Point_3 Point;  // A Point in 3D
typedef Repr::Vector_3 Vector;  // A Vector in 3D
typedef Repr::Plane_3 Plane;  // A Plane in 3D
typedef std::list<Point> myPolyline; // A polygonal line
typedef std::list<myPolyline> PolylineSet; // A set of polylines
typedef std::vector<Plane> PlaneSet; // A set of planes
typedef Repr::Segment_3 Segment;

/**
 * Computes the minimum enclosing parallelepiped of a
 * Delaunay triangulation
 *
 * @param triang  Delaunay triangulation (input)
 * @param min minimum coordinates of the parallelepiped (output)
 * @param max maximum coordinates of the parallelepiped (output)
 **/
void limits (Delaunay& triang, Point& min, Point& max)
{
    Delaunay::Point_iterator ip = triang.points_begin();
    assert (ip != triang.points_end());
    double mincoord[3],maxcoord[3];
    Point p = *ip++;
    for (int i = 0; i < 3; ++i) { mincoord [i] = maxcoord [i] = p [i]; }
    while (ip != triang.points_end()) {
        Point p = *ip++;
        for (int i = 0; i < 3; ++i) {
            if (p [i] < mincoord [i]) mincoord [i] = p[i];
            else if (p [i] > maxcoord [i]) maxcoord [i] = p[i];
        }
    }
    min = Point (mincoord [0], mincoord [1], mincoord [2]);
    max = Point (maxcoord [0], maxcoord [1], maxcoord [2]);
}

void checkIntersection(Plane plane, Segment seg)
{
    CGAL::Object result;
    Point ipoint;
    Segment iseg;

    result = CGAL::intersection(plane, seg);
	
    if (CGAL::assign(ipoint, result)) 
	{
        // handle the point intersection case.
		cout << "colisao deu um ponto" << endl;
    } 
	else
	{
        if (CGAL::assign(iseg, result)) 
		{
            // handle the segment intersection case.
			cout << "colisao deu um segmento" << endl;
        } 
		else 
		{
            // handle the no intersection case.
			cout << "nao houve colisao" << endl;
        }
	}
}

/**
 * Draws a Delaunay triangulation
 *
 * @param tr  Delaunay triangulation (input)
 **/
void draw (const Delaunay& tr)
{
    // Draw triangles
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glColor3f (0.5, 0.5, 0.5);  // gray
    glBegin(GL_TRIANGLES);
    Delaunay::Finite_faces_iterator it;
    for (it = tr.finite_faces_begin (); it != tr.finite_faces_end (); ++it) {
        Delaunay::Face f = *it;
        for (int i = 0; i < 3; i++) {
            Point p = (*(f.vertex (i))).point();
            glVertex3f (p[0], p[1], p[2]);
        }
    }
    glEnd ();
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Draw Triangle edges
    glColor3f (1.0, 1.0, 1.0);  // white
    for (it = tr.finite_faces_begin (); it != tr.finite_faces_end (); ++it) {
        Delaunay::Face f = *it;
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 3; i++) {
            Point p = (*(f.vertex (i))).point();
            glVertex3f (p[0], p[1], p[2]);
        }
        glEnd ();
    }

}

/**
 * Draws a set of polylines
 *
 * @param pls  set of polylines
 **/
void draw (const PolylineSet& pls)
{
    glColor3f (0.0, 0.0, 1.0);  // blue
    for (PolylineSet::const_iterator ipls = pls.begin (); ipls != pls.end (); ipls++) {
        glBegin (GL_LINE_STRIP);
        for (myPolyline::const_iterator ipoly = ipls->begin(); ipoly != ipls->end(); ipoly++){
            Point p = *ipoly;
            glVertex3f (p[0],p[1],p[2]);
        }
        glEnd ();
    }
}

/**
 * Global variables
 **/
Delaunay dt; // A Delaunay Triangulation
PlaneSet planes; // Planes for which intersection curves are computed
PolylineSet curves; // Intersection curves

Point center;  // Center of dt�s minimum enclosing parallelepiped
int width = 800, height = 600;  // Window size
int activebutton;  // Which mouse button was pressed
int xmouse, ymouse;  // Coords where mouse was pressed
double xangle = 0.0, yangle = 0.0; // Angles for viewing terrain
double modelview [16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; // Current modelview matrix

Point segmentPointStart;
Point segmentPointEnd;
Segment segmentPick;

void init ()
{
    // Create an orthogonal projection system for viewing
    // dt from any angle
    Point min, max;
    limits (dt, min, max);
    Vector size = 0.5 * (max - min);
    center = min + size;
    double radius = sqrt (size * size);
    size = Vector (radius, radius, radius);
    min = center - size;
    max = center + size;
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(min[0], max[0], min[1], max [1], -max[2], -min[2]);

    // Enable depth testing
    glEnable (GL_DEPTH_TEST);
}

void display ()
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    glTranslatef (center[0], center[1], center[2]);
    glRotatef (xangle, 1.0, 0.0, 0.0);
    glRotatef (yangle, 0.0, 1.0, 0.0);
    glTranslatef (-center[0], -center[1], -center[2]);
    glMultMatrixd (modelview);
       draw (dt);
       draw (curves);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
		glVertex3f(segmentPick.source()[0], segmentPick.source()[1], segmentPick.source()[2]);
		glVertex3f(segmentPick.target()[0], segmentPick.target()[1], segmentPick.target()[2]);
	glEnd();	
    glutSwapBuffers();
}

void reshape (int wid, int hgt)
{
    glViewport(0,0, wid, hgt);
}

 void createMouseRay(int mouseX, int mouseY)
{
    GLdouble matModelView[16], matProjection[16]; 
    GLint viewport[4]; 
	GLdouble m_startX, m_startY, m_startZ;
	GLdouble m_endX, m_endY, m_endZ;

    // get matrix and viewport:
    glGetDoublev( GL_MODELVIEW_MATRIX, matModelView ); 
    glGetDoublev( GL_PROJECTION_MATRIX, matProjection ); 
    glGetIntegerv( GL_VIEWPORT, viewport ); 

    // window pos of mouse, Y is inverted on Windows
    double winX = (double)mouseX; 
    double winY = viewport[3] - (double)mouseY; 

    // get point on the 'near' plane (third param is set to 0.0)
    gluUnProject(winX, winY, 0.0, matModelView, matProjection, 
         viewport, &m_startX, &m_startY, &m_startZ); 

    // get point on the 'far' plane (third param is set to 1.0)
    gluUnProject(winX, winY, 1.0, matModelView, matProjection, 
         viewport, &m_endX, &m_endY, &m_endZ); 
		 
	
	segmentPointStart = Point(m_startX, m_startY, m_startZ);
	segmentPointEnd = Point(m_endX, m_endY, m_endZ);
	segmentPick = Segment(segmentPointStart, segmentPointEnd);

    // now you can create a ray from m_start to m_end
	//cout << "start position: " << endl;
	//cout << m_startX << endl; 
	//cout << m_startY << endl; 
	//cout << m_startZ << endl; 
	//cout << "end position" << endl;
	//cout << m_endX << endl; 
	//cout << m_endY << endl; 
	//cout << m_endZ << endl; 
} 



void mouseClickHandler (int button, int state, int x, int y)
{
    xmouse = x;
    ymouse = y;
    if (button == GLUT_LEFT_BUTTON) 
	{
		if(state == GLUT_DOWN)
		{
			createMouseRay(x, y);
			for(int i = 0; i < planes.size(); i++)
			{
				checkIntersection(planes[i], segmentPick);
			}
		}
        if (state == GLUT_UP) {
            glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
            xangle = 0; yangle = 0;
        }
    }
    activebutton = button;  //saves the camera rotation
    glutPostRedisplay ();
}

//This function only detects the mouse movement while we are clicking a button
void mouseActiveMotionHandler (int x, int y)
{
    if (activebutton == GLUT_RIGHT_BUTTON) 
    {
        yangle = (x - xmouse) * 360 / width;
        xangle = (y - ymouse) * 180 / height;
    }
    glutPostRedisplay ();
}

//This function detects the mouse movement while the mouse is on this window
void mousePassiveMotionHandler (int x, int y)
{
    //cout << "mouse moving" << endl;
}




int main(int argc, char * argv [])
{
    // Read the terrain data into dt
    std::ifstream terrainfile ("terraindata.txt");
    std::istream_iterator<Point> beginpt(terrainfile);
    std::istream_iterator<Point> endpt;
    dt.insert(beginpt, endpt);

    // Read set of planes into planes
    std::ifstream planefile ("planedata.txt");
    std::istream_iterator<Plane> beginpl(planefile);
    std::istream_iterator<Plane> endpl;
    while (beginpl != endpl) {
        planes.push_back (*beginpl++);
    }

    //
    // Given dt and planes, store intersection curves into curves
    //
    //

    // Set up Graphics User Interface
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Terrain");
    glutDisplayFunc(display);
    
    //Mouse callback functions
    glutMouseFunc (mouseClickHandler);
    glutMotionFunc (mouseActiveMotionHandler);
    glutPassiveMotionFunc(mousePassiveMotionHandler);
    
    glutReshapeFunc (reshape);
    
    init ();
    glutMainLoop();
    return 0;

}
