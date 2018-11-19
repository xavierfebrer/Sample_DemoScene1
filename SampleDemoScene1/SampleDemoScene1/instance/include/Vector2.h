#pragma once

//INCLUDES
#include <math.h>
#include <string>

//VECTOR2 CLASS
template<typename T = double>
class Vector2{
public:
	//MEMBERS
	T x;
	T y;

	//CONSTRUCTORS
	Vector2(void);
	Vector2(T xValue, T yValue);
	Vector2(const Vector2 & v);
	Vector2(const Vector2 * v);

	//DECONSTRUCTOR
	~Vector2(void);

	//METHODS
	void Set(T xValue, T yValue);

	T Length() const;
	T LengthSquared() const;
	T Distance(const Vector2 & v) const;
	T DistanceSquared(const Vector2 & v) const;
	T Dot(const Vector2 & v) const;
	T Cross(const Vector2 & v) const;

	Vector2 & Normal();
	Vector2 & Normalize();

	//ASSINGMENT AND EQUALITY OPERATIONS
	inline Vector2 & operator=(const Vector2 & v) { x = v.x; y = v.y; return *this; }
	inline Vector2 & operator=(const T & f) { x = f; y = f; return *this; }
	inline Vector2 & operator-(void) { x = -x; y = -y; return *this; }
	inline bool operator==(const Vector2 & v) const { return (x == v.x) && (y == v.y); }
	inline bool operator!=(const Vector2 & v) const { return (x != v.x) || (y != v.y); }

	//VECTOR2 TO VECTOR2 OPERATIONS
	inline const Vector2 operator+(const Vector2 & v) const { return Vector2(x + v.x, y + v.y); }
	inline const Vector2 operator-(const Vector2 & v) const { return Vector2(x - v.x, y - v.y); }
	inline const Vector2 operator*(const Vector2 & v) const { return Vector2(x * v.x, y * v.y); }
	inline const Vector2 operator/(const Vector2 & v) const { return Vector2(x / v.x, y / v.y); }

	//VECTOR2 TO THIS OPERATIONS
	inline Vector2 & operator+=(const Vector2 & v) { x += v.x; y += v.y; return *this; }
	inline Vector2 & operator-=(const Vector2 & v) { x -= v.x; y -= v.y; return *this; }
	inline Vector2 & operator*=(const Vector2 & v) { x *= v.x; y *= v.y; return *this; }
	inline Vector2 & operator/=(const Vector2 & v) { x /= v.x; y /= v.y; return *this; }

	//SCALER TO VECTOR2 OPERATIONS
	inline const Vector2 operator+(T v) const { return Vector2(x + v, y + v); }
	inline const Vector2 operator-(T v) const { return Vector2(x - v, y - v); }
	inline const Vector2 operator*(T v) const { return Vector2(x * v, y * v); }
	inline const Vector2 operator/(T v) const { return Vector2(x / v, y / v); }

	//SCALER TO THIS OPERATIONS
	inline Vector2 & operator+=(T v) { x += v; y += v; return *this; }
	inline Vector2 & operator-=(T v) { x -= v; y -= v; return *this; }
	inline Vector2 & operator*=(T v) { x *= v; y *= v; return *this; }
	inline Vector2 & operator/=(T v) { x /= v; y /= v; return *this; }
};






//CONSTRUCTORS
template<typename T>
Vector2<T>::Vector2(void) { }
template<typename T>
Vector2<T>::Vector2(T xValue, T yValue) : x(xValue), y(yValue) { }
template<typename T>
Vector2<T>::Vector2(const Vector2 & v) : x(v.x), y(v.y) { }
template<typename T>
Vector2<T>::Vector2(const Vector2 * v) : x(v->x), y(v->y) { }

//DECONSTRUCTOR
template<typename T>
Vector2<T>::~Vector2(void) { }

//METHODS
template<typename T>
void Vector2<T>::Set(T xValue, T yValue) { x = xValue; y = yValue; }

template<typename T>
T Vector2<T>::Length() const { return sqrt(x * x + y * y); }
template<typename T>
T Vector2<T>::LengthSquared() const { return x * x + y * y; }
template<typename T>
T Vector2<T>::Distance(const Vector2 & v) const { return sqrt(((x - v.x) * (x -     v.x)) + ((y - v.y) * (y - v.y))); }
template<typename T>
T Vector2<T>::DistanceSquared(const Vector2 & v) const { return ((x - v.x) * (x -     v.x)) + ((y - v.y) * (y - v.y)); }
template<typename T>
T Vector2<T>::Dot(const Vector2 & v) const { return x * v.x + y * v.y; }
template<typename T>
T Vector2<T>::Cross(const Vector2 & v) const { return x * v.y + y * v.x; }

template<typename T>
Vector2<T> & Vector2<T>::Normal() { Set(-y, x); return *this; }
template<typename T>
Vector2<T> & Vector2<T>::Normalize(){
	if(Length() != 0){
		T length = LengthSquared();
		x /= length; y /= length;
		return *this;
	}
	x = y = 0;
	return *this;
}