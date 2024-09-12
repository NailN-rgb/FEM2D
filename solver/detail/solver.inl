#include <FEM2D/mesh/mesh_builder.h>


namespace FEM2D
{
namespace solver
{

template<
    typename IndexType,
    typename ValueType
> static void solver_runner<IndexType, ValueType>::apply() noexcept(false)
{
    try
    {
        using mesh_builder_type = FEM2D::mesh::Mesh_builder<index_type, value_type>;
        using FEM_solver_type   = FEM2D::solvers::TriFem::TriangleFEMStarter<index_type, value_type>;

        mesh_builder_type mesh;
        auto mesh_ptr = mesh.build_mesh(true);

        FEM_solver_type fem_solver;
        fem_solver.solve();

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