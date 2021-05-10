
#ifndef	METRIX_H
#define METRIX_H

#include "Point.h"
#include "_global.h"

template<int dim> 
class Matrix
{             
public:
    double mat[dim][dim];

public:
 	//构造函数
	Matrix ();
	Matrix (const Matrix& m);

	//赋值操作符
	void operator += (const Matrix& m);
	void operator -= (const Matrix& m);
	void operator *= (const Matrix& m);
	void operator *= (double num);
	void operator /= (double num);

	//矩阵加
	Matrix operator + (const Matrix& m) const;
	//矩阵减
	Matrix operator - (const Matrix& m) const;
	//矩阵乘
	Matrix operator * (const Matrix& m) const;
	//数乘
	Matrix operator * (double num) const;
	//数除
	Matrix operator / (double num) const;
	//求逆矩阵
	Matrix operator ! () const;
	
	void SetZero ();
	void SetUnit ();
};

class Matrix3 : public Matrix<3>
{
private:
	void Dummy ();

public:
	Matrix3 ();
	Matrix3 (const Matrix<3>& m);

	void SetShift (double dx, double dy);
	void SetScale (double sx, double sy);
	void SetRotate (double angle);
};

class Matrix4 : public Matrix<4>
{
private:
	void Dummy ();

public:
	Matrix4 ();
	Matrix4 (const Matrix<4>& m);

	void SetShift (double dx, double dy, double dz);
	void SetScale (double sx, double sy, double sz);
	void SetRotate (int axis, double angle);
	void SetRotateV (const Vector3D& v, double ang);
	void SetRotateZ (const Vector3D& v);
	void SetLocal (const Point3D& p0, const Vector3D& zv, const Vector3D& xv);
	void SetMirror (double a, double b, double c, double d);
};

Point2D operator * (const Point2D& p, const Matrix3& mat);
Vector2D operator * (const Vector2D& p, const Matrix3& mat);

Point3D operator * (const Point3D& p, const Matrix4& mat);
Vector3D operator * (const Vector3D& p, const Matrix4& mat);

template <int dim>
Matrix<dim>::Matrix ()
{
	memset (mat, 0, sizeof (mat));
}

template <int dim>
Matrix<dim>::Matrix (const Matrix& m)
{
	memcpy (mat, m.mat, sizeof (mat));
}

template <int dim>
void Matrix<dim>::SetZero ()
{
	memset (mat, 0, sizeof (mat));
}

template <int dim>
void Matrix<dim>::SetUnit ()
{
	SetZero ();
	for (int i = 0; i < dim; i++)
		mat[i][i] = 1;
}

template <int dim>
void Matrix<dim>::operator += (const Matrix& m)
{
	int		i, j;

	for (i=0; i<dim; i++)
		for (j=0; j<dim; j++)
			mat[i][j] += m.mat[i][j];
}

template <int dim>
void Matrix<dim>::operator -= (const Matrix& m)
{
	int		i, j;

	for (i=0; i<dim; i++)
		for (j=0; j<dim; j++)
			mat[i][j] -= m.mat[i][j];
}

template <int dim>
void Matrix<dim>::operator *= (double num)
{
	int		i, j;

	for (i=0; i<dim; i++)
		for (j=0; j<dim; j++)
			mat[i][j] *= num;
}

template <int dim>
void Matrix<dim>::operator /= (double num)
{
	int		i, j;

	if (fabs (num) > TOLER)
		for (i=0; i<dim; i++)
			for (j=0; j<dim; j++)
				mat[i][j] /= num;
}

template <int dim>
void Matrix<dim>::operator *= (const Matrix& m)
{
	int		i, j, k;
	Matrix	aa;

	for (i=0; i<dim; i++)
		for (j=0; j<dim; j++)
			for (k=0; k<dim; k++)
				aa.mat[i][j] += mat[i][k]*m.mat[k][j];

	(*this) = aa;
}

template <int dim> 
Matrix<dim> Matrix<dim>::operator + (const Matrix& m) const
{
	int		i, j;
	Matrix	aa (*this);

	for (i=0; i<dim; i++)
		for (j=0; j<dim; j++)
			aa.mat[i][j] += m.mat[i][j];
	return aa;
}

template <int dim>
Matrix<dim> Matrix<dim>::operator - (const Matrix& m) const
{
	int		i, j;
	Matrix	aa (*this);

	for (i=0; i<dim; i++)
		for (j=0; j<dim; j++)
			aa.mat[i][j] -= m.mat[i][j];
	return aa;
}

template <int dim>
Matrix<dim> Matrix<dim>::operator * (const Matrix& m) const
{
	int		i, j, k;
	Matrix	aa;

	for (i=0; i<dim; i++)
		for (j=0; j<dim; j++)
			for (k=0; k<dim; k++)
				aa.mat[i][j] += mat[i][k]*m.mat[k][j];
	return aa;
}

template <int dim>
Matrix<dim> Matrix<dim>::operator * (double num) const
{
	int		i, j;
	Matrix	aa (*this);

	for (i=0; i<dim; i++)
		for (j=0; j<dim; j++)
			aa.mat[i][j] *= num;
	return aa;
}

template <int dim>
Matrix<dim> Matrix<dim>::operator / (double num) const
{
	int		i, j;
	Matrix	aa (*this);

	if (fabs (num) > TOLER)
		for (i=0; i<dim; i++)
			for (j=0; j<dim; j++)
				aa.mat[i][j] /= num;
	return aa;
}

template <int dim>
Matrix<dim> Matrix<dim>::operator ! () const
{
	double delta, buf;
	int i, j, k;

	Matrix aa (*this), mat2;

	mat2.SetUnit ();
	for (i=0; i<dim; i++)
	{
		delta = aa.mat[i][i];
		if (fabs (delta)<=TOLER)
		{
			for (j=i+1; j<dim; j++)
				if (fabs (aa.mat[j][i]) > TOLER) break;
			if (j >= dim)
				return (*this);
			for (k=0; k<dim; k++)
			{
				buf = aa.mat[j][k];
				aa.mat[j][k] = aa.mat[i][k];
				aa.mat[i][k] = buf;
				buf = mat2.mat[j][k];
				mat2.mat[j][k] = mat2.mat[i][k];
				mat2.mat[i][k] = buf;
			}
			delta = aa.mat[i][i];
		}
		for (j=0; j<dim; j++)
		{
			aa.mat[i][j] /= delta;
			mat2.mat[i][j] /= delta;
		}
		for (k=0; k<dim; k++)
			if (k != i)
			{
				buf = -1*aa.mat[k][i];
				for (j=0; j<dim; j++)
				{
					aa.mat[k][j] = aa.mat[i][j]*buf + aa.mat[k][j];
					mat2.mat[k][j] = mat2.mat[i][j]*buf + mat2.mat[k][j];
				}
			}
	}

	return mat2;
}


/////////// 将原2D中的Vector.h 和 Matrix.h并入
//#ifdef _2D3DVERSION

template<class TYPE>
class CVector : public CObject
{
private:
	TYPE		*	m_Data;
	int				m_Size;
public:

	// 构造函数和析构函数
	CVector();
	CVector( int size );
	CVector( CVector & vector );
	~CVector();

	// 初始化
	void			Init( int size );
	// 向量维数
	int				GetSize();
	// 取分量
	TYPE &			operator [] ( int );
	// 赋值函数
	CVector &	operator = ( CVector & );

//友元（运算）函数
	//加
	friend CVector	operator + ( CVector &, CVector & );
	//减
	friend CVector	operator - ( CVector &, CVector & );
	//点积
	friend double	operator * ( CVector &, CVector & );
	//数乘
	friend CVector	operator * ( CVector &, double );
	friend CVector	operator / ( CVector &, double );
};


template<class TYPE>
CVector<TYPE>::CVector()
{
	m_Size = 0;
	m_Data = NULL;
}

template<class TYPE>
CVector<TYPE>::CVector( int size )
{
	ASSERT( size > 0 );

	m_Size = size;
	m_Data = new TYPE[ m_Size ];
}

template<class TYPE>
CVector<TYPE>::CVector( CVector<TYPE> & vector )
{
	m_Size = vector.m_Size;

	if( m_Size > 0 )
		m_Data = new TYPE[ m_Size ];
	else m_Data = NULL;

	for(int i = 0; i < m_Size; i++)
		m_Data[i] = vector.m_Data[i];
}

template<class TYPE>
CVector<TYPE>::~CVector()
{
	if( m_Data != NULL )
		delete [] m_Data;
}

// 初始化
template<class TYPE>
void CVector<TYPE>::Init( int size )
{
	ASSERT( size > 0 );

	m_Data = NULL;
	m_Size = size;
	m_Data = new TYPE[ m_Size ];
}

// 向量维数
template<class TYPE>
int CVector<TYPE>::GetSize()
{
	return m_Size;
}

// 取分量
template<class TYPE>
TYPE & CVector<TYPE>::operator [] ( int i )
{
	ASSERT( i < m_Size && i >= 0 );
	return m_Data[i];
}

// 赋值函数
template<class TYPE>
CVector<TYPE> & CVector<TYPE>::operator = ( CVector<TYPE> & vector )
{
	ASSERT( m_Size == vector.m_Size );

	for(int i = 0; i < m_Size; i++)
	{
		m_Data[i] = vector.m_Data[i];
	}
	return vector;
}

//友元（运算）函数
//加
template<class TYPE>
CVector<TYPE>  operator + ( CVector<TYPE> & v1, CVector<TYPE> & v2 )
{
	ASSERT( v1.m_Size == v2.m_Size && v1.m_Size > 0 );

	CVector<TYPE>	v(v1.m_Size);

	for(int i = 0; i < v.m_Size; i++)
	{
		v.m_Data[i] = v1.m_Data[i] + v2.m_Data[i];
	}

	return v;
}

//减
template<class TYPE>
CVector<TYPE>  operator - ( CVector<TYPE> & v1, CVector<TYPE> & v2 )
{
	ASSERT( v1.m_Size == v2.m_Size && v1.m_Size > 0 );

	CVector<TYPE>	v(v1.m_Size);

	for(int i = 0; i < v.m_Size; i++)
	{
		v.m_Data[i] = v1.m_Data[i] - v2.m_Data[i];
	}

	return v;
}

//点积
template<class TYPE>
double	operator * ( CVector<TYPE> & v1, CVector<TYPE> & v2 )
{
	ASSERT( v1.m_Size == v2.m_Size && v1.m_Size > 0 );

	double value = 0;

	for(int i = 0; i < v1.m_Size; i++)
	{
		value += v1.m_Data[i] * v2.m_Data[i];
	}

	return value;
}

//数乘
template<class TYPE>
CVector<TYPE>  operator * ( CVector<TYPE> & vv, double s )
{
	ASSERT( vv.m_Size > 0 );

	CVector<TYPE>	v(vv.m_Size);

	for(int i = 0; i < v.m_Size; i++)
	{
		v.m_Data[i] = vv.m_Data[i] * s;
	}

	return v;
}

template<class TYPE>
CVector<TYPE>  operator / ( CVector<TYPE> & vv, double s )
{
	ASSERT( vv.m_Size > 0 && fabs(s) > TOLER );

	CVector<TYPE>	v(vv.m_Size);

	for(int i = 0; i < v.m_Size; i++)
	{
		v.m_Data[i] = vv.m_Data[i] / s;
	}

	return v;
}

template<class TYPE>
class CMatrix : public CObject
{
private:
	TYPE		**		m_Data;
	int					m_RowNum;
	int					m_ColumnNum;
public:
	CMatrix();
	CMatrix(int row, int column, TYPE** data=NULL);
	CMatrix( CMatrix<TYPE>& matr );
	virtual ~CMatrix();
	bool					Init(int row, int coloum);
	int						Row()		const;
	int						Column()	const;

	TYPE*					operator[] (int i);
	void					operator = (CMatrix<TYPE>& matr);
	CMatrix<TYPE>			Transpose();
	CVector<TYPE> 		GetRowVector( int i );
	CVector<TYPE> 		GetColumnVector( int j );
	
	bool 					Inverse();
	bool 					GetMoor(CMatrix<TYPE>& moor);
	void					A( CMatrix<TYPE> & mat, int i, int j ); //代数余子式
	double					Det(); //行列式
	void					TestPrint(char* fn);

	//友元函数
	friend CMatrix<TYPE> operator + (CMatrix<TYPE>& matr1 , CMatrix<TYPE>& matr2);
	friend CMatrix<TYPE> operator - (CMatrix<TYPE>& matr1 , CMatrix<TYPE>& matr2);
	friend CMatrix<TYPE> operator * (CMatrix<TYPE>& matr1 , CMatrix<TYPE>& matr2);

	//右乘向量
	friend CVector<TYPE>	operator * (CMatrix<TYPE>& matr , CVector<TYPE>& v);
	//左乘向量
	friend CVector<TYPE>	operator * (CVector<TYPE>& v , CMatrix<TYPE>& matr);
};


template<class TYPE>
CMatrix<TYPE>::CMatrix()
{
	m_RowNum = 0;
	m_ColumnNum = 0;
	m_Data = NULL;
}

template<class TYPE>
CMatrix<TYPE>::CMatrix(int row, int column, TYPE** data)
{
	ASSERT((row>0) && (column>0));	

	m_RowNum=row;
	m_ColumnNum=column;
	Init(row, column);

	if (data)
	{
		for(int i=0; i<row; i++)
		{
			for(int j=0; j<column; j++)
			{
				m_Data[i][j] = data[i][j];
			}
		}
	}
}
                  
template<class TYPE>
CMatrix<TYPE>::CMatrix( CMatrix<TYPE>& matr )   //由一个已知矩阵构造另一个新矩阵
{
	m_RowNum    = matr.m_RowNum;
	m_ColumnNum = matr.m_ColumnNum;
	m_Data      = new (TYPE(*[m_RowNum]));
	for(int i=0; i<m_RowNum; i++)
	{
		m_Data[i] = new TYPE[m_ColumnNum];
		for(int j=0; j<m_ColumnNum; j++)
		{
			m_Data[i][j] = matr[i][j];
		}
	}
}

template<class TYPE>
CMatrix<TYPE>::~CMatrix()
{
	if(m_Data) 
	{
		for(int i=0; i<m_RowNum; i++)
			delete m_Data[i];
		delete [] m_Data;
	}
}

template<class TYPE>
bool CMatrix<TYPE>::Init(int row, int column)	//分配空间并置全0
{
	m_RowNum=row;
	m_ColumnNum=column;
	m_Data=new (TYPE(*[row]));
//	m_Data=new TYPE * [row];
	for(int i=0;i<row;i++)
	{
		m_Data[i]=new TYPE[column];
		memset(m_Data[i],0,column*sizeof(TYPE));
	}
	return true;
}

template<class TYPE>
int CMatrix<TYPE>::Row()		const
{
	return m_RowNum;
}
	
template<class TYPE>
int CMatrix<TYPE>::Column()	const
{
	return m_ColumnNum;
}
	
template<class TYPE>
TYPE* CMatrix<TYPE>::operator[](int i)
{
	ASSERT((i>=0) && (i<m_RowNum));
	return m_Data[i];
}

template<class TYPE>
void CMatrix<TYPE>::operator=(CMatrix<TYPE>& matr)     //已经为它分配了空间。
{
	ASSERT((m_RowNum==matr.Row()) && (m_ColumnNum==matr.Column()));

	for(int i=0; i<m_RowNum; i++)
		for(int j=0; j<m_ColumnNum; j++)
			m_Data[i][j] = matr[i][j];

//	memcpy(this,&matr,sizeof(CMatrix));
}

template<class TYPE>
CMatrix<TYPE> CMatrix<TYPE>::Transpose()
{
	CMatrix<TYPE>	mat(m_ColumnNum, m_RowNum);

	for(int i = 0; i < m_RowNum; i++)
		for(int j = 0; j < m_ColumnNum; j++)
			mat[j][i] = m_Data[i][j];

	return mat;
}

template<class TYPE>
CVector<TYPE> CMatrix<TYPE>::GetRowVector( int i )
{
	CVector<TYPE> v(m_ColumnNum);

	for(int j = 0; j < m_ColumnNum; j++)
		v[j] = m_Data[i][j];

	return v;
}

template<class TYPE>
CVector<TYPE> CMatrix<TYPE>::GetColumnVector( int j )
{
	CVector<TYPE> v(m_RowNum);

	for(int i = 0; i < m_RowNum; i++)
		v[i] = m_Data[i][j];

	return v;
}



#define MIN_VALUE 1e-13

template<class TYPE>
bool CMatrix<TYPE>::Inverse()
{
	int *is, *js, i, j, k;
  	double d, p;
    
    ASSERT(m_RowNum==m_ColumnNum);
  	is = new int[m_RowNum];
  	js = new int[m_RowNum];
  	int n=m_RowNum;
  	if (is && js)
  	{
		memset(is, 0, n*sizeof(int));
		memset(js, 0, n*sizeof(int));
	  	for( k = 0; k <= n-1; k ++ )
		{
			d = 0.0;
		 	for( i = k; i <= n-1; i ++ )
				for( j = k; j <= n-1; j ++ )
			  	{
			  		p = fabs(m_Data[i][j]);
			  		if( p > d )
				 	{
				 		d = p;
				 		is[k] = i;
				 		js[k] = j;
				 	}
			  	}
		 	if( d < MIN_VALUE)
			{
				delete is;
				delete js;
				return false;
			}
		 	if( is[k] != k )
				for( j = 0; j <= n-1; j ++ )
				{
					p = m_Data[k][j]; m_Data[k][j] = m_Data[is[k]][j]; m_Data[is[k]][j] = p;
			  	}
			if( js[k] != k )
				for( i = 0; i <= n-1; i ++ )
		  		{
		  			p = m_Data[i][k]; m_Data[i][k] = m_Data[i][js[k]]; m_Data[i][js[k]] = p;
		  		}
			m_Data[k][k] = 1.0/m_Data[k][k];
			for( j = 0; j <= n-1; j ++ )
				if( j != k )
		  		{
		  			m_Data[k][j] = m_Data[k][j]*m_Data[k][k];
		  		}
			for( i = 0; i <= n-1; i ++ )
				if( i != k )
		 			for( j = 0; j <= n-1; j ++ )
						if( j != k )
			  			{
							m_Data[i][j] = m_Data[i][j]-m_Data[i][k]*m_Data[k][j];
			  			}
		 	for( i = 0; i <= n-1; i ++ )
				if( i != k )
			  	{
			  		m_Data[i][k] = -m_Data[i][k]*m_Data[k][k];
				}
		}
		for( k = n-1; k >= 0; k -- )
		{
			if( js[k] != k )
				for( j = 0; j <= n-1; j ++ )
			  	{
			  		p = m_Data[k][j]; m_Data[k][j] = m_Data[js[k]][j]; m_Data[js[k]][j] = p;
			  	}
		 	if( is[k] != k )
				for( i = 0; i <= n-1; i ++ )
			  	{ 
			  		p = m_Data[i][k]; m_Data[i][k] = m_Data[i][is[k]]; m_Data[i][is[k]] = p;
			  	}
		}
		delete is;
		delete js;
	}
	return true;
}

template<class TYPE>
bool CMatrix<TYPE>::GetMoor(CMatrix<TYPE>& moor)
{
	int i,j,k;
	CMatrix<TYPE> square;
	square.Init(m_RowNum,m_RowNum);
	double sum;
	for(i=0;i<m_RowNum;i++)
		for(j=i;j<m_RowNum;j++)
		{
			sum=0;
			for(k=0;k<m_ColumnNum;k++)
				sum+=m_Data[i][k]*m_Data[j][k];
			square[i][j]=square[j][i]=sum;
		}
	if(!square.Inverse())
		return false;
	for(i=0;i<m_ColumnNum;i++)
		for(j=0;j<m_RowNum;j++)
		{
			sum=0;
			for(k=0;k<m_RowNum;k++)
				sum+=m_Data[k][i]*square[k][j];
			moor[i][j]=sum;
		}
	return true;
}

template<class TYPE>
void CMatrix<TYPE> :: A( CMatrix<TYPE> & mat, int i, int j )
{
	for(int ii = 0; ii < m_RowNum; ii++)
	{
		if( ii != i )
		{
			int	ki = ( ii > i ? ii-1 : ii );
			for(int jj = 0; jj < m_ColumnNum; jj++)
			{
				if( jj != j )
				{
					int kj = ( jj > j ? jj-1 : jj );
					mat[ki][kj] = m_Data[i][j];
				}
			}
		}
	}
}

#define	signA(i,j) ((i+j)%2 ? -1 : 1)

template<class TYPE>
double CMatrix<TYPE>::Det() //行列式
{
	if( m_RowNum == 1 ) return (double)m_Data[0][0];

	CMatrix<TYPE>	a(m_RowNum-1,m_ColumnNum-1);
	double				det = 0;

	for(int j = 0; j < m_ColumnNum; j++)
	{
		A( a, 0, j );
		det += signA(0,j)*((double)m_Data[0][j]) * a.Det();
	}
	return det;
}

template<class TYPE>
void CMatrix<TYPE>::TestPrint(char* fn)
{
	CFile file(fn, CFile::modeCreate | CFile::modeWrite);
	CString str, str1;

	str.Format("\nThis is a Matrix.\n");

	for (int i = 0; i < m_RowNum; i++)
	{
		for (int j = 0; j < m_ColumnNum; j++)
		{
				str1.Format("%7d", m_Data[i][j]);
				str += str1;
		}
		str += "\n";
	}
	file.Write(str.GetBuffer(256), str.GetLength());
	str.ReleaseBuffer();

	file.Close();
}

template<class TYPE>
CMatrix<TYPE> operator+(CMatrix<TYPE>& matr1 , CMatrix<TYPE>& matr2)
{
	ASSERT((matr1.Row()==matr2.Row()) && (matr1.Column()==matr2.Column()));
	CMatrix<TYPE> pret(matr1);
	for(int i=0;i<matr1.Row();i++)
		for(int j=0;j<matr1.Column();j++)
			pret[i][j]+=matr2[i][j];
	return pret;
}

template<class TYPE>
CMatrix<TYPE> operator-(CMatrix<TYPE>& matr1 , CMatrix<TYPE>& matr2)
{
	ASSERT((matr1.Row()==matr2.Row()) && (matr1.Column()==matr2.Column()));
	CMatrix<TYPE> pret(matr1);
//	CMatrix<TYPE> ret(&matr1);
	for(int i=0;i<matr1.Row();i++)
		for(int j=0;j<matr1.Column();j++)
			pret[i][j]-=matr2[i][j];
	return pret;
}

template<class TYPE>
CMatrix<TYPE> operator*(CMatrix<TYPE>& matr1 , CMatrix<TYPE>& matr2)
{
	ASSERT(matr1.Column()==matr2.Row());
	CMatrix<TYPE> pret(matr1.Row(), matr2.Column());
	for(int i=0;i<matr1.Row();i++)
		for(int j=0;j<matr2.Column();j++)
			for(int k=0;k<matr1.Column();k++)
				pret[i][j]+=matr1[i][k]*matr2[k][j];
	return pret;
}

template<class TYPE>
CVector<TYPE>	operator * (CMatrix<TYPE>& matr , CVector<TYPE>& v)
{
	ASSERT( matr.m_ColumnNum == v.GetSize() && v.GetSize() > 0 && matr.m_RowNum > 0 );

	CVector<TYPE>	vv( matr.m_RowNum );

	for(int i = 0; i < vv.GetSize(); i++)
	{
		vv[i] = 0;
		for(int j = 0; j < v.GetSize(); j++)
		{
			vv[i] += matr.m_Data[i][j] * v[j];
		}
	}

	return vv;
}

template<class TYPE>
CVector<TYPE>	operator * (CVector<TYPE>& v , CMatrix<TYPE>& matr)
{
	assert( matr.m_RowNum == v.GetSize() && v.GetSize() > 0 && matr.m_ColumnNum > 0 );

	CVector<TYPE>	vv( matr.m_ColumnNum );

	for(int i = 0; i < vv.GetSize(); i++)
	{
		vv[i] = 0;
		for(int j = 0; j < v.GetSize(); j++)
		{
			vv[i] += matr.m_Data[j][i] * v[j];
		}
	}

	return vv;
}

//#endif

//===============================================================================
// 4. 求反余弦函数
inline double getacos(double value)
{
	if(value - TOLER <= -1.0)
		return PI;
	if(value + TOLER >= 1.0)
		return 0.0;
	if(fabs(value) < TOLER)
		return PI_2;
	return acos(value);
}



//===============================================================================
// 5. 求反正弦函数
inline double getasin(double value)
{
	if(value - TOLER <= -1.0)
		return PI*1.5;
	if(value + TOLER >= 1.0)
		return PI_2;
	if(fabs(value) < TOLER)
		return 0;
	return asin(value);
}

#endif
