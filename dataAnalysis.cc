#include "/public/read.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
using namespace std;

int main() {
	ifstream ins("DoorTrafficData.csv");

	if (!ins) {
		cout << "No file!" << endl;
		exit(1);
	}
	string line = readline(ins); //get rid of first line 
	vector<vector<string>> csv_file;

	while (ins) { //parsing file
		string line = readline(ins);
		if (!ins) break;
		istringstream iss(line);
		vector<string> row;
		while (iss) {
			string str = readline(iss, ',');
			if (!iss) break;
			row.push_back(str);
		}
		csv_file.push_back(row);
	}

	for (const auto &row : csv_file) { //output parse file
		for (const auto &col : row) {
			cout << col << '\t';
		}
		cout << endl;
	}

}
