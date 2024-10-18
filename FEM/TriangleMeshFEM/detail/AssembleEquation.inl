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
> bool AssembleEquation<IndexType, ValueType>::assemble_equation(
    const mesh_type_pointer &mesh_data,
    const ell_equation_type &ell_equation
)
{
    try
    {
        // init arrays
        m_nodes_count = mesh_data->get_nodes_size();
        index_type dirichlet_bc_count = mesh_data->get_dirichlet_bc_count();

        m_global_matrix.zeros(m_nodes_count, m_nodes_count);
        m_global_vector.zeros(m_nodes_count);
        m_solution.zeros(m_nodes_count);

        this->create_equation_system(mesh_data, ell_equation);

        this->assemble_boundary_conditions(mesh_data, ell_equation);

        // TODO: #ifdef
        m_global_matrix.save("FEM matrix.txt", arma::raw_ascii);

        m_global_vector.save("FEM vector.txt", arma::raw_ascii);
        

        // solve
        m_solution = arma::solve(m_global_matrix, m_global_vector);

        // print mesh & solution
        FEM2D::Plot::PlotMesh(mesh_data);
        FEM2D::Plot::PlotSolution(ell_equation.solution, mesh_data);
        
       
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("AssembleEquation::assemble_equation::" + std::string(e.what()));
    }
    
    
    return true;
}


template<
    typename IndexType,
    typename ValueType
> bool AssembleEquation<IndexType, ValueType>::create_equation_system(
    const mesh_type_pointer &mesh_data,
    const ell_equation_type &equation
)
{
    try
    {
        // here we know the matrix and rhs vector sizes (m_trimesh->nodes.size())

        // local matrix container
        matrix_type local_matrix(3, 3);
        vector_type local_rhs(3);
        
        // basis gradients lsit
        matrix_type basis_grads(3, 2); 

        // calculate equation Functions at triangle mass centres
        // 1. Set mass centers to equation
        //equation.calculate_at_points(mesh_data->m_mass_centers_elems);

        index_type N_triangles = mesh_data->get_elements_size();

        // TODO: parralelize this
        for(index_type e = 0; e < N_triangles; e++)
        {
            std::vector<point_2d> triangle_points = mesh_data->get_points_by_triangle_id(e);

            // triangle area
            auto triangle_area = mesh_data->m_areas_triangle[e];

            // get basis functions gradients
            basis_grads = get_basis_gradients_on_element(mesh_data, triangle_points, triangle_area);

            // local matrix calculation loop
            for(index_type i = 0; i < m_dof; i++)
            {
                for(index_type j = 0; j < m_dof; j++)
                {
                    local_matrix(i,j) = triangle_area * ( 
                                        equation.get_a11(e) * basis_grads(j , 0) * basis_grads(i, 0) + 
                                        equation.get_a12(e) * basis_grads(j , 1) * basis_grads(i, 0) + 
                                        equation.get_a21(e) * basis_grads(j , 0) * basis_grads(i, 1) + 
                                        equation.get_a22(e) * basis_grads(j , 1) * basis_grads(i, 1) +
                                        (equation.get_b1(e)  * basis_grads(i, 0)) / 3 +
                                        (equation.get_b2(e)  * basis_grads(i, 1)) / 3 +
                                        (equation.get_c1(e)  * basis_grads(j, 0)) / 3 +
                                        (equation.get_c2(e)  * basis_grads(j, 1)) / 3 + 
                                        (equation.get_a(e)) / 9
                    );
                                        
                }
            }

            // get local rhs value
            local_rhs(0) = triangle_area * equation.get_f(e) / 3;

            // assemble local matrixes into global
            
            this->assemble_matrix(local_matrix, mesh_data->get_node_id(e));
            this->assemble_vector(local_rhs, mesh_data->get_node_id(e));
        }

        if(!m_global_matrix.is_finite())
        {
            m_global_matrix.save("FEM matrix.txt", arma::raw_ascii);
            throw std::runtime_error("global matrix have a infinite elem");
        }

        if(m_global_matrix.has_nan())
        {
            m_global_matrix.save("FEM matrix.txt", arma::raw_ascii);
            throw std::runtime_error("global matrix have a nan- elem");
        }

        return true;
    }
    catch(const std::exception &e)
    {
        throw std::runtime_error("AssembleEquation::create_equation_system::" + std::string(e.what()));
    }

    return false;
    
}


template<
    typename IndexType,
    typename ValueType
> arma::mat AssembleEquation<IndexType, ValueType>::get_basis_gradients_on_element(
    const mesh_type_pointer &trimesh,
    const std::vector<point_2d> &triangle_points,
    value_type tri_area
)
{
    matrix_type basis_derivative_matrix(3, 2);

    // components of first basis function (a_3 - a_1)^ort / 2|e|
    basis_derivative_matrix(0, 0) = - (triangle_points[2].y() - triangle_points[0].y()) / (2 * tri_area);
    basis_derivative_matrix(0, 1) =   (triangle_points[2].x() - triangle_points[0].x()) / (2 * tri_area);
    
    // components of first basis function (a_3 - a_1)^ort / 2|e|
    basis_derivative_matrix(1, 0) = - (triangle_points[0].y() - triangle_points[2].y()) / (2 * tri_area);
    basis_derivative_matrix(1, 1) =   (triangle_points[0].x() - triangle_points[2].x()) / (2 * tri_area);

    // components of first basis function (a_3 - a_1)^ort / 2|e|
    basis_derivative_matrix(2, 0) = - (triangle_points[1].y() - triangle_points[0].y()) / (2 * tri_area);
    basis_derivative_matrix(2, 1) =   (triangle_points[1].x() - triangle_points[0].x()) / (2 * tri_area);

    return basis_derivative_matrix;
}


template<
    typename IndexType,
    typename ValueType
> bool AssembleEquation<IndexType, ValueType>::assemble_matrix(
    const matrix_type &local_matr, 
    const std::vector<index_type> &global_indeces
)
{
    try
    {
        for(index_type i = 0; i < m_dof; i++)
        {
            for(index_type j = 0; j < m_dof; j++)
            {
                value_type val = local_matr(i, j);
                // we need '-1' for transition from triangle lib numeration to C++ arrays numeration
                m_global_matrix(global_indeces[i], global_indeces[j]) += val;
            }
        }
    }
    catch(const std::exception &e)
    {
        throw std::runtime_error("assemble_matrix " + std::string(e.what()));
    }
}


template<
    typename IndexType,
    typename ValueType
> bool AssembleEquation<IndexType, ValueType>::assemble_vector(
    const vector_type &local_vec, 
    const std::vector<index_type> &global_indeces
)
{
    try
    {
        for(index_type i = 0; i < m_dof; i++)
        {
            m_global_vector(global_indeces[i] - 1) += local_vec(i);
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("assemble_vector " + std::string(e.what()));
    }
}

} //
} //
} //