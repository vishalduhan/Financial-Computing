
#ifndef Stock_h
#define Stock_h
#include <string>
#include <vector>
#include <map>

using namespace std;
class Stock{
public:
    string StartDate;
    string EndDate;
    string AnnouncementDate;
    string Estimate;
    string Reported;
    string Surprise;
    vector<double> AdjClose;
    Stock(string Announce,string estimate,string reported,string surprise){AnnouncementDate=Announce;Estimate=estimate;Reported=reported;Surprise=surprise;}
    Stock(){}
    
    double Return=0.0;
    double AbnormalReturn=0.0;
};
typedef map<string,Stock> StockGroup;

#endif /* Stock_h */
