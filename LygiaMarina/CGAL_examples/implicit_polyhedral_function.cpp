#include <CGAL/AABB_intersections.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>

#include <CGAL/Mesh_3/Labeled_mesh_domain_3.h>
#include <CGAL/Mesh_3/polyhedral_to_labeled_function_wrapper.h>
#include <CGAL/make_mesh_3.h>

// IO
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/File_medit.h>


// Domain
struct K: public CGAL::Exact_predicates_inexact_constructions_kernel {};
typedef CGAL::Polyhedron_3<K> Polyhedron;
typedef CGAL::Mesh_3::Polyhedral_to_labeled_function_wrapper<Polyhedron, K> Polyhedral_wrapper;
typedef CGAL::Mesh_3::Labeled_mesh_domain_3<Polyhedral_wrapper, K> Mesh_domain;

// Triangulation
typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;

// Mesh Criteria
typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;
typedef Mesh_criteria::Facet_criteria Facet_criteria;
typedef Mesh_criteria::Cell_criteria Cell_criteria;


int main()
{
  // Create and fill polyhedron
  Polyhedron polyhedron;
  std::ifstream input("elephant.off");
  input >> polyhedron;
  input.close();

  // Implicit function built by AABB_tree projection queries
  Polyhedral_wrapper polyhedral_wrapper(polyhedron, 3, 0.01, -0.01);
  Mesh_domain domain(polyhedral_wrapper, polyhedral_wrapper.bounding_sphere(), 1e-4);

  // Set mesh criteria
  Facet_criteria facet_criteria(30, 0.01, 0.01); // angle, size, approximation
  Cell_criteria cell_criteria(4, 0.01); // radius-edge ratio, size
  Mesh_criteria criteria(facet_criteria, cell_criteria);
  
  // Mesh generation
  C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria);

  // Mesh refinement  
  Facet_criteria new_facet_criteria2(30, 0.01, 0.01);   
  Mesh_criteria new_criteria2(new_facet_criteria2, cell_criteria);  
  //CGAL::refine_mesh_3(c3t3, domain, new_criteria2);
  
  
  const Tr& tr = c3t3.triangulation();
  std::map<Tr::Vertex_handle, int> V;

  std::ofstream out("out5.tet");
   out<<"TET\n"; 

   out<< tr.number_of_vertices() << " " << tr.number_of_finite_cells() << " 0\n";
   int i = 0;
   for(Tr::Finite_vertices_iterator v = tr.finite_vertices_begin(); v != tr.finite_vertices_end();  v++){
      out<<v->point()<<std::endl;
      V[v] = i++;
   }

   for(C3t3::Cell_iterator c = c3t3.cells_begin(); c!= c3t3.cells_end(); c++){
      out<<"4 "<< V[c->vertex(0)] << " " << V[c->vertex(1)] << " " << V[c->vertex(2)] << " " << V[c->vertex(3)] <<"\n";
   }

  out.close();

  // Output
  std::ofstream medit_file("out.mesh");
  CGAL::output_to_medit(medit_file, c3t3);

  return 0;
}