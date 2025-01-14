#include <iostream>
#include <string>
using namespace std;

class Cards
{
public:
    enum Suit
    {
        hearts,
        diamonds,
        clubs,
        spades
    };
    enum Rank
    {
        ace = 1,
        two,
        three,
        four,
        five,
        six,
        seven,
        eight,
        nine,
        ten,
        jack,
        queen,
        king
    };

private:
    Suit suit;
    Rank rank;
    bool face_up;

public:
    Cards() : suit(hearts), rank(ace), face_up(false) {}
    Cards(Suit s, Rank r) : suit(s), rank(r), face_up(false) {}

    Suit getSuit() { return suit; }
    Rank getRank() { return rank; }
    bool getFaceStatus() { return face_up; }
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
        string current_suit;
        string current_rank;
        string color_code; // To store the color code

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
            color_code = "\033[31m";
            break;
        case diamonds:
            current_suit = "♦";
            color_code = "\033[31m";
            break;
        case clubs:
            current_suit = "♣";
            color_code = "\033[37m";
            break;
        case spades:
            current_suit = "♠";
            color_code = "\033[37m";
            break;
        }

        cout << color_code << current_rank + current_suit << "\033[0m" << endl; // Reset color after printing
    }

    // helper functions (idk if i'll ever use em but better safe than sorry)
    bool is_black(Cards c)
    {
        return c.suit == spades || c.suit == clubs;
    }

    bool toggleFace()
    {
        face_up = !face_up;
        return face_up;
    }

    bool is_red(Cards c)
    {
        return c.suit == hearts || c.suit == diamonds;
    }

    bool isDifferentColor(Cards c)
    {
        return (is_black(*this) && is_red(c)) || (is_red(*this) && is_black(c));
    }

    bool isDifferentSuit(Cards c)
    {
        return this->suit != c.suit;
    }

    bool is_same_color(Cards c1, Cards c2)
    {
        return (is_black(c1) && is_black(c2)) || (is_red(c1) && is_red(c2));
    }

    bool in_sequence(Cards c2)
    {
        return this->rank == c2.rank + 1;
    }

    bool can_stack_tableau(Cards c1, Cards c2)
    {
        return c1.isDifferentColor(c2) && c1.in_sequence(c2);
    }

    bool can_stack_foundations(Cards c1, Cards c2)
    {
        return is_same_color(c1, c2) && c1.in_sequence(c2);
    }

    string cardDataToString() const
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
        case hearts:
            current_suit = "♥";
            break;
        case diamonds:
            current_suit = "♦";
            break;
        case clubs:
            current_suit = "♣";
            break;
        case spades:
            current_suit = "♠";
            break;
        }
        return current_rank + current_suit;
    }
};