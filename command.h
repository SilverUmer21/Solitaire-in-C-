#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <stdexcept>
using namespace std;

class command
{
private:
    string commandtype;
    char source[3];
    char destination[3];
    int amount;
    bool flipped;

public:
    command(string c, char s[], char d[], int a, bool f = false) : commandtype(c), amount(a), flipped(f)
    {
        strncpy(source, s, sizeof(source) - 1);
        source[sizeof(source) - 1] = '\0';
        strncpy(destination, d, sizeof(destination) - 1);
        destination[sizeof(destination) - 1] = '\0';
    }

    string getCommandType() { return commandtype; }
    char *getsource() { return source; }
    char *getdestination() { return destination; }
    int getamount() { return amount; }
    bool getflipped() { return flipped; }
    void setflipped(bool f) { flipped = f; }
};

command parseCommand(string input)
{

    string commandtype;
    char source[3] = "";
    char destination[3] = "";
    int amount = 0;

    commandtype = input[0];
    if (commandtype == "m")
    {
        source[0] = input[2];
        if (source[0] == 'c')
        {
            source[1] = input[3];
            destination[0] = input[6];
            destination[1] = input[7];
            amount = input[10] - '0';
        }
        else if (source[0] == 'w')
        {
            destination[0] = input[5];
            destination[1] = input[6];
            amount = input[9] - '0';
        }
        else
        {
            throw invalid_argument("Invalid command");
        }
        return command(commandtype, source, destination, amount);
    }

    else if (commandtype == "s")
    {
        return command(commandtype, source, destination, 0);
    }

    else if (commandtype == "z")
    {
        return command(commandtype, source, destination, 0);
    }

    else
    {
        throw invalid_argument("Invalid command");
    }
}