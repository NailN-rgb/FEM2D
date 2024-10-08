#include <FEM2D/precompiled.h>
#include <FEM2D/mesh/boost_datatypes/types.h>

namespace FEM2D
{

namespace points_field
{

namespace bg = boost::geometry;

template<
    typename IndexType,
    typename ValueType
>
class Points_field
{
public:
    using value_type = ValueType;
    using index_type = IndexType;

public:
    using point_2d     = typename bg::geo<value_type>::point_2d;
	using segment_type = typename bg::geo<value_type>::segment_2d;
	using polygon_2d   = typename bg::geo<value_type>::polygon_2d;
    using box_2d       = typename bg::geo<value_type>::box_2d;
    using line_2d      = typename bg::geo<value_type>::line_2d;

    using equal_points = typename bg::geo<value_type>::equal_points;

public:
    using edge_t = std::tuple<index_type, index_type>;

public:
    using vector_of_indexes = std::vector<index_type>;
    using vector_of_values  = std::vector<value_type>;

    using vector_of_edges   = std::vector<edge_t>;
    using vector_of_points  = std::vector<std::pair<point_2d, bool>>;


public:
    polygon_2d boundary;
    vector_of_edges edge_vector;
    vector_of_points points;
    std::vector<bool> is_boundary_point;
public:
    value_type hx;
    value_type hy;


public:
    Points_field() = default;
    ~Points_field() = default;
    Points_field(const Points_field& pf) = default;

public:
    bool set_default_params();

public:
    bool create_points_cloud();

public:
    box_2d get_convex_hull()
    {
        using boost::geometry::return_envelope;

        return return_envelope<
                        box_2d,
                        polygon_2d
                    >(boundary);
    };

public:
    void create_convex_hull_points(
        const vector_of_values &x,
        const vector_of_values &y);

public:
    bool erase_non_polygon_points();

public:
    bool get_points_at_boundary();

};

} //

} //

#include <FEM2D/mesh/points_field/detail/points_field.inl>