# CSCI2270 Final Project 
This Project is designed to create a randomly seeded competition bracket. All public methods are documented here. Feel free to bother me with questions if you need help understanding some part of my code that I didn't properly comment.
#How to Install
Compile the main.cpp, Bracket.cpp, and Bracket.h files together. testFile.txt is just an example file for the type of file the program will correctly read. There are no command line arguments, so everything should run as is.

#Dependencies
None.

#Public Functions
Constructors:

  Bracket();
    Constructs an entirely empty bracket. Shouldn't be used under normal circumstances because the Matches tree won't be correctly initialized.
    
  Bracket(numberTeams);
    Constructs the Bracket with default team names of the specified size. Team names will be integers starting at 1 and ending and the number of teams. Seeds will be the Same. Bracket size will always be equal to 2^(finals round)-1 and matches will always be numbered in the following order:
          7           Finals (round 3)
       5      6       Semi-Finals (round 2)
     1   2  3   4     Prelims (round 1)
The "brackets" vector is always initialized the same way so that the match number is 1 greater than the index in the array.
The "teams" vector is always in order of the team's first match; teams[0] and teams[1] will always be in match 1 to start, teams[2] and teams[3] will always be in match 2, etc.

  Bracket(numberTeams, fileName)
    Constructs the bracket using a user given number of teams and a file that's in the following format:
    
teamname1
teamname2
teamname3
...

Where teamname1 is the name of the first seeded team, teamname2 is the name of the second seed, etc.
As long as the number of teams is less than or equal to the number of teams, the bracket should build correctly. Otherwise, the only difference between this and the previous constructor is the names of the teams.


    Destructor
  Clears the vectors within the class and deletes the class itself. pretty self explanitory.
  

    BFCreateMatches(numMatches, topLevel)
  Constructor Calls this to create the Matches Tree. In order to work correctly, topLevel must be log2(number of matches) rounded up, and numMatches must be 2^topLevel-1. Builds the tree breadth first, and assigns match numbers in reverse order in the above format. This tree won't change size after it's built, so no balancing is required for the tree. As it's built backwards, it also assigns each match to the correct position in the "matches" vector.
  
  
    printMatchNumbers()
  Reads off each of the match's match number preordered. Primarily used for debugging, as it's easier to read match data from the "matches" vector.
  
  
    CreateUnnamedTeams()
  DOESN'T WORK. IS BROKEN AND BAD. PLEASE IGNORE THIS LARGE, SCARY, BAD CODE.
  Actually, if you feel like wading through it, it's meant to create the bracket in standard tournement format. Right now though, it doesn't work. if you want to attempt fixing it, the format wanted is pretty easily found at challonge.com.
  
  
    printMatchups()
  Prints the matches and their data in order of match number. It SHOULD tell you everything you need to know about the match, including which match the winner goes to, which teams are participating in the match, and what round the match is in. If there is a Winner, it will display that as well.
  
  
    declareWinner()
  Declares the selected team as the winner of their current match. WORKS EVEN IF THE TEAM HAS NO OPPONENT YET. However, if the team was declared for a match that isn't part of the current round, the tree won't correctly update until the current round finishes. 
  
  
    UpdateWins()
  Updates the tree with the winning teams for each match. After a winner is declared, it won't be placed in it's next match until this is called. Also Automatically moves teams who have no opponent in the current round to their next match. If everyone in the current round has played, also updates current round.

    printTeams()
  prints the Teams in the order they are stored in the "teams" vector. This order will change if RandomizeStarting positions is called. 
  
    renameTeam(oldName, newName)
  searches the teams vector and renames the target team. Nothing fancy here.
  
    RandomizeStartingPositions()
  Randomizes the teams' starting positions. Will not change the position of empty Teamslots. That should probably be changed.
  
