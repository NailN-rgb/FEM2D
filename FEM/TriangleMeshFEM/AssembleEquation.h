#pragma once

#include <FEM2D/precompiled.h>

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>

#include <FEM2D/mesh/mesh_types/mesh_base.h> // include trimesh

#include <FEM2D/mesh/boost_datatypes/types.h>


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
    using point_2d = typename bg::geo<value_type>::point_2d;

// mess type
public:
    using mesh_type = typename FEM2D::mesh::mesh_types::MeshBase<IndexType, ValueType>;
    using mesh_type_pointer = std::unique_ptr<mesh_type>;

public:
    using ell_equation_type = FEM2D::equation::EllepticEquation<index_type, value_type>;
    using ell_equation_pointer_type = std::unique_ptr<ell_equation_type>;

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
    sparse_matrix_type m_global_matrix;

private:
    sparse_vector_type m_global_vector;

// degree of freedom
private:
    index_type m_dof = 3;

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
    boost::numeric::ublas::matrix<double> get_basis_gradients_on_element(
        const mesh_type_pointer &trimesh,
        const basis_grads_list_type &grads,
        index_type triangle_index
    );
    
};

} //
} //
} //

#include <FEM2D/FEM/TriangleMeshFEM/detail/AssembleEquation.inl>