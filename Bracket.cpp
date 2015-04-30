#include "Bracket.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <climits>
#include <time.h>

using namespace std;

//Check README for general descriptions of functions.

Bracket::Bracket(int Teams)
{
    srand (time(NULL));
    currentRound = 1;
    double logStore = log2(Teams)+.999;
    numTeams = Teams;
    int FinalsRound = (int)logStore;
    int numMatches = pow(2, FinalsRound);
    matches.resize(numMatches-1);
    BFCreateMatches(numMatches, FinalsRound);
    teamPlacementNoNames();
}

Bracket::Bracket(int Teams, string fileName)
{
    srand (time(NULL));
    currentRound = 1;
    double logStore = log2(Teams)+.999;
    numTeams = Teams;
    int FinalsRound = (int)logStore;
    int numMatches = pow(2, FinalsRound);
    matches.resize(numMatches-1);
    BFCreateMatches(numMatches, FinalsRound);
    teamPlacementNames(fileName);
}

Bracket::~Bracket()
{
    //cout << "Deleting Bracket" << endl;
    teams.erase(teams.begin(),teams.end());
    //cout << "teams deleted" << endl;
    matches.erase(matches.begin(),matches.end());
}

void Bracket::UpdateWins()
{
    bool nextRound = true;
    for(int i = 0; i < matches.size(); i++)
    {
        if(matches[i]->round == currentRound and matches[i]->winnerDeclared == false)
        {//only updates rounds where the winner hasn't already been declared and are part of the current round
            if(matches[i]->leftTeam == NULL)//First, update matches that don't have Opponents.
            {
                matches[i]->Winner = matches[i]->rightTeam; //winner is auto set, and placed in the corresponding spot in the next match.
                matches[i]->winnerDeclared = true;
                if(matches[i]->rightTeam != NULL)
                {
                    matches[i]->rightTeam->currentMatch = matches[i]->winnerMatch;
                }
                if(matches[i]->winnerMatch!= NULL)
                {
                    if(matches[i]->winnerMatch->leftMatch == matches[i])
                    {
                        matches[i]->winnerMatch->leftTeam = matches[i]->Winner;
                    }else{
                        matches[i]->winnerMatch->rightTeam = matches[i]->Winner;
                    }
                }
            }else if(matches[i]->rightTeam == NULL){ //Same as previous if but right side instead of left.
                matches[i]->Winner = matches[i]->leftTeam;
                matches[i]->winnerDeclared = true;
                matches[i]->leftTeam->currentMatch = matches[i]->winnerMatch;
                if(matches[i]->winnerMatch!= NULL)
                {
                    if(matches[i]->winnerMatch->leftMatch == matches[i])
                    {
                        matches[i]->winnerMatch->leftTeam = matches[i]->Winner;
                    }else{
                        matches[i]->winnerMatch->rightTeam = matches[i]->Winner;
                    }
                }
            }else if(matches[i]->Winner != NULL){//If both teams exist, set the "Winner as declared" and update the next match.
                matches[i]->winnerDeclared = true;
                matches[i]->Winner->currentMatch = matches[i]->winnerMatch;
                if(matches[i]->winnerMatch!= NULL) //Place in corresponding spot
                {
                    if(matches[i]->winnerMatch->leftMatch == matches[i])
                    {
                        matches[i]->winnerMatch->leftTeam = matches[i]->Winner;
                    }else{
                        matches[i]->winnerMatch->rightTeam = matches[i]->Winner;
                    }
                }
            }
            nextRound = false;
        }
    }
    if(nextRound == true)
    {//If every match in the round has a winner declared, we can go to the next round.
        currentRound++;
        cout << "Round Finished! Moving to Round: " << currentRound << endl;
    }
    if(Finals->Winner != NULL)
    { // If the Finals have a winner, all matches have been completed.
        cout << "We have a winner! Team: " << Finals->Winner->teamName;
    }
}

void Bracket::declareWinner(string teamName)
{
    int teamNumber = -1;
    for(int i = 0; i < teams.size(); i++)//Search the teams for the selected team
    {
        if(teams[i]->teamName == teamName)
        {
            teamNumber = i;
            break;
        }
    }
    
    //Either Declare the found team the winner of their current match or return an error.
    if(teamNumber == -1)
    {
        cout << "Team " << teamName << " not found. Check spelling and retry." << endl;
        return;
    }else{
        cout << "Winner Declared: " << teams[teamNumber]->teamName << "! "<< endl;
        teams[teamNumber]->currentMatch->Winner = teams[teamNumber];
    }
    UpdateWins();
}

void Bracket::renameTeam(string teamName, string newName)
{
    //Works almost exactly the same as the above, except it renames the current match.
    int teamNumber = -1;
    for(int i = 0; i < teams.size(); i++)
    {
        if(teams[i]->teamName == teamName)
        {
            teamNumber = i;
            break;
        }
    }
    if(teamNumber == -1)
    {
        cout << "Team " << teamName << " not found. Check spelling and retry." << endl;
        return;
    }else{
        teams[teamNumber]->teamName = newName;
        cout << "Team " << teamName << "Renamed!" << endl;
    }
    UpdateWins();
}

//This big ass commented out function was meant to be for placing teams in order of seeding.
//But, it doesn't work. so that sucks. I'd love to see someone get it working!
/*void Bracket::CreateUnnamedTeams()
{
    teams.resize(numTeams);
    for(int i = 0; i < matches.size()-1; i++)
    {
        matches[i]->weight = INT_MAX;
        matches[i]->buildCount = 0;
    }
    for(int i = 0; i < teams.size(); i++)
    {
        teams[i] = new team;
        teams[i]->seedRank = i+1;
        teams[i]->teamName = "butts";
        cout << teams[i]->teamName << teams[i] << " " << i << endl;
    }
    int Counter = 0;
    int numPrelims = pow(2, Finals->round-1);
    matchData* miterator = NULL;
    miterator = Finals;
    int tieBreaker = 4;
    int fixesNeeded = 0;
    int breakTies = 0;
    for(int i = 0; i < teams.size(); i++)
    {
        cout << "% thing" << i%tieBreaker << endl;
        if(i%tieBreaker == 0 and i != 0)
        {
            cout << "in if!" << log2(tieBreaker)-1 << endl;
            breakTies = log2(tieBreaker)-1;
            fixesNeeded = breakTies;
            tieBreaker = tieBreaker*2;
        }
        cout << "~~~~~~~~~~~~~placing " << i+1 <<"~~~~~~~~~~~~~~"<< endl;
        while(miterator->leftMatch != NULL)
        {
            cout << "current match" << miterator->matchNumber << endl;
            if(miterator->leftMatch->buildCount < miterator->rightMatch->buildCount)
            {
                cout << "going left because buildCount" << endl;
                miterator = miterator->leftMatch;
            }
            else if(miterator->leftMatch->buildCount > miterator->rightMatch->buildCount)
            {
                miterator = miterator->rightMatch;
                cout << "going right because buildCount" << endl;
            }
            else
            {
                cout << fixesNeeded << "Fixes Left" << endl;
                if(breakTies == 0)
                {
                    if(miterator->leftMatch->weight >= miterator->rightMatch->weight)
                    {
                        miterator = miterator->leftMatch;
                        cout << "going left because weight" << endl;
                    }
                    else
                    {
                        //if((i%pow(2,Finals->round) = 1)
                        miterator = miterator->rightMatch;
                        cout << "going right because weight" << endl;

                    }
                }else{
                    cout << "Creating tieBreak" << endl;
                    breakTies = fixesNeeded;
                    while(breakTies > 0)
                    {
                        miterator = miterator->leftMatch;
                        breakTies--;
                        cout << "gone left" << endl;
                    }
                }
            }
        }
        cout << "location found!" << endl;
        cout << miterator->matchNumber << endl;
        if(miterator->leftTeam == NULL)
        {
            cout << "left is Null" << endl;
            miterator->leftTeam = teams[i];
        }
        else
        {
            miterator->rightTeam = teams[i];
        }
        //cout << teams[i] << endl;
        teams[i]->currentMatch = miterator;
        //cout << miterator->winnerMatch->matchNumber << endl;
        cout << "before while" << endl;
        while(miterator->winnerMatch != NULL)
        {
            if(miterator->weight > teams[i]->seedRank)
            {
                miterator->weight = teams[i]->seedRank;
            }
            miterator->buildCount++;
            miterator = miterator->winnerMatch;
        }
        if(fixesNeeded > 0)
        {
            fixesNeeded--;
            breakTies = fixesNeeded;
        }
    }
}*/

void Bracket::RandomizeStartingPositions()
{
    team* Placeholder = NULL;
    matchData* matchPlaceholder = NULL;
    for(int i = 0; i < numTeams; i++)
    { //Performs "Swaps" between random teams. doing it as mean times as there is teams is realitively arbitarary, so  this could be changed.
        srand(time(NULL)+i);
        int random1 = rand()%numTeams;
        Placeholder = teams[random1];
        matchPlaceholder = teams[random1]->currentMatch;
        srand(time(NULL)+i+20);
        int random2 = rand()%numTeams;
        if(teams[random1]->currentMatch != teams[random2]->currentMatch)
        {
            if(teams[random1] == NULL)
            {
                //cout << "Swapping teams: " << "Empty" << " and " << teams[random2]->teamName << endl;
            }else if(teams[random2] == NULL){
                //cout << "Swapping teams: " << teams[random1]->teamName << " and " << "Empty" << endl;
            }else{
                //cout << "Swapping teams: " << teams[random1]->teamName << " and " << teams[random2]->teamName << endl;
            }

            if(teams[random1]->currentMatch->leftTeam == teams[random1])
            {
                teams[random1]->currentMatch->leftTeam = teams[random2];
            }else{
                teams[random1]->currentMatch->rightTeam = teams[random2];
            }
            teams[random1]->currentMatch = teams[random2]->currentMatch;

            if(teams[random2]->currentMatch->leftTeam == teams[random2])
            {
                teams[random2]->currentMatch->leftTeam = Placeholder;
            }else{
                teams[random2]->currentMatch->rightTeam = Placeholder;
            }
            teams[random2]->currentMatch = matchPlaceholder;
            teams[random1] = teams[random2];
            teams[random2] = Placeholder;
        }
    }
    return;
}

void Bracket::teamPlacementNoNames()
{
    teams.resize(numTeams);
    stringstream inStr;
    for(int i = 0; i < teams.size(); i++)
    {
        teams[i] = new team;
        inStr << i+1;
        teams[i]->seedRank = i+1;
        teams[i]->teamName = inStr.str();
        inStr.str(std::string());
        inStr.clear();
        //cout << teams[i]->teamName << " " << i << endl;
    }
    int numPrelims = pow(2, Finals->round-1)-1;
    cout << numPrelims << endl;
    int matchCounter = 0;
    for(int x = 0; x < numTeams; x++)
    {
        if(x%2 == 0)
        {
            //cout << "Assigning Match " << numPrelims-matchCounter << endl;
           // cout << teams[numTeams-x-1] << endl;
            matches[numPrelims-matchCounter]->leftTeam = teams[numTeams-x-1];
            //cout << "Left Team:" << teams[numTeams-x-1]->seedRank << endl;
            teams[numTeams-x-1]->currentMatch = matches[numPrelims-matchCounter];
        }else{
            //cout << "else" << endl;
            matches[numPrelims-matchCounter]->rightTeam = teams[numTeams-x-1];
            //cout << "Right Team:" << teams[numTeams-x-1]->seedRank << endl;
            teams[numTeams-x-1]->currentMatch = matches[numPrelims-matchCounter];
            matchCounter++;
        }
    }
    RandomizeStartingPositions();
    UpdateWins();
}

void Bracket::teamPlacementNames(string fileName)
{
    //Creates each team and assigns them to their starting location before randomization happens.
    teams.resize(numTeams);
    fstream file;
    file.open(fileName.c_str());
    if(file.fail())
    {
        cout << "file failed." << endl;
        return;
    }
    for(int i = 0; i < teams.size(); i++)
    {

        teams[i] = new team;
        teams[i]->seedRank = i+1;
        getline(file, teams[i]->teamName);
        //cout << teams[i]->teamName << " " << i << endl;
    }
    int numPrelims = pow(2, Finals->round-1)-1;
    cout << numPrelims << endl;
    int matchCounter = 0;
    for(int x = 0; x < numTeams; x++)
    {
        if(x%2 == 0)
        {
            cout << "Assigning Match " << numPrelims-matchCounter << endl;
           // cout << teams[numTeams-x-1] << endl;
            matches[numPrelims-matchCounter]->leftTeam = teams[numTeams-x-1];
            cout << "Left Team:" << teams[numTeams-x-1]->seedRank << endl;
            teams[numTeams-x-1]->currentMatch = matches[numPrelims-matchCounter];
        }else{
            cout << "else" << endl;
            matches[numPrelims-matchCounter]->rightTeam = teams[numTeams-x-1];
            cout << "Right Team:" << teams[numTeams-x-1]->seedRank << endl;
            teams[numTeams-x-1]->currentMatch = matches[numPrelims-matchCounter];
            matchCounter++;
        }
    }
    RandomizeStartingPositions();
    UpdateWins();
}

void Bracket::BFCreateMatches(int Teams, int topLevel)
{
    vector<matchData*> Queue;
    int numMatches = pow(2, topLevel);
    matches[Teams-2] = new matchData;
    //cout << topLevel << endl;
    matches[Teams-2]->round = topLevel;
    Finals = matches[Teams-2];
    int counter = 1;
    matches[Teams-2]->matchNumber = Teams-counter;
    counter++;
    Queue.push_back(matches[Teams-2]);
    //breadth first placement coming up!
    while(Queue.size() != 0)
    {
        matchData* u = Queue.front();
        //cout << "Current Match Getting Children" << u->matchNumber <<" round: " << u->round << endl;
        //cout << counter << " counter" << endl;
        Queue.erase(Queue.begin());
        if(u->round-1 > 0)
        {
            //cout << "creating new matches" << endl;
            u->leftMatch = new matchData;
            u->leftMatch->round = u->round-1;
            u->leftMatch->winnerMatch = u;
            u->rightMatch = new matchData;
            u->rightMatch->round = u->round-1;
            u->rightMatch->winnerMatch = u;
        }
        else
        {
            u->leftMatch = NULL;
            u->rightMatch = NULL;
        }
        if(u->rightMatch != NULL)
        {
            u->rightMatch->matchNumber = Teams-counter;
            matches[Teams-counter-1] = u->rightMatch;
            Queue.push_back(u->rightMatch);
            counter++;
        }
        if(u->leftMatch != NULL)
        {
            u->leftMatch->matchNumber = Teams-counter;
            matches[Teams-counter-1] = u->leftMatch;
            Queue.push_back(u->leftMatch);
            counter++;
        }
    }
    return;
}


//This is just a preorder traversal of the matches. Useful for debugging stuff.
void Bracket::printMatchNumbers()
{
    cout << "Traversing!" << endl;
    cout << Finals->matchNumber << endl;
    if(Finals->leftMatch != NULL)
    {
        //cout << "left" << endl;
        printMatchNumbers(Finals->leftMatch);
    }
    if(Finals->rightMatch != NULL)
    {
        //cout << "right" << endl;
        printMatchNumbers(Finals->rightMatch);
    }
    return;
}
void Bracket::printMatchNumbers(matchData* currentMatch)
{
    cout << currentMatch->matchNumber << endl;
    if(currentMatch->leftMatch != NULL)
    {
        //cout << "left" <<endl;
        printMatchNumbers(currentMatch->leftMatch);
    }
    if(currentMatch->rightMatch != NULL)
    {
        //cout << "right" <<endl;
        printMatchNumbers(currentMatch->rightMatch);
    }
    return;
}


//This just prints data for each match.
void Bracket::printMatchups()
{
    for(int i = 0; i < matches.size(); i++)
    {
        cout << "~~~~~~Match Number: " << matches[i]->matchNumber <<"~~~~~~~~"<< endl;
        if(matches[i]->winnerMatch != NULL)
        {
            cout << "Winner goes to match: " << matches[i]->winnerMatch->matchNumber << endl;;
        }else{
            cout << "Finals" << endl;
        }
        cout << "Round: " << matches[i]->round << endl;
        if(matches[i]->leftTeam != NULL)
        {
            cout << "Left Team: " << matches[i]->leftTeam->teamName << endl;
        }
        else
        {
            cout << "Left Team: None" << endl;
        }
        if(matches[i]->rightTeam != NULL)
        {
            cout << "Right Team: " << matches[i]->rightTeam->teamName << endl;
        }
        else
        {
            cout << "Right Team: None" << endl;
        }
        if(matches[i]->Winner != NULL)
        {
            cout << "Winner: " << matches[i]->Winner->teamName << endl;
        }else{
            cout << "Game Not yet Finished or No Winner Possible" << endl;
        }
    }
    cout << "____________________" << endl;
}

void Bracket::printTeams()
{
    for(int i = 0; i < teams.size(); i++)
    {
        cout << "----Team Name: " << teams[i]->teamName << "----" << endl;
        cout << "Seed: " << teams[i]->seedRank << endl;
        if(teams[i]->currentMatch->Winner == NULL)
        {
            cout << "STATUS: IN" << endl;
            cout << "Next Match: " << teams[i]->currentMatch->matchNumber << endl;
            cout << "Round: " << teams[i]->currentMatch->round << endl;
        }else if(teams[i]->currentMatch->Winner == teams[i]){
            cout << "STATUS: Winner" << endl;
            cout << "Final Match: " << teams[i]->currentMatch->matchNumber << endl;
        }else{
            cout << "STATUS: OUT" << endl;
            cout << "Lost in match: " << teams[i]->currentMatch->matchNumber << endl;
            cout << "Round: " << teams[i]->currentMatch->round << endl;
        }
    }
}
