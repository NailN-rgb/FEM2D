#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <boost/geometry/multi/geometries/multi_linestring.hpp>
#include <boost/geometry/algorithms/unique.hpp>

namespace boost::geometry
{

template <typename ValueType>
struct geo
{
    using value_type = ValueType;

    using point_2d   = model::d2::point_xy<value_type, boost::geometry::cs::cartesian>;
    using line_2d    = model::linestring<point_2d>;
    using polygon_2d = model::polygon<point_2d>;
    using segment_2d = model::segment<point_2d>;
    using box_2d     = model::box<point_2d>;

    struct equal_points
    {
        bool operator()(const point_2d& p1, const point_2d& p2) const
        {
            return ((fabs(p1.x() - p2.x()) < 1.e-6) && (fabs(p1.y() - p2.y()) < 1.e-6));
        }

        bool is_equal(const point_2d& p1, const point_2d& p2) const
        {
            return ((fabs(p1.x() - p2.x()) < 1.e-6) && (fabs(p1.y() - p2.y()) < 1.e-6));
        }
    };
};
}
