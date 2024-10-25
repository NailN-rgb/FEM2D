#pragma once

#include <FEM2D/precompiled.h>

#include <FEM2D/solver_types/FEM/TriangleMeshFEM/TriangleFEMStarter.h>

namespace FEM2D
{

namespace solvers
{

namespace TriFem
{

template<typename BaseSolver> bool TriangleFEMStarter<BaseSolver>::solve(
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

    return true;
    
}


template<typename BaseSolver> void TriangleFEMStarter<BaseSolver>::get_fem_system(
    const mesh_type_pointer &mesh_data
)
{
    try
    {
        m_assembler.assemble_equation(
            mesh_data,
            this->m_elliptic_equation
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