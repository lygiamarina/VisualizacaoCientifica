#include <GL/glut.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <list>

typedef CGAL::Cartesian<double> K;  ///< Kernel
typedef CGAL::Point_2<K> Point;  ///< Point in 2D
typedef CGAL::Segment_2<K> Segment;  ///< Line Segment in 2D
typedef CGAL::Ray_2<K> Ray;  ///< Ray (halfline) in 2D
typedef CGAL::Vector_2<K> Vector;  ///< Vector in 2D
typedef CGAL::Line_2<K> Line;  ///< Line in 2D
typedef CGAL::Delaunay_triangulation_2<K> Delaunay; ///< A Delaunay Triangulation
typedef std::list<Point> PointList;

PointList points;
Delaunay triang;

int width = 400;
int height = 400;

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);

    /// Draw points
    glColor3f (1.0, 1.0, 1.0);
    glPointSize (3);
    glBegin(GL_POINTS);
    PointList::iterator ip = points.begin ();
    while (ip != points.end ()) {
        Point p = *ip++;
        glVertex2f (p [0], p [1]);
    }
    glEnd();

    /// Draw Delaunay Triangulation
    glColor3f (1.0, 0.0, 1.0);
    glBegin (GL_LINES);
    Delaunay::Edge_iterator ie = triang.edges_begin ();
    while (ie != triang.edges_end ()) {
        Delaunay::Edge e = *ie++;
        if (triang.is_infinite (e)) continue;
        Delaunay::Segment s = triang.segment (e);
        for (int i = 0; i < 2; ++i) {
            Point p = s[i];
            glVertex2f (p [0], p [1]);
        }
    }
    glEnd ();

    /// Draw Voronoi Diagram
    glColor3f (1.0, 1.0, 0.0);
    glBegin (GL_LINES);
    ie = triang.edges_begin ();
    while (ie != triang.edges_end ()) {
        Delaunay::Edge e = *ie++;
        CGAL::Object obj = triang.dual (e);

        Segment s;
        Ray r;
        if (CGAL::assign (s, obj)) {
            glVertex2f (s[0][0], s[0][1]);
            glVertex2f (s[1][0], s[1][1]);
        } else if (CGAL::assign (r, obj)) {
	        Point p = r.source();
	        Vector v = r.direction().vector();
            glVertex2f (p[0], p[1]);
            p = p + 1000 * v;
            glVertex2f (p[0], p[1]);
        }
    }
    glEnd ();


    /// Finish
    glutSwapBuffers ();
}

void reshape (int wid, int hgt)
{
    glViewport(0,0,width=wid,height=hgt);
    glMatrixMode (GL_PROJECTION_MATRIX);
    glLoadIdentity ();
    gluOrtho2D (0, width, height, 0);
}

void mouse (int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
    Point p ((double) x, (double) y);
    points.push_back (p);

    triang.push_back (p);
    glutPostRedisplay ();
}

void init ()
{
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("test");
    glutDisplayFunc(display);
    glutMouseFunc (mouse);
    glutReshapeFunc (reshape);
    glutMainLoop();
    return 0;
}
