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


typedef CGAL::Cartesian<double> Repr;
typedef CGAL::Projection_traits_xy_3<Repr> Gt;
typedef CGAL::Delaunay_triangulation_2<Gt> Delaunay; 
//2D
typedef Repr::Point_2 Point2D;
typedef Repr::Triangle_2 Triangle2D;
typedef Repr::Vector_2 Vector2D;  
typedef Repr::Segment_2 LineSegment2D; 
typedef Repr::Ray_2 Ray2D; 
//3D
typedef Repr::Point_3 Point3D;
typedef Repr::Triangle_3 Triangle3D;  
typedef Repr::Vector_3 Vector3D; 
typedef Repr::Segment_3 LineSegment3D; 
typedef Repr::Plane_3 Plane3D; 



void limits(Delaunay& triang, Point3D& min, Point3D& max) 
{
    Delaunay::Point_iterator ip = triang.points_begin();
    assert(ip != triang.points_end());
    double mincoord[3], maxcoord[3];
    Point3D p = *ip++;
    for (int i = 0; i < 3; ++i) {
        mincoord [i] = maxcoord [i] = p [i];
    }
    while (ip != triang.points_end()) {
        Point3D p = *ip++;
        for (int i = 0; i < 3; ++i) {
            if (p [i] < mincoord [i]) mincoord [i] = p[i];
            else if (p [i] > maxcoord [i]) maxcoord [i] = p[i];
        }
    }
    min = Point3D(mincoord [0], mincoord [1], mincoord [2]);
    max = Point3D(maxcoord [0], maxcoord [1], maxcoord [2]);
}





Vector2D calculateGradient(Triangle3D face) 
{
    Vector3D u(face.vertex(0), face.vertex(1));
    Vector3D v(face.vertex(0), face.vertex(2));
    Vector3D normal = CGAL::cross_product(u, v);
    
    Vector2D gradient;
    //Avoiding division by zero exception
    if (normal.z() != 0) 
    {
        gradient = Vector2D(-normal.x() / normal.z(), -normal.y() / normal.z());
    } 

    return gradient;
}

double getNewZ(Plane3D trianglePlane, Point2D intersectedPoint)
{
    double newZ = -trianglePlane.a() * intersectedPoint.x();
    newZ += -trianglePlane.b() * intersectedPoint.y();
    newZ += -trianglePlane.d();
    newZ = newZ / trianglePlane.c();
    
    return newZ;
}


int getGradientIntersection(Delaunay::Face face, Point3D &intersectedPointOut) 
{
    Point3D p = (*(face.vertex(0))).point();
    Point3D q = (*(face.vertex(1))).point();
    Point3D r = (*(face.vertex(2))).point();
    
    //Transforming above points to 2D for intersection detection
    Point2D p2D = Point2D(p.x(), p.y());
    Point2D q2D = Point2D(q.x(), q.y());
    Point2D r2D = Point2D(r.x(), r.y());
    
    //Creating a 2D triangle with above points
    Triangle2D projectedTriangle = Triangle2D(p2D, q2D, r2D);
    
    //Representing the edges of the above 2D triangle
    LineSegment2D edgePQ = LineSegment2D(p2D, q2D);
    LineSegment2D edgeQR = LineSegment2D(q2D, r2D);
    LineSegment2D edgeRP = LineSegment2D(r2D, p2D);

    //Calculating the gradient of the triangle
    Vector2D gradient = calculateGradient(Triangle3D(p, q, r));
    double gradientModule = sqrt((gradient.x() * gradient.x()) + (gradient.y() * gradient.y()));
    if (gradientModule > 0) 
    {
        gradient = Vector2D(gradient.x() / gradientModule, gradient.y() / gradientModule);
    }
    Point2D source2D = CGAL::centroid(projectedTriangle);
    Ray2D gradientRay = Ray2D(source2D, gradient); //Used only for intersection detection

    
    //Checking gradient intersection with the triangle points
    if (gradientRay.has_on(p2D)) 
    {
        intersectedPointOut = p;
        return 1;
    }
    if (gradientRay.has_on(q2D)) 
    {
        intersectedPointOut = q;
        return 2;
    }
    if (gradientRay.has_on(r2D)) 
    {
        intersectedPointOut = r;
        return 4;
    }

    //Checking gradient intersection with the triangle edges
    CGAL::Object gradientIntersection;

    Plane3D trianglePlane = Plane3D(p, q, r);
    
    Point2D newIntersectedPoint;
    
    gradientIntersection = CGAL::intersection(edgeQR, gradientRay);
    if (gradientIntersection != NULL) 
    {
        if (CGAL::assign(newIntersectedPoint, gradientIntersection)) 
        {
            double pointZ = getNewZ(trianglePlane, newIntersectedPoint);
            intersectedPointOut = Point3D(newIntersectedPoint.x(), newIntersectedPoint.y(), pointZ);
            return 8;
        }
    }

    gradientIntersection = CGAL::intersection(edgeRP, gradientRay);
    if (gradientIntersection != NULL) 
    {
        if (CGAL::assign(newIntersectedPoint, gradientIntersection)) 
        {
            double pointZ = getNewZ(trianglePlane, newIntersectedPoint);
            intersectedPointOut = Point3D(newIntersectedPoint.x(), newIntersectedPoint.y(), pointZ);
            return 16;
        }
    }

    gradientIntersection = CGAL::intersection(edgePQ, gradientRay);
    if (gradientIntersection != NULL) 
    {
        if (CGAL::assign(newIntersectedPoint, gradientIntersection)) 
        {
            double pointZ = getNewZ(trianglePlane, newIntersectedPoint);
            intersectedPointOut = Point3D(newIntersectedPoint.x(), newIntersectedPoint.y(), pointZ);
            return 32;
        }
    }

    Point3D returnPoint = p.z() < q.z() ? p : q;
    returnPoint = returnPoint.z() < r.z() ? returnPoint : r;
    intersectedPointOut = returnPoint;

    if (returnPoint == p) 
    {
        return 1;
    } 
    else if (returnPoint == q) 
    {
        return 2;
    } 
    else if (returnPoint == r) 
    {
        return 4;
    }

    //cout << "Gradient Value: " << gradientRay << endl;
    
    return 0;
}




void calculatePath(const Delaunay& tr, std::vector<Point3D> &pathPoints, 
                        Delaunay::Face_handle firstFace) 
{

    std::vector<Delaunay::Face_handle> intersected;
    Delaunay::Face_handle currentFace;
    
    Point3D currentIntersectedPoint;

    std::vector<Delaunay::Vertex_handle> visitedVertices;
    bool gotVertex = false;
    bool walkOnEdge = false;
    bool foundMax = false;

    int currentGradientIntersection;

    intersected.push_back(firstFace);

    while (!foundMax) 
    {
        if (!gotVertex) 
        {
            currentFace = intersected.back();

            currentGradientIntersection = getGradientIntersection(*currentFace, currentIntersectedPoint);

            //Intersection with edges
            if (currentGradientIntersection > 4) 
            {
                int indexForNextFace = log2(currentGradientIntersection) - 3;
                Delaunay::Face_handle nextFace = currentFace->neighbor(indexForNextFace);

                //Checking if face is not already visited and is not infinite
                if (std::find(intersected.begin(), intersected.end(), nextFace) == intersected.end() && !tr.is_infinite(nextFace)) 
                {
                    //cout << "Next face found!" << endl;
                    pathPoints.push_back(currentIntersectedPoint);
                    intersected.push_back(nextFace);
                    gotVertex = false;
                }                   
                else 
                {
                    //Let's walk on the edge
                    walkOnEdge = true;

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

                    //Checking if vertex is not already visited and is not infinite
                    if (!tr.is_infinite(nextVertex) && std::find(visitedVertices.begin(), visitedVertices.end(), nextVertex) == visitedVertices.end()) {
                        pathPoints.push_back(nextVertex->point());
                        visitedVertices.push_back(nextVertex);
                        gotVertex = true;
                    }                        
                    else 
                    {
                        //Checking if vertex is not already visited and is not infinite
                        nextVertex = nextVertex == q ? r : q;
                        if (!tr.is_infinite(nextVertex) && std::find(visitedVertices.begin(), visitedVertices.end(), nextVertex) == visitedVertices.end()) 
                        {
                            pathPoints.push_back(nextVertex->point());
                            visitedVertices.push_back(nextVertex);
                            gotVertex = true;
                        } 
                        //No more vertices to visit on this face
                        else 
                        {
                            gotVertex = false;
                            break;
                        }
                    }
                }
            }
            //Intersection with a vertex
            else if (currentGradientIntersection < 8) 
            {
                int indexForVertex = log2(currentGradientIntersection);
                Delaunay::Vertex_handle vertex = currentFace->vertex(indexForVertex);

                //Checking if vertex is not already visited and is not infinite
                if (!tr.is_infinite(vertex) && std::find(visitedVertices.begin(), visitedVertices.end(), vertex) == visitedVertices.end()) 
                {
                    pathPoints.push_back(currentIntersectedPoint);
                    visitedVertices.push_back(vertex);
                    gotVertex = true;
                }
            }
        }            
        else 
        {
            Delaunay::Vertex_handle currentVertex = visitedVertices.back();

            //We search for new faces if we're not walking on an edge
            if (!walkOnEdge) 
            {
                Delaunay::Face_circulator faceCt = tr.incident_faces(currentVertex), done(faceCt);

                bool foundValidFacePath = false;

                while (tr.is_infinite(faceCt)) 
                {
                    faceCt++;
                    //If there aren't non infinite faces or not visited ones
                    if (faceCt == done) 
                    {
                        break;
                    }
                }

                Delaunay::Face_handle searchedFace = faceCt;
                
                Vector2D currentGradientLocal = calculateGradient(tr.triangle(searchedFace));
                double currentGradientModuleLocal = (currentGradientLocal.x() * currentGradientLocal.x()) +
                        (currentGradientLocal.y() * currentGradientLocal.y());
                double previousGradientModuleLocal = currentGradientModuleLocal;

                if (!tr.is_infinite(searchedFace) &&
                        std::find(intersected.begin(), intersected.end(), searchedFace) == intersected.end()) 
                {
                    foundValidFacePath = true;
                }

                faceCt++;

                //Getting the face with greater gradient
                while (faceCt != done) {
                    if (std::find(intersected.begin(), intersected.end(), faceCt) == intersected.end() && !tr.is_infinite(faceCt)) {
                        Point3D auxPoint;
                        Delaunay::Face_handle localFace = faceCt;
                        int gradientIntersectionFromCandidate = getGradientIntersection(*localFace, auxPoint);
                        if (log2(gradientIntersectionFromCandidate) != localFace->index(currentVertex)) {
                            currentGradientLocal = calculateGradient(tr.triangle(localFace));
                            currentGradientModuleLocal = (currentGradientLocal.x() * currentGradientLocal.x()) +
                                    (currentGradientLocal.y() * currentGradientLocal.y());
                            if (currentGradientModuleLocal > previousGradientModuleLocal) {
                                searchedFace = localFace;
                                previousGradientModuleLocal = currentGradientModuleLocal;
                                foundValidFacePath = true;
                            }
                        }
                    }

                    faceCt++;
                }

                if (foundValidFacePath) 
                {
                    intersected.push_back(searchedFace);
                    gotVertex = false;
                }                    
                else 
                {
                    gotVertex = true;
                    walkOnEdge = true;
                }
            }   
            //Walking on edges
            else 
            {
                Delaunay::Edge_circulator edgeCt = tr.incident_edges(currentVertex), doneEdge(edgeCt);

                while (tr.is_infinite(edgeCt)) 
                {
                    edgeCt++;
                    //If there aren't non infinite edges or not visited ones
                    if (edgeCt == doneEdge) 
                    {
                        break;
                    }
                }

                Delaunay::Edge searchedEdge = *edgeCt;

                Delaunay::Vertex_handle vertexA = searchedEdge.first->vertex((searchedEdge.second + 1) % 3);
                Delaunay::Vertex_handle vertexB = searchedEdge.first->vertex((searchedEdge.second + 2) % 3);

                //Next vertex can't be the current vertex
                Delaunay::Vertex_handle nextVertexLocal = currentVertex == vertexA ? vertexB : vertexA;

                //Checking if edge is non infinite and not visited
                while (tr.is_infinite(nextVertexLocal) && ++edgeCt != doneEdge) {
                    searchedEdge = *edgeCt;
                    vertexA = searchedEdge.first->vertex((searchedEdge.second + 1) % 3);
                    vertexB = searchedEdge.first->vertex((searchedEdge.second + 2) % 3);
                    nextVertexLocal = currentVertex == vertexA ? vertexB : vertexA;
                }

                double currentVertexZ = nextVertexLocal->point().z();
                double previousVertexZ = currentVertexZ;

                bool foundValidEdgePath = false;

                if (!tr.is_infinite(nextVertexLocal) &&
                        std::find(visitedVertices.begin(), visitedVertices.end(), nextVertexLocal) == visitedVertices.end() &&
                        currentVertexZ >= currentVertex->point().z()) {
                    foundValidEdgePath = true;
                }

                edgeCt++;

                //Getting edge with greater Z
                while (edgeCt != doneEdge) 
                {
                    if (!tr.is_infinite(edgeCt)) 
                    {
                        Delaunay::Edge localEdge = *edgeCt;
                        vertexA = localEdge.first->vertex((localEdge.second + 1) % 3);
                        vertexB = localEdge.first->vertex((localEdge.second + 2) % 3);

                        nextVertexLocal = currentVertex == vertexA ? vertexB : vertexA;
                        if (!tr.is_infinite(nextVertexLocal) &&
                                std::find(visitedVertices.begin(), visitedVertices.end(), nextVertexLocal) == visitedVertices.end()) {
                            currentVertexZ = nextVertexLocal->point().z();

                            if (currentVertexZ > previousVertexZ && currentVertexZ >= currentVertex->point().z()) {
                                searchedEdge = localEdge;
                                previousVertexZ = currentVertexZ;
                                foundValidEdgePath = true;
                            }
                        }
                    }

                    edgeCt++;
                }

                if (foundValidEdgePath) 
                {
                    vertexA = searchedEdge.first->vertex((searchedEdge.second + 1) % 3);
                    vertexB = searchedEdge.first->vertex((searchedEdge.second + 2) % 3);

                    nextVertexLocal = currentVertex == vertexA ? vertexB : vertexA;
                    
                    //If there are non infinite vertices remaining
                    if (!tr.is_infinite(nextVertexLocal)) 
                    {
                        pathPoints.push_back(nextVertexLocal->point());
                        visitedVertices.push_back(nextVertexLocal);
                        gotVertex = true;
                    } 
                    //Local maximum obtained!
                    else 
                    {
                        cout << "Local Maximum found at: "<< pathPoints.back()[0] << ", "
                                                          << pathPoints.back()[1] << ", "
                                                          << pathPoints.back()[0] << endl;
                        foundMax = true;
                        break;
                    }
                }    
                //Local maximum obtained!
                else 
                {
                    cout << "Local Maximum found at: "<< pathPoints.back()[0] << ", "
                                                      << pathPoints.back()[1] << ", "
                                                      << pathPoints.back()[0] << endl;
                    foundMax = true;
                    break;
                }
            }
        }
    }
}



GLdouble getWorldZ(Point3D position) 
{
    //Parameters for gluProject call
    GLdouble matModelView[16], matProjection[16];
    GLint viewport[4];
    GLdouble worldX, worldY, worldZ;

    //Get matrix and viewport:
    glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
    glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    //Getting the point in the world's coordinates 
    gluProject(position[0], position[1], position[2], matModelView, matProjection,
            viewport, &worldX, &worldY, &worldZ);

    return worldZ; //ranges from 0 to 1
}




Delaunay::Face_handle getClosestTriangle(const Delaunay& tr, LineSegment3D seg, 
                                            std::vector<Point3D> &pathPoints) 
{
    Delaunay::Finite_faces_iterator trianglesIt = tr.finite_faces_begin();
    Delaunay::Finite_faces_iterator closestIt = tr.finite_faces_end();
    
    Point3D currentPoint;
    Point3D previousPoint;
    
    CGAL::Object result;
    bool firstIntersected = false;

    //The first intersected triangle is not necessarily the closest
    for (trianglesIt = tr.finite_faces_begin(); trianglesIt != tr.finite_faces_end(); trianglesIt++) 
    {
        if (!tr.is_infinite(trianglesIt)) 
        {
            //Getting a triangle
            Delaunay::Face trFace = *trianglesIt;
            Point3D v0 = (*(trFace.vertex(0))).point();
            Point3D v1 = (*(trFace.vertex(1))).point();
            Point3D v2 = (*(trFace.vertex(2))).point();
            Triangle3D face(v0, v1, v2);
            
            result = CGAL::intersection(face, seg);

            if (CGAL::assign(currentPoint, result)) 
            {
                //The first intersected triangle doesn't have another triangle to compare
                if (firstIntersected == false) 
                {
                    firstIntersected = true;
                    previousPoint = currentPoint;
                    closestIt = trianglesIt;
                }

                //Of all intersected triangles, the lowest z is the closest
                GLdouble currentZ = getWorldZ(currentPoint);
                GLdouble previousZ = getWorldZ(previousPoint);
                if (currentZ < previousZ) 
                {
                    previousPoint = currentPoint;
                    closestIt = trianglesIt;
                }
            }
        }
    }

    pathPoints.push_back(currentPoint);
    return closestIt;
}

float firstColor = 0.0f;
float lastColor = 1.0f;
float currentColor = 0.1f;
float colorChangeRate = 0.015f;

void drawPath(const std::vector<Point3D> pathPoints) 
{
    glEnable(GL_LINE_SMOOTH);
    
    glColor3f(currentColor, currentColor, lastColor);
    glBegin(GL_LINE_STRIP); 
    for (int i = 0; i < pathPoints.size(); i++) 
    {
        Point3D p = pathPoints[i];
        glVertex3f(p[0], p[1], p[2]);
        
    }
    glEnd();
    
    currentColor += colorChangeRate;
    if((currentColor > (lastColor-0.01f)) || (currentColor < (firstColor+0.01f)))
    {
        colorChangeRate *= -1;
    }
    
    glDisable(GL_LINE_SMOOTH);
}



void drawDelaunayTriangulation(const Delaunay& tr) 
{
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    
    //Drawing the triangles
    glBegin(GL_TRIANGLES);
    glColor3f(0.5, 0.5, 0.5);
    Delaunay::Finite_faces_iterator it;
    for (it = tr.finite_faces_begin(); it != tr.finite_faces_end(); ++it) 
    {
        Delaunay::Face f = *it; 
        for (int i = 0; i < 3; i++) 
        {
            Point3D p = (*(f.vertex(i))).point();
            glVertex3f(p[0], p[1], p[2]);
        }
    }
    glEnd();
    glDisable(GL_POLYGON_OFFSET_FILL);


    //Drawing the triangles edges
    glColor3f(1.0, 1.0, 1.0); 
    for (it = tr.finite_faces_begin(); it != tr.finite_faces_end(); ++it) 
    {
        Delaunay::Face f = *it;
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 3; i++) {
            Point3D p = (*(f.vertex(i))).point();
            glVertex3f(p[0], p[1], p[2]);
        }
        glEnd();
    }

}


/**
 * Global variables
 **/
Delaunay dt; // A Delaunay Triangulation 

Point3D center; // Center of dt´s minimum enclosing parallelepiped
int width = 800, height = 600; // Window size
int activebutton; // Which mouse button was pressed
int xmouse, ymouse; // Coords where mouse was pressed
double xangle = 0.0, yangle = 0.0; // Angles for viewing terrain
double modelview [16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; // Current modelview matrix

LineSegment3D mouseRay; 
std::vector<Point3D> pathPoints;


void init() 
{
    // Create an orthogonal projection system for viewing
    // dt from any angle
    Point3D min, max;
    limits(dt, min, max);
    Vector3D size = 0.5 * (max - min);
    center = min + size;
    double radius = sqrt(size * size);
    size = Vector3D(radius, radius, radius);
    min = center - size;
    max = center + size;
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(min[0], max[0], min[1], max [1], -max[2], -min[2]);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
}



void display() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(center[0], center[1], center[2]);
    glRotatef(xangle, 1.0, 0.0, 0.0);
    glRotatef(yangle, 0.0, 1.0, 0.0);
    glTranslatef(-center[0], -center[1], -center[2]);
    glMultMatrixd(modelview);
    
    //Drawing the whole model 
    drawDelaunayTriangulation(dt);
    
    //Drawing the path to local maximum
    drawPath(pathPoints);
    
    //Drawing the mouse ray
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(mouseRay.source()[0], mouseRay.source()[1], mouseRay.source()[2]);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(mouseRay.target()[0], mouseRay.target()[1], mouseRay.target()[2]);
    glEnd();
    
    glutSwapBuffers();
    glutPostRedisplay();
}




void createMouseRay(int mouseX, int mouseY) 
{
    //Parameters for gluUnProject call
    GLdouble matModelView[16], matProjection[16];
    GLint viewport[4];
    GLdouble m_startX, m_startY, m_startZ;
    GLdouble m_endX, m_endY, m_endZ;

    //Get matrix and viewport:
    glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
    glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    //Mouse position on screen
    double winX = (double) mouseX;
    double winY = viewport[3] - (double) mouseY; //y axis inverted

    //Getting the point on the 'near' plane 
    gluUnProject(winX, winY, 0.0, matModelView, matProjection,
            viewport, &m_startX, &m_startY, &m_startZ);

    //Getting the point on the 'far' plane 
    gluUnProject(winX, winY, 1.0, matModelView, matProjection,
            viewport, &m_endX, &m_endY, &m_endZ);

    //Creating the mouse ray
    Point3D startPoint(m_startX, m_startY, m_startZ);
    Point3D endPoint(m_endX, m_endY, m_endZ);
    mouseRay = LineSegment3D(startPoint, endPoint);
}



void mouseLeftClickHandler(int button, int state, int x, int y) 
{
    xmouse = x;
    ymouse = y;
    if (button == GLUT_LEFT_BUTTON) 
    {
        if (state == GLUT_DOWN) 
        {
            createMouseRay(x, y);
            pathPoints.clear();
            
            Delaunay::Face_handle closestTriangle = getClosestTriangle(dt, mouseRay, pathPoints);
            if (closestTriangle != dt.finite_faces_end().base()) 
            {
                calculatePath(dt, pathPoints, closestTriangle);
            }
        }
    }
    activebutton = button; 
}



void rotateCamera(int x, int y) 
{
    if (activebutton == GLUT_RIGHT_BUTTON) 
    {
        yangle = (x - xmouse) * 360 / width;
        xangle = (y - ymouse) * 360 / height;
    }
}

int main(int argc, char * argv []) 
{
    // Read the terrain data into dt
    std::ifstream terrainfile("terraindata.txt");
    std::istream_iterator<Point3D> beginpt(terrainfile);
    std::istream_iterator<Point3D> endpt;
    dt.insert(beginpt, endpt);

    //
    // Given dt and planes, store intersection curves into curves
    //
    //

    // Set up Graphics User Interface
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Terrain Final Version");
    glutDisplayFunc(display);

    //Mouse callback functions
    glutMouseFunc(mouseLeftClickHandler);
    glutMotionFunc(rotateCamera);

    init();
    glutMainLoop();
    return 0;

}
