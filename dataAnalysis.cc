#include "/public/read.h"
#include <cstdlib>
#include <iomanip>
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

	/*
	for (const auto &row : csv_file) { //output parse file
		for (const auto &col : row) {
			cout << col << '\t';
		}
		cout << endl;
	}
*/

	map<string, int> patientCount;
	for (const vector<string> &row : csv_file) {
		string timestamp = row.at(0);
		string data = timestamp.substr(0, 10); //takes the YEAR/MONTH/DAY  
		patientCount[data]++;
	}
	
	for (const pair<string, int> p : patientCount) {
		cout << "On " << p.first << ", the Dental saw " << p.second/2 << " patients." << endl;
	}
	cout << endl;
	cout << "----------------Busiest hours-----------------" << endl;


	map<string, int> busiestTime;
	int topOpening1 = 0;
	int topOpening2 = 0;
	int topOpening3 = 0;
	string hour1; 
	string hour2; 
	string hour3; 
	for (const vector<string> &row : csv_file) {
		string timestamp = row.at(0);
		string busyHour = timestamp.substr(11, 2); //takes only the hour
		busiestTime[busyHour]++;
	}
	for (const pair<string, int> p : busiestTime) {	
		if (p.second > topOpening1) {
			if (topOpening2 > topOpening3) {
				topOpening3 = topOpening2;
			}
			topOpening2 = topOpening1;
			topOpening1 = p.second;
			hour1 = p.first;
		} else if (p.second > topOpening2) {
			topOpening3 = topOpening2;
			topOpening2 = p.second;
			hour2 = p.first;
		} else if (p.second > topOpening3) {
			topOpening3 = p.second;
			hour3 = p.first;
		}
		//cout << p.first << " : " << p.second << endl; //for checking to see if top 3 is correct
	}
	cout << "The busiest hour at the Dental is " << hour1 << ":00 with " << topOpening1 << " patients coming in during this hour." << endl;
	cout << "The second busiest hour at the Dental is " << hour2 << ":00 with " << topOpening2 << " patients coming in during this hour." << endl;
	cout << "The third busiest hour at the Dental is " << hour3 << ":00 with " << topOpening3 << " patients coming in during this hour." << endl;

	cout << endl;
	cout << "--------------Slowest hours------------------" << endl;

    map<string, int> slowestTime;
    int botOpening1 = 1000000;
    int botOpening2 = 1000000;
    int botOpening3 = 1000000;
    string slowhour1;
    string slowhour2;
    string slowhour3;
    for (const vector<string> &row : csv_file) {
        string timestamp = row.at(0);
        string slowHour = timestamp.substr(11, 2); //takes only the hour
        slowestTime[slowHour]++;
    }
    for (const pair<string, int> p : slowestTime) {
        if (p.second < botOpening1) {
            if (botOpening2 < botOpening3) {
                botOpening3 = botOpening2;
            }
            botOpening2 = botOpening1;
            botOpening1 = p.second;
            slowhour1 = p.first;
        } else if (p.second < botOpening2) {
            botOpening3 = botOpening2;
            botOpening2 = p.second;
            slowhour2 = p.first;
        } else if (p.second < botOpening3) {
            botOpening3 = p.second;
            slowhour3 = p.first;
        }
       // cout << p.first << " : " << p.second << endl; //for checking to see if top 3 is correct
    }
    cout << "The slowest hour at the Dental is " << slowhour1 << ":00 with " << botOpening1 << " patients coming in during this hour." << endl;
    cout << "The second slowest hour at the Dental is " << slowhour2 << ":00 with " << botOpening2 << " patients coming in during this hour." << endl;
    cout << "The third slowest hour at the Dental is " << slowhour3 << ":00 with " << botOpening3 << " patients coming in during this hour." << endl;













}
