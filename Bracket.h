#ifndef BRACKET_H
#define BRACKET_H
#include <vector>
#include <string>

struct team;
struct matchData
{
    int weight; // Only Needed for the big, commented out, createUnnamedTeams function.
    int buildCount; // Same as above. Otherwise useless.
    bool winnerDeclared=false;
    team* leftTeam;
    team* rightTeam;
    matchData* winnerMatch;
    matchData* rightMatch;
    matchData* leftMatch;
    team* Winner = NULL;
    int matchNumber; // First seed match against last seed will always be 1st, second match will be between the two closest seeds, third will be Second seed and second last, etc.
    int round; //finals will always be log2(numberofTeams), semis that-1, etc.
    matchData(){};
    matchData(team* left, team* right, int mNumber)
    {
        leftTeam = left;
        rightTeam = right;
        matchNumber = mNumber;
    }
};
struct team
{
    matchData* currentMatch;
    std::string teamName;
    int seedRank;
    team(){};

    team(int in_seedRank, std::string in_teamName, matchData* startingMatch)
    {
        seedRank = in_seedRank;
        teamName = in_teamName;
        currentMatch = startingMatch;
    }

};


class Bracket
{
    public:
        Bracket();
        Bracket(int teams, std::string fileName);
        Bracket(int teams);
        virtual ~Bracket();
        void BFCreateMatches(int Teams, int topLevel);
        void printMatchNumbers();
        void CreateUnnamedTeams();
        void printMatchups();
        void declareWinner(std::string teamName);
        void UpdateWins();
        void printTeams();
        void renameTeam(std::string oldName, std::string newName);
        void RandomizeStartingPositions();
    protected:
    private:
        void teamPlacementNoNames();
        void teamPlacementNames(std::string fileName);
        int numTeams;
        int currentRound;
        void printMatchNumbers(matchData* currentMatch);
        matchData* Finals;
        int buildCounter;
        std::string BracketType;
        std::vector<matchData*> matches;
        std::vector<team*> teams;
};

#endif // BRACKET_H
