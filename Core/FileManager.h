 // FileManager.h
#pragma once

#include <stdio.h>
#include <string>


using namespace std;

class Volume;
class GLVisualizer;
class CMesh;

class FileManager
{

public: 
	FileManager();
	~FileManager();
	
	Volume* read_MRC(CString filename);
	Volume* read_SOF(CString filename);
	CMesh* read_OBJ(CString filename); // read triangle mesh

	void write_MRC(Volume *vol, CString filename);
	
	void write_VRML(GLVisualizer *gl, CString filename);
};

void load_obj(const char* filename, CMesh* mesh);

/////////////////////////////////////////////////////////////////////////
// useful function

// e.g. C:\BenchmarkFlex\056574-5820-ff0.mrc --> 056574-5820-ff0.mrc
string get_file_name(const string &name);

// e.g. 056574-5820-ff0.mrc --> mrc
string get_file_extension_name(const string &name);

// e.g. 056574-5820-ff0.mrc --> 056574-5820-ff0
string get_file_name2(const string &name);

// e.g. 056574-5820-ff0.mrc --> 056574
string get_group_name(const string &name);

// e.g. C:\BenchmarkFlex\056574-5820-ff0.mrc --> C:\BenchmarkProtein
string get_file_direction(const string &name);