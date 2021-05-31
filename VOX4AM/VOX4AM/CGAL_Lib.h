#pragma once

#pragma comment(lib, "libmpfr-4.lib")
#pragma comment(lib, "libgmp-10.lib")

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/IO/OFF_reader.h>
#include <CGAL/IO/STL_reader.h>
#include <CGAL/IO/OBJ_reader.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Side_of_triangle_mesh.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_3 Point;
typedef CGAL::Polyhedron_3<K> Polyhedron;
typedef CGAL::AABB_face_graph_triangle_primitive<Polyhedron, CGAL::Default, CGAL::Tag_false> Primitive;
typedef CGAL::AABB_traits<K, Primitive> Traits;
typedef CGAL::AABB_tree<Traits> Tree;
typedef CGAL::Side_of_triangle_mesh<Polyhedron, K> Inside;