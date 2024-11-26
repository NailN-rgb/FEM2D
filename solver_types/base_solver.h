#pragma once
#include <FEM2D/precompiled.h>
#include <FEM2D/equationInit/EllipticEquation.h>

namespace FEM2D::solvers::BaseSolver
{

template<
    typename IndexType,
    typename ValueType
> class BaseSolverStarter
{
protected:
    using index_type = IndexType;
    using value_type = ValueType;

protected:
    using ell_equation_type = FEM2D::equation::EllepticEquation<index_type, value_type>;

protected:
    using mesh_type = typename FEM2D::mesh::mesh_types::MeshBase<IndexType, ValueType>;
    using mesh_type_pointer = std::shared_ptr<mesh_type>;

public:
    ell_equation_type m_elliptic_equation;

public:
    BaseSolverStarter() = default;
    BaseSolverStarter(const BaseSolverStarter& triangle_solver) = default;
    ~BaseSolverStarter() = default;

// virtual method need to be overrided at each child class
public:
    virtual bool solve(
        int solving_algorithm,
        const mesh_type_pointer &mesh_data
    ) = 0;

// set equation
protected:
    void set_elliptic_equation(
        const mesh_type_pointer &mesh_data
    );
};

} //

#include <FEM2D/solver_types/detail/base_solver.inl>
