#pragma once


#include <FEM2D/FEM/TriangleMeshFEM/AssembleEquation.h> 

namespace FEM2D
{
namespace solvers
{

namespace TriFem
{

template<
    typename IndexType,
    typename ValueType
> bool AssembleEquation<IndexType, ValueType>::set_equation_params()
{

}


template<
    typename IndexType,
    typename ValueType
> bool AssembleEquation<IndexType, ValueType>::assemble_equation()
{
    this->create_equation_system();
}


template<
    typename IndexType,
    typename ValueType
> boost::numeric::ublas::matrix<double> AssembleEquation<IndexType, ValueType>::get_basis_gradients_on_element(
    std::unique_ptr<FEM2D::mesh::mesh_types::MeshBase<index_type, value_type>> trimesh,
    const basis_grads_list_type &grads,
    index_type triangle_index
)
{
    boost::numeric::ublas::matrix<double> basis_derivative_matrix(3, 2);

    // get vector of points of triangle
    std::vector<point_2d> triangle_points = trimesh->get_points_by_triangle_id(triangle_index);

    // get triangle area
    auto triangle_area = trimesh->areas_triangle[triangle_index];

    // components of first basis function (a_3 - a_1)^ort / 2|e|
    basis_derivative_matrix(0, 0) = - (triangle_points[2].y() - triangle_points[0].y()) / (2 * triangle_area);
    basis_derivative_matrix(0, 1) =   (triangle_points[2].x() - triangle_points[0].x()) / (2 * triangle_area);
    
    // components of first basis function (a_3 - a_1)^ort / 2|e|
    basis_derivative_matrix(1, 0) = - (triangle_points[0].y() - triangle_points[2].y()) / (2 * triangle_area);
    basis_derivative_matrix(1, 1) =   (triangle_points[0].x() - triangle_points[2].x()) / (2 * triangle_area);

    // components of first basis function (a_3 - a_1)^ort / 2|e|
    basis_derivative_matrix(2, 0) = - (triangle_points[1].y() - triangle_points[0].y()) / (2 * triangle_area);
    basis_derivative_matrix(2, 1) =   (triangle_points[1].x() - triangle_points[0].x()) / (2 * triangle_area);

    return basis_derivative_matrix;
}


template<
    typename IndexType,
    typename ValueType
> bool AssembleEquation<IndexType, ValueType>::create_equation_system()
{
    // here we know the matrix and rhs vector sizes (m_trimesh->nodes.size())

    // local matrix container
    boost::numeric::ublas::matrix<double> local_matrix(3, 3);
    boost::numeric::ublas::vector<double> local_rhs(3);
    
    // basis gradients lsit
    basis_grads_list_type basis_grads(3, 2); 

    // calculate equation Functions at triangle mass centres
    // 1. Set mass centers to equation
    m_equation.calculate_at_points(m_trimesh->get_elements_mass_centers());


    // TODO: parralelize this
    for(index_type e = 0; i < N_triangles; i++)
    {
        // get basis functions gradients
        basis_grads = get_basis_gradients_on_element(m_trimesh, basis_grads, e);

        // triangle area
        auto triangle_area = trimesh->areas_triangle[e];

        // local matrix calculation loop
        for(index_type i = 0; i < m_dof; i++)
        {
            for(index_type j = 0; j < m_dof; j++)
            {
                local_martix(i,j) = triangle_area * ( 
                                    m_equation.get_a11(e) * basis_grads(j , 1) * basis_grads(i, 1) + 
                                    m_equation.get_a12(e) * basis_grads(j , 2) * basis_grads(i, 1) + 
                                    m_equation.get_a21(e) * basis_grads(j , 1) * basis_grads(i, 2) + 
                                    m_equation.get_a22(e) * basis_grads(j , 2) * basis_grads(i, 2) +
                                    (m_equation.get_b1(e)  * basis_grads(i, 1)) / 3 +
                                    (m_equation.get_b2(e)  * basis_grads(i, 2)) / 3 +
                                    (m_equation.get_c1(e)  * basis_grads(j, 1)) / 3 +
                                    (m_equation.get_c2(e)  * basis_grads(j, 2)) / 3 + 
                                    (m_equation.get_a(e)) / 9
                );
                                    
            }
        }

        // get local rhs value
        local_rhs(0) = triangle_area * m_equation.get_f(e) / 3;

        // assemble local matrixes into global
        // TODO: realize this
        this->assemble_matrix(local_matrix);
        this->assemble_vector(local_rhs);
    }
}

} //
} //
} //
