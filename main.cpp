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
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <set>
using namespace std;
int fun1();
int fun2();

/*
 * 
 */
int main(int argc, char** argv) {
    fun2();
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
    string participant = "", trial = "",condition;
    int rank=0;
    while (getline(inputfile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        if(participant==""){
            participant=fields[0];
            trial=fields[1];
            condition=fields[2];
        }
        if(participant!=fields[0] || trial!=fields[1]){
            participant=fields[0];
            trial=fields[1];
            condition=fields[2];
            rank=0;
            prevAOI.clear();
        }
        if(prevAOI.count(fields[6])==0){
            rank++;
            prevAOI.insert(fields[6]);
            outputfile<<participant<<","<<trial<<","<<condition<<","<<fields[6]<<","<<rank<<"\n";
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
    string participant = "", trial = "",condition;
    int rank=0;
    while (getline(inputfile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        if(boost::lexical_cast<int>(fields[3])>10000)
            continue;
        if(participant==""){
            participant=fields[0];
            trial=fields[1];
            condition=fields[2];        
        }
        if(participant!=fields[0] || trial!=fields[1] ){
            participant=fields[0];
            trial=fields[1];
            condition=fields[2];
            rank=0;
            prevAOI.clear();
        }
        if(prevAOI.count(fields[6])==0){
            rank++;
            prevAOI.insert(fields[6]);
            outputfile<<participant<<","<<trial<<","<<condition<<","<<fields[6]<<","<<rank<<"\n";
        }        
    }
    inputfile.close();
    outputfile.close();

}