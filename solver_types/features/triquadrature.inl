#pragma once
#include <FEM2D/precompiled.h>

namespace FEM2D::solvers::features
{

struct triquadrature
{
    // get Gaussian points & weights
    template<typename point_2d>
    auto get_quad_2(
        std::vector<point_2d> triangle_points
    ) -> std::pair<
        std::tuple<point_2d, point_2d, point_2d>, 
        std::tuple<double, double, double>
    >
    {
        // check triangle points size
        assert(triangle_points.size() == 3);

        auto coordinate_transformations = get_trans_from_reference_to_arbitrary();

        // quadrature points for 2-nd order Gauss Integration
        std::tuple<point_2d, point_2d, point_2d> quad_points = 
            {point_2d(0.5, 0), point_2d(0, 0.5), point_2d(0.5, 0.5)};
        
        std::tuple<point_2d, point_2d, point_2d> quad_points_calculated = {
            point_2d(
                coordinate_transformations.first(std::get<0>(quad_points)),
                coordinate_transformations.second(std::get<0>(quad_points)),
            ),
            point_2d(
                coordinate_transformations.first(std::get<1>(quad_points)),
                coordinate_transformations.second(std::get<1>(quad_points)),
            ),
            point_2d(
                coordinate_transformations.first(std::get<2>(quad_points)),
                coordinate_transformations.second(std::get<2>(quad_points)),
            )
        };

        // quadrature points weights
        std::tuple<double, double, double> quad_weights = 
            {1/6, 1/6, 1/6};

        return std::make_pair<quad_points_calculated, quad_weights>;
    }


    // get lambdas to get quadrature point at arbitrary triangle
    template<typename point_2d>
    auto get_trans_from_reference_to_arbitrary(
        std::vector<point_2d> triangle_points
    )
    {
        // by formulae 
        // g_1 = x_1 + (x_2 - x_1)s + (x_3 - x_1)t
        // g_2 = y_1 + (y_2 - y_1)s + (y_3 - y_1)t

        auto g_1 = [&triangle_points](const point_2d& quadrature_point)
        {
            return triangle_points[0].x() + 
                   (triangle_points[1].x() - triangle_points[0].x()) * quadrature_point.x() +
                   (triangle_points[2].x() - triangle_points[0].x()) * quadrature_point.y(); 
        }

        auto g_2 = [&triangle_points](const point_2d& quadrature_point)
        {
            return triangle_points[0].y() + 
                   (triangle_points[1].y() - triangle_points[0].y()) * quadrature_point.x() +
                   (triangle_points[2].y() - triangle_points[0].y()) * quadrature_point.y(); 
        }

        return std::make_pair<g_1, g_2>;
    }
};



}//