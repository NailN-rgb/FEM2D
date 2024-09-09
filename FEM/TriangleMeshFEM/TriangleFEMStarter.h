#pragma once
// class to call other FEM2D classes

#include <FEM2D/precompiled.h>
#include <FEM2D/equationInit/EllipticEquation.h>

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
    ell_equation_type elliptic_equation; // TODO: give points to ctor 

    // constructor

public:
    TriangleFEMStarter() = default;
    TriangleFEMStarter(const TriangleFEMStarter& trifem) = default;
    ~TriangleFEMStarter() = default;

public:
    void set_elliptic_equation();
    // call & complete equation initializer

    // call & complete matrix/RHS builder 
public:
    void get_fem_system();

    // call Equation System solver
public:
    void solve();

};

} //
} //

