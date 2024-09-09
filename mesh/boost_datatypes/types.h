#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <boost/geometry/multi/geometries/multi_linestring.hpp>
#include <boost/geometry/algorithms/unique.hpp>

namespace boost
{
namespace geometry
{
    template <typename ValueType>
    struct geo
    {
        using value_type = ValueType;

        typedef model::d2::point_xy<value_type, boost::geometry::cs::cartesian> point_2d;
        typedef model::linestring<point_2d> line_2d;
        typedef model::polygon<point_2d>    polygon_2d;
        typedef model::segment<point_2d>    segment_2d;
        typedef model::box<point_2d>        box_2d;
    };
}
}


