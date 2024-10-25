#pragma once

#include <FEM2D/precompiled.h>
#include <FEM2D/solver_types/FEM/TriangleMeshFEM/AssembleEquation.h>

namespace FEM2D::solvers::TriFem
{

template<
    typename BaseSolver
>
class TriangleFEMStarter : public BaseSolver 
{
// TriFem assembler type
private:
    using index_type = typename BaseSolver::index_type;
    using value_type = typename BaseSolver::value_type;
    using mesh_type_pointer = typename BaseSolver::mesh_type_pointer;

private:
    using equation_assembler_type = FEM2D::solvers::TriFem::AssembleEquation<index_type, value_type>;

// init equation
private:
    equation_assembler_type m_assembler; 

// default ctor's
public:
    TriangleFEMStarter() = default;
    TriangleFEMStarter(const TriangleFEMStarter& trifem) = default;
    ~TriangleFEMStarter() = default;

// main solving function
public:
    bool solve(
        int solving_algorithm,
        const mesh_type_pointer &mesh_data
    ) override;
    
// call & complete matrix/RHS builder 
private:
    void get_fem_system(
        const mesh_type_pointer &mesh_data
    ); 
};

} //

#include <FEM2D/solver_types/FEM/TriangleMeshFEM/detail/TriangleFEMStarter.inl>
