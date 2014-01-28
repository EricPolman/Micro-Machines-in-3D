#pragma once
#include <math.h>
#include <vector>
#include <string>
#include <sstream>

#define PI 3.141592653f

class Vector3{
public: 
	Vector3() : x(0), y(0), z(0), w(1) {}
	Vector3(const Vector3& a_vec) : x(a_vec.x), y(a_vec.y), z(a_vec.z), w(1){}
	Vector3(float a_fX, float a_fY, float a_fZ = 0) : x(a_fX), y(a_fY), z(a_fZ), w(1){}
	void Normalize(){
		if(x != 0 || y != 0 || z != 0)
		{
			float mag = sqrt(x*x + y*y + z*z);
			x /= mag; 
			y /= mag; 
			z /= mag;
		}
	}
	float Magnitude(){return sqrt(x*x + y*y + z*z);}
	float MagnitudeSquared(){return x*x + y*y + z*z;}
	float Distance(Vector3 a_vec)
  {
    float distX = x - a_vec.x; 
    float distY = y - a_vec.y; 
    float distZ = z - a_vec.z;
    return sqrt(distX*distX + distY*distY + distZ*distZ);
  }
	float DistanceSquared(Vector3& a_vec)
  {
    return (x - a_vec.x)*(x - a_vec.x) + (y - a_vec.y)*(y - a_vec.y) + (z - a_vec.z)*(z - a_vec.z);
  }

  float Dot (Vector3& a_vec) const
  {
    return (x * a_vec.x) + (y * a_vec.y) + (z * a_vec.z);
  }

  Vector3 Cross(Vector3& a_vec) const
  {
    Vector3 v;
    v.x = y*a_vec.z - z*a_vec.y;
    v.y = z*a_vec.x - x*a_vec.z;
    v.z = x*a_vec.y - y*a_vec.x;
    return v;
  }

	Vector3 operator+(Vector3& a_vec) const{return Vector3(x + a_vec.x, y + a_vec.y, z + a_vec.z);}
	inline void operator+=(Vector3 a_vec){x += a_vec.x, y += a_vec.y; z += a_vec.z; }
	inline bool operator==(Vector3 a_vec)
  {
    if(x == a_vec.x && y == a_vec.y && z == a_vec.z)return true; return false;
  }

	Vector3 operator-(Vector3& a_vec) const{return Vector3(x - a_vec.x, y - a_vec.y, z - a_vec.z);}
	Vector3 operator*(float a_scalar) const{return Vector3(x * a_scalar, y * a_scalar, z * a_scalar);}
	void operator*=(float a_scalar){x *= a_scalar; y *= a_scalar; z *= a_scalar;}
	Vector3 operator-(){return Vector3(-x, -y, -z);}

	float x; 
	float y; 
	float z;
  float w;
};

class Vector2{
public: 
	Vector2() : x(0), y(0) {}
	Vector2(const Vector2& a_vec) : x(a_vec.x), y(a_vec.y){}
	Vector2(float a_fX, float a_fY) : x(a_fX), y(a_fY){}
	void Normalize(){
		if(x != 0 || y != 0)
		{
			float mag = sqrt(x*x + y*y);
			x /= mag; 
			y /= mag; 
		}
	}
	float Magnitude(){return sqrt(x*x + y*y);}
	float MagnitudeSquared(){return x*x + y*y;}
	float Distance(Vector2 a_vec)
  {
    float distX = x - a_vec.x; 
    float distY = y - a_vec.y; 
    return sqrt(distX*distX + distY*distY);
  }
	float DistanceSquared(Vector2 a_vec)
  {
    return (x - a_vec.x)*(x - a_vec.x) + (y - a_vec.y)*(y - a_vec.y);
  }
	Vector2 operator+(Vector2 a_vec) const{return Vector2(x + a_vec.x, y + a_vec.y);}
	inline void operator+=(Vector2 a_vec){x += a_vec.x, y += a_vec.y; }
	inline bool operator==(Vector2 a_vec)
  {
    if(x == a_vec.x && y == a_vec.y)return true; return false;
  }

	Vector2 operator-(Vector2 a_vec) const{return Vector2(x - a_vec.x, y - a_vec.y);}
	Vector2 operator*(float a_scalar) const{return Vector2(x * a_scalar, y * a_scalar);}
	void operator*=(float a_scalar){x *= a_scalar; y *= a_scalar;}
	Vector2 operator-(){return Vector2(-x, -y);}

	float x; 
	float y;
};


class Vector2i
{
public:
	Vector2i(int a_x, int a_y) : x(a_x), y(a_y){}
	Vector2i() : x(0), y(0) {}
	int x;
	int y;
};

struct Rectangle
{
	Rectangle(float a_x, float a_y, float a_width, float a_height) 
    : x(a_x), y(a_y), width(a_width), height(a_height) {}
	Rectangle() : x(0), y(0), width(0), height(0) {}
	float x;
	float y;
  float width;
  float height;
  
  bool Intersects(const Rectangle& a_other) const;
  bool Intersects(const Vector3& a_position, float a_radius) const;
};

/*
class CollisionInfo{
public:
	CollisionInfo(){isColliding = false;}
	
	Vector3 depth;
	bool isColliding;
};

enum CollisionType {UNDEFINED, CIRCLE, BOX, LINE};
class CollisionObject
{
public:
	virtual bool Intersects(const CollisionObject& a_other) = 0;
  CollisionType type;
};

class CollisionBox : public CollisionObject
{
public:
	CollisionBox(Vector2 a_position, float a_width, float a_height)
		: m_position(a_position), width(a_width), height(a_height){type = BOX;}
	CollisionBox()
		: width(0), height(0){}

	virtual bool Intersects(const CollisionObject& a_other);
  void SetPosition(Vector2 a_vec) {m_position = a_vec;}

  Vector2 m_position;
	float width;
	float height;
};

class CollisionLine : public CollisionObject
{
public:
  CollisionLine(float a_beginX, float a_beginY, float a_endX, float a_endY)
    : m_start(a_beginX, a_beginY), m_end(a_endX, a_endY)
  {type = LINE;}
	virtual bool Intersects(const CollisionObject& a_other);

  Vector2 m_start;
  Vector2 m_end;
  
};


class CollisionCircle : public CollisionObject
{
public:
  CollisionCircle(float a_x, float a_y, float a_fRadius)
    : m_position(a_x, a_y), m_radius(a_fRadius)
  {type = CIRCLE;}

	virtual bool Intersects(const CollisionObject& a_other);
  
  Vector2 GetPosition() const {return m_position;}
  void SetPosition(Vector2 a_vec) {m_position = a_vec;}

  Vector2 m_position;
  float m_radius;
private:
  float padding;
};
*/


static std::vector<std::string> explode(std::string const & s, char delim)
{
  std::vector<std::string> result;
  std::istringstream iss(s);

  for (std::string token; std::getline(iss, token, delim); )
  {
      result.push_back(std::move(token));
  }

  return result;
};