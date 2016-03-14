#ifndef CARD_HPP
#define CARD_HPP


#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
using namespace std;


struct Card {
  private:
    char suit;
    int  rank;  // 9,10,J=11,Q=12,K=13,A=14

  public:
    Card() {suit='J'; rank=0;} // Default Constructor gives value of a "Joker"

    Card(char s, int r) { 
      suit=toupper(s);
      rank=r;
    }

   Card(string s) { 
     // This constructor will take a string format of "SRR" Where S is the valid Suit and RR 
     // represents two characters which will be converted into the integer rank.
     // Example - "S09" Would be "9 of Spades" or "suit="S",rank=9"
     // It will also take 1-4 for S,D,H,C respecively.

     char inputsuit = toupper(s.at(0));
      switch(inputsuit){
        case 'S'  :
          suit='S';
          break; 
        case 'D'  :
          suit='D';
          break; 
        case 'H'  :
          suit='H';
          break; 
        case 'C'  :
          suit='C';
          break; 
        case '1'  :
          suit='S';
          break; 
        case '2'  :
          suit='D';
          break; 
        case '3'  :
          suit='H';
          break; 
        case '4'  :
          suit='C';
          break; 
        default : 
          suit='J';
     }
     string str2 = s.substr (1,2);
     rank=atoi(str2.c_str());

   }

    Card(const Card& c) { // Required copy constructor, taking a Card value as the argument.
      suit=c.suit;
      rank=c.rank;
    }


    
    char getSuit();              // Returns the Suit of a Card
    int getRank();               // Returns the Rank of a Card
    string conciseCard();  // Returns the verbose card "S10" returns "S10"    
    string verboseCard();  // Returns the verbose card "S10" returns "Ten of Spades"


    ~Card(){};  // Destructor
};

bool operator==(Card a, Card b);     //Takes 2 Card values - Returns a.rank == b.rank and a.suit == b.suit
bool operator!=(Card a, Card b);     //Takes 2 Card values - Returns a.rank != b.rank and a.suit == b.suit
bool operator>=(Card a, Card b);     //Takes 2 Card values - Returns a.rank >= b.rank
bool operator<=(Card a, Card b);     //Takes 2 Card values - Returns a.rank <= b.rank
bool operator>(Card a, Card b);      //Takes 2 Card values - Returns a.rank > b.rank
bool operator<(Card a, Card b);      //Takes 2 Card values - Returns a.rank < b.rank

std::ostream& operator<<(std::ostream &output, Card c);  //Overload of << to allow use of std::cout
// std::istream& operator>>(std::istream &inpout, Card c);  //Overload of >> to allow use of std::cin

#endif