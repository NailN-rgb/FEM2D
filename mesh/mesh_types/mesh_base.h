#pragma once

#include <FEM2D/precompiled.h>

namespace mesh
{
    
namespace mesh_types
{

namespace bg = boost::geometry;
    
// base class for triangle elements
template<
    typename IndexType, 
    typename ValueType
>
class MeshBase
{
public:
    using index_type = IndexType;
    using value_type = ValueType;

public:
    using point_2d     = typename bg::geo<value_type>::point_2d;
	using segment_type = typename bg::geo<value_type>::segment_2d;
	using polygon_2d   = typename bg::geo<value_type>::polygon_2d;
    using box_2d       = typename bg::geo<value_type>::box_2d;
    using line_2d      = typename bg::geo<value_type>::line_2d;

// 1. List of nodes
public:
    using nodes_list_type = std::vector<point_2d>;

// 2. Edges List
public:
    using edges_list_type = std::vector<std::pair<index_type, index_type>>;

// 3. Triangles List
public:
    using triangles_list_type = std::vector<std::vector<index_type>>;

// 4. Boundary edges List
public:
    using boundary_edges_list_type = std::vector<std::pair<index_type, bool>>;

// 5. Length of edges
public:
    using edges_length_list_type = std::vector<value_type>;

// 6. Triangle Areas
public:
    using triangle_areas_list_type = std::vector<value_type>;

// 7. Triangles Center List
public:
    using triangle_center_list_type = std::vector<point_2d>;

// 8. Edges Center List
    using edges_center_list_type = std::vector<point_2d>;

// 9. Elements mass centers List
    using elems_mass_centers_list = std::vector<point_2>;
};


} //
} //