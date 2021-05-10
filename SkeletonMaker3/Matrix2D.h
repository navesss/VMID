////////////////////////////////////////////////////////////
// Matrix2D.h
#pragma once


// 2D Matrix with the size m_size*m_size
class Matrix2D 
{
public:
	float	**m_mat;
	int		m_size;
public:
	Matrix2D();
	Matrix2D(int n);
	~Matrix2D();

	// output matrix
	bool toFile(char* fname);
};