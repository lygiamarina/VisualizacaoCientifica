#include <CGAL/Cartesian.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/Random.h>
#include <CGAL/copy_n.h>
#include <iostream>
#include <fstream>
using namespace std;

#define __3D__
typedef CGAL::Cartesian<double>			Rep;
#ifdef __2D__
typedef CGAL::Point_2<Rep>			Point;
typedef CGAL::Random_points_in_disc_2<Point>	Random_points_in_disc;
#else
typedef CGAL::Point_3<Rep>			Point;
typedef CGAL::Random_points_in_sphere_3<Point>	Random_points;
#endif

int main ( int argc, char* argv[]) {

if (argc < 2) {
   cerr << "usage: " << argv[0] << " number of points" << endl;
   exit (0);
}

int npts;
sscanf ( argv[1], "%d", &npts );

std::filebuf fb;
fb.open ("rnd_points.txt",ios::out);
ostream os (&fb);
CGAL::Random rnd(1);
Random_points rnd_pts ( 1.0, rnd );

// generate "npts" random points into a unit disc or sphere, 
// and print them to a file.
os << npts << "\n";
CGAL::copy_n ( rnd_pts, npts, std::ostream_iterator <Point> (os, "\n") );
return 0;
}

