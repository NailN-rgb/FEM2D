#pragma once

#include <FEM2D/solver_types/HDG/TraiangleHDG/TriangleHDGStarter.h>

namespace FEM2D::solvers::TriHDG
{

template<typename BaseSolver>
bool TriangleHDGStarter<BaseSolver>::solve(
    int solving_algorithm,
    const mesh_type_pointer &mesh_data
)
{
    try
    {
        // set elletic equation data to mesh points
        this->set_elliptic_equation(mesh_data);

        // start HDG assembler
        this->solve_hdg_system(mesh_data);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("TriangleHDGStarter::solve " + std::string(e.what()));
    }

    return true;
}


template<typename BaseSolver>
bool TriangleHDGStarter<BaseSolver>::solve_hdg_system(const mesh_type_pointer &mesh_data)
{
    try
    {
        // call assemble equaiton function of AssembleHDG Class
        m_assembler.assemble_equation(
            mesh_data,
            this->m_elliptic_equation
        );
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("TriangleHDGStarter::solve_hdg_system " + std::string(e.what()));
    }

    return true;
}

}