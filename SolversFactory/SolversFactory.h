#pragma once

#include <FEM2D/precompiled.h>

namespace FEM2D::Factories
{

template<
    typename IndexType,
    typename ValueType
> struct SolverFactory
{
    using base_solver_type = FEM2D::solvers::BaseSolver::BaseSolverStarter<IndexType, ValueType>;
    using FEM_solver_type  = FEM2D::solvers::TriFem::TriangleFEMStarter<base_solver_type>;
    using HDG_solver_type  = FEM2D::solvers::TriHDG::TriangleHDGStarter<base_solver_type>;
public:
    static std::shared_ptr<base_solver_type> create_solver(const std::string& method_name)
    {
        if(method_name == "FEM")
        {
            // create FEM Starter Caller
            return std::make_unique<FEM_solver_type>();
        }
        else if(method_name == "HDG")
        {
            // create HDG Starter Caller
            return std::make_unique<HDG_solver_type>();
        }
        else if(method_name == "FVM")
        {
            // create FVM Starter Caller
            //return std::make_unique<FVMSolver>
            throw std::runtime_error("FVM Solver not realized now");
        }
        else
        {
            throw std::runtime_error("Unknown type of solver");
        }
    }    
};

} //