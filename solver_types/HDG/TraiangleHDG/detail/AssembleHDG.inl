#pragma once

#include <FEM2D/solver_types/HDG/TraiangleHDG/AssembleHDG.h>
#include <FEM2D/solver_types/features/triquadrature.inl>

namespace FEM2D::solvers::TriHDG
{

template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::assemble_equation(const mesh_type_pointer &mesh_data)
{
    try
    {
        // locate memory for global matrix & rhs
        locate_datas(mesh_data);

        // create equation system 

        // assemble BC

        // Solve SLES

        // postrocessing ?

        // visualization


    }
    catch(const std::exception& e)
    {
        std::runtime_error("AssembleHDG::assemble_equation " + std::string(e.what));
    }

    return true;
}


template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::locate_datas(const mesh_type_pointer &mesh_data)
{
    try
    {
        m_mesh = std::move(mesh_data);

        // size of Linear Equations System
        m_system_size = m_mesh->get_edges_size() + m_mesh->get_elements_size();

        // locate global HDG matrix & rhs memory
        m_global_matrix.zeros(m_system_size, m_system_size);
        m_global_vector.zeros(m_system_size);
    }
    catch(const std::exception& e)
    {
        std::runtime_error("AssembleHDG::assemble_equation " + std::string(e.what));
    }
    
}


template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::assemble_equation(const ell_equation_type &equation)
{
    try
    {
        // locate arrays for local matrix & rhs
        matrix_type local_matrix(m_dof, m_dof);
        vector_type local_rhs(m_dof);

        // discrette derivative matrix
        matrix_type discrette_derivative(3, m_dof);

        std::size_t N_triangles = m_mesh->get_elements_size();

        // loop by triangles
        for(std::size_t e = 0; e < N_triangles; e++)
        {
            // calculate discrette dirivative matrix for element
            this->calculate_discrette_derivative_matrix(discrette_derivative, e);

            // get Parts of Local Matrix A
            // A = A_1 + A_2 + A_3 + E
            this->calculate_A1(equation, lolcal_matrix, discrette_derivative, e);

            //this->calculate_A2

            //this->calculate_A3

            //this->calculate_E

            this->calculate_F_local(equation, lolcal_vector, e);

            // Write realization
            this->assemble_matrix(local_matrix, e);
            this->assemble_vector(local_vector, e);
        }

    }
    catch(const std::exception& e)
    {
        std::runtime_error("AssembleHDG::assemble_equation " + std::string(e.what));
    }

    return true;
}



template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::calculate_discrette_derivative_matrix(
        matrix_type &derivative_matrix,
        const std::size_t element_index
)
{
    try
    {
        std::vector<point_2d> triangle_points = m_mesh->get_points_by_triangle_id(element_index);
        auto triangle_area = m_mesh->m_areas_triangle[e];
        auto l = m_mesh->get_l(e);

        // first column
        derivative_matrix(0, 0) = 0;
        derivative_matrix(1, 0) = 0;
        derivative_matrix(2, 0) = -72 / l;

        // second column
        derivative_matrix(0, 1) = (m_mesh->get_point_by_id(triangle_points[2]).y()
                                    - m_mesh->get_point_by_id(triangle_points[1]).y()) / triangle_area;
        derivative_matrix(1, 1) = (m_mesh->get_point_by_id(triangle_points[1]).x()
                                    - m_mesh->get_point_by_id(triangle_points[2]).x()) / triangle_area;
        derivative_matrix(2, 1) = 24 / l;

        // third column
        derivative_matrix(0, 2) = (m_mesh->get_point_by_id(triangle_points[0]).y()
                                    - m_mesh->get_point_by_id(triangle_points[2]).y()) / triangle_area;
        derivative_matrix(1, 2) = (m_mesh->get_point_by_id(triangle_points[2]).x()
                                    - m_mesh->get_point_by_id(triangle_points[0]).x()) / triangle_area;
        derivative_matrix(2, 2) = 24 / l;

        // fourth column
        derivative_matrix(0, 3) = (m_mesh->get_point_by_id(triangle_points[1]).y()
                                    - m_mesh->get_point_by_id(triangle_points[0]).y()) / triangle_area;
        derivative_matrix(1, 3) = (m_mesh->get_point_by_id(triangle_points[0]).x()
                                    - m_mesh->get_point_by_id(triangle_points[1]).x()) / triangle_area;
        derivative_matrix(2, 3) = 24 / l;
        
    }
    catch(const std::exception& e)
    {
        std::runtime_error("AssembleHDG::calculate_discrette_derivative_matrix " + std::string(e.what));
    }
}


template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::calculate_A1(
        const ell_equation_type &equation
        matrix_type &local_matrix,
        const matrix_type& discrette_derivative,
        const std::size_t element_index
) const 
{
    try
    {
        matrix_type phi = arma::zeros(2, 3);

        matrix_type M = arma::zeros(m_dof, m_dof);

        // function for calculate matrix A at point
        auto A_func = [&equation](const point_2d point){
            matrix_type A_matr(2,2);

            A_matr(0,0) = equation.f_a11(point.x(), point.y()); 
            A_matr(1,0) = equation.f_a21(point.x(), point.y()); 
            A_matr(0,1) = equation.f_a12(point.x(), point.y()); 
            A_matr(1,1) = equation.f_a22(point.x(), point.y());

            return A_matr; 
        };

        // get edge's centers
        // get list of edges centers
        auto triangle_edges_centers = m_mesh->get_triangle_edges_centers(element_index);


        auto triangle_points = m_mesh->get_points_by_triangle_id(element_index);
        // get 2'd order Gauss quadrature points std::pair<tuple<point_2d>, tuple<value_type>>
        auto quadrature_data  = FEM2D::solvers::features::triquadrature::get_quad_2(triangle_points);

        for(std::size_t s = 0; s < 3; s++)
        {
            // get PHI
            this->get_phi_matrix(phi, triangle_edges_centers[s], m_mesh->get_mass_center(element_index));

            M = M + std::get<s>(quadrature_data.second) *
                phi.t() * A_func(std::get<s>(quadrature_data.first)) * phi;
        }

        local_matrix += discrette_derivative.t() * M * discrette_derivative;

        return true;
    }
    catch(const std::exception& e)
    {
        std::runtime_error("AssembleHDG::calculate_A1 " + std::string(e.what));
    }
}



template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::calculate_F_local(
        const ell_equation_type &equation
        vector_type &local_vector,
        const std::size_t element_index
) const 
{
    try
    {
        auto triangle_edges_centers = m_mesh->get_triangle_edges_centers(element_index);

        value_type triangle_area = m_mesh->get_triangle_area(element_index);

        local_vector(0) = triangle_area / 3 * (
            equation.f_f(triangle_edges_centers[0].x(), triangle_edges_centers[0].y()) + 
            equation.f_f(triangle_edges_centers[1].x(), triangle_edges_centers[1].y()) + 
            equation.f_f(triangle_edges_centers[2].x(), triangle_edges_centers[2].y())  
        ); 

        return true;
    }
    catch(const std::exception& e)
    {
        std::runtime_error("AssembleHDG::calculate_F_local " + std::string(e.what));
    }
    

}


template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::assemble_matrix(
    const matrix_type& local_matrix,
    std::size_t element_index
)
{
    try
    {
        auto traingle_edge_connectivity = m_mesh->get_traingle_edges_id(e);

        assert(traingle_edge_connectivity.size() == 3);

        auto assembling_indexes = {
            element_index,
            std::get<1>(traingle_edge_connectivity),
            std::get<2>(traingle_edge_connectivity),
            std::get<3>(traingle_edge_connectivity),
        };

        for(std::size_t i = 0; i < 4; i++)
        {
            for(std::size_t j = 0; j < 4; j++)
            {
                m_global_matrix(assembling_indexes[i], assembling_indexes[j]) +=
                    local_matrix(i, j);
            }
        }
    }
    catch(const std::exception& e)
    {
        std::runtime_error("AssembleHDG::assemble_matrix " + std::string(e.what));
    }
    
}


template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::assemble_vector(
        const vector_type& local_vector,
        std::size_t element_index
)
{
    try
    {
        auto traingle_edge_connectivity = m_mesh->get_traingle_edges_id(e);

        assert(traingle_edge_connectivity.size() == 3);

        auto assembling_indexes = {
            element_index,
            std::get<1>(traingle_edge_connectivity),
            std::get<2>(traingle_edge_connectivity),
            std::get<3>(traingle_edge_connectivity),
        };

        for(std::size_t i = 0; i < 4; i++)
        {
            m_global_vector(assembling_indexes[i]) += 
                m_local_vector(i);
        }
    }
    catch(const std::exception& e)
    {
        std::runtime_error("AssembleHDG::assemble_vector " + std::string(e.what));    
    }
    
}


template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::get_phi_matrix(
        matrix_type& phi,
        const point_2d& edge_center,
        const point_2d& triangle_mass_center
) const 
{
    try
    {
        phi(0, 0) = 1.;
        phi(1, 0) = 0.;
        
        phi(0, 1) = 0.;
        phi(1, 1) = 1.;
        
        phi(0, 2) = edge_center.x() - triangle_mass_center.x();
        phi(1, 2) = edge_center.y() - triangle_mass_center.y();
    }
    catch(const std::exception& e)
    {
        std::runtime_error("AssembleHDG::get_phi_matrix " + std::string(e.what));
    }
    
}

} //