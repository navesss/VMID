
inline Vector2D Point2D::operator - (const Point2D& v) const
{
	return Vector2D (x - v.x, y - v.y); 
}

inline Point2D Point2D::operator - () const
{
	return Point2D (-x, -y); 
}

inline Point2D Point2D::operator + (const Vector2D& v) const
{
	return Point2D (x + v.x, y + v.y); 
}

inline Point2D Point2D::operator - (const Vector2D& v) const
{
	return Point2D (x - v.x, y - v.y); 
}

inline int Point2D::operator == (const Point2D& v) const
{
	return Distance (*this, v) <= TOLER;
}

inline int Point2D::operator != (const Point2D& v) const
{
	return ! (operator == (v));
}

/*-------------------------------------------------------*/

inline Vector3D Point3D::operator - (const Point3D& v) const
{
	return Vector3D (x - v.x, y - v.y, z - v.z); 
}

inline Point3D Point3D::operator - () const
{
	return Point3D (-x, -y, -z); 
}

inline Point3D Point3D::operator + (const Vector3D& v) const
{
	return Point3D (x + v.x, y + v.y, z + v.z); 
}

inline Point3D Point3D::operator - (const Vector3D& v) const
{
	return Point3D (x - v.x, y - v.y, z - v.z); 
}

inline int Point3D::operator == (const Point3D& v) const
{
	return Distance (*this, v) <= TOLER;
}

inline int Point3D::operator != (const Point3D& v) const
{
	return ! (operator == (v));
}

/*-------------------------------------------------------*/

inline Vector2D::operator Point2D ()
{
	return Point2D (x, y); 
}

inline void Vector2D::SetUnit ()
{
	double a = Length (*this); // * (1 + 1e-15);
	if (a > TOLER) (*this) /= a;
}

inline int Vector2D::operator == (const Vector2D& v) const
{
	Vector2D a (*this), b (v);

	a.SetUnit ();
	b.SetUnit ();

	return Length (a - b) <= TOLER;
}

inline int Vector2D::operator != (const Vector2D& v) const
{
	return ! (operator == (v));
}

/*-------------------------------------------------------*/

inline Vector3D::operator Point3D ()
{
	return Point3D (x, y, z);
}

inline void Vector3D::SetUnit ()
{
	double a = Length (*this); // * (1 + 1e-15);
	if (a > TOLER) (*this) /= a;
}

inline int Vector3D::operator == (const Vector3D& v) const
{
	Vector3D a (*this), b (v);

	a.SetUnit ();
	b.SetUnit ();

	return Length (a - b) <= TOLER;
}

inline int Vector3D::operator != (const Vector3D& v) const
{
	return ! (operator == (v));
}

/*-------------------------------------------------------*/

inline double Distance (const Point2D& p1, const Point2D& p2)
{
	return Length (p1 - p2);
}

inline double Distance (const Point3D& p1, const Point3D& p2)
{
	return Length (p1 - p2);
}

inline Point2D MidPoint (const Point2D& p1, const Point2D& p2)
{
	return Point2D ((p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5);
}

inline Point3D MidPoint (const Point3D& p1, const Point3D& p2)
{
	return Point3D ((p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5, (p1.z + p2.z) * 0.5);
}

inline Point2D Min (const Point2D& p1, const Point2D& p2)
{
	return Point2D (__min (p1.x, p2.x), __min (p1.y, p2.y));
}

inline Point3D Min (const Point3D& p1, const Point3D& p2)
{
	return Point3D (__min (p1.x, p2.x), __min (p1.y, p2.y), __min (p1.z, p2.z));
}

inline Point2D Max (const Point2D& p1, const Point2D& p2)
{
	return Point2D (__max (p1.x, p2.x), __max (p1.y, p2.y));
}

inline Point3D Max (const Point3D& p1, const Point3D& p2)
{
	return Point3D (__max (p1.x, p2.x), __max (p1.y, p2.y), __max (p1.z, p2.z));
}

inline double Length (const Vector2D& v)
{
	return sqrt (v * v);
}

inline double Length (const Vector3D& v)
{
	return sqrt (v * v);
}

inline bool SamePoint (const Point2D& p1, const Point2D& p2, double toler)
{
	return Distance (p1, p2) < toler;
}

inline bool SamePoint (const Point3D& p1, const Point3D& p2, double toler)
{
	return Distance (p1, p2) < toler;
}
inline bool SameVector (const Vector2D& v1, const Vector2D& v2, double toler)
{
	return Length (v1 - v2) <= toler;	
}
inline bool SameVector (const Vector3D& v1, const Vector3D& v2, double toler)
{
	return Length (v1 - v2) <= toler;
}

inline void SwapPoint (Point2D& p1, Point2D& p2)
{
	Point2D p0 = p1; p1 = p2; p2 = p0;
}

inline void SwapPoint (Point3D& p1, Point3D& p2)
{
	Point3D p0 = p1; p1 = p2; p2 = p0;
}

inline void SwapVector (Vector2D& p1, Vector2D& p2)
{
	Vector2D p0 = p1; p1 = p2; p2 = p0;
}

inline void SwapVector (Vector3D& p1, Vector3D& p2)
{
	Vector3D p0 = p1; p1 = p2; p2 = p0;
}

