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
        // TODO: correct rhs

        // loop by rows
        // TODO: Use template 
        for(typename sparse_matrix_type::iterator1 it1 = m_global_matrix.begin1();
               it1 != m_global_matrix.end1(); ++it1)
        {
            if(nodes.count(it1.index1()) > 0)
            {
                // loop by columns
                for(typename sparse_matrix_type::iterator2 it2 = it1.begin();
                        it2 != it1.end(); ++it2)
                {
                    if (it2.index2() != it1.index1()) // Проверка, что это не элемент на главной диагонали
                    {
                        *it2 = 0.0; // Обнуление элемента
                    }
                    else // Если это элемент на главной диагонали
                    {
                        *it2 = 1.0; // Установка элемента равным 1
                    }
                }
            }
        }


        for(typename sparse_vector_type::iterator it = m_global_vector.begin(); it != m_global_vector.end(); ++it)
        {
            if(nodes.count(it.index()) > 0)
            {
                *it = ell_equation.get_sol(it.index());
            }
        }

        std::cout << "First BC assembled" << std::endl;
    }
    catch(const std::exception &e)
    {
        throw std::runtime_error("Error to assemble first bc");
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