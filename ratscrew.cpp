#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <string>
#include <deque>
#include <set>
#include <ctime>
#include <time.h> 
#include <conio.h>

using namespace std;

string suits[] = {"\x03", "\x04", "\x05", "\x06"};
string vals[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
int delays[] = {5000, 2000, 1000, 500, 350};
bool showReactionTime = true;
bool showDeckSize = true;

class Card {
  private:
    string suit;
    string val;
  public:
    Card(string s, string v) {
      suit = s;
      val = v;
    }
    string getSuit() {
      return suit;
    }
    string getVal() {
      return val;
    }
};

class Player {
  private:
    deque<Card> deck;
    string name;
  public:
    Player(deque<Card> d, string n) {
      deck = d;
      name = n;
    }
    void takeCards(deque<Card> &s) {
      for (int i = s.size() - 1; i > -1; i--) {
        // put cards from the back of the stack onto the back of the deck
        deck.push_back(s.at(i));
        s.pop_back();
      }
    }
    void place_card(deque<Card> &s) {
      // place a card from the front of the deck to the front of the stack
      s.push_front(deck.front());
      deck.pop_front();
    }
    bool hasCards() {
      return !deck.empty();
    }
    string getName() {
      return name;
    }
    int getDeckSize() {
      return deck.size();
    }
};

// construct a deque using a certain number of decks of cards
deque<Card> constructDeck(int size) {
  deque<Card> d;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < 52; j++) {
      string suit = suits[j/13];
      string val = vals[j % 13];
      //cout << suit << val << endl;
      d.push_back(Card(suit, val));
    }
  }
  return d;
}

vector<string> printCard(Card c) {
  vector<string> card;
  card.push_back("\xC9\xCD\xCD\xCD\xBB");
  card.push_back("\xBA "+c.getSuit()+c.getVal()+"\xBA");
  card.push_back("\xBA   \xBA");
  card.push_back("\xBA   \xBA");
  card.push_back("\xC8\xCD\xCD\xCD\xBC");
  if (c.getVal() == "10") {
    card.at(1) = card.at(1).replace(1,1,"");
  }
  return card;
}

void shuffle(deque<Card> &deck) {
  srand(time(0));
  for (int i= deck.size() - 1; i > -1; i--) { 
    int j = rand() % (i + 1);
    if(i != j) {
      swap(deck[j], deck[i]);
    }
  }
}

void deal(deque<Card> &d, deque<Card> &d1, deque<Card> &d2) {
  shuffle(d);
  for (int i = 0; i < d.size(); i++) {
    if (i % 2 == 0) {
      d1.push_back(d.at(i));
    } else {
      d2.push_back(d.at(i));
    }
  }
}

bool sameCard(Card c1, Card c2) {
  if (c1.getVal() == c2.getVal()) {
    if (c1.getSuit() == c2.getSuit()) {
      return true;
    }
  }
  return false;
}

bool checkPowerCard(Card c, Card &p, int &pc) {
  string v = c.getVal();
  if (v == "A") {
      p = c;
      pc = 4;
      return true;
  } else if (v == "J") {
      p = c;
      pc = 1;
      return true;
  } else if (v == "Q") {
      p = c;
      pc = 2;
      return true;
  } else if (v == "K") {
      p = c;
      pc = 3;
      return true;
  } else {
      if (pc != -1) pc--;
      return false;
  }
  return false;
}

void sleep(unsigned int mseconds) {
  clock_t goal = mseconds + clock();
  while (goal > clock());
}

bool isSlap(deque<Card> &s) {
  // check if double
  if (s.size() >= 2) {
    if (s.at(0).getVal() == s.at(1).getVal()) {
      return true;
    }
  }
  // check if sandwich
  if (s.size() >= 3) {
    if (s.at(0).getVal() == s.at(2).getVal()) {
      return true;
    }
  }
  return false;
}

int main() {
  bool gameover;
  bool pturn;
  char ch;
  int difficulty = 1;
  int slap_delay;
  deque<Card> stack;
  deque<Card> burntCards;
  
  // construct deck
  deque<Card> deck = constructDeck(1);
  
  // Prepare game
  string input;
  cout << "Welcome to Egyptian Ratscrew." << endl;
  cout << endl;
  cout << "Enter your name: ";
  cin >> input;
  deque<Card> d1;
  deque<Card> d2;
  
  // play game
  ch = 'y';
  clock_t t = clock();
  clock_t delay = clock();
  char key;
  vector<string> p;
  while (ch == 'y') {
    // prepare decks and players
    stack.clear();
    burntCards.clear();
    d1.clear();
    d2.clear();
    deal(deck, d1, d2);
    Player p1(d1, input);
    Player p2(d2, "Computer");
    if (!p1.hasCards() || !p2.hasCards()) {
      cout << endl << "Something went wrong.";
    }
    
    
    // set difficulty
    cout << "Hello "<<p1.getName()<<", are ready for some Egyptian Ratscrew?" << endl;
    cout << "Choose a difficulty level: " << endl << "[0] n00b, [1] Easy, [2] Intermediate, [3] Hard, [4] Pro: ";
    cin >> ch;
    cout << endl;
    difficulty = ch - '0';
    
    // play game
    gameover = false;
    pturn = true;
    Card pcard("0", "0");
    int pcounter = -1;
    while (!gameover) {
      if (!stack.empty()) {
        p = printCard(stack.front());
        for (int i = 0; i < p.size(); i++) {
          cout << p.at(i) << endl;
        }
      }
      if (pturn) {
        // before action
        t = clock();
        
        // register action
        key = '-';
        while (((int)key != 13) && (key != ' ') && (key != 'q'))
          key = _getch();
        
        // after action
        delay = clock() - t;
        
        if ((int)key == 13) {
          if (isSlap(stack)) {
            slap_delay = delays[difficulty] + (rand() % delays[difficulty]);
            if (slap_delay < delay) {
              // computer got the slap!
              cout << "Computer slapped!" << endl;
              p2.takeCards(burntCards);
              p2.takeCards(stack);
              pturn = false;
              pcounter = -1;
              continue;
            } else {
              // player buried the stack
            }
          }
          if (pcounter == 0) {
            cout << "Computer won stack." << endl;
            p2.takeCards(burntCards);
            p2.takeCards(stack);
            pturn = false;
            pcounter = -1;
            continue;
          }
          if (p1.hasCards()) {
            if (showDeckSize) {
              cout << "You play a card (" << p1.getDeckSize() << "): " << endl;
            } else {
              cout << "You play a card: " << endl;
            }
            // player plays the next card
            p1.place_card(stack);
            if (checkPowerCard(stack.front(), pcard, pcounter)) {
              pturn = !pturn;
              continue;
            }
            if (pcounter > -1) {
              // return to same turn, because power card is going
              continue;
            }
          } else {
            // player ran out of cards and lost
            cout << "You ran out of cards!  You lost." << endl;
            pcounter = -1;;
            gameover = true;
          }
        } else if (key == ' ') {
          if (showReactionTime) {
            cout << "SLAP! (" << delay << "ms)" << endl;
          } else {
            cout << "SLAP!" << endl;
          }
          if (isSlap(stack)) {
            slap_delay = delays[difficulty] + (rand() % delays[difficulty]);
            if (slap_delay < delay) {
              // computer got the slap before the player!
              cout << "The computer beat you to the slap!" << endl;
              p2.takeCards(burntCards);
              p2.takeCards(stack);
              pturn = false;
              pcounter = -1;
              continue;
            } else {
              // player got the slap!
              cout << "You got the stack!" << endl;
              p1.takeCards(burntCards);
              p1.takeCards(stack);
              pturn = true;
              pcounter = -1;
              continue;
            }
          } else {
            // player false slapped
            cout << "That wasn't a slap!  Burn a card." << endl;
            p1.place_card(burntCards);
            if (!p1.hasCards()) {
              cout << "You ran out of cards!  You lost." << endl;
              pcounter = -1;
              gameover = true;
            }
            continue;
          }
        } else if (key == 'q') {
          cout << "Quitting game." << endl;
          gameover = true;
          break;
        }
      } else {
        // before action
        t = clock();
        
        // register action
        key = '-';
        while (((int)key != 13) && (key != ' ') && (key != 'q'))
          key = _getch();
        
        // after action
        delay = clock() - t;
        
        if ((int)key == 13) {
          if (isSlap(stack)) {
            slap_delay = delays[difficulty] + (rand() % delays[difficulty]);
            if (slap_delay < delay) {
              // computer got the slap!
              cout << "Computer slapped!" << endl;
              p2.takeCards(burntCards);
              p2.takeCards(stack);
              pturn = false;
              pcounter = -1;
              continue;
            } else {
              // player buried the stack
            }
          }
          if (pcounter == 0) {
            // computer lost from power card
            cout << "Player won stack." << endl;
            p1.takeCards(burntCards);
            p1.takeCards(stack);
            pturn = true;
            pcounter = -1;
            continue;
          }
          // not a slap
          if (p2.hasCards()) {
            if (showDeckSize) {
              cout << "The computer plays a card (" << p2.getDeckSize() << "): " << endl;
            } else {
              cout << "The computer plays a card: " << endl;
            }
            // player plays the next card
            p2.place_card(stack);
            if (checkPowerCard(stack.front(), pcard, pcounter)) {
              pturn = !pturn;
              continue;
            }
            if (pcounter > -1) {
              // return to same turn because power card still going
              continue;
            }
          } else {
            // player ran out of cards and lost
            cout << "The computer ran out of cards!  You win!" << endl;
            pcounter = -1;
            gameover = true;
          }
        } else if (key == ' ') {
          if (showReactionTime) {
            cout << "SLAP! (" << delay << "ms)" << endl;
          } else {
            cout << "SLAP!" << endl;
          }
          if (isSlap(stack)) {
            slap_delay = delays[difficulty] + (rand() % delays[difficulty]);
            if (slap_delay < delay) {
              // computer got the slap before the player!
              cout << "The computer beat you to the slap!" << endl;
              p2.takeCards(burntCards);
              p2.takeCards(stack);
              pturn = false;
              pcounter = -1;
              continue;
            } else {
              // player got the slap!
              cout << "You got the stack!" << endl;
              p1.takeCards(burntCards);
              p1.takeCards(stack);
              pturn = true;
              pcounter = -1;
              continue;
            }
          } else {
            // player false slapped
            cout << "That wasn't a slap!  Burn a card." << endl;
            p1.place_card(burntCards);
            if (!p1.hasCards()) {
              cout << "You ran out of cards!  You lost." << endl;
              pcounter = -1;
              gameover = true;
            }
            continue;
          }
        } else if (key == 'q') {
          cout << "Quitting game." << endl;
          pcounter = -1;
          gameover = true;
          break;
        }
      }
      pturn = !pturn;
    }
    cout << endl << "Game over. Play again? y/n ";
    cin >> ch;
    cout << endl;
  }
}
