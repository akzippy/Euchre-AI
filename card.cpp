#include "card.hpp"


  char Card::getSuit() { return suit; }; // Takes a Card value and returns the numerator
  
  int Card::getRank() { return rank; }; // Takes a Card value and returns the denominator

  string Card::conciseCard() {
    string strConcise;
    strConcise = suit + to_string(rank);
    return strConcise;
  };

  string Card::verboseCard() {
    // Returns the verbose card name "S10" returns "Ten of Spades"
    string strVerbose;

    switch(rank){
      case 9  :
        strVerbose="Nine of ";
        break; 
      case 10  :
        strVerbose="Ten of ";
        break; 
      case 11  :
        strVerbose="Jack of ";
        break; 
      case 12  :
        strVerbose="Queen of ";
        break;
      case 13  :
        strVerbose="King of ";
        break; 
      case 14  :
        strVerbose="Ace of ";
        break;  
      default : 
        strVerbose="Joker";
    }

    switch(suit){
      case 'S'  :
        strVerbose=strVerbose+"Spades";
        break; 
      case 'D'  :
        strVerbose=strVerbose+"Diamonds";
        break; 
      case 'H'  :
        strVerbose=strVerbose+"Hearts";
        break; 
      case 'C'  :
        strVerbose=strVerbose+"Clubs";
        break; 
      default : 
        strVerbose=strVerbose+"";
    }
    
    return strVerbose;
  };

  bool operator==(Card a, Card b) { return ((a.getRank() == b.getRank()) and (a.getSuit() == b.getSuit())); };
  // Performs a logical == comparison of two Card values by comparing the ranks for equality and
  // the suits for equality.

  bool operator!=(Card a, Card b) { return not(a==b); };
  // Performs a logical != comparison by applying not to the == operator above.

  bool operator>(Card a, Card b) { return (a.getRank() > b.getRank()); };
  // Comparing two Card values for order by Rank only.

  bool operator<(Card a, Card b) { return (a.getRank() < b.getRank()); };
  // Functions as > except that it applies < instead of > .

  bool operator>=(Card a, Card b) { return ((a > b) or (a == b)); };
  // Uses the above functions to determine a > b and a==b and applies a logical OR to the results
  // to account for the "greater than or equal"

  bool operator<=(Card a, Card  b) { return ((a < b) or (a == b)); };
  // Uses the above functions to determine a < b and a==b and applies a logical OR to the results
  // to account for the "less than or equal"
  



  std::ostream& operator<<(std::ostream &output, Card c) {
  	// Overloads the << operator so that a Card can be passed through the std::cout stream

    string strrank;
    string suitunicode;
    string colorcode="\033[47m";
    string resetcolor="\033[0m";
    

    switch(c.getRank()){
      case 9  :
        strrank="9";
        break; 
      case 10  :
        strrank="10";
        break; 
      case 11  :
        strrank="J";
        break; 
      case 12  :
        strrank="Q";
        break;
      case 13  :
        strrank="K";
        break; 
      case 14  :
        strrank="A";
        break;  
      default : 
        strrank="0";
    }
    switch(c.getSuit()){
      case 'S'  :
        colorcode=colorcode + "\033[30m";
        suitunicode="\U00002660";
        break; 
      case 'D'  :
        colorcode=colorcode + "\033[31m";
        suitunicode="\U00002666";
        break; 
      case 'H'  :
        colorcode=colorcode + "\033[31m";
        suitunicode="\U00002665";
        break; 
      case 'C'  :
        colorcode=colorcode + "\033[30m";
        suitunicode="\U00002663";
        break; 
      default : 
        colorcode=colorcode + "\033[34m";
        suitunicode="*";
    }
    output << colorcode << suitunicode << strrank << suitunicode << resetcolor;
    return output;
  };
