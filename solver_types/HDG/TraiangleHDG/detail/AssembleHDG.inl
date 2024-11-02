#pragma once

#include <FEM2D/solver_types/HDG/TraiangleHDG/AssembleHDG.h>
#include <FEM2D/solver_types/features/triquadrature.inl>

namespace FEM2D::solvers::TriHDG
{

template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::assemble_equation(
    const mesh_type_pointer &mesh_data,
    const ell_equation_type& equation
)
{
    try
    {
        // locate memory for global matrix & rhs
        locate_datas(mesh_data);

        // create equation system 
        create_equation_system(equation);

        // assemble BC
        assemble_boundary_conditions(equation);

        // Solve SLES
        m_solution = arma::solve(m_global_matrix, m_global_vector);

        get_solution_error();
        // postrocessing ?

        // visualization


    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("AssembleHDG::assemble_equation " + std::string(e.what));
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

        m_edges_count = m_mesh->get_edges_size();
        m_elems_count = m_mesh->get_elements_size();

        // size of Linear Equations System
        m_system_size = m_edges_count + m_elems_count;

        // locate global HDG matrix & rhs memory
        m_global_matrix.zeros(m_system_size, m_system_size);
        m_global_vector.zeros(m_system_size);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("AssembleHDG::assemble_equation " + std::string(e.what));
    }
    
}


template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::create_equation_system(const ell_equation_type &equation)
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
            local_matrix = arma::zeros(m_dof, m_dof);
            local_rhs    = arma::zeros(m_dof);

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
        throw std::runtime_error("AssembleHDG::assemble_equation " + std::string(e.what));
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
        throw std::runtime_error("AssembleHDG::calculate_discrette_derivative_matrix " + std::string(e.what));
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
        throw std::runtime_error("AssembleHDG::calculate_A1 " + std::string(e.what));
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
        throw std::runtime_error("AssembleHDG::calculate_F_local " + std::string(e.what));
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
                m_global_matrix(std::get<i>(assembling_indexes), std::get<j>(assembling_indexes)) +=
                    local_matrix(i, j);
            }
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("AssembleHDG::assemble_matrix " + std::string(e.what));
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
            m_global_vector(std::get<i>(assembling_indexes)) += 
                m_local_vector(i);
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("AssembleHDG::assemble_vector " + std::string(e.what));    
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
        throw std::runtime_error("AssembleHDG::get_phi_matrix " + std::string(e.what));
    }
    
}


template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::assemble_boundary_conditions(const ell_equation_type &ell_equation)
{
    try
    {
        // get list of edges that have voundary condition

        auto boundary_markers = m_mesh->get_bc_edge_markers();

        // sets of boundary edges
        std::unordered_set<std::size_t> first_bc_edges;
        std::unordered_set<std::size_t> third_bc_edges;

        std::size_t edge_idx = 0;

        std::for_each(
            boundary_markers.begin(),
            boundary_markers.end(),
            [&](const auto& marker)
            {
                if(marker == 1)
                {
                    first_bc_edges.insert(edge_idx);
                }
                else if(marker == 3)
                {
                    third_bc_edges.insert(edge_idx);
                }
                else
                {
                    throw std::runtime_error("Unknown type of BC at " + edge_idx + "edge");
                }

                edge_idx++;
            }
        );

        // TODO: not realized yet
        // assemble_third_bc();

        assemble_first_bc();
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("AssembleHDG::assemble_boundary_conditions " + std::string(e.what));
    }

    return true;
}

template<typename NodesList>
template<
    typename IndexType,
    typename ValueType
> bool AssembleHDG<IndexType, ValueType>::assemble_first_bc(
        NodesList indexes,
        const ell_equation_type &ell_equation
)
{
    try
    {
        // offset of system
        auto offset = m_elems_count;

        // get solution at first BC edges
        for(std::size_t i = 0; i < m_edges_count; i++)
        {
            // if node have a Dirichlet bc marker 
            if(nodes.count(i) > 0)
            {
                auto edge_center = m_mesh->get_point_by_edge_id(i);
                value_type expl_solution = ell_equation.sol_f(edge_center.x(), edge_center.y());

                m_solution(offset + i) = expl_solution;
            }
        }

        // change rhs vector elements
        for(std::size_t i = 0; i < m_edges_count; i++)
        {
            m_global_vector(offset + i) -= arma::dot(m_global_matrix.row(offset + i), m_solution);
        }

        // assemble to global matrix
        for(std::size_t i = 0; i < m_edges_count; i++)
        {
            // if node have a Dirichlet bc marker 
            if(nodes.count(i) > 0)
            {
                arma::rowvec row(m_system_size);

                vector_type col(m_system_size);
                // TODO: rewrite this
                col(offset + i) = m_solution(i) > 0.00001 ? m_global_vector(i) / m_solution(offset + i) : 1e9;

                m_global_matrix.row(offset + i) = row;
                m_global_matrix.col(offset + i) = col;
            }
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("AssembleHDG::assemble_first_bc " + std::string(e.what));
    }   
}


template<
    typename IndexType,
    typename ValueType
> void AssembleHDG<IndexType, ValueType>::get_solution_error(const ell_equation_type& equation)
{
    // get true solution
    std::vector<value_type> true_solution;
    std::vector<value_type> error;
    // 1. calclulate at triangles
    for(std::size_t i = 0; i < m_mesh->get_elements_size(); i++)
    {
        auto triangle_center = m_mesh->get_mass_center(i);
        true_solution.push_back(
            equation.sol_f(triangle_center.x(), triangle_center.y());
        );
    }

    // 2. Calculate at edges
    for(std::size_t i = 0; i < m_mesh->get_edges_size(); i++)
    {
        auto edge_center = m_mesh->get_point_by_edge_id(i);
        true_solution.push_back(
            equation.sol_f(edge_center.x(), edge_center.y());
        );
    }

    for(std::size_t i = 0; i < m_system_size; i++)
    {
        error.push_back(std::fabs(m_solution(i) - true_solution[i]));
    }

    std::sort(
        error.begin(),
        error.end()
    );

    std::cout << "Max Error is " << error.back() << std::endl;
}

} //