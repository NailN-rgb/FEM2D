#pragma once

#include <FEM2D/precompiled.h>
namespace FEM2D::mesh::mesh_types
{

namespace bg = boost::geometry;
    
// base class for triangle elements
template<
    typename IndexType, 
    typename ValueType
>
class MeshBase
{
private:
    using index_type = IndexType;
    using value_type = ValueType;

    using point_2d     = typename bg::geo<value_type>::point_2d;
	using segment_type = typename bg::geo<value_type>::segment_2d;
	using polygon_2d   = typename bg::geo<value_type>::polygon_2d;
    using box_2d       = typename bg::geo<value_type>::box_2d;
    using line_2d      = typename bg::geo<value_type>::line_2d;

    using triangle_mesh_t       = FEM2D::mesh::trianglemesh::TriangleMesh;
    using triangle_mesh_pointer = std::shared_ptr<triangle_mesh_t>; 

// 1. List of nodes
    using nodes_list_type           = std::vector<point_2d>;
// 2. Edges List
    using edges_list_type           = std::vector<std::pair<std::size_t, std::size_t>>;
// 3. Triangles List
    using triangles_list_type       = std::vector<std::vector<std::size_t>>;
// 4. Tri-Edge List
    using tri_edge_list_type        = std::vector<std::vector<std::size_t>>;
// 5. Length of edges
    using edges_length_list_type    = std::vector<value_type>;
// 6. Triangle Areas
    using triangle_areas_list_type  = std::vector<value_type>;
// 7. Triangles Center List
    using triangle_center_list_type = std::vector<point_2d>;
// 8. Edges Center List
    using edges_center_list_type    = std::vector<point_2d>;
// 9. Elements mass centers List
    using elems_mass_centers_list   = std::vector<point_2d>;
// 10. Node boundary condition type
    using nodes_bc_list_type        = std::vector<std::size_t>;
// 11. Edges bouundary condition type 
    using edges_bc_list_type        = std::vector<std::size_t>;

public:
    nodes_list_type           m_nodes;
    triangles_list_type       m_elements;
    nodes_bc_list_type        m_node_markers;
    edges_list_type           m_edges;
    edges_bc_list_type        m_edge_markers;
    edges_length_list_type    m_length_edges;
    triangle_areas_list_type  m_areas_triangle;
    triangle_center_list_type m_centers_triangle;
    edges_center_list_type    m_centers_edges;
    elems_mass_centers_list   m_mass_centers_elems;
    tri_edge_list_type        m_tri_edge;

public:
    // default 
    std::string m_solver_type = "FEM";

//ctors
public:
    MeshBase()                   = default;
    MeshBase(const MeshBase& mb) = default;
    ~MeshBase()                  = default;

// set nodes, edges, triangles 
public:
    bool parse(const triangle_mesh_pointer &triangle_mesh);

// get additional mesh data
public:
    bool calculate_mesh_params(); 

// additional data calculators preudonames
public:
    void get_edges_length();
    void get_triangle_areas();
    void get_edges_centers();
    void get_triangles_mass_centers();
    bool create_triangle_edge_connectivity();

// class getters
public:
    nodes_list_type get_points() const { return m_nodes; }
    nodes_bc_list_type get_bc_markers() const { return m_node_markers; }
    edges_list_type get_edges() const { return m_edges; }
    edges_bc_list_type get_bc_edge_markers() const { return m_edge_markers; }
    triangles_list_type get_elements() const { return m_elements; }
    nodes_list_type get_mass_centers() const { return m_mass_centers_elems; }
    std::size_t get_nodes_size() const { return m_nodes.size(); }
    std::size_t get_edges_size() const { return m_edges.size(); }
    std::size_t get_elements_size() const { return m_elements.size(); }


// HELPER FUNCTIONS
// get_points
public:
    point_2d get_point_by_id(std::size_t node_index) const { return m_nodes[node_index];}

public:
    point_2d get_mass_center(std::size_t triangle_id) const { return m_mass_centers_elems[triangle_id];}

public:
    value_type get_triangle_area(std::size_t triangle_id) const { return m_areas_triangle[triangle_id];}

public:
    point_2d get_point_by_edge_id(std::size_t edge_index) const 
    {
        return get_segment_meidan_point(
            get_point_by_id(m_edges[edge_index].first),
            get_point_by_id(m_edges[edge_index].second)
        );
    }

public:
    auto get_triangle_edges_id(
        std::size_t triangle_id
    ) -> std::tuple<std::size_t, std::size_t, std::size_t>
    {
        return std::make_tuple(
            m_tri_edge[triangle_id][0],
            m_tri_edge[triangle_id][1],
            m_tri_edge[triangle_id][2]
        );
    }


public:
// return points of triangle
    nodes_list_type get_points_by_triangle_id(std::size_t idx) const
    {
        std::vector<std::size_t> triangle_points_indexes = get_node_id(idx);

        return nodes_list_type{
            m_nodes[triangle_points_indexes[0]],
            m_nodes[triangle_points_indexes[1]],
            m_nodes[triangle_points_indexes[2]]
        };
    };

public:
// return global id of node with local index local_num_position at triangle_id
    std::vector<std::size_t> get_node_id(std::size_t triangle_id) const
    {
        return m_elements[triangle_id];
    }

public:
    value_type get_line_length(const point_2d& p1, const point_2d& p2) const
    {
        return std::sqrt(std::pow(p2.x() - p1.x(), 2) + std::pow(p2.y() - p1.y(), 2));
    }

// get centers of triangle edges
public:
    nodes_list_type get_triangle_edges_centers(std::size_t triangle_id) const
    {
        nodes_list_type triangle_points = get_points_by_triangle_id(triangle_id);

        return nodes_list_type{
            get_segment_meidan_point(triangle_points[0], triangle_points[1]),
            get_segment_meidan_point(triangle_points[1], triangle_points[2]),
            get_segment_meidan_point(triangle_points[2], triangle_points[0]),
        };
    }

public:
    std::size_t get_dirichlet_bc_count() const
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

public:
    // get l = |e_1|^2 + |e_2|^2 + |e_3|^2
    value_type get_l(std::size_t triangle_id) const
    {
        std::vector<std::size_t> triangle_points = get_node_id(triangle_id);

        return std::pow(this->get_line_length(m_nodes[triangle_points[0]], m_nodes[triangle_points[1]]) , 2) +
               std::pow(this->get_line_length(m_nodes[triangle_points[1]], m_nodes[triangle_points[2]]) , 2) +
               std::pow(this->get_line_length(m_nodes[triangle_points[2]], m_nodes[triangle_points[0]]) , 2);
    }

public:
    point_2d get_segment_meidan_point(
        const point_2d& first,
        const point_2d& second
    ) const
    {
        // delta x / 2 + point with minimal x-coordinate value
        return point_2d(
            std::fabs((first.x() - second.x()) / 2) + (first.x() > second.x() ? second.x() : first.x()),
            std::fabs((first.y() - second.y()) / 2) + (first.y() > second.y() ? second.y() : first.y())
        );
    }

public:
    bool print_mesh();

public:
    bool print_nodes();
    bool print_edges();
    bool print_triangles();

};

} //

#include <FEM2D/mesh/mesh_types/detail/mesh_base.inl>