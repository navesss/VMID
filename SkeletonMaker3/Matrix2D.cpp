
#include "Matrix2D.h"
#include <stdio.h>


Matrix2D::Matrix2D()
{
	m_mat =	NULL;
	m_size = 0;
}

Matrix2D::Matrix2D(int n)
{
	int i, j;
	m_size = n;

	m_mat = new float*[n];
	for(i=0; i<n; i++)
		m_mat[i] = new float[n];

	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			m_mat[i][j] = 0;
}

Matrix2D::~Matrix2D()
{
	int i;

	for(i=0; i<m_size; i++)
		delete m_mat[i];
	delete []m_mat;
}

bool Matrix2D::toFile(char* fname)
{
	FILE* fp = fopen( fname, "w" ) ;

	if(fp == NULL)
		return false;

	int i, j;
	for(i=0; i<m_size; i++)
	{
		for(j=0; j<m_size; j++)
		{
			float dis = m_mat[i][j];
			fprintf(fp, "%f ", dis);
		}
		fprintf(fp,"\n");
	}

	fclose(fp);

	return true;
}