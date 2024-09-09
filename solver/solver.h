#pragma once

namespace FEM2D
{
namespace solver
{

template<
    typename IndexType,
    typename ValueType
>
class solver_runner
{
public:
    using index_type = IndexType;
    using value_type = ValueType;
    
public:
    static void apply() noexcept(false);
};

} //

} //

#include <FEM2D/solver/detail/solver.inl>