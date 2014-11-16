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
typedef Repr::Point_3 Point;                            //A Point in 3D
typedef Repr::Point_2 Point2D;                          //A Point in 2D
typedef Repr::Triangle_3 Triangle;                      //Triangle in 3D
typedef Repr::Triangle_2 Triangle2D;                    //Triangle in 2D
typedef Repr::Vector_3 Vector;                          //A Vector in 3D
typedef Repr::Vector_2 Vector2D;                        //A Vector in 2D
typedef Repr::Segment_3 LineSegment;                    //A 3D line segment
typedef Repr::Segment_2 LineSegment2D;                  //A 2D LineSegment
typedef Repr::Ray_2 Ray2D;                              //2D Ray
typedef Repr::Ray_3 Ray;                              	//3D Ray
typedef Repr::Line_3 Line;								//3D Line
typedef Repr::Plane_3 Plane;							//3D Plane

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

/**
* Returns gradient of a triangle
* 
* @param face Triangle from a Delaunay Face (input)
* @return gradient Computed gradient of triangle
**/
Vector2D calcGradientOfTriangle(Triangle face)
{
    Vector u(face.vertex(0), face.vertex(1));
    Vector v(face.vertex(0), face.vertex(2));
    Vector normal = CGAL::cross_product(u, v);
    Vector2D gradient;

	if (normal.z() != 0)
	{
		gradient = Vector2D(-normal.x()/normal.z(), -normal.y()/normal.z());
	}
	else
	{
		std::cout << "Normal with z = 0" << std::endl;
	}

    return gradient;
}

/**
* Returns an indicator of the which intersection 
* we got using gradient from face
* 
* @param face A Delaunay face
* @return int An integer power of 2 representing the intersection
**/
int getGradientIntersection(Delaunay::Face face, Point &intersectedPointOut)
{
    /*
    * Vertex(0): return 1 > log2(1) = 0
    * Vertex(1): return 2 > log2(2) = 1
    * Vertex(2): return 4 > log2(4) = 2
    * Edge opposite to Vertex(0): return 8 > log2(8) = 3 => 3-3 = 0
    * Edge opposite to Vertex(1): return 16 > log2(16) = 4 => 4-3 = 1
    * Edge opposite to Vertex(2): return 32 > log2(32) = 5 => 5-3 = 2
    */

    Vector2D gradient;
    double gradientModule;

    Point p, q, r;
    Point2D p2D, q2D, r2D, source2D;
    Triangle2D projectedTriangle;
    LineSegment2D edgePQ, edgeQR, edgeRP;

    Ray2D gradientRay;
    CGAL::Object gradientIntersection;

    Point2D intersectedPoint;
    
    Plane trianglePlane;

    p = (*(face.vertex(0))).point();
    q = (*(face.vertex(1))).point();
    r = (*(face.vertex(2))).point(); 
    
    trianglePlane = Plane(p, q, r); 

    p2D = Point2D(p.x(), p.y());
    q2D = Point2D(q.x(), q.y());
    r2D = Point2D(r.x(), r.y());

    edgePQ = LineSegment2D(p2D, q2D);
    edgeQR = LineSegment2D(q2D, r2D);
    edgeRP = LineSegment2D(r2D, p2D);

    projectedTriangle = Triangle2D(p2D, q2D, r2D);
    source2D = CGAL::centroid(projectedTriangle);

	gradient = calcGradientOfTriangle(Triangle(p, q, r));
	gradientModule = sqrt((gradient.x()*gradient.x()) + (gradient.y()*gradient.y()));
	if (gradientModule > 0)
	{
		gradient = Vector2D(gradient.x()/gradientModule, gradient.y()/gradientModule);
	}
    gradientRay = Ray2D(source2D, gradient);
    
    std::cout << "Gradient: " << gradient << std::endl;

    if (gradientRay.has_on(p2D)) 
    { 
		std::cout << "Intersected p" << std::endl;
		intersectedPointOut = p;
		return 1; 
	}

    if (gradientRay.has_on(q2D)) 
    { 
		std::cout << "Intersected q" << std::endl;
		intersectedPointOut = q;
		return 2; 
	}

    if (gradientRay.has_on(r2D)) 
    { 
		std::cout << "Intersected r" << std::endl;
		intersectedPointOut = r;
		return 4; 
	}
    
    gradientIntersection = CGAL::intersection(edgeQR, gradientRay);
    if (gradientIntersection != NULL) 
    { 
		std::cout << "Intersected QR" << std::endl;
		Point2D auxPoint;
		if (CGAL::assign(auxPoint, gradientIntersection))
		{
			double pointZ = -trianglePlane.a()*auxPoint.x();
			pointZ += -trianglePlane.b()*auxPoint.y();
			pointZ += -trianglePlane.d();
			pointZ = pointZ / trianglePlane.c();
			intersectedPointOut = Point(auxPoint.x(), auxPoint.y(), pointZ);
			return 8;
		} 
	}

    gradientIntersection = CGAL::intersection(edgeRP, gradientRay);
    if (gradientIntersection != NULL) 
    { 
		std::cout << "Intersected RP" << std::endl;
		Point2D auxPoint;
		if (CGAL::assign(auxPoint, gradientIntersection))
		{
			double pointZ = -trianglePlane.a()*auxPoint.x();
			pointZ += -trianglePlane.b()*auxPoint.y();
			pointZ += -trianglePlane.d();
			pointZ = pointZ / trianglePlane.c();
			intersectedPointOut = Point(auxPoint.x(), auxPoint.y(), pointZ);
			return 16;
		}
	}

    gradientIntersection = CGAL::intersection(edgePQ, gradientRay);
    if (gradientIntersection != NULL) 
    { 
		std::cout << "Intersected PQ" << std::endl;
		Point2D auxPoint;
		if (CGAL::assign(auxPoint, gradientIntersection))
		{
			double pointZ = -trianglePlane.a()*auxPoint.x();
			pointZ += -trianglePlane.b()*auxPoint.y();
			pointZ += -trianglePlane.d();
			pointZ = pointZ / trianglePlane.c();
			intersectedPointOut = Point(auxPoint.x(), auxPoint.y(), pointZ);
			return 32;
		}
	}

    Point returnPoint = p.z() < q.z()? p : q;
    returnPoint = returnPoint.z() < r.z()? returnPoint : r;
    intersectedPointOut = returnPoint;
    
    if(returnPoint == p) {return 1;}
    else if(returnPoint == q) {return 2;}
    else if(returnPoint == r) {return 4;}
    
    return 0;
}

/**
* Calculates the path to maximum
* 
* @param tr Delaunay triangulation (input)
* @param intersectedPoints Vector containing points of path (output & input)
* @param firstFace Start point to path (input)
**/
void calcPathToMaximum(const Delaunay& tr, std::vector<Point> &intersectedPoints, Delaunay::Face_handle firstFace)
{
	std::cout << "/////////////////////////////////////////////////////////////" << std::endl;
	std::cout << "Starting path finder" << std::endl;
	
	std::vector<Delaunay::Face_handle> intersected;
    Delaunay::Face_handle currentFace;
    Vector2D gradient;
    
    Point currentIntersectedPoint;
    
    std::vector<Delaunay::Vertex_handle> visitedVertices;
    bool gotVertex = false;
    bool gotMaximumEdge = false;
    bool foundMax = false;

    int currentGradientIntersection;

	intersected.push_back(firstFace);

    while (!foundMax)
    {
		//Am I dealing with a face?
		if (!gotVertex)
		{
			currentFace = intersected.back();

			currentGradientIntersection = getGradientIntersection(*currentFace, currentIntersectedPoint);
			
			//Is gradient intersecting nothing like vertex or edge?
			if (currentGradientIntersection == 0)
			{ 
				std::cout << "Error on gradient intersection calc" << std::endl; 
				break;
			}
			//Is gradient intersecting edge?
			else if (currentGradientIntersection > 4)
			{
				std::cout << "Current gradient integer pointing to edge: " << currentGradientIntersection << std::endl;
				int indexForNextFace = log2(currentGradientIntersection) - 3;
				Delaunay::Face_handle nextFace = currentFace->neighbor(indexForNextFace);

				std::cout << "Got neighbor" << std::endl;

				//Is neighbor face valid and not visited yet?
				if (std::find(intersected.begin(), intersected.end(), nextFace) == intersected.end() && !tr.is_infinite(nextFace))
				{
					std::cout << "Got next face" << std::endl;
					intersectedPoints.push_back(currentIntersectedPoint);
					intersected.push_back(nextFace);
					gotVertex = false;
				}
				//Neighbor face is not valid or it's been already visited
				else
				{
					std::cout << "Got a face already visited" << std::endl;
					
					gotMaximumEdge = true;
					
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


					Delaunay::Vertex_handle nextVertex = q->point().z() > r->point().z() ? q : r;
					
					//Is elected vertex valid and not visited yet?
					if(!tr.is_infinite(nextVertex) && std::find(visitedVertices.begin(), visitedVertices.end(), nextVertex) == visitedVertices.end())
					{
						intersectedPoints.push_back(nextVertex->point());						
						visitedVertices.push_back(nextVertex);
						gotVertex = true;
					}
					//Elected vertex is not valid or it's been visited before
					else
					{
						nextVertex = nextVertex == q ? r : q;
						if(!tr.is_infinite(nextVertex) && std::find(visitedVertices.begin(), visitedVertices.end(), nextVertex) == visitedVertices.end())
						{
							intersectedPoints.push_back(nextVertex->point());						
							visitedVertices.push_back(nextVertex);
							gotVertex = true;
						}
						else
						{
							std::cout << "Got infinite vertices" << std::endl;
							gotVertex = false;
							break;
						}
					}
				}
			}   
			//Is gradient intersecting vertex?
			else if (currentGradientIntersection < 8)
			{
				std:: cout << "Current gradient integer pointing to vertex: " << currentGradientIntersection << std::endl;
				int indexForVertex = log2(currentGradientIntersection);
				Delaunay::Vertex_handle vertex = currentFace->vertex(indexForVertex);
				
				//Is vertex valid?
				if (!tr.is_infinite(vertex) && std::find(visitedVertices.begin(), visitedVertices.end(), vertex) == visitedVertices.end())
				{
					intersectedPoints.push_back(currentIntersectedPoint);
					visitedVertices.push_back(vertex);
					gotVertex = true;
				}
				else
				{
					std::cout << "Gradient pointing to a vertex already visited" << std::endl;
					break;
				}
			}
		}
		//Am I dealing with a vertex?
		else
		{
			std::cout << "Got vertex" << std::endl;
			Delaunay::Vertex_handle currentVertex = visitedVertices.back();
		
			//Am I dealing with a vertex to which gradient was pointing?
			//If yes, search incident faces
			if (!gotMaximumEdge)
			{
				std::cout << "Searching faces" << std::endl;
				Delaunay::Face_circulator faceCt = tr.incident_faces(currentVertex), done(faceCt);
				
				bool foundValidFacePath = false;
						
				while (tr.is_infinite(faceCt))
				{ 
					faceCt++; 
					if (faceCt == done) 
					{ 
						std::cout << "There is only infinite faces" << std::endl;
						break; 
					}
				}
				
				Delaunay::Face_handle searchedFace = faceCt;
				Vector2D currentGradientLocal = calcGradientOfTriangle(tr.triangle(searchedFace));
				double currentGradientModuleLocal = (currentGradientLocal.x() * currentGradientLocal.x()) + 
										(currentGradientLocal.y() * currentGradientLocal.y());
				double previousGradientModuleLocal = currentGradientModuleLocal;
				
				if (!tr.is_infinite(searchedFace) && 
					std::find(intersected.begin(), intersected.end(), searchedFace) == intersected.end())
				{
					foundValidFacePath = true;
				}
				
				faceCt++;
				
				//Searching for faces where gradient points to an opposite edge
				//I pick the face that has the greater gradient module. Why? It goes up faster
				while (faceCt != done)
				{
					if (std::find(intersected.begin(), intersected.end(), faceCt) == intersected.end() && !tr.is_infinite(faceCt))
					{
						Point auxPoint;
						Delaunay::Face_handle localFace = faceCt;
						int gradientIntersectionFromCandidate = getGradientIntersection(*localFace, auxPoint);
						if (log2(gradientIntersectionFromCandidate) != localFace->index(currentVertex))
						{
							currentGradientLocal = calcGradientOfTriangle(tr.triangle(localFace));
							currentGradientModuleLocal = (currentGradientLocal.x() * currentGradientLocal.x()) + 
										(currentGradientLocal.y() * currentGradientLocal.y());
							if (currentGradientModuleLocal > previousGradientModuleLocal)
							{
								searchedFace = localFace;
								previousGradientModuleLocal = currentGradientModuleLocal;
								foundValidFacePath = true;
							}
						}
					}
					
					faceCt++;
				}
			
				//Is there a valid face to keep walking?
				if (foundValidFacePath)
				{
					std::cout << "Found valid face path" << std::endl;
					intersected.push_back(searchedFace);
					gotVertex = false;
				}
				//Should I try walk on edges?
				else
				{
					gotVertex = true;
					gotMaximumEdge = true;
				}
			}
			//Am I dealing with a vertex from a maximum edge?
			else 
			{
				std::cout << "Searching edges" << std::endl;
				Delaunay::Edge_circulator edgeCt = tr.incident_edges(currentVertex), doneEdge(edgeCt);
			
				while (tr.is_infinite(edgeCt))
				{ 
					edgeCt++; 
					if (edgeCt == doneEdge) { break; }
				}
				
				Delaunay::Edge searchedEdge = *edgeCt;
				
				Delaunay::Vertex_handle vertexA = searchedEdge.first->vertex((searchedEdge.second+1)%3);
				Delaunay::Vertex_handle vertexB = searchedEdge.first->vertex((searchedEdge.second+2)%3);

				//Next vertex can't be the current vertex
				Delaunay::Vertex_handle nextVertexLocal = currentVertex == vertexA ? vertexB : vertexA;
				
				while (tr.is_infinite(nextVertexLocal) && ++edgeCt != doneEdge)
				{
					searchedEdge = *edgeCt;
					vertexA = searchedEdge.first->vertex((searchedEdge.second+1)%3);
					vertexB = searchedEdge.first->vertex((searchedEdge.second+2)%3);
					nextVertexLocal = currentVertex == vertexA ? vertexB : vertexA;
				}
				
				double currentVertexZ = nextVertexLocal->point().z();
				double previousVertexZ = currentVertexZ;
				
				bool foundValidEdgePath = false;
				
				if (!tr.is_infinite(nextVertexLocal) && 
					std::find(visitedVertices.begin(), visitedVertices.end(), nextVertexLocal) == visitedVertices.end() && 
					currentVertexZ >= currentVertex->point().z())
				{
					foundValidEdgePath = true;
				}
				
				edgeCt++;
				
				//Searching edges that has the greater Z
				//Can't figure out why the hell I should look for greater Z if the z-axis grows on another direction
				//But it works...
				while (edgeCt != doneEdge)
				{
					if (!tr.is_infinite(edgeCt))
					{
						Delaunay::Edge localEdge = *edgeCt;
						vertexA = localEdge.first->vertex((localEdge.second+1)%3);
						vertexB = localEdge.first->vertex((localEdge.second+2)%3);
						
						nextVertexLocal = currentVertex == vertexA ? vertexB : vertexA;
						if (!tr.is_infinite(nextVertexLocal) && 
							std::find(visitedVertices.begin(), visitedVertices.end(), nextVertexLocal) == visitedVertices.end())
						{
							currentVertexZ = nextVertexLocal->point().z();
							
							if (currentVertexZ > previousVertexZ && currentVertexZ >= currentVertex->point().z())
							{
								searchedEdge = localEdge;
								previousVertexZ = currentVertexZ;
								foundValidEdgePath = true;
							}
						}
					}
					
					edgeCt++;
				}
				
				//Is there a valid edge to keep walking?
				if (foundValidEdgePath)
				{
					vertexA = searchedEdge.first->vertex((searchedEdge.second+1)%3);
					vertexB = searchedEdge.first->vertex((searchedEdge.second+2)%3);
						
					nextVertexLocal = currentVertex == vertexA ? vertexB : vertexA;
					
					if (!tr.is_infinite(nextVertexLocal))
					{
						intersectedPoints.push_back(nextVertexLocal->point());
						visitedVertices.push_back(nextVertexLocal);
						gotVertex = true;
					}
					else
					{
						std::cout << "Found maximum because next vertex is infinite" << std::endl;
						foundMax = true;
						break;
					}
				}
				//So... have I found the maximum?
				else
				{
					std::cout << "Found maximum because there is no vertex with minor z" << std::endl;
					foundMax = true;
					break;
				}
			}
		}
    }
}


/**
* Returns projected Z from the screen's point of view
* 
* @param position A point in 3D to have Z projected
* @return m_projectedZ A GLdouble with projected value of Z
**/
GLdouble getProjectedZ(Point position)
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
* Gets intersected triangle that has minor z
* Triangle is the closest to the screen
*
* @param tr  Delaunay triangulation (input)
* @param seg Line Segment representing ray from mouse click
* @param intersectedPoints Vector to receive point of click intersection
* 
* @return returnIt An iterator to the closes face returned as Face_handle
**/
Delaunay::Face_handle getFirstIntersectedFace (const Delaunay& tr, LineSegment seg, std::vector<Point> &intersectedPoints)
{
    Delaunay::Finite_faces_iterator it = tr.finite_faces_begin();
    Delaunay::Finite_faces_iterator returnIt = tr.finite_faces_end();
    Point currentPoint;
    Point previousPoint;
    CGAL::Object result;
    bool firstTriangleFound = false;

    for (it = tr.finite_faces_begin(); it != tr.finite_faces_end(); it++) {
		if (!tr.is_infinite(it))
		{
			Delaunay::Face trFace = *it;

			Point p = (*(trFace.vertex(0))).point();
			Point q = (*(trFace.vertex(1))).point();
			Point r = (*(trFace.vertex(2))).point();

			Triangle face(p, q, r);
			result = CGAL::intersection(face, seg);

			if (CGAL::assign(currentPoint, result))
			{
				if(firstTriangleFound == false)
				{
					firstTriangleFound = true;
					previousPoint = currentPoint;
					returnIt = it;
				}

				GLdouble currentZ = getProjectedZ(currentPoint);
				GLdouble previousZ = getProjectedZ(previousPoint);

				if(currentZ < previousZ)
				{
					previousPoint = currentPoint;
					returnIt = it;
				}
			}
		}
    }

	intersectedPoints.push_back(currentPoint);
    return returnIt;
}

/**
 * Draws the path formed with points
 * 
 * @param intersectedPoints A vector containing points to build the path
 * */
void draw (const std::vector<Point> intersectedPoints)
{
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(10.0f);
	glBegin(GL_LINE_STRIP);
    glColor3f (0.19, 0.95, 0);  // green
	for (int i = 0; i < intersectedPoints.size(); i++) {
        Point p = intersectedPoints[i];
        glVertex3f (p[0], p[1], p[2]);
    }
    glEnd();
    glDisable(GL_LINE_SMOOTH);
}

/**
 * Draws a Delaunay triangulation
 *
 * @param tr  Delaunay triangulation (inpu#04F100t)
 **/
void draw (const Delaunay& tr)
{
    // Draw triangles
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glBegin(GL_TRIANGLES);
    Delaunay::Finite_faces_iterator it;
    for (it = tr.finite_faces_begin (); it != tr.finite_faces_end (); ++it) {
        Delaunay::Face f = *it;
        glColor3f (1.0, 1.0, 1.0);  // gray
        for (int i = 0; i < 3; i++) {
            Point p = (*(f.vertex (i))).point();
            glVertex3f (p[0], p[1], p[2]);
        }
    }
    glEnd ();
    glDisable(GL_POLYGON_OFFSET_FILL);


    // Draw Triangle edges
    for (it = tr.finite_faces_begin (); it != tr.finite_faces_end (); ++it) {
        Delaunay::Face f = *it;
        glBegin(GL_LINE_LOOP);
        glColor3f (0.8, 0.8, 0.8);  // white
        for (int i = 0; i < 3; i++) {
            Point p = (*(f.vertex (i))).point();
            glVertex3f (p[0], p[1], p[2]);
        }
        glEnd ();
    }

}


/**
 * Global variables
 **/
Delaunay dt; // A Delaunay Triangulation
LineSegment currentSegment; //Segment for mouse ray
std::vector<Point> intersectedPoints; //Vector with points of path to max

Point center;  // Center of dt´s minimum enclosing parallelepiped
int width = 800, height = 600;  // Window size
int activebutton;  // Which mouse button was pressed
int xmouse, ymouse;  // Coords where mouse was pressed
double xangle = 0.0, yangle = 0.0; // Angles for viewing terrain
double modelview [16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; // Current modelview matrix


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
	draw (dt);
	draw (intersectedPoints);
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(currentSegment.source()[0], currentSegment.source()[1], currentSegment.source()[2]);
	glVertex3f(currentSegment.target()[0], currentSegment.target()[1], currentSegment.target()[2]);
	glEnd();
	glutSwapBuffers();
}

void reshape (int wid, int hgt)
{
    glViewport(0,0, wid, hgt);
}

/**
 * Creates a segment from mouse click position 'till backs
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
	Point startPoint(m_startX, m_startY, m_startZ);
	Point endPoint(m_endX, m_endY, m_endZ);
	currentSegment = LineSegment(startPoint, endPoint);
} 

void mouseClickHandler (int button, int state, int x, int y)
{
	xmouse = x;
	ymouse = y;
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
            createMouseRay(x, y);
            intersectedPoints.clear();
            Delaunay::Face_handle firstFace = getFirstIntersectedFace(dt, currentSegment, intersectedPoints);
            if (firstFace != dt.finite_faces_end().base())
            {
                calcPathToMaximum(dt, intersectedPoints, firstFace);
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
    std::istream_iterator<Point> beginpt(terrainfile);
    std::istream_iterator<Point> endpt;
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
