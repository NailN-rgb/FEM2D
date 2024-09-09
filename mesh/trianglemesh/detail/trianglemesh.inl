#include <polymesh/mesh/boost_geometry/typedefs.h>


namespace polymesh
{

namespace mesh
{

namespace trianglemesh
{

namespace detail
{
	template<typename json_type>
	json_type write_nodes(const triangulateio& tio)
	{
		json_type json;

		//vertices coordinates
		std::size_t i = 0;

		for (
			auto it  = tio.pointlist; 
				 it != tio.pointlist + 2 * tio.numberofpoints; 
				 ++it
		)
		{
			if (i++ % 2 == 0)
			{
				json["x"].push_back(*it);
			}
			else
			{
				json["y"].push_back(*it);
			}
		}
		
		//vertices attributes
		if (tio.numberofpointattributes != 0)
		{
			std::copy(
				tio.pointattributelist,
				tio.pointattributelist + tio.numberofpoints,
				std::back_inserter(json["pattrib"])
			);
		}


		//vertices markers
		if (tio.pointmarkerlist != 0)
		{
			std::copy(
				tio.pointmarkerlist,
				tio.pointmarkerlist + tio.numberofpoints,
				std::back_inserter(json["pmarker"])
			);
		}

		return {{"nodes", json}};
	};

	template<typename json_type>
	json_type write_edges(const triangulateio& tio)
	{
		json_type json;

		std::copy(
			tio.edgelist,
			tio.edgelist + 2 * tio.numberofedges,
			std::back_inserter(json["enid"])
		);

		if (tio.edgemarkerlist != 0)
		{
			std::copy(
				tio.edgemarkerlist,
				tio.edgemarkerlist + tio.numberofedges,
				std::back_inserter(json["emarker"])
			);
		}

		return {{"edges", json}};
	};

	template<typename json_type>
	json_type write_elems(const triangulateio& tio)
	{
		json_type json;

		std::copy(
			tio.trianglelist,
			tio.trianglelist + tio.numberofcorners * tio.numberoftriangles,
			std::back_inserter(json["elnid"])
		);

		if (tio.numberoftriangleattributes != 0)
		{
			std::copy(
				tio.triangleattributelist,
				tio.triangleattributelist + tio.numberoftriangleattributes * tio.numberoftriangles,
				std::back_inserter(json["eattrib"])
			);
		}

		return {{"elems", json}};
	};

	template<typename json_type>
	json_type write_norms(const triangulateio& tio)
	{
		json_type json;

		std::copy(
			tio.normlist,
			tio.normlist + 2 * tio.numberofedges,
			std::back_inserter(json["norms"])
		);

		return json;
	};

	template<
		typename json_type,
		typename polygon_type
	>
	json_type write_cells(
		const triangulateio& tio,
		const polygon_type& boundary
	)
	{
		using point_t       = typename bg::point_type<polygon_type>::type;
		using segment_t     = typename bg::model::segment<point_t>;
		
		json_type json;

		json["nodes"] = write_nodes<json_type>(tio).at("nodes");
		json["edges"] = write_edges<json_type>(tio).at("edges");
		json["norms"] = write_norms<json_type>(tio).at("norms");

#ifdef _DEBUG
		std::cout << "nodes size: " << json.at("nodes").at("x").size()    << std::endl;
		std::cout << "edges size: " << json.at("edges").at("enid").size() << std::endl;
		std::cout << "norms size: " << json.at("norms").size()            << std::endl;
#endif		

		//remove edges with vertex indices < 0
		//and make an intersection of infinite edges with boundary

		std::vector<std::size_t> jj;

		jj.reserve(json["edges"].size());

		std::size_t N_everts = json.at("nodes").at("x").size() + 1;

		//make vector of boundary segments
		std::vector<segment_t> segments;

		std::for_each( 
			boost::make_zip_iterator(
				boost::make_tuple(
					boundary.outer().begin(),
					boundary.outer().begin() + 1
				)
			),
			boost::make_zip_iterator(
				boost::make_tuple(
					boundary.outer().end() - 1,
					boundary.outer().end()
				)
			),
			[&segments](boost::tuple<point_t, point_t> tp)
			{
				segments.push_back(
					segment_t(
						boost::get<0>(tp),
						boost::get<1>(tp)
					)
				);
			}
		);

		for (auto it  = json.at("edges").at("enid").begin();
				  it != json.at("edges").at("enid").end();
			      it += 2
		)
		{
			if (*(it + 1) != -1) 
			{
				jj.push_back(*(it + 0));
				jj.push_back(*(it + 1));
			}
/*			else 
			{
				//get norm
				auto norm =
					(json.at("norms").begin() +
						std::distance(
							json.at("edges").at("enid").begin(),
							it
						)
					);

				//find intersection with boundary
				auto torg = bg::make<point_t>(
					(json.at("nodes").at("x").begin() + it->template get<std::size_t>() - 1).value(),
					(json.at("nodes").at("y").begin() + it->template get<std::size_t>() - 1).value()
				);

				auto tdest = bg::make<point_t>(
					torg.x() + 10. * norm->template get<value_type>(),
					torg.y() + 10. * std::next(norm)->template get<value_type>()
				);

				auto segment = segment_t(torg, tdest);

				//find shortest distance to boundary segment
				//from the first segment point
				
				auto it = std::min_element(
					segments.begin(),
					segments.end(),
					[&segment](auto s1, auto s2)
					{
						return bg::distance(s1,segment) < 	
							   bg::distance(s2,segment) < 	

					}
				);
				
				bg::for_each_segment(
					boundary,
					[&](const ref_segment_t& s)
					{
						if (bg::intersects(segment, s))
						{
							std::vector<point_t> pin;

							bg::intersection(s, segment, pin);

							json["nodes"]["x"].push_back(pin[0].x());
							json["nodes"]["y"].push_back(pin[0].y());

							jj.push_back(*(it + 0));
							jj.push_back(N_everts++);
						}
					}
				);
			}
*/		}

		/*if (json.at("edges").at("enid").size() != jj.size())
		{
			throw std::runtime_error("number of edges is not equal to initial number of edges");
		}*/

		json["edges"]["enid"] = std::move(jj);

		std::cout << "nodes size: " << json.at("nodes").at("x").size() << std::endl;
		std::cout << "edges size: " << json.at("edges").at("enid").size() << std::endl;

		return {{"cells", json}};

	};

}//end namespace detail


template<typename json_type>
json_type TriangleMesh::write_nodes_()
{
	return detail::write_nodes<json_type>(in_);
}


template<typename json_type>
json_type TriangleMesh::write_edges_()
{
	return detail::write_edges<json_type>(in_);
}


template<typename json_type>
json_type TriangleMesh::write_elems_()
{
	return detail::write_elems<json_type>(in_);
}


template<
	typename json_type,
	typename polygon_type
>
json_type TriangleMesh::write_cells_(const polygon_type& boundary)
{
	return detail::write_cells<json_type>(
					vorout_,
					boundary
				);
}


template<typename json_type>
bool TriangleMesh::write_nodes(json_type& json)
{
	//vertices coordinates
	std::size_t i = 0; 

	for (
		auto it  = in_.pointlist; 
		     it != in_.pointlist + 2 * in_.numberofpoints; 
			 ++it
	)
	{
		if (i++ % 2 == 0)
		{
			json["x"].push_back(*it);
		}
		else
		{
			json["y"].push_back(*it);
		}
	}

    //vertices attributes
	if (in_.numberofpointattributes != 0)
    {   
		std::copy(
			in_.pointattributelist, 
			in_.pointattributelist + in_.numberofpoints,
			std::back_inserter(json["pattrib"])
		);
    }
	

    //vertices markers
	if (in_.pointmarkerlist != 0)
    {   
		std::copy(
			in_.pointmarkerlist, 
			in_.pointmarkerlist + in_.numberofpoints, 
			std::back_inserter(json["pmarker"])
		);
	}

	return true;
};

template<typename json_type>
bool TriangleMesh::write_edges(json_type& json)
{
	std::copy(
		in_.edgelist, 
		in_.edgelist + 2 * in_.numberofedges, 
		std::back_inserter(json["enid"])
	);

	if (in_.edgemarkerlist != 0)
	{
		std::copy(
			in_.edgemarkerlist, 
			in_.edgemarkerlist + in_.numberofedges, 
			std::back_inserter(json["emarker"])
		);
	}

	return true;
}


template<typename json_type>
bool TriangleMesh::write_elems(json_type& json)
{
	std::copy(
		in_.trianglelist,
		in_.trianglelist + in_.numberofcorners * in_.numberoftriangles,
		std::back_inserter(json["elnid"])
	);

	if (in_.numberoftriangleattributes != 0)
	{
		std::copy(
			in_.triangleattributelist,
			in_.triangleattributelist + in_.numberoftriangleattributes * in_.numberoftriangles,
			std::back_inserter(json["eattrib"])
		);
	}
	
	return true;
}


template<typename json_type>
bool TriangleMesh::write_cells(json_type& json)
{
	std::copy(
		in_.trianglelist,
		in_.trianglelist + in_.numberofcorners * in_.numberoftriangles,
		std::back_inserter(json["elnid"])
	);

	if (in_.numberoftriangleattributes != 0)
	{
		std::copy(
			in_.triangleattributelist,
			in_.triangleattributelist + in_.numberoftriangleattributes * in_.numberoftriangles,
			std::back_inserter(json["eattrib"])
		);
	}

	return true;
}

} //namespace trianglemesh

} //namespace mesh

} //namespace polymesh
