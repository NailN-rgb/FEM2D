#pragma once

#include <FEM2D/precompiled.h>

#include <FEM2D/FEM/TriangleMeshFEM/TriangleFEMStarter.h>

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
template<
    typename ell_equation_type
>
bool AssembleEquation<IndexType, ValueType>::assemble_boundary_conditions(
    const mesh_type_pointer &trimesh,
    const ell_equation_type &ell_equation
)
{
    // TODO: relalize robin's bc assembling
    std::vector<index_type> boundary_markers = trimesh->get(get_bc_markers);

    // 1 - Dirichle't boundary cond, 3 - Robin's boundary cond

    index_type node_idx = 0;
    std::for_each(
        boundary_markers.begin(),
        boundary_markers.end(),
        [&](index_type point_index)
        {
            if(point_index == 1)
            {
                assemble_first_bc(node_idx);
                m_solution(point_index) = ell_equation.get_sol(point_index);
            }
            else if(point_index == 3)
            {
                assemble_third_bc(node_idx);
            }
            else
            {
                // TODO: throw error
            }

            node_idx++;
        }
    )

}



template<
    typename IndexType,
    typename ValueType
>
void AssembleEquation<IndexType, ValueType>::assemble_first_bc(index_type node_idx)
{
    try
    {
        // TODO: make it vectorized
        // TODO: correct rhs

        // loop by unassembled matrix
        for(index_type i = 0; i < m_nodes_count; i++)
        {
            // go trough matrix col
            if(i == node_idx)
            {
                m_global_matrix(node_idx, node_idx) = 1.;
            }
            else
            {
                m_global_matrix(node_idx, i) = 0.;
                m_global_matrix(i, node_idx) = 0.;
            }
        }
    }
    catch(const std::exception &e)
    {
        throw std::runtime_error("Error to assemble first bc at " std::string(node_idx) + " node");
    }
}


template<
    typename IndexType,
    typename ValueType
>
void AssembleEquation<IndexType, ValueType>::assemble_third_bc(index_type node_idx)
{
    try
    {

    }
    catch(const std::exception &e)
    {
        throw std::runtime_error("Error to assemble third bc at " std::string(node_idx) + " node");
    }
}

} //
} //
} //