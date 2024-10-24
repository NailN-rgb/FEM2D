#pragma once

#include <FEM2D/precompiled.h>

namespace FEM2D
{

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

public:
    using triangle_mesh_t   = FEM2D::mesh::trianglemesh::TriangleMesh;
    using triangle_mesh_pointer = std::unique_ptr<triangle_mesh_t>; 

// 1. List of nodes
public:
    using nodes_list_type = std::vector<point_2d>;

// 2. Edges List
public:
    using edges_list_type = std::vector<std::vector<index_type>>;

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

public:
// 9. Elements mass centers List
    using elems_mass_centers_list = std::vector<point_2d>;

// 10. Node boundary condition type
    using nodes_bc_list_type = std::vector<index_type>;

public:
    nodes_list_type m_nodes;

public:
    nodes_bc_list_type m_node_markers;

public:
    edges_list_type m_edges;

public:
    triangles_list_type m_elements;

public:
    boundary_edges_list_type m_b_edges;

public:
    edges_length_list_type m_length_edges;

public:
    triangle_areas_list_type m_areas_triangle;

public:
    triangle_center_list_type m_centers_triangle;

public:
    edges_center_list_type m_centers_edges;

public:
    elems_mass_centers_list m_mass_centers_elems;


//ctors
public:
    MeshBase() = default;
    MeshBase(const MeshBase& mb) = default;
    ~MeshBase() = default;

// set nodes, edges, triangles 
public:
    bool parse(const triangle_mesh_pointer &trimesh);

// get additional mesh data
public:
    bool calculate_mesh_params(); 

// additional data calculators preudonames
public:
    void get_boundary_edges();
    void get_edges_length();
    void get_triangle_areas();
    void get_triangle_centers();
    void get_edges_centers();
    void get_triangles_mass_centers();


// class getters
public:
    nodes_list_type get_points() { return m_nodes; }

public:
    nodes_bc_list_type get_bc_markers() { return m_node_markers; }

public:
    edges_list_type get_edges() { return m_edges; }

public:
    triangles_list_type get_elements() { return m_elements; }

public:
    nodes_list_type get_mass_centers() { return m_mass_centers_elems; }

public:
    std::size_t get_nodes_size() { return m_nodes.size(); }

public:
    std::size_t get_edges_size() { return m_edges.size(); }

public:
    std::size_t get_elements_size() { return m_elements.size(); }


// HELPER FUNCTIONS
// return points of triangle
public:
    nodes_list_type get_points_by_triangle_id(index_type idx);

// return global id of node with local index local_num_position at triangle_id
public:
    std::vector<index_type> get_node_id(index_type triangle_id)
    {
        return m_elements[triangle_id];
    }

public:
    value_type get_line_length(const point_2d& p1, const point_2d& p2)
    {
        return std::sqrt(std::pow(p2.x() - p1.x(), 2) + std::pow(p2.y() - p2.x(), 2));
    }

public:
    std::size_t get_dirichlet_bc_count()
    {
        std::size_t diriclet_nodes = 0;

        std::for_each(
            m_node_markers.begin(),
            m_node_markers.end(),
            [&diriclet_nodes](index_type marker)
            {
                if(marker == 1)
                {
                    diriclet_nodes++;
                }
            }
        );

        return diriclet_nodes;
    }

};


} //
} //
} //

#include <FEM2D/mesh/mesh_types/detail/mesh_base.inl>