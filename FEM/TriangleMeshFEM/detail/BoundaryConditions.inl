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
> bool AssembleEquation<IndexType, ValueType>::assemble_boundary_conditions(
    const mesh_type_pointer &trimesh
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