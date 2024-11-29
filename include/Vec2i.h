#ifndef VEC2I_H
#define VEC2I_H


struct Vec2i
{
    int x;
    int y;


    Vec2i operator+(Vec2i other)
    {
        Vec2i result = {x + other.x, y + other.y};
        return result;
    }

    Vec2i operator-(Vec2i other)
    {
        Vec2i result = {x - other.x, y - other.y};
        return result;
    }
    
    Vec2i operator*(int scalar)
    {
        Vec2i result = {this->x *= scalar, this->y *= scalar};
        
        return result;
    }

    Vec2i& operator+=(Vec2i other)
    {
        this->x += other.x;
        this->y += other.y;
        
        return *this;
    }

    Vec2i& operator-=(Vec2i other)
    {
        this->x -= other.x;
        this->y -= other.y;
        
        return *this;
    }

    Vec2i& operator*=(Vec2i other)
    {
        this->x *= other.x;
        this->y *= other.y;
        
        return *this;
    }
    
};




#endif;