
#ifndef GetGroup_h
#define GetGroup_h
#include <fstream>
#include "Stock.h"
using namespace std;

class GetGroup{
public:
    string FileName;
    StockGroup Group;
    GetGroup(string FileName);
    void GroupPrepare(vector<string> datelist,int N);
};

GetGroup::GetGroup(string filename){
    FileName=filename;
    ifstream MyFile(FileName);
    string line;
    //getline(MyFile,line);
    
    while (getline(MyFile,line)){
        string s1,s2;
        s1=line.substr(0,line.find_first_of(','));
        string temp=line.substr(line.find_first_of(',')+1);
        s2=temp.substr(0,temp.find_first_of(','));
        string s3;
        if (s2.size()==10){
            s3=s2.substr(6,4)+"-"+s2.substr(0,2)+"-"+s2.substr(3,2);}
        if (s2.size()==9&&s2.substr(0,1)=="1"){
            s3=s2.substr(5,4)+"-"+s2.substr(0,2)+"-0"+s2.substr(3,1);
        }
        if (s2.size()==9&&s2.substr(0,1)=="9"){
            s3=s2.substr(5,4)+"-0"+s2.substr(0,1)+"-"+s2.substr(2,2);
        }
        string temp2=temp.substr(temp.find_first_of(',')+1);
        string estimate=temp2.substr(0,temp2.find_first_of(','));
        string temp3=temp2.substr(temp2.find_first_of(',')+1);
        string reported=temp3.substr(0,temp3.find_first_of(','));
        string surprise=temp3.substr(temp3.find_first_of(',')+1);
        Group[s1]=Stock(s3,estimate,reported,surprise);
    }
}

void GetGroup::GroupPrepare(vector<string> datelist,int N){
    for(auto itr=Group.begin();itr!=Group.end();itr++){
        for (int i=0;i<datelist.size();i++){
            if (datelist[i]==itr->second.AnnouncementDate){
                itr->second.StartDate=datelist[i-N];
                itr->second.EndDate=datelist[i+N];
                continue;
            }
        }
    }
    
}


#endif /* GetGroup_h */
