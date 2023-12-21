#include <cmath>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>

int
main(int argc, char *argv[])
{
	using namespace std;

	string filename;
	filename = argv[1];
	ifstream file(filename.c_str());

	vector<float> X;

	float x;

	while (file>>x)
	{
		X.push_back(x);
	}

	file.close();

	FILE *fp;
	fp = fopen("X.bin", "w");
	fwrite(&X[0], sizeof(float), X.size(), fp);
	fclose(fp);

	return 0;

}
