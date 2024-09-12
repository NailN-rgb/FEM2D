#pragma once

#include <FEM2D/precompiled.h>

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>

#include <FEM2D/mesh/mesh_types/mesh_base.h> // include trimesh



// class for assemble FEM system by given mesh
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
class AssembleEquation
{
public:
    using index_type = IndexType;
    using value_type = ValueType;

public:
    using ell_equation_type = FEM2D::Equation::EllepticEquation<index_type, value_type>;

// pointer to mesh
public:
    using trimesh_type = std::unique_ptr<FEM2D::mesh::mesh_types::MeshBase<index_type, value_type>>;

// boost compressed matrix
public:
    using sparse_matrix_type = boost::numeric::ublas::compressed_matrix<value_type>; 

// boost compressed vector
public:
    using sparse_vector_type = boost::numeric::ublas::compressed_vector<value_type>; 

public:
// 1-Point FEM basis functions gradients
    using basis_grads_list_type = boost::numeric::ublas::matrix<value_type>;

private:
    ell_equation_type m_equation;

private:
    trimesh_type m_trimesh;

private:
    sparse_matrix_type m_global_matrix;

private:
    sparse_vector_type m_global_vector;

private:
    index_type N_points;

private:
    index_type N_triangles;

// degree of freedom
private:
    index_type m_dof = 3;

public:
    AssembleEquation() = default;
    AssembleEquation(const AssembleEquation& ae) = default;
    ~AssembleEquation() = default;

public:
    void set_equation_params();

public:
    void assemble_equation();

private:
    void create_equation_system();

private:
    boost::numeric::ublas::matrix<double> get_basis_gradients_on_element(
        std::unique_ptr<FEM2D::mesh::mesh_types::MeshBase<index_type, value_type>> trimesh,
        const basis_grads_list_type &grads,
        index_type triangle_index
    );
    


};

} //
} //
} //