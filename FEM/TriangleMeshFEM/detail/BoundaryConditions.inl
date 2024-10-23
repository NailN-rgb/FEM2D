#pragma once

#include <FEM2D/precompiled.h>

namespace FEM2D
{

namespace solvers
{

namespace TriFem
{

template<
    typename IndexType,
    typename ValueType
> 
bool AssembleEquation<IndexType, ValueType>::assemble_boundary_conditions(
    const mesh_type_pointer &trimesh,
    const ell_equation_type &ell_equation
)
{
    // TODO: relalize robin's bc assembling
    std::vector<index_type> boundary_markers = trimesh->get_bc_markers();

    // 1 - Dirichle't boundary cond, 3 - Robin's boundary cond

    std::unordered_set<index_type> first_bc_points;

    index_type node_idx = 0;
    std::for_each(
        boundary_markers.begin(),
        boundary_markers.end(),
        [&](index_type point_marker)
        {
            if(point_marker == 1)
            {
                first_bc_points.insert(node_idx);
            }
            else if(point_marker == 3)
            {
                assemble_third_bc(node_idx);
            }
            else
            {
                // TODO: throw error
            }

            node_idx++;
        }
    );

    assemble_first_bc(first_bc_points, ell_equation);

    return true;
}



template<
    typename IndexType,
    typename ValueType
>
template<typename NodesList>
void AssembleEquation<IndexType, ValueType>::assemble_first_bc(
    NodesList nodes,
    const ell_equation_type &ell_equation
)
{
    try
    {
        // TODO: make it vectorized

        // calculate true solution at Dirichlet Points
        for(index_type i = 0; i < m_nodes_count; i++)
        {
            // if node have a Dirichlet bc marker 
            if(nodes.count(i) > 0)
            {
                value_type expl_solution = ell_equation.get_sol(i); 
                m_solution(i) = expl_solution;
            }
        }

        // change rhs vector elements
        for(std::size_t i = 0; i < m_nodes_count; i++)
        {
            m_global_vector(i) -= arma::dot(m_global_matrix.row(i), m_solution);
        }

        // assemble to global matrix
        for(index_type i = 0; i < m_nodes_count; i++)
        {
            // if node have a Dirichlet bc marker 
            if(nodes.count(i) > 0)
            {
                arma::rowvec row(m_nodes_count);

                vector_type col(m_nodes_count);
                // TODO: rewrite this
                col(i) = m_solution(i) > 0.00001 ? m_global_vector(i) / m_solution(i) : 1e9;

                m_global_matrix.row(i) = row;
                m_global_matrix.col(i) = col;
            }
        }

        std::cout << "First BC assembled" << std::endl;
    }
    catch(const std::exception &e)
    {
        throw std::runtime_error("Error to assemble first bc" + std::string(e.what()));
    }
}


template<
    typename IndexType,
    typename ValueType
>
template<typename NodesList>
void AssembleEquation<IndexType, ValueType>::assemble_third_bc(NodesList nodes)
{
    try
    {

    }
    catch(const std::exception &e)
    {
        throw std::runtime_error("Error to assemble third bc");
    }
}

} //
} //
} //