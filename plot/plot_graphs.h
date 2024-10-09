#pragma once

#include <FEM2D/precompiled.h>
#include <FEM2D/plot/matplotlibcpp.h>


// CMAKE Flag -lpython2.7

namespace FEM2D
{

namespace Plot
{

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
    const char* filename = "./basic.png";
    std::cout << "Saving result to " << filename << std::endl;;
    matplotlibcpp::save(filename);
}


} //
} //