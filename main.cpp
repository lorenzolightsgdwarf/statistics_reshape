/* 
 * File:   main.cpp
 * Author: chili
 *
 * Created on March 13, 2015, 2:59 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <set>
using namespace std;
int fun1();
int fun2();
int fun3();
int fun4();

bool fun4_connected(string, string, map<string, string>);

/*
 * 
 */
int main(int argc, char** argv) {
    fun4();
    return 0;
}

//Make a file with the ranking of the AOI

int fun1() {

    ifstream inputfile("/home/chili/Desktop/DF_Dwells");

    if (!inputfile.is_open()) {
        cout << "Error opening input file";
        return -1;
    }

    ofstream outputfile("/home/chili/Desktop/DF_Dwells_Rank");
    if (!outputfile.is_open()) {
        cout << "Error opening output file";
        return -1;
    }
    outputfile << "Participant,Trial,Condition,AOI,Rank\n";
    string line;
    getline(inputfile, line);
    set<string> prevAOI;
    vector<string> fields;
    string participant = "", trial = "", condition;
    int rank = 0;
    while (getline(inputfile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        if (participant == "") {
            participant = fields[0];
            trial = fields[1];
            condition = fields[2];
        }
        if (participant != fields[0] || trial != fields[1]) {
            participant = fields[0];
            trial = fields[1];
            condition = fields[2];
            rank = 0;
            prevAOI.clear();
        }
        if (prevAOI.count(fields[6]) == 0) {
            rank++;
            prevAOI.insert(fields[6]);
            outputfile << participant << "," << trial << "," << condition << "," << fields[6] << "," << rank << "\n";
        }
    }
    inputfile.close();
    outputfile.close();

}

//Make a file with the ranking of the AOI in the first 10 sec

int fun2() {

    ifstream inputfile("/home/chili/Desktop/DF_Dwells");

    if (!inputfile.is_open()) {
        cout << "Error opening input file";
        return -1;
    }

    ofstream outputfile("/home/chili/Desktop/DF_Dwells_Rank_10sec");
    if (!outputfile.is_open()) {
        cout << "Error opening output file";
        return -1;
    }
    outputfile << "Participant,Trial,Condition,AOI,Rank\n";
    string line;
    getline(inputfile, line);
    set<string> prevAOI;
    vector<string> fields;
    string participant = "", trial = "", condition;
    int rank = 0;
    while (getline(inputfile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        if (boost::lexical_cast<int>(fields[3]) > 10000)
            continue;
        if (participant == "") {
            participant = fields[0];
            trial = fields[1];
            condition = fields[2];
        }
        if (participant != fields[0] || trial != fields[1]) {
            participant = fields[0];
            trial = fields[1];
            condition = fields[2];
            rank = 0;
            prevAOI.clear();
        }
        if (prevAOI.count(fields[6]) == 0) {
            rank++;
            prevAOI.insert(fields[6]);
            outputfile << participant << "," << trial << "," << condition << "," << fields[6] << "," << rank << "\n";
        }
    }
    inputfile.close();
    outputfile.close();

}

int fun3() {

    ifstream inputfile("/home/chili/Desktop/DF_Dwells");

    if (!inputfile.is_open()) {
        cout << "Error opening input file";
        return -1;
    }

    ofstream outputfile("/home/chili/Desktop/DF_Dwells_Revisit_Joints");
    if (!outputfile.is_open()) {
        cout << "Error opening output file";
        return -1;
    }
    outputfile << "Participant,Trial,Condition,AOI,Length\n";
    string line;
    getline(inputfile, line);
    vector<string> fields;
    string participant = "", trial = "", condition, prevAOI;
    int length = 0;
    while (getline(inputfile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        if (participant == "") {
            participant = fields[0];
            trial = fields[1];
            condition = fields[2];
        }
        if (participant != fields[0] || trial != fields[1]) {
            participant = fields[0];
            trial = fields[1];
            condition = fields[2];
            length = 0;
        }
        if (fields[6].find("Joint") != string::npos) {
            if (prevAOI != fields[6]) {
                outputfile << participant << "," << trial << "," << condition << "," << fields[6] << "," << length << "\n";
            }
            length = 0;
        } else {
            length++;
        }
        prevAOI = fields[6];

    }
    inputfile.close();
    outputfile.close();

}

//Check for triangles patterns

int fun4() {

    //Load the triangles
    string line;
    vector<string> fields;

    vector< map<string, string> > triangleMaps, expandMap;
    for (int i = 0; i < 7; i++) {
        map<string, string> tmp;
        triangleMaps.push_back(tmp);
        map<string, string> tmp2;
        expandMap.push_back(tmp2);
    }
    ifstream trianglesFile("/home/chili/Desktop/Triangles.txt");
    if (!trianglesFile.is_open()) {
        cout << "Error opening triangles file";
        return -1;
    }
    getline(trianglesFile, line);
    while (getline(trianglesFile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        int trial = boost::lexical_cast<int>(fields[0]);
        triangleMaps[trial - 1].operator[](fields[1]) = fields[2];
    }
    trianglesFile.close();

    ifstream expansionFile("/home/chili/Desktop/Expansions.txt");
    if (!expansionFile.is_open()) {
        cout << "Error opening expansion file";
        return -1;
    }
    getline(expansionFile, line);
    while (getline(expansionFile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        int trial = boost::lexical_cast<int>(fields[0]);
        expandMap[trial - 1].operator[](fields[1]) = fields[2];
    }
    expansionFile.close();

    ifstream inputfile("/home/chili/Desktop/DF_Dwells");

    if (!inputfile.is_open()) {
        cout << "Error opening input file";
        return -1;
    }

    ofstream outputfile("/home/chili/Desktop/DF_Triangles");
    if (!outputfile.is_open()) {
        cout << "Error opening output file";
        return -1;
    }
    outputfile << "Participant,Trial,Condition,Triangle,Line\n";
    getline(inputfile, line);
    string participant = "", trial = "", condition;

    list<string> prevAOIs;
    int lineIndex = 1;
    while (getline(inputfile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        /**********/
        if (participant == "") {
            participant = fields[0];
            trial = fields[1];
            condition = fields[2];
        }
        if (participant != fields[0] || trial != fields[1]) {
            participant = fields[0];
            trial = fields[1];
            condition = fields[2];
        }
        /************/
        string AOI;
        if (fields[6].find("Joint") != string::npos)
            AOI = fields[6].substr(5, 1);
        else
            AOI = fields[6];

        if (prevAOIs.size() == 0) {
            prevAOIs.push_back(AOI);
            lineIndex++;
            continue;
        }
        if (!fun4_connected(AOI, prevAOIs.back(), expandMap[ boost::lexical_cast<int>(trial) - 1])) {
            prevAOIs.clear();
        }
        prevAOIs.push_back(AOI);
        if (prevAOIs.size() > 2) {
            vector<string> seq;
            string pAOI;
            for (list<string>::reverse_iterator rit = prevAOIs.rbegin(); rit != prevAOIs.rend(); ++rit) {
                string tmp = *rit;
                if (pAOI.empty()) {
                    if (tmp.length() > 1)
                        seq.push_back(tmp);
                    pAOI = tmp;
                    continue;
                }
                /*Beam-joint case or beam beam case*/
                if (tmp.length() > 1) {
                    seq.push_back(tmp);
                } else if (tmp.length() == 1 && pAOI.length() == 1) {
                    string entry;
                    if (tmp.compare(pAOI) < 0)
                        entry = tmp + pAOI;
                    else
                        entry = pAOI + tmp;
                    string expansion = expandMap[ boost::lexical_cast<int>(trial) - 1].operator[](entry);
                    boost::split(fields, expansion, boost::is_any_of(":"));
                    int collitions = 0;
                    for (int i = 0; i < fields.size(); i++) {
                        seq.push_back(fields[i]);
                    }
                }

                pAOI = tmp;
            }

            for (map<string, string>::iterator item = triangleMaps[boost::lexical_cast<int>(trial) - 1].begin();
                    item != triangleMaps[boost::lexical_cast<int>(trial) - 1].end(); item++) {
                boost::split(fields, item->second, boost::is_any_of(":"));
                /*Check for triangles*/
                if (seq.size() < fields.size())
                    continue;
                bool is_valid;
                for (int i = 0; i < fields.size(); i++){
                    is_valid=false;
                    for(int j=0;j<fields.size();j++)
                         if (seq[j].compare(fields[i])==0)
                            is_valid = true;
                    if(!is_valid)
                        break;
                }    
                if (is_valid){
                    outputfile << participant << "," << trial << "," << condition << "," << item->first << "\n";
                    prevAOIs.clear();
                }
            }

        }
        if (prevAOIs.size() > 20)
            prevAOIs.pop_front();
        lineIndex++;
    }
    inputfile.close();
    outputfile.close();

}

bool fun4_connected(string AOI, string prevAOI, map<string, string> expansion) {
    /****/
    if (AOI.length() == 1 && prevAOI.length() == 1) {
        string tmp;
        if (AOI.compare(prevAOI) < 0)
            tmp = AOI + prevAOI;
        else
            tmp = prevAOI + AOI;
        if (expansion.count(tmp) > 0)
            return true;
        else
            return false;

    } else if (AOI.length() == 1 && prevAOI.length() > 1) {
        if (prevAOI.find(AOI) != string::npos)
            return true;
        else
            return false;
    } else if (AOI.length() > 1 && prevAOI.length() == 1) {
        if (AOI.find(prevAOI) != string::npos)
            return true;
        else
            return false;
    } else {
        if (AOI.find(prevAOI.at(0)) != string::npos || AOI.find(prevAOI.at(1)) != string::npos)
            return true;
        else
            return false;
    }
}