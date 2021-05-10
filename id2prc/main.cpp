#include <stdio.h>
#include <list>
#include <vector>
#include <map>
#include <string>

using namespace std;

typedef struct {
	int ii, jj;
	float diff;
	char same;
	float sen, spe; // spe: recall, sen: precision
} group;

map<string, string> grps;	//protein => group
map<string, int> res;		//protein => residue size
map<int, int> yes;
vector<string> proteins;
float* matrix;
group* result;
int total;

int diffCmp(const void* p1, const void* p2) {
	const group *item1 = (const group*)p1;
	const group *item2 = (const group*)p2;
	if (item1->diff <  item2->diff) return -1;
	if (item1->diff == item2->diff) return  0;
	return 1;
}

int main(int argc, char*argv[]) {
	int i, j;
	char* pMatrix = "evhmatrix.txt";
	if (argc>1) pMatrix = argv[1];
	
	string IndexDir	= string("C:\\BenchmarkFlex\\");

	string				filename1, filename2, filename3, filename4;

	filename1 = IndexDir + string("proteins.txt");
	filename2 = IndexDir + string("types.txt");
	filename3 = IndexDir + string("evhmatrix.txt");
	filename4 = IndexDir + string("PR.txt");

	// pdbid group residue-size
	FILE * ft = fopen(filename2.c_str(), "r");
	if (!ft) { printf("Can't open types.txt.\n"); return -1; }
	char id[128], type[128]; int size;
	while (!feof(ft)) {
		if (fscanf(ft, "%s %s %d", id, type, &size) == 3) {
			grps[id] = type; res[id] = size;
		}
	}
	fclose(ft);

	// proteins
	total = 0;
	FILE * fp = fopen(filename1.c_str(), "r");
	if (!fp) { printf("Can't open types.txt.\n"); return -1; }
	while (!feof(fp)) {
		if (fscanf(fp, "%s", id) == 1) {
			proteins.push_back(id);
			total ++;
		}
	}
	fclose(ft);

	matrix = (float*)malloc(sizeof(float)*total*total);
	if (!matrix) return -1;

	// read big matrix
	FILE * fm = fopen(filename3.c_str(), "r");
	for (i=0; i<total*total; i++) {
		float f;
		int ret = fscanf(fm, "%f", &f);
		if (ret != 1 || f< 0) { printf("Data error.\n"); return -1; }
		matrix[i] = f;
	}
	fclose(fm);

	// result matrix
	result = (group*)malloc(sizeof(group)*total*total);
	memset(result, 0, sizeof(group)*total*total);
	if (!result) return -1;
	int ii=0, jj=0;
	for (i=0; i<total; i++) {
		string x = grps[proteins[i]];
		if (!x.length()) continue;
		for (j=0; j<total; j++) {
			string y = grps[proteins[j]];
			if (!y.length()) continue;
			result[ii*total+jj].ii	= i;
			result[ii*total+jj].jj	= j;
			result[ii*total+jj].diff = matrix[i*total+j];
			result[ii*total+jj].same = (x==y)?'Y':'N';
			if (ii == jj && (x!=y)) {
				{ printf("Matrix2.\n"); return -1; }
			}
			if (result[ii*total+jj].diff < 0) { printf("Matrix3.\n"); return -1; }
			jj++;
		} 
		result[ii*total+ii].diff = -1;
		ii++; jj=0;
	}

	group *s = new group[total];

	// sort && count #yes
	for (int ix=0; ix<total; ix++) {
		memcpy(s, result+ix*total, sizeof(group)*total);
		qsort(s, total, sizeof(group), diffCmp);
		memcpy(result+ix*total, s, sizeof(group)*total);
		if (result[ix*total].same != 'Y')
			printf("%d?\n", ix);
		int sum = 0;
		for (j=0; j<total; j++) {
			if (result[ix*total+j].same == 'Y') sum++;
		} 
		yes[ix] = sum;
		printf("%d: %d\n", ix, yes[ix]);
	}

	// sen & spe
	for (i=0; i<total; i++) {
		int correct = 0;
		for (j=0; j<total; j++) {
			if (result[i*total+j].same == 'Y') correct++;
			result[i*total+j].sen = correct/(float)(j+1);
			result[i*total+j].spe = correct/(float)yes[i];
		}
	};
/*
	// sen & spe
	for (i=0; i<total; i++) {
		int correct = 0;
		float last_precision;
		for (j=0; j<total; j++) {
			if (result[i*total+j].same == 'Y') correct++;

			if(correct < yes[i])
			{
				result[i*total+j].sen = correct/(float)(j+1);
				result[i*total+j].spe = correct/(float)yes[i];

				last_precision = correct/(float)(j+1);
			}
			else
			{
				result[i*total+j].sen = last_precision;
				result[i*total+j].spe = 1;
			}
		}
	};
*/
	// average sen & spe
	FILE* fo=fopen(filename4.c_str(), "w");
	for (i=0; i<total; i++) {
		float sumSen = 0, sumSpe = 0;
		for (j=0; j<total; j++) {
			sumSen += result[i+j*total].sen;
			sumSpe += result[i+j*total].spe;
		}
		fprintf(fo, "%f\t%f\n", sumSen/total, sumSpe/total);
	}
	fclose(fo);

	// debug
	//FILE* fd=fopen("dump.txt", "w");
	//for (i=0; i<total; i++) {
	//	for (j=0; j<total; j++) {
	//		group* p = &result[i*total+j];
	//		fprintf(fd, "%f(%c:%f:%f)\n", p->diff, p->same, p->sen, p->spe);
	//		//fprintf(fd, "%d %d %s %s %f\n", i, j, proteins[p->ii].c_str(), proteins[p->jj].c_str(), p->diff);
	//	}
	//	 fprintf(fd, "\n");
	//}
	//fclose(fd);

	free(matrix);
	delete s;
	return 0;
}