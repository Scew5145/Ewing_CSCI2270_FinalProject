#include <iostream>
#include "Bracket.h"
#include <cstdlib>

using namespace std;

int main()
{
    Bracket* newBracket = NULL; // New brackets must be pointers if you want to avoid strange, foreign errors. Believe me. It's for the best.
    //newBracket->printTeams();
    int input;
    string rawInput;
    bool bracketBuilt = false;
    string rawInput2;
    bool winsDeclared = false;
    while(input != 10)
    {
        cout << "======Main Menu=====" << endl;
        cout << "1. Print Matches" << endl;
        cout << "2. Print Teams" << endl;
        cout << "3. Declare Winner" << endl;
        cout << "4. Clear Current Bracket" << endl;
        cout << "5. Create new Bracket With Names" << endl;
        cout << "6. Create new Bracket without Names" << endl;
        cout << "7. Update Bracket" << endl;
        cout << "8. Re-Randomize Starting Positions" << endl;
        cout << "9. Rename Team" << endl;
        cout << "10. Quit" << endl;
        getline(cin, rawInput);
        input =  atoi(rawInput.c_str());
        if(input == 1)
        {
            if(bracketBuilt == false)
            {
                cout << "Please Create a new bracket before Printing." << endl;
            }else{
                newBracket->printMatchups();
            }
        }else if(input == 2){
            if(bracketBuilt == false)
            {
                cout << "Please Create a new bracket before Printing." << endl;
            }else{
                newBracket->printTeams();
            }
        }else if(input == 3){
            if(bracketBuilt == false)
            {
                cout << "Please Create a new bracket before Declaring Winners." << endl;
            }else{
                cout << "Enter a team name: " << endl;
                getline(cin, rawInput);
                newBracket->declareWinner(rawInput);
                winsDeclared = true;
            }
        }else if(input == 4){
            if(bracketBuilt == false)
            {
                cout << "How do you delete nothing?" << endl;
            }else{
                delete newBracket;
                bracketBuilt = false;
                winsDeclared = false;
                cout << "Bracket Deleted." << endl;
            }
        }else if(input == 5){
            if(bracketBuilt == false)
            {
                cout << "Enter filename to read Team Names in with: " << endl;
                getline(cin, rawInput);
                cout << "Enter number of teams (Must be less than or equal to number of names in file): " << endl;
                getline(cin, rawInput2);
                newBracket = new Bracket(atoi(rawInput2.c_str()), rawInput);
                bracketBuilt = true;
            }else{
                cout << "bracket already built. Please clear current bracket before building a new one." << endl;
            }
        }else if(input == 6){
            if(bracketBuilt == false)
            {
                cout << "Enter number of teams: " << endl;
                getline(cin, rawInput2);
                newBracket = new Bracket(atoi(rawInput2.c_str()));
                bracketBuilt = true;
            }else{
                cout << "bracket already built. Please clear current bracket before building a new one." << endl;
            }
        }else if(input == 7){
            if(bracketBuilt == false)
            {
                cout << "Please Build Bracket before trying to update." << endl;
            }else{
                newBracket->UpdateWins();
            }
        }else if(input == 8){
            if(bracketBuilt == false)
            {
                cout << "Please Build Bracket before trying to randomize." << endl;
            }else{
                if(winsDeclared == false)
                {
                    newBracket->RandomizeStartingPositions();
                }else{
                    cout << "Can't randomize starting positions once matches have begun." << endl;
                }
            }
        }else if(input == 9){
            if(bracketBuilt == false)
            {
                cout << "Please Build Bracket before trying to rename a Team." << endl;
            }else{
                cout << "Enter team's current name: " << endl;
                getline(cin, rawInput);
                cout << "Enter team's new name: " << endl;
                getline(cin, rawInput2);
                newBracket->renameTeam(rawInput, rawInput2);
            }
        }
    }
    delete newBracket;
    cout << "Goodbye!" << endl;
    return 0;
}
