#include <iostream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <windows.h>
#include <algorithm>
#include <string>
#include <thread>
#include <cstring>
#include "stack.h"
#include "list.h"
#include "card.h"
#include "command.h"
using namespace std;

class Game
{   
public:
    string getCard(const Cards &card)
    {
        string Ccode;
        string suit_symbol = card.getSuitSymbol();

        // Set color based on suit
        if (suit_symbol == "♥" || suit_symbol == "♦")     // Red for hearts and diamonds
        {
            Ccode = "\033[31m"; 
        }
        else
        {
            Ccode = "\033[97m";    // Bright white for spades and clubs cox we aint got white screen               
        }
        return Ccode + card.cardDataToString() + "\033[0m"; 
    }
    void initializeGame()    
    {
        clearTable();   // clear all the previous console things   !! The order of these functions must not change  !! 
        Set_Deck();     // initialize the deck before shuffling 
        Shuffle();      // shuffle all the cards in the deck

        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                tableau[i].add(deck.pop());
            }
            tableau[i].get(i)->toggleFace();
        }
        for (int i = 0; i < 24; i++)
        {
            stock.push(deck.pop());
        }
    }
    void SetThetable()                          // prepare the table * printing requirments on Console      
    {
        cout << "\033[34m  stock\033[0m"        << "\t\t"
             << "\033[33m  Waste\033[0m"        << "\t\t\t"
             << "\033[35mFoundation 1\033[0m"   << "\t"   
             << "\033[36mFoundation 2\033[0m"   << "\t"
             << "\033[32mFoundation 3\033[0m"   << "\t"   
             << "\033[31mFoundation 4 \033[0m"  << endl;
        cout << "  [   ]" << "\t\t";
        if (waste.isEmpty())
        {
            cout << "  [   ]" << "\t\t\t";
        }
        else
        {
            cout << "   " << getCard(waste.peek()) << " " << "\t\t\t";
        }

        for (int i = 0; i < 4; i++)
        {
            if (foundation[i].isEmpty())
            {
                cout << "[          ]" << '\t';
            }
            else
            {
                cout << "     " << getCard(foundation[i].peek()) << "     " << '\t';
            }
        }
        cout << endl;

        cout << "(" << stock.size() << " cards)" << '\t';
        cout << "(" << waste.size() << " cards)" << "\t\t ";
        for (int i = 0; i < 4; i++)
        {
            cout << "(" << foundation[i].size() << " cards)" << "\t  ";
        }
        cout << endl;
        cout << endl;

        cout << "\033[33mColumn 1\033[0m" << '\t' << "\033[36mColumn 2\033[0m" << '\t' << "\033[32m Column 3\033[0m" << '\t'
             << "\033[31mColumn 4\033[0m" << '\t' << "\033[34mColumn 5\033[0m" << '\t' << "\033[35mColumn 6\033[0m"  << '\t'
             << "\033[33mColumn 7 \033[0m"<< endl;

        for (int i = 0; i < 7; i++)
        {
            cout << "(" << tableau[i].size() << " cards)";
            if (i < 6)
            {
                cout << '\t';
            }
        }
        cout << endl;

        int MaxSizeDeck = 0;
        for (int i = 0; i < 7; i++)
        {
            if (tableau[i].size() > MaxSizeDeck)
            {
                MaxSizeDeck = tableau[i].size();
            }
        }

        for (int i = 0; i < MaxSizeDeck; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                if (i < tableau[j].size())
                {
                    if (tableau[j].get(i)->getFaceStatus())
                    {
                        cout << getCard(*tableau[j].get(i)) << "\t\t";
                    }
                    else
                    {
                        cout << "[   ]" << "\t\t";
                    }
                }
                else
                {
                    cout << "     " << "\t\t";
                }
            }
            cout << endl;
        }
    }
    bool GameStatus()                           // check if the game is won      
    {
        for (int i = 0; i < 4; i++)
        {
            if (foundation[i].size() != 13)
            {
                return false;
            }
        }
        return true;
    }
    bool moveValidator(command parsedCommand)   // cheking move ye na ho ke move vaild hi na ho or hum chala dein  
    {
        int num = parsedCommand.getamount();
        string src = parsedCommand.getsource();
        string dest = parsedCommand.getdestination();
        if (src[0] == 'c' && dest[0] == 'c')
        {
            int SrcCol = src[1] - '1';
            int destCol = dest[1] - '1';
            if (SrcCol >= 0 && SrcCol <= 6 && destCol >= 0 && destCol <= 6)
            {
                if (tableau[SrcCol].isEmpty())
                {
                    return false;
                }
                if (tableau[destCol].isEmpty())
                {
                    // Only a King can be moved to an empty column
                    return tableau[SrcCol].get(tableau[SrcCol].size() - num)->getRank() == Cards::king;
                }
                Cards *SrcCard = tableau[SrcCol].get(tableau[SrcCol].size() - num);
                Cards *destCard = tableau[destCol].get(tableau[destCol].size() - 1);
                Cards *src_card2 = tableau[SrcCol].get(tableau[SrcCol].size() - num + 1);
                // Check if the source card is one rank lower and of a different color than the destination card
                return SrcCard->getRank() == destCard->getRank() - 1 && SrcCard->isDifferentColor(*destCard);
            }
            return false;
        }
        else if (src[0] == 'c' && dest[0] == 'f')
        {
            int SrcCol = src[1] - '1';
            int destCol = dest[1] - '1';
            if (SrcCol >= 0 && SrcCol <= 7 && destCol >= 0 && destCol <= 4)
            {
                if (tableau[SrcCol].isEmpty())
                {
                    return false;
                }
                if (foundation[destCol].isEmpty())
                {
                    if (tableau[SrcCol].get(tableau[SrcCol].size() - num)->getRank() == Cards::ace)
                    {
                        return true;
                    }
                    return false;
                }
                if (tableau[SrcCol].get(tableau[SrcCol].size() - num)->getRank() == foundation[destCol].peek().getRank() + 1)
                {
                    if (tableau[SrcCol].get(tableau[SrcCol].size() - num)->getSuit() == foundation[destCol].peek().getSuit())
                    {
                        return true;
                    }
                    return false;
                }
                return false;
            }
            return false;
        }
        else if (src[0] == 'w' && dest[0] == 'c')
        {
            int destCol = dest[1] - '1';
            if (destCol >= 0 && destCol <= 6)
            {
                if (waste.isEmpty())
                {
                    return false;
                }
                if (tableau[destCol].isEmpty())
                {
                    // Only a King can be moved to an empty column
                    return waste.peek().getRank() == Cards::king;
                }
                Cards *SrcCard = &waste.peek();
                Cards *destCard = tableau[destCol].get(tableau[destCol].size() - 1);
                // Check if the source card is one rank lower and of a different color than the destination card
                return SrcCard->getRank() == destCard->getRank() - 1 && SrcCard->isDifferentColor(*destCard);
            }
            return false;
        }
        else if (src[0] == 'w' && dest[0] == 'f')
        {
            int destCol = dest[1] - '1';
            if (destCol >= 0 && destCol <= 4)
            {
                if (waste.isEmpty())
                {
                    return false;
                }
                if (foundation[destCol].isEmpty())
                {
                    if (waste.peek().getRank() == Cards::ace)
                    {
                        return true;
                    }
                    return false;
                }
                if (waste.peek().getRank() == foundation[destCol].peek().getRank() + 1)
                {
                    if (waste.peek().getSuit() == foundation[destCol].peek().getSuit())
                    {
                        return true;
                    }
                    return false;
                }
                return false;
            }
            return false;
        }
        return false;
    } 
    void moveCards(command &parsedCommand)      // function to move cards with all the conditions ;)     
    {
        string src = parsedCommand.getsource();
        string dest = parsedCommand.getdestination();
        int num = parsedCommand.getamount();

        if (src[0] == 'c' && dest[0] == 'c')
        {
            int SrcCol = src[1] - '1';
            int destCol = dest[1] - '1';
            if (SrcCol >= 0 && SrcCol < 7 && destCol >= 0 && destCol < 7)
            {
                DLL<Cards> temp;
                for (int i = 0; i < num; i++)
                {
                    if (!tableau[SrcCol].isEmpty())
                    {
                        temp.add(tableau[SrcCol].remove());
                    }
                    else
                    {
                        throw invalid_argument("Invalid move");
                    }
                }
                for (int i = 0; i < num; i++)
                {
                    tableau[destCol].add(temp.remove());
                }
                if (tableau[destCol].size() > num && parsedCommand.getflipped())
                {
                    tableau[destCol].get(tableau[destCol].size() - num - 1)->toggleFace();
                }
                if (!tableau[SrcCol].isEmpty())
                {
                    Cards &c = *tableau[SrcCol].get(tableau[SrcCol].size() - 1);
                    if (!c.getFaceStatus())
                    {
                        c.toggleFace();
                        parsedCommand.setflipped(true);
                    }
                }
            }
        }
        else if (src[0] == 'c' && dest[0] == 'f')
        {
            int SrcCol = src[1] - '1';
            int destCol = dest[1] - '1';
            if (SrcCol >= 0 && SrcCol <= 7 && destCol >= 0 && destCol <= 4)
            {
                for (int i = 0; i < num; i++)
                {
                    if (!tableau[SrcCol].isEmpty())
                    {
                        foundation[destCol].push(tableau[SrcCol].remove());
                    }
                    else
                    {
                        throw invalid_argument("Invalid move");
                    }
                }
                if (!tableau[SrcCol].isEmpty())
                {
                    Cards &c = *tableau[SrcCol].get(tableau[SrcCol].size() - 1);
                    if (!c.getFaceStatus())
                    {
                        c.toggleFace();
                        parsedCommand.setflipped(true);
                    }
                }
            }
        }
        else if (src[0] == 'w' && dest[0] == 'c')
        {
            int destCol = dest[1] - '1';
            if (destCol >= 0 && destCol <= 7)
            {
                for (int i = 0; i < num; i++)
                {
                    if (!waste.isEmpty())
                    {
                        tableau[destCol].add(waste.pop());
                    }
                    else
                    {
                        throw invalid_argument("Invalid move");
                    }
                }
            }
        }
        else if (src[0] == 'w' && dest[0] == 'f')
        {
            int destCol = dest[1] - '1';
            if (destCol >= 0 && destCol <= 4)
            {
                for (int i = 0; i < num; i++)
                {
                    if (!waste.isEmpty())
                    {
                        foundation[destCol].push(waste.pop());
                    }
                    else
                    {
                        throw invalid_argument("Invalid move");
                    }
                }
            }
        }
        else if (src[0] == 'f' && dest[0] == 'c')
        {
            int SrcCol = src[1] - '1';
            int destCol = dest[1] - '1';
            if (destCol >= 0 && destCol < 7 && SrcCol >= 0 && SrcCol < 4)
            {
                for (int i = 0; i < num; i++)
                {
                    if (!foundation[SrcCol].isEmpty())
                    {
                        tableau[destCol].add(foundation[SrcCol].pop());
                    }
                    else
                    {
                        throw invalid_argument("Invalid move");
                    }
                }
                if (tableau[destCol].size() > num && parsedCommand.getflipped())
                {
                    tableau[destCol].get(tableau[destCol].size() - num - 1)->toggleFace();
                }
            }
        }
        else if (src[0] == 'f' && dest[0] == 'w')
        {
            int SrcCol = src[1] - '1';
            if (SrcCol >= 0 && SrcCol <= 4)
            {
                for (int i = 0; i < num; i++)
                {
                    if (!foundation[SrcCol].isEmpty())
                    {
                        waste.push(foundation[SrcCol].pop());
                    }
                    else
                    {
                        throw invalid_argument("Invalid move");
                    }
                }
            }
        }
        else if (src[0] == 'c' && dest[0] == 'w')
        {
            int SrcCol = src[1] - '1';
            if (SrcCol >= 0 && SrcCol <= 7)
            {
                for (int i = 0; i < num; i++)
                {
                    if (!tableau[SrcCol].isEmpty())
                    {
                        waste.push(tableau[SrcCol].remove());
                    }
                    else
                    {
                        throw invalid_argument(" Invalid move");
                    }
                }
            }
        }
        else
        {
            throw invalid_argument(" nvalid move");
        }
    }
    void DrawAcard()                            // draw cardssssssssssssssss
    {
        if (stock.isEmpty())
        {
            while (!waste.isEmpty())
            {
                Cards card = waste.pop();
                card.toggleFace();
                stock.push(card);
            }
        }
        else
        {
            Cards card = stock.pop();
            card.toggleFace();
            waste.push(card);
        }
    }
    void UndoCardsPlayed()                      // undo the draw
    {
        if (waste.isEmpty())
        {
            while (!stock.isEmpty())
            {
                Cards card = stock.pop();
                card.toggleFace();
                waste.push(card);
            }
        }
        else
        {
            Cards card = waste.pop();
            card.toggleFace();
            stock.push(card);
        }
    }
    void UndoLast()                             // undo the move
    {
        if (!undo.isEmpty())
        {
            command parsedCommand = undo.pop();
            if (parsedCommand.getCommandType() == "s")
            {
                UndoCardsPlayed();
            }
            else
            {
                command newCommand(parsedCommand.getCommandType(), parsedCommand.getdestination(), parsedCommand.getsource(), parsedCommand.getamount(), parsedCommand.getflipped());
                moveCards(newCommand);
            }
        }
    }
    void clearTable()                           // clear all the stacks , columns and foundations etc 
    {
        // Clear all the stacks
        
        while (!stock.isEmpty())
        {
            stock.pop();
        }
        while (!waste.isEmpty())
        {
            waste.pop();
        }
        for (int i = 0; i < 7; i++)
        {
            while (!tableau[i].isEmpty())
            {
                tableau[i].remove();
            }
        }
        for (int i = 0; i < 4; i++)
        {
            while (!foundation[i].isEmpty())
            {
                foundation[i].pop();
            }
        }
        while (!deck.isEmpty())
        {
            deck.pop();
        }
        while (!undo.isEmpty())
        {
            undo.pop();
        }


    }
    void play()                                 // The main function of our Game Class . This is responsible for taking arguments from player and checking them
    {
        int delay=1;
        string Eror = "INVALID COMMAND  !!  TRY AGAIN";
        int lives = 10;     // Only time you get second chance in life . So do well 
        initializeGame();
        for(;!GameStatus() && lives != 0;)
        {
            cin.clear();
            cout << "\033[2J\033[H";    // bring cursor to First line 
            SetThetable();
            cout << "\033[31m" << "Remaining Lives : " <<lives<< "♥ \033[0m" << endl;
            cout << "Enter command:"<<endl;
            cout.flush();        // for taking all input . No character should be on console before
            string input;
            getline(cin, input);
            try
            {
                if (input == "exit")
                {
                    cout << " Thank you for playing . Program Exited " << endl;
                    this_thread::sleep_for(chrono::seconds(delay));             // For delay between console output and other commands 
                    break;
                }
                else if (input == "reset")
                {
                    cout << " Game Reset " << endl;
                    this_thread::sleep_for(chrono::seconds(delay));
                    initializeGame();
                    continue;
                }
                command parsedCommand = parseCommand(input);
                if (parsedCommand.getCommandType() == "m")
                {
                    if (moveValidator(parsedCommand))
                    {
                        moveCards(parsedCommand);
                        undo.push(parsedCommand);
                    }
                    else
                    {
                        throw invalid_argument("Invalid move");
                    }
                }
                else if (parsedCommand.getCommandType() == "s")
                {
                    undo.push(parsedCommand);
                    DrawAcard();
                }
                else if (parsedCommand.getCommandType() == "z")
                {
                    UndoLast();
                }
                else
                {
                    throw invalid_argument("INVALID COMMAND    ");
                }
            }
            catch (const invalid_argument &e)
            {
                cout << Eror << endl;
                this_thread::sleep_for(chrono::seconds(delay));
                lives--;
            }
        }
        if (GameStatus())   // Game won or not
        {
            cout << " Congratulations ! YOU WON " << endl;
            this_thread::sleep_for(chrono::seconds(delay));
        }
        else if (lives == 0 )   // Player has used all the lives 
        {
            cout << " No more lives left . GAME OVER " << endl;
            this_thread::sleep_for(chrono::seconds(delay));
        }
        else
        {
            cout << "   Game over ." << endl;
            this_thread::sleep_for(chrono::seconds(delay));
        }
    }
    
    private: 
        stack <Cards>   deck;
        DLL  <Cards>   tableau[7];
        stack <Cards>   foundation[4];
        stack <Cards>   stock;
        stack <Cards>   waste;
        stack <command> undo;

        void Set_Deck() // put cards in the deck
        {
        for (int i = Cards::hearts; i <= Cards::spades; i++)
        {
            for (int j = Cards::ace; j <= Cards::king; j++)
            {
                deck.push(Cards(static_cast<Cards::Suit>(i), static_cast<Cards::Rank>(j)));
            }
        }
        }  
        void Shuffle()
        {
            srand(static_cast<unsigned int>(chrono::high_resolution_clock::now().time_since_epoch().count()));  // real Shuffle  everytime 
            int size = deck.size();
            Cards *array = new Cards[size];
        
            for (int i = 0; i < size; ++i)      // First put cards from Deck since its a stack
            {
                array[i] = deck.pop();
            }
            for (int i = size - 1; i > 0; --i)  // cards shuffled 
            {   
                int j = rand() % (i + 1);
                Cards temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
            for (int i = 0; i < size; ++i)      // put cards back in the deck 
            {
                deck.push(array[i]);
            }
            delete[] array;
        }
};
void startingScreen() 
{
    cout << "\033[2J\033[H";    
    cout << "\033[32m" 
              << "=============================================\n"
              << "||                                         ||\n"
              << "||          Welcome to Solitaire !         ||\n"
              << "||               Made by Umer              ||\n"
              << "||                 23L-0682                ||\n"
              << "||                                         ||\n"
              << "=============================================\n"
              << "\033[0m";
    
    this_thread::sleep_for(chrono::seconds(2)); // Wait for 5 seconds
}
int main()
{
    startingScreen();   // Game intro 
    Game Solitaire;     // Game starts 
    Solitaire.play();
    return 0;
}