#pragma once

#include <FEM2D/mesh/mesh_builder.h>
#include <FEM2D/mesh/mesh_types/mesh_base.h>
#include <FEM2D/solver_types/base_solver.h>
#include <FEM2D/solver_types/FEM/TriangleMeshFEM/TriangleFEMStarter.h>
#include <FEM2D/SolversFactory/SolversFactory.h>


namespace FEM2D
{
namespace solver
{

template<
    typename IndexType,
    typename ValueType
> void solver_runner<IndexType, ValueType>::apply()
{
    try
    {
        using mesh_builder_type = FEM2D::mesh::Mesh_builder<index_type, value_type>;
        using base_solver_type = FEM2D::solvers::BaseSolver::BaseSolverStarter<index_type, value_type>;

        // create mesh
        mesh_builder_type mesh;
        auto mesh_ptr = mesh.build_mesh(true);

        // get pointer-to-solver 
        auto solver = FEM2D::Factories::SolverFactory<index_type, value_type>::create_solver("FEM");

        if(solver->solve(1, mesh_ptr))
        {
            std::cout << "Solver Finished" << std::endl;
        }
        
        // select solver type

        std::string solver_type = "FEM";

        // TODO: Code to start solver 

    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("solver::solver_runner:: " + std::string(e.what()));
    }
}

} //

} //