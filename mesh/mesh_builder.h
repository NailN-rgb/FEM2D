#pragma once

#include <FEM2D/precompiled.h>
#include <FEM2D/mesh/boost_datatypes/types.h>
#include <FEM2D/mesh/points_field/points_field.h>
#include <FEM2D/mesh/trianglemesh/trianglemesh.h>

namespace FEM2D
{

namespace mesh
{

namespace bg = boost::geometry;

template<
    typename IndexType,
    typename ValueType
>
class Mesh_builder
{
// types
public:
    using index_type = IndexType;
    using value_type = ValueType;

public:
    using point_2d     = typename bg::geo<value_type>::point_2d;
	using segment_type = typename bg::geo<value_type>::segment_2d;
	using polygon_2d   = typename bg::geo<value_type>::polygon_2d;

public:
    using points_field_type = typename FEM2D::points_field::Points_field<IndexType, ValueType>;
    using triangle_mesh_t   = FEM2D::mesh::trianglemesh::TriangleMesh;

    using triangle_mesh_pointer = std::unique_ptr<triangle_mesh_t>; 

// variables
public:
    points_field_type m_points_field;

public:
    triangle_mesh_pointer m_triangle_mesh; 

// methods
public:
    Mesh_builder() = default;
    Mesh_builder(const Mesh_builder& mesh_build) = default;
    ~Mesh_builder() = default;

public:
    void parse();

public:
    bool save_mesh();

public:
    bool write_mesh_to_vtk();

public:
    bool create_poly_file(std::stringstream& PolyFile);

public:
    bool build_mesh();
    bool build_voronoi_mesh();

};

} //

} //

#include <FEM2D/mesh/detail/mesh_builder.inl>