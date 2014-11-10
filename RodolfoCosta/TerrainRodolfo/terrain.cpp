#include <CGAL/Cartesian.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <GL/glut.h>
#include "GL/gl.h"
#include <fstream>
#include <iostream>
#include <assert.h>
#include <math.h>

using namespace std;

// Main CGAL types
typedef CGAL::Cartesian<double> Repr;
typedef CGAL::Projection_traits_xy_3<Repr> Gt;
typedef CGAL::Delaunay_triangulation_2<Gt> Delaunay;    //A Delaunay triangulation
typedef Repr::Point_3 Point3D;                           //A Point in 3D
typedef Repr::Point_2 Point2D;                          //A Point in 2D

typedef Repr::Triangle_3 Triangle3D;                     //Triangle in 3D
typedef Repr::Triangle_2 Triangle2D;                    //Triangle in 2D

typedef Repr::Vector_3 Vector3D;                         //A Vector in 3D
typedef Repr::Vector_2 Vector2D;                        //A Vector in 2D

typedef Repr::Segment_3 LineSegment3D;                    //A 3D line segment
typedef Repr::Segment_2 LineSegment2D;                  //A 2D LineSegment

typedef Repr::Ray_2 Ray2D;                              //2D Ray

/**
 * Computes the minimum enclosing parallelepiped of a
 * Delaunay triangulation
 *
 * @param triang  Delaunay triangulation (input)
 * @param min minimum coordinates of the parallelepiped (output)
 * @param max maximum coordinates of the parallelepiped (output)
 **/
void limits (Delaunay& triang, Point3D& min, Point3D& max)
{
    Delaunay::Point_iterator ip = triang.points_begin();
    assert (ip != triang.points_end());
    double mincoord[3],maxcoord[3];
    Point3D p = *ip++;
    for (int i = 0; i < 3; ++i) { mincoord [i] = maxcoord [i] = p [i]; }
    while (ip != triang.points_end()) {
        Point3D p = *ip++;
        for (int i = 0; i < 3; ++i) {
            if (p [i] < mincoord [i]) mincoord [i] = p[i];
            else if (p [i] > maxcoord [i]) maxcoord [i] = p[i];
        }
    }
    min = Point3D (mincoord [0], mincoord [1], mincoord [2]);
    max = Point3D (maxcoord [0], maxcoord [1], maxcoord [2]);
}

/**
* Returns a 2D gradient of a 3D triangle
* 
* @param face Triangle from a Delaunay Face (input)
* @return gradient Computed gradient of triangle
**/
Vector2D calculateGradient(Triangle3D face)
{
    Vector3D u(face.vertex(0), face.vertex(1));
    Vector3D v(face.vertex(0), face.vertex(2));
    Vector3D normal = CGAL::cross_product(u, v);

    Vector2D gradient(-normal.x()/normal.z(), -normal.y()/normal.z());

    return gradient;
}

/**
* Returns an indicator of the which intersection 
* we got using gradient from face
* 
* @param face A Delaunay face
* @return int An integer power of 2 representing the intersection
**/
int getGradientIntersection(Delaunay::Face face)
{
    /*
    * Vertex(0): return 1 > log2(1) = 0
    * Vertex(1): return 2 > log2(2) = 1
    * Vertex(2): return 4 > log2(4) = 2
    * Edge opposite to Vertex(0): return 8 > log2(8) = 3 => 3-3 = 0
    * Edge opposite to Vertex(1): return 16 > log2(16) = 4 => 4-3 = 1
    * Edge opposite to Vertex(2): return 32 > log2(32) = 5 => 5-3 = 2
    */

    Point3D p = (*(face.vertex(0))).point();
    Point3D q = (*(face.vertex(1))).point();
    Point3D r = (*(face.vertex(2))).point();
    
    Point2D p2D = Point2D(p.x(), p.y());
    Point2D q2D = Point2D(q.x(), q.y()); 
    Point2D r2D = Point2D(r.x(), r.y());
    Point2D source2D = CGAL::barycenter(p2D, 0.33, q2D, 0.33, r2D, 0.33);
    
    LineSegment2D edgePQ = LineSegment2D(p2D, q2D);
    LineSegment2D edgeQR = LineSegment2D(q2D, r2D);
    LineSegment2D edgeRP = LineSegment2D(r2D, p2D);

    Vector2D gradient = calculateGradient(Triangle3D(p, q, r));
    
    Ray2D gradientRay = Ray2D(source2D, gradient);
    
    CGAL::Object gradientIntersection;

    Point2D intersectedPoint;
    
    gradientIntersection = CGAL::intersection(p2D, gradientRay);
    if (gradientIntersection != NULL) { return 1; }

    gradientIntersection = CGAL::intersection(q2D, gradientRay);
    if (gradientIntersection != NULL) { return 2; }

    gradientIntersection = CGAL::intersection(r2D, gradientRay);
    if (gradientIntersection != NULL) { return 4; }

    gradientIntersection = CGAL::intersection(edgeQR, gradientRay);
    if (CGAL::assign(intersectedPoint, gradientIntersection)) { return 8; }

    gradientIntersection = CGAL::intersection(edgeRP, gradientRay);
    if (CGAL::assign(intersectedPoint, gradientIntersection)) { return 16; }
    
    gradientIntersection = CGAL::intersection(edgePQ, gradientRay);
    if (CGAL::assign(intersectedPoint, gradientIntersection)) { return 32; }

    return 0;  
}

/**
* Calculates the path to maximum
* 
* @param tr Delaunay triangulation (input)
* @param intersected Vector containing faces of path (output & input)
**/
void calculatePath(const Delaunay& tr, std::vector<Delaunay::Face_handle> &intersected)
{
    Delaunay::Face_handle previousFace;
    Delaunay::Face_handle currentFace;

    Vector2D gradient;
    Vector2D previousGradient;

    int currentGradientIntersection;
    double gradientModule;
    double previousModule;

    currentFace = intersected.back();

    previousFace = currentFace;

    gradient = calculateGradient(tr.triangle(currentFace));
    gradientModule = (gradient.x() * gradient.x()) + (gradient.y() * gradient.y());

    while (gradientModule > 0)
    {
        currentFace = intersected.back();

        currentGradientIntersection = getGradientIntersection(*currentFace);

        if (currentGradientIntersection == 0)
        {
            std::cout << "Error on getting intersection with gradient" << std::endl;
            break;
        }
        else if (currentGradientIntersection > 4)
        {
            //std::cout << "Current gradient integer pointing to edge: " << currentGradientIntersection << std::endl;
            int indexForNextFace = log2(currentGradientIntersection) - 3;
            Delaunay::Face_handle nextFace = currentFace->neighbor(indexForNextFace);

            //std::cout << "Got neighbor" << std::endl;

            if (std::find(intersected.begin(), intersected.end(), nextFace) == intersected.end())
            {
                //std::cout << "Got next face" << std::endl;
                previousFace = currentFace;
                intersected.push_back(nextFace);
            }
            else
            {
		Delaunay::Face_handle poppedFace = currentFace;
		intersected.pop_back();
		currentFace = intersected.back();
                //std::cout << "Got maximum edge" << std::endl;
                Delaunay::Vertex_handle q;
                Delaunay::Vertex_handle r;

                if (indexForNextFace == 0)
                {
                    q = currentFace->vertex(1);
                    r = currentFace->vertex(2);
                }
                else if (indexForNextFace == 1)
                {
                    q = currentFace->vertex(0);
                    r = currentFace->vertex(2);
                }
                else if (indexForNextFace == 2)
                {
                    q = currentFace->vertex(0);
                    r = currentFace->vertex(1);
                }

                Delaunay::Vertex_handle nextVertex = q->point().z() < r->point().z() ? q : r;

                Delaunay::Face_circulator faceCt = tr.incident_faces(nextVertex);

                while (tr.is_infinite(faceCt))
                {
                    faceCt++;
                }

                Delaunay::Face_handle faceHandleCt = faceCt;

                Vector2D previousGradientLocal = calculateGradient(tr.triangle(faceHandleCt));
                Vector2D gradientLocal = previousGradient;

                Delaunay::Face_circulator done(faceCt); 

                double gradientModuleLocal;
                double previousModuleLocal;
                previousModuleLocal = (previousGradientLocal.x() * previousGradientLocal.x()) + 
                                        (previousGradientLocal.y() * previousGradientLocal.y());

                Delaunay::Face_handle candidateFromCurrent = currentFace->neighbor(indexForNextFace);
                Delaunay::Face_handle candidateFromPrevious = previousFace->neighbor(indexForNextFace);

                while (++faceCt != done)
                {
                    faceHandleCt = faceCt;

                    if (faceHandleCt == candidateFromPrevious || faceHandleCt == candidateFromCurrent)
                    {
                        gradientLocal = calculateGradient(tr.triangle(faceHandleCt));

                        gradientModuleLocal = (gradientLocal.x() * gradientLocal.x()) + 
                                                        (gradientLocal.y() * gradientLocal.y());

                        if (gradientModuleLocal > previousModuleLocal)
                        {
                            nextFace = faceHandleCt;
                            previousModuleLocal = gradientModuleLocal;
                        }
                    }
                }

                if (nextFace != poppedFace && std::find(intersected.begin(), intersected.end(), nextFace) == intersected.end())
                {
                    previousFace = currentFace;
                    intersected.push_back(nextFace);
                }
                else { break; }
            }
        }   
        else if (currentGradientIntersection < 8)
        {
            std:: cout << "Current gradient integer pointing to vertex: " << currentGradientIntersection << std::endl;
            Delaunay::Face_handle nextFace;
            int indexForPointedVertex = log2(currentGradientIntersection);
            Delaunay::Vertex_handle vertex = currentFace->vertex(indexForPointedVertex);
            
            Delaunay::Face_handle candidateFromCurrent1;
            Delaunay::Face_handle candidateFromCurrent2;

            if (indexForPointedVertex == 0) 
            { 
                candidateFromCurrent1 = currentFace->neighbor(1); 
                candidateFromCurrent2 = currentFace->neighbor(2); 
            }
            else if (indexForPointedVertex == 1) 
            { 
                candidateFromCurrent1 = currentFace->neighbor(0); 
                candidateFromCurrent2 = currentFace->neighbor(2); 
            }
            else if (indexForPointedVertex == 2) 
            { 
                candidateFromCurrent1 = currentFace->neighbor(0); 
                candidateFromCurrent2 = currentFace->neighbor(1); 
            }

            Delaunay::Face_circulator faceCt = tr.incident_faces(vertex);

            Delaunay::Face_handle faceHandleCt = faceCt;
            Vector2D previousGradientLocal = calculateGradient(tr.triangle(faceHandleCt));
            Vector2D gradientLocal = previousGradient;

            Delaunay::Face_circulator done(faceCt);

            double gradientModuleLocal;
            double previousModuleLocal;
            previousModuleLocal = (previousGradientLocal.x() * previousGradientLocal.x()) + 
                                    (previousGradientLocal.y() * previousGradientLocal.y());

            while (++faceCt != done)
            {
                faceHandleCt = faceCt;
                if (faceHandleCt == candidateFromCurrent1 && faceHandleCt == candidateFromCurrent2)
                {
                    gradient = calculateGradient(tr.triangle(faceHandleCt));
                    gradientModuleLocal = (gradientLocal.x() * gradientLocal.x()) + 
                                                    (gradientLocal.y() * gradientLocal.y());

                    if (gradientModuleLocal >= previousModuleLocal)
                    {
                        nextFace = faceHandleCt;
                        previousModuleLocal = gradientModuleLocal;
                    }
                }
            }

            if (std::find(intersected.begin(), intersected.end(), nextFace) == intersected.end())
            {
                previousFace = currentFace;
                intersected.push_back(nextFace);
            }
            else
            {
                //std::cout << "A vertex is a maximum" << std::endl;
                break;
            }
        }
    }
}


/**
* Returns projected Z from the screen's point of view when mouse is left clicked.
* 
* @param position Where the left click happened
* @return m_projectedZ A GLdouble with the Z value of the left click.
**/
GLdouble getMouseZ(Point3D position)
{
    GLdouble matModelView[16], matProjection[16]; 
    GLint viewport[4]; 
    GLdouble m_projectedX, m_projectedY, m_projectedZ;

    // get matrix and viewport:
    glGetDoublev( GL_MODELVIEW_MATRIX, matModelView ); 
    glGetDoublev( GL_PROJECTION_MATRIX, matProjection ); 
    glGetIntegerv( GL_VIEWPORT, viewport ); 

    gluProject(position[0], position[1], position[2], matModelView, matProjection, 
               viewport, &m_projectedX, &m_projectedY, &m_projectedZ); 

    return m_projectedZ;
}

/**
* Returns the triangle that is closest to the viewer.
*
* @param tr  Delaunay triangulation (model)
* @param seg Ray from mouse click
* 
* @return returnIt An iterator(pointer) to the closest triangle
**/
Delaunay::Face_handle getClosestTriangle (const Delaunay& tr, LineSegment3D seg)
{
    Delaunay::Finite_faces_iterator it = tr.finite_faces_begin();
    Delaunay::Finite_faces_iterator returnIt = tr.finite_faces_end();
    
    Point3D currentPoint;
    Point3D previousPoint;
    
    CGAL::Object result;
    bool firstTriangleFound = false;

    for (it = tr.finite_faces_begin(); it != tr.finite_faces_end(); it++) 
    {
        Delaunay::Face trFace = *it;

        Point3D p = (*(trFace.vertex(0))).point();
        Point3D q = (*(trFace.vertex(1))).point();
        Point3D r = (*(trFace.vertex(2))).point();

        Triangle3D face(p, q, r);
        result = CGAL::intersection(face, seg);

        //The point with closest Z represents the closest triangle
        if (CGAL::assign(currentPoint, result))
        {
            if(firstTriangleFound == false)
            {
                firstTriangleFound = true;
                previousPoint = currentPoint;
                returnIt = it;
            }

            GLdouble currentZ = getMouseZ(currentPoint);
            GLdouble previousZ = getMouseZ(previousPoint);

            if(currentZ < previousZ)
            {
                previousPoint = currentPoint;
                returnIt = it;
            }
        }
    }

    return returnIt;
}

float firstColor = 0.0f;
float lastColor = 1.0f;
float currentColor = 0.1f;
float colorChangeRate = 0.015f;

/**
 * Draws a Delaunay triangulation
 *
 * @param tr  Delaunay triangulation (inpu#04F100t)
 * @param intersectedFaces A vector containing faces intersected on the path to maximum
 **/
void draw (const Delaunay& tr, const std::vector<Delaunay::Face_handle> intersectedFaces)
{
    // Draw triangles
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glBegin(GL_TRIANGLES);
    Delaunay::Finite_faces_iterator it;
    for (it = tr.finite_faces_begin (); it != tr.finite_faces_end (); ++it) {
        Delaunay::Face f = *it;
        glColor3f (0.5, 0.5, 0.5);  // gray
        for (int i = 0; i < intersectedFaces.size(); i++)
        {
            if (it.base() == intersectedFaces[0])
            {
                glColor3f (1.0f, 0.0f, 0.0f); 
            }
            else if(it.base() == intersectedFaces[i])
            {
                glColor3f (currentColor, currentColor, lastColor);
            }
        }
        for (int i = 0; i < 3; i++) {
            Point3D p = (*(f.vertex (i))).point();
            glVertex3f (p[0], p[1], p[2]);
        }
    }
    glEnd ();
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Draw Triangle edges
    for (it = tr.finite_faces_begin (); it != tr.finite_faces_end (); ++it) {
        Delaunay::Face f = *it;
        glBegin(GL_LINE_LOOP);
        glColor3f (1.0, 1.0, 1.0);  // white
        for (int i = 0; i < 3; i++) {
            Point3D p = (*(f.vertex (i))).point();
            glVertex3f (p[0], p[1], p[2]);
        }
        glEnd ();
    }
}


/**
 * Global variables
 **/
Delaunay dt; // A Delaunay Triangulation
LineSegment3D currentSegment; //Segment for mouse ray
std::vector<Delaunay::Face_handle> intersectedFaces;

Point3D center;  // Center of dt´s minimum enclosing parallelepiped
int width = 800, height = 600;  // Window size
int activebutton;  // Which mouse button was pressed
int xmouse, ymouse;  // Coords where mouse was pressed
double xangle = 0.0, yangle = 0.0; // Angles for viewing terrain
double modelview [16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; // Current modelview matrix
double rotMatrixX [16] = {1, 0, 0, 0, 0, cos(0.0), -sin(0.0), 0, 0, sin(0.0), cos(0.0), 0, 0, 0, 0, 1};
double rotMatrixY [16] = {cos(0.0), 0, sin(0.0), 0, 0, 1, 0, 0, -sin(0.0), 0, cos(0.0), 0, 0, 0, 0, 1};


void init ()
{
    // Create an orthogonal projection system for viewing
    // dt from any angle
    Point3D min, max;
    limits (dt, min, max);
    Vector3D size = 0.5 * (max - min);
    center = min + size;
    double radius = sqrt (size * size);
    size = Vector3D (radius, radius, radius);
    min = center - size;
    max = center + size;
    glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
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
    glLoadIdentity();
    glTranslatef (center[0], center[1], center[2]);
    glRotatef (xangle, 1.0, 0.0, 0.0);
    glRotatef (yangle, 0.0, 1.0, 0.0);
    glTranslatef (-center[0], -center[1], -center[2]);
    glMultMatrixd (modelview);
    draw (dt, intersectedFaces);
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(currentSegment.source()[0], currentSegment.source()[1], currentSegment.source()[2]);
    glVertex3f(currentSegment.target()[0], currentSegment.target()[1], currentSegment.target()[2]);
    glEnd();
    
    currentColor += colorChangeRate;
    if((currentColor > (lastColor-0.01f)) || (currentColor < (firstColor+0.01f)))
    {
        colorChangeRate *= -1;
    }
    //cout << currentColor << endl;
    
    glutPostRedisplay ();
    glutSwapBuffers();
}

void reshape (int wid, int hgt)
{
    glViewport(0,0, wid, hgt);
}

/**
 * Creates a segment from mouse click position to the world's most distant z.
 * 
 * @param mouseX Mouse's X position
 * @param mouseY Mouse's Y position
 **/
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

	//Defining segment
	Point3D startPoint(m_startX, m_startY, m_startZ);
	Point3D endPoint(m_endX, m_endY, m_endZ);
	currentSegment = LineSegment3D(startPoint, endPoint);
} 

void mouseClickHandler (int button, int state, int x, int y)
{
	xmouse = x;
	ymouse = y;
	if (button == GLUT_LEFT_BUTTON) 
        {
            if (state == GLUT_DOWN) 
            {
		//glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
		//xangle = 0; yangle = 0;
                firstColor = 0.0f;
                lastColor = 1.0f;
                currentColor = 0.1f;
                colorChangeRate = 0.015f;
                
                createMouseRay(x, y);
                intersectedFaces.clear();
                Delaunay::Face_handle firstFace = getClosestTriangle(dt, currentSegment);
                if (firstFace != dt.finite_faces_end().base())
                {
                    intersectedFaces.push_back(firstFace);
                    calculatePath(dt, intersectedFaces);
                }
            }
	}
	activebutton = button;  //saves the camera rotation
	glutPostRedisplay ();
}

/**
* This function only detects the mouse movement while we are clicking a button
**/
void mouseActiveMotionHandler (int x, int y)
{
    if (activebutton == GLUT_RIGHT_BUTTON) 
    {
        yangle = (x - xmouse) * 360 / width;
        xangle = (y - ymouse) * 360 / height;
    }
    glutPostRedisplay ();
}

int main(int argc, char * argv [])
{
    // Read the terrain data into dt
    std::ifstream terrainfile ("terraindata.txt");
    std::istream_iterator<Point3D> beginpt(terrainfile);
    std::istream_iterator<Point3D> endpt;
    dt.insert(beginpt, endpt);

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
    
    glutReshapeFunc (reshape);
    
    init ();
    glutMainLoop();
    return 0;

}
