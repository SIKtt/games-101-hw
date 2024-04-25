//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>
#include <stdexcept>

rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);
    std::cout << "pos num" << id << "\n";
    return {id};
}

rst::pos_buf_id rst::rasterizer::load_positions_cube(const std::vector<Eigen::Vector3f> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);
    std::cout << "pos num" << id << "\n";
    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
    auto id = get_next_id();
    std::cout << "ind num" << id << "\n";
    std::cout << (indices[0]);
    ind_buf.emplace(id, indices);
    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices_cube(const std::vector<Eigen::Matrix<int, 8, 1>> &indices)
{
    auto id = get_next_id();
    std::cout << "ind num" << id << "\n";
    std::cout << (indices[0]) << "\n";
    //ind_buf_cube.insert({id, indices});
    ind_buf_cube.emplace(id, indices);
    std::cout << "ind over" << "\n";
    return {id};
}

// Bresenham's line drawing algorithm
// Code taken from a stack overflow answer: https://stackoverflow.com/a/16405254
void rst::rasterizer::draw_line(Eigen::Vector3f begin, Eigen::Vector3f end)
{
    auto x1 = begin.x();
    auto y1 = begin.y();
    auto x2 = end.x();
    auto y2 = end.y();

    Eigen::Vector3f line_color = {255, 255, 255};

    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;

    dx=x2-x1;
    dy=y2-y1;
    dx1=fabs(dx);
    dy1=fabs(dy);
    px=2*dy1-dx1;
    py=2*dx1-dy1;

    if(dy1<=dx1)
    {
        if(dx>=0)
        {
            x=x1;
            y=y1;
            xe=x2;
        }
        else
        {
            x=x2;
            y=y2;
            xe=x1;
        }

        Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
        set_pixel(point,line_color);

        for(i=0;x<xe;i++)
        {
            x=x+1;
            if(px<0)
            {
                px=px+2*dy1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    y=y+1;
                }
                else
                {
                    y=y-1;
                }
                px=px+2*(dy1-dx1);
            }
//            delay(0);
            Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
            set_pixel(point,line_color);
        }
        
    }
    else
    {
        if(dy>=0)
        {
            x=x1;
            y=y1;
            ye=y2;
        }
        else
        {
            x=x2;
            y=y2;
            ye=y1;
        }
        Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
        set_pixel(point,line_color);
        for(i=0;y<ye;i++)
        {
            y=y+1;
            if(py<=0)
            {
                py=py+2*dx1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    x=x+1;
                }
                else
                {
                    x=x-1;
                }
                py=py+2*(dx1-dy1);
            }
//            delay(0);
            Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
            set_pixel(point,line_color);
        }
    }
}

auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}

void rst::rasterizer::draw(rst::pos_buf_id pos_buffer, rst::ind_buf_id ind_buffer, rst::Primitive type)
{

    if (type != rst::Primitive::Triangle 
        && type != rst::Primitive::Line 
        && type != rst::Primitive::Cube)
    {
        //throw std::runtime_error("Drawing primitives other than triangle is not implemented yet!");
        throw std::runtime_error("Drawing primitives more than default is not implemented yet!");
    }

    if (type == rst::Primitive::Triangle)
    {
        draw_triangle(pos_buffer, ind_buffer);
    }

    if (type == rst::Primitive::Cube)
    {

        draw_cube(pos_buffer, ind_buffer);
        //std::cout << "1100" << "\n";    }
}

void rst::rasterizer::draw_triangle(rst::pos_buf_id pos_buffer, rst::ind_buf_id ind_buffer)
{
        auto& buf = pos_buf[pos_buffer.pos_id];
        auto& ind = ind_buf[ind_buffer.ind_id];

        float f1 = (100 - 0.1) / 2.0;
        float f2 = (100 + 0.1) / 2.0;

        Eigen::Matrix4f mvp = projection * view * model;

        for (auto& i : ind) // c++ 11 ranged-based loop
        {
            Triangle t;

            Eigen::Vector4f v[] = {
                    mvp * to_vec4(buf[i[0]], 1.0f),
                    mvp * to_vec4(buf[i[1]], 1.0f),
                    mvp * to_vec4(buf[i[2]], 1.0f)
            };

            for (auto& vec : v) {
                //seems to get vertical
                vec /= vec.w(); // return 4th element e.g. x(), y(), z(), w()?
                
            }

            for (auto & vert : v)
            {
                // mid in screen
                vert.x() = 0.5*width*(vert.x()+1.0);
                vert.y() = 0.5*height*(vert.y()+1.0);
                vert.z() = vert.z() * f1 + f2;
            }

            for (int i = 0; i < 3; i++)
            {
                //std::cout << i << "\n";
                // why three times? // texture
                t.setVertex(i, v[i].head<3>());
                // t.setVertex(i, v[i].head<3>());
                // t.setVertex(i, v[i].head<3>());
            }
            
            // rarely to see 
            t.setColor(0, 255.0,  0.0,  0.0);
            t.setColor(1, 0.0  ,255.0,  0.0);
            t.setColor(2, 0.0  ,  0.0,255.0);

            rasterize_wireframe(t);
        }
}

void rst::rasterizer::draw_cube(rst::pos_buf_id pos_buffer, rst::ind_buf_id ind_buffer)
{
        auto& buf = pos_buf[pos_buffer.pos_id];
        auto& ind = ind_buf_cube[ind_buffer.ind_id];

        float f1 = (100 - 0.1) / 2.0;
        float f2 = (100 + 0.1) / 2.0;

        Eigen::Matrix4f mvp = projection * view * model;


        for (auto& i : ind) // c++ 11 ranged-based loop
        {
            std::cout << "1100" << "\n";
            Cube t;

            //max + 1 to
            Eigen::Vector4f v[] = 
            {
                    mvp * to_vec4(buf[i[0]], 1.0f),
                    mvp * to_vec4(buf[i[1]], 1.0f),
                    mvp * to_vec4(buf[i[2]], 1.0f),
                    mvp * to_vec4(buf[i[3]], 1.0f),
                    mvp * to_vec4(buf[i[4]], 1.0f),
                    mvp * to_vec4(buf[i[5]], 1.0f),
                    mvp * to_vec4(buf[i[6]], 1.0f),
                    mvp * to_vec4(buf[i[7]], 1.0f)
            };
        std::cout << "1111" << "\n";
            for (auto& vec : v) {
                //seems to get vertical
                vec /= vec.w(); // return 4th element e.g. x(), y(), z(), w()?
                
            }

            for (auto & vert : v)
            {
                // mid in screen
                vert.x() = 0.5*width*(vert.x()+1.0);
                vert.y() = 0.5*height*(vert.y()+1.0);
                vert.z() = vert.z() * f1 + f2;
            }

            for (int i = 0; i < 8; i++)
            {
                //std::cout << i << "\n";
                // why three times? // texture
                t.setVertex(i, v[i].head<3>());
                // t.setVertex(i, v[i].head<3>());
                // t.setVertex(i, v[i].head<3>());
            }
            
            // one pix  
            // t.setColor(0, 255.0,  0.0,  0.0);
            // t.setColor(1, 0.0  ,255.0,  0.0);
            // t.setColor(2, 0.0  ,  0.0,255.0);

            rasterize_wireframe_cube(t);
        }

}

void rst::rasterizer::rasterize_wireframe(const Triangle& t)
{
    draw_line(t.c(), t.a());
    draw_line(t.c(), t.b());
    draw_line(t.b(), t.a());
}

void rst::rasterizer::rasterize_wireframe_cube(const Cube& c)
{
    draw_line(c.bbl(), c.bbr());
    draw_line(c.bbr(), c.btr());
    draw_line(c.btr(), c.btl());
    draw_line(c.btl(), c.bbl());

    draw_line(c.bbl(), c.tbl());
    draw_line(c.bbr(), c.tbr());
    draw_line(c.btl(), c.ttl());
    draw_line(c.bbl(), c.tbl());

    draw_line(c.tbl(), c.tbr());
    draw_line(c.tbr(), c.ttr());
    draw_line(c.ttr(), c.ttl());
    draw_line(c.ttl(), c.tbl());
}

void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    depth_buf.resize(w * h);
}

int rst::rasterizer::get_index(int x, int y)
{
    return (height-y)*width + x;
}

void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    if (point.x() < 0 || point.x() >= width ||
        point.y() < 0 || point.y() >= height) return;
    auto ind = (height-point.y())*width + point.x();
    frame_buf[ind] = color;
}

