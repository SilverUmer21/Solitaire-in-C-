#include <iostream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <windows.h>
#include <algorithm>
#include <string>
#include <thread>
#include <cstring>
using namespace std;

class Cards     // easiest way to make cards 
{
public:
    enum Suit
    {
        hearts,
        diamonds,
        clubs,
        spades
    };
    enum Rank   // the ranks can be used as if integers
    {
        ace = 1,two,three,four,five,six,seven,eight,nine,ten,jack,queen,king
    };

private:
    Suit suit;
    Rank rank;
    bool faceStatus;
public:
    Cards() : suit(hearts), rank(ace), faceStatus(false) {}
    Cards(Suit s, Rank r) : suit(s), rank(r), faceStatus(false) {}
    Suit    getSuit() { return suit; }
    Rank    getRank() { return rank; }
    bool    getFaceStatus() { return faceStatus; }
    void    display() const
    {
        string current_rank;
        string current_suit;
        string Ccode;       // UTF code ko rakh lo bhai 

        switch (rank)
        {
        case ace:
            current_rank = "A";
            break;
        case jack:
            current_rank = "J";
            break;
        case queen:
            current_rank = "Q";
            break;
        case king:
            current_rank = "K";
            break;
        default:
            current_rank = to_string(rank);
        }

        switch (suit)
        {
        case hearts:
            current_suit = "♥";
            Ccode = "\033[31m";
            break;
        case diamonds:
            current_suit = "♦";
            Ccode = "\033[31m";
            break;
        case clubs:
            current_suit = "♣";
            Ccode = "\033[37m";
            break;
        case spades:
            current_suit = "♠";
            Ccode = "\033[37m";
            break;
        }

        cout << Ccode << current_rank + current_suit << "\033[0m" << endl;  // color to wapas krdo terminal ko 
    }
    bool    black(Cards c)
    {
        return c.suit == spades || c.suit == clubs;     // bs yahi hein black 
    }
    bool    red(Cards c)
    {
        return c.suit == hearts || c.suit == diamonds;  // or ye red hein apne pass 
    }
    bool    isDifferentColor(Cards c)
    {
        return (black(*this) && red(c)) || (red(*this) && black(c));
    }
    bool    toggleFace()
    {
        faceStatus = !faceStatus;
        return faceStatus;
    }
    string  getSuitSymbol() const
    {
        switch (suit)
        {
        case hearts:
            return "♥";
        case diamonds:
            return "♦";
        case clubs:
            return "♣";
        case spades:
            return "♠";
        }
        return "";
    }
    string  cardDataToString() const
    {
        string current_suit;
        string current_rank;
        string color;

        switch (rank)
        {
        case ace:
            current_rank = "A";
            break;
        case jack:
            current_rank = "J";
            break;
        case queen:
            current_rank = "Q";
            break;
        case king:
            current_rank = "K";
            break;
        default:
            current_rank = to_string(rank);
        }

        switch (suit)
        {
        case diamonds:
            current_suit = "♦";
            break;
        case hearts:
            current_suit = "♥";
            break;
        
        case clubs:
            current_suit = "♣";
            break;
        case spades:
            current_suit = "♠";
            break;
        }
        return current_rank + current_suit;     // 5A type return krni ha , easy to print 
    }
};
class command
{
private:
    int amount;
    char source[3];
    char destination[3];
    string commandtype;
    bool isFlipped;

public:
    command(string c, char s[], char d[], int a, bool f = false) : commandtype(c), amount(a), isFlipped(f)
    {
        strncpy(source, s, sizeof(source) - 1);
        source[sizeof(source) - 1] = '\0';
        strncpy(destination, d, sizeof(destination) - 1);
        destination[sizeof(destination) - 1] = '\0';
    }

    string  getCommandType()     { return commandtype; }
    char   *getsource()          { return source; }
    char   *getdestination()     { return destination; }
    int     getamount()          { return amount; }
    bool    getflipped()         { return isFlipped; }
    void    setflipped(bool f)   { isFlipped = f; }
};
command parseCommand(string input)
{
    string commandtype;     // sab se mushkil kaam 
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
            throw invalid_argument("INVALID COMMAND   ");
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
        throw invalid_argument("INVALID COMMAND   ");
    }
}
template <class T>
class List   
{
private:
    class Node
    {
    public:
        T card;
        Node *next;
        Node *prev;

        Node(T c) : card(c), next(NULL), prev(NULL) {}
    };
    Node *head;
    Node *tail;
public:
    List() : head(NULL), tail(NULL) {}
    ~List()
    {
        while (head != NULL)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }
    void add(T c)
    {
        Node *new_node = new Node(c);
        if (head == NULL)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        tail->next = NULL;
    }
    T remove()
    {
        if (head == NULL)
        {
            throw invalid_argument("Empty list");
        }

        Node *temp = head;
        if (head->next == NULL) // Only one element in the list
        {
            T c = head->card;
            delete head;
            head = NULL;
            return c;
        }

        // Traverse to the last node
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        T c = temp->card;
        temp->prev->next = NULL;
        tail = temp->prev;
        delete temp;
        return c;
    }
    int size()
    {
        Node *current = head;
        int count = 0;
        while (current != NULL)
        {
            count++;
            current = current->next;
        }
        return count;
    }
    T *get(int index)
    {
        Node *current = head;
        int count = 0;
        while (current != NULL)
        {
            if (count == index)
            {
                return &(current->card);
            }
            count++;
            current = current->next;
        }

        return NULL;
    }
    bool isEmpty()
    {
        return head == NULL;
    }
    class iterator
    {
    private:
        Node *current;

    public:
        iterator(Node *current) : current(current) {}
        iterator &operator++()
        {
            current = current->next;
            return *this;
        }
        iterator &operator--()
        {
            current = current->prev;
            return *this;
        }
        T operator*() { return current->card; }
        bool operator!=(const iterator &other) { return current != other.current; }
    };
    iterator begin() { return iterator(head); }
    iterator end() { return iterator(NULL); }
};
template <class T>
class STACK
{
private:
    class Node
    {
    public:
        T data;
        Node *next;

        Node(T d) : data(d), next(NULL) {}
    };

    Node *top;
    int count;
public:
    STACK() : top(NULL), count(0) {}
    ~STACK()
    {
        while (top != NULL)
        {
            Node *temp = top;
            top = top->next;
            delete temp;
        }
    }
    void push(const T &d)
    {
        Node *new_node = new Node(d);
        new_node->next = top;
        top = new_node;
        ++count;
    }
    T pop()
    {
        if (top == NULL)
        {
            cout << "STACK is empty" << endl;
            throw runtime_error("STACK is empty");
        }
        T temp = top->data;
        Node *temp_node = top;
        top = top->next;
        delete temp_node;
        --count;
        return temp;
    }
    T &peek()
    {
        if (top == NULL)
        {
            cout << "STACK is empty" << endl;
            throw runtime_error("STACK is empty");
        }
        return top->data;
    }
    bool isEmpty()
    {
        return top == NULL;
    }
    int size()
    {
        return count;
    }
};
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
                List<Cards> temp;
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
        STACK <Cards>   deck;
        List  <Cards>   tableau[7];
        STACK <Cards>   foundation[4];
        STACK <Cards>   stock;
        STACK <Cards>   waste;
        STACK <command> undo;

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