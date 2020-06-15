//
//  SelectRandomStocks.h
//  Project_Final
//
//  Created by sanmuma on 2020/5/5.
//  Copyright © 2020 NYU. All rights reserved.
//

#ifndef SelectRandomStocks_h
#define SelectRandomStocks_h
#include <random>
#include "Stock.h"

class StockShuffler{
private:
    StockGroup stock_groups;
public:
    StockShuffler (StockGroup stock_groups_){stock_groups=stock_groups_;}
    vector <string> Get30StocksPerGroup();
};

vector<string> StockShuffler::Get30StocksPerGroup(){
    vector <string> stock_name,choose_stock;
    for (auto itr=stock_groups.begin();itr!=stock_groups.end();itr++)
    {
        stock_name.push_back(itr->first);
    }
    random_shuffle(stock_name.begin(),stock_name.end());
    for (int j=0;j<30;j++){
        choose_stock.push_back(stock_name[j]);
    }
    return choose_stock;
}


#endif /* SelectRandomStocks_h */
