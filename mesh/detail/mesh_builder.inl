
namespace FEM2D
{

namespace mesh
{
    
template<
    typename IndexType,
    typename ValueType
> void Mesh_builder<IndexType, ValueType>::parse()
{
    try
    {
        // read input file or set default mesh params

        m_points_field.set_default_params();

        m_points_field.create_points_cloud();

        // save in mesh builder class
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("mesh_builder::parse:: " + std::string(e.what()));
    }
}


template<
    typename IndexType,
    typename ValueType
> bool Mesh_builder<IndexType, ValueType>::create_poly_file(std::stringstream& PolyFile)
{
    try
    {
        if (!PolyFile.good())
		{
			throw std::invalid_argument("file was not found or could not be read");
		}

        index_type number_of_points      = m_points_field.points.size();
        index_type current_point_index   = 1;
        // index_type current_segment_index = 1;
        
        //set header of file
        PolyFile << number_of_points << "   2   0   1" << "\n";

        std::for_each(
            m_points_field.points.begin(),
            m_points_field.points.end(),
            [&](point_2d point)
            {
                PolyFile << current_point_index++
                         << point.x() << "\t"
                         << point.y() << "\t"
                         << "0\n";
            }
        );

        // index_type num_segments = 


        return true;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(
            "mesh_builder::create_poly_file:: " + 
            std::string(e.what())
        );
    }

    return false;
}



template<
    typename IndexType,
    typename ValueType
> bool Mesh_builder<IndexType, ValueType>::build_mesh()
{
    try
    {
        // turn off output
        std::string triangle_switches("QQq");

        std::stringstream poly_file;

        this->create_poly_file(poly_file);

        m_triangle_mesh.reset();
        m_triangle_mesh = std::make_unique<triangle_mesh_t>();

        if(!(m_triangle_mesh->build_mesh(triangle_switches, poly_file)))
        {
            throw std::runtime_error("triangle mesh coult not be build");
        }

        return true;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(
            "mesh_builder::build_mesh:: " + 
            std::string(e.what()) 
        );
    }

    return false;
}


template<
    typename IndexType,
    typename ValueType
> bool Mesh_builder<IndexType, ValueType>::save_mesh()
{
    if(true)
    {
        this->write_mesh_to_vtk();
    }

    return true;
}


template<
    typename IndexType,
    typename ValueType
> bool Mesh_builder<IndexType, ValueType>::write_mesh_to_vtk()
{
    // TODO: add code here

    return true;
}


} // 
} //