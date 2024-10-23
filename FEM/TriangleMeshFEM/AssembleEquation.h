#pragma once

#include <FEM2D/precompiled.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/operation.hpp>

#include <FEM2D/mesh/mesh_types/mesh_base.h> // include trimesh

#include <FEM2D/mesh/boost_datatypes/types.h>

// for visualization
#include <FEM2D/plot/plot_graphs.h>

// class for assemble FEM system by given mesh
namespace FEM2D
{
namespace solvers
{

namespace TriFem
{

namespace bg = boost::geometry;

template<
    typename IndexType,
    typename ValueType
>
class AssembleEquation
{
public:
    using index_type = IndexType;
    using value_type = ValueType;

public: 
    using vector_of_values = std::vector<value_type>;

public:
    using point_2d = typename bg::geo<value_type>::point_2d;

// mess type
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
    index_type m_dof = 3;

private:
    index_type m_nodes_count;

private:
    index_type m_elems_count;

public:
    AssembleEquation() = default;
    AssembleEquation(const AssembleEquation& ae) = default;
    ~AssembleEquation() = default;

// class algorithms entry function
public:
    bool assemble_equation(
        const mesh_type_pointer &mesh_data,
        const ell_equation_type &ell_equation
    );

// get global matrix & rhs
private:
    bool create_equation_system(
        const mesh_type_pointer &mesh_data,
        const ell_equation_type &equation
    );

// get graidents of basis funcs
private:
    arma::mat get_basis_gradients_on_element(
        const mesh_type_pointer &trimesh,
        const std::vector<point_2d> &triangle_points,
        value_type tri_area
        );
    
private:
    bool assemble_matrix(const matrix_type& local_matrix, const std::vector<index_type> &global_indeces);

private:
    bool assemble_vector(const vector_type& local_vector, const std::vector<index_type> &global_indeces);

private:
    bool assemble_boundary_conditions(
        const mesh_type_pointer &trimesh,
        const ell_equation_type &ell_equation
    );

private:
    template<typename NodesList>
    void assemble_first_bc(
        NodesList nodes,
        const ell_equation_type &ell_equation
    );

private:
    template<typename NodesList>
    void assemble_third_bc(NodesList nodes);

};

} //
} //
} //

#include <FEM2D/FEM/TriangleMeshFEM/detail/AssembleEquation.inl>
#include <FEM2D/FEM/TriangleMeshFEM/detail/BoundaryConditions.inl>