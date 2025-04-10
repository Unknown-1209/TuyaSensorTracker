#pragama once
#include "/public/read.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <string>

using namespace std;

//we took in the file as a string so I made these functions to return ints
string getDateTime(const vector<string>& row) {
    return (row.size() > 0) ? row[0] : "";
}

string getOpenCloseStatus(const vector<string>& row) {
    return (row.size() > 1) ? row[1] : "";
}

float getCounter(const vector<string>& row) {
    return (row.size() > 2) ?  stof(row[2]) : 0;
}

float getnextDuration(const vector<string>& row) {//duration between one open to another
    return (row.size() > 3) ?  stof(row[3]) : 0;
}

float getDurations(const vector<string>& row) {
    return (row.size() > 4) ?  stof(row[4]) : 0;
}

vector<float> analysis(vector<vector<string>>& csv_file){

float totalOpens=0;  //these will all go into a vector of dataAnalysis, add to it whatever
float avgDuration=0;
float maxOpen=0;// these are durations for open and closed
float minOpen=3;
float totDuration;
int customer =0;

for(const auto& row: csv_file){   //basic loop to go through file
float opendoor = getDurations(row);
float heldopen = getnextDuration(row);
totalOpens= getCounter(row); //eventually we will reach the final count
if(getDurations(row)>0)totDuration += getDurations(row); // we need this to calc avg duration

if(maxOpen< getDurations(row)){maxOpen= getDurations(row);}
if(minOpen> getDurations(row)and getDurations(row)>2){minOpen= getDurations(row);}

if(opendoor>7 and heldopen>90){customer +=1;}// calculated that 7 was the average, this get accurate customer number 
}

avgDuration = totDuration/totalOpens;

return {avgDuration,maxOpen,minOpen,totalOpens,customer};
}
