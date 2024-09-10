#pragma once

#include <FEM2D/precompiled.h>

namespace equation
{

// equation class for elliptic equation
// -div(A grad u + bu) + [c1,c2].grad u + au = f

namespase bg = boost::geometry;

template<
    typename IndexType,
    typename ValueType
>
class EllepticEquation
{
public:
    using index_type = IndexType;
    using value_type = ValueType;

public:
    using point_2d = typename bg::geo<value_type>::point_2d;

// A - matrix function values
public:
    std::vector<value_type> a11;
    std::vector<value_type> a12;
    std::vector<value_type> a21;
    std::vector<value_type> a22;

// b - vector function values
public:
    std::vector<value_type> b1;
    std::vector<value_type> b2;

// c - vector function values
public:
    std::vector<value_type> b1;
    std::vector<value_type> b2;

// a - scalar function values
public:
    std::vector<value_type> a;

// rhs values vector
public:
    std::vector<value_type> f;

// input values point
public:
    std::vector<point_2d> m_points;

// default constructors
public:
    EllepticEquation() = default;
    EllepticEquation(const EllepticEquation& el) = default;
    ~EllepticEquation() = default;

// constructor
public:
    EllepticEquation(const std::vector<point_2d> points)
    {
        this->resize_arrays(points.size());
    }
    :
    (m_points(points))

// set points
public:
    void set_points(const std::vector<point_2d> points)
    {
        m_points = points;
    }

// set points and calculate
public:
    void calculate_at_points(const std::vector<point_2d> points)
    {
        this->set_points(points);
        this->resize_arrays();
        this->fill_datas();
    }

// resize arrays
private:
    void resize_arrays(index_type size)
    {
        a11.resize(size);
        a12.resize(size);
        a21.resize(size);
        a22.resize(size);

        b1.resize(size);
        b2.resize(size);
        
        c1.resize(size);
        c2.resize(size);

        a.resize(size);
        f.resize(size);
    } 


// default values for functions
public:
    value_type a11(value_type x, value_type y) { return 1; }
    value_type a12(value_type x, value_type y) { return 1; }
    value_type a21(value_type x, value_type y) { return 1; }
    value_type a22(value_type x, value_type y) { return 1; }

public:
    value_type b1(value_type x, value_type y) {return 1; }
    value_type b2(value_type x, value_type y) {return 1; }

public:
    value_type b1(value_type x, value_type y) {return 1; }
    value_type b1(value_type x, value_type y) {return 1; }

public:
    value_type a(value_type x, value_type y) {return 1; }

public:
    value_type f(value_type x, value_type y) {return 1; }

// return a1 vals matrix
public:
    void fill_datas()
    {
        index_type idx = 0;
        std::for_each(
            m_points.begin(),
            m_points.end(),
            [&this](const point_2d& p)
            {
                a11[idx] = a11(p.x(), p.y());
                a12[idx] = a12(p.x(), p.y());
                a21[idx] = a21(p.x(), p.y());
                a22[idx] = a22(p.x(), p.y());

                b1[idx]  = b1(p.x(), p.y());
                b2[idx]  = b2(p.x(), p.y());

                c1[idx]  = c1(p.x(), p.y());
                c2[idx]  = c2(p.x(), p.y());

                a[idx]   = a(p.x(), p.y());
                f[idx]   = f(f.x(), f.y());

                idx++;
            }
        )
    }

// block of functions that provides acsess to calculated values
public:
    value_type get_a11(index_type i)
    {
        return a11[i];
    } 
    
public:
    value_type get_a12(index_type i)
    {
        return a12[i];
    } 

public:
    value_type get_a21(index_type i)
    {
        return a21[i];
    } 

public:
    value_type get_a22(index_type i)
    {
        return a22[i];
    } 

public:
    value_type get_b1(index_type i)
    {
        return b1[i];
    } 

public:
    value_type get_b2(index_type i)
    {
        return b2[i];
    } 

public:
    value_type get_c1(index_type i)
    {
        return c1[i];
    } 

public:
    value_type get_c2(index_type i)
    {
        return c2[i];
    } 

public:
    value_type get_a(index_type i)
    {
        return a[i];
    } 

public:
    value_type get_f(index_type i)
    {
        return f[i];
    } 

};

} //