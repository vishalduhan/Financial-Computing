//
//  PlotChart.h
//  Project_Final
//
//  Created by sanmuma on 2020/5/7.
//  Copyright © 2020 NYU. All rights reserved.
//

#ifndef PlotChart_h
#define PlotChart_h

#include <vector>
#include <string>

using namespace std;

inline void plotChart(vector<double> beat, vector<double> meet, vector<double> miss,int N,const char* name) {
    FILE* gnuplotPipe, * tempDataFile;
    size_t dataSize = beat.size() - 1;
    const char* tempDataFileBeat = "Beat";
    const char* tempDataFileMeet = "Meet";
    const char* tempDataFileMiss = "Miss";

    double x1, y1, x2, y2, x3, y3;

    //set gnuplot enviorment
    gnuplotPipe = popen("/opt/local/bin/gnuplot", "w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "set title \"%s\"\n", name);
        fprintf(gnuplotPipe, "set grid\n");
        fprintf(gnuplotPipe, "set xlabel \"%s\"\n", "Days to report day");
        fprintf(gnuplotPipe, "set ylabel \"%s\"\n", name);
        fprintf(gnuplotPipe, "plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n", tempDataFileBeat, tempDataFileMeet, tempDataFileMiss);
        fflush(gnuplotPipe);
        //open beat file
        tempDataFile = fopen(tempDataFileBeat, "w");
        for (int i = 0; i <= dataSize; i++) {
            x1 = i - N;
            y1 = beat[i];
            fprintf(tempDataFile, "%lf %lf\n", x1, y1);
        }
        fclose(tempDataFile);
        //open meet file
        tempDataFile = fopen(tempDataFileMeet, "w");
        for (int i = 0; i <= dataSize; i++) {
            x2 = i - N;
            y2 = meet[i];
            fprintf(tempDataFile, "%lf %lf\n", x2, y2);
        }
        fclose(tempDataFile);
        //open miss file
        tempDataFile = fopen(tempDataFileMiss, "w");
        for (int i = 0; i <= dataSize; i++) {
            x3 = i - N;
            y3 = miss[i];
            fprintf(tempDataFile, "%lf %lf\n", x3, y3);
        }
        fclose(tempDataFile);
        
        printf("press enter to continue...");
        getchar();
   
        
        
        
        //delete 3 temporary files
        remove(tempDataFileBeat);
        remove(tempDataFileMeet);
        remove(tempDataFileMiss);

        fprintf(gnuplotPipe, "exit \n");
    }
    else {
        printf("gnuplot not found...");
    }
}



#endif
