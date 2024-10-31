#pragma once

#include <FEM2D/solver_types/base_solver.h>

namespace FEM2D::solvers::BaseSolver
{

template<
    typename IndexType,
    typename ValueType
> void BaseSolverStarter<IndexType, ValueType>::set_elliptic_equation(
    const mesh_type_pointer &mesh_data
)
{
    try
    {
        // May be we need some entry equation data parce?

        // set points and calculate fields at points
        m_elliptic_equation.calculate_at_points(mesh_data->get_mass_centers());
        m_elliptic_equation.solution_at_point(mesh_data->get_points());
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("set_elliptic_equation:: " + std::string(e.what()));
    }
}

} //
