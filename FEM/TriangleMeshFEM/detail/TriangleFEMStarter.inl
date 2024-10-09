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
> void TriangleFEMStarter<IndexType, ValueType>::solve(
    int solving_algorithm,
    const mesh_type_pointer &mesh_data
)
{
    try
    {
        // main solve process
        this->set_elliptic_equation(mesh_data);

        this->get_fem_system(mesh_data);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("TriangleFemStarter::solve " + std::string(e.what()));
    }
    
}


template<
    typename IndexType,
    typename ValueType
> void TriangleFEMStarter<IndexType, ValueType>::set_elliptic_equation(
    const mesh_type_pointer &mesh_data
)
{
    try
    {
        // initialize equation params
        // May be we need some entry equation data parce?

        // set points and calculate entered functions at points
        m_elliptic_equation.calculate_at_points(mesh_data->get_points());
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("set_elliptic_equation:: " + std::string(e.what()));
    }
}


template<
    typename IndexType,
    typename ValueType
> void TriangleFEMStarter<IndexType, ValueType>::get_fem_system(
    const mesh_type_pointer &mesh_data
)
{
    try
    {
        m_assembler.assemble_equation(
            mesh_data,
            m_elliptic_equation
        );
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("get_fem_system:: " + std::string(e.what()));
    }
}

} //
} //
} //