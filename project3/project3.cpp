// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <math.h>
#include <deque>
#include <set>
#include <string.h>

using namespace std;

#ifdef _MSC_VER
inline int strcasecmp(const char *s1, const char *s2) {
    return _stricmp(s1, s2);
}
#endif 

// bool lowerLexComp(char c1, char c2)
// {
//     return tolower(c1) == tolower(c2);
// }

struct LogEntries
{
    long long timestampInt;
    string timestamp;
    string message;
    string category;
    unsigned int entryID = 0;
};


// struct timeComp
// {
//     bool operator()(const LogEntries lhs, const LogEntries rhs) const
//     {
//         return ((lhs.timestampInt < rhs.timestampInt));
//     }
// };

struct logComp
{
    bool operator()(const LogEntries& lhs, const LogEntries& rhs) const
    {
        // transform(lhs.category.begin(), lhs.category.end(), lhs.category.begin(), ::tolower);
        // transform(rhs.category.begin(), rhs.category.end(), rhs.category.begin(), ::tolower);

        // return ( (lhs.timestampInt < rhs.timestampInt) ||
        //         ( (lhs.timestampInt == rhs.timestampInt) && (lhs.categoryLower.compare(rhs.categoryLower) < 0) ) ||
        //         ( (lhs.timestampInt == rhs.timestampInt) && (lhs.categoryLower.compare(rhs.categoryLower) == 0) && (lhs.entryID < rhs.entryID)) );

        return ( (lhs.timestampInt < rhs.timestampInt) ||
                ( (lhs.timestampInt == rhs.timestampInt) && (strcasecmp(lhs.category.c_str(), rhs.category.c_str()) < 0) ) ||
                ( (lhs.timestampInt == rhs.timestampInt) && (strcasecmp(lhs.category.c_str(), rhs.category.c_str()) == 0) && (lhs.entryID < rhs.entryID)) );

               // ((lhs.timestampInt == rhs.timestampInt) && 
               //  (lexicographical_compare(lhs.category.begin(), lhs.category.end(), rhs.category.begin(), rhs.category.end(), lowerLexComp))));
    }
};

struct lowerComp
{
    bool operator()(const LogEntries& val, long long time) const
    {
        if(val.timestampInt < time) return true;
        return false;
        // return((val.timestampInt <= time) ||
        //     (val.timestampInt == time) && (val.))
    }
};

struct upperComp
{
    bool operator()(long long time, const LogEntries& val) const
    {
        if(val.timestampInt > time) return true;
        return false;
    }
};

string toLower(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

long long convertTime(string s)
{

    s.erase(std::remove(s.begin(), s.end(), ':'), s.end());
    long long timestampInt = stoll(s);

    return timestampInt;
}


int main(int argc, char** argv)
{
    argc = argc;
    
    string helpCommand = "-h";
    string helpCommandLong = "--help";
    if (argv[1] == helpCommand || argv[1] == helpCommandLong)
    {
        cout << "This program is ..." << '\n';
        exit(0);
    }

    ifstream logFile;
    string ifname;
    ifname = argv[1];

    logFile.open(ifname.c_str());


    bool readSuccess = false;
    while (!readSuccess)
    {
        if (logFile.fail())
        {
            cout << "Error unable to open file! " << ifname << '\n';
            cin >> ifname;
            logFile.open(ifname.c_str());
        }

        else
        {
            readSuccess = true;
        }
    }
    
    // unordered_map<int, vector<>> logEntries;
    // unordered_map<string, vector<int>> timestampMap;
    unordered_map<string, vector<size_t>> categoryMap;
    unordered_map<string, vector<size_t>> keywordMap;

    vector<LogEntries> logEntriesVec;
    // vector<LogEntries> logEntriesVecOriginal;
    // vector<long long int> timestampIntVec;
    deque<size_t> excerptList;
    vector<size_t> searchIndices;

    // vector<vector<LogEntries>> searchResults;

    string line;
    // size_t i =0
    int numOfEntries=0;
    

    while (getline(logFile, line))
    {
        // string timestamp;
        // string category;
        // string message;
        LogEntries entry;
        entry.entryID = numOfEntries;

        stringstream linestream(line);
        getline(linestream, entry.timestamp, '|');
        // auto it = timestampMap.find(entry.timestamp);
        // if(it != timestampMap.end())
        // {
        //     it->second.push_back(entry.entryID);
        // }
        // else
        // {
        //     vector<int> v;
        //     v.push_back(entry.entryID);
        //     timestampMap.insert(make_pair(entry.timestamp, v));
        // }
        // cout << entry.timestamp << endl;
        // string::size_type sz = 1;

        // string timestampCopy = entry.timestamp;
        // // cout << timestampCopy << endl;

        // long long lresult = 0;

        // for(int i=0; i<5; i++)
        // {
        //     long long ll = stoll(timestampCopy, &sz);

        //     double tenth = pow(10.0,8.0-(2.0*i));
        //     long long tenths = static_cast<long long>(tenth);
            

        //     lresult += ll*tenths;
        //     if(timestampCopy.size() == 2)
        //     {
        //         break;
        //     }
        //     timestampCopy = timestampCopy.substr(3);   
        // }
        entry.timestampInt = convertTime(entry.timestamp);
        // timestampIntVec.push_back(lresult);
        // cout << lresult << endl;    
        
        // cout << x << endl;
        getline(linestream, entry.category, '|');
        // entry.categoryLower = toLower(entry.category);
        // transform(entry.categoryLower.begin(), entry.categoryLower.end(), entry.categoryLower.begin(), ::tolower);
        getline(linestream, entry.message, '\n');
        // entry.messageLower = toLower(entry.message);
        // transform(entry.messageLower.begin(), entry.messageLower.end(), entry.messageLower.begin(), ::tolower);

        logEntriesVec.push_back(entry);

        // cout << logEntriesVec[numOfEntries].timestamp << '|' << logEntriesVec[numOfEntries].category << '|' << logEntriesVec[numOfEntries].message << endl;



        numOfEntries++;
    }


    // for (auto it = timestampMap.begin(); it != timestampMap.end(); ++it)
    // {
    //     cout << it->first << " : " << it->second[0] << endl;
    // }

    // sort(logEntriesVec.begin(), logEntriesVec.end(), timeComp());
    // for(int i=0; i<logEntriesVec.size(); i++)
    // {
    //     cout << logEntriesVec[i].timestamp << '|' << logEntriesVec[i].category << '|' << logEntriesVec[i].message << endl;
    // }
    // logEntriesVecOriginal = logEntriesVec;
    sort(logEntriesVec.begin(), logEntriesVec.end(), logComp()); // logComp()
    vector<size_t> afterSortedIndex;
    afterSortedIndex.reserve(logEntriesVec.size());

    // for(auto it=logEntriesVec.begin(); it!=logEntriesVec.end(); it++)
    // {

    // }


    string categoryLower;
    string messageLower;



    
    for(size_t i=0; i<logEntriesVec.size(); i++)
    {
        // auto it = timestampMap.find(logEntriesVec[i].timestamp);
        // if(it != timestampMap.end())
        // {
        //     it->second.push_back(i);
        // }
        // else
        // {
        //     vector<int> v;
        //     v.push_back(i);
        //     timestampMap.insert(make_pair(logEntriesVec[i].timestamp, v));
        // }
        // afterSortedIndex.push_back(logEntriesVec[i].entryID);

        //find entryID
        LogEntries cit = logEntriesVec[i];
        // cout << "check point" << endl;
        afterSortedIndex[cit.entryID] = i;
        // cout << "check point" << endl;




        categoryLower = toLower(logEntriesVec[i].category);
        auto it = categoryMap.find(categoryLower);
        if(it != categoryMap.end())
        {
            it->second.push_back(i);
        }
        else
        {
            vector<size_t> v;
            v.push_back(i);
            categoryMap.insert(make_pair(categoryLower, v));
        }
        string categoryalnum = "";
        for(size_t j=0; j<categoryLower.length(); j++)
        {
            if(isalnum(categoryLower[j]))
            {
                categoryalnum = categoryalnum + categoryLower[j];
            }
            else if((!(isalnum(categoryLower[j])))
                && (categoryalnum != ""))
            {
                auto it = keywordMap.find(categoryalnum);
                if((it != keywordMap.end()))
                {
                    if(i != it->second.back())
                    {
                        it->second.push_back(i);
                    }
                    
                }
                else
                {
                    vector<size_t> s;
                    s.push_back(i);
                    keywordMap.insert(make_pair(categoryalnum, s));
                }
                categoryalnum = "";
            }
            if((j == categoryLower.length()-1)
                && (categoryalnum != ""))
            {
                auto it = keywordMap.find(categoryalnum);
                if(it != keywordMap.end())
                {
                    if(i != it->second.back())
                    {
                        it->second.push_back(i);
                    }
                    
                }
                else
                {
                    vector<size_t> s;
                    s.push_back(i);
                    keywordMap.insert(make_pair(categoryalnum, s));
                }
            }
        }






        messageLower = toLower(logEntriesVec[i].message);

        string messagealnum = "";
        for(size_t j=0; j<messageLower.length(); j++)
        {
            if(isalnum(messageLower[j]))
            {
                messagealnum = messagealnum + messageLower[j];
            }
            else if((!(isalnum(messageLower[j])))
                && (messagealnum!=""))
            {
                auto it = keywordMap.find(messagealnum);
                if(it != keywordMap.end())
                {
                    if(i != it->second.back())
                    {
                        it->second.push_back(i);
                    }
                    
                }
                else
                {
                    vector<size_t> s;
                    s.push_back(i);
                    keywordMap.insert(make_pair(messagealnum, s));
                }
                messagealnum = "";
            }
            if((j == messageLower.length()-1)
                && (messagealnum!=""))
            {
                auto it = keywordMap.find(messagealnum);
                if(it != keywordMap.end())
                {
                    if(i != it->second.back())
                    {
                        it->second.push_back(i);
                    }
                    
                }
                else
                {
                    vector<size_t> s;
                    s.push_back(i);
                    keywordMap.insert(make_pair(messagealnum, s));
                }
            }
        }


    }
    

    cout << numOfEntries << " entries read" << '\n';

    cout << "% ";

    // int lowerBoundIndex_t;
    // int upperBoundIndex_t;

    // char mostRecentSearchCommand;


    char userCommands;
    // int entriesFound = 0;
    bool isSearched = false;
    while ((cin >> userCommands) && (userCommands!='q'))
    {
        if( (userCommands!='t') 
            && (userCommands!='m') 
            && (userCommands!='c')
            && (userCommands!='k')
            && (userCommands!='a')
            && (userCommands!='r')
            && (userCommands!='d')
            && (userCommands!='b')
            && (userCommands!='e')
            && (userCommands!='s')
            && (userCommands!='l')
            && (userCommands!='g')
            && (userCommands!='p')
            && (userCommands!='q')
            && (userCommands!='#') )
        {
            cerr << "Error: wrong command." << '\n';
            cout << "% ";
        }
        switch (userCommands)
        {
            case 't': //timestamp search
            {
                // sort(logEntriesVec.begin(), logEntriesVec.end(), logComp()); // logComp()

                // for(size_t i=0; i<logEntriesVec.size(); i++)
                // {
                //     cout << logEntriesVec[i].timestamp << '|' << logEntriesVec[i].category << '|' << logEntriesVec[i].message << endl;
                // }
                searchIndices.clear();
                string timestamp1;
                string timestamp2;

                // cout << userCommands << endl;
                
                cin >> ws;
                // stringstream linestreams()
                getline(cin, timestamp1, '|');


                cin >> timestamp2;

                if((timestamp1.length()!=14) || (timestamp2.length()!=14))
                {
                    cerr << "timestamp out of range." << '\n';
                    cout << "% ";
                    break;
                }
                // cout << timestamp1 << "|" << timestamp2 << endl;
                long long lresult1 = 0;
                long long lresult2 = 0;
                lresult1 = convertTime(timestamp1);
                lresult2 = convertTime(timestamp2);
                // string::size_type sz = 1;
                // for(int i=0; i<5; i++)
                // {
                //     long long ll = stoll(timestamp1, &sz);
                //     double tenth = pow(10.0,8.0-(2.0*i));
                //     long long tenths = static_cast<long long>(tenth);
                    

                //     lresult1 += ll*tenths;
                //     if(timestamp1.size() == 2)
                //     {
                //         break;
                //     }
                //     timestamp1 = timestamp1.substr(3);   
                // }
                // for(int i=0; i<5; i++)
                // {
                //     long long ll = stoll(timestamp2, &sz);
                //     double tenth = pow(10.0,8.0-(2.0*i));
                //     long long tenths = static_cast<long long>(tenth);
                    

                //     lresult2 += ll*tenths;
                //     if(timestamp2.size() == 2)
                //     {
                //         break;
                //     }
                //     timestamp2 = timestamp2.substr(3);   
                // }

                // cout << lresult1 << "|" << lresult2 << endl;
                //make a function for converting timestamp into long long int later

                // cout << lresult1 << " " << lresult2 << endl;

                auto lower = lower_bound(logEntriesVec.begin(), logEntriesVec.end(), lresult1, lowerComp());
                auto upper = upper_bound(logEntriesVec.begin(), logEntriesVec.end(), lresult2, upperComp());
                // upper = upper_bound(upper, logEntriesVec.end(), lresult2, upperComp())
                auto lowerIndex = lower - logEntriesVec.begin();;
                int lowerIndexInt = static_cast<int>(lowerIndex);
                // lowerBoundIndex_t = static_cast<int>(lowerIndex);
                // int lowerIndex = distance(logEntriesVec.begin(), lower);
                auto upperIndex = upper - logEntriesVec.begin();
                int upperIndexInt = static_cast<int>(upperIndex);
                // upperBoundIndex_t = static_cast<int>(upperIndex);
                // int upperIndex = distance(logEntriesVec.begin(), upper);
                // cout << lowerIndex << " " << upperIndex << endl;
                // cout << lowerIndexInt << ", " << upperIndexInt << endl;
                // searchIndices.clear();
                for(int i=lowerIndexInt; i<upperIndexInt; i++)
                {
                    searchIndices.push_back(i);
                }

                // mostRecentSearchCommand = 't';

                
                // entriesFound = upperIndexInt - lowerIndexInt;
                cout << "Timestamps search: " << upperIndexInt - lowerIndexInt << " entries found" << '\n';

                isSearched = true;

                // cout << lowerIndex << " " << upperIndex << endl;
                // cout << lower->timestamp << endl;
                // cout << upper->timestamp << endl;

                // cout << timestamp1 << '|' << timestamp2 << endl;

                cout << "% ";
                break;
            }
            case 'm':
            {
                searchIndices.clear();
                string timestampMatching;
                cin >> ws;
                cin >> timestampMatching;

                if(timestampMatching.length()!=14)
                {
                    cerr << "timestamp out of range." << '\n';
                    cout << "% ";
                    break;
                }


                long long lresult1 = 0;
                // string::size_type sz = 1;
                // for(int i=0; i<5; i++)
                // {
                //     long long ll = stoll(timestampMatching, &sz);
                //     double tenth = pow(10.0,8.0-(2.0*i));
                //     long long tenths = static_cast<long long>(tenth);
                    

                //     lresult1 += ll*tenths;
                //     if(timestampMatching.size() == 2)
                //     {
                //         break;
                //     }
                //     timestampMatching = timestampMatching.substr(3);   
                // }

                lresult1 = convertTime(timestampMatching);

                auto lower = lower_bound(logEntriesVec.begin(), logEntriesVec.end(), lresult1, lowerComp());
                auto upper = upper_bound(logEntriesVec.begin(), logEntriesVec.end(), lresult1, upperComp());


                // auto it = timestampMap.find(timestampMatching);
                if (lower == logEntriesVec.end())
                {
                    // entriesFound = 0;
                    cout << "Timestamp search: " << 0 << " entries found" << '\n';
                }
                else
                {
                    auto lowerIndex = lower - logEntriesVec.begin();
                    auto upperIndex = upper - logEntriesVec.begin();

                    int lowerIndexInt = static_cast<int>(lowerIndex);
                    int upperIndexInt = static_cast<int>(upperIndex);

                    // entriesFound = upperIndexInt - lowerIndexInt;
                    cout << "Timestamp search: " << upperIndexInt - lowerIndexInt << " entries found" << '\n';
                    
                    for(int i=lowerIndexInt; i<upperIndexInt; i++)
                    {
                        searchIndices.push_back(i);
                    }
                }
                isSearched = true;
                cout << "% ";
                break;
            }
            case 'c':
            {
                searchIndices.clear();
                string category;
                cin >> ws;
                getline(cin, category);
                transform(category.begin(), category.end(), category.begin(), ::tolower);
                // cout << category << endl;
                auto it = categoryMap.find(category);
                if (it == categoryMap.end())
                {
                    // entriesFound = 0;
                    cout << "Category search: " << 0 << " entries found" << '\n';
                }
                else
                {
                    // entriesFound = static_cast<int>(it->second.size());
                    cout << "Category search: " << it->second.size() << " entries found" << '\n';
                    
                    // for(int i=0; i<entriesFound; i++)
                    // {
                    //     searchIndices.push_back(it->second[i]);
                    // }
                    searchIndices = it->second;
                } 
                isSearched = true;
                cout << "% ";
                break;
            }
            case 'k':
            {
                searchIndices.clear();
                string keywords;
                cin >> ws;
                getline(cin, keywords);
                transform(keywords.begin(), keywords.end(), keywords.begin(), ::tolower);
                // cout << keywords << endl;
                vector<string> keywordsVec;


                string keyword = "";
                for(size_t i=0; i<keywords.length(); i++)
                {

                    if(isalnum(keywords[i]))
                    {      
                        // keyword.push_back(keywords[i]);
                        keyword = keyword + keywords[i];
                    }
                    else if(!(isalnum(keywords[i])))
                    {
                        if(keyword != "")
                        {
                            keywordsVec.push_back(keyword);
                        }
                        keyword = "";
                    }
                    if((i == keywords.length()-1)
                        && (keyword != ""))
                    {
                        keywordsVec.push_back(keyword);
                    }
                }
                // for(size_t i=0; i<keywordsVec.size(); i++)
                // {
                //     cout << keywordsVec[i] << endl;
                // }


                // bool isKeyword = true;
                // set<int> intersectFinal;
                // vector<size_t> intersectSet;
                // vector<size_t> currentSet;
                
                for(size_t i=0; i<keywordsVec.size(); i++)
                {
                    auto it = keywordMap.find(keywordsVec[i]);
                    if (it == keywordMap.end())
                    {
                        // intersectSet.clear();
                        // entriesFound = 0;
                        // cout << "Keyword search: " << entriesFound << " entries found" << '\n'; 
                        searchIndices.clear();
                        break;
                    }
                    else
                    {
                        
                        if(i==0)
                        {
                            searchIndices = it->second;
                        }
                        else
                        {
                            vector<size_t> intersectFinalTemp;
                            // currentSet = it->second;
                            set_intersection(searchIndices.begin(), searchIndices.end(), it->second.begin(), it->second.end(),
                            inserter(intersectFinalTemp, intersectFinalTemp.begin()));
                            searchIndices = intersectFinalTemp;
                            // intersectFinalTemp.empty();
                        }
                        
                    }//else
                   
                }//for loop
                // entriesFound = (searchIndices.size());
                
                // for (auto it = intersectSet.begin(); it!= intersectSet.end(); it++)
                // {
                //     searchIndices.push_back(*it);
                // }
                cout << "Keyword search: " << searchIndices.size() << " entries found" << '\n';
                // cout << keywordsVec[1] << endl;
                // for(int i=0; i<keywordsVec.size(); i++)
                // {
                //     cout << keywordsVec[i] << endl;
                // }
                isSearched = true;
                cout << "% ";
                break;


            }
            case 'a':
            {
                cin >> ws;
                unsigned int entryNum;
                cin >> entryNum;
                if(entryNum >= logEntriesVec.size())
                {
                    cerr << "entry number out of range" << '\n';
                    cout << "% ";
                    break;
                }
                if((entryNum < logEntriesVec.size()))
                {
                    cout << "log entry " << entryNum << " appended" << '\n';
                    excerptList.push_back(afterSortedIndex[entryNum]);
                    // excerptList.push_back(logEntriesVecOriginal[entryNum]);
                    // for(auto it=logEntriesVec.begin(); it!=logEntriesVec.end(); it++)
                    // {
                    //     if(logEntriesVec[it-logEntriesVec.begin()].entryID == entryNum)
                    //     {
                    //         excerptList.push_back(it-logEntriesVec.begin());
                    //     }
                    // }

                    // excerptList.push_back(logEntriesVecOriginal[entryNum]);
                }
                

                cout << "% ";
                break;
            }
            case 'r':
            {
                if(isSearched == false)
                {
                    cerr << "no previous search occured." << '\n';
                    cout << "% ";
                    break;
                }
                // if((mostRecentSearchCommand == 't') || (mostRecentSearchCommand == 'm'))
                // {
                //     for(size_t i = lowerBoundIndex_t; i < upperBoundIndex_t; i++)
                //     {
                //         excerptList.push_back(i);
                //     }
                // }
                for(size_t i=0; i<searchIndices.size(); i++)
                {
                    // size_t index = searchIndices[i];
                    excerptList.push_back(searchIndices[i]);
                }
                cout << searchIndices.size() << " log entries appended" << '\n';

                cout << "% ";
                break;
            }
            case 'd':
            {
                cin >> ws;
                unsigned int listIndex;
                cin >> listIndex;

                if(listIndex >= excerptList.size())
                {
                    cerr << "list index out of range." << '\n';
                    cout << "% ";
                    break;
                }

                excerptList.erase(excerptList.begin() + listIndex);
                cout << "Deleted excerpt list entry " << listIndex << '\n';
                cout << "% ";
                break;
            }
            case 'b':
            {
                cin >> ws;
                unsigned int listIndex;
                cin >> listIndex;

                if(listIndex >= excerptList.size())
                {
                    cerr << "list index out of range." << '\n';
                    cout << "% ";
                    break;
                }

                size_t temp;
                temp = excerptList[listIndex];

                excerptList.erase(excerptList.begin() + listIndex);
                excerptList.push_front(temp);

                cout << "Moved excerpt list entry " << listIndex << '\n';

                cout << "% ";
                break;
            }
            case 'e':
            {
                cin >> ws;
                unsigned int listIndex;
                cin >> listIndex;

                if((listIndex >= excerptList.size()))
                {
                    cerr << "list index out of range." << '\n';
                    cout << "% ";
                    break;
                }

                size_t temp;
                temp = excerptList[listIndex];

                excerptList.erase(excerptList.begin() + listIndex);
                excerptList.push_back(temp);

                cout << "Moved excerpt list entry " << listIndex << '\n'; 

                cout << "% ";
                break;
            }
            case 's':
            {
                cout << "excerpt list sorted" << '\n';
                if(excerptList.empty())
                {     
                    cout << "(previously empty)" << '\n';
                }
                else
                {
                    cout << "previous ordering:" << '\n';
                    cout << 0 << '|' << logEntriesVec[excerptList[0]].entryID << '|' << logEntriesVec[excerptList[0]].timestamp << '|' << logEntriesVec[excerptList[0]].category << '|' << logEntriesVec[excerptList[0]].message << '\n';
                    cout << "..." << '\n';
                    cout << excerptList.size() - 1 << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].entryID << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].timestamp << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].category << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].message << '\n';
                    sort(excerptList.begin(), excerptList.end());
                    cout << "new ordering:" << '\n';
                    cout << 0 << '|' << logEntriesVec[excerptList[0]].entryID << '|' << logEntriesVec[excerptList[0]].timestamp << '|' << logEntriesVec[excerptList[0]].category << '|' << logEntriesVec[excerptList[0]].message << '\n';
                    cout << "..." << '\n';
                    cout << excerptList.size() - 1 << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].entryID << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].timestamp << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].category << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].message << '\n';
                }
                

                cout << "% ";
                break;
            }
            case 'l':
            {
                cout << "excerpt list cleared" << '\n';
                if(excerptList.empty())
                {
                    cout << "(previously empty)" << '\n';
                }
                else
                {
                    cout << "previous contents:" << '\n';
                    cout << 0 << '|' << logEntriesVec[excerptList[0]].entryID << '|' << logEntriesVec[excerptList[0]].timestamp << '|' << logEntriesVec[excerptList[0]].category << '|' << logEntriesVec[excerptList[0]].message << '\n';
                    cout << "..." << '\n';
                    cout << excerptList.size() - 1 << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].entryID << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].timestamp << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].category << '|' << logEntriesVec[excerptList[excerptList.size() - 1]].message << '\n';
                    excerptList.clear();

                }
                cout << "% ";
                break;
            }
            case 'g':
            {
                if(isSearched == false)
                {
                    cerr << "no previous search occured." << '\n';
                    cout << "% ";
                    break;
                }
                for(size_t i=0; i<searchIndices.size(); i++)
                {
                    // size_t index = searchIndices[i];
                    cout << logEntriesVec[searchIndices[i]].entryID << '|' << logEntriesVec[searchIndices[i]].timestamp << '|' << logEntriesVec[searchIndices[i]].category << '|' << logEntriesVec[searchIndices[i]].message << '\n';
                }
                cout << "% ";
                break;
            }
            case 'p':
            {
                for(size_t i=0; i<excerptList.size(); i++)
                {
                    cout << i << '|' << logEntriesVec[excerptList[i]].entryID << '|' << logEntriesVec[excerptList[i]].timestamp << '|' << logEntriesVec[excerptList[i]].category << '|' << logEntriesVec[excerptList[i]].message << '\n';
                }
                
                cout << "% ";
                break;
            }
            case 'q':
            {
                break;
            }
            case '#':
            {
                string junk;
                getline(cin, junk);
                cout << "% ";
                break;
            }
        }
    }

    return 0;
}



