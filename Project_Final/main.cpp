
#include <thread>
#include <iostream>
#include "GetGroup.h"
#include "RetrieveData.h"
#include "SelectRandomStocks.h"
#include "Calculation.h"
#include "PlotChart.h"


using namespace std;

int main(int argc, const char * argv[]) {
 /*
    Stock AAPL;
    Stock FL;
    FL.StartDate= "2019-04-01";
    FL.EndDate="2020-04-20";
    AAPL.StartDate= "2019-04-01";
    AAPL.EndDate="2020-04-20";
    StockGroup MyGroup;
    MyGroup["FDX"]=AAPL;
    MyGroup["ORCL"]=FL;
    RetrieveData(MyGroup);
    cout<<MyGroup["FDX"].AdjClose.size();
    return 0;
*/
    /*
    map<string,double>SPY;vector<string> datelist;
    RetrieveSPY(SPY,datelist,"2019-01-10","2020-04-01");
    map<string,double> spy=findSPYReturn(SPY);
    
    int N=50;
    
    
    string s1="/Users/sanmuma/Desktop/6883/Project_Final/MISS.csv";
    GetGroup Group1(s1);
    Group1.GroupPrepare(datelist, N);
   
    
    string s2="/Users/sanmuma/Desktop/6883/Project_Final/MEET.csv";
    GetGroup Group2(s2);
    Group2.GroupPrepare(datelist, N);
   
    
    string s3="/Users/sanmuma/Desktop/6883/Project_Final/BEAT.csv";
    GetGroup Group3(s3);
    Group3.GroupPrepare(datelist, N);
    
    
    
    std::thread t1(std::ref(RetrieveData),std::ref(Group1.Group));
    std::thread t2(std::ref(RetrieveData),std::ref(Group2.Group));
    std::thread t3(std::ref(RetrieveData),std::ref(Group3.Group));
    t1.join();
    t2.join();
    t3.join();
    
    Catch_Strange(Group1.Group, Group2.Group, Group3.Group);
    ///*
       for(auto it=Group1.Group.begin();it!=Group1.Group.end();it++){
        cout<<it->first<<endl;
        cout<<it->second.AnnouncementDate<<endl;
        cout<<it->second.StartDate<<endl;
        cout<<it->second.EndDate<<endl;
        cout<<it->second.Estimate<<endl;
        cout<<it->second.Surprise<<endl;
        cout<<it->second.Reported<<endl;
        cout<<it->second.AdjClose[it->second.AdjClose.size()-1]<<endl;
    }
    //*/
    

    
    
    /*
    Matrix Result(3, vector<vector<double>>(4));
    
        Result = finalCalculation(Group1.Group, Group2.Group, Group3.Group, spy, datelist, N);
        //Result = finalCalculation(Group3.Group, Group3.Group, Group3.Group, spy, datelist, N);
    
    
    const char* index[] = {"AAR", "AARSD", "CAAR", "CAARSD"};
    for (int i = 0; i < 4; i++) {
        plotChart(Result[2][i], Result[1][i], Result[0][i], N, index[i]);
    }
    
    
    /*
        bool cont = true;
        while (cont) {
            cout << "Choose group: " << endl;
            cout << " 0 for miss, 1 for beat, 2 for meet, 3 for exit" << endl;
            int groupselect;
            cin >> groupselect;
            if (groupselect == 3) { return 0; }
            cout << "Day == AAR == AAR-SD == CAAR == CAAR-SD " << endl;
            for (int i = 0; i < 2 * N; i++) {
                cout << setprecision(0) << i - N << " ";
                cout << setprecision(7) << Result[groupselect][0][i] << " " << Result[groupselect][1][i] << " " << Result[groupselect][2]
     [i] << " " << Result[groupselect][3][i] << endl;
            }
        }

    
    cout<<"done"<<endl;
    return 0;
//*/
    int opt;
    int N = 0;
    string choice1, choice2, choice3;
    string userticker;
    bool warning1 = false;
    bool warning2 = true, warning3 = true;
    map<string, double> SPY;
    vector<string> datelist;
    map<string, double> spy;
    const char* index[] = { "AAR", "AARSD", "CAAR", "CAARSD" };
    string s1 = "/Users/vishalduhan/Downloads/6883_Final_version/MISS.csv";
    GetGroup Group1(s1);
    string s2 = "/Users/vishalduhan/Downloads/6883_Final_version/MEET.csv";
    GetGroup Group2(s2);
    string s3 = "/Users/vishalduhan/Downloads/6883_Final_version/BEAT.csv";
    GetGroup Group3(s3);
    Matrix Result(3, vector<vector<double>>(4));
    
    while (true) {
        bool warning4 = true, warning5 = true;
        cout << "Please input the number to proceed..." << endl;
        cout << "1: Get historical price data of all stocks" << endl;
        cout << "2: Search Information for 1 specific stock" << endl;
        cout << "3: AAR, AAR-SD, CAAR, CAAR-SD for one group" << endl;
        cout << "4: Plot CAAR for all three groups" << endl;
        cout << "5: Exit" << endl<<endl;
        cin >> opt;
        switch (opt) {
        case 1:
        {
            while (warning4 && warning5)
            {
                cout << "Input days of prices {30 <= N <= 90}" << endl;
                cout << "Input 0 to back to menu" << endl;
                cout << "Days =";
                cin >> N;
                if (N == 0)
                {
                    warning5 = false;
                    break;
                }
                else {
                    if ((N >= 30) && (N <= 90))
                    /*if ((N >= 1) && (N <= 90))*/
                    {
                        warning4 = false;
                    }
                    else {
                        cout << "Wrong Input!" << endl;
                    }
                }
                cout << endl;
            }
    
            if (!warning5) { break; }
    
            RetrieveSPY(SPY, datelist, "2019-01-10", "2020-04-01");
            spy = findSPYReturn(SPY);
    
            Group1.GroupPrepare(datelist, N);
            //RetrieveData(Group1.Group);
    
            Group2.GroupPrepare(datelist, N);
            //RetrieveData(Group2.Group);
    
            Group3.GroupPrepare(datelist, N);
            //RetrieveData(Group3.Group);
            std::thread t1(std::ref(RetrieveData),std::ref(Group1.Group));
            std::thread t2(std::ref(RetrieveData),std::ref(Group2.Group));
            std::thread t3(std::ref(RetrieveData),std::ref(Group3.Group));
            t1.join();
            t2.join();
            t3.join();
    
            Catch_Strange(Group1.Group, Group2.Group, Group3.Group);
            Result = finalCalculation(Group1.Group, Group2.Group, Group3.Group, spy, datelist, N);
    
            cout << "Complete Data Load!" << endl;
            warning1 = true;
            break;
        }
        case 2:
        {
            if (warning1 == false) { cout << "You must get data first! Please enter 1" << endl << endl; break; }
            while (warning2)
            {
                cout << "Input ticker:";
                cin >> userticker;
                cout << endl;
                if ((Group1.Group.find(userticker) != Group1.Group.end())) {
                    cout << "Ticker:             " << userticker << endl;
                    cout << "Earnings Annountce Date: " << Group1.Group[userticker].AnnouncementDate << endl;
                    cout << "Actual EPS:         " << Group1.Group[userticker].Reported << endl;
                    cout << "Estimated EPS:      " << Group1.Group[userticker].Estimate << endl;
                    cout << "Surprise:           " << Group1.Group[userticker].Surprise << endl;
                    cout << "Group:              " << "MISS Group" << endl;
                    cout << endl;
                }
                else {
                    if ((Group2.Group.find(userticker) != Group2.Group.end())) {
                        cout << "Ticker:             " << userticker << endl;
                        cout << "Earnings Annountce Date: " << Group2.Group[userticker].AnnouncementDate << endl;
                        cout << "Actual EPS:         " << Group2.Group[userticker].Reported << endl;
                        cout << "Estimated EPS:      " << Group2.Group[userticker].Estimate << endl;
                        cout << "Surprise:           " << Group2.Group[userticker].Surprise << endl;
                        cout << "Group:              " << "MEET Group" << endl;
                        cout << endl;
                    }
                    else {
                        if ((Group3.Group.find(userticker) != Group3.Group.end())) {
                            cout << "Ticker:             " << userticker << endl;
                            cout << "Earnings Annountce Date: " << Group3.Group[userticker].AnnouncementDate << endl;
                            cout << "Actual EPS:         " << Group3.Group[userticker].Reported << endl;
                            cout << "Estimated EPS:      " << Group3.Group[userticker].Estimate << endl;
                            cout << "Surprise:           " << Group3.Group[userticker].Surprise << endl;
                            cout << "Group:              " << "BEAT Group" << endl;
                            cout << endl;
                        }
                        else {
                            cout << "No Data for Ticker!" << endl;
                        }
                    }
                }
                cout << "Press Y/y to Continue Search , N/n to Back)" << endl;
                cin >> choice1;
                if ((choice1 == "Y") || (choice1 == "y")) {
                    continue;
                }
                else {
                    if ((choice1 == "N") || (choice1 == "n")) {
                        warning2 = false;
                    }
                    else {
                        cout << "Wrong Input! Back to Main Menu..." << endl;
                        warning2 = false;
                    }
                }
            }
            break;
        }
        case 3:
        {
            if (warning1 == false) { cout << "You must get data first! Please enter 1" << endl << endl; break; }
            while (warning3)
            {
                cout << "Choose group: " << endl;
                cout << " 0 for miss, 1 for beat, 2 for meet" << endl;
                int groupselect;
                cin >> groupselect;
                if (groupselect != 1 && groupselect != 2 && groupselect != 0){
                    cout << "Invalid choice!" << endl;
                    break;
                }
                cout << "Day == AAR == AAR-SD == CAAR == CAAR-SD " << endl;
                for (int i = 0; i < 2 * N; i++) {
                    cout << setprecision(0) << i - N << " ";
                    cout << setprecision(7) << Result[groupselect][0][i] << " " << Result[groupselect][1][i] << " " << Result[groupselect][2][i] << " " << Result[groupselect][3][i] << endl << endl;
                }
    
                cout << "Press Y/y to Continue Search , N/n to Back)" << endl;
                cin >> choice2;
                if ((choice2 == "Y") || (choice2 == "y")) {
                    continue;
                }
                else {
                    if ((choice2 == "N") || (choice2 == "n")) {
                        warning3 = false;
                    }
                    else {
                        cout << "Wrong Input! Back to Main Menu..." << endl;
                        warning3 = false;
                    }
                }
            }
            break;
        }
        case 4:
        {
            if (warning1 == false) { cout << "You must get data first! Please enter 1" << endl << endl; break; }
            //plotChart(Result[2][0], Result[1][0], Result[0][0], N, index[0]);
            for (int i = 0; i < 4; i++) {
            plotChart(Result[2][i], Result[1][i], Result[0][i], N, index[i]);
            }
            break;
        }
        case 5:
        {
            cout << "Are you sure you want to exit (Y/y to confirm, N/n to cancel)" << endl;
            cin >> choice3;
            if ((choice3 == "Y") || (choice3 == "y")) {
                return 0;
            }
            else {
                cout << "Exit Unconfirmed. Not exiting." << endl;
                break;
            }
            break;
        }
        default:
        {
            cout << "Enter wrong number!" << endl;
            break;
        }
        };
    };
    return 0;
        //*/
    
    
    
}
