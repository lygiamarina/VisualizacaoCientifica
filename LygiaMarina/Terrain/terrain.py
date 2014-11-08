#!/usr/bin/python

# Author: Paulo Roma Cavalcanti in 07/03/2007.

# Draws a terrain triangulation.
#

import sys
from sys import *
from math import sqrt
#path.append("C:\cgal-python-0.9.1")                               # Windows

try:
  from CGAL.Kernel import *
  from CGAL import *
  from CGAL.Triangulations_2 import *
except:
  print '''ERROR: cgal-python not installed properly.'''
  print '''Go get it: http:/atrpms.net/'''
  exit ( 1 )

# must be imported AFTER the CGAL stuff!!
try:
  from OpenGL.GLUT import *
  from OpenGL.GL import *
  from OpenGL.GLU import *
except:
  print '''ERROR: PyOpenGL not installed properly.'''
  print '''Go get it: http:/atrpms.net/'''
  exit ( 2 )

#
# Global variables
#
dt = Delaunay_triangulation_2() # A Delaunay Triangulation
ptz = {}			# Hash for z coordinates
planes = [] 			# Planes for which intersection curves are computed
curves = [] 			# Intersection curves - not used yet

center = Point_3 (0.0,0.0,0.0)	# Center of dt minimum enclosing parallelepiped
width  = 400 
height = 300  			# Window size
activebutton = GLUT_LEFT_BUTTON # Which mouse button was pressed
xmouse = 0 
ymouse = 0 			# Coords where mouse was pressed
xangle = 0.0
yangle = 0.0 			# Angles for viewing terrain
# Current modelview matrix
modelview = [1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0] 


#
# Computes the minimum enclosing parallelepiped of a
# Delaunay triangulation
#
# @param triang  Delaunay triangulation (input)
# @param min minimum coordinates of the parallelepiped (output)
# @param max maximum coordinates of the parallelepiped (output)
#
def limits (triang):
    mincoord = []
    maxcoord = []
    ip = iter ( triang.points )
    p2 = ip.next()
    p  = Point_3 ( p2[0], p2[1], ptz[p2[0],p2[1]] )
    for i in range (3):  
       mincoord.append (p [i]) 
       maxcoord.append (p [i]) 
    for ip in triang.points:
       p2 = Point_2 ( ip )	
       p = Point_3 ( p2[0], p2[1], ptz[p2[0],p2[1]] )
       for i in range (3):
           if (p [i] < mincoord [i]):
                  mincoord [i] = p[i]
           elif (p [i] > maxcoord [i]): 
                  maxcoord [i] = p[i]
    min = Point_3 (mincoord [0], mincoord [1], mincoord [2])
    max = Point_3 (maxcoord [0], maxcoord [1], maxcoord [2])
    return (min, max)


#
# Draws a Delaunay triangulation
#
# @param tr  Delaunay triangulation (input)
#
def drawTriangulation (tr):
    global dl_triangles
    try:
        glCallList (dl_triangles)
    except NameError:
        dl_triangles = glGenLists(1)
        glNewList(dl_triangles, GL_COMPILE_AND_EXECUTE)
        # Draw triangles
        glEnable(GL_POLYGON_OFFSET_FILL)
        glPolygonOffset(1.0, 1.0)
        glColor3f (0.5, 0.5, 0.5)  # gray
        glBegin(GL_TRIANGLES)
        for it in tr.faces: 
           for i in range (3):
               p2 = it.vertex(i).point()
               p  = Point_3 (p2[0], p2[1], ptz[p2[0],p2[1]])
               glVertex3f (p[0], p[1], p[2])
        glEnd ()
        glDisable(GL_POLYGON_OFFSET_FILL)
        # Draw Triangle edges
        glColor3f (1.0, 1.0, 1.0)  # white
        for it in tr.faces:
            glBegin(GL_LINE_LOOP)
            for i in range (3):
                p2 = it.vertex(i).point()
                p  = Point_3 (p2[0], p2[1], ptz[p2[0],p2[1]])
                glVertex3f (p[0], p[1], p[2])
            glEnd ()
        glEndList()



#
# Draws a set of polylines
#
# @param pls set of polylines
#
def drawCurves (pls):
    glColor3f (0.0, 0.0, 1.0)  # blue
    for ipls in pls:
        glBegin (GL_LINE_STRIP)
        for ipoly in pls(ipls):
            p = Point_3 (ipoly)
            glVertex3f (p[0],p[1],p[2])
        glEnd ()


def init ():
    global center
    # Create an orthogonal projection system for viewing
    # dt from any angle
    min, max = limits (dt)
    size = 0.5 * (max - min)
    center = min + size
    radius = sqrt (size * size)
    size = Vector_3 (radius, radius, radius)
    min = center - size
    max = center + size
    glMatrixMode (GL_PROJECTION)
    glLoadIdentity ()
    glOrtho(min[0], max[0], min[1], max [1], -max[2], -min[2])

    # Enable depth testing
    glEnable (GL_DEPTH_TEST)


def display ():
    global modelview
    global xangle, yangle
    global center
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glMatrixMode (GL_MODELVIEW)
    glLoadIdentity ()
    glTranslatef (center[0], center[1], center[2])
    glRotatef (xangle, 1.0, 0.0, 0.0)
    glRotatef (yangle, 0.0, 1.0, 0.0)
    glTranslatef (-center[0], -center[1], -center[2])
    glMultMatrixd (modelview)
    drawTriangulation (dt)
    drawCurves (curves)
    glutSwapBuffers()


def reshape (wid, hgt):
    global width, height
    width  = wid
    height = hgt
    glViewport(0,0,wid,hgt)


def mouse (button, state, x, y):
    global xangle, yangle
    global xmouse, ymouse
    global modelview
    global activebutton
    xmouse = x
    ymouse = y
    if (button == GLUT_LEFT_BUTTON):
        if (state == GLUT_UP):
            modelview = glGetDoublev (GL_MODELVIEW_MATRIX)
            xangle = 0.0
            yangle = 0.0
        
    activebutton = button
    glutPostRedisplay ()


def motion (x, y):
    global xangle, yangle
    global xmouse, ymouse
    global activebutton
    if (activebutton == GLUT_LEFT_BUTTON):
        yangle = float(x - xmouse) * 360.0 / width
        xangle = float(y - ymouse) * 180.0 / height
    glutPostRedisplay ()

def main(argv=None):
    if argv is None:
       argv = sys.argv  

    # Read the terrain data into dt
    try:
       terrainfile = open ("terraindata.txt")
    except:
       print "No terrain data found!"
       return 3
    for line in terrainfile.read().split('\n'): 
         line = line.split()
         pt = Point_2 (float(line[0]), float(line[1]))
         ptz [pt[0],pt[1]] = float(line[2])
         dt.insert(pt)
    terrainfile.close() 

    # Read set of planes into planes
    try:
       planefile = open ("planedata.txt")
    except:
       print "No plane data found!"
       return 4
    for line in planefile.read().split('\n'):
         line = line.split()
         pl = [float(line[0]), float(line[1]), float(line[2]), float(line[3])]
         planes.append (pl)
    planefile.close()


    #
    # Given dt and planes, store intersection curves into curves
    #
    #

    # Set up Graphics User Interface
    glutInit(argv)
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
    glutInitWindowSize (width, height)
    glutInitWindowPosition (100, 100)
    glutCreateWindow ("Terrain")
    glutDisplayFunc(display)
    glutMouseFunc (mouse)
    glutMotionFunc (motion)
    glutReshapeFunc (reshape)
    init ()
    glutMainLoop()
    return 0

# When main() calls sys.exit(), your interactive Python interpreter
# will exit! The remedy is to let main()'s return value specify
# the exit status.
#
if __name__=='__main__':
    sys.exit(main())

