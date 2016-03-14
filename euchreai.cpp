//----------------------------------------------------------------------------------
//AI Final Project - Euchre AI
//Implementation by Andrew Klenotic & Dylan Stavarz
//Artificial Intelligence and Heuristic Programming
//3460:460 - Secion 001 - Fall 2014
//----------------------------------------------------------------------------------

#include "card.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

// Global Variables - May make local later but these are probably going to be widely used.
char trumpsuit='N';
char tricksuit='N';
Card myhand[5];
Card thistrick[4];
bool silent = false;
 bool uselogfile = true;
// bool logtoscreen = true;
// bool uselogfile = false;
bool logtoscreen = false;
bool handover = false;
bool keepplaying = true;
bool male = false;
string logfilename;
string logstring;
string playername[4];
int scoreteama;
int scoreteamb;
int hands;                         // The % of hands/4 also determines dealer.
int pointgoal=10;
int maker;
int dealer;
int firstplayer;
int goalonethresh=80;
int trumpthresh=65;
int tricksbyplayer[4];
vector<Card> cardsplayed;
ofstream logfile;
int trickwinner;
int tricksteama;
int tricksteamb;

string truefalse(bool tf) {
  if (tf) { return "TRUE"; }
  else { return "FALSE"; };
};

void logentry(string comment) {
  if (uselogfile) { logfile << comment << '\n'; };
  if (logtoscreen) { cout << "\033[32m" << comment << "\033[0m\n"; };
};

void speak(string sayit) {
  string command = "espeak ";
  if (male) { command = command + "-ven+m1 "; }
  else { command = command + "-ven+f5 "; };
  command = command + "\"" + sayit + "\"";
  cout << sayit << '\n';
  if ( !silent ) system(command.c_str());
  if(uselogfile) {
    // logfile << "\nSilent = " << truefalse(silent) << " Cmd: |" << command << "|\n\n";
  };
  if (logtoscreen) {
    cout << "\033[32m\n";
    cout << "\nSilent = " << truefalse(silent) << " Cmd: |" << command << "|\n\n";
    cout << "\033[0m\n";
  };

//  cout << "\033[32mXXXX" << command << "XXXX" << command.c_str() << "XXX\033[0m\n";
};

void clearscreen() {
  cout << "\033[2J";
};

void welcome() {
  speak("Hello! Why don't we play some Euchre?");
  time_t now;
  struct tm newyear;
  int seconds;
  time(&now);  /* get current time; same as: now = time(NULL)  */
  newyear = *localtime(&now);
  newyear.tm_hour = 0; newyear.tm_min = 0; newyear.tm_sec = 0;
  newyear.tm_mon = 0;  newyear.tm_mday = 1;
  seconds = difftime(now,mktime(&newyear));
  logfilename="euchrelogfile-" + to_string(seconds);
  if (uselogfile) { logfile.open(logfilename); };
  // current date/time based on current system
  now = time(0);
  // convert now to string form
  string dt = ctime(&now);
  logentry("==========================================================================");
  logentry("Welcome to EuchreAI - Session Begins");
  logentry("The local date and time is: " + dt );
  logentry("==========================================================================");
};

void goodbye() {
  speak("Goodbye! I look forward to playing again soon!");
  time_t now;
  now = time(0);
  // convert now to string form
  string dt = ctime(&now);
  logentry("==========================================================================");
  logentry("Farewell from EuchreAI - Session Ends");
  logentry("The local date and time is: " + dt );
  logentry("==========================================================================");
  logentry("");
  if (uselogfile) { logfile.close(); };
};

void introduce() {
  if (male) playername[0]="John";
  else playername[0]="Jane";
  speak("My name is " + playername[0] + ", what are yours?");
  cout << "Enter name of " << playername[0] << "'s partner -> ";
  cin >> playername[2];
  cout << "\nEnter the name of the first opponent  -> ";
  cin >> playername[1];
  cout << "\nEnter the name of the second opponent -> ";
  cin >> playername[3];
  cout << "\n\n";
  speak("I'm happy to meet you " + playername[2] 
      + ", and I'm looking forward to a good game against " + playername[1] + " and " + playername[3] + "!"); 
  logentry("==========================================================================");
  logentry("Player names are as follows...");
  logentry("AI Name [0] = " + playername[0] + "  AI Partner [2] = " + playername[2]);
  logentry("Opponent [1] = " + playername[1] + "  Opponent [3] = " + playername[3]);
  logentry("==========================================================================");
  logentry("");
 
};

void begingame() {
  scoreteama = 0;
  scoreteamb = 0;
  hands=0;
  handover=false;
  keepplaying=true;
  tricksbyplayer[0]=0;
  tricksbyplayer[1]=0;
  tricksbyplayer[2]=0;
  tricksbyplayer[3]=0;
};

void deal() {
  logentry("-=<Dealing Cards>=-"); 
  int cardsdealt[4];
  speak("Looks like it's my deal.");
  speak(playername[2]+" can you help me?  I left my hands at home.");
  speak("Make sure you give it a good shuffle!");
  srand (time(NULL));
  cardsdealt[1] = rand() % 4 + 1;
  speak("Ready? " + playername[1] + " gets " + to_string(cardsdealt[1]) + ".");
  cardsdealt[2] = rand() % 4 + 1;
  speak("and " + to_string(cardsdealt[2]) + " for you, " + playername[2] + ".");
  cardsdealt[3] = rand() % 4 + 1;
  speak(playername[3] + " gets " + to_string(cardsdealt[3]) + ".");
  cardsdealt[0] = rand() % 4 + 1;
  speak("and I get " + to_string(cardsdealt[0]) + ".");
  speak("so " + playername[1] + " gets " + to_string(5-cardsdealt[1]) + " more.");
  speak("and you get " + to_string(5-cardsdealt[2]) + " more, " + playername[2]);
  speak("and " + playername[3] + " gets " + to_string(5-cardsdealt[3]) + " more.");
  speak("and finally, I get " + to_string(5-cardsdealt[0]) + ".");
  logentry(playername[0] + " - " + to_string(cardsdealt[0]) + " - " + to_string(5-cardsdealt[0]));
  logentry(playername[1] + " - " + to_string(cardsdealt[1]) + " - " + to_string(5-cardsdealt[1]));
  logentry(playername[2] + " - " + to_string(cardsdealt[2]) + " - " + to_string(5-cardsdealt[2]));
  logentry(playername[3] + " - " + to_string(cardsdealt[3]) + " - " + to_string(5-cardsdealt[3]));
  logentry("-=<Cards Dealt>=-"); 
};

void displayscore() {
  speak("After " + to_string(hands) + " hands, ");
  speak(playername[2] + " and I have " + to_string(scoreteama) + " points and ");
  speak(playername[1] + " and " + playername[3] + " have " + to_string(scoreteamb) + ".");
  logentry("Fuction Call - void displayscore() - scoreteama=" + to_string(scoreteama) + " scoreteamb=" + to_string(scoreteamb) + ".");

};

bool yesorno(string prompt) {
  char tempinput;
  cout << prompt << "\n";
  cout << "'Y'es or 'N'o -> ";
  cin >> tempinput;
  return toupper(tempinput)=='Y';
};

string suitName(char suit) {
  string strVerbose;
  switch(suit){
    case 'S'  :
      strVerbose="Spades";
      break; 
    case 'D'  :
      strVerbose="Diamonds";
      break; 
    case 'H'  :
      strVerbose="Hearts";
      break; 
    case 'C'  :
      strVerbose="Clubs";
      break; 
    default : 
      strVerbose="Joker";
  }
  return strVerbose;
};


char oppositecolor(char suit) {
  switch(suit){
    case 'S'  :
      return 'C';
      break; 
    case 'D'  :
      return 'H';
      break; 
    case 'H'  :
      return 'D';
      break; 
    case 'C'  :
      return 'S';
      break;
    default :
      return 'N'; 
   }
};

bool insuit(Card c, char suit, char trump) {
  //  logentry("Function Call - bool insuit(Card c, char suit, char trump)");
  // Is a card in a suit?  This covers the Right & Left Bowers
  if (c.getRank()==11) {
    if ((c.getSuit()==suit and suit!=oppositecolor(trump)) or (c.getSuit()==oppositecolor(suit) and suit==trump)) { return true; }
    else { return false; };
  }
  else {
    if (c.getSuit()==suit) { return true; }
    else { return false; };
  };
};

bool notinsuit(Card c, char suit, char trump) {
  //  logentry("Function Call - notinsuit(Card c, char suit, char trump)");
  // Is a card in a suit?  This covers the Right & Left Bowers
  if (c.getRank()==11) {
    if (c.getSuit()==suit or (c.getSuit()==oppositecolor(suit) and suit==trump)) { return false; }
    else { return true; };
  }
  else {
    if (c.getSuit()==suit or c.getSuit()=='J') { return false; }
    else { return true; };
  };
};

bool gotany(Card c[], char suit, char trump, int size) {
  //  logentry("Function Call - gotany(Card c[], char suit, char trump, int size)");
  bool suitfound=false;
  for (int j=0; j<size; j++) {
    if (insuit(c[j], suit, trump)) { suitfound=true; };
  };
  return suitfound;
};

int cardvalue(Card c, char trump, char trick) {
  char cardsuit=c.getSuit();
  int  cardrank=c.getRank();
  char opcolor=oppositecolor(trump);
  if (cardrank == 11 and cardsuit == trump) { return 24; }    // Right Bower is 1st
  else if (cardrank == 11 and cardsuit == opcolor) { return 23; }   // Left Bower is 2nd
  else if (cardsuit==trump) {
      switch(cardrank){
      case 9  :
        return 18;
        break; 
      case 10  :
        return 19;
        break; 
      case 11  :
        return 24;
        break; 
      case 12  :
        return 20;
        break;
      case 13  :
        return 21;
        break; 
      case 14  :
        return 22;
        break;  
      default : 
        return 0;   // Should not occur and indicates an error condition.
      }
  } 
  else if (cardsuit==trick and cardsuit!=trump and cardsuit==opcolor) {
      switch(cardrank){
      case 9  :
        return 12;
        break; 
      case 10  :
        return 13;
        break; 
      case 11  :
        return 23;
        break; 
      case 12  :
        return 15;
        break;
      case 13  :
        return 16;
        break; 
      case 14  :
        return 17;
        break;  
      default : 
        return 0;   // Should not occur and indicates an error condition.
      }
  }
  else if (cardsuit==trick and cardsuit!=opcolor) {
      switch(cardrank){
      case 9  :
        return 12;
        break; 
      case 10  :
        return 13;
        break; 
      case 11  :
        return 14;
        break; 
      case 12  :
        return 15;
        break;
      case 13  :
        return 16;
        break; 
      case 14  :
        return 17;
        break;  
      default : 
        return 0;   // Should not occur and indicates an error condition.
      }
  }
  else if (cardsuit!=trick and cardsuit!=trump and cardsuit==opcolor) {
      switch(cardrank){
      case 9  :
        return 1;
        break; 
      case 10  :
        return 2;
        break; 
      case 11  :
        return 23;
        break; 
      case 12  :
        return 4;
        break;
      case 13  :
        return 5;
        break; 
      case 14  :
        return 6;
        break;  
      default : 
        return 0;   // Should not occur and indicates an error condition.
      }
  }
  else if (cardsuit!=trick and cardsuit!=trump and cardsuit!=opcolor) {
      switch(cardrank){
      case 9  :
        return 1;
        break; 
      case 10  :
        return 2;
        break; 
      case 11  :
        return 3;
        break; 
      case 12  :
        return 4;
        break;
      case 13  :
        return 5;
        break; 
      case 14  :
        return 6;
        break;  
      default : 
        return 0;   // Should not occur and indicates an error condition.
      }
  }
  return 0;
};

int lowestcard(Card c[], char trump, char trick, int size) {
  //   logentry("Function Call - lowestcard(Card c[], char trump, char trick, int size)");
   int lowestcard=0;
   int lowestvalue=24;
   int tempvalue;
   for (int i=0; i<size ; i++) {
     tempvalue=cardvalue(c[i], trump, trick);
     if (tempvalue <= lowestvalue and tempvalue!=0) { lowestcard = i; lowestvalue=tempvalue; }; 
   };
   return lowestcard;
};

int lowestcardgt(Card c[], int limit, char trump, char trick, int size) {
  //  logentry("Function Call - lowestcardgt(Card c[], int limit, char trump, char trick, int size)");
   int lowestcard=-1;      // -1 indicates no card was found that applied.
   int lowestvalue=24;
   int tempvalue;
   for (int i=0; i<size ; i++) {
     tempvalue=cardvalue(c[i], trump, trick);
     if (tempvalue <= lowestvalue and tempvalue!=0 and tempvalue>limit) { lowestcard = i; lowestvalue=tempvalue; }; 
   };
   return lowestcard;
};

int highestcard(Card c[], char trump, char trick, int size) {
  //  logentry("Function Call - highestcard(Card c[], char trump, char trick, int size)");
   int highestcard=0;
   int highestvalue=0;
   int tempvalue;
   for (int i=0; i<size ; i++) {
     tempvalue=cardvalue(c[i], trump, trick);
     if (tempvalue >= highestvalue) { highestcard = i; highestvalue=tempvalue; }; 
   };
   return highestcard;
};

int lowestcardof(Card c[], char suit, char trump, int size) {
  //  logentry("Function Call - lowestcardof(Card c[], char suit, char trump, int size)");
   int lowestcard=-1;   // -1 = Not found 
   int lowestvalue=24;
   int tempvalue;
   for (int i=0; i<size ; i++) {
     tempvalue=cardvalue(c[i], suit, 'N');
     if (tempvalue <= lowestvalue and tempvalue!=0 and insuit(c[i], suit, trump))  
       { lowestcard = i; lowestvalue=tempvalue; }; 
   };
   return lowestcard;
};

int lowestcardofgt(Card c[], int limit, char suit, char trump, int size) {
  //  logentry("Function Call - lowestcardofgt(Card c[], int limit, char suit, char trump, int size)");
   int lowestcard=-1;  // -1 indicates no card was found.
   int lowestvalue=24;
   int tempvalue;
   for (int i=0; i<size ; i++) {
     tempvalue=cardvalue(c[i], suit, 'N');
     if (tempvalue <= lowestvalue and tempvalue!=0 and insuit(c[i], suit, trump) and tempvalue>limit) 
       { lowestcard = i; lowestvalue=tempvalue; }; 
   };
   return lowestcard;
};

int highestcardof(Card c[], char suit, char trump, int size) {
  //  logentry("Function Call - highestcardof(Card c[], char suit, char trump, int size)");
   int highestcard=-1;  // -1 = None found
   int highestvalue=0;
   int tempvalue;
   for (int i=0; i<size ; i++) {
     tempvalue=cardvalue(c[i], suit, 'N');
     if (tempvalue >= highestvalue and insuit(c[i], suit, trump)) { highestcard = i; highestvalue=tempvalue; }; 
   };
   return highestcard;
};

int lowestcardnotof(Card c[], char suit, char trump, int size) {
  //  logentry("Function Call - lowestcardnotof(Card c[], char suit, char trump, int size)");
   int lowestcard=-1;   // -1 = Not Found
   int lowestvalue=24;
   int tempvalue;
   for (int i=0; i<size ; i++) {
     tempvalue=cardvalue(c[i], suit, 'N');
     if (tempvalue <= lowestvalue and tempvalue!=0 and notinsuit(c[i], suit, trump)) { lowestcard = i; lowestvalue=tempvalue; }; 
   };
   return lowestcard;
};

int highestcardnotof(Card c[], char suit, char trump, int size) {
  //  logentry("Function Call - highestcardnotof(Card c[], char suit, char trump, int size)");
   int highestcard=-1;  // -1 Not Found
   int highestvalue=0;
   int tempvalue;
   for (int i=0; i<size ; i++) {
     tempvalue=cardvalue(c[i], suit, 'N');
     if (tempvalue >= highestvalue and notinsuit(c[i], suit, trump)) { highestcard = i; highestvalue=tempvalue; }; 
   };
   return highestcard;
};

void logstate(string comment) {
  if(uselogfile) {

  };
  if (logtoscreen) {
    cout << "\033[32m\n";
    cout << "\033[0m\n";
  };

};

void callinground() {
  string cardinput;
  Card topkitty;
  char tempinput;
  int i;
  int player;
  int cardsum;
  int hsum;
  int ssum;
  int csum;
  int dsum;
  int bestsum;
  int templowest;
  speak("What is the top card of the kitty? ->");
  cin >> cardinput;
  topkitty = cardinput;
  cout << topkitty << ", " << topkitty.verboseCard() << "\n";
  logentry("Top Card of the Kitty -=> " + topkitty.verboseCard());
  // pop it on the stack =topkitty;
  trumpsuit='N';
  i=0;
  while ((trumpsuit == 'N') and (i < 4)) {
    player=(firstplayer+i) % 4;
    if (player != 0) {
      speak(playername[player] + ", Order it up, or Pass?");
      cout << "Please enter 'O' or 'P' -> ";
      cin >> tempinput;
      if (toupper(tempinput)=='O') {
         trumpsuit=topkitty.getSuit(); maker=player;
         logentry(playername[player] + " Ordered it Up.");
      }
      else {
         logentry(playername[player] + " passed.");
      };
    i++;
    }
    else {
      cardsum=0;
      templowest = lowestcard(myhand, topkitty.getSuit(), 'N', 5);
      for (int j=0; j<5; j++) {
        if (j != templowest) { cardsum=cardsum+cardvalue(myhand[j], topkitty.getSuit(), 'N'); }
      };
      cardsum=cardsum+cardvalue(topkitty, topkitty.getSuit(), 'N');
      if (cardsum >=trumpthresh) {
        trumpsuit=topkitty.getSuit();
        speak("I'll order it up!"); 
        logentry("AI Orders it Up!");
        cout << myhand[templowest] << " ";
        speak("And discard card # " + to_string(templowest+1) + " the " + myhand[templowest].verboseCard() + ".");
        myhand[templowest]=topkitty;
        maker=player;
        // pop the card off the "played" stack
      }
      else { speak ("I'll pass."); logentry("AI Passed");};
      i++;
    }
  }
  // If trump is stil 'N' everyone passed.
  if (trumpsuit == 'N') {
    logentry("Everyone Passes...");
  };
  i=0;
  while ((trumpsuit == 'N') and (i < 4)) {
    player=(firstplayer+i) % 4;
    if (player != 0) {
      speak(playername[player] + ", Choose Trump?  or Pass?");
      cout << "Please enter ";
      if (topkitty.getSuit() != 'S') cout << "'S' "; 
      if (topkitty.getSuit() != 'D') cout << "'D' "; 
      if (topkitty.getSuit() != 'H') cout << "'H' "; 
      if (topkitty.getSuit() != 'C') cout << "'C' ";
      cout << "or 'P' to Pass.\n"; 
      cin >> tempinput;
      tempinput=toupper(tempinput);
      if (tempinput=='S' and topkitty.getSuit() !='S') { trumpsuit='S'; maker=player; logentry(playername[player] + " chooses Spades."); }
      else if (tempinput=='D' and topkitty.getSuit() !='D') { trumpsuit='D'; maker=player; logentry(playername[player] + " chooses Diamonds."); }
      else if (tempinput=='H' and topkitty.getSuit() !='H') { trumpsuit='H'; maker=player; logentry(playername[player] + " chooses Hearts."); }
      else if (tempinput=='C' and topkitty.getSuit() !='C') { trumpsuit='C'; maker=player; logentry(playername[player] + " chooses Clubs."); }
      else { logentry(playername[player] + " Passes."); cout << "Pass\n"; i++; }
    }
    else {
      ssum=0;
      for (int j=0; j<5; j++) {
        if (topkitty.getSuit() != 'S') { ssum=ssum+cardvalue(myhand[j], 'S', 'N'); }
      };
      dsum=0;
      for (int j=0; j<5; j++) {
        if (topkitty.getSuit() != 'D') { dsum=dsum+cardvalue(myhand[j], 'D', 'N'); }
      };
      hsum=0;
      for (int j=0; j<5; j++) {
        if (topkitty.getSuit() != 'H') { hsum=hsum+cardvalue(myhand[j], 'H', 'N'); }
      };
      csum=0;
      for (int j=0; j<5; j++) {
        if (topkitty.getSuit() != 'C') { csum=csum+cardvalue(myhand[j], 'C', 'N'); }
      };
      bestsum=0;
      if (ssum>=trumpthresh and ssum>=bestsum) { trumpsuit='S'; maker=player; bestsum=ssum; }
      if (dsum>=trumpthresh and dsum>=bestsum) { trumpsuit='D'; maker=player; bestsum=dsum; }
      if (hsum>=trumpthresh and hsum>=bestsum) { trumpsuit='H'; maker=player; bestsum=hsum; }
      if (csum>=trumpthresh and csum>=bestsum) { trumpsuit='C'; maker=player; bestsum=csum; }
      else { trumpsuit='N'; };
      if (trumpsuit!='N') { speak("I choose " + suitName(trumpsuit) + "."); logentry("AI chooses " + suitName(trumpsuit)); }
      else { speak("I'll pass."); logentry(playername[player] + "AI Passes."); }
      i++;
    }
    
  }
  if (trumpsuit=='N') { speak("No one chose a trump, re-deal..."); }
  else { speak("Trump is " + suitName(trumpsuit) + ".") ; logentry("Trump Suit is -=> " + suitName(trumpsuit)); }
};

void showmyhand() {             // Strictly a testing function to be called when necessary.
  cout << "1 [0] " << myhand[0] << " cv(" << cardvalue(myhand[0], trumpsuit, tricksuit) << "), ";
  cout << "2 [1] " << myhand[1] << " cv(" << cardvalue(myhand[1], trumpsuit, tricksuit) << "), ";
  cout << "3 [2] " << myhand[2] << " cv(" << cardvalue(myhand[2], trumpsuit, tricksuit) << "), ";
  cout << "4 [3] " << myhand[3] << " cv(" << cardvalue(myhand[3], trumpsuit, tricksuit) << "), ";
  cout << "5 [4] " << myhand[4] << " cv(" << cardvalue(myhand[4], trumpsuit, tricksuit) << ") \n";
  cout << "My lowest card is - " << lowestcard(myhand, trumpsuit, tricksuit, 5) << " \n";
  cout << "My highest card is - " << highestcard(myhand, trumpsuit, tricksuit, 5) << " \n";
  cout << "Given Trump:" << trumpsuit << " Trick:" << tricksuit << ".\n";  
};

void logmyhand() {             // Strictly a testing function to be called when necessary.
  if (uselogfile) {
    logfile << "Current Display of Cards.\n";
    logfile << "1 [0] " << myhand[0].conciseCard() << " cv(" << cardvalue(myhand[0], trumpsuit, tricksuit) << ")  /  ";
    logfile << "2 [1] " << myhand[1].conciseCard() << " cv(" << cardvalue(myhand[1], trumpsuit, tricksuit) << ")  /  ";
    logfile << "3 [2] " << myhand[2].conciseCard() << " cv(" << cardvalue(myhand[2], trumpsuit, tricksuit) << ")  /  ";
    logfile << "4 [3] " << myhand[3].conciseCard() << " cv(" << cardvalue(myhand[3], trumpsuit, tricksuit) << ")  /  ";
    logfile << "5 [4] " << myhand[4].conciseCard() << " cv(" << cardvalue(myhand[4], trumpsuit, tricksuit) << ") \n";
    logfile << "My lowest card is - " << lowestcard(myhand, trumpsuit, tricksuit, 5) << " \n";
    logfile << "My highest card is - " << highestcard(myhand, trumpsuit, tricksuit, 5) << " \n";
    logfile << "Given Trump:" << trumpsuit << " Trick:" << tricksuit << ".\n";
    logfile << "\n";
  };
  if (logtoscreen) {
    cout << "\033[32mCurrent Display of Cards\n";
    cout << "1 [0] " << myhand[0].conciseCard() << " cv(" << cardvalue(myhand[0], trumpsuit, tricksuit) << ")  /  ";
    cout << "2 [1] " << myhand[1].conciseCard() << " cv(" << cardvalue(myhand[1], trumpsuit, tricksuit) << ")  /  ";
    cout << "3 [2] " << myhand[2].conciseCard() << " cv(" << cardvalue(myhand[2], trumpsuit, tricksuit) << ")  /  ";
    cout << "4 [3] " << myhand[3].conciseCard() << " cv(" << cardvalue(myhand[3], trumpsuit, tricksuit) << ")  /  ";
    cout << "5 [4] " << myhand[4].conciseCard() << " cv(" << cardvalue(myhand[4], trumpsuit, tricksuit) << ") \n";
    cout << "My lowest card is - " << lowestcard(myhand, trumpsuit, tricksuit, 5)+1 << " \n";
    cout << "My highest card is - " << highestcard(myhand, trumpsuit, tricksuit, 5)+1 << " \n";
    cout << "Given Trump:" << trumpsuit << " Trick:" << tricksuit << ".\n";
    cout << "\033[0m\n";  
  };
};

void logthistrick() {             // Strictly a testing function to be called when necessary.
  if (uselogfile) {
    logfile << "Current Display of Trick Cards.\n";
    logfile << "[0] " << thistrick[0].conciseCard() << " cv(" << cardvalue(thistrick[0], trumpsuit, tricksuit) << ")  /  ";
    logfile << "[1] " << thistrick[1].conciseCard() << " cv(" << cardvalue(thistrick[1], trumpsuit, tricksuit) << ")  /  ";
    logfile << "[2] " << thistrick[2].conciseCard() << " cv(" << cardvalue(thistrick[2], trumpsuit, tricksuit) << ")  /  ";
    logfile << "[3] " << thistrick[3].conciseCard() << " cv(" << cardvalue(thistrick[3], trumpsuit, tricksuit) << ")\n";
    logfile << "The lowest card is - " << lowestcard(thistrick, trumpsuit, tricksuit, 5) << " \n";
    logfile << "The highest card is - " << highestcard(thistrick, trumpsuit, tricksuit, 5) << " \n";
    logfile << "Given Trump:" << trumpsuit << " Trick:" << tricksuit << ".\n";
    logfile << "\n";
  };
  if (logtoscreen) {
    cout << "\033[32mCurrent Display of Trick Cards\n";
    cout << "[0] " << thistrick[0].conciseCard() << " cv(" << cardvalue(thistrick[0], trumpsuit, tricksuit) << ")  /  ";
    cout << "[1] " << thistrick[1].conciseCard() << " cv(" << cardvalue(thistrick[1], trumpsuit, tricksuit) << ")  /  ";
    cout << "[2] " << thistrick[2].conciseCard() << " cv(" << cardvalue(thistrick[2], trumpsuit, tricksuit) << ")  /  ";
    cout << "[3] " << thistrick[3].conciseCard() << " cv(" << cardvalue(thistrick[3], trumpsuit, tricksuit) << ")\n";
    cout << "The lowest card is - " << lowestcard(thistrick, trumpsuit, tricksuit, 5) << " \n";
    cout << "The highest card is - " << highestcard(thistrick, trumpsuit, tricksuit, 5) << " \n";
    cout << "Given Trump:" << trumpsuit << " Trick:" << tricksuit << ".\n";
    cout << "\033[0m\n";  
  };
};

void readmycards() {
  string cardinput;
  speak(playername[2] + ", could you help me see my cards?");
  cout << "\n";
  for (int i=0; i<5 ; i++) {
    cout << "Enter or scan card for position # " << i+1 << " -> ";
    cin >> cardinput;
    myhand[i]=cardinput;
  };
  cout << "\n\n";
  speak("Thank you " + playername[2] + ".");
  logentry("Cards Read In -");
  logmyhand();
};

int playtrick(int startplayer) {     // returns the player who won the trick
//  Card thistrick[5];
  Card tempcard;
  string tempinput;
  int cardtoplay;
  int player;
  int i=0;
  Card Joker;
  player=(startplayer+i) % 4;  // For the first card
  // showmyhand();
  if (player==0) {
    logentry("AI going first in this trick.");
    logentry("");
    logentry("SHORT AI ROUTINE BEGINS");
    logentry("=======================");
    logmyhand();
    cardtoplay=highestcardnotof(myhand, trumpsuit, trumpsuit, 5);   // Attempt to play highest non-trump
    if (cardtoplay!=-1) {
      logentry("Found a Non-Trump to play first. (FTW!)");
      thistrick[player]=myhand[cardtoplay];
      tricksuit=myhand[cardtoplay].getSuit();   // This is the first card, so it sets the tricksuit.
      cout << myhand[cardtoplay];
      speak(" I'm playing card number " + to_string(cardtoplay+1) + " the " + myhand[cardtoplay].verboseCard() + ".\n");
      logentry(" I'm playing card number " + to_string(cardtoplay+1) + " the " + myhand[cardtoplay].verboseCard());
      myhand[cardtoplay]=Joker ; // Basically turn that card to a joker/blank
    }
    else { 
      // No non-trump cards, go for the highest
      logentry("I have no non-trump, must play a trump. (FTW!)");
      cardtoplay=highestcardof(myhand, trumpsuit, trumpsuit, 5);   // Attempt to play highest trump
      thistrick[player]=myhand[cardtoplay];
      tricksuit=myhand[cardtoplay].getSuit();   // This is the first card, so it sets the tricksuit.
      cout << myhand[cardtoplay];
      speak(" I'm playing card number " + to_string(cardtoplay+1) + " the " + myhand[cardtoplay].verboseCard() + ".\n");
      logentry(" I'm playing card number " + to_string(cardtoplay+1) + " the " + myhand[cardtoplay].verboseCard());
      myhand[cardtoplay]=Joker ; // Basically turn that card to a joker/blank
    };
  }
  else {
    logentry(playername[player] + " is going first in this trick.");
    // This is the human case.
    cout << "Your card " << playername[player] << "? -> \n";
    cin >> tempinput;
    tempcard=tempinput;
    cout << playername[player] << " plays " << tempcard << " the " << tempcard.verboseCard() << ".\n";
    logentry("Human " + playername[player] + " plays " + tempcard.verboseCard()); 
    thistrick[player]=tempcard;
    tricksuit=tempcard.getSuit();   // This is the first card, so it sets the tricksuit.
  };
  logentry ("Trick Suit is -=> " + suitName(tricksuit));
  speak("Trick Suit is -=> " + suitName(tricksuit));  
  for (int i=1; i<4; i++) {
    player=(startplayer+i) % 4;
    if (player==0) {
      logentry("My turn to go - Trump=" + suitName(trumpsuit) + " - Trick=" + suitName(tricksuit));
      logentry("MAIN AI ROUTINE BEGINS");
      logentry("======================");
      logmyhand();
      logthistrick();
      // This is the AI Code for choosing what card to play on any given trick if not first.
      logentry("Checking for Trick cards - " + suitName(tricksuit));
      if (gotany(myhand, tricksuit, trumpsuit, 5)) {
        logentry(":.I have Trick cards.  Check for played Trump cards.");
        if (gotany(thistrick, trumpsuit, trumpsuit, 4)) {
          logentry("  :.There are Trump cards played - Play lowest Trick card. (Give up!)");
          cardtoplay=lowestcardof(myhand, tricksuit, trumpsuit, 5);
        }
        else {
          logentry("  :.There are no Trump cards played, Play highest Trick card. (FTW!)");
          cardtoplay=highestcardof(myhand, tricksuit, trumpsuit, 5);
        };
      }
      else {
        logentry(":.I do not have Trick cards.  Do I have any Trump - " + suitName(trumpsuit));
        if (gotany(myhand, trumpsuit, trumpsuit, 5)) {
          logentry("  :.I do have Trump, are any other Trump cards played?");
          if (gotany(thistrick, trumpsuit, trumpsuit, 4)) {
            logentry("    :.There are Trump Cards Played - Is it higher than my highest Trump?");
            if (cardvalue(thistrick[highestcardof(thistrick, trumpsuit, trumpsuit, 4)] , trumpsuit, tricksuit) > cardvalue(myhand[highestcardof(myhand, trumpsuit, trumpsuit, 5)] , trumpsuit, tricksuit) ) {
              logentry("      :.Yes, so throw garbage (Give up!)");
              cardtoplay=lowestcard(myhand, trumpsuit, tricksuit, 5);
            }
            else {
              logentry("      :.No, so is that card my partner's? (Be a Team Player!)");
              if (highestcardof(thistrick, trumpsuit, trumpsuit, 4)==2) {   // Highest trump is my partner's
                logentry("        :.Yes, so throw garbage (Let Partner Win!)");
                cardtoplay=lowestcard(myhand, trumpsuit, tricksuit, 5);
              }
              else {
                logentry("        :.No, it is theirs, so play the lowest Trump card higher than theirs. (FTW!)");
                cardtoplay=lowestcardofgt(myhand, cardvalue(thistrick[highestcardof(thistrick, trumpsuit, trumpsuit, 4)] , trumpsuit, tricksuit), trumpsuit, trumpsuit, 5);
              };
            };
          }
          else {
            logentry("    :.There are no other Trump cards, so play my lowest Trump card. (FTW!)");
            cardtoplay=lowestcardof(myhand, trumpsuit, trumpsuit, 5);
          };
        }
        else {
          logentry("  :.I do not have Trick or Trump - Toss Garbage (Give up!");
          cardtoplay=lowestcard(myhand, trumpsuit, tricksuit, 5);
        };
      };
      // cardtoplay should now be a valid card, so play it.
      thistrick[player]=myhand[cardtoplay];
      cout << myhand[cardtoplay];
      speak(" I'm playing card number " + to_string(cardtoplay+1) + " the " + myhand[cardtoplay].verboseCard() + ".\n");
      logentry(" I'm playing card number " + to_string(cardtoplay+1) + " the " + myhand[cardtoplay].verboseCard());
      myhand[cardtoplay]=Joker ; // Basically turn that card to a joker/blank
    }
    else {
      // This is the human case.
      cout << "Your card " << playername[player] << "? -> \n";
      cin >> tempinput;
      tempcard=tempinput;
      cout << playername[player] << " plays " << tempcard << " the " << tempcard.verboseCard() << ".\n";
      logentry("Human " + playername[player] + " plays " + tempcard.verboseCard());
      thistrick[player]=tempcard;
    };
  };
  return highestcard(thistrick, trumpsuit, tricksuit, 4);
};

void scoretrick(int winner, int maker) {
  speak(playername[winner] + " takes the trick!");
  logentry(playername[winner] + " takes the trick!");
  logentry("");
  logentry("");
  tricksbyplayer[winner]++;
  if (winner==1 or winner==3) {
    tricksteamb++;
  }
  else {
    tricksteama++;
  };

};

void scorehand(int tricksteama, int tricksteamb, int maker) {
  if (maker==1 or maker==3) {
  	if (tricksteamb == 5) {
      speak("Team B were the Makers and swept the tricks and score 2 points.");
      logentry("Team B were the Makers and swept the tricks and score 2 points.");
  	  scoreteamb=scoreteamb+2;
  	} // Team B sweeps (all 5 tricks, 2 points)
  	else if (tricksteamb == 3 or tricksteamb == 4) {
      speak("Team B were the Makers and got more than 3 tricks and score 1 point.");
      logentry("Team B were the Makers and got more than 3 tricks and score 1 point.");
  	  scoreteamb=scoreteamb+1;
  	} // Team B gets 3 or 4 tricks, 1 point.
  	else {
      speak("Team B were the Makers and got Euchred.  Team A gets 2 points.");
      logentry("Team B were the Makers and got Euchred.  Team A gets 2 points.");
  	  scoreteama=scoreteama+2;
  	}; // Team B got Euchred by Team A (2 points for them.)
  }
  else {
  	if (tricksteama == 5) {
      speak("Team A were the Makers and swept the tricks and score 2 points.");
      logentry("Team A were the Makers and swept the tricks and score 2 points.");
  	  scoreteama=scoreteama+2;
  	} // Team A sweeps (all 5 tricks, 2 points)
  	else if (tricksteama == 3 or tricksteama == 4) {
      speak("Team A were the Makers and got more than 3 tricks and score 1 point.");
      logentry("Team A were the Makers and got more than 3 tricks and score 1 point.");
  	  scoreteama=scoreteama+1;
  	} // Team A gets 3 or 4 tricks, 1 point.
  	else {
      speak("Team A were the Makers and got Euchred.  Team B gets 2 points.");
      logentry("Team A were the Makers and got Euchred.  Team B gets 2 points.");
  	  scoreteamb=scoreteamb+2;
  	}; // Team A got Euchred by Team B (2 points for them.)
  };
  
};

int main() {

  welcome();                               // Welcome Message, Helpful Information, Variables.
  introduce();                             // Get the names of the Human Players and the teams, points to win
  // while (keepplaying) {
  begingame();                             // Reset the scores and reset variables for played cards, etc.
  while (keepplaying) {
    trumpsuit='N';
    tricksuit='N';
    tricksteama=0;
    tricksteamb=0;
    dealer=hands%4;
    firstplayer=(hands+1)%4;
    if (dealer == 0) deal();        // Deal, if dealer, or wait. (Only says how many cards to each player)
    readmycards();                             // Human help to read computer's hand
    // showmyhand();
    logmyhand();
    logentry("CALLING ROUND");
    logentry("=============");
    logentry("");
    callinground();                            // Figure Trump and PlayAlone
    // showmyhand();
    logmyhand();
    if (trumpsuit!='N') {
      for (int trick=1; trick<=5; trick++) {        // for 1 to 5 {
        tricksuit='N';
        logentry("PLAY A TRICK");
        logentry("============");
        logentry("");
        trickwinner=playtrick(firstplayer);
        firstplayer=trickwinner;
        scoretrick(trickwinner,maker);
      };                            
    };
    scorehand(tricksteama, tricksteamb, maker);
    hands++;
    displayscore();
    keepplaying=yesorno("Shall we play another hand?");
  };
  displayscore();
  goodbye();

};
