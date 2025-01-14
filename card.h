#include <iostream>
#include <string>
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

    Suit getSuit() { return suit; }
    Rank getRank() { return rank; }
    bool getFace() { return faceStatus; }
    string getSuitSymbol() const
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

    void display() const
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

    bool black(Cards c)
    {
        return c.suit == spades || c.suit == clubs;     // bs yahi hein black 
    }

    bool toggleFace()
    {
        faceStatus = !faceStatus;
        return faceStatus;
    }

    bool red(Cards c)
    {
        return c.suit == hearts || c.suit == diamonds;  // or ye red hein apne pass 
    }

    bool isDifferentColor(Cards c)
    {
        return (black(*this) && red(c)) || (red(*this) && black(c));
    }
    string toString() const
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
