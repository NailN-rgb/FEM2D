#pragma once

#include <FEM2D/precompiled.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/operation.hpp>

#include <FEM2D/mesh/boost_datatypes/types.h>

// for visualization
#include <FEM2D/plot/plot_graphs.h>


namespace FEM2D::solvers::TriHDG
{

namespace bg = boost::geometry;

template<
    typename IndexType,
    typename ValueType
>
class AssembleHDG
{
// HDG Method Assembler Class
public:
    using index_type = IndexType;
    using value_type = ValueType;

public: 
    using vector_of_values = std::vector<value_type>;

public:
    using point_2d = typename bg::geo<value_type>::point_2d;

// mesh type
public:
    using mesh_type = typename FEM2D::mesh::mesh_types::MeshBase<IndexType, ValueType>;
    using mesh_type_pointer = std::unique_ptr<mesh_type>;

public:
    using ell_equation_type = FEM2D::equation::EllepticEquation<index_type, value_type>;
    using ell_equation_pointer_type = std::unique_ptr<ell_equation_type>;

// boost compressed matrix
// TODO: is really sparse?
public:
    using matrix_type = arma::mat; 

// boost compressed vector
public:
    using vector_type = arma::vec; 

private:
    matrix_type m_global_matrix;

private:
    vector_type m_global_vector;

private:
    vector_type m_solution;

// degree of freedom
private:
    std::size_t m_dof = 4;

private:
    std::size_t m_nodes_count;

private:
    std::size_t m_elems_count;

public:
    AssembleHDG() = default;
    AssembleHDG(const AssembleHDG& hdg_assembler) = default;
    ~AssembleHDG() = default;

public:
    bool assemble_equation(const mesh_type_pointer &mesh_data);

};

} //

#include <FEM2D/solver_types/HDG/TriangleHDG/detail/AssembleHDG.inl>