#pragma once

#include <FEM2D/solver_types/HDG/TraiangleHDG/AssembleHDG.h>

namespace FEM2D::solvers::TriHDG
{

template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::assemble_equation(const mesh_type_pointer &mesh_data)
{
    try
    {
        // write realization
    }
    catch(const std::exception& e)
    {
        std::runtime_error("AssembleHDG::assemble_equation " + std::string(e.what));
    }

    return true;
}


}