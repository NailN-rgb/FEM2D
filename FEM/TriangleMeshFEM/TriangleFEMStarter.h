#pragma once
// class to call other FEM2D classes

#include <FEM2D/precompiled.h>
#include <FEM2D/equationInit/EllipticEquation.h>

#include <FEM2D/FEM/TriangleMeshFEM/AssembleEquation.h>

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
    using ell_equation_type = FEM2D::Equation::EllepticEquation<index_type, value_type>;

public:
    using equation_assembler_type = FEM2D::solvers::TriFem::AssembleEquation<index_type, value_type>;

public:
    ell_equation_type elliptic_equation; // TODO: give points to ctor 

public:
    equation_assembler_type assembler; 

    // constructor

public:
    TriangleFEMStarter() = default;
    TriangleFEMStarter(const TriangleFEMStarter& trifem) = default;
    ~TriangleFEMStarter() = default;

// main solving function
public:
    void solve();

public:
    void set_elliptic_equation();
    // call & complete equation initializer

    // call & complete matrix/RHS builder 
public:
    void get_fem_system()
    {
        assembler.set_equation_params(); // TODO
        assembler.assemble_equation();
    }

    // call Equation System solver
public:
    void solve();

};

} //
} //
} //

