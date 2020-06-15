
#ifndef Calculation_h
#define Calculation_h
#include "Stock.h"
#include "SelectRandomStocks.h"
using namespace std;

typedef map<string, vector<double>> Return;
typedef vector <double> Vector;
typedef vector<vector<vector<double>>> Matrix;
typedef vector<vector<double>> Container;

int Catch_Strange(StockGroup& Group1, StockGroup& Group2, StockGroup& Group3)
{
    map<string, Stock> ::iterator itr;
    itr = Group1.begin();
    while (itr != Group1.end()) {
        if (itr->second.AdjClose.size() == 0) {
            Group1.erase(itr++);
        }
        else {
            ++itr;
        }
    }
    itr = Group2.begin();
    while (itr != Group2.end()) {
        if (itr->second.AdjClose.size() == 0) {
            Group2.erase(itr++);
        }
        else {
            ++itr;
        }
    }
    itr = Group3.begin();
    while (itr != Group3.end()) {
        if (itr->second.AdjClose.size() == 0) {
            Group3.erase(itr++);
        }
        else {
            ++itr;
        }
    }

    return 0;
}
//catch strange ignores value stock with value 0.0
Vector operator +(Vector& V1, const Vector& V2) {
    for (int i = 0; i < V2.size(); i++) {
        V1[i] = V1[i] + V2[i] / 30;
        //30 samples
    }
    return V1;
}

Vector operator+(Container& V1, Vector& V2) {
    for (int i = 0; i < V2.size(); i++) {
        for (int j = 0; j < V1.size(); j++) {
            V2[i] = V2[i] + V1[j][i] / 30;
            //30 samples
        }
    }
    return V2;
}

Container operator -(Container& V1, const Vector& V2) {
    for (int i = 0; i < V2.size(); i++) {
        for (int j = 0; j < V1.size(); j++) {
            V1[j][i] = V1[j][i] - V2[i];
        }
    }
    return V1;
}

Container operator ^(Container& V1, Container& V2) {
    for (int i = 0; i < V2[0].size(); i++) {
        for (int j = 0; j < V2.size(); j++) {
            V1[j][i] = V2[j][i] * V2[j][i];
        }
    }
    return V1;
}

Vector squareRootOperator(Vector& V1) {
    for (int i = 0; i < V1.size(); i++) {
        V1[i] = sqrt(V1[i]);
    }
    return V1;

}

map<string, double> findSPYReturn(map<string, double> spy) {
    map<string, double> SPYRETURN;
    vector<double> trans;
    for (map<string, double> ::iterator itr = spy.begin(); itr != spy.end(); itr++)
    {
        trans.push_back(itr -> second);
        SPYRETURN.insert(pair<string, double>(itr->first, 0.0));
    }
    int i = 0;
    for (map<string, double> ::iterator itr = spy.begin(); itr != spy.end(); itr++)
    {
        if (i == spy.size() - 1)
        {
            break;
        };
        double SPY1 = trans[i];
        double SPY2 = trans[i + 1];
        double SPY3 = (SPY2 - SPY1) / SPY1;
        SPYRETURN[itr->first] = SPY3;
        i = i + 1;
    }
    return SPYRETURN;
}
//Calculating the daily return of SPY^^
Return calculationOfReturn(Return abnormalReturn, StockGroup group, map<string, double> spy, vector<string> datelist, int N) {

    for (StockGroup::iterator itr = group.begin(); itr != group.end(); itr++) {
        vector<double> abnormal;
        vector<double> stockPrice = itr->second.AdjClose;
        string stockName = itr->first;

        string startDate = itr->second.StartDate;
        int startDateIndex = 0;
        for (int i = 0; i < datelist.size(); i++) {
            if (datelist[i] == startDate) {
                startDateIndex = i;
                break;
            }
        }

        for (int i = 0; i < 2 * N ; i++) {
            double one = stockPrice[i];
            double two = stockPrice[i + 1];
            double result = (two - one) / one;
            itr->second.Return = result;

            double SPY = spy[datelist[startDateIndex+i]];
            result = result - SPY;
            itr->second.AbnormalReturn = result;
            abnormal.push_back(result);
        }
        abnormalReturn.insert({ stockName,abnormal });

    }

    return abnormalReturn;

}
//Calculation of abnormal return AR(it)
Vector calculationOfAAR(Return abnormalReturn, int N) {
    Vector calculation(2 * N, 0.0);
    for (Return::iterator it = abnormalReturn.begin(); it != abnormalReturn.end(); it++) {
        for (int i = 0; i < 2 * N; i++) {
            calculation[i] += it->second[i] / 30;
            //30 stocks
        }
    }
    return calculation;

}

Vector calculationOfCAAR(Vector calculation, int N) {
    Vector calculationCAAR(2 * N, 0);
    calculationCAAR[0] = calculation[0];
    for (int i = 0; i < 2 * N - 1; i++) {
        calculationCAAR[i + 1] = calculationCAAR[i] + calculation[i + 1];
    }
    return calculationCAAR;
}

Matrix finalCalculation(StockGroup group1, StockGroup group2, StockGroup group3, map<string, double> spy, vector<string> datelist, int N) {
    vector<double> original(2 * N, 0.0);
    Vector AAR1(2 * N, 0.0);
    Vector AARSD1(2 * N, 0.0);
    Vector AAR2(2 * N, 0.0);
    Vector AARSD2(2 * N, 0.0);
    Vector AAR3(2 * N, 0.0);
    Vector AARSD3(2 * N, 0.0);
    Container temp11(30, original);
    Container temp21(30, original);
    Container temp12(30, original);
    Container temp22(30, original);
    Container temp31(30, original);
    Container temp32(30, original);
    Vector CAAR1(2 * N, 0.0);
    Vector CAARSD1(2 * N, 0.0);
    Vector CAAR2(2 * N, 0.0);
    Vector CAARSD2(2 * N, 0.0);
    Vector CAAR3(2 * N, 0.0);
    Vector CAARSD3(2 * N, 0.0);

    Matrix Result(3, vector<vector<double>>(4));

    Return abnormalReturnWhole1, abnormalReturnWhole2, abnormalReturnWhole3;
    abnormalReturnWhole1 = calculationOfReturn(abnormalReturnWhole1, group1, spy, datelist, N);
    abnormalReturnWhole2 = calculationOfReturn(abnormalReturnWhole2, group2, spy, datelist, N);
    abnormalReturnWhole3 = calculationOfReturn(abnormalReturnWhole3, group3, spy, datelist, N);

    for (int j = 0; j < 30; j++) {
        //30 samples
        StockShuffler shuffle1 = StockShuffler(group1);
        vector <string> choose_group1 = shuffle1.Get30StocksPerGroup();
        StockShuffler shuffle2 = StockShuffler(group2);
        vector <string> choose_group2 = shuffle2.Get30StocksPerGroup();
        StockShuffler shuffle3 = StockShuffler(group3);
        vector <string> choose_group3 = shuffle3.Get30StocksPerGroup();

        Return abnormalReturn1;
        Return abnormalReturn2;
        Return abnormalReturn3;

        for (vector<string>::iterator itr = choose_group1.begin(); itr != choose_group1.end(); itr++) {
            if (abnormalReturnWhole1.find(*itr) != abnormalReturnWhole1.end()) {
                string name = *itr;
                abnormalReturn1.insert({ name,abnormalReturnWhole1[name] });
            }
        }
        for (vector<string>::iterator itr = choose_group2.begin(); itr != choose_group2.end(); itr++) {
            if (abnormalReturnWhole2.find(*itr) != abnormalReturnWhole2.end()) {
                string name = *itr;
                abnormalReturn2.insert({ name,abnormalReturnWhole2[name] });
            }
        }
        for (vector<string>::iterator itr = choose_group3.begin(); itr != choose_group3.end(); itr++) {
            if (abnormalReturnWhole3.find(*itr) != abnormalReturnWhole3.end()) {
                string name = *itr;
                abnormalReturn3.insert({ name,abnormalReturnWhole3[name] });
            }
        }

        Vector averageAbnormalReturn1;
        averageAbnormalReturn1 = calculationOfAAR(abnormalReturn1, N);
        Vector averageAbnormalReturn2;
        averageAbnormalReturn2 = calculationOfAAR(abnormalReturn2, N);
        Vector averageAbnormalReturn3;
        averageAbnormalReturn3 = calculationOfAAR(abnormalReturn3, N);

        temp11[j] = averageAbnormalReturn1;
        AAR1 = operator+ (AAR1, averageAbnormalReturn1);
        temp21[j] = averageAbnormalReturn2;
        AAR2 = operator+ (AAR2, averageAbnormalReturn2);
        temp31[j] = averageAbnormalReturn3;
        AAR3 = operator+ (AAR3, averageAbnormalReturn3);

        Vector culmulativeAbnormalReturn1;
        Vector culmulativeAbnormalReturn2;
        Vector culmulativeAbnormalReturn3;
        culmulativeAbnormalReturn1 = calculationOfCAAR(averageAbnormalReturn1, N);
        culmulativeAbnormalReturn2 = calculationOfCAAR(averageAbnormalReturn2, N);
        culmulativeAbnormalReturn3 = calculationOfCAAR(averageAbnormalReturn3, N);
        temp12[j] = culmulativeAbnormalReturn1;
        CAAR1 = operator+(CAAR1, culmulativeAbnormalReturn1);
        temp22[j] = culmulativeAbnormalReturn2;
        CAAR2 = operator+(CAAR2, culmulativeAbnormalReturn2);
        temp32[j] = culmulativeAbnormalReturn3;
        CAAR3 = operator+(CAAR3, culmulativeAbnormalReturn3);
    }
//at the end of this loop aar will have last value
    temp11 = operator-(temp11, AAR1);
    temp11 = operator ^(temp11, temp11);
    temp21 = operator-(temp21, AAR2);
    temp21 = operator ^(temp21, temp21);
    temp31 = operator-(temp31, AAR3);
    temp31 = operator ^(temp31, temp31);
    AARSD1 = operator+(temp11, AARSD1);
    AARSD1 = squareRootOperator(AARSD1);
    AARSD2 = operator+(temp21, AARSD2);
    AARSD2 = squareRootOperator(AARSD2);
    AARSD3 = operator+(temp31, AARSD3);
    AARSD3 = squareRootOperator(AARSD3);

    temp12 = operator-(temp12, CAAR1);
    temp12 = operator ^(temp12, temp12);
    temp22 = operator-(temp22, CAAR2);
    temp22 = operator ^(temp22, temp22);
    temp32 = operator-(temp32, CAAR3);
    temp32 = operator ^(temp32, temp32);
    CAARSD1 = operator+(temp12, CAARSD1);
    CAARSD1 = squareRootOperator(CAARSD1);
    CAARSD2 = operator+(temp22, CAARSD2);
    CAARSD2 = squareRootOperator(CAARSD2);
    CAARSD3 = operator+(temp32, CAARSD3);
    CAARSD3 = squareRootOperator(CAARSD3);

    Result[0][0] = AAR1;
    Result[0][1] = AARSD1;
    Result[0][2] = CAAR1;
    Result[0][3] = CAARSD1;

    Result[1][0] = AAR2;
    Result[1][1] = AARSD2;
    Result[1][2] = CAAR2;
    Result[1][3] = CAARSD2;

    Result[2][0] = AAR3;
    Result[2][1] = AARSD3;
    Result[2][2] = CAAR3;
    Result[2][3] = CAARSD3;

    
    //for (int i = 0; i < 1; i++)
    //{
    //    for (int j = 0; j < 4; j++)
    //    {
    //        for (int k = 0; k < Result[i][j].size(); k++)
    //        {
    //            cout << Result[i][j][k] << '\n';
    //        }
    //        cout << endl;
    //    }
    //}
    return Result;
}
#endif /* Calculation_h */
