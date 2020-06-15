//
//  RetrieveData.h
//  Project_Final
//
//  Created by sanmuma on 2020/5/3.
//  Copyright © 2020 NYU. All rights reserved.
//

#ifndef RetrieveData_h
#define RetrieveData_h
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include "curl/curl.h"
#include "Stock.h"
using namespace std;

typedef map<string,Stock> StockGroup;

int write_data(void *ptr, int size, int nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}
struct MemoryStruct {
    char *memory;
    size_t size;
};
void *myrealloc(void *ptr, size_t size)
{    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}
int write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;
    mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}

string getTimeinSeconds(string Time)
{
    std::tm t = {0};
    std::istringstream ssTime(Time);
    char time[100];
    memset(time, 0, 100);
    if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
    {
        //cout << std::put_time(&t, "%c %Z") << "\n"
                  //<< std::mktime(&t) << "\n";
        sprintf (time, "%lld", mktime(&t));
        return string(time);
    }
    else
    {
        cout << "Parse failed\n";
        return "";
    }
}

int RetrieveData(StockGroup &Group)
{
    auto itr=Group.begin();
    
    struct MemoryStruct data;
    data.memory = NULL;
    data.size = 0;
    
    // declaration of an object CURL
    CURL *handle;
    
    CURLcode result;
    
    // set up the program environment that libcurl needs
    curl_global_init(CURL_GLOBAL_ALL);
    
    // curl_easy_init() returns a CURL easy handle
    handle = curl_easy_init();
    
    // file pointer to create file that store the data
    FILE *fp;
    string sCookies, sCrumb;
    if (handle)
    {
        
        if (sCookies.length() == 0 || sCrumb.length() == 0)
        {
            //curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
            curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
            curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
            curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

            curl_easy_setopt(handle, CURLOPT_ENCODING, "");

            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);

            // perform, then store the expected code in result
            result = curl_easy_perform(handle);

            if (result != CURLE_OK)
            {
                // if errors have occured, tell us what is wrong with result
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }

            char cKey[] = "CrumbStore\":{\"crumb\":\"";
            char *ptr1 = strstr(data.memory, cKey);
            char *ptr2 = ptr1 + strlen(cKey);
            char *ptr3 = strstr(ptr2, "\"}");
            if ( ptr3 != NULL )
                *ptr3 = NULL;

            sCrumb = ptr2;

            fp = fopen("cookies.txt", "r");
            char cCookies[100];
            if (fp) {
                while (fscanf(fp, "%s", cCookies) != EOF);
                fclose(fp);
            }
            else
                cerr << "cookies.txt does not exists" << endl;

            sCookies = cCookies;
        }
        
        while(true)
        {
            if (itr == Group.end())
            break;
            
            free(data.memory);
            data.memory = NULL;
            data.size= 0;
            
            string startTime = getTimeinSeconds(itr->second.StartDate+"T16:00:00");
            string endTime = getTimeinSeconds(itr->second.EndDate+"T16:00:00");
            
            string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
            string symbol = itr->first;
            string urlB = "?period1=";
            string urlC = "&period2=";
            string urlD = "&interval=1d&events=history&crumb=";
            string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
            const char * cURL = url.c_str();
            const char * cookies = sCookies.c_str();
            curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
            curl_easy_setopt(handle, CURLOPT_URL, cURL);
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
            result = curl_easy_perform(handle);

            if (result != CURLE_OK)
            {
                // if errors have occurred, tell us what is wrong with result
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            stringstream sData;
            sData.str(data.memory);
            
            
            
            
            string sValue;
            double dValue = 0;
            vector<double> adjs;
            string line;
            getline(sData, line);
            while ( getline(sData, line) ) {
                line.erase(line.find_last_of(','));
                sValue = line.substr(line.find_last_of(',')+1);
                dValue = strtod(sValue.c_str(), NULL);
                //cout << std::fixed << ::setprecision(6) << sValue << endl;
                adjs.push_back(dValue);
                
                
                
                
                
            }
            itr->second.AdjClose=adjs;
            
            
            
            itr++;
            
        }
        
        free(data.memory);
        data.size= 0;
        
    }
    
    else
    {
            fprintf(stderr, "Curl init failed!\n");
            return 1;
    }

    // cleanup since you've used curl_easy_init
    curl_easy_cleanup(handle);

    // release resources acquired by curl_global_init()
    curl_global_cleanup();
    
    return 0;
}


int RetrieveSPY(map<string,double>&SPY,vector<string>&datelist,string StartDate,string EndDate)
{
    
    struct MemoryStruct data;
    data.memory = NULL;
    data.size = 0;
    
    // declaration of an object CURL
    CURL *handle;
    
    CURLcode result;
    
    // set up the program environment that libcurl needs
    curl_global_init(CURL_GLOBAL_ALL);
    
    // curl_easy_init() returns a CURL easy handle
    handle = curl_easy_init();
    
    // file pointer to create file that store the data
    FILE *fp;
    string sCookies, sCrumb;
    if (handle)
    {
        
        if (sCookies.length() == 0 || sCrumb.length() == 0)
        {
            //curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
            curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
            curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
            curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

            curl_easy_setopt(handle, CURLOPT_ENCODING, "");

            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);

            // perform, then store the expected code in result
            result = curl_easy_perform(handle);

            if (result != CURLE_OK)
            {
                // if errors have occured, tell us what is wrong with result
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }

            char cKey[] = "CrumbStore\":{\"crumb\":\"";
            char *ptr1 = strstr(data.memory, cKey);
            char *ptr2 = ptr1 + strlen(cKey);
            char *ptr3 = strstr(ptr2, "\"}");
            if ( ptr3 != NULL )
                *ptr3 = NULL;

            sCrumb = ptr2;

            fp = fopen("cookies.txt", "r");
            char cCookies[100];
            if (fp) {
                while (fscanf(fp, "%s", cCookies) != EOF);
                fclose(fp);
            }
            else
                cerr << "cookies.txt does not exists" << endl;

            sCookies = cCookies;
        }
        
            
        free(data.memory);
        data.memory = NULL;
        data.size= 0;
            
        string startTime = getTimeinSeconds(StartDate+"T16:00:00");
        string endTime = getTimeinSeconds(EndDate+"T16:00:00");
            
        string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
        string symbol = "SPY";
        string urlB = "?period1=";
        string urlC = "&period2=";
        string urlD = "&interval=1d&events=history&crumb=";
        string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
        const char * cURL = url.c_str();
        const char * cookies = sCookies.c_str();
        curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
        curl_easy_setopt(handle, CURLOPT_URL, cURL);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
        result = curl_easy_perform(handle);

        if (result != CURLE_OK)
        {
                // if errors have occurred, tell us what is wrong with result
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
            return 1;
        }
        
        stringstream sData;
        sData.str(data.memory);
        string sValue,sDate;
        double dValue = 0;

        string line;
        getline(sData, line);
        while ( getline(sData, line) ) {
            sDate = line.substr(0, line.find_first_of(','));
            line.erase(line.find_last_of(','));
            sValue = line.substr(line.find_last_of(',')+1);
            dValue = strtod(sValue.c_str(), NULL);
            //cout << std::fixed << ::setprecision(6) << sValue << endl;
            datelist.push_back(sDate);
            SPY[sDate]=dValue;
        }
      
        free(data.memory);
        data.size= 0;
        
    }
    
    else
    {
            fprintf(stderr, "Curl init failed!\n");
            return 1;
    }

    // cleanup since you've used curl_easy_init
    curl_easy_cleanup(handle);

    // release resources acquired by curl_global_init()
    curl_global_cleanup();
    
    return 0;
}

























#endif /* RetrieveData_h */
