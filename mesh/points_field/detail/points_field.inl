namespace FEM2D
{

namespace points_field
{

template<
    typename IndexType,
    typename ValueType
> bool Points_field<IndexType, ValueType>::set_default_params()
{
    try
    {
        // fill boundary
        bg::append(boundary.outer(), point_2d(0., 0.));
        bg::append(boundary.outer(), point_2d(1., 0.));
        bg::append(boundary.outer(), point_2d(1., 1.));
        bg::append(boundary.outer(), point_2d(0., 1.));
        bg::append(boundary.outer(), point_2d(0., 0.));

        hx = 0.3;
        hy = 0.3;

        return true;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("points_field::set_default_params:: " + std::string(e.what()));
    }
}


template<
    typename IndexType,
    typename ValueType
> bool Points_field<IndexType, ValueType>::create_points_cloud()
{
    box_2d convex_hull = get_convex_hull();

    point_2d left_low = convex_hull.min_corner();
    point_2d right_up = convex_hull.max_corner();

    auto nx = static_cast<index_type>((right_up.x() - left_low.x()) / hx + 0.5) + 1;
    auto ny = static_cast<index_type>((right_up.y() - left_low.y()) / hy + 0.5) + 1;

    vector_of_values x(nx - 1);
    vector_of_values y(ny - 1);

    x[0] = left_low.x() + hx;

    for(index_type i = 1; i < nx - 1; i++)
    {
        x[i] = x[i - 1] + hx;
    }

    y[0] = left_low.y() + hy;

    for(index_type i = 1; i < ny - 1; i++)
    {
        y[i] = y[i - 1] + hy;
    }
    
    // create points at boundary
    get_points_at_boundary();

    // generate points inside convex hull
    create_convex_hull_points(x, y);

    // remove points outside polygon
    // erase_non_polygon_points();

    return true;
}


template<
    typename IndexType,
    typename ValueType
> void Points_field<IndexType, ValueType>::create_convex_hull_points(
    const vector_of_values &x,
    const vector_of_values &y
)
{
    points.insert(points.begin(), boundary.outer().begin(), boundary.outer().end());

    for(size_t i = 0; i < x.size(); i++)
    {
        for(size_t j = 0; j < y.size(); j++)
        {
            points.push_back(
                point_2d(
                    x[i],
                    y[j]
                )
            );
        }
    }
}


template<
    typename IndexType,
    typename ValueType
> bool Points_field<IndexType, ValueType>::erase_non_polygon_points()
{
    //remove points out of boundary
    line_2d line(boundary.outer().begin(), boundary.outer().end());
    auto dist = 0.2;

    points.erase(
        std::remove_if(
            points.begin(),
            points.end(),
            [&](point_2d& p)
            {
                return (
                        !boost::geometry::within(p, boundary) || 
                            boost::geometry::distance(p, line) < dist
                        );
            }
        ),
        points.end()
    );

    return true;
}


template<
    typename IndexType,
    typename ValueType
> bool Points_field<IndexType, ValueType>::get_points_at_boundary()
{
    std::vector<point_2d> polygon_points = boundary.outer();

    auto step = (hx + hy) / 2 * std::sqrt(2);

    boundary.clear();

    // loop by boundary segments
    for(index_type i = 0; i < polygon_points.size() - 1; i++)
    {
        point_2d first_pt = polygon_points[i];
        point_2d second_pt = polygon_points[i + 1];

        value_type distance = std::sqrt(
            std::pow(second_pt.x() - first_pt.x(), 2) +
            std::pow(second_pt.y() - first_pt.y(), 2)
        );

        index_type num_intermediate_points = static_cast<int>(std::floor(distance / step));

        bg::append(boundary.outer(), polygon_points[i]);

        for(index_type j = 1; j < num_intermediate_points - 1; j++)
        {
            value_type ratio = static_cast<value_type>(j) / (num_intermediate_points + 1);
            value_type x = static_cast<value_type>(first_pt.x() + ratio * (second_pt.x() - first_pt.x()));
            value_type y = static_cast<value_type>(first_pt.y() + ratio * (second_pt.y() - first_pt.y()));

            bg::append(boundary.outer(), point_2d(x, y));
        }
    }
}

} //

} //