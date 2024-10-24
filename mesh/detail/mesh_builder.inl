
namespace FEM2D
{

namespace mesh
{

template<
    typename IndexType,
    typename ValueType
> 
typename Mesh_builder<IndexType, ValueType>::mesh_type_pointer
Mesh_builder<IndexType, ValueType>::build_mesh(bool form_vtk)
{
    this->parse();
    this->build_mesh();

    return save_mesh(form_vtk);
}
    
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
            [&](const std::pair<point_2d, bool> &point)
            {
                PolyFile << current_point_index++ << "\t"
                         << static_cast<value_type>(point.first.x()) << "\t"
                         << static_cast<value_type>(point.first.y()) << "\t"
                         << static_cast<index_type>(point.second)
                         << "\n";
            }
        );

        // FIXME: For rectangular area only
        index_type number_of_segments = m_points_field.boundary.outer().size();
        index_type current_segment_number = 0;

        PolyFile << "\n" << number_of_segments << " 1" << "\n";

        for(index_type i = 0; i < number_of_segments - 1; i++)
        {
            PolyFile << ++current_segment_number << "\t"
                     << i + 1 << "\t"
                     << i + 2 << "\t" 
                     << 1
                     << "\n"; 
        }

        // write last boundary segment
        PolyFile << ++current_segment_number << "\t"
                 << number_of_segments << "\t"
                 << 1 << "\t"
                 << 1 << "\t"
                 << "\n";

        // write holes list
        PolyFile << "\n" << 0;
        
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
        // programm will fall if we dont have 'e' key
        std::string triangle_switches("-pe");

        std::stringstream poly_file;

        this->create_poly_file(poly_file);

        std::ofstream ofs("test_polyfile.poly");
			ofs << poly_file.str();
			ofs.close();

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
> 
typename Mesh_builder<IndexType, ValueType>::mesh_type_pointer
Mesh_builder<IndexType, ValueType>::save_mesh(bool form_vtk)
{
    try
    {
        if(m_base_mesh.parse(m_triangle_mesh))
        {
            m_base_mesh.calculate_mesh_params();
        }
        else
        {
            throw std::runtime_error("Mesh data's reading error");
        }

        if(form_vtk)
        {
            this->write_mesh_to_vtk();
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Mesh_builder::save_mesh:: " + std::string(e.what()));
    }
    

    return std::make_unique<mesh_type>(m_base_mesh);
}


template<
    typename IndexType,
    typename ValueType
> bool Mesh_builder<IndexType, ValueType>::write_mesh_to_vtk()
{
    try
    {
        // take this data from m_mesh_base
        std::vector<point_2d> mesh_points = m_triangle_mesh->get_points_list(m_triangle_mesh->in_);

        std::vector<std::vector<index_type>> mesh_segments = m_triangle_mesh->get_segments_list(m_triangle_mesh->in_);

        std::vector<std::vector<index_type>> mesh_triangles = m_triangle_mesh->get_triangle_list(m_triangle_mesh->in_); 


        std::ofstream file;

        file.open("mesh.vtk");

        if(file.is_open())
        {
            size_t points_count = mesh_points.size();
            size_t elems_count  = mesh_triangles.size();

            file << "# vtk DataFile Version 2.0" << std::endl;
            file << "Semi sphere mesh" << std::endl;
            file << "ASCII" << std::endl;
            file << "DATASET UNSTRUCTURED_GRID" << std::endl;
            file << "POINTS " << points_count << " float" << std::endl; 

            for (size_t i = 0; i < points_count; i++)
            {
                file << float(mesh_points[i].x()) << "\t" << float(mesh_points[i].y()) << "\t" << 0.0 << std::endl;
            }

            file << "CELLS " << points_count + elems_count << "\t" << 2 * points_count + 4 * elems_count << std::endl;

            for(size_t i = 0; i < points_count; i++)
            {
                file << 1 << "\t" << i << std::endl;
            }

            for(size_t i = 0; i < elems_count; i++)
            {
                file << 3 << "\t";
                
                for(auto idx : mesh_triangles[i])
                {
                    file << idx - 1<< '\t'; 
                }

                file << std::endl;
            }

            file << "CELL_TYPES " << points_count + elems_count << std::endl;

            for(size_t i = 0; i < points_count; i++)
            {
                file << 2 << std::endl;
            }

            for(size_t i = 0; i < elems_count; i++)
            {
                file << 5 << std::endl;
            }

            file.close();
        }
        else
        {
            throw std::runtime_error("File can not to be opened");
        }


        return true;
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(
                "mesh_builder::write_mesh_to_vtk:: " + 
                std::string(e.what()));
    }

    return false;
}


} // 
} //