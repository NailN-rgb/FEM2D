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
    using index_type = IndexType;
    using value_type = ValueType;
    using point_2d = typename bg::geo<value_type>::point_2d;

    using vector_of_values = std::vector<value_type>;
    using tri_edge_list    = std::vector<std::tuple(std::size_t, std::size_t, std::size_t)>;

// mesh type
    using mesh_type = typename FEM2D::mesh::mesh_types::MeshBase<IndexType, ValueType>;
    using mesh_type_pointer = std::unique_ptr<mesh_type>;

    using ell_equation_type = FEM2D::equation::EllepticEquation<index_type, value_type>;
    using ell_equation_pointer_type = std::unique_ptr<ell_equation_type>;

// boost compressed matrix
// TODO: is really sparse?
    using matrix_type = arma::mat; 

// boost compressed vector
    using vector_type = arma::vec; 

private:
    matrix_type m_global_matrix;
    vector_type m_global_vector;
    vector_type m_solution;

// degree of freedom
private:
    std::size_t m_dof = 4;
    std::size_t m_system_size;
    std::size_t m_edges_count;
    std::size_t m_elems_count;

private:
    mesh_type_pointer m_mesh;

private:
    tri_edge_list m_triedge;

public:
    AssembleHDG() = default;
    AssembleHDG(const AssembleHDG& hdg_assembler) = default;
    ~AssembleHDG() = default;

public:
    bool assemble_equation(
        const mesh_type_pointer& mesh_data,
        const ell_equation_type& equation
    );

private:
    bool locate_datas(const mesh_type_pointer& mesh_data);

private:
    bool create_equation_system(const ell_equation_type &equation);

private:
    bool calculate_discrette_derivative_matrix(
        matrix_type &derivative_matrix,
        const std::size_t element_index
    );

private:
    bool calculate_A1(
        const ell_equation_type &equation
        matrix_type &local_matrix,
        const matrix_type& discrette_derivative,
        const std::size_t element_index
    ) const;

private:
    bool calculate_F_local(
        const ell_equation_type &equation
        vector_type &local_vector,
        const std::size_t element_index
    ) const;

private:
    bool assemble_matrix(
        const matrix_type& local_matrix,
        std::size_t element_index
    );

private:
    bool assemble_vector(
        const vector_type& local_vector,
        std::size_t element_index
    );

private:
    bool get_phi_matrix(
        matrix_type& phi,
        const point_2d& edge_center,
        const point_2d& triangle_mass_center
    ) const;

private:
    bool assemble_boundary_conditions(
        const ell_equation_type &ell_equation
    );

private:
    template<typename NodesList>
    void assemble_first_bc(
        NodesList indexes,
        const ell_equation_type &ell_equation
    );

private:
    template<typename NodesList>
    void assemble_third_bc(NodesList nodes);

private:
    void get_solution_error();

};

} //

#include <FEM2D/solver_types/HDG/TriangleHDG/detail/AssembleHDG.inl>