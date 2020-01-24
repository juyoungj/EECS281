// Project Identifier: 8729BF7B2234189B8DF8A6DD31770D2B18569C27
#include <getopt.h>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <math.h>
#include <algorithm>

#include <limits>

using namespace std;


struct DonutShop
{
    int x;
    int y;
    // unsigned int location;
    char country;
};

struct Node
{
    double dv = numeric_limits<double>::infinity();
    bool kv = false;
    unsigned int pv = 0;
};

struct MST
{
    vector<Node> primsVec;
    // unsigned int numOfShops;
    double totalWeight;
    vector<int> pathFromFASTTSP;

    double partialMSTTotalWeight;
    double upperBound;
    double actualCost;
    vector<int> bestPath;

    vector<vector<double>> distanceMatrix;

    double runningWeight = 0.0;


    // void setNumOfShops(unsigned int x);

    void computeMST(vector<DonutShop>& donutShops, unsigned int& numOfShops);
    void getTotalWeight(unsigned int& numOfShops);
    void printMST(unsigned int& numOfShops);

    void computeFASTTSP(vector<DonutShop>& donutShops, unsigned int& numOfShops); //implementing nearest insertion
    void getTotalWeightFASTTSP(vector<DonutShop>& donutShops, unsigned int& numOfShops);
    void printFASTTSP(unsigned int& numOfShops);

    void setDistanceMatrix(vector<DonutShop>& donutShops, unsigned int& numOfShops);

    void computeMSTUnvisited(unsigned int& numOfFixed, vector<int>& bestPathSoFar, unsigned int& numOfShops);
    void computeOPTTSP(unsigned int& numOfShops);
    void printOPTTSP(unsigned int& numOfShops);
    void genPerms(vector<int>& path, unsigned int permLength, unsigned int numOfShops);
    bool promising(unsigned int numOfFixed, vector<int>& bestPathSoFar, unsigned int& numOfShops, double& runningWeight);
    double computeEdges(unsigned int numOfFixed, vector<int>& bestPathSoFar, unsigned int& numOfShops);

};

// struct FASTTSP
// {
//     vector<int> pathFastTSP()
// };


// template <typename T>
// void genPerms(vector<T> &path, size_t permLength) {
//   if (path.size() == permLength) {
//     // Do something with the path
//     return;
//   } // if
//   if (!promising(path, permLength))
//     return;
//   for (size_t i = permLength; i < path.size(); ++i) {
//     swap(path[permLength], path[i]);
//     genPerms(path, permLength + 1);
//     swap(path[permLength], path[i]);
//   } // for
// } // genPerms()


string getMode(int argc, char* argv[])
{
    string mode;

    opterr = true; // Give us help with errors
    int choice;
    int option_index = 0;

    option long_options[] = {
        { "help",    no_argument,       nullptr, 'h'},
        { "mode",   required_argument,  nullptr, 'm'},
        { nullptr, 0,                   nullptr, '\0' }
    };

    // DONE: Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "hm:", long_options, &option_index)) != -1) {
        switch (choice) {
            case 'h':
                cout << "Time to Eat the Donuts!\n";
                exit(0);

            // TODO: 
            case 'm':{
                mode = optarg;
                if ((mode!="MST") && (mode!="FASTTSP") && (mode!="OPTTSP")){
                    cerr << "Wrong mode input.\n";
                    exit(1);
                }
            }   
                break;

            default:
                cerr << "Error: invalid option\n";
                exit(1);
        } 
    }
    return mode;
}

double calculateDistance(int x1, int y1, int x2, int y2)
{
    double x = (double)(x1 - x2);
    double y = (double)(y1 - y2);
    double dist;

    dist = sqrt(x*x + y*y);

    return dist;
}

double triangleDistance(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return calculateDistance(x1, y1, x3, y3) + 
    calculateDistance(x3, y3, x2, y2) - 
    calculateDistance(x1, y1, x2, y2);
}


int main(int argc, char* argv[])
{
    cout << std::setprecision(2);
    cout << std::fixed;

    string mode = getMode(argc, argv);

    
    unsigned int numOfShops;

    vector<DonutShop> donutShops;
    // donutShops.reserve(numOfShops);
    // vector<Node> primsVec;
    // primsVec.reserve(numOfShops);


    bool shopInUS = false;
    bool shopInCanada = false;
    bool shopInBorder = false;

    cin >> numOfShops;
    donutShops.reserve(numOfShops);
    for(unsigned int i=0; i<numOfShops; ++i)
    {
        DonutShop donutShopInput;
        cin >> donutShopInput.x;
        cin >> donutShopInput.y;
        // cout << donutShopInput.x << ' ' << donutShopInput.y << '\n';
        // donutShopInput.location = i;
        if((donutShopInput.x>0 && donutShopInput.y==0) ||
            (donutShopInput.x==0 && donutShopInput.y>0) ||
            (donutShopInput.x==0 && donutShopInput.y==0) )
        {
            donutShopInput.country = 'b';
            shopInBorder = true;
        }
        else if(donutShopInput.x>0 && donutShopInput.y>0)
        {
            donutShopInput.country = 'c';
            shopInCanada = true;
        }
        else
        {
            donutShopInput.country = 'u';
            shopInUS = true;
        }
        donutShops.push_back(donutShopInput);
    }

    // vector<unsigned int> innieVec;
    // vecotr<unsigned int> outieVec;

    if(mode == "MST")
    {
        if(shopInUS && shopInCanada)
        {
            if(!shopInBorder)
            {
                cerr << "Cannot construct MST\n";
                exit(1);
            }
        }

        MST mst;
        // mst.setNumOfShops(numOfShops);
        mst.computeMST(donutShops, numOfShops);
        mst.getTotalWeight(numOfShops);
        mst.printMST(numOfShops);


    }// if mode is mst
    else if(mode == "FASTTSP")
    {
        MST mst;
        // cout << "object created" << endl;
        // mst.setNumOfShops(numOfShops);
        // cout << "number of shops set" << endl;
        mst.computeFASTTSP(donutShops, numOfShops);
        // cout << "compute FASTTSP" << endl;
        mst.getTotalWeightFASTTSP(donutShops, numOfShops);
        // cout << "get total weight of FASTTSP" << endl;
        mst.printFASTTSP(numOfShops);
        // cout << "print FASTTSP" << endl;
    }
    else if(mode == "OPTTSP")
    {
        // vector<vector<double>> distanceMatrix(numOfShops, vector<double>(numOfShops,0));

        // for(unsigned int i=0; i<numOfShops; i++)
        // {
        //     for(unsigned int j=0; j<numOfShops; j++)
        //     {
        //         distanceMatrix[i][j] = calculateDistance(donutShops[i].x, donutShops[i].y, donutShops[j].x, donutShops[j].y);
        //     }
        // }



        MST mst;
        mst.setDistanceMatrix(donutShops, numOfShops);
        mst.computeFASTTSP(donutShops, numOfShops);
        mst.getTotalWeightFASTTSP(donutShops, numOfShops);
        mst.computeOPTTSP(numOfShops);
        mst.printOPTTSP(numOfShops);

    }

    

    return 0;
}

// --------------------------------------------------------------------
//                    MST struct Definitions
// --------------------------------------------------------------------

//compute MST
// void MST::setNumOfShops(unsigned int x)
// {
//     numOfShops = x;
// }

void MST::computeMST(vector<DonutShop>& donutShops, unsigned int& numOfShops)
{
    // unsigned int numOfShops = donutShops.size();
    primsVec.reserve(numOfShops);

    for(unsigned int i=0; i<numOfShops; i++)
    {
        primsVec[i].dv = numeric_limits<double>::infinity();
        primsVec[i].kv = false;
        primsVec[i].pv = 0;
    }

    
    primsVec[0].dv = 0;
    primsVec[0].kv = true;

 
    for(unsigned int i=1;i<numOfShops;i++)
    {
        if( (donutShops[0].country == 'b') || 
            (donutShops[i].country == 'b') ||
            (donutShops[0].country == donutShops[i].country) )
        {
            primsVec[i].dv = calculateDistance(donutShops[0].x, donutShops[0].y, 
                                    donutShops[i].x, donutShops[i].y);
        }
        

    }// for loop

    

    for(unsigned int j=1;j<numOfShops;j++)
    {
        int shortestIndex = 0;
        double shortestDist = numeric_limits<double>::infinity();

        for(unsigned int i=0;i<numOfShops;i++) // possible change to starting from i=1 when time matters
        {
            // cout << primsVec[i].dv << endl;

            if((primsVec[i].kv == false) && (primsVec[i].dv < shortestDist))
            {
                // cout << "index!!" << endl;
                // cout << i << endl;
                shortestIndex = i;
                shortestDist = primsVec[i].dv;
                // cout << "shortest distance is " << shortestDist << endl;
            }
        }

       

        primsVec[shortestIndex].kv = true;
        // pv[shortestIndex]=0;
        // 0 = shortestIndex;
        for(unsigned int i=0;i<numOfShops;i++) // possible change to starting from i=1 when time matters
        {
            
            if(primsVec[i].kv == false && 
                primsVec[i].dv > calculateDistance(donutShops[shortestIndex].x, donutShops[shortestIndex].y, donutShops[i].x, donutShops[i].y))
            {
                if((donutShops[shortestIndex].country == 'b') || 
                    (donutShops[i].country == 'b') || 
                    (donutShops[shortestIndex].country == donutShops[i].country))
                {
                    primsVec[i].dv = calculateDistance(donutShops[shortestIndex].x, donutShops[shortestIndex].y, donutShops[i].x, donutShops[i].y);
                    primsVec[i].pv = shortestIndex;
                
                }   

            }// if statement

        }// for loop

    }// for loop

}

void MST::getTotalWeight(unsigned int& numOfShops)
{
    totalWeight = 0.0;
    for(unsigned int i=0; i<numOfShops; i++)
    {
        totalWeight += primsVec[i].dv;    
    }
}

void MST::printMST(unsigned int& numOfShops)
{
    cout << totalWeight << '\n';
    for(unsigned int i=1; i<numOfShops; i++)
    {
        if (primsVec[i].pv < i)
        {
            cout << primsVec[i].pv << ' ' << i << '\n';
        }
        else
        {
            cout << i << ' ' << primsVec[i].pv << '\n';
        }
        // cout << std::min(i,primsVec[i].pv) << ' ' << std::max(i,primsVec[i].pv) << '\n';
    }
}

void MST::computeFASTTSP(vector<DonutShop>& donutShops, unsigned int& numOfShops)
{
    // unsigned int numOfShops = donutShops.size();
   
    vector<bool> isVisited(numOfShops, false);
    
    // pathFromFASTTSP.reserve(numOfShops+1);

    double shortestDist = numeric_limits<double>::infinity();
    int shortestIndex = 0;

    isVisited[0] = true;
    pathFromFASTTSP.push_back(0);

    for(unsigned int i=1;i<numOfShops;i++)
    {
        if(calculateDistance(donutShops[0].x, donutShops[0].y, 
                                    donutShops[i].x, donutShops[i].y) < shortestDist)
        {
            shortestDist = calculateDistance(donutShops[0].x, donutShops[0].y, 
                                    donutShops[i].x, donutShops[i].y);
            shortestIndex = i;
        }
    }

    // cout << "second index is " << shortestIndex << endl;

    pathFromFASTTSP.push_back(shortestIndex);
    pathFromFASTTSP.push_back(0);

    isVisited[shortestIndex] = true;
    // initiailzing first two points

    unsigned int numOfShopsVisited = 2;

    while(numOfShopsVisited != numOfShops)
    {
        

        int randomIndex = rand() % numOfShops;
        while(isVisited[randomIndex]==true)
        {
            randomIndex = rand() % numOfShops;
        }

        int insertIndex;
        double minCost = numeric_limits<double>::infinity();
        double currentCost;
        for(unsigned int p=0; p<numOfShopsVisited; p++)
        {
            currentCost = triangleDistance(donutShops[pathFromFASTTSP[p]].x, donutShops[pathFromFASTTSP[p]].y, 
                donutShops[pathFromFASTTSP[p+1]].x, donutShops[pathFromFASTTSP[p+1]].y, 
                donutShops[randomIndex].x, donutShops[randomIndex].y);
            if(currentCost<minCost)
            {
                minCost = currentCost;
                insertIndex = p+1;
            }
        }

        pathFromFASTTSP.insert(pathFromFASTTSP.begin() + insertIndex, randomIndex);

        isVisited[randomIndex] = true;
        numOfShopsVisited++;
        
    }
    // cout << "ending?" << endl;

}

void MST::getTotalWeightFASTTSP(vector<DonutShop>& donutShops, unsigned int& numOfShops)
{
    totalWeight = 0.0;
    
    for(unsigned int i=0; i<numOfShops; i++)
    {
        totalWeight+=calculateDistance(donutShops[pathFromFASTTSP[i]].x,
                                        donutShops[pathFromFASTTSP[i]].y,
                                        donutShops[pathFromFASTTSP[i+1]].x,
                                        donutShops[pathFromFASTTSP[i+1]].y); 
    }
}

void MST::printFASTTSP(unsigned int& numOfShops)
{
    cout << totalWeight << '\n';
    cout << pathFromFASTTSP[0];
    for(unsigned int i=1; i<numOfShops; i++)
    {
        cout << " " << pathFromFASTTSP[i];
    }
    cout << '\n';
}



void MST::setDistanceMatrix(vector<DonutShop>& donutShops, unsigned int& numOfShops)
{
    // distanceMatrix(numOfShops, vector<double>(numOfShops,0));
    distanceMatrix.reserve(numOfShops);
    while(distanceMatrix.size() < numOfShops)
    {
        vector<double> temp(numOfShops, 0);
        distanceMatrix.push_back(temp);
    }
    // setDistanceMatrix

    for(unsigned int i=0; i<numOfShops; i++)
    {
        for(unsigned int j=0; j<numOfShops; j++)
        {
            distanceMatrix[i][j] = calculateDistance(donutShops[i].x, donutShops[i].y, donutShops[j].x, donutShops[j].y);
        }
    }
}



void MST::computeMSTUnvisited(unsigned int& numOfFixed, vector<int>& bestPathSoFar, unsigned int& numOfShops)
{
    unsigned int numOfUnvisited = numOfShops - numOfFixed;
    primsVec.reserve(numOfUnvisited);

    for(unsigned int i=0; i<numOfUnvisited; i++)
    {
        primsVec[i].dv = numeric_limits<double>::infinity();
        primsVec[i].kv = false;
        primsVec[i].pv = 0;
    }
    
    primsVec[0].dv = 0;
    primsVec[0].kv = true;

    for(unsigned int i=1; i<numOfUnvisited; i++)
    {
        primsVec[i].dv = distanceMatrix[bestPathSoFar[numOfFixed]][bestPathSoFar[numOfFixed+i]];
    }

    for(unsigned int j=1; j<numOfUnvisited; j++)
    {
        int shortestIndex = 0;
        double shortestDist = numeric_limits<double>::infinity();
        for(unsigned int i=0; i<numOfUnvisited; i++)
        {
            if((primsVec[i].kv == false) && (primsVec[i].dv < shortestDist))
            {
                shortestIndex = i;
                shortestDist = primsVec[i].dv;
            }
        }
        primsVec[shortestIndex].kv = true;

        for(unsigned int i=0; i<numOfUnvisited; i++)
        {
            if(primsVec[i].kv == false && 
                primsVec[i].dv > distanceMatrix[bestPathSoFar[shortestIndex + numOfFixed]][bestPathSoFar[numOfFixed + i]])
            {
                primsVec[i].dv = distanceMatrix[bestPathSoFar[shortestIndex + numOfFixed]][bestPathSoFar[numOfFixed + i]];
                primsVec[i].pv = shortestIndex;
            }
        }
    }

    partialMSTTotalWeight = 0.0;

    for(unsigned int i=0; i<numOfUnvisited; i++)
    {
        partialMSTTotalWeight += primsVec[i].dv;
    }
    



}

void MST::computeOPTTSP(unsigned int& numOfShops)
{
    upperBound = totalWeight;
    bestPath = pathFromFASTTSP;
    pathFromFASTTSP.pop_back();
    


    genPerms(pathFromFASTTSP, 1, numOfShops);


}

void MST::printOPTTSP(unsigned int& numOfShops)
{
    cout << upperBound << '\n';
    cout << bestPath[0];
    for(unsigned int i=1; i<numOfShops; i++)
    {
        cout << " " << bestPath[i];
    }
    cout << '\n';
}

void MST::genPerms(vector<int>& path, unsigned int permLength, unsigned int numOfShops)
{
    

    if (path.size() == permLength) 
    {

        if(runningWeight + distanceMatrix[path[0]][path[permLength-1]] < upperBound)
        {
            upperBound = runningWeight + distanceMatrix[path[0]][path[permLength-1]];
            bestPath = path;
        }
        // Do something with the path
        return;
    } // if
    if (!promising(permLength, path, numOfShops, runningWeight))
        return;
    for (size_t i = permLength; i < path.size(); ++i) 
    {
        swap(path[permLength], path[i]);
        runningWeight += distanceMatrix[path[permLength]][path[permLength-1]];
        genPerms(path, permLength + 1, numOfShops);
        runningWeight -= distanceMatrix[path[permLength]][path[permLength-1]];
        swap(path[permLength], path[i]);
    } // for
}

bool MST::promising(unsigned int numOfFixed, vector<int>& bestPathSoFar, unsigned int& numOfShops, double& runningWeight)
{

    unsigned int k = numOfShops - numOfFixed;
    if (k < 5)
    {
        return true;
    }

    computeMSTUnvisited(numOfFixed, bestPathSoFar, numOfShops);

    // actualCost = 0.0;
    // for(unsigned int i=0; i<numOfFixed-1; i++)
    // {
    //     actualCost += distanceMatrix[bestPathSoFar[i]][bestPathSoFar[i+1]];
    // }

    if(partialMSTTotalWeight + runningWeight + computeEdges(numOfFixed, bestPathSoFar, numOfShops) < upperBound)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double MST::computeEdges(unsigned int numOfFixed, vector<int>& bestPathSoFar, unsigned int& numOfShops)
{
    double edge1 = numeric_limits<double>::infinity();
    double edge2 = numeric_limits<double>::infinity();

    if (bestPathSoFar.empty())
    {
        return 0;
    } 
    for (unsigned int i=numOfFixed; i<numOfShops; i++)
    {
        double d1 = distanceMatrix[0][bestPathSoFar[i]];
        double d2 = distanceMatrix[bestPathSoFar[numOfFixed-1]][bestPathSoFar[i]];

        if (d1 < edge1)
        {
            edge1 = d1;
        }
        if (d2 < edge2)
        {
            edge2 = d2;
        }
    }
    return edge1 + edge2;
}