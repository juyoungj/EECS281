#include <getopt.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <queue>
#include "P2random.h"

using namespace std;

struct Deployment
{
    int timeStamp;
    string side;
    int generalId;
    int planetNum;
    int force;
    // int numTroops;
    mutable int numTroops;
    int order; 

    void setNumTroops(int n) const
    {
        numTroops = n;
    }

    bool isVerbose = false;
    bool isMedian = false;
    bool isGeneral = false;
    bool isWatcher = false;

    void getMode(int argc, char * argv[]);

};

struct movieWatcher
{
    // movieWatcher();
    int timeStamp;
    int force;
};


struct JediComp
{
    bool operator()(const Deployment &lhs,
                    const Deployment &rhs) const 
    {
        return ((lhs.force > rhs.force) ||
               ((lhs.force == rhs.force) && (lhs.order > rhs.order)));
    } // operator ()
}; // JediComp ()

struct SithComp
{
    bool operator()(const Deployment &lhs,
                    const Deployment &rhs) const 
    {
        return ((lhs.force < rhs.force) ||
               ((lhs.force == rhs.force) && (lhs.order > rhs.order)));
    } // operator ()
}; // SithComp ()

struct Planet
{
    priority_queue<Deployment, vector<Deployment>, JediComp> jedi;
    priority_queue<Deployment, vector<Deployment>, SithComp> sith;
};

struct General
{
    int numJediTroops=0;
    int numSithTroops=0;
    int destroyedTroops=0;

};

struct AttackMode
{
    movieWatcher bestJedi;
    movieWatcher bestSith;
    movieWatcher potentialJedi;
};

struct AmbushMode
{
    movieWatcher bestJedi;
    movieWatcher bestSith;
    movieWatcher potentialSith;
};

struct Median
{
    //Values greater than the median sorted so that smallest is on top
    vector<priority_queue<int, vector<int>, greater<int> >> upper;
    //Values smaller than the median sorted so that greatest is on top
    vector<priority_queue<int, vector<int>, less<int> >> lower;

    int finalMed;

    void updateMedian(int troopsLost, int planetNum);
    int getMedian(int planetNum);
};




int main(int argc, char* argv[])
{
    Deployment currentDeployment;
    currentDeployment.getMode(argc, argv);

    bool isVerbose = currentDeployment.isVerbose;
    bool isGeneral = currentDeployment.isGeneral;
    bool isMedian = currentDeployment.isMedian;
    bool isWatcher = currentDeployment.isWatcher;

    Median median;
    

    // int currentTimeStamp = 0;

    string inputmode;   
    int numGenerals = 0;
    int numPlanets = 0;
    int randomSeed = 0;
    int numDeployments = 0;
    int arrivalRate = 0;

    vector<Planet> planets;
    vector<General> generals;
    vector<AttackMode> attack;
    vector<AmbushMode> ambush;

    // vector<priority_queue<int>> troopsLost;

    bool PR_MODE = false;
    // vector<char> modevector = getMode(argc, argv);
    cout << "Deploying troops..." << '\n';

    string junkline;
    getline(cin, junkline);

    string junkstring;
    cin >> junkstring >> inputmode;
    cin >> junkstring >> numGenerals;
    cin >> junkstring >> numPlanets;
    
    int currentTimeStamp = 0;
    int numBattles = 0;

    stringstream ss;
    if(inputmode == "DL")
    {
        PR_MODE = false;    
    }
    else if(inputmode == "PR")
    {
        PR_MODE = true;
        string junkstring;
        cin >> junkstring >> randomSeed;
        cin >> junkstring >> numDeployments;
        cin >> junkstring >> arrivalRate;

        P2random::PR_init(ss, randomSeed, numGenerals, numPlanets, numDeployments, arrivalRate);
    }
    planets.resize(numPlanets);
    generals.resize(numGenerals);
    attack.resize(numPlanets);
    ambush.resize(numPlanets);

    // troopsLost.resize(numPlanets);
    median.upper.resize(numPlanets);
    median.lower.resize(numPlanets);

    int deploymentOrder=0;
    int pastTimeStamp=0;

    movieWatcher currentMovieWatcher;

    char junkchar;
    istream& inputStream = PR_MODE ? ss : cin;
    while(inputStream >> currentDeployment.timeStamp >> currentDeployment.side >> 
            junkchar >> currentDeployment.generalId >> junkchar >> currentDeployment.planetNum >>
            junkchar >> currentDeployment.force >> junkchar >> currentDeployment.numTroops)
    {

        if(pastTimeStamp > currentDeployment.timeStamp)
        {
            cerr << "Error: Timestamp should be non-decreasing." << '\n';
            exit(1);
        }
        if(currentDeployment.timeStamp < 0)
        {
            cerr << "Error: timestamp out of range" << '\n';
            exit(1);
        }
        if((currentDeployment.generalId < 0) || (currentDeployment.generalId > numGenerals-1))
        {
            cerr << "Error: wrong general ID" << '\n';
            exit(1);
        }
        if((currentDeployment.planetNum < 0) || (currentDeployment.planetNum > numPlanets-1))
        {
            cerr << "Error: wrong planet ID" << '\n';
            exit(1);
        }
        if((currentDeployment.force <= 0) || (currentDeployment.numTroops <= 0))
        {
            cerr << "Error: wrong force or wrong number of troops" << '\n';
            exit(1);
        }

        pastTimeStamp = currentDeployment.timeStamp;

        currentDeployment.order = deploymentOrder;
        // cout << deploymentOrder << endl;
        deploymentOrder++;

        currentMovieWatcher.force = currentDeployment.force;
        currentMovieWatcher.timeStamp = currentDeployment.timeStamp;


        if(currentDeployment.timeStamp != currentTimeStamp)
        {
            // if(find(modevector.begin(), modevector.end(), 'm') != modevector.end())
            if(isMedian)
            {
                for(int i=0; i < numPlanets; ++i)
                {
                    if(!median.upper[i].empty())
                    {
                        cout << "Median troops lost on planet " << i;
                        cout << " at time " << currentTimeStamp;
                        cout << " is " << median.getMedian(i) << "." << '\n'; 
                    }
                    
                }
                
            }
            currentTimeStamp = currentDeployment.timeStamp;
           
        }

        int numTroopsLost = 0;
        if(currentDeployment.side == "JEDI")
        {
            planets[currentDeployment.planetNum].jedi.push(currentDeployment);
            generals[currentDeployment.generalId].numJediTroops+=currentDeployment.numTroops;

            if(ambush[currentDeployment.planetNum].bestSith.force != 0)
            {
                if(ambush[currentDeployment.planetNum].bestJedi.force == 0)
                {
                    if(currentMovieWatcher.force <= ambush[currentDeployment.planetNum].bestSith.force)
                    {
                        ambush[currentDeployment.planetNum].bestJedi.force = currentMovieWatcher.force;
                        ambush[currentDeployment.planetNum].bestJedi.timeStamp = currentMovieWatcher.timeStamp;
                    }

                }
                else if(ambush[currentDeployment.planetNum].bestJedi.force != 0)
                {
                    if(ambush[currentDeployment.planetNum].potentialSith.force == 0)
                    {
                        if(currentMovieWatcher.force < ambush[currentDeployment.planetNum].bestJedi.force)
                        {
                            ambush[currentDeployment.planetNum].bestJedi.force = currentMovieWatcher.force;
                            ambush[currentDeployment.planetNum].bestJedi.timeStamp = currentMovieWatcher.timeStamp;
                        }
                    }
                    else if(ambush[currentDeployment.planetNum].potentialSith.force != 0)
                    {
                        if(((ambush[currentDeployment.planetNum].potentialSith.force - currentMovieWatcher.force) 
                            > (ambush[currentDeployment.planetNum].bestSith.force - ambush[currentDeployment.planetNum].bestJedi.force)))
                        {
                            ambush[currentDeployment.planetNum].bestSith.force = ambush[currentDeployment.planetNum].potentialSith.force;
                            ambush[currentDeployment.planetNum].bestSith.timeStamp = ambush[currentDeployment.planetNum].potentialSith.timeStamp;
                            ambush[currentDeployment.planetNum].bestJedi.force = currentMovieWatcher.force;
                            ambush[currentDeployment.planetNum].bestJedi.timeStamp = currentMovieWatcher.timeStamp;
                            ambush[currentDeployment.planetNum].potentialSith.force = 0;
                            ambush[currentDeployment.planetNum].potentialSith.timeStamp = 0;
                        }
                    }
                }
            }

            if(attack[currentDeployment.planetNum].bestJedi.force == 0)
            {
                attack[currentDeployment.planetNum].bestJedi.force = currentMovieWatcher.force;
                attack[currentDeployment.planetNum].bestJedi.timeStamp = currentMovieWatcher.timeStamp;
            }
            else if(attack[currentDeployment.planetNum].bestJedi.force != 0)
            {
                if(attack[currentDeployment.planetNum].bestSith.force == 0)
                {
                    if(currentMovieWatcher.force < attack[currentDeployment.planetNum].bestJedi.force)
                    {
                        attack[currentDeployment.planetNum].bestJedi.force = currentMovieWatcher.force;
                        attack[currentDeployment.planetNum].bestJedi.timeStamp = currentMovieWatcher.timeStamp;
                    }
                }
                else if(attack[currentDeployment.planetNum].bestSith.force != 0)
                {
                    if(attack[currentDeployment.planetNum].potentialJedi.force == 0)
                    {
                        if(currentMovieWatcher.force < attack[currentDeployment.planetNum].bestJedi.force)
                        {
                            attack[currentDeployment.planetNum].potentialJedi.force = currentMovieWatcher.force;
                            attack[currentDeployment.planetNum].potentialJedi.timeStamp = currentMovieWatcher.timeStamp;
                        }
                    }
                    else if(attack[currentDeployment.planetNum].potentialJedi.force != 0)
                    {
                        if(currentMovieWatcher.force < attack[currentDeployment.planetNum].potentialJedi.force)
                        {
                            attack[currentDeployment.planetNum].potentialJedi.force = currentMovieWatcher.force;
                            attack[currentDeployment.planetNum].potentialJedi.timeStamp = currentMovieWatcher.timeStamp;
                        }
                    }
                }
            }
        }
        else if(currentDeployment.side == "SITH")
        {
            planets[currentDeployment.planetNum].sith.push(currentDeployment);
            generals[currentDeployment.generalId].numSithTroops += currentDeployment.numTroops;

            if(ambush[currentDeployment.planetNum].bestSith.force == 0)
            {
                ambush[currentDeployment.planetNum].bestSith.force = currentMovieWatcher.force;
                ambush[currentDeployment.planetNum].bestSith.timeStamp = currentMovieWatcher.timeStamp;
            }
            else if(ambush[currentDeployment.planetNum].bestSith.force != 0)
            {
                if(ambush[currentDeployment.planetNum].bestJedi.force == 0)
                {
                    if(currentMovieWatcher.force > ambush[currentDeployment.planetNum].bestSith.force)
                    {
                        ambush[currentDeployment.planetNum].bestSith.force = currentMovieWatcher.force;
                        ambush[currentDeployment.planetNum].bestSith.timeStamp = currentMovieWatcher.timeStamp;
                    }
                }
                else if(ambush[currentDeployment.planetNum].bestJedi.force != 0)
                {
                    if(ambush[currentDeployment.planetNum].potentialSith.force == 0)
                    {
                        if(currentMovieWatcher.force > ambush[currentDeployment.planetNum].bestSith.force)
                        {
                            ambush[currentDeployment.planetNum].potentialSith.force = currentMovieWatcher.force;
                            ambush[currentDeployment.planetNum].potentialSith.timeStamp = currentMovieWatcher.timeStamp;
                        }
                    }
                    else if(ambush[currentDeployment.planetNum].potentialSith.force != 0)
                    {
                        if(currentMovieWatcher.force > ambush[currentDeployment.planetNum].potentialSith.force)
                        {
                            ambush[currentDeployment.planetNum].potentialSith.force = currentMovieWatcher.force;
                            ambush[currentDeployment.planetNum].potentialSith.timeStamp = currentMovieWatcher.timeStamp;
                        }
                    }
                }
            }

            if(attack[currentDeployment.planetNum].bestJedi.force != 0)
            {
                if(attack[currentDeployment.planetNum].bestSith.force == 0)
                {
                    if(currentMovieWatcher.force >= attack[currentDeployment.planetNum].bestJedi.force)
                    {
                        attack[currentDeployment.planetNum].bestSith.force = currentMovieWatcher.force;
                        attack[currentDeployment.planetNum].bestSith.timeStamp = currentMovieWatcher.timeStamp;
                    }

                }
                else if(attack[currentDeployment.planetNum].bestSith.force != 0)
                {
                    if(attack[currentDeployment.planetNum].potentialJedi.force == 0)
                    {
                        if(currentMovieWatcher.force > attack[currentDeployment.planetNum].bestSith.force)
                        {
                            attack[currentDeployment.planetNum].bestSith.force = currentMovieWatcher.force;
                            attack[currentDeployment.planetNum].bestSith.timeStamp = currentMovieWatcher.timeStamp;
                        }
                    }
                    else if(attack[currentDeployment.planetNum].potentialJedi.force != 0)
                    {
                        if(((currentMovieWatcher.force - attack[currentDeployment.planetNum].potentialJedi.force) 
                            > (attack[currentDeployment.planetNum].bestSith.force - attack[currentDeployment.planetNum].bestJedi.force)))
                        {
                            attack[currentDeployment.planetNum].bestJedi.force = attack[currentDeployment.planetNum].potentialJedi.force;
                            attack[currentDeployment.planetNum].bestJedi.timeStamp = attack[currentDeployment.planetNum].potentialJedi.timeStamp;
                            attack[currentDeployment.planetNum].bestSith.force = currentMovieWatcher.force;
                            attack[currentDeployment.planetNum].bestSith.timeStamp = currentMovieWatcher.timeStamp;
                            attack[currentDeployment.planetNum].potentialJedi.force = 0;
                            attack[currentDeployment.planetNum].potentialJedi.timeStamp = 0;
                        }
                    }
                }
            }                  
        }


        
        if((!planets[currentDeployment.planetNum].jedi.empty()) && (!planets[currentDeployment.planetNum].sith.empty()))
        {
            int jediNumTroops = planets[currentDeployment.planetNum].jedi.top().numTroops;
            int sithNumTroops = planets[currentDeployment.planetNum].sith.top().numTroops;
            while(planets[currentDeployment.planetNum].jedi.top().force <= planets[currentDeployment.planetNum].sith.top().force)
            {
                
                if(jediNumTroops != sithNumTroops)
                {
                    // jediNumTroops = jediNumTroops - sithNumTroops;
                    numTroopsLost = 2*min(jediNumTroops, sithNumTroops);
                    generals[planets[currentDeployment.planetNum].jedi.top().generalId].destroyedTroops+=(numTroopsLost/2);
                    generals[planets[currentDeployment.planetNum].sith.top().generalId].destroyedTroops+=(numTroopsLost/2);
                    // troopsLost[currentDeployment.planetNum].push(numTroopsLost);
                    // RunningMedian rm;
                    // rm.push(numTroopsLost);
                    median.updateMedian(numTroopsLost, currentDeployment.planetNum);
                    // troopsLost[currentDeployment.planetNum].push(numTroopsLost);
                    // if(find(modevector.begin(), modevector.end(), 'v') != modevector.end())
                    if(isVerbose)
                    {
                        cout << "General " << planets[currentDeployment.planetNum].sith.top().generalId;
                        cout << "'s battalion attacked General " << planets[currentDeployment.planetNum].jedi.top().generalId;
                        cout << "'s battalion on planet " << currentDeployment.planetNum;
                        cout << ". " << numTroopsLost << " troops were lost." << '\n';
                    }
                    

                    if(jediNumTroops > sithNumTroops)
                    {
                        planets[currentDeployment.planetNum].jedi.top().setNumTroops(jediNumTroops - sithNumTroops);
                        jediNumTroops = planets[currentDeployment.planetNum].jedi.top().numTroops;
                        planets[currentDeployment.planetNum].sith.pop();
                        sithNumTroops = planets[currentDeployment.planetNum].sith.top().numTroops;
                        numBattles++;
                    }
                    else if(jediNumTroops < sithNumTroops)
                    { 
                        planets[currentDeployment.planetNum].sith.top().setNumTroops(sithNumTroops - jediNumTroops);
                        sithNumTroops = planets[currentDeployment.planetNum].sith.top().numTroops;
                        planets[currentDeployment.planetNum].jedi.pop();
                        jediNumTroops = planets[currentDeployment.planetNum].jedi.top().numTroops;
                        numBattles++;
                    }
                    
                }
                else
                {
                    numTroopsLost = 2*jediNumTroops;
                    generals[planets[currentDeployment.planetNum].jedi.top().generalId].destroyedTroops+=(numTroopsLost/2);
                    generals[planets[currentDeployment.planetNum].sith.top().generalId].destroyedTroops+=(numTroopsLost/2);
                    // RunningMedian rm;
                    // rm.push(numTroopsLost);
                    median.updateMedian(numTroopsLost, currentDeployment.planetNum);
                    // troopsLost[currentDeployment.planetNum].push(numTroopsLost);
                    // if(find(modevector.begin(), modevector.end(), 'v') != modevector.end())
                    if(isVerbose)
                    {
                        cout << "General " << planets[currentDeployment.planetNum].sith.top().generalId;
                        cout << "'s battalion attacked General " << planets[currentDeployment.planetNum].jedi.top().generalId;
                        cout << "'s battalion on planet " << currentDeployment.planetNum;
                        cout << ". " << numTroopsLost << " troops were lost." << '\n';
                    }
                    planets[currentDeployment.planetNum].jedi.pop();
                    planets[currentDeployment.planetNum].sith.pop();
                    jediNumTroops = planets[currentDeployment.planetNum].jedi.top().numTroops;
                    sithNumTroops = planets[currentDeployment.planetNum].sith.top().numTroops;
                    numBattles++;
                }
                if (planets[currentDeployment.planetNum].jedi.empty() || planets[currentDeployment.planetNum].sith.empty())
                {
                    // cout << "empty!" << endl;
                    break;
                }
                
            }
        }
    }
    // if(find(modevector.begin(), modevector.end(), 'm') != modevector.end())
    if(isMedian)
    {
        for(int i=0; i < numPlanets; ++i)
        {
            if(!median.upper[i].empty())
            {
                cout << "Median troops lost on planet " << i;
                cout << " at time " << currentTimeStamp;
                cout << " is " << median.getMedian(i) << "." << '\n'; 
            }
        
        }
    }
    cout << "---End of Day---" << '\n';
    cout << "Battles: " << numBattles << '\n';



    // cout << generals[2].numJediTroops << endl;

    // if(find(modevector.begin(), modevector.end(), 'g') != modevector.end())
    if(isGeneral)
    {
        cout << "---General Evaluation---" << '\n';
        for(int i=0; i < numGenerals; ++i)
        {
            // int numSurvived;
            // numSurvived = generals[i].destroyedTroops;
            cout << "General " << i;
            cout << " deployed " << generals[i].numJediTroops;
            cout << " Jedi troops and " << generals[i].numSithTroops;
            cout << " Sith troops, and " << generals[i].numJediTroops + generals[i].numSithTroops - generals[i].destroyedTroops;
            cout << '/' << generals[i].numJediTroops + generals[i].numSithTroops;
            cout << " troops survived." << '\n';
        }
    }

    // if(find(modevector.begin(), modevector.end(), 'w') != modevector.end())
    if(isWatcher)
    {
        cout << "---Movie Watcher---" << '\n';
        for(int i=0; i < numPlanets; ++i)
        {
            if(ambush[i].bestJedi.force != 0)
            {
                cout << "A movie watcher would enjoy an ambush on planet " << i;
                cout << " with Sith at time " << ambush[i].bestSith.timeStamp;
                cout << " and Jedi at time " << ambush[i].bestJedi.timeStamp <<"." << '\n';
            }
            else if(ambush[i].bestJedi.force == 0)
            {
                cout << "A movie watcher would enjoy an ambush on planet " << i;
                cout << " with Sith at time " << "-1";
                cout << " and Jedi at time " << "-1" <<"." << '\n';
            }

            if(attack[i].bestSith.force != 0)
            {
                cout << "A movie watcher would enjoy an attack on planet " << i;
                cout << " with Jedi at time " << attack[i].bestJedi.timeStamp;
                cout << " and Sith at time " << attack[i].bestSith.timeStamp <<"." << '\n';
            }
            else if(attack[i].bestSith.force == 0)
            {
                cout << "A movie watcher would enjoy an attack on planet " << i;
                cout << " with Jedi at time " << "-1";
                cout << " and Sith at time " << "-1" <<"." << '\n';
            }

            
        }
        
    }

    return 0;
}

//Definitions

void Deployment::getMode(int argc, char * argv[]) 
{
    // bool modeSpecified = false;

    // vector<char> modevector;

    // char outputmode = '\0';
    

    // These are used with getopt_long()
    opterr = true; // Give us help with errors
    int choice;
    int option_index = 0;
    option long_options[] = {
        // { "help",    no_argument,       nullptr, 'h'},
        { "verbose",   no_argument,       nullptr, 'v'},
        { "median",   no_argument,       nullptr, 'm'}, 
        { "general-eval",   no_argument,       nullptr, 'g'},
        { "watcher",   no_argument,       nullptr, 'w'},
        { nullptr, 0,                   nullptr, '\0' }
    };

    // DONE: Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "hvmgw", long_options, &option_index)) != -1) {
        switch (choice) {
            case 'v':
                // modeSpecified = true;
                isVerbose = true;
                // outputmode = 'v';
                // modevector.push_back(outputmode);
                break;

            // TODO: 
            case 'm':
                // modeSpecified = true;
                isMedian = true;
                // outputmode = 'm';
                // modevector.push_back(outputmode);
                break;

            case 'g':
                // modeSpecified = true;
                isGeneral = true;
                // outputmode = 'g';
                // modevector.push_back(outputmode);
                break;

            case 'w':
                // modeSpecified = true;
                // outputmode = 'w';
                isWatcher = true;
                // modevector.push_back(outputmode);
                break;

            default:
                cerr << "Error: invalid option" << "\n";
                exit(1);
        } 
        // if (!modeSpecified)
        // {
        //     cerr << "Error: no mode specified" << "\n";
        //     exit(1);
        // } 
    } 

    // return modevector;
} // getMode()

void Median::updateMedian(int troopsLost, int planetNum)
{
    // upper[planetNum].push(troopsLost);
    // int x = upper[planetNum].top();
    if(upper[planetNum].empty())
    {
        upper[planetNum].push(troopsLost);
    }
    else if(troopsLost >= upper[planetNum].top())
    {
        upper[planetNum].push(troopsLost);
    }
    else
    {
        lower[planetNum].push(troopsLost);
    }
    if(upper[planetNum].size() - lower[planetNum].size() == 2)
    {
        lower[planetNum].push(upper[planetNum].top());
        upper[planetNum].pop();
    }
    else if(lower[planetNum].size()-upper[planetNum].size() == 2)
    {
        upper[planetNum].push(lower[planetNum].top());
        lower[planetNum].pop();
    }

}

int Median::getMedian(int planetNum)
{
    if(upper[planetNum].size()==lower[planetNum].size())
    {
        finalMed = (upper[planetNum].top()+lower[planetNum].top())/((int)2.0);
    }
    else if(upper[planetNum].size()>lower[planetNum].size())
    {
        finalMed = upper[planetNum].top();
    }
    else
    {
        finalMed = lower[planetNum].top();
    }
    return finalMed;
}