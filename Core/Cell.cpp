// PolygonalMesh.cpp: PolygonalMesh 
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "stdafx.h"
#include "cell.h"
#include "volume.h"
#include "Material.h"
#include "Utils3d.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

CCell::CCell()
{
	m_vertices.clear();
	m_faces.clear();
}

CCell::CCell(Volume *vol)
{
	
}

CCell::~CCell()
{
	m_vertices.clear();

	int i;
	for(i=0; i<m_faces.size(); i++)
		delete m_faces[i];
	m_faces.clear();
}

void CCell::normalizeScale(Point3D center, float scale)
{
	int i, n1;

	n1 = m_vertices.size();

	for(i=0; i<n1; i++)
	{
		Point3D pt = m_vertices[i];
		pt = pt - center;

		m_vertices[i] = pt;
	}

}

void CCell::draw()
{

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glPolygonOffset(1.0,1.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);

	// draw center
	float r, g, b;

	//r = 100; g = 100; b = 100;
	//r = g = b = 150;
	r = 0; g = 200; b = 200;

	CMaterial material;
	material.SetAmbient((float)r/255,(float)g/255,(float)b/255,1);
	material.SetDiffuse((float)r/255,(float)g/255,(float)b/255,1);
	material.SetSpecular(0,0,0,1);
	material.SetEmission(0,0,0,1);
	material.SetShininess(0);

	::glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,material.GetAmbient());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material.GetDiffuse());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material.GetSpecular());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material.GetEmission());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,material.GetShininess());

	//float clr[4] = {(float)r/255,(float)g/255,(float)b/255, 0.2f};
	float clr[4] = {(float)r/255,(float)g/255,(float)b/255, 1.0f};
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHTING);
	glDepthFunc(GL_LEQUAL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, clr);

	int i;
	
	for(i=0; i<m_faces.size(); i++)
	{
		
		CCellFace *face = m_faces[i];
		Point3D pt[4];
		
		pt[0] = m_vertices[face->m_vts[0]];
		pt[1] = m_vertices[face->m_vts[1]];
		pt[2] = m_vertices[face->m_vts[2]];
		pt[3] = m_vertices[face->m_vts[3]];

		Vector3D v1 = pt[1] - pt[0];
		Vector3D v2 = pt[2] - pt[1];
		Vector3D nv = v2 - v1;
		nv.SetUnit();

		glColor3f(0.8f, 0.8f, 0.8f);
		glBegin(GL_POLYGON);
		glNormal3f(nv.x, nv.y, nv.z);
		glVertex3f(pt[0].x, pt[0].y, pt[0].z);
		glVertex3f(pt[1].x, pt[1].y, pt[1].z);
		glVertex3f(pt[2].x, pt[2].y, pt[2].z);
		glVertex3f(pt[3].x, pt[3].y, pt[3].z);
		glVertex3f(pt[0].x, pt[0].y, pt[0].z);
		glEnd();
	}


}

// this function is written from: 
// void toOFFCells2( char* fname, float thr )
CCell* vol2cell2(Volume *vol, float thr)
{
	int sizex, sizey, sizez;

	sizex = vol->getSizeX();
	sizey = vol->getSizeY();
	sizez = vol->getSizeZ();

	int i, j, k ;
	Volume* indvol = new Volume( sizex, sizey, sizez, -1 ) ;

	// Get number of cells to write
	int numverts = 0, numfaces = 0 ;
	for ( i = 0 ; i < sizex ; i ++ )
	{
		for ( j = 0 ; j < sizey ; j ++ )
		{
			for ( k = 0 ; k < sizez ; k ++ )
			{
				if ( vol->getDataAt( i, j, k ) >= thr )
				{
					indvol->setDataAt( i,j,k, numverts ) ;
					numverts ++ ;

					for ( int mi = 0 ; mi < 3 ; mi ++ )
					{
						int find = mi * 4 + 3 ;
						int isFace = 1 ;
						for ( int mj = 0 ; mj < 4 ; mj ++ )
						{
							int nx = i + sheetNeighbor[find][mj][0] ;
							int ny = j + sheetNeighbor[find][mj][1] ;
							int nz = k + sheetNeighbor[find][mj][2] ;

							if ( vol->getDataAt( nx, ny, nz ) < thr )
							{
								isFace = 0 ;
								break ;
							}
						}
						if ( isFace )
						{
							numfaces ++ ;
						}

						int eind = mi * 2 + 1 ;
						if ( vol->getDataAt( i + neighbor6[eind][0], j + neighbor6[eind][1], k + neighbor6[eind][2]) >= thr )
						{
							numfaces ++ ;
						}
					}
				}
			}
		}
	}

	//FILE* fin = fopen( fname, "w" ) ;
	//fprintf( fin, "OFF\n" ) ;
	//fprintf( fin, "%d %d 0\n", numverts, numfaces ) ;

	CCell *cell = new CCell;

	// Write vertices
	for ( i = 0 ; i < sizex ; i ++ )
	{
		for ( j = 0 ; j < sizey ; j ++ )
		{
			for ( k = 0 ; k < sizez ; k ++ )
			{
				if ( vol->getDataAt( i,j,k ) >= thr )
				{
					//fprintf( fin, "%d %d %d\n", i, j, k ) ;
					Point3D pt = Point3D(i, j, k);
					cell->m_vertices.push_back(pt);
				}
			}
		}
	}

	// Write faces
	int ct = 0 ;
	for ( i = 0 ; i < sizex ; i ++ )
	{
		for ( j = 0 ; j < sizey ; j ++ )
		{
			for ( k = 0 ; k < sizez ; k ++ )
			{
				if ( vol->getDataAt( i,j,k ) >= thr )
				{
					int thisvt = (int)( indvol->getDataAt( i,j,k ) );
					for ( int mi = 0 ; mi < 3 ; mi ++ )
					{
						int find = mi * 4 + 3 ;
						int isFace = 1 ;
						int vts[4] ;
						for ( int mj = 0 ; mj < 4 ; mj ++ )
						{
							int nx = i + sheetNeighbor[find][mj][0] ;
							int ny = j + sheetNeighbor[find][mj][1] ;
							int nz = k + sheetNeighbor[find][mj][2] ;

							vts[ mj ] = (int)( indvol->getDataAt( nx, ny, nz ) );

							if ( vol->getDataAt( nx, ny, nz ) < thr )
							{
								isFace = 0 ;
								break ;
							}
						}
						if ( isFace )
						{
							//fprintf( fin, "4 %d %d %d %d\n", vts[0], vts[1], vts[3], vts[2] ) ;
							CCellFace *face = new CCellFace(vts[0], vts[1], vts[3], vts[2]);
							cell->m_faces.push_back(face);
						}

						int eind = mi * 2 + 1 ;
						int mx = i + neighbor6[eind][0] ;
						int my = j + neighbor6[eind][1] ;
						int mz = k + neighbor6[eind][2] ;
						int vt = (int)( indvol->getDataAt( mx, my, mz ) );
						if ( vol->getDataAt( mx, my, mz ) >= thr )
						{
							//fprintf( fin, "4 %d %d %d %d\n", thisvt, thisvt, vt, vt ) ;
							CCellFace *face = new CCellFace(thisvt, thisvt, vt, vt);
							cell->m_faces.push_back(face);
						}
					}
				}
			}
		}
	}

	// free
	delete indvol ;

	return cell;

}


// this function is written from: 
// void toOFFCells( char* fname, float thr )
CCell* vol2cell(Volume *vol, float thr)
{
	int sizex, sizey, sizez;

	sizex = vol->getSizeX();
	sizey = vol->getSizeY();
	sizez = vol->getSizeZ();

	int i, j, k ;
	// Volume* indvol = new Volume( sizex, sizey, sizez, -1 ) ;

	// Get number of cells to write
	int numverts = 0, numfaces = 0 ;
	for ( i = 0 ; i < sizex ; i ++ )
	{
		for ( j = 0 ; j < sizey ; j ++ )
		{
			for ( k = 0 ; k < sizez ; k ++ )
			{
				if ( vol->getDataAt( i, j, k ) >= thr )
				{
					numverts += 8 ;
					for ( int mi = 0 ; mi < 6 ; mi ++ )
					{
						if ( vol->getDataAt( i + neighbor6[mi][0], j + neighbor6[mi][1], k + neighbor6[mi][2] ) < thr )
						{
							numfaces ++ ;
						}
					}
				}
			}
		}
	}

	//FILE* fin = fopen( fname, "w" ) ;
	//fprintf( fin, "OFF\n" ) ;
	//fprintf( fin, "%d %d 0\n", numverts, numfaces ) ;

	CCell *cell = new CCell;

	// Write vertices
	for ( i = 0 ; i < sizex ; i ++ )
	{
		for ( j = 0 ; j < sizey ; j ++ )
		{
			for ( k = 0 ; k < sizez ; k ++ )
			{
				if ( vol->getDataAt( i,j,k ) >= thr )
				{
					float ox = i - 0.5f ;
					float oy = j - 0.5f ;
					float oz = k - 0.5f ;

					for ( int mi = 0 ; mi < 2 ; mi ++ )
						for ( int mj = 0 ; mj < 2 ; mj ++ )
							for ( int mk = 0 ; mk < 2 ; mk ++ )
							{
								//fprintf( fin, "%f %f %f\n", ox + mi, oy + mj, oz + mk ) ;

								Point3D pt = Point3D(ox + mi, oy + mj, oz + mk );
								cell->m_vertices.push_back(pt);
							}
				}
			}
		}
	}

	// Write faces
	int ct = 0 ;
	for ( i = 0 ; i < sizex ; i ++ )
	{
		for ( j = 0 ; j < sizey ; j ++ )
		{
			for ( k = 0 ; k < sizez ; k ++ )
			{
				if ( vol->getDataAt( i,j,k ) >= thr )
				{
					for ( int mi = 0 ; mi < 6 ; mi ++ )
					{
						if ( vol->getDataAt( i + neighbor6[mi][0], j + neighbor6[mi][1], k + neighbor6[mi][2] ) < thr )
						{
							//fprintf( fin, "4 %d %d %d %d\n", cubeFaces[mi][0] + ct, cubeFaces[mi][1] + ct, cubeFaces[mi][2] + ct, cubeFaces[mi][3] + ct ) ;

							CCellFace *face = new CCellFace(cubeFaces[mi][0] + ct, cubeFaces[mi][1] + ct, cubeFaces[mi][2] + ct, cubeFaces[mi][3] + ct);
							cell->m_faces.push_back(face);
						}
					}

					ct += 8 ;
				}
			}
		}
	}

//	fclose( fin ) ;
	//delete indvol ;

	return cell;
}