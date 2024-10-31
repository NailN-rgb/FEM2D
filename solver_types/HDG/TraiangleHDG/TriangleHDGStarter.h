#pragma once

#include <FEM2D/precompiled.h>
#include <FEM2D/solver_types/HDG/TriangleHDG/AssembleHDG.h>

namespace FEM2D::solvers::TriHDG
{

template<
    typename BaseSolver
>
class TriangleHDGStarter : public BaseSolver 
{
// HDGFEM starter class
private:
    using index_type = typename BaseSolver::index_type;
    using value_type = typename BaseSolver::value_type;
    using mesh_type_pointer = typename BaseSolver::mesh_type_pointer;

private:
    using equation_assembler_type = FEM2D::solvers::TriHDG::AssembleHDG<index_type, value_type>;

private:
    equation_assembler_type m_assembler;

// default ctor's
public:
    TriangleHDGStarter() = default;
    TriangleHDGStarter(const TriangleHDGStarter& triangle_hdg) = default;
    ~TriangleHDGStarter() = default;

// main solving function
public:
    bool solve(
        int solving_algorithm,
        const mesh_type_pointer &mesh_data
    ) override;

// call & complete matrix/RHS builder 
private:
    void solve_hdg_system(const mesh_type_pointer &mesh_data); 
};

} //

#include <FEM2D/solver_types/HDG/TriangleHDG/detail/TriangleHDGStarter.inl>