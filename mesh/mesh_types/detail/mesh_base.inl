#pragma once

#include <FEM2D/precompiled.h>
#include <FEM2D/mesh/mesh_types/mesh_base.h>

namespace FEM2D
{

namespace mesh
{
    
namespace mesh_types
{

template<
    typename IndexType,
    typename ValueType
> bool MeshBase<IndexType, ValueType>::parse(const triangle_mesh_pointer &trimesh)
{
    try
    {
        // get nodes
        m_nodes = trimesh->get_points_list(trimesh->in_);

        // get edges
        m_edges = trimesh->get_segments_list(trimesh->in_);

        // get triangles
        m_elements = trimesh->get_triangle_list(trimesh->in_);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("MeshBase::parse:: " + std::string(e.what()));
    }
}

template<
    typename IndexType,
    typename ValueType
> bool MeshBase<IndexType, ValueType>::calculate_mesh_params()
{
    this->get_boundary_edges();
    this->get_edges_length();
    this->get_triangle_areas();
    this->get_triangle_centers();
    this->get_edges_centers();
    this->get_triangles_mass_centers();

    return true;
}


template<
    typename IndexType,
    typename ValueType
> void MeshBase<IndexType, ValueType>::get_boundary_edges()
{
    // TODO: need to formulate edges attributes list first

}


template<
    typename IndexType,
    typename ValueType
> void MeshBase<IndexType, ValueType>::get_edges_length()
{
    std::for_each(
        m_edges.begin(),
        m_edges.end(),
        [this](std::vector<index_type> edge)
        {
            m_length_edges.push_back(
                get_line_length(m_nodes[edge[0]], m_nodes[edge[1]])
            );
        }
    );
}


template<
    typename IndexType,
    typename ValueType
> void MeshBase<IndexType, ValueType>::get_triangle_areas()
{
    // by formula S = 1/2 * |(x_2 - x_1)(y_3 - y_1) - (x_3 - x_1)(y_2 - y_1)|
    std::for_each(
        m_elements.begin(),
        m_elements.end(),
        [this](std::vector<index_type> triangle)
        {
            m_areas_triangle.push_back(
                0.5 * std::fabs(
                    (m_nodes[triangle[1]].x() - m_nodes[triangle[0]].x()) * 
                    (m_nodes[triangle[2]].y() - m_nodes[triangle[0]].y()) - 
                    (m_nodes[triangle[2]].x() - m_nodes[triangle[0]].x()) *
                    (m_nodes[triangle[1]].y() - m_nodes[triangle[0]].y())
                )
            );
        }
    );
}


template<
    typename IndexType,
    typename ValueType
> void MeshBase<IndexType, ValueType>::get_triangle_centers()
{
    // TODO: we need that?
}

template<
    typename IndexType,
    typename ValueType
> void MeshBase<IndexType, ValueType>::get_edges_centers()
{
    std::for_each(
        m_edges.begin(),
        m_edges.end(),
        [this](std::vector<index_type> edge)
        {
            m_centers_edges.push_back(
                point_2d(
                    (m_nodes[edge[0]].x() + m_nodes[edge[1]].x()) / 2,
                    (m_nodes[edge[0]].y() + m_nodes[edge[1]].y()) / 2
                )
            );
        }
    );
}


template<
    typename IndexType,
    typename ValueType
> void MeshBase<IndexType, ValueType>::get_triangles_mass_centers()
{
    std::for_each(
        m_elements.begin(),
        m_elements.end(),
        [this](std::vector<index_type> triangle)
        {
            m_mass_centers_elems.push_back(
                point_2d(
                    (m_nodes[triangle[0]].x() + m_nodes[triangle[1]].x() + m_nodes[triangle[2]].x()) / 3,
                    (m_nodes[triangle[0]].y() + m_nodes[triangle[1]].y() + m_nodes[triangle[2]].y()) / 3
                )
            );
        }
    );
}

} //
} //
} //
