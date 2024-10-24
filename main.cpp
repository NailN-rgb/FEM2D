#include <FEM2D/solver/solver.h>

int main()
{
    // start solver
    try
    {
        using index_type = int;
        using value_type = double;

        FEM2D::solver::solver_runner<index_type, value_type>::apply();
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("main::" + std::string(e.what()));
    }
    
    std::cout << "Solver Finished" << std::endl;

    return 0;
}