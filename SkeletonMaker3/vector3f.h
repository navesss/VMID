#ifndef __VECTOR3F_H__
#define __VECTOR3F_H__

#include <cmath>
#include "vector3f.h"
//#include "io/io.h"
#include <iostream>
#include <fstream>
using namespace std;

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef M_PI
#define M_PI          3.14159265358979323846
#endif

class vector3f {
public:

    float x;
    float y;
    float z;

    vector3f() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    void zero() {
      x = 0.0f;
      y = 0.0f;
      z = 0.0f;
    }

    bool equals(const vector3f &other) const;

    vector3f(ifstream &input, bool ascii);

    void write(ofstream &output, bool ascii) const;
    void read(ifstream &input, bool ascii);

    vector3f(float x_, float y_, float z_);
    void set(float x_, float y_, float z_);
    float length(void) const;
    void normalize(void);

   /**
     * Distance to other vector.
     */
    float distance(const vector3f &v2) const;
    vector3f perpendicular() const;

    /**
      * Compute the dot product of the two vectors:
      * \f$\mathbf{v}_1 \cdot \mathbf{v}_2 = x_1 \cdot x_2 + y_1 \cdot y_2 + z_1 \cdot z_2\f$.
      * @param v2 the other vector
      */
    float dotProduct(const vector3f &v2 ) const;

    /**
      * Compute the cross product of the two vectors: 
      * \f$\mathbf{v}_1 \times \mathbf{v}_2 = (y_1 \cdot z_2 - z_1 \cdot y_2, -x_1 \cdot z_2 + z_1 \cdot x_2, x_1 \cdot y_2 - y_1 \cdot x_2)\f$.
      * @param v2 the other vector
      */
    vector3f crossProduct(const vector3f &v2) const;

    void print() const;

    // Operators...
    vector3f operator + (const vector3f &other) const;
    vector3f operator + (const float scalar) const;
    vector3f operator - (const vector3f &other) const;
    vector3f operator - (const float scalar) const;
    vector3f operator * (const vector3f &other) const;
    vector3f operator / (const vector3f &other) const;

    vector3f operator * (const float scalar) const;
    friend vector3f operator * (const float scalar, const vector3f &other);
    
    vector3f& operator = (const vector3f &other);
    vector3f& operator += (const vector3f &other);
    vector3f& operator -= (const vector3f &other);
    vector3f& operator *= (const float scalar);
    vector3f& operator *= (const vector3f &other);

    vector3f operator + (void) const;
	vector3f operator - (void) const;

	//È¡ÔªËØ
	float& operator [] (int i)
		{
			switch (i) 
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: return x;
			}
		}
};


inline vector3f::vector3f( float x_, float y_, float z_ ) {
    x = x_;
    y = y_;
    z = z_;
}

inline vector3f::vector3f(ifstream &input, bool ascii) {
   read(input, ascii);
}

inline void vector3f::set( float x_, float y_, float z_ ) {
    x = x_;
    y = y_;
    z = z_;
}

inline void vector3f::print() const {
//   cout << x << "," << y << "," << z << endl;
}

inline vector3f vector3f::perpendicular() const {
   vector3f result;
   if (fabs(x) < fabs(y)) {
      if (fabs(x) < fabs(z)) {
         result.x = 1.0f-x*x;
         result.y = -x*y;
         result.z = -x*z;
         return result;
      }
   }
   else {
      if (fabs(y) < fabs(z)) {
         result.x  =-y*x;
         result.y = 1.0f-y*y;
         result.z = -y*z;
         return result;
      }
   }
   result.x = -z*x;
   result.y = -z*y;
   result.z = 1.0f-z*z;
   return result;
}


inline void vector3f::write(ofstream &output, bool ascii) const {
   //writeFloat(output, x, ascii);
   //writeFloat(output, y, ascii);
   //writeFloat(output, z, ascii);
}

inline void vector3f::read(ifstream &input, bool ascii) {
   //x = readFloat(input, ascii);
   //y = readFloat(input, ascii);
   //z = readFloat(input, ascii);
}

inline float vector3f::length() const {
    return( (float)sqrt( x * x + y * y + z * z ) );
}

inline void vector3f::normalize() {
	float fLength = length();

	x = x / fLength;
	y = y / fLength;
	z = z / fLength;
}


inline float vector3f::distance(const vector3f &v2) const {
	float dx = x - v2.x;
	float dy = y - v2.y;
	float dz = z - v2.z;

	return (float)sqrt( dx * dx + dy * dy + dz * dz );
}

inline float vector3f::dotProduct(const vector3f &v2 ) const {
	return( x * v2.x + y * v2.y + z * v2.z  );
}

inline vector3f vector3f::crossProduct(const vector3f &v2 ) const {
	vector3f vCrossProduct;
	vCrossProduct.x =  y * v2.z - z * v2.y;
	vCrossProduct.y = -x * v2.z + z * v2.x;
	vCrossProduct.z =  x * v2.y - y * v2.x;

	return vCrossProduct;
}

// Operators...

inline vector3f vector3f::operator + ( const vector3f &other ) const {
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x + other.x;
    vResult.y = y + other.y;
    vResult.z = z + other.z;

    return vResult;
}

inline vector3f vector3f::operator + ( const float scalar ) const {
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x + scalar;
    vResult.y = y + scalar;
    vResult.z = z + scalar;

    return vResult;
}
inline vector3f vector3f::operator + ( void ) const {
    return *this;
}

inline vector3f vector3f::operator - ( const vector3f &other ) const {
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x - other.x;
    vResult.y = y - other.y;
    vResult.z = z - other.z;

    return vResult;
}

inline vector3f vector3f::operator - ( const float scalar ) const {
    vector3f vResult(0.0f, 0.0f, 0.0f);
	
    vResult.x = x - scalar;
    vResult.y = y - scalar;
    vResult.z = z - scalar;
	
    return vResult;
}

inline vector3f vector3f::operator - ( void ) const {
    vector3f vResult(-x, -y, -z);

    return vResult;
}

inline vector3f vector3f::operator * ( const vector3f &other ) const {
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x * other.x;
    vResult.y = y * other.y;
    vResult.z = z * other.z;

    return vResult;
}

inline vector3f vector3f::operator * ( const float scalar ) const {
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x * scalar;
    vResult.y = y * scalar;
    vResult.z = z * scalar;

    return vResult;
}

inline vector3f operator * ( const float scalar, const vector3f &other ) {
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = other.x * scalar;
    vResult.y = other.y * scalar;
    vResult.z = other.z * scalar;

    return vResult;
}

inline vector3f vector3f::operator / ( const vector3f &other ) const {
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x / other.x;
    vResult.y = y / other.y;
    vResult.z = z / other.z;

    return vResult;
}

inline vector3f& vector3f::operator = ( const vector3f &other ) {
	x = other.x;
	y = other.y;
	z = other.z;

    return *this;
}

inline vector3f& vector3f::operator += ( const vector3f &other ) {
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

inline vector3f& vector3f::operator *= ( const vector3f &other ) {
    x *= other.x;
    y *= other.y;
    z *= other.z;

    return *this;
}

inline vector3f& vector3f::operator *= ( const float scalar ) {
    x *= scalar;
    y *= scalar;
    z *= scalar;

    return *this;
}

inline vector3f& vector3f::operator -= ( const vector3f &other ) {
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

inline bool vector3f::equals(const vector3f &s) const {
   return (abs(x-s.x)<0.0000001)&&(abs(y-s.y)<0.0000001)&&(abs(z-s.z)<0.00000001);
}

#endif /* __VECTOR3F_H__ */
