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
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
using namespace std;
int fun1();
int fun2();
int fun3();
int fun4();

bool fun4_connected(string, string, map<string, string>);
int fun5();
int fun6(int, char**);
int fun7();
int fun8();

/*
 * 
 */
int main(int argc, char** argv) {
    fun8();
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
            prevAOI = "";
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

    ifstream inputfile("/home/chili/Desktop/DF_Dwells_Whitespace");

    if (!inputfile.is_open()) {
        cout << "Error opening input file";
        return -1;
    }

    ofstream outputfile("/home/chili/Desktop/DF_Triangles_Collapse");
    if (!outputfile.is_open()) {
        cout << "Error opening output file";
        return -1;
    }
    outputfile << "Participant,Trial,Condition,Triangle,Duration\n";
    getline(inputfile, line);
    string participant = "", trial = "", condition;
    list<string> prevAOIs;
    list<int> durations;
    int lineIndex = 1;
    string prevTriangle;
    while (getline(inputfile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        if (fields[1].compare("8") == 0)
            continue;
        if (fields[6].length() == 0) {
            prevAOIs.clear();
            durations.clear();
            continue;
        }
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
            durations.push_back(boost::lexical_cast<int>(fields[4]));
            lineIndex++;
            continue;
        }
        if (!fun4_connected(AOI, prevAOIs.back(), expandMap[ boost::lexical_cast<int>(trial) - 1])) {
            durations.clear();
            prevAOIs.clear();
        }
        prevAOIs.push_back(AOI);
        durations.push_back(boost::lexical_cast<int>(fields[4]));
        if (prevAOIs.size() > 2) {
            vector<string> seq;
            vector<int> seqDur;
            string pAOI;
            int pDuration = 0;
            list<int>::reverse_iterator ritDur;
            list<string>::reverse_iterator rit;
            for (rit = prevAOIs.rbegin(),
                    ritDur = durations.rbegin();
                    rit != prevAOIs.rend(); ++rit, ++ritDur) {

                string tmp = *rit;
                int tmpDur = *ritDur;

                if (pAOI.empty()) {
                    if (tmp.length() > 1) {
                        seq.push_back(tmp);
                        seqDur.push_back(tmpDur);
                    }
                    pAOI = tmp;
                    pDuration = tmpDur;
                    continue;
                }
                /*Beam-joint case or beam beam case*/
                if (tmp.length() > 1) {
                    seq.push_back(tmp);
                    if (pAOI.length() == 1)
                        seqDur.push_back(tmpDur + pDuration);
                    else
                        seqDur.push_back(tmpDur);
                } else if (tmp.length() == 1 && pAOI.length() == 1) {
                    string entry;
                    if (tmp.compare(pAOI) < 0)
                        entry = tmp + pAOI;
                    else
                        entry = pAOI + tmp;
                    string expansion = expandMap[ boost::lexical_cast<int>(trial) - 1].operator[](entry);
                    boost::split(fields, expansion, boost::is_any_of(":"));
                    for (int i = 0; i < fields.size(); i++) {
                        seq.push_back(fields[i]);
                        seqDur.push_back(tmpDur + pDuration);
                        tmpDur = 0;
                        pDuration = 0;
                    }
                    pDuration = 0;
                } else
                    pDuration = tmpDur;

                pAOI = tmp;
            }

            for (map<string, string>::iterator item = triangleMaps[boost::lexical_cast<int>(trial) - 1].begin();
                    item != triangleMaps[boost::lexical_cast<int>(trial) - 1].end(); item++) {
                boost::split(fields, item->second, boost::is_any_of(":"));
                /*Check for triangles*/
                if (seq.size() < fields.size())
                    continue;
                bool is_valid;
                int durationTriangle = 0;
                for (int i = 0; i < fields.size(); i++) {
                    is_valid = false;
                    for (int j = 0; j < fields.size(); j++)
                        if (seq[j].compare(fields[i]) == 0) {
                            durationTriangle += seqDur[j];
                            is_valid = true;
                        }
                    if (!is_valid)
                        break;
                }
                if (is_valid) {
                    if (prevTriangle.empty() || prevTriangle.compare(item->first) != 0)
                        outputfile << participant << "," << trial << "," << condition << "," << item->first << "," << durationTriangle << "\n";
                    prevAOIs.clear();
                    prevTriangle = item->first;
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

int fun5() {

    ifstream inputfile("/home/chili/Desktop/DF_Dwells_Whitespace");

    if (!inputfile.is_open()) {
        cout << "Error opening input file";
        return -1;
    }

    ofstream outputfile("/home/chili/Desktop/DF_Joints_Entries");
    if (!outputfile.is_open()) {
        cout << "Error opening output file";
        return -1;
    }
    outputfile << "Participant,Trial,Condition,AOI,Entries\n";
    string line;
    getline(inputfile, line);
    vector<string> fields;
    string participant = "", trial = "", condition, prevAOI;
    int length = 0;
    map<string, int> entries;
    while (getline(inputfile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        if (participant == "") {
            participant = fields[0];
            trial = fields[1];
            condition = fields[2];
        }
        if (participant != fields[0] || trial != fields[1]) {
            for (map<string, int>::iterator it = entries.begin();
                    it != entries.end(); it++)
                outputfile << participant << "," << trial << "," << condition << "," << it->first << "," << it->second << "\n";
            participant = fields[0];
            trial = fields[1];
            condition = fields[2];
            prevAOI = "";
            entries.clear();
        }
        /************************/
        if (fields[6].find("Joint") != string::npos) {
            string joint = fields[6].substr(5, 1);
            if (prevAOI.find(joint) != string::npos) {
                if (entries.count(joint) == 0)
                    entries[joint] = 1;
                else
                    entries[joint] = entries[joint] + 1;
                prevAOI = "";

            } else
                prevAOI = fields[6];

        } else if (prevAOI.find("Joint") != string::npos) {
            string joint = prevAOI.substr(5, 1);
            if (fields[6].find(joint) != string::npos) {
                if (entries.count(joint) == 0)
                    entries[joint] = 1;
                else
                    entries[joint] = entries[joint] + 1;
                prevAOI = "";
            } else
                prevAOI = fields[6];

        } else
            prevAOI = fields[6];

    }
    inputfile.close();
    outputfile.close();

}

int fun6(int argc, char** argv) {

    ifstream input(argv[1]);

    if (!input.is_open()) {
        cout << "Error open input file";
        return -1;
    }

    ofstream output("/home/chili/Desktop/DF_Duration", ios_base::app);
    if (!output.is_open()) {
        cout << "Error open output file";
        return -1;
    }
    vector<string> fields;
    string line;

    boost::split(fields, argv[1], boost::is_any_of("-"));
    line = fields[0];
    boost::split(fields, line, boost::is_any_of("_"));

    string participant = fields[1];
    getline(input, line);

    while (getline(input, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        int start = boost::lexical_cast<int>(fields[1]);
        int stop = boost::lexical_cast<int>(fields[2]);
        output << participant << "," << argv[2] << "," << fields[0] << "," << stop - start << "\n";
    }
    output.close();
    input.close();






}

//Saliency map per partciipant

int fun7() {
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    map<int, string> beams, joints;
    int from_to[] = {0, 0, 1, 1, 2, 2, 3, 3};
    ifstream inputfile("/home/chili/Desktop/Saliency");

    if (!inputfile.is_open()) {
        cout << "Error opening input file";
        return -1;
    }
    vector<cv::Mat> AOIImages;
    vector<cv::Mat> refImage;
    for (int i = 0; i < 7; i++) {
        string ref = "/home/chili/Desktop/Models/";
        string index = boost::lexical_cast<string>(i + 1);
        ref = ref + index + ".JPG";
        string AOI = "/home/chili/Desktop/Models/";
        AOI = AOI + index + ".AOI.BMP";
        refImage.push_back(cv::imread(ref));
        AOIImages.push_back(cv::imread(AOI));
    }

    ifstream inputAOICode("/home/chili/Desktop/Models/AOI_codes.csv");
    if (!inputAOICode.is_open()) {
        cerr << "aoicode file open failed:";
        return -1;
    }

    string line;
    while (getline(inputAOICode, line)) {
        vector <std::string> fields;
        boost::split(fields, line, boost::is_any_of(","));
        string aoiName = fields[0];
        //cout << fields[1] << " " << fields[2] << "\n";
        int r = boost::lexical_cast<int>(fields[1]);
        int b = boost::lexical_cast<int>(fields[2]);
        //Beam
        if (r > 0)
            beams[r] = aoiName;
        else
            joints[b] = aoiName;
    }
    inputAOICode.close();



    getline(inputfile, line);
    vector<string> fields;
    string trial, participant, condition;
    int trial_index;
    cv::Mat currImg;
    while (getline(inputfile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        //Participant,Trial,Condition,Learner,primary.AOI,Dwell,Tot
        if (fields[1].compare("8") == 0)
            continue;
        /**********/
        if (participant.empty()) {
            participant = fields[0];
            trial = fields[1];
            trial_index = boost::lexical_cast<int>(trial) - 1;
            condition = fields[2];
            currImg = cv::Mat(refImage[trial_index].rows, refImage[trial_index].cols, CV_8UC3);
        }
        if (participant != fields[0] || trial != fields[1]) {
            string filename = "/home/chili/Desktop/" + participant + "-" + trial + "-saliency.png";
            cv::imwrite(filename, currImg, compression_params);
            participant = fields[0];
            trial = fields[1];
            condition = fields[2];
            trial_index = boost::lexical_cast<int>(trial) - 1;
            currImg = cv::Mat(refImage[trial_index].rows, refImage[trial_index].cols, CV_8UC3);
        }
        /************/



        for (int i = 0; i < currImg.rows; i++)
            for (int j = 0; j < currImg.cols; j++) {
                cv::Vec3b intensity = AOIImages[trial_index].at<cv::Vec3b>(i, j);
                cv::Vec3b truePixel = refImage[trial_index].at<cv::Vec3b>(i, j);
                int blue = (int) intensity.val[0];
                int red = (int) intensity.val[2];
                string pixelAOI;
                if (red > 0 && red < 255) {
                    pixelAOI = beams[red];
                } else if (blue > 0 && blue < 255) {
                    pixelAOI = joints[blue];
                }
                if (pixelAOI.empty()) {
                    cv::Vec3b newIntensity(truePixel.val[0], truePixel.val[1],
                            truePixel.val[2]); //cv::saturate_cast<uchar>(dwells/tot * UCHAR_MAX/2)+UCHAR_MAX/2);
                    currImg.at<cv::Vec3b>(i, j) = newIntensity;

                }

                if (pixelAOI.compare(fields[4]) == 0) {
                    float dwells = boost::lexical_cast<float>(fields[5]);
                    float tot = boost::lexical_cast<float>(fields[6]);
                    cv::Vec3b newIntensity(0, 0, 0);
                    float ratio = dwells / tot;
                    if (ratio < 0.5) {
                        newIntensity[0] = UCHAR_MAX * (1 - ratio) / 0.5;
                        newIntensity[1] = UCHAR_MAX * (ratio) / 0.5;
                    } else {
                        newIntensity[1] = UCHAR_MAX * (1 - ratio - 0.5) / 0.5;
                        newIntensity[2] = UCHAR_MAX * (ratio - 0.5) / 0.5;
                    }
                    currImg.at<cv::Vec3b>(i, j) = newIntensity;
                }
            }
    }
}

int fun8() {
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    map<int, string> beams, joints;
    ifstream inputfile("/home/chili/Desktop/Saliency_Learner");

    if (!inputfile.is_open()) {
        cout << "Error opening input file";
        return -1;
    }
    vector<cv::Mat> AOIImages;
    vector<cv::Mat> refImage;
    for (int i = 0; i < 7; i++) {
        string ref = "/home/chili/Desktop/Models/";
        string index = boost::lexical_cast<string>(i + 1);
        ref = ref + index + ".JPG";
        string AOI = "/home/chili/Desktop/Models/";
        AOI = AOI + index + ".AOI.BMP";
        refImage.push_back(cv::imread(ref));
        AOIImages.push_back(cv::imread(AOI));
    }

    ifstream inputAOICode("/home/chili/Desktop/Models/AOI_codes.csv");
    if (!inputAOICode.is_open()) {
        cerr << "aoicode file open failed:";
        return -1;
    }

    string line;
    while (getline(inputAOICode, line)) {
        vector <std::string> fields;
        boost::split(fields, line, boost::is_any_of(","));
        string aoiName = fields[0];
        //cout << fields[1] << " " << fields[2] << "\n";
        int r = boost::lexical_cast<int>(fields[1]);
        int b = boost::lexical_cast<int>(fields[2]);
        //Beam
        if (r > 0)
            beams[r] = aoiName;
        else
            joints[b] = aoiName;
    }
    inputAOICode.close();



    getline(inputfile, line);
    vector<string> fields;
    string trial, condition;
    int trial_index;
    cv::Mat currImg;
    while (getline(inputfile, line)) {
        boost::split(fields, line, boost::is_any_of(","));
        //Trial,Condition,primary.AOI,prop,max_prop,sum_prop,min_prop
        if (fields[0].compare("8") == 0)
            continue;
        /**********/
        if (trial.empty()) {
            trial = fields[0];
            trial_index = boost::lexical_cast<int>(trial) - 1;
            condition = fields[1];
            currImg = cv::Mat(refImage[trial_index].rows, refImage[trial_index].cols, CV_8UC3);
        }
        if (trial.compare(fields[0]) != 0 || condition.compare(fields[1]) != 0) {
            string filename = "/home/chili/Desktop/" + trial + "-" + condition + "-saliency.png";
            cv::imwrite(filename, currImg, compression_params);
            trial = fields[0];
            condition = fields[1];
            trial_index = boost::lexical_cast<int>(trial) - 1;
            currImg = cv::Mat(refImage[trial_index].rows, refImage[trial_index].cols, CV_8UC3);
        }
        /************/



        for (int i = 0; i < currImg.rows; i++)
            for (int j = 0; j < currImg.cols; j++) {
                cv::Vec3b intensity = AOIImages[trial_index].at<cv::Vec3b>(i, j);
                cv::Vec3b truePixel = refImage[trial_index].at<cv::Vec3b>(i, j);
                int blue = (int) intensity.val[0];
                int red = (int) intensity.val[2];
                string pixelAOI;
                if (red > 0 && red < 255) {
                    pixelAOI = beams[red];
                } else if (blue > 0 && blue < 255) {
                    pixelAOI = joints[blue];
                }
                if (pixelAOI.empty()) {
                    cv::Vec3b newIntensity(truePixel.val[0], truePixel.val[1],
                            truePixel.val[2]); //cv::saturate_cast<uchar>(dwells/tot * UCHAR_MAX/2)+UCHAR_MAX/2);
                    currImg.at<cv::Vec3b>(i, j) = newIntensity;

                }

                if (pixelAOI.compare(fields[2]) == 0) {
                    float prop = boost::lexical_cast<float>(fields[3]);
                    float max = boost::lexical_cast<float>(fields[4]);
                    float sum = boost::lexical_cast<float>(fields[5]);
                    float min = boost::lexical_cast<float>(fields[6]);

                    cv::Vec3b newIntensity(0, 0, 0);
                    float ratio = (prop-min) / (max-min);
                    if (ratio < 0.5) {
                        newIntensity[0] = UCHAR_MAX * (1 - ((ratio) / 0.5));
                        newIntensity[1] = UCHAR_MAX * (ratio) / 0.5;
                    } else {
                        newIntensity[1] = UCHAR_MAX * (1 - ((ratio - 0.5) / 0.5));
                        newIntensity[2] = UCHAR_MAX * (ratio - 0.5) / 0.5;
                    }
                    currImg.at<cv::Vec3b>(i, j) = newIntensity;
                }
            }
    }
    string filename = "/home/chili/Desktop/" + trial + "-" + condition + "-saliency.png";
    cv::imwrite(filename, currImg, compression_params);
}



