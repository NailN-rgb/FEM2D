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

        mesh_builder_type mesh;
        mesh.parse();
        mesh.build_mesh();
        mesh.save_mesh();

        // TODO: get pointer to mesh class from mesh builder

        // select solver type

        std::string solver_type = "FEM";

        switch(solver_type)
        {
            case "FEM"
                break;

            case "HGD"
                break;

            case "FVM"
                break;
        }

    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("solver::solver_runner:: " + std::string(e.what()));
    }
}

} //

} //