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

        hx = 0.1;
        hy = 0.1;

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

    vector_of_values x(nx);
    vector_of_values y(ny);

    x[0] = left_low.x();

    for(index_type i = 1; i < nx; i++)
    {
        x[i] = x[i - 1] + hx;
    }

    y[0] = left_low.y();

    for(index_type i = 1; i < ny; i++)
    {
        y[i] = y[i - 1] + hy;
    }

    // generate points inside convex hull
    create_convex_hull_points(x, y);

    // remove points outside polygon
    erase_non_polygon_points();


    return true;
}


template<
    typename IndexType,
    typename ValueType
> void Points_field<IndexType, ValueType>::create_convex_hull_points(
    vector_of_values x,
    vector_of_values y
)
{
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

} //

} //