#pragma once
// class to call other FEM2D classes

#include <FEM2D/precompiled.h>
#include <FEM2D/equationInit/EllipticEquation.h>

#include <FEM2D/FEM/TriangleMeshFEM/AssembleEquation.h>

#include <FEM2D/mesh/mesh_types/mesh_base.h>

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
class TriangleFEMStarter
{
public:
    using index_type = IndexType;
    using value_type = ValueType;

public:
    using ell_equation_type = FEM2D::equation::EllepticEquation<index_type, value_type>;

public:
    using equation_assembler_type = FEM2D::solvers::TriFem::AssembleEquation<index_type, value_type>;

public:
    using mesh_type = typename FEM2D::mesh::mesh_types::MeshBase<IndexType, ValueType>;
    using mesh_type_pointer = std::unique_ptr<mesh_type>;

private:
    ell_equation_type m_elliptic_equation;

private:
    equation_assembler_type m_assembler; 

    // constructor

public:
    TriangleFEMStarter() = default;
    TriangleFEMStarter(const TriangleFEMStarter& trifem) = default;
    ~TriangleFEMStarter() = default;

// main solving function
public:
    void solve(
        int solving_algorithm,
        const mesh_type_pointer &mesh_data
    );

// call & complete equation initializer
public:
    void set_elliptic_equation(
        const mesh_type_pointer &mesh_data
    );
    
// call & complete matrix/RHS builder 
public:
    void get_fem_system(
        const mesh_type_pointer &mesh_data
    ); 

// call Equation System solver
public:
    void solve();

};

} //
} //
} //

#include <FEM2D/FEM/TriangleMeshFEM/detail/TriangleFEMStarter.inl>
