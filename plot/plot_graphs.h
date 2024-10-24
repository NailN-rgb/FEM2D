#pragma once

#include <FEM2D/precompiled.h>
#include <FEM2D/plot/matplotlibcpp.h>

#include <FEM2D/mesh/boost_datatypes/types.h>


// CMAKE Flag -lpython2.7

namespace FEM2D
{

namespace Plot
{

namespace bg = boost::geometry;

void TestPlot()
{
    std::vector<std::vector<double>> x, y, z;
    for (double i = -5; i <= 5;  i += 0.25) {
        std::vector<double> x_row, y_row, z_row;
        for (double j = -5; j <= 5; j += 0.25) {
            x_row.push_back(i);
            y_row.push_back(j);
            z_row.push_back(::std::sin(::std::hypot(i, j)));
        }
        x.push_back(x_row);
        y.push_back(y_row);
        z.push_back(z_row);
    }

    matplotlibcpp::plot_surface(x, y, z);
    const char* filename = "basic.png";
    std::cout << "Saving result to " << filename << std::endl;;
    matplotlibcpp::save(filename);
}


template<typename MeshData>
bool PlotMesh(
    const MeshData& mesh
)
{
    try
    {
        using point_type = typename bg::geo<double>::point_2d;
        using index_type = int;

        // get mesh datas
        std::vector<point_type> points = mesh->get_points();
        std::vector<std::vector<index_type>> segments = mesh->get_edges();
        std::vector<std::vector<index_type>> triangles = mesh->get_elements();

        std::vector<double> x(points.size());
        std::vector<double> y(points.size());

        // get points arrays
        std::for_each(
            points.begin(),
            points.end(),
            [&x, &y](const auto& point)
            {
                x.push_back(point.x());
                y.push_back(point.y());
            }
        );

        // plot points 
        matplotlibcpp::scatter(x, y, 1., {{"color", "blue"}, {"label", "Points"}});

        // plot edges
        std::vector<double> x_edge(2);
        std::vector<double> y_edge(2);

        std::for_each(
            segments.begin(),
            segments.end(),
            [&x_edge, &y_edge, &points](const auto& edge_points)
            {
                x_edge[0] = points[edge_points[0]].x();
                x_edge[1] = points[edge_points[1]].x();

                y_edge[0] = points[edge_points[0]].y();
                y_edge[1] = points[edge_points[1]].y();

                matplotlibcpp::plot(x_edge, y_edge, "r");
            }
        );

        const char* filename = "TriangleMesh.png";
        matplotlibcpp::save(filename);

    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Error when plotting mesh: " + std::string(e.what()));
        return false;
    }

    return true;
}


template<
    typename Vector,
    typename MeshData
> bool PlotExplicitSolution(
    Vector& solution,
    const MeshData& mesh
)
{
    try
    {
        // get mesh datas
        auto points = mesh->get_points();
        
        std::vector<double> x;
        std::vector<double> y;

        // get points arrays
        std::for_each(
            points.cbegin(),
            points.cend(),
            [&x, &y](const auto& point)
            {
                x.push_back(point.x());
                y.push_back(point.y());
            }
        );

        matplotlibcpp::scatter(x, y, solution);
        const char* filename = "solution.png";
        matplotlibcpp::save(filename);

        return true;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("PlotSolution: " + std::string(e.what()));
    }

    return true;
}


template<
    typename Vector,
    typename MeshData
> bool PlotSolution(
    Vector& solution,
    const MeshData& mesh
) 
{
    try
    {
        // get mesh datas
        auto points = mesh->get_points();
        
        std::vector<double> x;
        std::vector<double> y;

        // get points arrays
        std::for_each(
            points.cbegin(),
            points.cend(),
            [&x, &y](const auto& point)
            {
                x.push_back(point.x());
                y.push_back(point.y());
            }
        );

        matplotlibcpp::scatter(
            x, 
            y, 
            arma::conv_to<std::vector<double>>::from(solution)
        );
        const char* filename = "solution_calculated.png";
        matplotlibcpp::save(filename);

        return true;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("PlotSolution: " + std::string(e.what()));
    }

    return true;
}


} //
} //