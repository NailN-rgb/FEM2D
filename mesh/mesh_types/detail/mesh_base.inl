#pragma once

#include <FEM2D/precompiled.h>
#include <FEM2D/mesh/mesh_types/mesh_base.h>

namespace FEM2D::mesh::mesh_types
{

template<
    typename IndexType,
    typename ValueType
> bool MeshBase<IndexType, ValueType>::parse(const triangle_mesh_pointer &triangle_mesh)
{
    try
    {
        // get nodes
        m_nodes = triangle_mesh->get_points_list(triangle_mesh->in_);

        //get boundary nodes attributes
        m_node_markers = triangle_mesh->get_boundary_attributes(triangle_mesh->in_);

        // get edges
        m_edges = triangle_mesh->get_segments_list(triangle_mesh->in_);

        // get marekers at edges
        m_edge_markers = triangle_mesh->get_segment_marker_list(triangle_mesh->in_);

        // get triangles
        m_elements = triangle_mesh->get_triangle_list(triangle_mesh->in_);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("MeshBase::parse:: " + std::string(e.what()));
    }

    return true;
}

template<
    typename IndexType,
    typename ValueType
> bool MeshBase<IndexType, ValueType>::calculate_mesh_params()
{
    get_edges_length();
    get_triangle_areas();
    get_edges_centers();
    get_triangles_mass_centers();

    create_triangle_edge_connectivity();

    return true;
}


template<
    typename IndexType,
    typename ValueType
> void MeshBase<IndexType, ValueType>::get_edges_length()
{
    try
    {
        std::for_each(
            m_edges.begin(),
            m_edges.end(),
            [this](std::pair<std::size_t, std::size_t> edge)
            {
                m_length_edges.push_back(
                    get_line_length(m_nodes[edge.first], m_nodes[edge.second])
                );
            }
        );
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("MeshBase::get_edges_length:: " + std::string(e.what()));
    }
}


template<
    typename IndexType,
    typename ValueType
> void MeshBase<IndexType, ValueType>::get_triangle_areas()
{
    try
    {
        // by formula S = 1/2 * |(x_2 - x_1)(y_3 - y_1) - (x_3 - x_1)(y_2 - y_1)|
        std::for_each(
            m_elements.begin(),
            m_elements.end(),
            [this](std::vector<std::size_t> triangle)
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
    catch(const std::exception& e)
    {
        throw std::runtime_error("MeshBase::get_triangle_areas:: " + std::string(e.what()));
    }
}


template<
    typename IndexType,
    typename ValueType
> void MeshBase<IndexType, ValueType>::get_edges_centers()
{
    try
    {
        std::for_each(
            m_edges.begin(),
            m_edges.end(),
            [this](std::pair<std::size_t, std::size_t> edge)
            {
                m_centers_edges.push_back(
                    point_2d(
                        (m_nodes[edge.first].x() + m_nodes[edge.second].x()) / 2,
                        (m_nodes[edge.first].y() + m_nodes[edge.second].y()) / 2
                    )
                );
            }
        );
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("MeshBase::get_edges_centers:: " + std::string(e.what()));
    }
}


template<
    typename IndexType,
    typename ValueType
> void MeshBase<IndexType, ValueType>::get_triangles_mass_centers()
{
    try
    {
        std::for_each(
            m_elements.begin(),
            m_elements.end(),
            [this](std::vector<std::size_t> triangle)
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
    catch(const std::exception& e)
    {
        throw std::runtime_error("MeshBase::get_triangles_mass_centers:: " + std::string(e.what()));
    }
}


template<
    typename IndexType,
    typename ValueType
> bool MeshBase<IndexType, ValueType>::create_triangle_edge_connectivity()
{
    try
    {
        // Dictionary for find edge index
        std::map<
            std::pair<std::size_t, std::size_t>,
            std::size_t
        > edge_indexes;

        for (std::size_t i = 0; i < m_edges.size(); ++i) 
        {
            std::size_t u = m_edges[i].first;
            std::size_t v = m_edges[i].second;

            if (u > v) {std::swap(u, v);}
            edge_indexes[{u, v}] = i; // Save edge index
        }

        for(const auto& triangle : m_elements)
        {
            std::vector<std::size_t> edges_indexes;

            for(std::size_t i = 0; i < 3; i++)
            {
                std::size_t u = triangle[i];
                std::size_t v = triangle[(i + 1) % 3]; 

                if (u > v) {std::swap(u, v);} 

                if (edge_indexes.find({u, v}) != edge_indexes.end()) 
                {
                    edges_indexes.push_back(edge_indexes[{u, v}]);
                }
            }

            m_tri_edge.push_back(edges_indexes);
        }
        return true;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("MeshBase::create_triangle_edge_connectivity:: " + std::string(e.what()));
    }


    return true;
}


template<
    typename IndexType,
    typename ValueType
> bool MeshBase<IndexType, ValueType>::print_mesh()
{
    try
    {
        this->print_nodes();
        this->print_edges();
        this->print_triangles();

        return true;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("MeshBase::print_mesh:: " + std::string(e.what()));
    }

    return true;
}


template<
    typename IndexType,
    typename ValueType
> bool MeshBase<IndexType, ValueType>::print_nodes()
{
    try
    {
        std::ofstream node_of;
        node_of.open("mesh_nodes.txt");

        if(node_of.is_open())
        {
            // write total number of points
            node_of << m_nodes.size() << std::endl;

            // write other coordinates
            for(std::size_t i = 0; i < m_nodes.size(); i++)
            {
                node_of << m_nodes[i].x() << "\t" << m_nodes[i].y() << std::endl;
            }
        }

        node_of.close();

        return true;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("MeshBase::print_nodes:: " + std::string(e.what()));
    }

    return true;
}


template<
    typename IndexType,
    typename ValueType
> bool MeshBase<IndexType, ValueType>::print_edges()
{
    try
    {
        std::ofstream edge_of;
        edge_of.open("mesh_edges.txt");

        if(edge_of.is_open())
        {
            // write total number of edges
            edge_of << m_edges.size() << std::endl;

            // write edge list
            for(std::size_t i = 0; i < m_edges.size(); i++)
            {
                edge_of << m_edges[i].first << "\t" << m_edges[i].second << std::endl;
            }
        }

        edge_of.close();

        return true;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("MeshBase::print_edges:: " + std::string(e.what()));
    }

    return true;
}


template<
    typename IndexType,
    typename ValueType
> bool MeshBase<IndexType, ValueType>::print_triangles()
{
    try
    {
        std::ofstream tri_of;
        tri_of.open("mesh_triangles.txt");

        if(tri_of.is_open())
        {
            // write total number of triangles
            tri_of << m_elements.size() << std::endl;

            // write trilist
            for(std::size_t i = 0; i < m_elements.size(); i++)
            {
                tri_of << m_elements[i][0] << "\t" << m_elements[i][1] << "\t" << m_elements[i][2] << std::endl;
            }
        }

        tri_of.close();

        return true;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("MeshBase::print_triangles:: " + std::string(e.what()));
    }

    return true;
}


} //
