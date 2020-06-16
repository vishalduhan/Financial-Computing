# Financial-Computing
Used liburl to retrieve historical price data from Yahoo Finance: A function retrieves the adjusted close prices for selected S & P 500 stocks and SPY into memory.
<br>
• Created a set of classes such as class for stock to handle EPS estimate and price information.
<br>
• Used member functions or independent functions for all calculation. Overload a few arithmetic operators for vector/matrix.
<br>
• The stocks and their corresponding price information for each group stored in a STL map, with stock symbol as its keys.
<br>
• The expected AAR, AAR STD, and expected CAAR and CAAR STD for 3 groups are presented in a matrix. The row of the matrix is the group#, matrix columns are for AAR, AAR-STD, CAAR, CAAR-STD
<br>
• Used gnuplot to show the CAAR from all 3 groups in one graph.
<br>
<br>

Bootstrap Algorithm:
<br>
From Zacks, used a query to pull 2019 3rd quarter earnings releases (if a company’s 3rd quarter is far away from the 3rd calendar quarter of 2019, select a quarterly earning close to 3rd calendar quarter of 2019) for all S&P 500 stocks, sorted and divided them into 3 groups:
<br>
<blockquote>
a. Calculated earnings surprise for each stock:
<br>
    Surprise % = (Actual EPS – EPS Estimate) / abs(EPS Estimate)
    <br>
b. Sorted all the surprises in ascending order, and split all the stocks into 3 groups with relatively equivalent numbers of stocks:<br>
    <blockquote>
    i. Highest surprise group: Beat Estimate Group
    <br>
    ii. Lowest surprise group: Miss Estimate Group
    <br>
    iii. The rest stocks in between: Meet Estimate Group
    <br>
    </blockquote>
2. Defined day “zero” for a stock as the day the earning is announced.
<br>
3. Implement Bootstrapping:
<br><blockquote>
    a. Sampling the stocks by randomly selecting 30 stocks from each group, total 90 stocks.
    <br>
    b. Used libcurl lib to retrieve 2N+1 days of historical prices for S&P500 stocks and ETF ticker SPY around the date of earning release . N is integer which must be greater or equal to 30, will be entered by users. Users will be warned if there is not enough historical prices for 2N+1.
    </blockquote>
    <br>
    c. For each stock calculate the daily returns Rit for N days before the day “zero” and N days after, such as t = -60, -59,…-1, 0, 1,…, 59, 60:
    <br>
    <blockquote>
        Rit = (Pricet – Pricet-1)/ Pricet-1
        <br>
        Using adjusted daily closing price for your calculation
        <br>
        </blockquote>
    d. Calculated the corresponding daily return Rmt for SPY for the same days.
    <br>
    e. Defined abnormal returns as the difference ARit= Rit–Rmt.
    <br>
    f. Calculated average daily abnormal returns for each group of stocks (with M stocks) for all 2N reference days:
    <br>
    <blockquote>
        AARt =1/M ∑ARit
        <br>
        </blockquote>
    g. Cumulated the returns on the first T days to CAAR:
    <br>
    <blockquote>
        CAAR = ∑ ARRt
        <br>
        </blockquote>
    h. Repeated steps a to g 30 times and then Calculated
    <br>
    <blockquote>
        i. Average AAR and CAAR for 30 samplings for each group
        <br>
        ii. Standard Deviation of AAR and CAAR for each group
        <br>
        </blockquote>
</blockquote>
