#pragma once
#include <vector>
#include <directxmath.h>
#include "basic_utility.h"

template<typename V>
class basic_geometry
{
public:
	static basic_geometry make_tex_mapped_cube(float width)
	{
		basic_geometry mtm;
		float p = width / 2.0f;
		mtm.vertex_info.resize(14);
		mtm.vertex_info[0].pos = { -p,  p, 0.0f };
		mtm.vertex_info[1].pos = { p,  p, 0.0f };
		mtm.vertex_info[2].pos = { -p, -p, 0.0f };
		mtm.vertex_info[3].pos = { p, -p, 0.0f };
		mtm.vertex_info[4].pos = { -p,  p, width };
		mtm.vertex_info[5].pos = { -p, -p, width };
		mtm.vertex_info[6].pos = { -p,  p, width };
		mtm.vertex_info[7].pos = { p,  p, width };
		mtm.vertex_info[8].pos = { p,  p, width };
		mtm.vertex_info[9].pos = { p, -p, width};
		mtm.vertex_info[10].pos = { -p, -p, width };
		mtm.vertex_info[11].pos = { p, -p, width };
		mtm.vertex_info[12].pos = { -p,  p, width };
		mtm.vertex_info[13].pos = { p,  p, width};

		mtm.vertex_info[0].tex = { 1.0f / 3.0f, 0.25f };
		mtm.vertex_info[1].tex = { 2.0f / 3.0f, 0.25f };
		mtm.vertex_info[2].tex = { 1.0f / 3.0f, 0.5f };
		mtm.vertex_info[3].tex = { 2.0f / 3.0f, 0.5f };
		mtm.vertex_info[4].tex = { 0.0f, 0.25f };
		mtm.vertex_info[5].tex = { 0.0f, 0.5f };
		mtm.vertex_info[6].tex = { 1.0f / 3.0f, 0.0f };
		mtm.vertex_info[7].tex = { 2.0f / 3.0f, 0.0f };
		mtm.vertex_info[8].tex = { 1.0f, 0.25f };
		mtm.vertex_info[9].tex = { 1.0f, 0.5f };
		mtm.vertex_info[10].tex = { 1.0f / 3.0f, 0.75f };
		mtm.vertex_info[11].tex = { 2.0f / 3.0f, 0.75f };
		mtm.vertex_info[12].tex = { 1.0f / 3.0f, 1.0f };
		mtm.vertex_info[13].tex = { 2.0f / 3.0f, 1.0f };

		mtm.indicies =
		{
			2, 0, 1,
			2, 1, 3,
			2, 5, 4,
			2, 4, 0,
			0, 7, 1,
			0, 6, 7,
			3, 1, 8,
			3, 8, 9,
			2, 11, 10,
			2, 3, 11,
			13, 12, 10,
			13, 10, 11,
		};
		return std::move(mtm);
	}
	static basic_geometry make_individual_cube(float width)
	{
		basic_geometry mtm;
		float p = width / 2.0f;
		mtm.vertex_info.resize(24);
		mtm.vertex_info[0].pos = { -p, -p, -p };
		mtm.vertex_info[1].pos = { p, -p,  -p };
		mtm.vertex_info[2].pos = { -p,  p, -p };
		mtm.vertex_info[3].pos = { p,  p,  -p };
			
		mtm.vertex_info[4].pos = { -p, -p, p };
		mtm.vertex_info[5].pos = { p, -p,  p };
		mtm.vertex_info[6].pos = { -p,  p, p };
		mtm.vertex_info[7].pos = { p,  p,  p };
			
		mtm.vertex_info[8].pos = { -p, -p, p };
		mtm.vertex_info[9].pos = { -p, -p, -p };
		mtm.vertex_info[10].pos = { -p,  p, p };
		mtm.vertex_info[11].pos = { -p,  p, -p };
			
		mtm.vertex_info[12].pos = { p, -p, p };
		mtm.vertex_info[13].pos = { p, -p, -p };
		mtm.vertex_info[14].pos = { p,  p, p };
		mtm.vertex_info[15].pos = { p,  p, -p };

		mtm.vertex_info[16].pos = { -p,  p, -p };
		mtm.vertex_info[17].pos = { p,  p, -p };
		mtm.vertex_info[18].pos = { -p,  p, p };
		mtm.vertex_info[19].pos = { p,  p, p };

		mtm.vertex_info[20].pos = { -p, -p, -p };
		mtm.vertex_info[21].pos = { p, -p, -p };
		mtm.vertex_info[22].pos = { -p, -p, p };
		mtm.vertex_info[23].pos = { p, -p, p };
		mtm.indicies =
		{
			0,2,1,
			3,1,2,
			5,6,4,
			6,5,7,
			9,8,11,
			11,8,10,
			13,15,12,
			14,12,15,
			16,18,17,
			19,17,18,
			22,20,21,
			21,23,22
		};
		return std::move(mtm);
	}
	static basic_geometry make_indexed_cube(float width)
	{
		basic_geometry mtm;
		float p = width / 2.0f;
		mtm.vertex_info.resize(8);
		vertex_info[0].pos = { -p, -p, 0.0f };
		vertex_info[1].pos = { p, -p, 0.0f };
		vertex_info[2].pos = { -p,  p, 0.0f };
		vertex_info[3].pos = { p,  p, 0.0f };
		vertex_info[4].pos = { -p, -p, width };
		vertex_info[5].pos = { p, -p, width };
		vertex_info[6].pos = { -p,  p, width };
		vertex_info[7].pos = { p,  p, width};
		
		mtm.indicies =
		{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4,
		};
		return std::move(mtm);
	}
	static basic_geometry make_sphere(float radius, unsigned int n_lat, unsigned int n_long)
	{
		basic_geometry mtm;
		const DirectX::XMVECTOR base = DirectX::XMVectorSet( 0.0f, 0.0f, radius , 0.0f);
		for (int lat_c = 1; lat_c < n_lat; lat_c++)
		{
			const DirectX::XMVECTOR lat_base = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationX(PI/n_lat * lat_c));
			for (int long_c = 0; long_c < n_long; long_c++)
			{
				auto v = DirectX::XMVector3Transform(lat_base, DirectX::XMMatrixRotationZ((2 * PI/n_long) * long_c));
				mtm.vertex_info.emplace_back();
				DirectX::XMStoreFloat3(&mtm.vertex_info.back().pos, v);
			}
		}
		const auto i_north = (unsigned short)mtm.vertex_info.size();
		mtm.vertex_info.emplace_back();
		DirectX::XMStoreFloat3(&mtm.vertex_info.back().pos, base);
		const auto i_south = (unsigned short)mtm.vertex_info.size();
		mtm.vertex_info.emplace_back();
		DirectX::XMStoreFloat3(&mtm.vertex_info.back().pos, DirectX::XMVectorNegate(base));
		const auto calc_index = [n_lat, n_long](unsigned short lat_c, unsigned short long_c)
		{ return lat_c * n_long + long_c; };
		for (unsigned short lat_c = 0; lat_c < n_lat - 2; lat_c++)
		{
			for (unsigned short long_c = 0; long_c < n_long - 1; long_c++)
			{
				mtm.indicies.push_back(calc_index(lat_c, long_c));
				mtm.indicies.push_back(calc_index(lat_c + 1, long_c));
				mtm.indicies.push_back(calc_index(lat_c, long_c + 1));
				mtm.indicies.push_back(calc_index(lat_c, long_c + 1));
				mtm.indicies.push_back(calc_index(lat_c + 1, long_c));
				mtm.indicies.push_back(calc_index(lat_c + 1, long_c + 1));
			}
			// wrap band
			mtm.indicies.push_back(calc_index(lat_c, n_long - 1));
			mtm.indicies.push_back(calc_index(lat_c + 1, n_long - 1));
			mtm.indicies.push_back(calc_index(lat_c, 0));
			mtm.indicies.push_back(calc_index(lat_c, 0));
			mtm.indicies.push_back(calc_index(lat_c + 1, n_long - 1));
			mtm.indicies.push_back(calc_index(lat_c + 1, 0));
		}
		for (int c_long = 0; c_long < n_long - 1; c_long++)
		{
			mtm.indicies.push_back(i_north);
			mtm.indicies.push_back(c_long);
			mtm.indicies.push_back(c_long + 1);

			mtm.indicies.push_back(calc_index(n_lat - 2, c_long + 1));
			mtm.indicies.push_back(calc_index(n_lat - 2, c_long));
			mtm.indicies.push_back(i_south);
		}
		mtm.indicies.push_back(i_north);
		mtm.indicies.push_back(calc_index(0, n_long - 1));
		mtm.indicies.push_back(calc_index(0, 0));
		// south
		mtm.indicies.push_back(calc_index(n_lat - 2, 0));
		mtm.indicies.push_back(calc_index(n_lat - 2, n_long - 1));
		mtm.indicies.push_back(i_south);
		return mtm;
	}
	static basic_geometry make_sphere_normals(float radius, unsigned int n_lat, unsigned int n_long)
	{
		basic_geometry mtm;
		const DirectX::XMVECTOR base = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
		for (int lat_c = 1; lat_c < n_lat; lat_c++)
		{
			const DirectX::XMVECTOR lat_base = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationX(PI / n_lat * lat_c));
			for (int long_c = 0; long_c < n_long; long_c++)
			{
				auto v = DirectX::XMVector3Transform(lat_base, DirectX::XMMatrixRotationZ((2 * PI / n_long) * long_c));
				mtm.vertex_info.emplace_back();
				DirectX::XMStoreFloat3(&mtm.vertex_info.back().pos, v);
			}
		}
		const auto i_north = (unsigned short)mtm.vertex_info.size();
		mtm.vertex_info.emplace_back();
		DirectX::XMStoreFloat3(&mtm.vertex_info.back().pos, base);
		const auto i_south = (unsigned short)mtm.vertex_info.size();
		mtm.vertex_info.emplace_back();
		DirectX::XMStoreFloat3(&mtm.vertex_info.back().pos, DirectX::XMVectorNegate(base));
		for (auto& e : mtm.vertex_info)
		{
			DirectX::XMStoreFloat3(&e.normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&e.pos)));
		}
		const auto calc_index = [n_lat, n_long](unsigned short lat_c, unsigned short long_c)
		{ return lat_c * n_long + long_c; };
		for (unsigned short lat_c = 0; lat_c < n_lat - 2; lat_c++)
		{
			for (unsigned short long_c = 0; long_c < n_long - 1; long_c++)
			{
				mtm.indicies.push_back(calc_index(lat_c, long_c));
				mtm.indicies.push_back(calc_index(lat_c + 1, long_c));
				mtm.indicies.push_back(calc_index(lat_c, long_c + 1));
				mtm.indicies.push_back(calc_index(lat_c, long_c + 1));
				mtm.indicies.push_back(calc_index(lat_c + 1, long_c));
				mtm.indicies.push_back(calc_index(lat_c + 1, long_c + 1));
			}
			// wrap band
			mtm.indicies.push_back(calc_index(lat_c, n_long - 1));
			mtm.indicies.push_back(calc_index(lat_c + 1, n_long - 1));
			mtm.indicies.push_back(calc_index(lat_c, 0));
			mtm.indicies.push_back(calc_index(lat_c, 0));
			mtm.indicies.push_back(calc_index(lat_c + 1, n_long - 1));
			mtm.indicies.push_back(calc_index(lat_c + 1, 0));
		}
		for (int c_long = 0; c_long < n_long - 1; c_long++)
		{
			mtm.indicies.push_back(i_north);
			mtm.indicies.push_back(c_long);
			mtm.indicies.push_back(c_long + 1);

			mtm.indicies.push_back(calc_index(n_lat - 2, c_long + 1));
			mtm.indicies.push_back(calc_index(n_lat - 2, c_long));
			mtm.indicies.push_back(i_south);
		}
		mtm.indicies.push_back(i_north);
		mtm.indicies.push_back(calc_index(0, n_long - 1));
		mtm.indicies.push_back(calc_index(0, 0));
		// south
		mtm.indicies.push_back(calc_index(n_lat - 2, 0));
		mtm.indicies.push_back(calc_index(n_lat - 2, n_long - 1));
		mtm.indicies.push_back(i_south);
		return mtm;
	}
	const std::vector<V>& get_vertices() const noexcept
	{
		return vertex_info;
	}
	const std::vector<unsigned short>& get_indicies() const noexcept
	{
		return indicies;
	}
	std::vector<V>& get_vertex_info()
	{
		return vertex_info;
	}
	std::vector<unsigned short>& get_index_info()
	{
		return indicies;
	}
private:
	std::vector<V> vertex_info;
	std::vector<unsigned short> indicies;
};



