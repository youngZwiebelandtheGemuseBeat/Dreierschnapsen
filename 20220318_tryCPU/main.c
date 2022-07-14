//
//  main.c
//  20211226_Bauernschnapsen
//
//  Created by Luca Candussi on 26.12.21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//-----------------------------------------------------------------------------
/// definition of various precompiler directives
//
#define ADD_ONE         +1
#define MINUS_ONE       -1
#define CARD_QUANTITY   5
#define QUANTITY_VALUES 5
#define QUANTITY_SUITS  4
#define OFFSET          0
#define MAXIMUM_TURNS   6
#define TURN_PLAYER     1
#define TURN_PLAYER_1     1
#define TURN_PLAYER_2     2
#define TURN_PLAYER_3     3
#define TURN_DEALER       4
#define OPPONENTS_WIN     5
#define TURN_OVER       0
#define CALL_HIT        0
#define CALL_STAND      1
#define INVALID_INPUT   -1
#define BLACKJACK       21
#define CONTINUE        2
#define HIT             'h'
#define STAND           's'
#define ACE             11
#define TEN             10
#define KING            4
#define QUEEN           3
#define JACK            2
#define TRUE            1
#define FALSE           0
#define GO_ON           0
#define BREAK           1
#define ANE_AUF         11

//-----------------------------------------------------------------------------
/// definition of various error and return values
//
#define SUCCESS                   0
#define ERROR_CODE_WRONG_USAGE    -1
#define ERROR_CODE_OUT_OF_MEMORY  -2
#define ERROR_CODE_INVALID_FILES  -3
#define ERROR_CODE_WRONG_MODE     -4    // impossible though
#define ERROR_CODE_WRONG_COMMAND  -5    // impossible as well

//-----------------------------------------------------------------------------
/// ADDITIONAL definition of various error and return values
//
#define HAND            6
#define TALON           2
#define QUANTITY_PLAYERS 3
#define GAME            1
#define SCHNAPSER       2
#define LAND            3
#define BAUERNSCHNAPSER 4
#define WEITER          0
#define JODLER          5
#define HERREN_JODLER   6
#define KONTRA_SCHNAPSER        7
#define KONTRA_BAUERNSCHNAPSER  8
#define SUITS           4
#define CHAR_TO_INT     - '0'
#define INT_TO_CHAR     + '0'
#define HUMAN           'H'
#define INSTANCE_TRUMP    4
#define INSTANCE_POLAR    2
#define INSTANCE_CARD     6
#define RETRY           'R'
#define MAXIMUM_POINTS  66
#define ENOUGH          5
static const char COMMANDS_CARDS[6] = {'q', 'w', 'e', 'a', 's', 'd'};
static const char COMMANDS_POLAR[2] = {'y', 'n'};
static const char COMMANDS_TRUMP[5] = {0, 1, 2, 3, 4};

//-----------------------------------------------------------------------------
/// definition of the 'data type' Card
///
/// image_   stores image string
/// value_   stores card's value
//
typedef struct _Card_
{
  char* image_;
  unsigned int value_;
  char* suit_;
  char* sign_;
  int is_bock_;
  int is_trump_;
} Card;

//-----------------------------------------------------------------------------
/// definition of the 'data type' Turn
///
/// image_   stores image string
/// value_   stores card's value
//
typedef struct _Turn_
{
  Card card_;
  char position_;
  int player_;
} Turn;

//-----------------------------------------------------------------------------
/// definition of the 'data type' Pair
///
/// position_played_   position of the card left of a pair that has been played
/// points_                  stores either 20 or 40 points to  be added to player's or opponents' points (...)
/// bool_trick_            bool to check whether player has made one trick and therefore points may be added
//
typedef struct _Pair_
{
//  int position_played_;
  int points_;
//  int bool_trick_;    // might be neglectable and could therefore be deleted
  int bool_pair_;
} Pair;

//-----------------------------------------------------------------------------
/// definition of the 'data type' Player
///
//
typedef struct _Player_
{
  char name_[10];
  int points_;
  unsigned int CPU_bool_;
} Player;

//-----------------------------------------------------------------------------
/// definition of the 'data type' Points
///
//
typedef struct _Points_
{
  int caller_;
  int points_;
  int winner_;
} Points;

//-----------------------------------------------------------------------------
/// definition of the 'data type' Commands
///
//
typedef struct _Commands_
{
  int player_;
  char commands_[7];
} Commands;

//-----------------------------------------------------------------------------
/// declarations of functions
//
void initializeDummyDeck(Card* dummy_deck);
void printHand(Card* deck, int number, int player, char* name);
void createFullDeck(Card* deck, Card* dummy_deck, char** suits);
void FisherYates(Card* deck, int size,int random_seed);
void dealCards(int turn, int* index, Card* source, Card* target);
void startGame(Card* deck, Card* deck_dealer,
               Card* deck_player_1, Card* deck_player_2, Card* deck_player_3,
               int* turns, int* index_dealer,
               int* index_player_1, int* index_player_2, int* index_player_3,
               int* trump, Player* players, int* order, char** suits,
               Card** hands);
void printErrorMessage(int error_code, char* argument);
//void turnPlayer(int* call, int* state, int* turns, int* index, Card* deck,
//                Card* hand, int* score);
void turnDealer(int* turns, int* state, int* call, int* score_dealer,
                int* score_player, int* index_dealer, int* index_player,
                Card* deck, Card* deck_dealer);
int checkPlayerBlackjack(int* score_player, int* score_dealer, int* state,
                         Card* deck_dealer);
int getSeed(char* argument);
// hier ließen sich wahrscheinlich einige Parameter streichen. Beispielsweise
// score_player_1, player_2, ...
Points playGame(Card* deck, Card* deck_dealer,
                Card* deck_player_1, Card* deck_player_2, Card* deck_player_3,
                int* turns, int* index_dealer,
                int* index_player_1, int* index_player_2, int* index_player_3,
                int* state, int* score_dealer,
                int* score_player_1, int* score_player_2, int* score_player_3,
                int* call, int* trump, char** suits, int* bool_fleck,
                int* bool_retour, char* player_1, char* player_2, char* player_3,
                Player* players, int* order);
int callTrump(Card auf);
int callMode(int mode, int state, int* start, char* player);
int fleck(int player);
int fleckBack(int player);
void switchMode(int* mode);
int raiseMode(int* mode, int start);
int determineBeginner(int value_1, int value_2, int value_3, int* mode,
                      Player* players, int* order);
int habSelbst(int start, int* mode, char* player);
Points modeGame(Card** hands, int start, char* trump, Player* players, int* order);
Points modeRufer(Card** hands, int start, char* trump, Player* players, char** players_commands);
Points switchRufer(Card** hands, int start, char* trump, Player* players,
                  int* order);
Points modeSchnapser(Card** hands, int start, char* trump, Player* players);
Points modeLand(/*int bool_trump,*/ Card** hands, int start, Player* players);
Points modeBauernschnapser(/*int bool_trump,*/ Card** hands, int start, int trump,
                         Player* players);
Points modeJodler(Card** hands, int start, char* suit, Player* players);
Points modeHerrenJodler(Card** hands, int start, char* trump, Player* players);
int callRaise(int mode, /*int state,*/ int start, Player* players);
int buy(int start, Card** hands, Card* deck_dealer, int* done_1, int* done_2,
        Player* players, int* order);
void switchCards(Card* a, Card* b);
int highest(int bool_trump, int start, Card call, Card answer_1, Card answer_2);
void countPoints(int bool_trump, int start, Card call, Card answer_1,
                 Card answer_2, int* points_call, int* points_opponent);
int seekAndDestroy(char find, char* list);
//int removeChar(char *str, char c);
void playCard(Card* card);
void getCall(int start, int* call, int* answer_1, int* answer_2);
void removeCard(Card* card);
char* decodeSuit(int trump);
int callSuit(int start);
void setTrump(Card* deck, int trump);
void nextAndPoints(int* start, int buffer_start, Card call, Card answer_1,
                   Card answer_2, int* points_call, int* points_opponents,
                   int* points_1, int* points_2, int* points_3, int* order);
void setOrder(Card* deck, int test_case, int trump);
void sortHands(Card** hands, char** suits);
//void copyCard(Card* source, Card* target);
void sort(int* positions, Card* hand, int quantity, int* position_hand,
          Card* buffer, char* suit);
int highlight(Card* hand, int player, int* positions);
int resetPairs(int* position);
void resetHandlePairs(Pair* handle_pairs);
int pairHandler(Pair* handle_pairs, int position, int points_to_add,
                 int* i, int* points_call, int* points_opponents,
                 Card* hand, int is_caller);
int checkContinue(char* player);
void swapOrder(Player* players);
int decomposeQWEASD (char character);
void printTable(Player players);
// CPU players
char randomCall(int instance);
int validCommandCPU(char to_compare, char* with);
char checkCPU(Player to_check, int instance, char* commands);
// new additions and fixes 20220628
Points next(int* initial_order, int* player, Card call, Card answer_1,
          Card answer_2);
int highestCard(Card* cards);
void printBummerl(Player* players);
void distributePoints(int points_call, int points_opponents,
                      Points* points_and_caller);
void sortOrderHands(int start, Card** hands, Card** initial_hands);
void sortOrderPlayers(int start, Player* players, Player* initial_order);
void sortOrderCommands(int start, char** players_commands, int* initial_commands);

//-----------------------------------------------------------------------------
///
/// This function handles the whole process by defining and initializing
/// various variables, calling functions and terminating the programm in
/// case of an error or the end of the game.
///
/// @param  argc    number of command line arguments received seperated by
///                 space
/// @param  argv    array where command line arguments are stored
///
/// @return         0 in case of having successfully started and played the
///                 game or an integer value between -3 and -1 depending on
///                 its corresponding error case
//
int main(int argc, char* argv[])
{
  char* path                = NULL;
  int error_code            = SUCCESS;
  int turns                 = 0;
  int index_dealer          = 0;
  int index_player_1        = 0;
  int index_player_2        = 0;
  int index_player_3        = 0;
  int state                 = 0;
  int call                  = 0;
  int random_seed           = 0;
  int score_dealer          = 0;
//  int score_player_1        = 0;
//  int score_player_2        = 0;
//  int score_player_3        = 0;
  int counter               = 0;
  int trump                 = 0;
  // die gehören eigentlich alle "sauberer"/eleganter initialisiert -----------
  Card dummy_deck[CARD_QUANTITY]    = {{NULL}, {0}, "", ' '};
  Card deck[CARD_QUANTITY * 4]      = {{NULL}, {0}, "", ' '};
  Card deck_dealer[2]       = {{NULL}, {0}, "", ' '};
  Card deck_player_1[HAND]  = {{NULL}, {0}, "", ' '};
  Card deck_player_2[HAND]  = {{NULL}, {0}, "", ' '};
  Card deck_player_3[HAND]  = {{NULL}, {0}, "", ' '};
  // --------------------------------------------------------------------------
  char* suits[SUITS]  = {"hearts", "spades", "clubs", "diamonds"};
  void playCard(Card* card);
  int bool_fleck            = 0;
  int bool_retour           = 0;
//  int points_A              = 0;
//  int points_B              = 0;
//  int points_C              = 0;
  Player players[QUANTITY_PLAYERS] = {{"Seppi ", 0, TRUE},
                                      {"Hansi ", 0, TRUE},    // CPU
                                      {"Franzi", 0, TRUE}};   // CPU
  int counter_players       = 0;
  int go_on                 = TRUE;
  int check_continue[QUANTITY_PLAYERS] = {0, 0, 0};
  
  Points points_and_caller  = {0, 0, 0, 0};
  int bummerl[QUANTITY_PLAYERS] = {0, 0, 0};
  int order[3] = {0, 1, 2};
  
  // TODO:
  // CHECK USAGE:
//  if (argc < 2 || argc > 3)
//  {
//    error_code = ERROR_CODE_WRONG_USAGE;
//    printErrorMessage(error_code, argv[0]);
//  }
  
  // potential program call error handling
  if(0);
  
  // EXECUTE PROGRAM:
  else
  {
    initializeDummyDeck(dummy_deck);
    createFullDeck(deck, dummy_deck, suits);
    
    if (error_code == SUCCESS)
    {
      // loop: another Bummerl (so far until at least one player quits)
      do
      {
        // loop (until at least one player has >= 24 points)
        counter_players = 0;
        do
        {
          // initialize deck for each round
//          initializeDummyDeck(dummy_deck);            // dummy_deck = initialization of values
//          createFullDeck(deck, dummy_deck, suits);
          index_player_1  = 0;
          index_player_2  = 0;
          index_player_3  = 0;
          index_dealer    = 0;
          turns           = 0;
          state           = 0;
          
          // with loop iterating check who's currently who
//          if (!(counter % 3))
//          {
//            printf("Player %d (%s) calls trump.\n",
//                   order[counter_players] ADD_ONE, players[order[counter_players]].name_);
//            printf("---------------------------------------------------------------\n");
//          }
//
//          else if (!((counter MINUS_ONE) % 3))
//          {
//            printf("Player %d (%s) calls trump.\n",
//                   order[counter_players] ADD_ONE, players[order[counter_players]].name_);
//            printf("---------------------------------------------------------------\n");
//          }
//
//          else
//          {
//            printf("Player %d (%s) calls trump.\n",
//                   order[counter_players] ADD_ONE, players[order[counter_players]].name_);
//            printf("---------------------------------------------------------------\n");
//          }
          
          // GAME
          random_seed = getSeed(argv[1]);
          FisherYates(deck, CARD_QUANTITY * 4, random_seed);
          
          // ist mir jetzt Wurscht, lös'ma's (derweil) halt billig -------------------
          if (counter_players == 1)
          {
            order[0] = counter_players;
            order[1] = 2;
            order[2] = 0;
          }

          if (counter_players == 2)
          {
            order[0] = counter_players;
            order[1] = 0;
            order[2] = 1;
          }
          
          if (counter_players == 0)
          {
            order[0] = counter_players;
            order[1] = 1;
            order[2] = 2;
          }
          
          // ------------------------------------------------------------------------
          
          points_and_caller = playGame(deck, deck_dealer,
                                       deck_player_1, deck_player_2, deck_player_3,
                                       &turns, &index_dealer,
                                       &index_player_1, &index_player_2, &index_player_3,
                                       &state, &score_dealer,
                                       &players[0].points_, &players[1].points_, &players[2].points_,
                                       &call, &trump, suits, &bool_fleck, &bool_retour,
                                       players[0].name_, players[1].name_, players[2].name_,
                                       players, order);
          // AFTER GAME
          // TODO: get points, count points (player, opponents), fleck/retour,
          //        Bummerl (>= 24), another Bummerl?
          
          if (bool_fleck == TRUE)
          {
            if (bool_retour == TRUE) /* retour fleck */
            {
              points_and_caller.points_ = points_and_caller.points_ * 4;
            }
            
            else /* fleck only */
            {
              points_and_caller.points_ = points_and_caller.points_ * 2;
            }
          }
          
          if (points_and_caller.winner_ == OPPONENTS_WIN)   // works for all modes
          {
            if (points_and_caller.caller_ == TURN_PLAYER_1)     // we only need to check modes
            {                                                   // Schnapser & Bauernschnapser
              players[1].points_ += points_and_caller.points_;
              players[2].points_ += points_and_caller.points_;
//              printf("test 2 & 3\n");
            }
            
            else if (points_and_caller.caller_ == TURN_PLAYER_2)
            {
              players[0].points_ += points_and_caller.points_;
              players[2].points_ += points_and_caller.points_;
//              printf("test 1 & 3\n");
            }
            
            else if (points_and_caller.caller_ == TURN_PLAYER_3)
            {
              players[0].points_ += points_and_caller.points_;
              players[1].points_ += points_and_caller.points_;
//              printf("test 1 & 2\n");
            }
          }
          
          else
          {
            if (points_and_caller.winner_ == TURN_PLAYER_1)
            {
              players[0].points_ += points_and_caller.points_;
//              printf("test 1\n");
            }
            
            else if (points_and_caller.winner_ == TURN_PLAYER_2)
            {
              players[1].points_ += points_and_caller.points_;
//              printf("test 2\n");
            }
            
            else if (points_and_caller.winner_ == TURN_PLAYER_3)
            {
              players[2].points_ += points_and_caller.points_;
//              printf("test 3\n");
            }
          }
          
          // TODO: check which players get a Bummerl and whether they get two
          //        in case of a Schneider
          // TODO: check who is who
          if (!(counter % 3))
          {
//            printf("%s called trump.\n", players[counter_players].name_);
            bummerl[0] = players[0].points_;
            bummerl[1] = players[1].points_;
            bummerl[2] = players[2].points_;
          }
          
          else if (!((counter MINUS_ONE) % 3))
          {
//            printf("%s called trump.\n", players[counter_players].name_);
            bummerl[0] = players[1].points_;
            bummerl[1] = players[2].points_;
            bummerl[2] = players[0].points_;
          }
          
          else
          {
//            printf("%s called trump.\n", players[counter_players].name_);
            bummerl[0] = players[2].points_;
            bummerl[1] = players[0].points_;
            bummerl[2] = players[1].points_;
          }
          
  //        printf("counter: %d\n", counter);
          counter++;
          counter_players++;
          
          // 0, 1, 2, 0, 1, ..
          if (counter_players == QUANTITY_PLAYERS)
            counter_players = 0;
          
          printf("  %s: %d\n  %s: %d\n  %s: %d\n",
                 players[0].name_, players[0].points_, players[1].name_,
                 players[1].points_, players[2].name_, players[2].points_);
          printf("---------------------------------------------------------------\n");
          
        } while (!(players[0].points_ >= 24) && !(players[1].points_ >= 24)
                 && !(players[2].points_ >= 24));   // end loop Bummerl
        
        printBummerl(players);
        
        // another Bummerl?
        if (players[0].points_ >= 24 || players[1].points_ >= 24
            || players[2].points_ >= 24)
        {
          // who wants to continue
          for (counter_players = 0; counter_players < QUANTITY_PLAYERS; counter_players++)
          {
            // check: human or CPU
            if (players[counter_players].CPU_bool_ == TRUE)
              check_continue[counter_players] = FALSE/*TRUE*/;
            else
              check_continue[counter_players]
                = checkContinue(players[counter_players].name_);
          }
          // all three
          if (check_continue[0] == TRUE
              && check_continue[0] == check_continue[1]
              && check_continue[1] == check_continue[2])
          {
            printf("Let's play another Bummerl!\n");
            go_on = TRUE;

            // with each new Bummerl first to call changes
            swapOrder(players);
          }

          // at this point we could "look for other player(s)" in case
          // not all three players quitting - for now it's "all or none"
          /*else*/ if (check_continue[0] != TRUE)
          {
            printf("%s quit.\n", players[0].name_);
            go_on = FALSE;
          }

          /*else*/ if (check_continue[1] != TRUE)
          {
            printf("%s quit.\n", players[1].name_);
            go_on = FALSE;
          }

          /*else*/ if (check_continue[2] != TRUE)
          {
            printf("%s quit.\n", players[2].name_);
            go_on = FALSE;
          }
          
          go_on = FALSE;
        }
      } while (go_on == TRUE);    // end loop: another Bummerl?
      
    } // end if (SUCCESS)
    
    else
    {
      printErrorMessage(error_code, argv[0]);
    }
  }
  return error_code;
}

//-----------------------------------------------------------------------------
///
/// This function defines the different types of cards and initializes a deck
/// in which all these basic definitions are stored. Note that there are not
/// any images stored in this deck yet.
///
/// @param  dummy_deck    array that stores each type of card
///
/// @return no return value since this is a 'void' function
//
void initializeDummyDeck(Card* dummy_deck)
{
  // define card values
  Card ace      = {"A",  11, ""};
  Card king     = {"K",  4, ""};
  Card queen    = {"Q",  3, ""};
  Card jack     = {"J",  2, ""};
  Card ten      = {"10", 10, ""};

  // store card dummies
  dummy_deck[0] = ace;
  dummy_deck[1] = king;
  dummy_deck[2] = queen;
  dummy_deck[3] = jack;
  dummy_deck[4] = ten;
}

//-----------------------------------------------------------------------------
///
/// This function displays several cards in line.
///
/// @param  deck      array to get images to be displayed from
/// @param  number  number of cards to display
/// @param  player  number of player
/// @param  name      name of player
///
/// @return no return value since this is a 'void' function
//
void printHand(Card* deck, int number, int player, char* name)
{
  // initialize character array where images to be printed are stored
  int counter = 0;
  
  if (player < TURN_DEALER)
    printf("Player %d (%s):     ", player, name);
  else if (player == TURN_DEALER)
    printf("TALON:        ");
  
  for (counter = 0; counter < number; counter++)
  {
    printf("[%s %s] ", deck[counter].sign_, deck[counter].image_);
  }
  printf("\n");
}

//-----------------------------------------------------------------------------
///
/// This function creates a full deck by taking four of each of the "dummy
/// deck's" cards.
///
/// @param  deck          full deck to be played with (52 cards)
/// @param  dummy_deck    deck that holds one card definition of each type
///
/// @return no return value since this is a 'void' function
//
void createFullDeck(Card* deck, Card* dummy_deck, char** suits)
{
//  int suits   = 4;
//  char* suits[SUITS]  = {"hearts", "spades", "clubs", "diamonds"};
  char* signs[SUITS]   = {"♥", "♠", "♣", "♦"};
  int counter         = 0;
  int counter_suits   = 0;
  
  // define card values
  Card ace      = {dummy_deck[0].image_, 11};
  Card king     = {dummy_deck[1].image_, 4};
  Card queen    = {dummy_deck[2].image_, 3};
  Card jack     = {dummy_deck[3].image_, 2};
  Card ten      = {dummy_deck[4].image_, 10};
  
  // store dummies in deck
  while (counter < (CARD_QUANTITY * 4))
  {
    if (counter < SUITS)
    {
//      printf("counter_suits: %d\nsuit: %s\n", counter_suits, suits[counter_suits]);
      deck[counter] = ace;
      deck[counter].suit_ = suits[counter_suits];
//      strcpy(deck[counter].suit_, suits[counter_suits]);
      deck[counter].sign_ = signs[counter_suits];
      counter++;
      counter_suits++;
//      printf("Wert: %d, Farbe: %s\n", deck[counter].value_, deck[counter].suit_);
      if (counter_suits == 4)
        counter_suits = 0;
    }
    else if (counter < SUITS * 2)
    {
      deck[counter] = king;
      deck[counter].suit_ = suits[counter_suits];
      deck[counter].sign_ = signs[counter_suits];
      counter++;
      counter_suits++;
      if (counter_suits == 4)
        counter_suits = 0;
    }
    else if (counter < SUITS * 3)
    {
      deck[counter] = queen;
      deck[counter].suit_ = suits[counter_suits];
      deck[counter].sign_ = signs[counter_suits];
      counter++;
      counter_suits++;
      if (counter_suits == 4)
        counter_suits = 0;
    }
    else if (counter < SUITS * 4)
    {
      deck[counter] = jack;
      deck[counter].suit_ = suits[counter_suits];
      deck[counter].sign_ = signs[counter_suits];
      counter++;
      counter_suits++;
      if (counter_suits == 4)
        counter_suits = 0;
    }
    else if (counter < SUITS * 5)
    {
      deck[counter] = ten;
      deck[counter].suit_ = suits[counter_suits];
      deck[counter].sign_ = signs[counter_suits];
      counter++;
      counter_suits++;
      if (counter_suits == 4)
        counter_suits = 0;
    }
    
    deck[counter].is_bock_ = FALSE;
  }
  //check deck
//  printHand(deck, 20, 1);
}

//-----------------------------------------------------------------------------
///
/// Fisher Yates shuffle algorithm as shown as pseudo code in the assignment's
/// description.
///
/// This particular function was implemented in the exact way the pseudo code
/// is represented in the assignment's description to make sure it works on
/// the test system just the way it is supposed to. I would like to mention
/// this hereby just in case of being accused of plagiarism. I did not copy
/// any code. I just followed the given pseudo code and made it work in C.
///
/// @param  deck            deck whose cards shall be shuffled
/// @param  size            number of cards in deck
/// @param  random_seed     seed for pseudo randomisation
///
/// @return no return value since this is a 'void' function
//
void FisherYates(Card* deck, int size, int random_seed)
{
  int index       = 0;
  int swap_index  = 0;
  Card buffer     = {NULL, 0};
  
  // SET_RANDOM_SEED(random_seed)
  srand(random_seed);
  
  // FOR index = (size - 1) to 0 step -1:
  for (index = (size - 1); index > 0; index--)
  {
    // swap_index = RANDOM_IN_RANGE(0,index)
    swap_index = rand() % (index + 1);
    
    // SWAP(deck[index],deck[swap_index])
    buffer    = deck[index];

    deck[index] = deck[swap_index];
    deck[swap_index] = buffer;
  }
}

//-----------------------------------------------------------------------------
///
/// This function represents taking the card on top of a deck and dealing it
/// to either a player or the dealer
///
/// @param  turn      number of card being dealt
/// @param  index     index of target's deck
/// @param  source    source deck to pick the top card from
/// @param  target    target deck to pass the picked card to
///
/// @return no return value since this is a 'void' function
//
void dealCards(int turn, int* index, Card* source, Card* target)
{
  target[*index] = source[turn];
}

//-----------------------------------------------------------------------------
///
/// This function handles the start of the game prior to a player's or
/// dealer's move. It passes the first two cards to the player and the
/// following two are passed to the dealer. Then it displays the dealer's
/// first card and both of the player's cards.
///
/// TODO: dismiss deck_player1/2/3 .. use hand - just added
///
//
void startGame(Card* deck, Card* deck_dealer,
               Card* deck_player_1, Card* deck_player_2, Card* deck_player_3,
               int* turns,
               int* index_dealer,
               int* index_player_1, int* index_player_2, int* index_player_3,
               int* trump, Player* players, int* order, char** suits,
               Card** hands)
{
  int counter = 0;
  
  // 3 CARDS FOR PLAYER 1
  for (counter = 0; counter < 3; counter++)
  {
    dealCards(*turns, index_player_1, deck, /*deck_player_1*/hands[order[0]]);
    (*turns)++;
    (*index_player_1)++;
  }
  
  printHand(/*deck_player_1*/hands[order[0]], 3, /*TURN_PLAYER_1*/order[0] ADD_ONE,
            players[order[0]].name_);         // ACHTUNG HARD CODED VALUE
  
  // 3 CARDS FOR PLAYER 2
  for (counter = 0; counter < 3; counter++)
  {
    dealCards(*turns, index_player_2, deck, /*deck_player_2*/hands[order[1]]);
    (*turns)++;
    (*index_player_2)++;
  }
  
  printHand(/*deck_player_2*/hands[order[1]], 3, /*TURN_PLAYER_2*/ order[1] ADD_ONE,
            players[order[1]].name_);
  
  // 3 CARDS FOR PLAYER 3
  for (counter = 0; counter < 3; counter++)
  {
    dealCards(*turns, index_player_3, deck, /*deck_player_3*/hands[order[2]]);
    (*turns)++;
    (*index_player_3)++;
  }
  
  printHand(/*deck_player_3*/hands[order[2]], 3, /*TURN_PLAYER_3*/ order[2] ADD_ONE,
            players[order[2]].name_);    // ACHTUNG HARD CODED VALUE
  
  // 2 CARDS (TALON)
  for (counter = 0; counter < TALON; counter++)
  {
    dealCards(*turns, index_dealer, deck, deck_dealer);
    (*turns)++;
    (*index_dealer)++;
  }
  
//  printHand(deck_dealer, 2, TURN_DEALER);
  
  printf("---------------------------------------------------------------\n");
  
  // PLAYER 1 CALL TRUMP
  printf("Player %d (%s), call trump!\n-----------------------\n",
         order[0] ADD_ONE, players[order[0]].name_);
  // check: human or CPU
  if (players[order[0]].CPU_bool_ == FALSE)
    *trump = callTrump(deck[ANE_AUF]);
  else
  {
    *trump = (rand() % (4 - 1 + 1)) + 1;
//    printf("trump: %d\n", *trump);
    printf("%s is trump!\n", decodeSuit(*trump));
    printf("---------------------------------------------------------------\n");
  }
  
  /* Naja, theoretisch könnten direkt jedem 6 Karten hingeteilt werden und nur
   3 gezeigt werden bevor Trumpf angesagt wurde ... */
  
  // 3 CARDS FOR PLAYER 1
  for (counter = 0; counter < 3; counter++)
  {
    dealCards(*turns, index_player_1, deck, /*deck_player_1*/hands[order[0]]);
    (*turns)++;
    (*index_player_1)++;
  }
  
  // 3 CARDS FOR PLAYER 2
  for (counter = 0; counter < 3; counter++)
  {
    dealCards(*turns, index_player_2, deck, /*deck_player_2*/hands[order[1]]);
    (*turns)++;
    (*index_player_2)++;
  }
  
  // 3 CARDS FOR PLAYER 3
  for (counter = 0; counter < 3; counter++)
  {
    dealCards(*turns, index_player_3, deck, /*deck_player_3*/hands[order[2]]);
    (*turns)++;
    (*index_player_3)++;
  }
  
  // set trump cards
  setTrump(/*deck_player_1*/hands[order[0]], *trump);
  setTrump(/*deck_player_2*/hands[order[1]], *trump);
  setTrump(/*deck_player_3*/hands[order[2]], *trump);
  setTrump(deck_dealer, *trump);
  
  //CHEAT to debug
//  setOrder(deck_player_1, 40, *trump);
//  setOrder(deck_player_2, 41, *trump);
//  setOrder(deck_player_3, 42, *trump);
//  setOrder(deck_dealer,   43, *trump);
  
  sortHands(hands, suits);
  printHand(/*deck_player_1*/hands[order[0]], HAND, /*TURN_PLAYER_1*/ order[0] ADD_ONE,
            players[order[0]].name_);
  printHand(/*deck_player_2*/hands[order[1]], HAND, /*TURN_PLAYER_2*/ order[1] ADD_ONE,
            players[order[1]].name_);
  printHand(/*deck_player_3*/hands[order[2]], HAND, /*TURN_PLAYER_3*/ order[2] ADD_ONE,
            players[order[2]].name_);
}

//-----------------------------------------------------------------------------
///
/// This function receives error values and prints out a corresponding error
/// message.
///
/// @param  error_code    value to decide which error message to print
/// @param  argument      argv[0]: the path this game is called from
///
/// @return no return value since this is a 'void' function
//
void printErrorMessage(int error_code, char* argument)
{
  switch (error_code)
  {
    case ERROR_CODE_WRONG_USAGE:
      printf("usage: %s <input_folder>\n", argument);
      break;
    
    case ERROR_CODE_OUT_OF_MEMORY:
      printf("[ERR] Out of memory.\n");
      break;
      
    case ERROR_CODE_INVALID_FILES:
      printf("[ERR] Invalid File(s).\n");
      break;
      
    default:
      break;
  } // switch end
}

//-----------------------------------------------------------------------------
///
/// This function checks if there is a command line argument that will be used
/// as seed for the pseudo randomized Fisher Yates Shuffle Algorithm. If there
/// is not one, a fixed seed will be used. Here it is 73.
///
/// The function "strtol()" was used in tutorium and I looked it up on the
/// online reference "cplusplus.com" as well in order to understand how it
/// functions and how to use it myself. I would like to mention this hereby
/// just in case of being accused of plagiarism. I did not copy any code.
///
/// @param  argument    argv[2]
///
/// @return seed
//
int getSeed(char* argument)
{
  int seed = 0;
  
  // seed from command line
  if (argument != NULL)
  {
    seed = (int)strtol(argument, NULL, 10);
  }
  
  // fixed seed
  else
  {
//    seed = 73;
    seed = (unsigned int)clock();
  }
  
  return seed;
}

//-----------------------------------------------------------------------------
///
/// This function handles the whole playing process by getting player's input,
/// displaying card images and scores and checking who wins or loses. It does
/// this by calling several functions in the right order and checking each
/// possibile outcome that comes with each turn.
///
/// @param  deck            the whole deck to be played with
/// @param  deck_dealer     the dealer's cards
/// @param  deck_player     the player's cards
/// @param  turns           number of current turn
/// @param  index_dealer    index of dealer's cards
/// @param  index_player    index of player's cards
/// @param  state           represents whose turn it is or
///                         if one's turn is over
/// @param  score_dealer    sum of dealer's cards' value
/// @param  score_player    sum of player's cards' value
/// @param  call            representing hit or stand
///
/// @return no return value since this is a 'void' function
//
Points playGame(Card* deck, Card* deck_dealer,
                Card* deck_player_1, Card* deck_player_2, Card* deck_player_3,
                int* turns, int* index_dealer,
                int* index_player_1, int* index_player_2, int* index_player_3,
                int* state, int* score_dealer,
                int* score_player_1, int* score_player_2, int* score_player_3,
                int* call, int* trump, char** suits, int* bool_fleck,
                int* bool_retour, char* player_1, char* player_2, char* player_3,
                Player* players, int* order)
{
  int mode          = GAME;
  int mode_buffer_1 = GAME;
  int mode_buffer_2 = GAME;
  int mode_buffer_3 = GAME;
  int start         = TURN_PLAYER_1;
  int buffer_start  = start;
//  int bool_fleck    = 0;
//  int bool_trump    = TRUE;
  int talon         = GO_ON;
  int counter_talon = 0;
  Card* hands[3]    = {};
//  int suit          = 0;
  Points points_and_caller = {0, 0, 0};
  
  // Hände der Einfachheit zusammenfassen
  hands[order[0]] = deck_player_1;
  hands[order[1]] = deck_player_2;
  hands[order[2]] = deck_player_3;
  
  // deal three cards to each player
  // two cards as talon
  // player 1 calls trump
  // deal three cards to each player
  startGame(deck, deck_dealer, deck_player_1, deck_player_2, deck_player_3,
            turns, index_dealer,
            index_player_1, index_player_2, index_player_3, trump, players,
            order, suits, hands);

  // player 1 to call mode
  *state = TURN_PLAYER_1;
//  *state = order[0] ADD_ONE;
  
  // check: human or CPU
  if (players[order[0]].CPU_bool_ == FALSE)
    mode = callMode(mode, *state, &start, /*player_1*/ players[order[0]].name_);
  else
    mode = 1;
  
  if (mode < 5)
  {
    mode_buffer_1 = mode;
    
    // player 2 raise
    *state = TURN_PLAYER_2;
//    *state = order[1] ADD_ONE;
    // check: human or CPU
    if (players[order[1]].CPU_bool_ == FALSE)
      mode = callMode(mode, *state, &start, players[order[1]].name_);
    else
    {
      printf("Player %d (%s(CPU)) does not raise.\n",
             order[*state MINUS_ONE] ADD_ONE, players[order[1]].name_);
    }
    if (start == TURN_PLAYER_2)
      mode_buffer_2 = mode;

    // player 3 raise
    *state = TURN_PLAYER_3;
//    *state = order[2] ADD_ONE;
    // check: human or CPU
    if (players[order[2]].CPU_bool_ == FALSE)
      mode = callMode(mode, *state, &start, players[order[2]].name_);
    else
    {
      printf("Player %d (%s(CPU)) does not raise.\n",
             order[*state MINUS_ONE] ADD_ONE, players[order[2]].name_);
    }
    if (start == TURN_PLAYER_3)
      mode_buffer_3 = mode;
    
    // determine the beginner
    start = determineBeginner(mode_buffer_1, mode_buffer_2, mode_buffer_3,
                              &mode, players, order);
    
    // player 1 begins anyway
    if (start != /*TURN_PLAYER_1*/ order[0] ADD_ONE && mode != SCHNAPSER && mode /* < */ != BAUERNSCHNAPSER)
      // explanation: Kontraschnapser = 12: Player 1 would have to trump that
      //              with a Bauernschnapser and that "sounds" unlikely ..
      //               .. should have called it beforehand
    {
//      if (start != TURN_PLAYER_1)
      start = habSelbst(start, &mode, players[order[0]].name_);
      
      if (start == TURN_PLAYER_1)
      {
        printf("---------------------------------------------------------------\n");
        printf("Player %d (%s) trumps that and starts!\n", start,
               players[order[0]].name_);
        printf("---------------------------------------------------------------\n");
      }
    }
  }
  
//  printHand(deck_dealer, 2, TURN_DEALER);                           // ACHTUNG HARD CODED VALUES
  printf("---------------------------------------------------------------\n");
  
  // check variables against talon "abuse"
  int done_1      = FALSE;
  int done_2      = FALSE;
  
  // here might be a good time/place to sort hands
  sortHands(hands, suits);
  
  // check: human or CPU
  if (players[order[start MINUS_ONE]].CPU_bool_ == FALSE)
  {
    // Spieler darf aus dem Talon kaufen
    printHand(hands[start MINUS_ONE], 6, start, players[order[start MINUS_ONE]].name_);
    
    while (talon != BREAK && counter_talon < 2)  // maximum two exchanges
  //  while (talon != BREAK)  // so you may exchange and decide until you end with 0
    {
      talon = buy(start, hands, deck_dealer, &done_1, &done_2, players, order);
      
      // sort again hand after each exchange
      // so far I do not care about the fact that all three hands will be sorted again
      sortHands(hands, suits);
      printHand(hands[start MINUS_ONE], 6, start, players[order[start MINUS_ONE]].name_);
      
      counter_talon++;
    }
    
    mode = callRaise(mode, /**state,*/ start, players);
  }
  
  // Fleck and or Reh? - später interessant für Punkte
  // könnte man sauberer oder transparenter lösen
  switch (start)
  {
    case TURN_PLAYER_1:
      if (players[1].CPU_bool_ == TRUE || players[2].CPU_bool_ == TRUE)
        break;
      else
      {
        if (fleck(TURN_PLAYER_2) && fleck(TURN_PLAYER_3))
          {
            *bool_fleck = TRUE;
            printf("Player %d (%s) is g'fleckt!    (points x2)\n", start,
                   players[order[start MINUS_ONE]].name_);
            
            *bool_retour = fleckBack(TURN_PLAYER_1);
          }
        else
        {
          *bool_fleck   = FALSE;
          *bool_retour  = FALSE;
        }
      }
      break;
      
    case TURN_PLAYER_2:
      if (players[0].CPU_bool_ == TRUE || players[2].CPU_bool_ == TRUE)
        break;
      else
      {
        if (fleck(TURN_PLAYER_1) && fleck(TURN_PLAYER_3))
          {
            *bool_fleck = TRUE;
            printf("Player %d (%s) is g'fleckt!    (points x2)\n", start,
                   players[order[start MINUS_ONE]].name_);
            
            *bool_retour = fleckBack(TURN_PLAYER_1);
          }
        else
        {
          *bool_fleck   = FALSE;
          *bool_retour  = FALSE;
        }
      }
      break;
      
    case TURN_PLAYER_3:
      if (players[0].CPU_bool_ == TRUE || players[1].CPU_bool_ == TRUE)
        break;
      else
      {
        if (fleck(TURN_PLAYER_1) && fleck(TURN_PLAYER_2))
          {
            *bool_fleck = TRUE;
            printf("Player %d (%s) is g'fleckt!    (points x2)\n", start,
                   players[order[start MINUS_ONE]].name_);
            
            *bool_retour = fleckBack(TURN_PLAYER_1);
          }
        else
        {
          *bool_fleck   = FALSE;
          *bool_retour  = FALSE;
        }
      }
      break;
      
    default:
      break;
  }
  
  // then game - according to prior determined mode
  switch (mode)
  {
    case GAME:
      // 1 - 3 points
//      points_and_caller = modeGame(hands, start, decodeSuit(*trump), players,
//                                   order);
      points_and_caller = switchRufer(hands, start, decodeSuit(*trump),
                                      players, order);
      return points_and_caller;
      break;
      
    case SCHNAPSER:
      // 6 points || CONTRA: 12 points
      points_and_caller = modeSchnapser(hands, start, decodeSuit(*trump), players);
      return points_and_caller;
      break;
      
    case LAND:
      // 9 points
      points_and_caller = modeLand(/*FALSE,*/ hands, start, players);
      return points_and_caller;
      break;
      
    case BAUERNSCHNAPSER:
      // 12 points || CONTRA: 24 points
      points_and_caller = modeBauernschnapser(/*TRUE,*/ hands, start, *trump, players);
      return points_and_caller;
      break;
      
    case JODLER:
      // 18 points
      printf("With which suit would you like to play the JODLER, Player %d?\n",
             start);
      points_and_caller = modeJodler(hands, start, decodeSuit(callSuit(start)), players);
      return points_and_caller;
      break;
      
    case HERREN_JODLER:
      // 24 points
      /*points_and_caller =*/ modeHerrenJodler(hands, start, decodeSuit(*trump), players);
      return points_and_caller;
      break;
      
    default:  // impossible case though
      points_and_caller.points_ = -1;
      points_and_caller.winner_ = -1;
      points_and_caller.caller_ = -1;
      return points_and_caller;
      break;
  }
}

int callTrump(Card auf)
{
  int trump = 0;
//  printf("Player 1, call trump!\n-----------------------\n");
  printf("1       CLUBS    (♣)\n");
  printf("2       SPADES   (♠)\n");
  printf("3       HEARTS   (♥)\n");
  printf("4       DIAMONDS (♦)\n");
  printf("---------------------------------------------------------------\n");
//  printf("0       \"Ane auf!\"\n");
  printf("0       \"Hit me!\"\n");
  printf("---------------------------------------------------------------\n");
  
  do
  {
    system ("/bin/stty raw");
    trump = getchar();
    printf("\r");
    if (trump != '1' && trump != '2' && trump != '3' && trump != '4'
        && trump != '0')
      printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
  } while (trump != '1' && trump != '2' && trump != '3' && trump != '4'
           && trump != '0');
  system ("/bin/stty cooked");
  
  switch (trump)
  {
    case '0':
      printf("AUF: [%s %s]\n", auf.sign_, auf.image_);
      
      if (!strcmp(auf.suit_, "clubs"))
      {
        trump = '1';
      }
      
      else if (!strcmp(auf.suit_, "spades"))
      {
        trump = '2';
      }
      
      else if (!strcmp(auf.suit_, "hearts"))
      {
        trump = '3';
      }
      
      else /* if (!strcmp(auf.suit_, "diamonds")) */
      {
        trump = '4';
      }
      
      break;
      
    case '1':
      printf("CLUBS (♣) is trump!\n");
      break;
      
    case '2':
      printf("SPADES (♠) is trump!\n");
      break;
      
    case '3':
      printf("HEARTS (♥) is trump!\n");
      break;
      
    case '4':
      printf("DIAMONDS (♦) is trump!\n");
      break;
      
    default:
      break;
  }
  
  printf("---------------------------------------------------------------\n");
//  printf("%d\n", (int)trump-'0');
  return (int) trump - '0';
}

int callSuit(int start)
{
  int suit = 0;
  printf("Player %d, call trump!\n-----------------------\n", start);
  printf("1       CLUBS    (♣)\n");
  printf("2       SPADES   (♠)\n");
  printf("3       HEARTS   (♥)\n");
  printf("4       DIAMONDS (♦)\n");
  printf("---------------------------------------------------------------\n");
  
  do
  {
    system ("/bin/stty raw");
    suit = getchar();
    printf("\r");
    if (suit != '1' && suit != '2' && suit != '3' && suit != '4')
      printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
  } while (suit != '1' && suit != '2' && suit != '3' && suit != '4');
  system ("/bin/stty cooked");
  
  printf("---------------------------------------------------------------\n");
//  printf("%d\n", (int)suit-'0');
  return (int) suit - '0';
}

int callMode(int mode, int state, int* start, char* player)
{
  int buffer_mode = mode;
  
  switch (state)
  {
    case TURN_PLAYER_1:
      printf("Player 1 (%s), call mode!\n-----------------------\n", player);
      
      printf("1       GAME\n");
      printf("2       SCHNAPSER\n");
      printf("3       LAND\n");
      printf("4       BAUERNSCHNAPSER\n");
      printf("5       JODLER\n");
      printf("6       HERRENJODLER\n");
      printf("---------------------------------------------------------------\n");
      
      do
      {
        system ("/bin/stty raw");
        mode = getchar();
        mode = mode CHAR_TO_INT;
        printf("\r");
        if (mode != GAME && mode != SCHNAPSER && mode != LAND
            && mode != BAUERNSCHNAPSER && mode != JODLER
            && mode != HERREN_JODLER)
          printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
      } while (mode != GAME && mode != SCHNAPSER && mode != LAND
               && mode != BAUERNSCHNAPSER && mode != JODLER
               && mode != HERREN_JODLER);
      system ("/bin/stty cooked");
      
      switchMode(&mode);
      
      break;
      
    
    case TURN_PLAYER_2:
      if (mode != HERREN_JODLER
          /* && !(mode == BAUERNSCHNAPSER && start != TURN_PLAYER_1) */)
      {
        printf("Player 2 (%s), would you want to raise the mode?", player);
        printf("\n-----------------------\n");
        *start = raiseMode(&mode, TURN_PLAYER_2);
        
        if (mode >= buffer_mode && mode == BAUERNSCHNAPSER
            && !(*start == TURN_PLAYER_1))
          *start = TURN_PLAYER_2;
      }
      break;
      
    case TURN_PLAYER_3:
      if (mode != HERREN_JODLER
          && !(mode == BAUERNSCHNAPSER && *start == TURN_PLAYER_2))
      {
        printf("Player 3 (%s), would you want to raise the mode?", player);
        printf("\n-----------------------\n");
        *start = raiseMode(&mode, TURN_PLAYER_3);
        
        if (/* mode >= buffer_mode && */
            mode == BAUERNSCHNAPSER && *start == TURN_PLAYER_3)
          *start = TURN_PLAYER_3;
      }
      break;
      
    default:
      break;
  }
  return mode;
}

int callRaise(int mode, /*int state,*/ int start, Player* players)
{
  if (mode > GAME && mode < HERREN_JODLER)
  {
    switch (start)
    {
      case TURN_PLAYER_1:
        printf("Player 1 (%s), would you want to raise the mode?",
               players[0].name_);
        printf("\n-----------------------\n");
        raiseMode(&mode, start);
        break;
        
      case TURN_PLAYER_2:
        printf("Player 2 (%s), would you want to raise the mode?",
               players[1].name_);
        printf("\n-----------------------\n");
        raiseMode(&mode, start);
        break;
        
      case TURN_PLAYER_3:
        printf("Player 3 (%s), would you want to raise the mode?",
               players[2].name_);
        printf("\n-----------------------\n");
        raiseMode(&mode, start);
        break;
        
      default:
        break;
    }
  }
  return mode;
}

void switchMode(int* mode)
{
//  if (*mode == 3)
//    printf("Mit was sagst'n du a Land an!?\n");
  
  switch (*mode)
  {
    case 1:
      printf("Mode: GAME(default)!\n");
      *mode = GAME;
      break;
      
    case 2:
      printf("Mode: SCHNAPSER!\n");
      *mode = SCHNAPSER;
      break;
      
    case 3:
      printf("Mode: LAND!\n");
      *mode = LAND;
      break;
      
    case 4:
      printf("Mode: BAUERNSCHNAPSER!\n");
      *mode = BAUERNSCHNAPSER;
      break;
      
    case 5:
      printf("Mode: JODLER!\n");
      *mode = JODLER;
      break;
      
    case 6:
      printf("Mode: HERRENJODLER!\n");
      *mode = HERREN_JODLER;
      break;
      
//    case '0':
//      break;
      
    default:
      break;
  }
}

int fleck(int player)
{
  int buffer = 0;
  printf("---------------------------------------------------------------\n");
  printf("Player %d, would you like to FLECK?\n", player);
  printf("y       YES\n");
  printf("n       NO\n");
  printf("---------------------------------------------------------------\n");
  
  do
  {
    system ("/bin/stty raw");
    buffer = getchar();
    printf("\r");
    if (buffer != 'y' && buffer != 'n')
      printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
  } while (buffer != 'y' && buffer != 'n');
  system ("/bin/stty cooked");
  
  if (buffer == 'y')
  {
    return TRUE;
  }
  
  else
    return FALSE;
}

int fleckBack(int player)
{
  int buffer = 0;
  printf("---------------------------------------------------------------\n");
  printf("Player %d, would like to FLECK back?\n", player);
  printf("y       YES\n");
  printf("n       NO\n");
  printf("---------------------------------------------------------------\n");
  
  do
  {
    system ("/bin/stty raw");
    buffer = getchar();
    printf("\r");
    if (buffer != 'y' && buffer != 'n')
      printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
  } while (buffer != 'y' && buffer != 'n');
  system ("/bin/stty cooked");
  
  if (buffer == 'y')
  {
    printf("Z'ruckg'fleckt!         (points x4)\n");
    return TRUE;
  }
  
  else
  {
    return FALSE;
  }
}

int raiseMode(int* mode, int start)
{
//  *mode = *mode CHAR_TO_INT;
  int mode_buffer = *mode;
  
  if (start == TURN_PLAYER_1)
  {
    switch (*mode)
    {
      case GAME:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("2       SCHNAPSER\n");
        printf("3       LAND\n");
        printf("4       BAUERNSCHNAPSER\n");
        printf("5       JODLER\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != SCHNAPSER && *mode != LAND
              && *mode != BAUERNSCHNAPSER && *mode != JODLER
              && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != SCHNAPSER && *mode != LAND
                 && *mode != BAUERNSCHNAPSER && *mode != JODLER
                 && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
          *mode = mode_buffer;
        
        switchMode(mode);
        break;
        
      case SCHNAPSER:
//        if (start != TURN_PLAYER_1)                           // kann I mir wahrscheinlich sparen
//        {
//          printf("0       NO\n");
//          printf("---------------------------------------------------------------\n");
//  //        printf("3       LAND\n");
//          printf("4       BAUERNSCHNAPSER\n");
//          printf("5       JODLER\n");
//          printf("6       HERRENJODLER\n");
//          printf("---------------------------------------------------------------\n");
//
//          do
//          {
//            system ("/bin/stty raw");
//            *mode = getchar();
//            *mode = *mode CHAR_TO_INT;
//            printf("\r");
//            if (*mode != WEITER && *mode != BAUERNSCHNAPSER
//                && *mode != JODLER && *mode != HERREN_JODLER)
//              printf("Gibt's nicht!\n");
//          } while (*mode != WEITER && *mode != BAUERNSCHNAPSER
//                   && *mode != JODLER && *mode != HERREN_JODLER);
//          system ("/bin/stty cooked");
//
//          if (*mode == WEITER)
//            *mode = mode_buffer;
//
//          switchMode(mode);
//          break;
//        }
//
//        else
//        {
          printf("0       NO\n");
          printf("---------------------------------------------------------------\n");
          printf("3       LAND\n");
          printf("4       BAUERNSCHNAPSER\n");
          printf("5       JODLER\n");
          printf("6       HERRENJODLER\n");
          printf("---------------------------------------------------------------\n");
          
          do
          {
            system ("/bin/stty raw");
            *mode = getchar();
            *mode = *mode CHAR_TO_INT;
            printf("\r");
            if (*mode != WEITER && *mode != LAND && *mode != BAUERNSCHNAPSER
                && *mode != JODLER && *mode != HERREN_JODLER)
              printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
          } while (*mode != WEITER && *mode != LAND && *mode != BAUERNSCHNAPSER
                   && *mode != JODLER && *mode != HERREN_JODLER);
          system ("/bin/stty cooked");
          
          if (*mode == WEITER)
            *mode = mode_buffer;
          
          switchMode(mode);
          break;
//        }
        
      case LAND:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("4       BAUERNSCHNAPSER\n");
        printf("5       JODLER\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != BAUERNSCHNAPSER && *mode != JODLER
              && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != BAUERNSCHNAPSER && *mode != JODLER
                 && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
          *mode = mode_buffer;
        
        switchMode(mode);
        break;
        
      case BAUERNSCHNAPSER:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("5       JODLER\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != JODLER && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != JODLER && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
          *mode = mode_buffer;
        
        switchMode(mode);
        break;
        
      case JODLER:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
          *mode = mode_buffer;
        
        switchMode(mode);
        break;
        
      case HERREN_JODLER:
        switchMode(mode);
        break;
        
      default:
        break;
    }
    
    return start;
  }
  
  else if (start == TURN_PLAYER_2)
  {
    switch (*mode)
    {
      case GAME:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("2       KONTRA-SCHNAPSER\n");
        printf("3       LAND\n");
        printf("4       KONTRA-BAUERNSCHNAPSER\n");
        printf("5       JODLER\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != SCHNAPSER && *mode != LAND
              && *mode != BAUERNSCHNAPSER && *mode != JODLER
              && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != SCHNAPSER && *mode != LAND
                 && *mode != BAUERNSCHNAPSER && *mode != JODLER
                 && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
        {
          *mode = mode_buffer;
          start = TURN_PLAYER_1;
        }
        
        switchMode(mode);
        break;
        
      case SCHNAPSER:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("2       KONTRA-SCHNAPSER\n");
        printf("3       LAND\n");
        printf("4       KONTRA-BAUERNSCHNAPSER\n");
        printf("5       JODLER\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != SCHNAPSER
              && *mode != LAND && *mode != BAUERNSCHNAPSER
              && *mode != JODLER && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != SCHNAPSER
                 && *mode != LAND && *mode != BAUERNSCHNAPSER
                 && *mode != JODLER && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
        {
          *mode = mode_buffer;
          start = TURN_PLAYER_1;
        }
        
        switchMode(mode);
        break;
        
      case LAND:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("2       KONTRA-SCHNAPSER\n");
        printf("4       KONTRA-BAUERNSCHNAPSER\n");
        printf("5       JODLER\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != SCHNAPSER
              && *mode != BAUERNSCHNAPSER && *mode != JODLER
              && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != SCHNAPSER
                 && *mode != BAUERNSCHNAPSER && *mode != JODLER
                 && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
        {
          *mode = mode_buffer;
          start = TURN_PLAYER_1;
        }
        
        switchMode(mode);
        break;
        
      case BAUERNSCHNAPSER:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("4       KONTRA-BAUERNSCHNAPSER\n");
        printf("5       JODLER\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != BAUERNSCHNAPSER && *mode != JODLER
              && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != BAUERNSCHNAPSER && *mode != JODLER
                 && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
        {
          *mode = mode_buffer;
          start = TURN_PLAYER_1;
        }
        
        switchMode(mode);
        break;
        
      case JODLER:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
        {
          *mode = mode_buffer;
          start = TURN_PLAYER_1;
        }
        
        switchMode(mode);
        break;
        
      case HERREN_JODLER:
        switchMode(mode);
        break;
        
      default:
        break;
    }
    
    return start;
  }
  
  else /* if (start == TURN_PLAYER_3) */
  {
    switch (*mode)
    {
      case GAME:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("2       KONTRA-SCHNAPSER\n");
        printf("3       LAND\n");
        printf("4       KONTRA-BAUERNSCHNAPSER\n");
        printf("5       JODLER\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != SCHNAPSER && *mode != LAND
              && *mode != BAUERNSCHNAPSER && *mode != JODLER
              && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != SCHNAPSER && *mode != LAND
                 && *mode != BAUERNSCHNAPSER && *mode != JODLER
                 && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
        {
          if (start == TURN_PLAYER_1)
          {
            *mode = mode_buffer;
            start = TURN_PLAYER_1;
          }
          else /* if (start == TURN_PLAYER_2) */
          {
            *mode = mode_buffer;
            start = TURN_PLAYER_2;
          }
        }
        
        switchMode(mode);
        break;
        
      case SCHNAPSER:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        //printf("3       LAND\n");
        printf("4       KONTRA-BAUERNSCHNAPSER\n");
        printf("5       JODLER\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER /*&& *mode != LAND*/ && *mode != BAUERNSCHNAPSER
              && *mode != JODLER && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER /*&& *mode != LAND*/ && *mode != BAUERNSCHNAPSER
                 && *mode != JODLER && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
        {
          if (start == TURN_PLAYER_1)
          {
            *mode = mode_buffer;
            start = TURN_PLAYER_1;
          }
          else /* if (start == TURN_PLAYER_2) */
          {
            *mode = mode_buffer;
            start = TURN_PLAYER_2;
          }
        }
        
        switchMode(mode);
        break;
        
      case LAND:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("2       KONTRA-SCHNAPSER\n");
        printf("4       KONTRA-BAUERNSCHNAPSER\n");
        printf("5       JODLER\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != SCHNAPSER
              && *mode != BAUERNSCHNAPSER && *mode != JODLER
              && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != SCHNAPSER
                 && *mode != BAUERNSCHNAPSER && *mode != JODLER
                 && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
        {
          if (start == TURN_PLAYER_1)
          {
            *mode = mode_buffer;
            start = TURN_PLAYER_1;
          }
          else /* if (start == TURN_PLAYER_2) */
          {
            *mode = mode_buffer;
            start = TURN_PLAYER_2;
          }
        }
        
        switchMode(mode);
        break;
        
      case BAUERNSCHNAPSER:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("4       KONTRA-BAUERNSCHNAPSER\n");
        printf("5       JODLER\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != BAUERNSCHNAPSER && *mode != JODLER
              && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != BAUERNSCHNAPSER && *mode != JODLER
                 && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
        {
          if (start == TURN_PLAYER_1)
          {
            *mode = mode_buffer;
            start = TURN_PLAYER_1;
          }
          else /* if (start == TURN_PLAYER_2) */
          {
            *mode = mode_buffer;
            start = TURN_PLAYER_2;
          }
        }
        
        switchMode(mode);
        break;
        
      case JODLER:
        printf("0       NO\n");
        printf("---------------------------------------------------------------\n");
        printf("6       HERRENJODLER\n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          *mode = getchar();
          *mode = *mode CHAR_TO_INT;
          printf("\r");
          if (*mode != WEITER && *mode != HERREN_JODLER)
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (*mode != WEITER && *mode != HERREN_JODLER);
        system ("/bin/stty cooked");
        
        if (*mode == WEITER)
        {
          if (start == TURN_PLAYER_1)
          {
            *mode = mode_buffer;
            start = TURN_PLAYER_1;
          }
          else /* if (start == TURN_PLAYER_2) */
          {
            *mode = mode_buffer;
            start = TURN_PLAYER_2;
          }
        }
        
        switchMode(mode);
        break;
        
      case HERREN_JODLER:
        switchMode(mode);
        break;
        
      default:
        break;
    }
  }
  
  return start;
}

int determineBeginner(int value_1, int value_2, int value_3, int* mode,
                      Player* players, int* order)
{
//  if ((value_2 == value_1 && value_1 < value_3)
//      || (value_1 < value_2 && value_2 < value_3))
  if (value_3 >= value_1 && value_3 >= value_2 && value_3 != GAME)
  {
    printf("Player %d (%s) starts!\n", order[2] ADD_ONE, players[order[2]].name_);
    *mode = value_3;
    return /*TURN_PLAYER_3*/ order[2] ADD_ONE;
  }
  
//  else if (value_1 == value_2 && value_1 == value_3 && value_1 != BAUERNSCHNAPSER)
  else if (value_1 > value_2 && value_1 > value_3)
  {
    printf("Player %d (%s) starts!\n", order[0] ADD_ONE, players[order[0]].name_);
    *mode = value_1;
    return /*TURN_PLAYER_1*/ order[0] ADD_ONE;
  }
  
//  else if (value_1 == value_2 && value_1 == value_3 && value_1 == BAUERNSCHNAPSER)
//  else if ()
//  {
//    printf("Player 3 (%s) starts!\n", players[order[2]].name_);
//    *mode = value_1;
//    return TURN_PLAYER_3;
//  }
  
  else if (value_1 == value_2 && value_1 == value_3 && value_1 == GAME)
  {
    printf("Player %d (%s) starts!\n", order[0] ADD_ONE, players[order[0]].name_);
    *mode = value_1;
    return /*TURN_PLAYER_1*/ order[0] ADD_ONE;
  }
  
  else
  {
    printf("Player %d (%s) starts!\n", order[1] ADD_ONE, players[order[1]].name_);
    *mode = value_2;
    return /*TURN_PLAYER_2*/ order[1] ADD_ONE;
  }
}

int habSelbst(int start, int* mode, char* player)
{
  int buffer = 0;
  printf("---------------------------------------------------------------\n");
  printf("Player 1 (%s), would you prefer playing mode %d?\n", player, *mode);
  printf("y       YES\n");
  printf("n       NO\n");
  printf("---------------------------------------------------------------\n");
  
  do
  {
    system ("/bin/stty raw");
    buffer = getchar();
//    *mode = *mode CHAR_TO_INT;
    printf("\r");
    if (buffer != 'y' && buffer != 'n')
      printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
  } while (buffer != 'y' && buffer != 'n');
  system ("/bin/stty cooked");
  
  if (buffer == 'y')
  {
//    printf("Ah! Des hab I selber!\n");
    printf("Player 1 (%s) takes precedence!\n", player);
    return TURN_PLAYER_1;
  }
  
  else
    return start;
}

Points modeGame(Card** hands, int start, char* trump, Player* players, int* order)
{
  // with trump
  // with 20 and 40
  // all 6 rounds
  // win with 66 points

  // TODO: check "call" und "start"
  //       can't we abandone call?
  
  int counter_turns = 0;
  int counter_cards = 0;
  int counter_position = 0;
  int counter_suit = 0;
  int counter_trump = 0;
  int counter_hand = 0;
  int counter_command = 0;
  char commands[7] = "qweasd";
  long length_commands = strlen(commands);
  char commands_1[7] = "\0";
  char commands_2[7] = "\0";
  char commands_3[7] = "\0";
  strcpy(commands_1, commands);
  strcpy(commands_2, commands);
  strcpy(commands_3, commands);
  
  // default
//  int call      = TURN_PLAYER_1;
//  int answer_1  = TURN_PLAYER_2;
//  int answer_2  = TURN_PLAYER_3;
  int call      = order[0] ADD_ONE;
  int answer_1  = order[1] ADD_ONE;
  int answer_2  = order[2] ADD_ONE;
  int initial_order[3] = {call, answer_1, answer_2};   // initial order
  
  int player[3] = {call, answer_1, answer_2};
  char* players_commands[3] = {commands_1, commands_2, commands_3};
  
  int position[3]   = {0};
  int position_Q[3] = {0};
  int pairs         = 0;
  
  int check = FALSE;
  int buffer = 0;
  int i = 0;
  int counter_Q = 0;
  int points_call = 0;
  int points_opponents = 0;
  int points_pair = 0;
  int buffer_start = 0;
  int bool_trump = TRUE;
  
  int points_1 = 0;
  int points_2 = 0;
  int points_3 = 0;
  int points[3] = {points_1, points_2, points_3};
  int count_bock    = 0;   // bock
  int count_suit    = 0;   // right suit
  int count_trump   = 0;   // trump
  int count_permit  = 0;
  int count_valid   = 0;
  
  Pair handle_pairs[3] = {0, 0, 0};
  
  Turn buffer_turn[3] = {{{NULL}, {0}, "", ' '}, ' ', 0};
  getCall(start, &call, &answer_1, &answer_2);
  buffer_start = start;
  Points points_and_caller = {0, 0, 0};
  char buffer_higher[6] = {'\0'};
  
  char input_CPU = '\0';
  
  Points next_and_points = {0, 0, 0};
  int bool_trumped_already = FALSE;
  
  for (counter_turns = 0; counter_turns < MAXIMUM_TURNS; counter_turns++)
  {
    // call
    printf("TURN: %d\n", counter_turns + 1);
    
    // 3 cards each round
    for (i = 0; i < 3; i++)
    {
      printHand(hands[player[i] MINUS_ONE], HAND, player[i], players[player[/*order[i]*/ i] MINUS_ONE].name_);
      printf("                         ^     ^     ^     ^     ^     ^  \n");
      printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
      
      // highlight 20 & 40
      if (i == 0)
        pairs = highlight(hands[player[i] MINUS_ONE], player[i], position_Q);
      
      printf("---------------------------------------------------------------\n");
      
      // first to call
      if (i == 0)
      {
        // check: human or CPU
        if (players[player[i] MINUS_ONE].CPU_bool_ == FALSE)
        {
          do
          {
            system ("/bin/stty raw");
            buffer = getchar();
            printf("\r");
            if (!(check = seekAndDestroy(buffer, players_commands[i])))
              printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
          } while (!check);
          system ("/bin/stty cooked");
        }
        
        else
        {
          input_CPU = checkCPU(players[player[i] MINUS_ONE], INSTANCE_CARD, players_commands[i]);
          
          while (input_CPU == RETRY)
          {
            input_CPU = checkCPU(players[player[i] MINUS_ONE], INSTANCE_CARD, players_commands[i]);
          }
          
          buffer = input_CPU;
        }
        
        switch (buffer)
        {
          case 'q':
            position[i] = 0;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // at this position we will only find a KING
              if (position[i] == position_Q[0] MINUS_ONE
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0] MINUS_ONE,
                                                           points_pair, &i, &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 40?\n");
                    pairs--;
                    // sollte ich hier nun auch theoretische positions_Q
                    // "hochrutschen"? - 1 auf 0, 2 auf 1
                    //
                    // Ja sicher!
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                }
                // 20
                else /* is_trump == FALSE */
                {
                  points_pair = 20;
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0],
                                                           points_pair,
                                                           &i, &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 20?\n");
                    pairs--;
                    // sollte ich hier nun auch theoretische positions_Q
                    // "hochrutschen"? - 1 auf 0, 2 auf 1
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                }
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                        players[turn[i] MINUS_ONE].name_);
              printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                        players[player[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          case 'w':
            position[i] = 1;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // second card = QUEEN
              if (position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 40?\n");
                }
                // 20
                else /* is_trump == FALSE */
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 20?\n");
                }
              }
              // second card = KING
              else if (position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0] MINUS_ONE,
                                                           points_pair, &i,
                                                           &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
//                  printf("check K 40?\n");
                }
                // 20
                else /* is_trump == FALSE */
                {
                  points_pair = 20;
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0],
                                                           points_pair,
                                                           &i, &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 20?\n");
                    pairs--;
                    // sollte ich hier nun auch theoretische positions_Q
                    // "hochrutschen"? - 1 auf 0, 2 auf 1
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check K 20?\n");
                }
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                        players[turn[i] MINUS_ONE].name_);
              printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                        players[player[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          case 'e':
            position[i] = 2;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // third card = QUEEN
              if (position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check Q 40?\n");
                }
                // 20
                else /* is_trump == FALSE */
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check Q 20?\n");
                }
              }
              // third card = KING
              else if ((position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                       || (position[i] == position_Q[1] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0] MINUS_ONE,
                                                           points_pair, &i,
                                                           &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                  
//                  printf("check K 40?\n");
                }
                // 40
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[1] MINUS_ONE,
                                                           points_pair, &i,
                                                           &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                  
//                  printf("check K 40?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0],
                                                           points_pair,
                                                           &i, &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 20?\n");
                    pairs--;
                    // sollte ich hier nun auch theoretische positions_Q
                    // "hochrutschen"? - 1 auf 0, 2 auf 1
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check K 20?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[1],
                                                           points_pair,
                                                           &i, &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 20?\n");
                    pairs--;
                    // sollte ich hier nun auch theoretische positions_Q
                    // "hochrutschen"? - 1 auf 0, 2 auf 1
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check K 20?\n");
                }
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                        players[turn[i] MINUS_ONE].name_);
              printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                        players[player[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          case 'a':
            position[i] = 3;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // fourth card = QUEEN
              if ((position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  || ((position[i] == position_Q[1]
                       && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)))
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check Q 40?\n");
                }
                // 40
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[1],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check Q 40?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check Q 20?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[1],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check Q 20?\n");
                }
              }
              // fourth card = KING
              else if ((position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                       || (position[i] == position_Q[1] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
              {
                if (position_Q[0] > position[0])  // first 20 or 40
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0],
                                                             points_pair,
                                                             &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      pairs--;
                      // sollte ich hier nun auch theoretische positions_Q
                      // "hochrutschen"? - 1 auf 0, 2 auf 1
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check K 20?\n");
                  }
                }
                
                else /* if (position_Q[0] < position[0]) */ // second 20 or 40
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[1] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[1],
                                                             points_pair,
                                                             &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      pairs--;
                      // sollte ich hier nun auch theoretische positions_Q
                      // "hochrutschen"? - 1 auf 0, 2 auf 1
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check K 20?\n");
                  }
                }
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                        players[turn[i] MINUS_ONE].name_);
              printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                        players[player[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          case 's':
            position[i] = 4;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // fifth card = QUEEN
              if ((position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  || ((position[i] == position_Q[1]
                       && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)))
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check Q 40?\n");
                }
                // 40
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[1],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check Q 40?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check Q 20?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[1],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                  printf("check Q 20?\n");
                }
              }
              // fifth card = KING
              else if ((position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                       || (position[i] == position_Q[1] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)
                       || (position[i] == position_Q[2] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[2]].value_ == QUEEN))
              {
                // first 20 or 40
                if (position_Q[0] > position[0])
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 20?\n");
                  }
                }
                // third 20 or 40
                else if (position_Q[2] MINUS_ONE == 4)      // only possible
                                                            // on position 4
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[2]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[2] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[2]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[2] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 20?\n");
                  }
                }
                // second 20 or 40
                else /* best work around yet :D */
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[1] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[1] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 20?\n");
                  }
                }
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                        players[turn[i] MINUS_ONE].name_);
              printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                        players[player[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          case 'd':
            position[i] = 5;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // at this position we will only find a QUEEN
              if ((position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  || (position[i] == position_Q[1]
                      && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)
                  || (position[i] == position_Q[2]
                      && hands[player[i] MINUS_ONE][position_Q[2]].value_ == QUEEN))
                
                /* QUICK AND DIRTY FIX .. IMPROVE IN "POST PRODUCTION" (; */
                
              {
                for (int j = 0; j < 3; j++)                       // sowas tu'I normalerwiese NIE
                {
                  if (position_Q[j] == position[i] /* 5 */)
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[j]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[j],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 40?\n");
                    }
                    // 20
                    else /* is_trump == FALSE */
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[j],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 20?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 20?\n");
                    }
                  }
                }
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                        players[turn[i] MINUS_ONE].name_);
              printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                        players[player[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          default:
            break;
        }
      }
      
      // further playing
      else if (i != 0 && i < 3)
      {
        // -----------
        
        // rewind bock
        if (hands[player[i] MINUS_ONE][0].is_bock_ == TRUE) // hard coded 0 sollte passen
                                                            // zumal: ein bock: alle bock
        {
          for (counter_cards = 0; counter_cards < HAND; counter_cards++)
          {
            hands[player[i] MINUS_ONE][counter_cards].is_bock_ = FALSE;
          }
        }

        // check position(s) of valid cards for this round
        // do not allow any other card
        // delete non valid characters from commands
        count_bock    = 0;   // bock
        count_suit    = 0;   // right suit
        count_trump   = 0;   // trump
        
        // seek
        for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
             counter_cards++)
        {
          // have trump
          if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_ == TRUE)
          {
//            if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_)
            count_trump++;
          }
        }
        
        for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
             counter_cards++)
        {
          // right suit
          if (i == 1)
          {
            if ((hands)[player[i] MINUS_ONE][counter_cards].suit_
                == (hands)[player[i - 1] MINUS_ONE][position[i - 1]].suit_)
            {
              // rigtht suit is trump (: trump has been played )
              if (hands[player[i] MINUS_ONE][counter_cards].is_trump_ == TRUE)
              {
                count_trump--;
              }
              count_suit++;
            }
          }
          
          else /* if (i == 2) */
          {
            if ((hands)[player[i] MINUS_ONE][counter_cards].suit_
                == (hands)[player[i - 2] MINUS_ONE][position[i - 2]].suit_)
            {
              // rigtht suit is trump (: trump has been played )
              if (hands[player[i] MINUS_ONE][counter_cards].is_trump_ == TRUE)
              {
                count_trump--;
              }
              count_suit++;
            }
          }
        }
        
        for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
             counter_cards++)
        {
          // no trump && no suit = all bock
          if (i == 1)
          {
            if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_ == FALSE
                && (hands)[player[i] MINUS_ONE][counter_cards].suit_
                != (hands)[player[i - 1] MINUS_ONE][position[i - 1]].suit_)
            {
              count_bock++;
            }
          }
          
          else /* if (i == 2) */
          {
            if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_ == FALSE
                && (hands)[player[i] MINUS_ONE][counter_cards].suit_
                != (hands)[player[i - 2] MINUS_ONE][position[i - 2]].suit_)
            {
              count_bock++;
            }
          }
        }
        
        // .. and adjust ------------
        strcpy(players_commands[i], "000000");    // maybe there is a better
                                                  // more professinal way
        
        counter_trump   = 0;
        counter_suit    = 0;
        counter_cards   = 0;
        counter_hand    = 0;
        counter_command = 0;
        
        // all bock - no matter which answer
        if ((count_suit == 0 && count_trump == 0))
        {
          // according to turn, -1
//          printf("Alles Bock!\n");
          while (counter_hand < (HAND))
          {
            if ((hands)[player[i] MINUS_ONE][counter_hand].suit_ != NULL)
            {
              players_commands[i][counter_cards] = commands[counter_command];
              counter_command++;
              counter_cards++;
            }
            
            else
            {
              counter_command++;
            }
            
            counter_hand++;
          }
        }
        
        // no valid suit but at least one trump
        else if (count_suit == 0 && count_trump > 0)
        {
          counter_trump   = 0;
          counter_suit    = 0;
          counter_cards   = 0;
          counter_hand    = 0;
          counter_command = 0;
          // must hold trump
          // allow trump(s)
//          printf("Muaßt an Trumpf spiel'n!\n");

          while (counter_hand < (HAND/* - counter_turns*/)/* eben nicht strlen(players_commands[i])*/)
          {
            // differenciate between answer 1 and answer 2
            // answer 1 must trump call & answer 2 must trump either
            if ((hands)[player[i] MINUS_ONE][counter_hand].suit_ != NULL
                && hands[player[i] MINUS_ONE][counter_hand].is_trump_ == TRUE)
            {
              players_commands[i][counter_cards] = commands[counter_command];
              counter_command++;
              counter_cards++;
            }
            
            else
            {
              counter_command++;
            }
            
            counter_hand++;
          }
          
          // remove right
          for (counter_cards = count_trump; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
          {
            players_commands[i][counter_cards] = '\0';
          }
        } // end else if (must play trump)
        
        else /* must hold suit, at leas one */
        {
//          printf("Muaßt Farb' spielen!\n");
          counter_cards = 0;
          counter_command = 0;
          
          // remove left
          for (counter_hand = 0; counter_hand < (HAND/* - counter_turns*/); counter_hand++)
          {
            // suit found
            if (i == 1)   // first to answer
            {
              if ((hands)[player[i] MINUS_ONE][counter_hand].suit_
                  == (hands)[player[i - 1] MINUS_ONE][position[i - 1]].suit_)
              {
//                printf("gleiche Farbe @ %d\n", counter_hand);
//                printf("%c = %c\n", players_commands[i][counter_hand], commands[counter_command]);
                players_commands[i][counter_cards] = commands[counter_command];
                counter_command++;
                counter_cards++;
              }
              else
              {
                counter_command++;
              }
            }
            
            else /* (i == 1) */   // second to answer
            {
              if ((hands)[player[i] MINUS_ONE][counter_hand].suit_
                  == (hands)[player[i - 2] MINUS_ONE][position[i - 2]].suit_)
              {
//                printf("gleiche Farbe @ %d\n", counter_hand);
                players_commands[i][counter_cards] = commands[counter_command];
                counter_command++;
                counter_cards++;
              }
              else
              {
//                players_commands[i][counter_hand] = commands[counter_command];
                counter_command++;
              }
            }
          }
          
          // remove right
          for (counter_cards = count_suit; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
          {
            players_commands[i][counter_cards] = '\0';
          }
        } // end else (must play suit)
        
        // now that we have found valid cards, go on and only allow permitted cards
        // we do not have to mind case:     all bock
        // but                              trump (in case trump has been played)
        //                                  suit
        // what do we need? - (answer 1): called card's value
        //                    (answer 2): called card's value and answer 1's value
        
        if (i == 1)
        {
          // if there is at least one valid card to answer with
          if (!(count_suit == 0 && count_trump == 0))
          {
            // compare values and only allow weaker cards if there is not one
            // which could trump call
            counter_command = 0;
            counter_cards   = 0;
            count_permit    = 0;
            
            if (count_suit > 0)
            {
              while (players_commands[i][counter_command] != '\0')
              {
                if ((hands)[player[i] MINUS_ONE][counter_cards].value_ >
                    /* called card */ hands[player[0] MINUS_ONE][position[0]].value_)
                {
                  printf("%s %d - %s %d\n", hands[player[0] MINUS_ONE][position[0]].suit_,
                         hands[player[0] MINUS_ONE][position[0]].value_,
                         (hands)[player[i] MINUS_ONE][counter_cards].suit_,
                         (hands)[player[i] MINUS_ONE][counter_cards].value_);
                  buffer_higher[counter_cards] = players_commands[i][counter_cards];
                  buffer_higher[counter_cards ADD_ONE] = '\0';
                  counter_cards++;
                  
                  if (player[0] == initial_order[0])
                  {
                    bool_trumped_already = TRUE;
                  }
                }
                counter_command++;
              }
            }
            
            if (counter_cards > 0)
            {
              strcpy(players_commands[i], "\0\0\0\0\0\0");
              strcpy(players_commands[i], buffer_higher);
              strcpy(buffer_higher, "\0\0\0\0\0\0");
            }
          }
        }
        
        else /* if (i == 2) */
        {
          // if there is at least one valid card to answer with
          if (!(count_suit == 0 && count_trump == 0))
          {
            // compare values and only allow weaker cards if there is not one
            // which could trump any (call || answer 1)
            counter_command = 0;
            counter_cards   = 0;
            count_permit    = 0;
            
            if (count_suit > 0)
            {
              if (bool_trumped_already == FALSE)
              {
                while (players_commands[i][counter_command] != '\0')
                {
                  // has to trump caller if possible
                  if ((hands)[player[i] MINUS_ONE][counter_cards].value_ <
                      /* called card */ hands[player[0] MINUS_ONE][position[0]].value_
                      || (hands)[player[i] MINUS_ONE][counter_cards].value_ <
                      /* answer 1's card */ hands[player[i - 1] MINUS_ONE][position[1/*0*/]].value_)
                  {

                  }
                  else
                  {
                    buffer_higher[counter_cards] = players_commands[i][counter_cards];
                    buffer_higher[counter_cards ADD_ONE] = '\0';
                    counter_cards++;
                  }
                counter_command++;
                }
              }

              else
              {
                // no need to trump caller
              }
            }
            
            else if (count_trump > 0)
            {
//              printf("%d\n", (hands)[player[i] MINUS_ONE][position[i MINUS_ONE]].is_trump_);
              if ((hands)[player[i] MINUS_ONE][position[i MINUS_ONE]].is_trump_)
              {
                while (players_commands[i][counter_command] != '\0')
                {
                  // only need to compare trump's value with answer 1
                  if ((hands)[player[i] MINUS_ONE][counter_cards].value_ <
                      /* answer 1's card */ hands[player[i MINUS_ONE] MINUS_ONE][position[i MINUS_ONE]].value_)
                  {
                    printf("\n!!! 3622 !!!\n");
                  }
                  else
                  {
                    buffer_higher[counter_cards] = players_commands[i][counter_cards];
                    buffer_higher[counter_cards ADD_ONE] = '\0';
                    counter_cards++;
                  }
                  counter_command++;
                }
              }
            }
            
            if (counter_cards > 0)
            {
              strcpy(players_commands[i], "\0\0\0\0\0\0");
              strcpy(players_commands[i], buffer_higher);
              strcpy(buffer_higher, "\0\0\0\0\0\0");
            }
          }
        }
        
        // -----------
        
        // check: human or CPU
        if (players[player[i] MINUS_ONE].CPU_bool_ == FALSE)
        {
          do
          {
            system ("/bin/stty raw");
            buffer = getchar();
            printf("\r");
            if (!(check = seekAndDestroy(buffer, players_commands[i])))
              printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
          } while (!check);
          system ("/bin/stty cooked");
        }
        
        else
        {
          input_CPU = checkCPU(players[player[i] MINUS_ONE], INSTANCE_CARD, players_commands[i]);
          
          while (input_CPU == RETRY)
          {
            input_CPU = checkCPU(players[player[i] MINUS_ONE], INSTANCE_CARD, players_commands[i]);
          }
          
          buffer = input_CPU;
        }
        
        switch (buffer)
        {
          case 'q':
            position[i] = 0;
            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
  //          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
            printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                      players[player[i] MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'w':
            position[i] = 1;
            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
  //          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
            printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                      players[player[i] MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'e':
            position[i] = 2;
            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
  //          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
            printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                      players[player[i] MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'a':
            position[i] = 3;
            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
  //          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
            printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                      players[player[i] MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 's':
            position[i] = 4;
            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
  //          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
            printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                      players[player[i] MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'd':
            position[i] = 5;
            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
  //          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
            printHand((hands)[player[i] MINUS_ONE], 6, player[i],
                      players[player[i] MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          default:
            break;
        }
      } // end else
    } // end of for( i )
    
    // --------------------------------------------------------------------------------
    if (i > 3) // exit condition: 20 or 40 shown only - enough & exit
    {
      printf("points caller: %d\n", points_call);
      printf("points opponents: %d\n", points_opponents);
      break;
    }
    // --------------------------------------------------------------------------------
    
    i = 0;
    
    next_and_points = next(initial_order, player,
                           hands[player[i] MINUS_ONE][position[i]],
                           (hands)[player[i + 1] MINUS_ONE][position[i + 1]],
                           (hands)[player[i + 2] MINUS_ONE][position[i + 2]]);
    
    // ...
    if (initial_order[0] == TURN_PLAYER_2)
    {
      if (next_and_points.winner_ == 2)
        next_and_points.winner_ = TURN_PLAYER_3;
      else if (next_and_points.winner_ == 3)
        next_and_points.winner_ = TURN_PLAYER_1;
      else
        next_and_points.winner_ = TURN_PLAYER_2;
    }
    
    if (initial_order[0] == TURN_PLAYER_3)
    {
      if (next_and_points.winner_ == 2)
        next_and_points.winner_ = TURN_PLAYER_1;
      else if (next_and_points.winner_ == 3)
        next_and_points.winner_ = TURN_PLAYER_2;
      else
        next_and_points.winner_ = TURN_PLAYER_3;
    }
    // ...
    
    start = next_and_points.winner_;
    
    // TODO: irgendwas passt nicht, wenn in weiterer Folge wer anderes Rufer ist
    
    if (start == initial_order[0])
      points_call += next_and_points.points_;
    else
      points_opponents += next_and_points.points_;
    
    getCall(start, &call, &answer_1, &answer_2);
    
    // reset (points) for next round
    points[0] = 0;
    points[1] = 0;
    points[2] = 0;
    pairs = resetPairs(position_Q);
    resetHandlePairs(handle_pairs);
    points_pair = 0;
    next_and_points.points_ = 0;
    next_and_points.winner_ = 0;
    next_and_points.caller_ = 0;
    bool_trumped_already = FALSE;
    
    if (points_call >= 66)
    {
//      points_and_caller = {};
      break;
    }
    
    else if (points_opponents >= 66)
    {
//      points_and_caller = {};
      break;
    }
    
    // who has played the highest card calls next - and exactly this is the bitch here
    
    // may we do this here -----------------------
    removeCard(&hands[player[i] MINUS_ONE][position[i]]);
    removeCard(&hands[player[i + 1] MINUS_ONE][position[i + 1]]);
    removeCard(&hands[player[i + 2] MINUS_ONE][position[i + 2]]);
    // -------------------------------------------
    // write all left over cards onto commands

    // player 1
    counter_position = 0 ;
    for (counter_cards = 0; counter_cards < HAND; counter_cards++)
    {
      if ((hands)[player[i] MINUS_ONE][counter_cards].suit_ != NULL)
      {
        players_commands[i][counter_position] = commands[counter_cards];
        counter_position++;
      }
    }
    players_commands[i][counter_position] = '\0';
    
    // player 2
    counter_position = 0 ;
    for (counter_cards = 0; counter_cards < HAND; counter_cards++)
    {
      if ((hands)[player[i + 1] MINUS_ONE][counter_cards].suit_ != NULL)
      {
        players_commands[i + 1][counter_position] = commands[counter_cards];
        counter_position++;
      }
    }
    players_commands[i + 1][counter_position] = '\0';
    
    // player 3
    counter_position = 0 ;
    for (counter_cards = 0; counter_cards < HAND; counter_cards++)
    {
      if ((hands)[player[i + 2] MINUS_ONE][counter_cards].suit_ != NULL)
      {
        players_commands[i + 2][counter_position] = commands[counter_cards];
        counter_position++;
      }
    }
    players_commands[i + 2][counter_position] = '\0';
    // -------------------------------------------
    
    // sort players' order
//    getCall(start, &call, &answer_1, &answer_2); -> copied to 3771
    
    player[0] = call;
    player[1] = answer_1;
    player[2] = answer_2;
    
    
    if (player[0] == TURN_PLAYER_1)
    {
      players_commands[0] = commands_1;
      players_commands[1] = commands_2;
      players_commands[2] = commands_3;
    }
    
    if (player[0] == TURN_PLAYER_2)
    {
      players_commands[0] = commands_2;
      players_commands[1] = commands_3;
      players_commands[2] = commands_1;
    }
    
    if (player[0] == TURN_PLAYER_3)
    {
      players_commands[0] = commands_3;
      players_commands[1] = commands_1;
      players_commands[2] = commands_2;
    }
    
    // print points
    printf("player:  %d\nopponents:  %d\n", points_call, points_opponents);

  } // end for()
  
  // TODO: letzter Stuch bringt nix .. Call < 66 = TOT
  
  points_and_caller.caller_ = TURN_PLAYER_1;
  
  if (points_call < 66)
  {
    if (points_call == 0)
    {
      printf("Opponents win 3 points!\n");
      printf("---------------------------------------------------------------\n");
      points_and_caller.points_ = 3;
      // points_and_caller.caller_ = IRGENDWER;
      points_and_caller.winner_ = OPPONENTS_WIN;
      return points_and_caller;
    }
    
    else if (points_call < 33)
    {
      printf("Opponents win 2 points!\n");
      printf("---------------------------------------------------------------\n");
      points_and_caller.points_ = 2;
      // points_and_caller.caller_ = IRGENDWER;
      points_and_caller.winner_ = OPPONENTS_WIN;
      return points_and_caller;
    }
    
    else /*if (points_call > 33)*/
    {
      printf("Opponents win 1 points!\n");
      printf("---------------------------------------------------------------\n");
      points_and_caller.points_ = 1;
      // points_and_caller.caller_ = IRGENDWER;
      points_and_caller.winner_ = OPPONENTS_WIN;
      return points_and_caller;
    }
  }
  
  else /* caller wins (modeGame() = player 1) */
  {
    if (points_opponents == 0)
    {
      printf("Player %d wins 3 points!\n", buffer_start);
      printf("---------------------------------------------------------------\n");
      points_and_caller.points_ = 3;
      points_and_caller.winner_ = TURN_PLAYER_1 /*buffer_start*/;
      return points_and_caller;
    }
    
    else if (points_opponents < 33)
    {
      printf("Player %d wins 2 points!\n", buffer_start);
      printf("---------------------------------------------------------------\n");
      points_and_caller.points_ = 2;
      points_and_caller.winner_ = TURN_PLAYER_1;
      return points_and_caller;
    }
    
    else /*if (points_opponent > 33)*/
    {
      printf("Player %d wins 1 points!\n", buffer_start);
      printf("---------------------------------------------------------------\n");
      points_and_caller.points_ = 1;
      points_and_caller.winner_ = TURN_PLAYER_1;
      return points_and_caller;
    }
  }
}

Points modeSchnapser(Card** hands, int start, char* trump, Player* players)
{
  // with trump
  // with 20 and 40
  // 4 rounds
  // win with 66
  
  // TODO: check "call" und "start"
  //       can't we abandone call?
  
  int counter_turns = 0;
  int counter_cards = 0;
  int counter_position = 0;
  int counter_suit = 0;
  int counter_trump = 0;
  int counter_hand = 0;
  int count_permit  = 0;
  int counter_command = 0;
  int counter_commands  = 0;
  char commands[7] = "qweasd";
  char commands_1[7] = "\0";
  char commands_2[7] = "\0";
  char commands_3[7] = "\0";
  strcpy(commands_1, commands);
  strcpy(commands_2, commands);
  strcpy(commands_3, commands);
  
  // default
  int call      = TURN_PLAYER_1;
  int answer_1  = TURN_PLAYER_2;
  int answer_2  = TURN_PLAYER_3;
  int turn[3] = {call, answer_1, answer_2};
  
  int player[3] = {call, answer_1, answer_2};
  char* players_commands[3] = {commands_1, commands_2, commands_3};
  
  int position[3]   = {0};
  int position_Q[3] = {0};
  int pairs         = 0;
  
  int check = FALSE;
  int buffer = 0;
  int i = 0;
  int points_call = 0;
  int points_opponents = 0;
  int buffer_start = 0;
  
  int points_1  = 0;
  int points_2  = 0;
  int points_3  = 0;
  int points_pair   = 0;
  int points[3] = {points_1, points_2, points_3};
  int count_bock    = 0;   // bock
  int count_suit    = 0;   // right suit
  int count_trump   = 0;   // trump
  
  Pair handle_pairs[3] = {0, 0, 0};
  Points points_and_caller = {0, 0};
  
  char buffer_higher[6] = {'\0'};
  
  // sort
  if (start != call)
  {
    if (start == TURN_PLAYER_2)
    {
      points[0]           = points_2;
      points[1]           = points_3;
      points[2]           = points_1;
      players_commands[0] = commands_2;
      players_commands[1] = commands_3;
      players_commands[2] = commands_1;
      player[0]           = answer_1;
      player[1]           = answer_2;
      player[2]           = call;
      turn[0]             = answer_1;
      turn[1]             = answer_2;
      turn[2]             = call;
    }
    
    else if (start == TURN_PLAYER_3)
    {
      points[0]           = points_3;
      points[1]           = points_1;
      points[2]           = points_2;
      players_commands[0] = commands_3;
      players_commands[1] = commands_1;
      players_commands[2] = commands_2;
      player[0]           = answer_2;
      player[1]           = call;
      player[2]           = answer_1;
      turn[0]             = answer_2;
      turn[1]             = call;
      turn[2]             = answer_1;
    }
  }
  
  Turn buffer_turn[3] = {{{NULL}, {0}, "", ' '}, ' ', 0};
  getCall(start, &call, &answer_1, &answer_2);
  buffer_start = start;
  
  // Schnapser
  if (player[0] == TURN_PLAYER_1)
  {
    for (counter_turns = 0; counter_turns < 4; counter_turns++)
    {
      // call
      printf("TURN: %d\n", counter_turns + 1);
      
      // 3 cards each round
      for (i = 0; i < 3; i++)
      {
        printHand(hands[player[i] MINUS_ONE], HAND, turn[i],
                  players[turn[i] MINUS_ONE].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        
        // highlight 20 & 40
        if (i == 0)
        {
          pairs = highlight(hands[player[i] MINUS_ONE], player[i], position_Q);
        
          printf("TEST: %d pairs with first Queen @ %d\n", pairs, position_Q[0]);
        }
        // and reset, after having worked with it
//        pairs = resetPairs(position_Q);
        
        printf("---------------------------------------------------------------\n");
        
        // first to call
        if (i == 0)
        {
          do
          {
            system ("/bin/stty raw");
            buffer = getchar();
            printf("\r");
            if (!(check = seekAndDestroy(buffer, players_commands[i])))
              printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
          } while (!check);
          system ("/bin/stty cooked");
          
          switch (buffer)     // copied from modeGame(first to call)
                              // extract as function in post_production
          {
            case 'q':
              position[i] = 0;
              
              // check for 20 or 40
              if (pairs > 0)
              {
                // at this position we will only find a KING
                if (position[i] == position_Q[0] MINUS_ONE
                     && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      pairs--;
                      // sollte ich hier nun auch theoretische positions_Q
                      // "hochrutschen"? - 1 auf 0, 2 auf 1
                      //
                      // Ja sicher!
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
//                    printf("check K 40?\n");
                  }
                  // 20
                  else /* is_trump == FALSE */
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0],
                                                             points_pair,
                                                             &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      pairs--;
                      // sollte ich hier nun auch theoretische positions_Q
                      // "hochrutschen"? - 1 auf 0, 2 auf 1
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
//                    printf("check K 20?\n");
                  }
                }
              }
              
              if (i == ENOUGH)
              {
                break;
              }
              
              else
              {
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[turn[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
              }
              
            case 'w':
              position[i] = 1;
              
              // check for 20 or 40
              if (pairs > 0)
              {
                // second card = QUEEN
                if (position[i] == position_Q[0]
                     && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[0],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
//                    printf("check Q 40?\n");
                  }
                  // 20
                  else /* is_trump == FALSE */
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[0],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
//                    printf("check Q 20?\n");
                  }
                }
                // second card = KING
                else if (position[i] == position_Q[0] MINUS_ONE
                         && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0] MINUS_ONE,
                                                             points_pair, &i,
                                                             &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
//                    printf("check K 40?\n");
                  }
                  // 20
                  else /* is_trump == FALSE */
                  {
                    points_pair = 20;
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0],
                                                             points_pair,
                                                             &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      pairs--;
                      // sollte ich hier nun auch theoretische positions_Q
                      // "hochrutschen"? - 1 auf 0, 2 auf 1
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
//                    printf("check K 20?\n");
                  }
                }
              }
              
              if (i == ENOUGH)
              {
                break;
              }
              
              else
              {
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[turn[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
              }
              
            case 'e':
              position[i] = 2;
              
              // check for 20 or 40
              if (pairs > 0)
              {
                // third card = QUEEN
                if (position[i] == position_Q[0]
                     && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[0],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check Q 40?\n");
                  }
                  // 20
                  else /* is_trump == FALSE */
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[0],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check Q 20?\n");
                  }
                }
                // third card = KING
                else if ((position[i] == position_Q[0] MINUS_ONE
                         && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                         || (position[i] == position_Q[1] MINUS_ONE
                             && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0] MINUS_ONE,
                                                             points_pair, &i,
                                                             &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 40?\n");
                  }
                  // 40
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[1] MINUS_ONE,
                                                             points_pair, &i,
                                                             &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0],
                                                             points_pair,
                                                             &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      pairs--;
                      // sollte ich hier nun auch theoretische positions_Q
                      // "hochrutschen"? - 1 auf 0, 2 auf 1
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check K 20?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[1],
                                                             points_pair,
                                                             &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      pairs--;
                      // sollte ich hier nun auch theoretische positions_Q
                      // "hochrutschen"? - 1 auf 0, 2 auf 1
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check K 20?\n");
                  }
                }
              }
              
              if (i == ENOUGH)
              {
                break;
              }
              
              else
              {
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[turn[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
              }
              
            case 'a':
              position[i] = 3;
              
              // check for 20 or 40
              if (pairs > 0)
              {
                // fourth card = QUEEN
                if ((position[i] == position_Q[0]
                     && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                    || ((position[i] == position_Q[1]
                         && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)))
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[0],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check Q 40?\n");
                  }
                  // 40
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[1],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check Q 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[0],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check Q 20?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[1],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check Q 20?\n");
                  }
                }
                // fourth card = KING
                else if ((position[i] == position_Q[0] MINUS_ONE
                         && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                         || (position[i] == position_Q[1] MINUS_ONE
                             && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
                {
                  if (position_Q[0] > position[0])  // first 20 or 40
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[0] MINUS_ONE,
                                                               points_pair, &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
                      
//                      printf("check K 40?\n");
                    }
                    // 20
                    else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[0],
                                                               points_pair,
                                                               &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 20?\n");
                        pairs--;
                        // sollte ich hier nun auch theoretische positions_Q
                        // "hochrutschen"? - 1 auf 0, 2 auf 1
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check K 20?\n");
                    }
                  }
                  
                  else /* if (position_Q[0] < position[0]) */ // second 20 or 40
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[1] MINUS_ONE,
                                                               points_pair, &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
                      
//                      printf("check K 40?\n");
                    }
                    // 20
                    else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[1],
                                                               points_pair,
                                                               &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 20?\n");
                        pairs--;
                        // sollte ich hier nun auch theoretische positions_Q
                        // "hochrutschen"? - 1 auf 0, 2 auf 1
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check K 20?\n");
                    }
                  }
                }
              }
              
              if (i == ENOUGH)
              {
                break;
              }
              
              else
              {
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[turn[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
              }
              
            case 's':
              position[i] = 4;
              
              // check for 20 or 40
              if (pairs > 0)
              {
                // fifth card = QUEEN
                if ((position[i] == position_Q[0]
                     && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                    || ((position[i] == position_Q[1]
                         && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)))
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[0],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check Q 40?\n");
                  }
                  // 40
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[1],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check Q 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[0],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check Q 20?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_
                      = pairHandler(handle_pairs, position_Q[1],
                                    points_pair, &i, &points_call,
                                    &points_opponents,
                                    hands[player[i] MINUS_ONE],
                                    (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("Q 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                    printf("check Q 20?\n");
                  }
                }
                // fifth card = KING
                else if ((position[i] == position_Q[0] MINUS_ONE
                         && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                         || (position[i] == position_Q[1] MINUS_ONE
                             && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)
                         || (position[i] == position_Q[2] MINUS_ONE
                             && hands[player[i] MINUS_ONE][position_Q[2]].value_ == QUEEN))
                {
                  // first 20 or 40
                  if (position_Q[0] > position[0])
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[0] MINUS_ONE,
                                                               points_pair, &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
                      
//                      printf("check K 40?\n");
                    }
                    // 20
                    else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[0] MINUS_ONE,
                                                               points_pair, &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 20?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
                      
//                      printf("check K 20?\n");
                    }
                  }
                  // third 20 or 40
                  else if (position_Q[2] MINUS_ONE == 4)      // only possible
                                                              // on position 4
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[2]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[2] MINUS_ONE,
                                                               points_pair, &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
                      
//                      printf("check K 40?\n");
                    }
                    // 20
                    else if (hands[player[i] MINUS_ONE][position_Q[2]].is_trump_ != TRUE)
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[2] MINUS_ONE,
                                                               points_pair, &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 20?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
                      
//                      printf("check K 20?\n");
                    }
                  }
                  // second 20 or 40
                  else /* best work around yet :D */
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[1] MINUS_ONE,
                                                               points_pair, &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
                      
//                      printf("check K 40?\n");
                    }
                    // 20
                    else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                    {
                      points_pair = 20;
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[1] MINUS_ONE,
                                                               points_pair, &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 20?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
                      
//                      printf("check K 20?\n");
                    }
                  }
                }
              }
              
              if (i == ENOUGH)
              {
                break;
              }
              
              else
              {
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[turn[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
              }
              
            case 'd':
              position[i] = 5;
              
              // check for 20 or 40
              if (pairs > 0)
              {
                // at this position we will only find a QUEEN
                if ((position[i] == position_Q[0]
                     && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                    || (position[i] == position_Q[1]
                        && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)
                    || (position[i] == position_Q[2]
                        && hands[player[i] MINUS_ONE][position_Q[2]].value_ == QUEEN))
                  
                  /* QUICK AND DIRTY FIX .. IMPROVE IN "POST PRODUCTION" (; */
                  
                {
                  for (int j = 0; j < 3; j++)                       // sowas tu'I normalerwiese NIE
                  {
                    if (position_Q[j] == position[i] /* 5 */)
                    {
                      // 40
                      if (hands[player[i] MINUS_ONE][position_Q[j]].is_trump_ == TRUE)
                      {
                        points_pair = 40;
                        
                        handle_pairs[i].bool_pair_
                          = pairHandler(handle_pairs, position_Q[j],
                                        points_pair, &i, &points_call,
                                        &points_opponents,
                                        hands[player[i] MINUS_ONE],
                                        (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("Q 40?\n");
                          
                          pairs--;
                          
                          position_Q[0] = position_Q[1];
                          position_Q[1] = position_Q[2];
                          position_Q[2] = 0;
                        }
                        
//                        printf("check Q 40?\n");
                      }
                      // 20
                      else /* is_trump == FALSE */
                      {
                        points_pair = 20;
                        
                        handle_pairs[i].bool_pair_
                          = pairHandler(handle_pairs, position_Q[j],
                                        points_pair, &i, &points_call,
                                        &points_opponents,
                                        hands[player[i] MINUS_ONE],
                                        (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("Q 20?\n");
                          
                          pairs--;
                          
                          position_Q[0] = position_Q[1];
                          position_Q[1] = position_Q[2];
                          position_Q[2] = 0;
                        }
                        
//                        printf("check Q 20?\n");
                      }
                    }
                  }
                }
              }
              
              if (i == ENOUGH)
              {
                break;
              }
              
              else
              {
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[turn[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
              }
              
            default:
              break;
          }
        }
        
        // further playing
        else
        {
          // -----------
          
          // rewind bock
          if (hands[player[i] MINUS_ONE][0].is_bock_ == TRUE) // hard coded 0 sollte passen
                                                              // zumal: ein bock: alle bock
          {
            for (counter_cards = 0; counter_cards < HAND; counter_cards++)
            {
              hands[player[i] MINUS_ONE][counter_cards].is_bock_ = FALSE;
            }
          }

          // check position(s) of valid cards for this round
          // do not allow any other card
          // delete non valid characters from commands
          count_bock    = 0;   // bock        // zählt "leider" auch weiterhin
                                                  // gespielte Karten mit
          count_suit    = 0;   // right suit
          count_trump   = 0;   // trump
          
          // seek
          for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
               counter_cards++)
          {
            // have trump
            if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_ == TRUE)
            {
  //            if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_)
              count_trump++;
            }
          }
          
          for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
               counter_cards++)
          {
            // right suit
            if (i == 1)
            {
              if ((hands)[player[i] MINUS_ONE][counter_cards].suit_
                  == (hands)[player[i - 1] MINUS_ONE][position[i - 1]].suit_)
              {
                // rigtht suit is trump (: trump has been played )
                if (hands[player[i] MINUS_ONE][counter_cards].is_trump_ == TRUE)
                {
                  count_trump--;
                }
                count_suit++;
              }
            }
            
            else
            {
              if ((hands)[player[i] MINUS_ONE][counter_cards].suit_
                  == (hands)[player[i - 2] MINUS_ONE][position[i - 2]].suit_)
              {
                // rigtht suit is trump (: trump has been played )
                if (hands[player[i] MINUS_ONE][counter_cards].is_trump_ == TRUE)
                {
                  count_trump--;
                }
                count_suit++;
              }
            }
          }
          
          for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
               counter_cards++)
          {
            // no trump && no suit = all bock
            if (i == 1)
            {
              if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_ == FALSE
                  && (hands)[player[i] MINUS_ONE][counter_cards].suit_
                  != (hands)[player[i - 1] MINUS_ONE][position[i - 1]].suit_)
              {
                count_bock++;
              }
            }
            
            else
            {
              if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_ == FALSE
                  && (hands)[player[i] MINUS_ONE][counter_cards].suit_
                  != (hands)[player[i - 2] MINUS_ONE][position[i - 2]].suit_)
              {
                count_bock++;
              }
            }
          }
          
  //        printf("bock: %d\nsuit: %d\ntrump: %d\n",
  //               count_bock, count_suit, count_trump);
          
          // .. and adjust ------------
          strcpy(players_commands[i], "000000");    // maybe there is a better
                                                    // more professinal way
          
          // all bock
          counter_trump   = 0;
          counter_suit    = 0;
          counter_cards   = 0;
          counter_hand    = 0;
          counter_command = 0;
          
          // all bock - nix sticht
          if ((count_suit == 0 && count_trump == 0))
          {
            // according to turn, -1
//            printf("Alles Bock!\n");
            
            while (counter_hand < (HAND))
            {
              if ((hands)[player[i] MINUS_ONE][counter_hand].suit_ != NULL)
              {
                players_commands[i][counter_cards] = commands[counter_command];
                counter_command++;
                counter_cards++;
              }
              
              else
              {
                counter_command++;
              }
              
              counter_hand++;
            }
          }
          
          // no valid suit but trump
          else if (count_suit == 0 && count_trump > 0)
          {
            counter_trump   = 0;
            counter_suit    = 0;
            counter_cards   = 0;
            counter_hand    = 0;
            counter_command = 0;
            // must hold trump
            // allow trump(s)
//            printf("Muaßt an Trumpf spiel'n!\n");

            while (counter_hand < (HAND/* - counter_turns*/)/* eben nicht strlen(players_commands[i])*/)
            {
              if ((hands)[player[i] MINUS_ONE][counter_hand].suit_ != NULL
                  && hands[player[i] MINUS_ONE][counter_hand].is_trump_ == TRUE)
              {
                players_commands[i][counter_cards] = commands[counter_command];
                counter_command++;
                counter_cards++;
              }
              
              else
              {
                counter_command++;
              }
              
              counter_hand++;
            }
            
            // remove right
            for (counter_cards = count_trump; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
            {
              players_commands[i][counter_cards] = '\0';
            }
          }
          
          else /* must hold suit, at least one */
          {
//            printf("Muaßt Farb' spielen!\n");
            counter_cards = 0;
            counter_command = 0;
            
            // remove left
            for (counter_hand = 0; counter_hand < (HAND/* - counter_turns*/); counter_hand++)
            {
              // suit found
              if (i == 1)   // first to answer
              {
                if ((hands)[player[i] MINUS_ONE][counter_hand].suit_
                    == (hands)[player[i - 1] MINUS_ONE][position[i - 1]].suit_)
                {
  //                printf("gleiche Farbe @ %d\n", counter_hand);
  //                printf("%c = %c\n", players_commands[i][counter_hand], commands[counter_command]);
                  players_commands[i][counter_cards] = commands[counter_command];
                  counter_command++;
                  counter_cards++;
                }
                else
                {
                  counter_command++;
                }
              }
              
              else    // second to answer
              {
                if ((hands)[player[i] MINUS_ONE][counter_hand].suit_
                    == (hands)[player[i - 2] MINUS_ONE][position[i - 2]].suit_)
                {
  //                printf("gleiche Farbe @ %d\n", counter_hand);
                  players_commands[i][counter_cards] = commands[counter_command];
                  counter_command++;
                  counter_cards++;
                }
                else
                {
  //                players_commands[i][counter_hand] = commands[counter_command];
                  counter_command++;
                }
              }
            }
            
            // remove right
            for (counter_cards = count_suit; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
            {
              players_commands[i][counter_cards] = '\0';
            }
          }

          if (i == 1)
          {
            // if there is at least one valid card to answer with
            if (!(count_suit == 0 && count_trump == 0))
            {
              // compare values and only allow weaker cards if there is not one
              // which could trump call
              counter_command = 0;
              counter_cards   = 0;
              count_permit    = 0;

              if (count_suit > 0)
              {
                while (players_commands[i][counter_command] != '\0')
                {
                  if ((hands)[player[i] MINUS_ONE][counter_cards].value_ >
                      /* called card */ hands[player[0] MINUS_ONE][position[0]].value_)
                  {
                    buffer_higher[counter_cards] = players_commands[i][counter_cards];
                    buffer_higher[counter_cards ADD_ONE] = '\0';
                    counter_cards++;
                  }
                  counter_command++;
                }
              }

              if (counter_cards > 0)
              {
                strcpy(players_commands[i], "\0\0\0\0\0\0");
                strcpy(players_commands[i], buffer_higher);
                strcpy(buffer_higher, "\0\0\0\0\0\0");
              }
            }
          }

          else /* if (i == 2) */
          {
            // if there is at least one valid card to answer with
            if (!(count_suit == 0 && count_trump == 0))
            {
              // compare values and only allow weaker cards if there is not one
              // which could trump any (call || answer 1)
              counter_command = 0;
              counter_cards   = 0;
              count_permit    = 0;

              if (count_suit > 0)
              {
                while (players_commands[i][counter_command] != '\0')
                {
                  if ((hands)[player[i] MINUS_ONE][counter_cards].value_ >
                        /* called card */ hands[player[0] MINUS_ONE][position[0]].value_)
                  {
                    buffer_higher[counter_cards] = players_commands[i][counter_cards];
                    buffer_higher[counter_cards ADD_ONE] = '\0';
                    counter_cards++;
                  }
                  counter_command++;
                }
              }

              else if (count_trump > 0)
              {
  //              printf("%d\n", (hands)[player[i MINUS_ONE] MINUS_ONE][position[i MINUS_ONE]].is_trump_);
                if ((hands)[player[i MINUS_ONE] MINUS_ONE][position[i MINUS_ONE]].is_trump_)
                {
                  while (players_commands[i][counter_command] != '\0')
                  {
                    // only need to compare trump's value with answer 1
                    if ((hands)[player[i] MINUS_ONE][counter_cards].value_ >
                        /* answer 1's card */ hands[player[i - 1] MINUS_ONE][position[1]].value_)
                    {
                      buffer_higher[counter_cards] = players_commands[i][counter_cards];
                      buffer_higher[counter_cards ADD_ONE] = '\0';
                      counter_cards++;
                    }
                    counter_command++;
                  }
                }
              }

              if (counter_cards > 0)
              {
                strcpy(players_commands[i], "\0\0\0\0\0\0");
                strcpy(players_commands[i], buffer_higher);
                strcpy(buffer_higher, "\0\0\0\0\0\0");
              }
            }
          }
          
          // STUCHZWANG (answer 1&2)
//          counter_commands  = 0;
//          counter_cards     = 0;
//
//          if (!hands[player[i] MINUS_ONE][0].is_bock_)
//          {
//            while (players_commands[i][counter_commands] != '\0')
//            {
//              if (hands[player[i] MINUS_ONE][counter_commands].value_ >
//                  hands[player[0] MINUS_ONE][position[0]].value_)
//              {
//                buffer_higher[counter_cards] = players_commands[i][counter_cards];
//                buffer_higher[counter_cards ADD_ONE] = '\0';
//                counter_cards++;
//              }
//              counter_commands++;
//            }
//          }
//
//          if (counter_cards > 0)
//          {
//            strcpy(players_commands[i], "\0\0\0\0\0\0");
//            strcpy(players_commands[i], buffer_higher);
//            strcpy(buffer_higher, "\0\0\0\0\0\0");
//          }
          // ---------------------
          
          do
          {
            system ("/bin/stty raw");
            buffer = getchar();
            printf("\r");
            if (!(check = seekAndDestroy(buffer, players_commands[i])))
              printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
          } while (!check);
          system ("/bin/stty cooked");
          
          switch (buffer)       // stays the same, since it is answers only
          {
            case 'q':
              position[i] = 0;
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
    //          playCard(&(buffer_turn)[i].card_);
              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                        players[turn[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
              
            case 'w':
              position[i] = 1;
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
    //          playCard(&(buffer_turn)[i].card_);
              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                        players[turn[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
              
            case 'e':
              position[i] = 2;
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
    //          playCard(&(buffer_turn)[i].card_);
              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                        players[turn[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
              
            case 'a':
              position[i] = 3;
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
    //          playCard(&(buffer_turn)[i].card_);
              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                        players[turn[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
              
            case 's':
              position[i] = 4;
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
    //          playCard(&(buffer_turn)[i].card_);
              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                        players[turn[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
              
            case 'd':
              position[i] = 5;
              playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
    //          playCard(&(buffer_turn)[i].card_);
              printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                        players[turn[i] MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
              
            default:
              break;
          }
          
          // --------------------------------------------------------------------------
          // here we could already check whether one answer trumped call and break;
          if (!(hands[player[1] MINUS_ONE][position[1]].is_bock_)
              && (!(hands[player[2] MINUS_ONE][position[2]].is_bock_)))
          {
            // answer 1 trumps suit
            if (hands[player[0] MINUS_ONE][position[0]].suit_
                == hands[player[1] MINUS_ONE][position[1]].suit_
                && hands[player[0] MINUS_ONE][position[0]].value_ <
                    hands[player[1] MINUS_ONE][position[1]].value_)
            {
              printf("FOLD!\n");
              printf("---------------------------------------------------------------\n");
//              printf("answer 1 HAT IHN SCHON - suit!\n");
              break;
            }
            // answer 2 trumps suit
            else if (hands[player[0] MINUS_ONE][position[0]].suit_
                == hands[player[2] MINUS_ONE][position[2]].suit_
                && hands[player[0] MINUS_ONE][position[0]].value_ <
                    hands[player[2] MINUS_ONE][position[2]].value_)
            {
              printf("FOLD!\n");
              printf("---------------------------------------------------------------\n");
//              printf("answer 2 HAT IHN SCHON - suit!\n");
              break;
            }
            // answer 1 trumped with trump
            else if (hands[player[0] MINUS_ONE][position[0]].is_trump_ == FALSE
                     && hands[player[1] MINUS_ONE][position[1]].is_trump_ == TRUE)
            {
              printf("FOLD!\n");
              printf("---------------------------------------------------------------\n");
//              printf("answer 1 HAT IHN SCHON - trump!\n");
              break;
            }
            // answer 2 trumped with trump
            else if (hands[player[0] MINUS_ONE][position[0]].is_trump_ == FALSE
                     && hands[player[2] MINUS_ONE][position[2]].is_trump_ == TRUE)
            {
              printf("FOLD!\n");
              printf("---------------------------------------------------------------\n");
//              printf("answer 2 HAT IHN SCHON - trump!\n");
              break;
            }
            // otherwhise call wins and Schnapser continues
          }
          // --------------------------------------------------------------------------
        } // end else
      } // end of for( i )
      
      i = 0;
      
      nextAndPoints(&start, buffer_start, hands[player[i] MINUS_ONE][position[i]],
                    (hands)[player[i + 1] MINUS_ONE][position[i + 1]],
                    (hands)[player[i + 2] MINUS_ONE][position[i + 2]],
                    &points_call, &points_opponents,
                    &points[0], &points[1],
                    &points[2], turn);
      
      // player 1 wins and played first
      /* else */ if (player[0] == buffer_start && start == buffer_start)
      {
//        printf("points_1: %d\npoints_2: %d\npoints_3: %d\n",
//               points[0], points[1],
//               points[2]);
        points_call       += points[0];
        points_opponents  += points[1] + points[2];
        
        if (points_call > 0)
        {
          points_call += points_pair;
        }
      }
      
      points[0] = 0;
      points[1] = 0;
      points[2] = 0;
      pairs     = resetPairs(position_Q);
      resetHandlePairs(handle_pairs);
      points_pair = 0;
      
      
      if (points_call >= 66)
      {
        break;
      }
      
      else if (points_opponents >= 66)
      {
        break;
      }
      
      // who has played the highest card calls next - and exactly this is the bitch here
      
      // may we do this here -----------------------
      removeCard(&hands[player[i] MINUS_ONE][position[i]]);
      removeCard(&hands[player[i + 1] MINUS_ONE][position[i + 1]]);
      removeCard(&hands[player[i + 2] MINUS_ONE][position[i + 2]]);
      // -------------------------------------------
      // write all left over cards onto commands

      // player 1
      counter_position = 0 ;
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        if ((hands)[player[i] MINUS_ONE][counter_cards].suit_ != NULL)
        {
          players_commands[i][counter_position] = commands[counter_cards];
          counter_position++;
        }
      }
      players_commands[i][counter_position] = '\0';
      
      // player 2
      counter_position = 0 ;
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        if ((hands)[player[i + 1] MINUS_ONE][counter_cards].suit_ != NULL)
        {
          players_commands[i + 1][counter_position] = commands[counter_cards];
          counter_position++;
        }
      }
      players_commands[i + 1][counter_position] = '\0';
      
      // player 3
      counter_position = 0 ;
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        if ((hands)[player[i + 2] MINUS_ONE][counter_cards].suit_ != NULL)
        {
          players_commands[i + 2][counter_position] = commands[counter_cards];
          counter_position++;
        }
      }
      
      players_commands[i + 2][counter_position] = '\0';
      // -------------------------------------------
      
      // sort players' order
      getCall(start, &call, &answer_1, &answer_2);
      
      player[0] = call;
      player[1] = answer_1;
      player[2] = answer_2;
      
      
      if (player[0] == TURN_PLAYER_1)
      {
        players_commands[0] = commands_1;
        players_commands[1] = commands_2;
        players_commands[2] = commands_3;
        
  //      points[0] = points_1;
  //      points[1] = points_2;
  //      points[2] = points_3;
      }
      
      if (player[0] == TURN_PLAYER_2)
      {
        players_commands[0] = commands_2;
        players_commands[1] = commands_3;
        players_commands[2] = commands_1;
        
  //      points[0] = points_2;
  //      points[1] = points_3;
  //      points[2] = points_1;
      }
      
      if (player[0] == TURN_PLAYER_3)
      {
        players_commands[0] = commands_3;
        players_commands[1] = commands_1;
        players_commands[2] = commands_2;
        
  //      points[0] = points_3;
  //      points[1] = points_1;
  //      points[2] = points_2;
      }
      
      turn[0]   = call;
      turn[1]   = answer_1;
      turn[2]   = answer_2;
      
      // print points
      printf("player:  %d\nopponents:  %d\n", points_call, points_opponents);
      
      // calling player must not lose one round
      if (start != buffer_start)
      {
        printf("FOLD!\n");
        printf("---------------------------------------------------------------\n");
        points_and_caller.points_ = 6;
        points_and_caller.winner_ = OPPONENTS_WIN;          // 5 means caller loses/opponents win
        points_and_caller.caller_ = TURN_PLAYER_1;
        return points_and_caller;
        break;
      }

    } // end for()
    
    // not enough points
    if (points_call < 66)
    {
      printf("Opponents win 6 points!\n");
      printf("---------------------------------------------------------------\n");
      points_and_caller.points_ = 6;
      points_and_caller.winner_ = OPPONENTS_WIN;
      points_and_caller.caller_ = TURN_PLAYER_1;
      return points_and_caller;
    }
    
    // win
    else
    {
      printf("Player %d wins 6 points!\n", buffer_start);
      printf("---------------------------------------------------------------\n");
      points_and_caller.points_ = 6;
      points_and_caller.winner_ = TURN_PLAYER_1;
      points_and_caller.caller_ = TURN_PLAYER_1;
      return points_and_caller;
    }
  }
  
  // KontraSchnapser
  else /* player[0] != TURN_PLAYER_1 */
  {
    // player 1 plays the first card
    for (counter_turns = 0; counter_turns < 4; counter_turns++)
    {
      // call
      printf("TURN: %d\n", counter_turns + 1);
      
      // first turn "CONTRA"
      if (counter_turns == 0)
      {
        start = TURN_PLAYER_1;
        // player 1 to start
        i = 0;
        
        printHand(hands[TURN_PLAYER_1 MINUS_ONE], HAND, TURN_PLAYER_1,
                  players[TURN_PLAYER_1 MINUS_ONE].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        
        // highlight 20 & 40 .. hm .. why though?
        /* if (i == 0)
        {
          pairs = highlight(hands[start MINUS_ONE], start, position_Q);     // therefore adjust switch(first call)
                                                                              // as well (!!!)
          printf("TEST: %d pairs with first Queen @ %d\n", pairs, position_Q[0]);
          printf("A comment tells me to check switch(buffer) at this FIRST contra call\n");
        } */
        
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          buffer = getchar();
          printf("\r");
          if (!(check = seekAndDestroy(buffer, players_commands[TURN_PLAYER_1 - 1])))
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (!check);
        system ("/bin/stty cooked");
        
        switch (buffer)   // should be fine
        {
          case 'q':
//            position[i] = 0;
            position[TURN_PLAYER_1 MINUS_ONE] = 0;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // at this position we will only find a KING
              if (position[i] == position_Q[0] MINUS_ONE
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0] MINUS_ONE,
                                                           points_pair, &i, &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 40?\n");
                    pairs--;
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                }
                // 20
                else /* is_trump == FALSE */
                {
                  points_pair = 20;
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0],
                                                           points_pair,
                                                           &i, &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 20?\n");
                    pairs--;
                    // sollte ich hier nun auch theoretische positions_Q
                    // "hochrutschen"? - 1 auf 0, 2 auf 1
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                }
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][position[TURN_PLAYER_1 MINUS_ONE]]);
              printHand((hands)[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                        players[TURN_PLAYER_1 MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          case 'w':
//            position[i] = 1;
            position[TURN_PLAYER_1 MINUS_ONE] = 1;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // second card = QUEEN
              if (position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf(" check Q 40?\n");
                }
                // 20
                else /* is_trump == FALSE */
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 20?\n");
                }
              }
              // second card = KING
              else if (position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0] MINUS_ONE,
                                                           points_pair, &i,
                                                           &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
//                  printf("check K 40?\n");
                }
                // 20
                else /* is_trump == FALSE */
                {
                  points_pair = 20;
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0],
                                                           points_pair,
                                                           &i, &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 20?\n");
                    pairs--;
                    // sollte ich hier nun auch theoretische positions_Q
                    // "hochrutschen"? - 1 auf 0, 2 auf 1
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check K 20?\n");
                }
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][position[TURN_PLAYER_1 MINUS_ONE]]);
              printHand((hands)[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                        players[TURN_PLAYER_1 MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          case 'e':
//            position[i] = 2;
            position[TURN_PLAYER_1 MINUS_ONE] = 2;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // third card = QUEEN
              if (position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 40?\n");
                }
                // 20
                else /* is_trump == FALSE */
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 20?\n");
                }
              }
              // third card = KING
              else if ((position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                       || (position[i] == position_Q[1] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0] MINUS_ONE,
                                                           points_pair, &i,
                                                           &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
//                  printf("check K 40?\n");
                }
                // 40
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[1] MINUS_ONE,
                                                           points_pair, &i,
                                                           &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
//                  printf("check K 40?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0],
                                                           points_pair,
                                                           &i, &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 20?\n");
                    pairs--;
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check K 20?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[1],
                                                           points_pair,
                                                           &i, &points_call,
                                                           &points_opponents,
                                                           hands[player[i] MINUS_ONE],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("K 20?\n");
                    pairs--;
                    // sollte ich hier nun auch theoretische positions_Q
                    // "hochrutschen"? - 1 auf 0, 2 auf 1
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check K 20?\n");
                }
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][position[TURN_PLAYER_1 MINUS_ONE]]);
              printHand((hands)[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                        players[TURN_PLAYER_1 MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          case 'a':
//            position[i] = 3;
            position[TURN_PLAYER_1 MINUS_ONE] = 3;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // fourth card = QUEEN
              if ((position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  || ((position[i] == position_Q[1]
                       && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)))
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 40?\n");
                }
                // 40
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[1],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 40?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 20?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[1],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 20?\n");
                }
              }
              // fourth card = KING
              else if ((position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                       || (position[i] == position_Q[1] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
              {
                if (position_Q[0] > position[0])  // first 20 or 40
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
//                    printf("check K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0],
                                                             points_pair,
                                                             &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      pairs--;
                      // sollte ich hier nun auch theoretische positions_Q
                      // "hochrutschen"? - 1 auf 0, 2 auf 1
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
//                    printf("check K 20?\n");
                  }
                }
                
                else /* if (position_Q[0] < position[0]) */ // second 20 or 40
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[1] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
//                    printf("check K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[1],
                                                             points_pair,
                                                             &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      pairs--;
                      // sollte ich hier nun auch theoretische positions_Q
                      // "hochrutschen"? - 1 auf 0, 2 auf 1
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
//                    printf("check K 20?\n");
                  }
                }
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][position[TURN_PLAYER_1 MINUS_ONE]]);
              printHand((hands)[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                        players[TURN_PLAYER_1 MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          case 's':
//            position[i] = 4;
            position[TURN_PLAYER_1 MINUS_ONE] = 4;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // fifth card = QUEEN
              if ((position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  || ((position[i] == position_Q[1]
                       && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)))
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 40?\n");
                }
                // 40
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[1],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 40?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[0],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 20?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[i].bool_pair_
                    = pairHandler(handle_pairs, position_Q[1],
                                  points_pair, &i, &points_call,
                                  &points_opponents,
                                  hands[player[i] MINUS_ONE],
                                  (player[0] == buffer_start));
                  if (handle_pairs[i].bool_pair_ == TRUE)
                  {
//                    printf("Q 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
//                  printf("check Q 20?\n");
                }
              }
              // fifth card = KING
              else if ((position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                       || (position[i] == position_Q[1] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
              {
                if (position_Q[0] > position[0])  // first 20 or 40
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
//                    printf("check K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[0] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
//                    printf("check K 20?\n");
                  }
                }
                
                // third 20 or 40 --------------------------------------------------------------------------
                else if (position_Q[2] MINUS_ONE == 4)      // only possible
                                                            // on position 4
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[2]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[2] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[2]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[2] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
                    
//                    printf("check K 20?\n");
                  }
                }
                // --------------------------------------------------------------------------------------
                // second 20 or 40
                else
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[1] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
//                    printf("check K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                  {
                    points_pair = 20;
                    handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                             position_Q[1] MINUS_ONE,
                                                             points_pair, &i, &points_call,
                                                             &points_opponents,
                                                             hands[player[i] MINUS_ONE],
                                                             (player[0] == buffer_start));
                    if (handle_pairs[i].bool_pair_ == TRUE)
                    {
//                      printf("K 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];                            // naja, hardcoded
                      position_Q[1] = position_Q[2];                            // oba viel besseres
                      position_Q[2] = 0;                                        // fällt mir hier nit ein
                    }
//                    printf("check K 20?\n");
                  }
                }
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][position[TURN_PLAYER_1 MINUS_ONE]]);
              printHand((hands)[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                        players[TURN_PLAYER_1 MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          case 'd':
//            position[i] = 5;
            position[TURN_PLAYER_1 MINUS_ONE] = 5;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // at this position we will only find a QUEEN
              if ((position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  || (position[i] == position_Q[1]
                      && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)
                  || (position[i] == position_Q[2]
                      && hands[player[i] MINUS_ONE][position_Q[2]].value_ == QUEEN))
              {
                
                /* QUICK AND DIRTY FIX (SAME AS IN modeGame())
                  .. IMPROVE IN "POST PRODUCTION" (; */
                
                for (int j = 0; j < 3; j++)                       // sowas tu'I normalerwiese NIE
                {
                  if (position_Q[j] == position[i] /* 5 */)
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[j]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[j],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 40?\n");
                    }
                    // 20
                    else /* is_trump == FALSE */
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[j],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 20?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 20?\n");
                    }
                  }
                }
                
              }
            }
            
            if (i == ENOUGH)
            {
              break;
            }
            
            else
            {
              playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][position[TURN_PLAYER_1 MINUS_ONE]]);
              printHand((hands)[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                        players[TURN_PLAYER_1 MINUS_ONE].name_);
              printf("---------------------------------------------------------------\n");
              break;
            }
            
          default:
            break;
        }     // should be fine

        // player 2 -----------------------------------------------------------------------------------------
        
        i = 1;
        
        // rewind bock
        if (hands[TURN_PLAYER_2 MINUS_ONE][0].is_bock_ == TRUE) // hard coded 0 sollte passen
                                                            // zumal: ein bock: alle bock
        {
          for (counter_cards = 0; counter_cards < HAND; counter_cards++)
          {
            hands[TURN_PLAYER_2 MINUS_ONE][counter_cards].is_bock_ = FALSE;
          }
        }

        // check position(s) of valid cards for this round
        // do not allow any other card
        // delete non valid characters from commands
        count_bock    = 0;   // bock
        count_suit    = 0;   // right suit
        count_trump   = 0;   // trump
        
        // seek
        for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
             counter_cards++)
        {
          // have trump
          if ((hands)[TURN_PLAYER_2 MINUS_ONE][counter_cards].is_trump_ == TRUE)
          {
            count_trump++;
          }
        }
        
        for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
             counter_cards++)
        {
          // right suit
          if (i == 1)
          {
            if ((hands)[TURN_PLAYER_2 MINUS_ONE][counter_cards].suit_
                == (hands)[TURN_PLAYER_1 MINUS_ONE][position[i - 1]].suit_)   // ... position
            {
              // rigtht suit is trump (: trump has been played )
              if (hands[TURN_PLAYER_2 MINUS_ONE][counter_cards].is_trump_ == TRUE)
              {
                count_trump--;
              }
              count_suit++;
            }
          }
          
          else
          {
            if ((hands)[TURN_PLAYER_2 MINUS_ONE][counter_cards].suit_
                == (hands)[TURN_PLAYER_2 - 2 MINUS_ONE][position[i - 2]].suit_)   // .. position
            {
              // rigtht suit is trump (: trump has been played )
              if (hands[TURN_PLAYER_2 MINUS_ONE][counter_cards].is_trump_ == TRUE)
              {
                count_trump--;
              }
              count_suit++;
            }
          }
        }
        
        for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
             counter_cards++)
        {
          // no trump && no suit = all bock
          if (i == 1)
          {
            if ((hands)[TURN_PLAYER_2 MINUS_ONE][counter_cards].is_trump_ == FALSE
                && (hands)[TURN_PLAYER_2 MINUS_ONE][counter_cards].suit_
                != (hands)[TURN_PLAYER_2 - 1 MINUS_ONE][position[i - 1]].suit_)   // ... position
            {
              count_bock++;
            }
          }
          
          else
          {
            if ((hands)[TURN_PLAYER_2 MINUS_ONE][counter_cards].is_trump_ == FALSE
                && (hands)[TURN_PLAYER_2 MINUS_ONE][counter_cards].suit_
                != (hands)[TURN_PLAYER_2 MINUS_ONE][position[i - 2]].suit_)       // ... position
            {
              count_bock++;
            }
          }
        }
        
        // .. and adjust ------------
        strcpy(players_commands[i], "000000");    // maybe there is a better
                                                  // more professinal way
        
        // all bock
        counter_trump   = 0;
        counter_suit    = 0;
        counter_cards   = 0;
        counter_hand    = 0;
        counter_command = 0;
        
        // all bock
        if ((count_suit == 0 && count_trump == 0))
        {
          // according to turn, -1
//          printf("Alles Bock!\n");
//          players_commands[i] = commands;
//          players_commands[i][counter_cards] = '\0';
          while (counter_hand < (HAND))
          {
            if ((hands)[TURN_PLAYER_2 MINUS_ONE][counter_hand].suit_ != NULL)
            {
              players_commands[i][counter_cards] = commands[counter_command];
              counter_command++;
              counter_cards++;
            }
            
            else
            {
              counter_command++;
            }
            
            counter_hand++;
          }
        }
        
        // no valid suit
        else if (count_suit == 0 && count_trump > 0)
        {
          counter_trump   = 0;
          counter_suit    = 0;
          counter_cards   = 0;
          counter_hand    = 0;
          counter_command = 0;
          // must hold trump
          // allow trump(s)
//          printf("Muaßt an Trumpf spiel'n!\n");

          while (counter_hand < (HAND/* - counter_turns*/)/* eben nicht strlen(players_commands[i])*/)
          {
            if ((hands)[TURN_PLAYER_2 MINUS_ONE][counter_hand].suit_ != NULL
                && hands[TURN_PLAYER_2 MINUS_ONE][counter_hand].is_trump_ == TRUE)
            {
              players_commands[i][counter_cards] = commands[counter_command];
              counter_command++;
              counter_cards++;
            }
            
            else
            {
              counter_command++;
            }
            
            counter_hand++;
          }
          
          // remove right
          for (counter_cards = count_trump; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
          {
            players_commands[i][counter_cards] = '\0';
          }
        }
        
        else /* must hold suit, at leas one */
        {
//          printf("Muaßt Farb' spielen!\n");
          counter_cards = 0;
          counter_command = 0;
          
          // remove left
          for (counter_hand = 0; counter_hand < (HAND/* - counter_turns*/); counter_hand++)
          {
            // suit found
            if (i == 1)   // first to answer
            {
              if ((hands)[TURN_PLAYER_2 MINUS_ONE][counter_hand].suit_
                  == (hands)[TURN_PLAYER_2 - 1 MINUS_ONE][position[i - 1]].suit_)
              {
//                printf("gleiche Farbe @ %d\n", counter_hand);
//                printf("%c = %c\n", players_commands[i][counter_hand], commands[counter_command]);
                players_commands[i][counter_cards] = commands[counter_command];
                counter_command++;
                counter_cards++;
              }
              else
              {
                counter_command++;
              }
            }
            
            else    // second to answer
            {
              if ((hands)[TURN_PLAYER_2 MINUS_ONE][counter_hand].suit_
                  == (hands)[TURN_PLAYER_2 - 2 MINUS_ONE][position[i - 2]].suit_)
              {
//                printf("gleiche Farbe @ %d\n", counter_hand);
                players_commands[i][counter_cards] = commands[counter_command];
                counter_command++;
                counter_cards++;
              }
              else
              {
//                players_commands[i][counter_hand] = commands[counter_command];
                counter_command++;
              }
            }
          }
          
          // remove right
          for (counter_cards = count_suit; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
          {
            players_commands[i][counter_cards] = '\0';
          }
        }
          
        // ------------- copied from modeGame() -----------------------

        // if there is at least one valid card to answer with
        if (!(count_suit == 0 && count_trump == 0))
        {
          // compare values and only allow weaker cards if there is not one
          // which could trump call
          counter_command = 0;
          counter_cards   = 0;
          count_permit    = 0;

          if (count_suit > 0)
          {
            while (players_commands[/*i*/ TURN_PLAYER_2 MINUS_ONE][counter_command] != '\0')
            {
//              if ((hands)[player[i] MINUS_ONE][counter_cards].value_ >
//                  /* called card */ hands[player[0] MINUS_ONE][position[0]].value_)
              if ((hands)[TURN_PLAYER_2 MINUS_ONE][counter_cards].value_ >
                  /* called card */ hands[TURN_PLAYER_1 MINUS_ONE][position[0]].value_)
              {
                buffer_higher[counter_cards] = players_commands[/*i*/ TURN_PLAYER_2 MINUS_ONE][counter_cards];
                buffer_higher[counter_cards ADD_ONE] = '\0';
                counter_cards++;
              }
              counter_command++;
            }
          }
          
          else if (count_trump > 0)
          {
            while (players_commands[TURN_PLAYER_2 MINUS_ONE][counter_command] != '\0')
            {
              if (hands[TURN_PLAYER_2 MINUS_ONE][position[1]].is_trump_)
              {
                buffer_higher[counter_cards] = players_commands[TURN_PLAYER_3 MINUS_ONE][counter_cards];
                buffer_higher[counter_cards ADD_ONE] = '\0';
                counter_cards++;
              }
              counter_command++;
            }
          }

          if (counter_cards > 0)
          {
            strcpy(players_commands[/*i*/ TURN_PLAYER_2 MINUS_ONE], "\0\0\0\0\0\0");
            strcpy(players_commands[/*i*/ TURN_PLAYER_2 MINUS_ONE], buffer_higher);
            strcpy(buffer_higher, "\0\0\0\0\0\0");
          }
        }
        // ------------- end of copy ---------------------------------------------------
        
        printHand(hands[TURN_PLAYER_2 MINUS_ONE], HAND, TURN_PLAYER_2,
                  players[TURN_PLAYER_2 MINUS_ONE].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        
        // highlight 20 & 40
        if (i == 0)
        {
//          highlight(hands[player[i] MINUS_ONE], player[i]);
          pairs = highlight(hands[TURN_PLAYER_2 MINUS_ONE], TURN_PLAYER_2, position_Q);
          printf("TEST: %d pairs with first Queen @ %d\n", pairs, position_Q[0]);
        }
        
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          buffer = getchar();
          printf("\r");
          if (!(check = seekAndDestroy(buffer, players_commands[TURN_PLAYER_2 - 1])))
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (!check);
        system ("/bin/stty cooked");
        
        switch (buffer)   // does not need to be changed, since it is an answer
        {
          case 'q':
            position[TURN_PLAYER_2 - 1] = 0;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][position[TURN_PLAYER_2 - 1]]);
            printHand((hands)[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'w':
            position[TURN_PLAYER_2 - 1] = 1;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][position[TURN_PLAYER_2 - 1]]);
            printHand((hands)[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'e':
            position[TURN_PLAYER_2 - 1] = 2;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][position[TURN_PLAYER_2 - 1]]);
            printHand((hands)[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'a':
            position[TURN_PLAYER_2 - 1] = 3;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][position[TURN_PLAYER_2 - 1]]);
            printHand((hands)[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 's':
            position[TURN_PLAYER_2 - 1] = 4;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][position[TURN_PLAYER_2 - 1]]);
            printHand((hands)[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'd':
            position[TURN_PLAYER_2 - 1] = 5;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][position[TURN_PLAYER_2 - 1]]);
            printHand((hands)[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          default:
            break;
        }
        
        // player 3
        
        i = 2;
        
        // rewind bock
        if (hands[TURN_PLAYER_3 MINUS_ONE][0].is_bock_ == TRUE) // hard coded 0 sollte passen
                                                            // zumal: ein bock: alle bock
        {
          for (counter_cards = 0; counter_cards < HAND; counter_cards++)
          {
            hands[TURN_PLAYER_3 MINUS_ONE][counter_cards].is_bock_ = FALSE;
          }
        }

        // check position(s) of valid cards for this round
        // do not allow any other card
        // delete non valid characters from commands
        count_bock    = 0;   // bock
        count_suit    = 0;   // right suit
        count_trump   = 0;   // trump
        
        // seek
        for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
             counter_cards++)
        {
          // have trump
          if ((hands)[TURN_PLAYER_3 MINUS_ONE][counter_cards].is_trump_ == TRUE)
          {
//            if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_)
            count_trump++;
          }
        }
        
        for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
             counter_cards++)
        {
          // right suit
          if (i == 1)
          {
            if ((hands)[TURN_PLAYER_3 MINUS_ONE][counter_cards].suit_
                == (hands)[TURN_PLAYER_3 - 1 MINUS_ONE][position[i - 1]].suit_)
            {
              // rigtht suit is trump (: trump has been played )
              if (hands[TURN_PLAYER_3 MINUS_ONE][counter_cards].is_trump_ == TRUE)
              {
                count_trump--;
              }
              count_suit++;
            }
          }
          
          else
          {
            if ((hands)[TURN_PLAYER_3 MINUS_ONE][counter_cards].suit_
                == (hands)[TURN_PLAYER_3 - 2 MINUS_ONE][position[i - 2]].suit_)
            {
              // rigtht suit is trump (: trump has been played )
              if (hands[TURN_PLAYER_3 MINUS_ONE][counter_cards].is_trump_ == TRUE)
              {
                count_trump--;
              }
              count_suit++;
            }
          }
        }
        
        for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
             counter_cards++)
        {
          // no trump && no suit = all bock
          if (i == 1)
          {
            if ((hands)[TURN_PLAYER_3 MINUS_ONE][counter_cards].is_trump_ == FALSE
                && (hands)[TURN_PLAYER_3 MINUS_ONE][counter_cards].suit_
                != (hands)[TURN_PLAYER_3 - 1 MINUS_ONE][position[i - 1]].suit_)
            {
              count_bock++;
            }
          }
          
          else
          {
            if ((hands)[TURN_PLAYER_3 MINUS_ONE][counter_cards].is_trump_ == FALSE
                && (hands)[TURN_PLAYER_3 MINUS_ONE][counter_cards].suit_
                != (hands)[TURN_PLAYER_3 - 2 MINUS_ONE][position[i - 2]].suit_)
            {
              count_bock++;
            }
          }
        }
        
        // .. and adjust
        strcpy(players_commands[i], "000000");    // maybe there is a better
                                                  // more professinal/elegant way
        
        // all bock
        counter_trump   = 0;
        counter_suit    = 0;
        counter_cards   = 0;
        counter_hand    = 0;
        counter_command = 0;
        
        // all bock
        if ((count_suit == 0 && count_trump == 0))
        {
          // according to turn, -1
//          printf("Alles Bock!\n");
//          players_commands[i] = commands;
//          players_commands[i][counter_cards] = '\0';
          while (counter_hand < (HAND))
          {
            if ((hands)[TURN_PLAYER_3 MINUS_ONE][counter_hand].suit_ != NULL)
            {
              players_commands[i][counter_cards] = commands[counter_command];
              counter_command++;
              counter_cards++;
            }
            
            else
            {
              counter_command++;
            }
            
            counter_hand++;
          }
        }
        
        // no valid suit
        else if (count_suit == 0 && count_trump > 0)
        {
          counter_trump   = 0;
          counter_suit    = 0;
          counter_cards   = 0;
          counter_hand    = 0;
          counter_command = 0;
          // must hold trump
          // allow trump(s)
//          printf("Muaßt an Trumpf spiel'n!\n");

          while (counter_hand < (HAND/* - counter_turns*/)/* eben nicht strlen(players_commands[i])*/)
          {
            if ((hands)[TURN_PLAYER_3 MINUS_ONE][counter_hand].suit_ != NULL
                && hands[TURN_PLAYER_3 MINUS_ONE][counter_hand].is_trump_ == TRUE)
            {
              players_commands[i][counter_cards] = commands[counter_command];
              counter_command++;
              counter_cards++;
            }
            
            else
            {
              counter_command++;
            }
            
            counter_hand++;
          }
          
          // remove right
          for (counter_cards = count_trump; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
          {
            players_commands[i][counter_cards] = '\0';
          }
        }
        
        else /* must hold suit, at leas one */
        {
//          printf("Muaßt Farb' spielen!\n");
          counter_cards = 0;
          counter_command = 0;
          
          // remove left
          for (counter_hand = 0; counter_hand < (HAND/* - counter_turns*/); counter_hand++)
          {
            // suit found
            if (i == 1)   // first to answer
            {
              if ((hands)[TURN_PLAYER_3 MINUS_ONE][counter_hand].suit_
                  == (hands)[TURN_PLAYER_3 - 1 MINUS_ONE][position[i - 1]].suit_)
              {
//                printf("gleiche Farbe @ %d\n", counter_hand);
//                printf("%c = %c\n", players_commands[i][counter_hand], commands[counter_command]);
                players_commands[i][counter_cards] = commands[counter_command];
                counter_command++;
                counter_cards++;
              }
              else
              {
                counter_command++;
              }
            }
            
            else    // second to answer
            {
              if ((hands)[TURN_PLAYER_3 MINUS_ONE][counter_hand].suit_
                  == (hands)[TURN_PLAYER_3 - 2 MINUS_ONE][position[i - 2]].suit_)
              {
//                printf("gleiche Farbe @ %d\n", counter_hand);
                players_commands[i][counter_cards] = commands[counter_command];
                counter_command++;
                counter_cards++;
              }
              else
              {
//                players_commands[i][counter_hand] = commands[counter_command];
                counter_command++;
              }
            }
          }
          
          // remove right
          for (counter_cards = count_suit; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
          {
            players_commands[i][counter_cards] = '\0';
          }
        }
        
        // ------------- copied from modeGame() -----------------------

        /* if (i == 2) */
        // if there is at least one valid card to answer with
        if (!(count_suit == 0 && count_trump == 0))
        {
          // compare values and only allow weaker cards if there is not one
          // which could trump any (call || answer 1)
          counter_command = 0;
          counter_cards   = 0;
          count_permit    = 0;

          if (count_suit > 0)
          {
            while (players_commands[i][counter_command] != '\0')
            {
              if (!(hands[TURN_PLAYER_1 MINUS_ONE][position[0]].suit_
                  == hands[TURN_PLAYER_2 MINUS_ONE][position[1]].suit_
                  && hands[TURN_PLAYER_1 MINUS_ONE][position[0]].value_
                  > hands[TURN_PLAYER_2 MINUS_ONE][position[1]].value_))
              {
                // answer 2 must only trump call
                if ((hands)[TURN_PLAYER_3 MINUS_ONE][counter_cards].value_ >
                      /* called card */ hands[TURN_PLAYER_1 MINUS_ONE][position[0]].value_)
                {
                  buffer_higher[counter_cards] = players_commands[/*i*/ TURN_PLAYER_3 MINUS_ONE][counter_cards];
                  buffer_higher[counter_cards ADD_ONE] = '\0';
                  counter_cards++;
                }
              }
              counter_command++;
            }
          }

          else if (count_trump > 0)
          {
            if (hands[TURN_PLAYER_2 MINUS_ONE][position[TURN_PLAYER_2 MINUS_ONE]].is_trump_)
            {
              while (players_commands[TURN_PLAYER_3 MINUS_ONE][counter_command] != '\0')
              {
                // only need to compare trump's value with answer 1
                if (hands[TURN_PLAYER_3 MINUS_ONE][counter_cards].value_ >
                    /* answer 1's card */ hands[TURN_PLAYER_2 MINUS_ONE][position[1]].value_)
                {
                  buffer_higher[counter_cards] = players_commands[TURN_PLAYER_3 MINUS_ONE][counter_cards];
                  buffer_higher[counter_cards ADD_ONE] = '\0';
                  counter_cards++;
                }
                counter_command++;
              }
            }
          }

          if (counter_cards > 0)
          {
            strcpy(players_commands[TURN_PLAYER_3 MINUS_ONE], "\0\0\0\0\0\0");
            strcpy(players_commands[TURN_PLAYER_3 MINUS_ONE], buffer_higher);
            strcpy(buffer_higher, "\0\0\0\0\0\0");
          }
        }
        // ------------- end of copy --------------------------------------------
        
        printHand(hands[TURN_PLAYER_3 MINUS_ONE], HAND, TURN_PLAYER_3,
                  players[TURN_PLAYER_3 MINUS_ONE].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        
        // highlight 20 & 40
        if (i == 0)
        {
          pairs = highlight(hands[TURN_PLAYER_3 MINUS_ONE], TURN_PLAYER_3, position_Q);
          printf("TEST: %d pairs with first Queen @ %d\n", pairs, position_Q[0]);
        }
        
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          buffer = getchar();
          printf("\r");
          if (!(check = seekAndDestroy(buffer, players_commands[TURN_PLAYER_3 - 1])))
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (!check);
        system ("/bin/stty cooked");
        
        switch (buffer)     // hm .. don't knwo why I check 20 or 40 here,
                            // since pair will always be 0 at this point
                            // I think. Therefore I do not change it according
                            // to points_pair (yet). I might delete the checks
                            // in post
        {
          case 'q':
//            position[i] = 0;
            position[TURN_PLAYER_3 MINUS_ONE] = 0;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // at this position we will only find a KING
              if (position[i] == position_Q[0] MINUS_ONE
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
//                  printf("K 40?\n");
                }
                // 20
                else /* is_trump == FALSE */
                {
//                  printf("K 20?\n");
                }
              }
            }
            
//            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i]);
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position[TURN_PLAYER_3 MINUS_ONE]]);
            printHand((hands)[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'w':
//            position[i] = 1;
            position[TURN_PLAYER_3 MINUS_ONE] = 1;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // second card = QUEEN
              if (position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
//                  printf("Q 40?\n");
                }
                // 20
                else /* is_trump == FALSE */
                {
//                  printf("Q 20?\n");
                }
              }
              // second card = KING
              else if (position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
//                  printf("K 40?\n");
                }
                // 20
                else /* is_trump == FALSE */
                {
//                  printf("K 20?\n");
                }
              }
            }
            
//            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i]);
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position[TURN_PLAYER_3 MINUS_ONE]]);
            printHand((hands)[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'e':
//            position[i] = 2;
            position[TURN_PLAYER_3 MINUS_ONE] = 2;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // third card = QUEEN
              if (position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
//                  printf("Q 40?\n");
                }
                // 20
                else /* is_trump == FALSE */
                {
//                  printf("Q 20?\n");
                }
              }
              // third card = KING
              else if ((position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                       || (position[i] == position_Q[1] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
//                  printf("K 40?\n");
                }
                // 40
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                {
//                  printf("K 40?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                {
//                  printf("K 20?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                {
//                  printf("K 20?\n");
                }
              }
            }
            
//            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i]);
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position[TURN_PLAYER_3 MINUS_ONE]]);
            printHand((hands)[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'a':
//            position[i] = 3;
            position[TURN_PLAYER_3 MINUS_ONE] = 3;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // fourth card = QUEEN
              if ((position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  || ((position[i] == position_Q[1]
                       && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)))
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
//                  printf("Q 40?\n");
                }
                // 40
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                {
//                  printf("Q 40?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                {
//                  printf("Q 20?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                {
//                  printf("Q 20?\n");
                }
              }
              // fourth card = KING
              else if ((position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                       || (position[i] == position_Q[1] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
              {
                if (position_Q[0] > position[0])  // first 20 or 40
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
//                    printf("K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                  {
//                    printf("K 20?\n");
                  }
                }
                
                else /* if (position_Q[0] < position[0]) */ // second 20 or 40
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                  {
//                    printf("K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                  {
//                    printf("K 20?\n");
                  }
                }
              }
            }
            
//            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i]);
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position[TURN_PLAYER_3 MINUS_ONE]]);
            printHand((hands)[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 's':
//            position[i] = 4;
            position[TURN_PLAYER_3 MINUS_ONE] = 4;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // fourth card = QUEEN
              if ((position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  || ((position[i] == position_Q[1]
                       && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)))
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
//                  printf("Q 40?\n");
                }
                // 40
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                {
//                  printf("Q 40?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                {
//                  printf("Q 20?\n");
                }
                // 20
                else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                {
//                  printf("Q 20?\n");
                }
              }
              // fourth card = KING
              else if ((position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                       || (position[i] == position_Q[1] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
              {
                if (position_Q[0] > position[0])  // first 20 or 40
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                  {
//                    printf("K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                  {
//                    printf("K 20?\n");
                  }
                }
                
                else /* if (position_Q[0] < position[0]) */ // second 20 or 40
                {
                  // 40
                  if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                  {
//                    printf("K 40?\n");
                  }
                  // 20
                  else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                  {
//                    printf("K 20?\n");
                  }
                }
              }
            }
            
//            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i]);
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position[TURN_PLAYER_3 MINUS_ONE]]);
            printHand((hands)[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'd':
//            position[i] = 5;
            position[TURN_PLAYER_3 MINUS_ONE] = 5;
            
            // check for 20 or 40
            if (pairs > 0)
            {
              // at this position we will only find a QUEEN
              if ((position[i] == position_Q[0]
                   && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  || (position[i] == position_Q[1]
                      && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)
                  || (position[i] == position_Q[2]
                      && hands[player[i] MINUS_ONE][position_Q[2]].value_ == QUEEN))
              {
                // 40
                if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                {
//                  printf("Q 40?\n");
                }
                // 20
                else /* is_trump == FALSE */
                {
//                  printf("Q 20?\n");
                }
              }
            }
            
//            playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i]);
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position[TURN_PLAYER_3 MINUS_ONE]]);
            printHand((hands)[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          default:
            break;
        }
        
        i = 0;
        
        nextAndPoints(&start, buffer_start, hands[TURN_PLAYER_1 MINUS_ONE][position[i]],
                      (hands)[TURN_PLAYER_2 MINUS_ONE][position[i + 1]],
                      (hands)[TURN_PLAYER_3 MINUS_ONE][position[i + 2]],
                      &points_call, &points_opponents,
                      &points[0], &points[1],
                      &points[2], turn);
        
        // calling player is not next to call and therefore lost
        if (buffer_start != start)
        {
          break;
        }
        
        else
        {
          // naja .. ist irgendwie hinfällig
          if (buffer_start == TURN_PLAYER_1)
          {
            points_call       += points[TURN_PLAYER_1 MINUS_ONE];
            points_opponents  += points[TURN_PLAYER_2 MINUS_ONE]
                                  + points[TURN_PLAYER_3 MINUS_ONE];
            
            if (points_call > 0)
            {
              points_call += points_pair;
            }
          }
          
          else if (buffer_start == TURN_PLAYER_2)
          {
            points_call       += points[TURN_PLAYER_2 MINUS_ONE];
            points_opponents  += points[TURN_PLAYER_1 MINUS_ONE]
                                  + points[TURN_PLAYER_3 MINUS_ONE];
            
            if (points_call > 0)
            {
              points_call += points_pair;
            }
          }
          
          else if (buffer_start == TURN_PLAYER_3)
          {
            points_call       += points[TURN_PLAYER_3 MINUS_ONE];
            points_opponents  += points[TURN_PLAYER_1 MINUS_ONE]
                                  + points[TURN_PLAYER_2 MINUS_ONE];
            
            if (points_call > 0)
            {
              points_call += points_pair;
            }
          }
        }
        
        points[0] = 0;
        points[1] = 0;
        points[2] = 0;
        pairs = resetPairs(position_Q);
        resetHandlePairs(handle_pairs);
        points_pair = 0;
        
        
        if (points_call >= 66)
        {
          break;
        }
        
        // may we do this here -----------------------
        removeCard(&hands[TURN_PLAYER_1 MINUS_ONE][position[i]]);
        removeCard(&hands[TURN_PLAYER_2 MINUS_ONE][position[i + 1]]);
        removeCard(&hands[TURN_PLAYER_3 MINUS_ONE][position[i + 2]]);
        // -------------------------------------------
        // write all left over cards onto commands

        // player 1
        counter_position = 0 ;
        for (counter_cards = 0; counter_cards < HAND; counter_cards++)
        {
          if ((hands)[player[i] MINUS_ONE][counter_cards].suit_ != NULL)
          {
            players_commands[i][counter_position] = commands[counter_cards];
            counter_position++;
          }
        }
        players_commands[i][counter_position] = '\0';
        
        // player 2
        counter_position = 0 ;
        for (counter_cards = 0; counter_cards < HAND; counter_cards++)
        {
          if ((hands)[player[i + 1] MINUS_ONE][counter_cards].suit_ != NULL)
          {
            players_commands[i + 1][counter_position] = commands[counter_cards];
            counter_position++;
          }
        }
        players_commands[i + 1][counter_position] = '\0';
        
        // player 3
        counter_position = 0 ;
        for (counter_cards = 0; counter_cards < HAND; counter_cards++)
        {
          if ((hands)[player[i + 2] MINUS_ONE][counter_cards].suit_ != NULL)
          {
            players_commands[i + 2][counter_position] = commands[counter_cards];
            counter_position++;
          }
        }
        players_commands[i + 2][counter_position] = '\0';
        
        // sort players' order .. ob I mir das nicht sparen kann ..
        getCall(start, &call, &answer_1, &answer_2);
        
        player[0] = call;
        player[1] = answer_1;
        player[2] = answer_2;
        
        
        if (player[0] == TURN_PLAYER_1)
        {
          players_commands[0] = commands_1;
          players_commands[1] = commands_2;
          players_commands[2] = commands_3;
        }
        
        if (player[0] == TURN_PLAYER_2)
        {
          players_commands[0] = commands_2;
          players_commands[1] = commands_3;
          players_commands[2] = commands_1;
        }
        
        if (player[0] == TURN_PLAYER_3)
        {
          players_commands[0] = commands_3;
          players_commands[1] = commands_1;
          players_commands[2] = commands_2;
        }
        
        turn[0]   = call;
        turn[1]   = answer_1;
        turn[2]   = answer_2;
        
        // print points
        printf("player:  %d\nopponents:  %d\n", points_call, points_opponents);
        
        // calling player must not lose one round
        if (start != buffer_start)
        {
          printf("FOLD!\n");
          printf("---------------------------------------------------------------\n");
          points_and_caller.points_ = 12;
          // ganz dumm, weil I an leichten Flieger im Flieger hab gerade
          if (buffer_start == TURN_PLAYER_1)
          {
            points_and_caller.winner_ = OPPONENTS_WIN;
            points_and_caller.caller_ = TURN_PLAYER_1;
          }
          else if (buffer_start == TURN_PLAYER_2)
          {
            points_and_caller.winner_ = OPPONENTS_WIN;
            points_and_caller.caller_ = TURN_PLAYER_2;
          }
          else /* if (buffer_start == TURN_PLAYER_3) */
          {
            points_and_caller.winner_ = OPPONENTS_WIN;
            points_and_caller.caller_ = TURN_PLAYER_3;
          }
          break;
          return points_and_caller;
        }
      } // end if (CONTRA: first) .. sollte soweit passen -----------------
      
      // following turns -------------------------------------------------------------
      else
      {
        // 3 cards each round
        for (i = 0; i < 3; i++)
        {
          printHand(hands[player[i] MINUS_ONE], HAND, turn[i],
                    players[player[i] MINUS_ONE].name_);
          printf("                         ^     ^     ^     ^     ^     ^  \n");
          printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
          
          // highlight 20 & 40
          if (i == 0)
          {
            pairs = highlight(hands[player[i] MINUS_ONE],
                              player[i] /* vielleicht doch turn[i] */, position_Q);
          
            printf("TEST: %d pairs with first Queen @ %d\n", pairs, position_Q[0]);
          }
//          pairs = resetPairs(position_Q);
          
          printf("---------------------------------------------------------------\n");
          
          // first to call
          if (i == 0)
          {
            do
            {
              system ("/bin/stty raw");
              buffer = getchar();
              printf("\r");
              if (!(check = seekAndDestroy(buffer, players_commands[i])))
                printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
            } while (!check);
            system ("/bin/stty cooked");
            
            switch (buffer)
            {
              case 'q':
                position[i] = 0;
//                position[TURN_PLAYER_3 MINUS_ONE] = 0;
                
                // check for 20 or 40
                if (pairs > 0)
                {
                  // at this position we will only find a KING
                  if (position[i] == position_Q[0] MINUS_ONE
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[0] MINUS_ONE,
                                                               points_pair, &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 40?\n");
                        pairs--;
                        // sollte ich hier nun auch theoretische positions_Q
                        // "hochrutschen"? - 1 auf 0, 2 auf 1
                        //
                        // Ja sicher!
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
//                      printf("check K 40?\n");
                    }
                    // 20
                    else /* is_trump == FALSE */
                    {
                      points_pair = 20;
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[0],
                                                               points_pair,
                                                               &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 20?\n");
                        pairs--;
                        // sollte ich hier nun auch theoretische positions_Q
                        // "hochrutschen"? - 1 auf 0, 2 auf 1
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
//                      printf("check K 20?\n");
                    }
                  }
                }
                
                if (i == ENOUGH)
                {
                  break;
                }
                
                else
                {
                  playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                  printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                            players[player[i] MINUS_ONE].name_);
                  printf("---------------------------------------------------------------\n");
                  break;
                }
                
              case 'w':
                position[i] = 1;
//                position[TURN_PLAYER_3 MINUS_ONE] = 1;
                
                // check for 20 or 40
                if (pairs > 0)
                {
                  // second card = QUEEN
                  if (position[i] == position_Q[0]
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[0],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
//                      printf("check Q 40?\n");
                    }
                    // 20
                    else /* is_trump == FALSE */
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[0],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 20?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
//                      printf("check Q 20?\n");
                    }
                  }
                  // second card = KING
                  else if (position[i] == position_Q[0] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[0] MINUS_ONE,
                                                               points_pair, &i,
                                                               &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
//                      printf("check K 40?\n");
                    }
                    // 20
                    else /* is_trump == FALSE */
                    {
                      points_pair = 20;
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[0],
                                                               points_pair,
                                                               &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 20?\n");
                        pairs--;
                        // sollte ich hier nun auch theoretische positions_Q
                        // "hochrutschen"? - 1 auf 0, 2 auf 1
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
//                      printf("check K 20?\n");
                    }
                  }
                }
                
                if (i == ENOUGH)
                {
                  break;
                }
                
                else
                {
                  playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                  printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                            players[player[i] MINUS_ONE].name_);
                  printf("---------------------------------------------------------------\n");
                  break;
                }
                
              case 'e':
                position[i] = 2;
//                position[TURN_PLAYER_3 MINUS_ONE] = 2;
                
                // check for 20 or 40
                if (pairs > 0)
                {
                  // third card = QUEEN
                  if (position[i] == position_Q[0]
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[0],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 40?\n");
                    }
                    // 20
                    else /* is_trump == FALSE */
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[0],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 20?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 20?\n");
                    }
                  }
                  // third card = KING
                  else if ((position[i] == position_Q[0] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                           || (position[i] == position_Q[1] MINUS_ONE
                               && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[0] MINUS_ONE,
                                                               points_pair, &i,
                                                               &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
                      
//                      printf("check K 40?\n");
                    }
                    // 40
                    else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[1] MINUS_ONE,
                                                               points_pair, &i,
                                                               &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];                            // naja, hardcoded
                        position_Q[1] = position_Q[2];                            // oba viel besseres
                        position_Q[2] = 0;                                        // fällt mir hier nit ein
                      }
                      
//                      printf("check K 40?\n");
                    }
                    // 20
                    else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[0],
                                                               points_pair,
                                                               &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 20?\n");
                        pairs--;
                        // sollte ich hier nun auch theoretische positions_Q
                        // "hochrutschen"? - 1 auf 0, 2 auf 1
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check K 20?\n");
                    }
                    // 20
                    else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                               position_Q[1],
                                                               points_pair,
                                                               &i, &points_call,
                                                               &points_opponents,
                                                               hands[player[i] MINUS_ONE],
                                                               (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("K 20?\n");
                        pairs--;
                        // sollte ich hier nun auch theoretische positions_Q
                        // "hochrutschen"? - 1 auf 0, 2 auf 1
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check K 20?\n");
                    }
                  }
                }
                
                if (i == ENOUGH)
                {
                  break;
                }
                
                else
                {
                  playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                  printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                            players[player[i] MINUS_ONE].name_);
                  printf("---------------------------------------------------------------\n");
                  break;
                }
                
              case 'a':
                position[i] = 3;
//                position[TURN_PLAYER_3 MINUS_ONE] = 3;
                
                // check for 20 or 40
                if (pairs > 0)
                {
                  // fourth card = QUEEN
                  if ((position[i] == position_Q[0]
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                      || ((position[i] == position_Q[1]
                           && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)))
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[0],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 40?\n");
                    }
                    // 40
                    else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[1],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 40?\n");
                    }
                    // 20
                    else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[0],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 20?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 20?\n");
                    }
                    // 20
                    else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[1],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 20?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 20?\n");
                    }
                  }
                  // fourth card = KING
                  else if ((position[i] == position_Q[0] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                           || (position[i] == position_Q[1] MINUS_ONE
                               && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN))
                  {
                    if (position_Q[0] > position[0])  // first 20 or 40
                    {
                      // 40
                      if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                      {
                        points_pair = 40;
                        
                        handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                                 position_Q[0] MINUS_ONE,
                                                                 points_pair, &i, &points_call,
                                                                 &points_opponents,
                                                                 hands[player[i] MINUS_ONE],
                                                                 (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("K 40?\n");
                          
                          pairs--;
                          
                          position_Q[0] = position_Q[1];                            // naja, hardcoded
                          position_Q[1] = position_Q[2];                            // oba viel besseres
                          position_Q[2] = 0;                                        // fällt mir hier nit ein
                        }
                        
//                        printf("check K 40?\n");
                      }
                      // 20
                      else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                      {
                        points_pair = 20;
                        
                        handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                                 position_Q[0],
                                                                 points_pair,
                                                                 &i, &points_call,
                                                                 &points_opponents,
                                                                 hands[player[i] MINUS_ONE],
                                                                 (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("K 20?\n");
                          pairs--;
                          // sollte ich hier nun auch theoretische positions_Q
                          // "hochrutschen"? - 1 auf 0, 2 auf 1
                          position_Q[0] = position_Q[1];
                          position_Q[1] = position_Q[2];
                          position_Q[2] = 0;
                        }
                        
//                        printf("check K 20?\n");
                      }
                    }
                    
                    else /* if (position_Q[0] < position[0]) */ // second 20 or 40
                    {
                      // 40
                      if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                      {
                        points_pair = 40;
                        
                        handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                                 position_Q[1] MINUS_ONE,
                                                                 points_pair, &i, &points_call,
                                                                 &points_opponents,
                                                                 hands[player[i] MINUS_ONE],
                                                                 (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("K 40?\n");
                          
                          pairs--;
                          
                          position_Q[0] = position_Q[1];                            // naja, hardcoded
                          position_Q[1] = position_Q[2];                            // oba viel besseres
                          position_Q[2] = 0;                                        // fällt mir hier nit ein
                        }
                        
//                        printf("check K 40?\n");
                      }
                      // 20
                      else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                      {
                        points_pair = 20;
                        
                        handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                                 position_Q[1],
                                                                 points_pair,
                                                                 &i, &points_call,
                                                                 &points_opponents,
                                                                 hands[player[i] MINUS_ONE],
                                                                 (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("K 20?\n");
                          pairs--;
                          // sollte ich hier nun auch theoretische positions_Q
                          // "hochrutschen"? - 1 auf 0, 2 auf 1
                          position_Q[0] = position_Q[1];
                          position_Q[1] = position_Q[2];
                          position_Q[2] = 0;
                        }
                        
//                        printf("check K 20?\n");
                      }
                    }
                  }
                }
                
                if (i == ENOUGH)
                {
                  break;
                }
                
                else
                {
                  playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                  printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                            players[player[i] MINUS_ONE].name_);
                  printf("---------------------------------------------------------------\n");
                  break;
                }
                
              case 's':
                position[i] = 4;
//                position[TURN_PLAYER_3 MINUS_ONE] = 4;
                
                // check for 20 or 40
                if (pairs > 0)
                {
                  // fifth card = QUEEN
                  if ((position[i] == position_Q[0]
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                      || ((position[i] == position_Q[1]
                           && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)))
                  {
                    // 40
                    if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[0],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 40?\n");
                    }
                    // 40
                    else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                    {
                      points_pair = 40;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[1],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 40?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 40?\n");
                    }
                    // 20
                    else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[0],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 20?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 20?\n");
                    }
                    // 20
                    else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                    {
                      points_pair = 20;
                      
                      handle_pairs[i].bool_pair_
                        = pairHandler(handle_pairs, position_Q[1],
                                      points_pair, &i, &points_call,
                                      &points_opponents,
                                      hands[player[i] MINUS_ONE],
                                      (player[0] == buffer_start));
                      if (handle_pairs[i].bool_pair_ == TRUE)
                      {
//                        printf("Q 20?\n");
                        
                        pairs--;
                        
                        position_Q[0] = position_Q[1];
                        position_Q[1] = position_Q[2];
                        position_Q[2] = 0;
                      }
                      
//                      printf("check Q 20?\n");
                    }
                  }
                  // fifth card = KING
                  else if ((position[i] == position_Q[0] MINUS_ONE
                           && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                           || (position[i] == position_Q[1] MINUS_ONE
                               && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)
                           || (position[i] == position_Q[2] MINUS_ONE
                               && hands[player[i] MINUS_ONE][position_Q[2]].value_ == QUEEN))
                  {
                    // first 20 or 40
                    if (position_Q[0] > position[0])
                    {
                      // 40
                      if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ == TRUE)
                      {
                        points_pair = 40;
                        
                        handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                                 position_Q[0] MINUS_ONE,
                                                                 points_pair, &i, &points_call,
                                                                 &points_opponents,
                                                                 hands[player[i] MINUS_ONE],
                                                                 (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("K 40?\n");
                          
                          pairs--;
                          
                          position_Q[0] = position_Q[1];                            // naja, hardcoded
                          position_Q[1] = position_Q[2];                            // oba viel besseres
                          position_Q[2] = 0;                                        // fällt mir hier nit ein
                        }
                        
//                        printf("check K 40?\n");
                      }
                      // 20
                      else if (hands[player[i] MINUS_ONE][position_Q[0]].is_trump_ != TRUE)
                      {
                        points_pair = 20;
                        
                        handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                                 position_Q[0] MINUS_ONE,
                                                                 points_pair, &i, &points_call,
                                                                 &points_opponents,
                                                                 hands[player[i] MINUS_ONE],
                                                                 (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("K 20?\n");
                          
                          pairs--;
                          
                          position_Q[0] = position_Q[1];                            // naja, hardcoded
                          position_Q[1] = position_Q[2];                            // oba viel besseres
                          position_Q[2] = 0;                                        // fällt mir hier nit ein
                        }
                        
//                        printf("check K 20?\n");
                      }
                    }
                    // third 20 or 40
                    else if (position_Q[2] MINUS_ONE == 4)      // only possible
                                                                // on position 4
                    {
                      // 40
                      if (hands[player[i] MINUS_ONE][position_Q[2]].is_trump_ == TRUE)
                      {
                        points_pair = 40;
                        
                        handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                                 position_Q[2] MINUS_ONE,
                                                                 points_pair, &i, &points_call,
                                                                 &points_opponents,
                                                                 hands[player[i] MINUS_ONE],
                                                                 (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("K 40?\n");
                          
                          pairs--;
                          
                          position_Q[0] = position_Q[1];                            // naja, hardcoded
                          position_Q[1] = position_Q[2];                            // oba viel besseres
                          position_Q[2] = 0;                                        // fällt mir hier nit ein
                        }
                        
//                        printf("check K 40?\n");
                      }
                      // 20
                      else if (hands[player[i] MINUS_ONE][position_Q[2]].is_trump_ != TRUE)
                      {
                        points_pair = 20;
                        
                        handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                                 position_Q[2] MINUS_ONE,
                                                                 points_pair, &i, &points_call,
                                                                 &points_opponents,
                                                                 hands[player[i] MINUS_ONE],
                                                                 (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("K 20?\n");
                          
                          pairs--;
                          
                          position_Q[0] = position_Q[1];                            // naja, hardcoded
                          position_Q[1] = position_Q[2];                            // oba viel besseres
                          position_Q[2] = 0;                                        // fällt mir hier nit ein
                        }
                        
//                        printf("check K 20?\n");
                      }
                    }
                    // second 20 or 40
                    else /* best work around yet :D */
                    {
                      // 40
                      if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ == TRUE)
                      {
                        points_pair = 40;
                        
                        handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                                 position_Q[1] MINUS_ONE,
                                                                 points_pair, &i, &points_call,
                                                                 &points_opponents,
                                                                 hands[player[i] MINUS_ONE],
                                                                 (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("K 40?\n");
                          
                          pairs--;
                          
                          position_Q[0] = position_Q[1];                            // naja, hardcoded
                          position_Q[1] = position_Q[2];                            // oba viel besseres
                          position_Q[2] = 0;                                        // fällt mir hier nit ein
                        }
                        
//                        printf("check K 40?\n");
                      }
                      // 20
                      else if (hands[player[i] MINUS_ONE][position_Q[1]].is_trump_ != TRUE)
                      {
                        points_pair = 20;
                        handle_pairs[i].bool_pair_ = pairHandler(handle_pairs,
                                                                 position_Q[1] MINUS_ONE,
                                                                 points_pair, &i, &points_call,
                                                                 &points_opponents,
                                                                 hands[player[i] MINUS_ONE],
                                                                 (player[0] == buffer_start));
                        if (handle_pairs[i].bool_pair_ == TRUE)
                        {
//                          printf("K 20?\n");
                          
                          pairs--;
                          
                          position_Q[0] = position_Q[1];                            // naja, hardcoded
                          position_Q[1] = position_Q[2];                            // oba viel besseres
                          position_Q[2] = 0;                                        // fällt mir hier nit ein
                        }
                        
//                        printf("check K 20?\n");
                      }
                    }
                  }
                }
                
                if (i == ENOUGH)
                {
                  break;
                }
                
                else
                {
                  playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                  printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                            players[player[i] MINUS_ONE].name_);
                  printf("---------------------------------------------------------------\n");
                  break;
                }
                
              case 'd':
                position[i] = 5;
//                position[TURN_PLAYER_3 MINUS_ONE] = 5;
                
                // check for 20 or 40
                if (pairs > 0)
                {
                  // at this position we will only find a QUEEN
                  if ((position[i] == position_Q[0]
                       && hands[player[i] MINUS_ONE][position_Q[0]].value_ == QUEEN)
                      || (position[i] == position_Q[1]
                          && hands[player[i] MINUS_ONE][position_Q[1]].value_ == QUEEN)
                      || (position[i] == position_Q[2]
                          && hands[player[i] MINUS_ONE][position_Q[2]].value_ == QUEEN))
                    
                  /* QUICK AND DIRTY FIX (SAME AS IN modeGame() AND ABOVE)
                    .. IMPROVE IN "POST PRODUCTION" (; */
                    
                  {
                    for (int j = 0; j < 3; j++)                       // sowas tu'I normalerwiese NIE
                    {
                      if (position_Q[j] == position[i] /* 5 */)
                      {
                        // 40
                        if (hands[player[i] MINUS_ONE][position_Q[j]].is_trump_ == TRUE)
                        {
                          points_pair = 40;
                          
                          handle_pairs[i].bool_pair_
                            = pairHandler(handle_pairs, position_Q[j],
                                          points_pair, &i, &points_call,
                                          &points_opponents,
                                          hands[player[i] MINUS_ONE],
                                          (player[0] == buffer_start));
                          if (handle_pairs[i].bool_pair_ == TRUE)
                          {
//                            printf("Q 40?\n");
                            
                            pairs--;
                            
                            position_Q[0] = position_Q[1];
                            position_Q[1] = position_Q[2];
                            position_Q[2] = 0;
                          }
                          
//                          printf("check Q 40?\n");
                        }
                        // 20
                        else /* is_trump == FALSE */
                        {
                          points_pair = 20;
                          
                          handle_pairs[i].bool_pair_
                            = pairHandler(handle_pairs, position_Q[j],
                                          points_pair, &i, &points_call,
                                          &points_opponents,
                                          hands[player[i] MINUS_ONE],
                                          (player[0] == buffer_start));
                          if (handle_pairs[i].bool_pair_ == TRUE)
                          {
//                            printf("Q 20?\n");
                            
                            pairs--;
                            
                            position_Q[0] = position_Q[1];
                            position_Q[1] = position_Q[2];
                            position_Q[2] = 0;
                          }
                          
//                          printf("check Q 20?\n");
                        }
                      }
                    }
                  }
                }
                
                if (i == ENOUGH)
                {
                  break;
                }
                
                else
                {
                  playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
                  printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                            players[player[i] MINUS_ONE].name_);
                  printf("---------------------------------------------------------------\n");
                  break;
                }
                
              default:
                break;
            }
          }
          
          // further playing
          else
          {
            // rewind bock
            if (hands[player[i] MINUS_ONE][0].is_bock_ == TRUE) // hard coded 0 sollte passen
                                                                // zumal: ein bock: alle bock
            {
              for (counter_cards = 0; counter_cards < HAND; counter_cards++)
              {
                hands[player[i] MINUS_ONE][counter_cards].is_bock_ = FALSE;
              }
            }

            // check position(s) of valid cards for this round
            // do not allow any other card
            // delete non valid characters from commands
            count_bock    = 0;   // bock
            count_suit    = 0;   // right suit
            count_trump   = 0;   // trump
            
            // seek
            for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
                 counter_cards++)
            {
              // have trump
              if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_ == TRUE)
              {
    //            if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_)
                count_trump++;
              }
            }
            
            for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
                 counter_cards++)
            {
              // right suit
              if (i == 1)
              {
                if ((hands)[player[i] MINUS_ONE][counter_cards].suit_
                    == (hands)[player[i - 1] MINUS_ONE][position[i - 1]].suit_)
                {
                  // rigtht suit is trump (: trump has been played )
                  if (hands[player[i] MINUS_ONE][counter_cards].is_trump_ == TRUE)
                  {
                    count_trump--;
                  }
                  count_suit++;
                }
              }
              
              else
              {
                if ((hands)[player[i] MINUS_ONE][counter_cards].suit_
                    == (hands)[player[i - 2] MINUS_ONE][position[i - 2]].suit_)
                {
                  // rigtht suit is trump (: trump has been played )
                  if (hands[player[i] MINUS_ONE][counter_cards].is_trump_ == TRUE)
                  {
                    count_trump--;
                  }
                  count_suit++;
                }
              }
            }
            
            for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
                 counter_cards++)
            {
              // no trump && no suit = all bock
              if (i == 1)
              {
                if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_ == FALSE
                    && (hands)[player[i] MINUS_ONE][counter_cards].suit_
                    != (hands)[player[i - 1] MINUS_ONE][position[i - 1]].suit_)
                {
                  count_bock++;
                }
              }
              
              else
              {
                if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_ == FALSE
                    && (hands)[player[i] MINUS_ONE][counter_cards].suit_
                    != (hands)[player[i - 2] MINUS_ONE][position[i - 2]].suit_)
                {
                  count_bock++;
                }
              }
            }
            // .. and adjust ------------
            strcpy(players_commands[i], "000000");    // maybe there is a better
                                                      // more professinal way
            
            // all bock
            counter_trump   = 0;
            counter_suit    = 0;
            counter_cards   = 0;
            counter_hand    = 0;
            counter_command = 0;
            
            // all bock
            if ((count_suit == 0 && count_trump == 0))
            {
              // according to turn, -1
//              printf("Alles Bock!\n");
    //          players_commands[i] = commands;
    //          players_commands[i][counter_cards] = '\0';
              while (counter_hand < (HAND))
              {
                if ((hands)[player[i] MINUS_ONE][counter_hand].suit_ != NULL)
                {
                  players_commands[i][counter_cards] = commands[counter_command];
                  counter_command++;
                  counter_cards++;
                }
                
                else
                {
                  counter_command++;
                }
                
                counter_hand++;
              }
            }
            
            // no valid suit but trump
            else if (count_suit == 0 && count_trump > 0)
            {
              counter_trump   = 0;
              counter_suit    = 0;
              counter_cards   = 0;
              counter_hand    = 0;
              counter_command = 0;
              // must hold trump
              // allow trump(s)
//              printf("Muaßt an Trumpf spiel'n!\n");

              while (counter_hand < (HAND/* - counter_turns*/)/* eben nicht strlen(players_commands[i])*/)
              {
                if ((hands)[player[i] MINUS_ONE][counter_hand].suit_ != NULL
                    && hands[player[i] MINUS_ONE][counter_hand].is_trump_ == TRUE)
                {
                  players_commands[i][counter_cards] = commands[counter_command];
                  counter_command++;
                  counter_cards++;
                }
                
                else
                {
                  counter_command++;
                }
                
                counter_hand++;
              }
              
              // remove right
              for (counter_cards = count_trump; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
              {
                players_commands[i][counter_cards] = '\0';
              }
            }
            
            else /* must hold suit, at leas one */
            {
//              printf("Muaßt Farb' spielen!\n");
              counter_cards = 0;
              counter_command = 0;
              
              // remove left
              for (counter_hand = 0; counter_hand < (HAND/* - counter_turns*/); counter_hand++)
              {
                // suit found
                if (i == 1)   // first to answer
                {
                  if ((hands)[player[i] MINUS_ONE][counter_hand].suit_
                      == (hands)[player[i - 1] MINUS_ONE][position[i - 1]].suit_)
                  {
    //                printf("gleiche Farbe @ %d\n", counter_hand);
    //                printf("%c = %c\n", players_commands[i][counter_hand], commands[counter_command]);
                    players_commands[i][counter_cards] = commands[counter_command];
                    counter_command++;
                    counter_cards++;
                  }
                  else
                  {
                    counter_command++;
                  }
                }
                
                else    // second to answer
                {
                  if ((hands)[player[i] MINUS_ONE][counter_hand].suit_
                      == (hands)[player[i - 2] MINUS_ONE][position[i - 2]].suit_)
                  {
    //                printf("gleiche Farbe @ %d\n", counter_hand);
                    players_commands[i][counter_cards] = commands[counter_command];
                    counter_command++;
                    counter_cards++;
                  }
                  else
                  {
    //                players_commands[i][counter_hand] = commands[counter_command];
                    counter_command++;
                  }
                }
              }
              
              // remove right
              for (counter_cards = count_suit; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
              {
                players_commands[i][counter_cards] = '\0';
              }
            }
            
            // ------------- copied and pasted from modeGame() -----------------------
            if (i == 1)
            {
              // if there is at least one valid card to answer with
              if (!(count_suit == 0 && count_trump == 0))
              {
                // compare values and only allow weaker cards if there is not one
                // which could trump call
                counter_command = 0;
                counter_cards   = 0;
                count_permit    = 0;

                if (count_suit > 0)
                {
                  while (players_commands[i][counter_command] != '\0')
                  {
                    // answer 1 kills call with suit
                    if ((hands)[player[i] MINUS_ONE][counter_cards].value_ >
                        /* called card */ hands[player[0] MINUS_ONE][position[0]].value_)
                    {
                      buffer_higher[counter_cards] = players_commands[i][counter_cards];
                      buffer_higher[counter_cards ADD_ONE] = '\0';
                      counter_cards++;
                    }
                    counter_command++;
                  }
                }
                
                // I really think we may neglect the case (count_trump > 0),
                // since we would not need to further adjust commands

                if (counter_cards > 0)
                {
                  strcpy(players_commands[i], "\0\0\0\0\0\0");
                  strcpy(players_commands[i], buffer_higher);
                  strcpy(buffer_higher, "\0\0\0\0\0\0");
                }
              }
            }

            else /* if (i == 2) */
            {
              // if there is at least one valid card to answer with
              if (!(count_suit == 0 && count_trump == 0))
              {
                // compare values and only allow weaker cards if there is not one
                // which could trump any (call || answer 1)
                counter_command = 0;
                counter_cards   = 0;
                count_permit    = 0;

                if (count_suit > 0)
                {
                  while (players_commands[i][counter_command] != '\0')
                  {
                    if (!(hands[player[i MINUS_ONE] MINUS_ONE][position[i MINUS_ONE]].suit_
                          == hands[player[0] MINUS_ONE][position[0]].suit_
                          && hands[player[i MINUS_ONE] MINUS_ONE][position[i MINUS_ONE]].value_ <
                              hands[player[0] MINUS_ONE][position[0]].value_))
                    {
                      // answer 2 must only trump call
                      if (hands[player[i] MINUS_ONE][position[i]].value_ >
                          hands[player[0] MINUS_ONE][position[0]].value_)
                      {
                        buffer_higher[counter_cards] = players_commands[i][counter_cards];
                        buffer_higher[counter_cards ADD_ONE] = '\0';
                        counter_cards++;
                      }
                    }
                    counter_command++;
                  }
                }

                else if (count_trump > 0)
                {
                  if (hands[player[i MINUS_ONE] MINUS_ONE][position[i MINUS_ONE]].is_trump_)
                  {
                    while (players_commands[i][counter_command] != '\0')
                    {
                      // only need to compare trump's value with answer 1
                      if ((hands)[player[i] MINUS_ONE][counter_cards].value_ >
                          /* answer 1's card */ hands[player[0]][position[0]].value_)
                      {
                        buffer_higher[counter_cards] = players_commands[i][counter_cards];
                        buffer_higher[counter_cards ADD_ONE] = '\0';
                        counter_cards++;
                      }
                      counter_command++;
                    }
                  }
                }

                if (counter_cards > 0)
                {
                  strcpy(players_commands[i], "\0\0\0\0\0\0");
                  strcpy(players_commands[i], buffer_higher);
                  strcpy(buffer_higher, "\0\0\0\0\0\0");
                }
              }
            }
            // ------------- end of copy --------------------------------------------
            
            do
            {
              system ("/bin/stty raw");
              buffer = getchar();
              printf("\r");
              if (!(check = seekAndDestroy(buffer, players_commands[i])))
                printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
            } while (!check);
            system ("/bin/stty cooked");
            
            switch (buffer)     // no need to be changed, since it is an answer
            {
              case 'q':
                position[i] = 0;
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
      //          playCard(&(buffer_turn)[i].card_);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[player[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
                
              case 'w':
                position[i] = 1;
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
      //          playCard(&(buffer_turn)[i].card_);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[player[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
                
              case 'e':
                position[i] = 2;
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
      //          playCard(&(buffer_turn)[i].card_);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[player[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
                
              case 'a':
                position[i] = 3;
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
      //          playCard(&(buffer_turn)[i].card_);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[player[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
                
              case 's':
                position[i] = 4;
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
      //          playCard(&(buffer_turn)[i].card_);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[player[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
                
              case 'd':
                position[i] = 5;
                playCard(&(hands)[player[i] MINUS_ONE][position[i]]);
      //          playCard(&(buffer_turn)[i].card_);
                printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
                          players[player[i] MINUS_ONE].name_);
                printf("---------------------------------------------------------------\n");
                break;
                
              default:
                break;
            }
          } // end else (further playing after first call)
        } // end of for( i ) "3 cards each round"
        
        // ---------
        
        i = 0;
        
        nextAndPoints(&start, buffer_start, hands[player[i] MINUS_ONE][position[i]],
                      (hands)[player[i + 1] MINUS_ONE][position[i + 1]],
                      (hands)[player[i + 2] MINUS_ONE][position[i + 2]],
                      &points_call, &points_opponents,
                      &points[0], &points[1],
                      &points[2], turn);    // in this function call's points are
                                      // is points[0]
        
        // calling player is not next to call and therefore lost
        if (buffer_start != start)
        {
          break;
        }
        
        else
        {
          points_call       += points[0];
          points_opponents  += points[1] + points[2];
          points_call       += points_pair;
        }
        
        points[0] = 0;
        points[1] = 0;
        points[2] = 0;
        pairs = resetPairs(position_Q);
        resetHandlePairs(handle_pairs);
        points_pair = 0;
        
        if (points_call >= 66)
        {
          printf("Player %d wins 12 points!\n", buffer_start);
          printf("---------------------------------------------------------------\n");
          break;
        }
        
        // may we do this here -----------------------
        removeCard(&hands[TURN_PLAYER_1 MINUS_ONE][position[i]]);
        removeCard(&hands[TURN_PLAYER_2 MINUS_ONE][position[i + 1]]);
        removeCard(&hands[TURN_PLAYER_3 MINUS_ONE][position[i + 2]]);
        // -------------------------------------------
        // write all left over cards onto commands

        // player 1
        counter_position = 0 ;
        for (counter_cards = 0; counter_cards < HAND; counter_cards++)
        {
          if ((hands)[player[i] MINUS_ONE][counter_cards].suit_ != NULL)
          {
            players_commands[i][counter_position] = commands[counter_cards];
            counter_position++;
          }
        }
        players_commands[i][counter_position] = '\0';
        
        // player 2
        counter_position = 0 ;
        for (counter_cards = 0; counter_cards < HAND; counter_cards++)
        {
          if ((hands)[player[i + 1] MINUS_ONE][counter_cards].suit_ != NULL)
          {
            players_commands[i + 1][counter_position] = commands[counter_cards];
            counter_position++;
          }
        }
        players_commands[i + 1][counter_position] = '\0';
        
        // player 3
        counter_position = 0 ;
        for (counter_cards = 0; counter_cards < HAND; counter_cards++)
        {
          if ((hands)[player[i + 2] MINUS_ONE][counter_cards].suit_ != NULL)
          {
            players_commands[i + 2][counter_position] = commands[counter_cards];
            counter_position++;
          }
        }
        players_commands[i + 2][counter_position] = '\0';
        
        // sort players' order .. ob I mir das nicht sparen kann ..
        getCall(start, &call, &answer_1, &answer_2);
        
        player[0] = call;
        player[1] = answer_1;
        player[2] = answer_2;
        
        
        if (player[0] == TURN_PLAYER_1)
        {
          players_commands[0] = commands_1;
          players_commands[1] = commands_2;
          players_commands[2] = commands_3;
        }
        
        if (player[0] == TURN_PLAYER_2)
        {
          players_commands[0] = commands_2;
          players_commands[1] = commands_3;
          players_commands[2] = commands_1;
        }
        
        if (player[0] == TURN_PLAYER_3)
        {
          players_commands[0] = commands_3;
          players_commands[1] = commands_1;
          players_commands[2] = commands_2;
        }
        
        turn[0]   = call;
        turn[1]   = answer_1;
        turn[2]   = answer_2;
        
        // print points
        printf("player:  %d\nopponents:  %d\n", points_call, points_opponents);
        
        // calling player must not lose one round
        if (start != buffer_start)
        {
          printf("FOLD!\n");
          printf("---------------------------------------------------------------\n");
          break;
        }
      } // end else "further playing - after first call"
    } // end for()
    
    // Call < 66 = TOT
    if (points_call < 66)
    {
      printf("Opponents win 12 points!\n");
      printf("---------------------------------------------------------------\n");
      points_and_caller.points_ = 12;
      points_and_caller.winner_ = OPPONENTS_WIN;
      
      if (buffer_start == TURN_PLAYER_2)
      {
        points_and_caller.caller_ = TURN_PLAYER_2;
      }
      else /* if (buffer_start == TURN_PLAYER_3) */
      {
        points_and_caller.caller_ = TURN_PLAYER_3;
      }
      return points_and_caller;
    }
    
    else /* contra playing player wins */
    {
      printf("Caller wins 12 points!\n");
      printf("---------------------------------------------------------------\n");
      points_and_caller.points_ = 12;
      
      if (buffer_start == TURN_PLAYER_2)
      {
        points_and_caller.winner_ = TURN_PLAYER_2;
        points_and_caller.caller_ = points_and_caller.winner_;
      }
      else /* if (buffer_start == TURN_PLAYER_3)*/
      {
        points_and_caller.winner_ = TURN_PLAYER_3;
        points_and_caller.caller_ = points_and_caller.winner_;
      }
      return points_and_caller;
    }
  } // end of else (= KontraSchnapser)
}

Points modeLand(/*int bool_trump,*/ Card** hands, int start, Player* players)
{
  int counter = 0;
  int counter_turns = 0;
  int counter_1 = 0;
  int counter_cards = 0;
  int buffer_start = 0;
  buffer_start = start;
  char buffer = 0;
  int counter_commands = 0;
  
  char commands[7] = "qweasd";
//  long length_commands = strlen(commands);
  char commands_1[7] = "0";
  char commands_2[7] = "0";
  char commands_3[7] = "0";
  char commands_2_left[7] = "qweasd";
  char commands_3_left[7] = "qweasd";
  
  strcpy(commands_1, commands);
//  strcpy(commands_2, commands);
//  strcpy(commands_3, commands);
  int position_1 = 0;
  int position_2 = 0;
  int position_3 = 0;
  
  // default
  int call      = TURN_PLAYER_1;
  int answer_1  = TURN_PLAYER_2;
  int answer_2  = TURN_PLAYER_3;
  
  int check = FALSE;
  int bool_trump = FALSE;
  
  Points points_and_caller = {0, 0, 0};
  char buffer_higher[6] = {'\0'};
  
  getCall(start, &call, &answer_1, &answer_2);
  
  for (counter_turns = 0; counter_turns < MAXIMUM_TURNS; counter_turns++)
  {
    printf("TURN: %d\n", counter_turns + 1);
    
    // call
    printHand(hands[call MINUS_ONE], 6, call,
              players[call MINUS_ONE].name_);
    printf("                         ^     ^     ^     ^     ^     ^  \n");
    printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
    printf("---------------------------------------------------------------\n");
    
    do
    {
      system ("/bin/stty raw");
      buffer = getchar();
//      position = &buffer;
      printf("\r");
      if (!(check = seekAndDestroy(buffer, commands_1)))
        printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
    } while (!check);
    system ("/bin/stty cooked");
    
    switch (buffer)
    {
      case 'q':
        position_1 = 0;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'w':
        position_1 = 1;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'e':
        position_1 = 2;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'a':
        position_1 = 3;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 's':
        position_1 = 4;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'd':
        position_1 = 5;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      default:
        break;
    }
    
    // answer 1
    counter_1 = 0;
    
    printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
              players[answer_1 MINUS_ONE].name_);
    printf("                         ^     ^     ^     ^     ^     ^  \n");
    printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
    printf("---------------------------------------------------------------\n");
    
    // rewind bock
    if (hands[answer_1 MINUS_ONE][0].is_bock_ == TRUE)
    {
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_1 MINUS_ONE][counter_cards].is_bock_ = FALSE;
      }
    }
    
    // check position(s) of valid cards for this round
    // do not allow any other card
    for (counter = 0; counter < HAND; counter++)
      commands_2[counter] = '\0';
    for (counter = 0; counter < HAND; counter++)
    {
      if (hands[call MINUS_ONE][position_1].suit_
          == hands[answer_1 MINUS_ONE][counter].suit_)
      {
        commands_2[counter_1] = commands[counter];
        counter_1++;
      }
    }
    // better:
    for (counter = counter_1; counter < HAND; counter++)
    {
      commands_2[counter] = '\0';
    }
    
    // no valid suit
    // no valid value
    if (commands_2[0] == '\0')
    {
      counter = 0;
      counter_cards = 0;
      while (counter_cards < HAND)
      {
        if (commands_2_left[counter_cards] == '0')
        {
          counter_cards++;
        }
        
        if (commands_2_left[counter_cards] != '0')
        {
          commands_2[counter] = commands_2_left[counter_cards];
          counter++;
        }
        
        counter_cards++;
      }
      
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_1 MINUS_ONE][counter_cards].is_bock_ = TRUE;
      }
    }
    
    // STUCHZWANG (answer 1)
    counter_commands  = 0;
    counter_cards     = 0;
    
    if (!hands[answer_1 MINUS_ONE][0].is_bock_)
    {
      while (commands_2[counter_commands] != '\0')
      {
        if (hands[answer_1 MINUS_ONE][decomposeQWEASD(commands_2[counter_commands])].value_ >
            hands[call MINUS_ONE][position_1].value_)
        {
          buffer_higher[counter_cards] = commands_2[counter_cards];
          buffer_higher[counter_cards ADD_ONE] = '\0';
          counter_cards++;
        }
        counter_commands++;
      }
    }
    
    if (counter_cards > 0)
    {
      strcpy(commands_2, "\0\0\0\0\0\0");
      strcpy(commands_2, buffer_higher);
      strcpy(buffer_higher, "\0\0\0\0\0\0");
    }
    
    do
    {
      system ("/bin/stty raw");
      buffer = getchar();
      printf("\r");
      if (!(check = seekAndDestroy(buffer, commands_2)))
        printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
    } while (!check);
    system ("/bin/stty cooked");
    
    switch (buffer)
    {
      case 'q':
        position_2 = 0;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_2_left[0] = '0';
        break;
        
      case 'w':
        position_2 = 1;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_2_left[1] = '0';
        break;
        
      case 'e':
        position_2 = 2;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_2_left[2] = '0';
        break;
        
      case 'a':
        position_2 = 3;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_2_left[3] = '0';
        break;
        
      case 's':
        position_2 = 4;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_2_left[4] = '0';
        break;
        
      case 'd':
        position_2 = 5;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_2_left[5] = '0';
        break;
        
      default:
        break;
    }
    
    // and now we could immediately check if caller is TOT already
    if (counter_cards > 0)  // this condition is enough
    {
      printf("FOLD!\n");
      printf("---------------------------------------------------------------\n");
//      printf("answer 1 HAT IHN SCHON!\n");
      start = answer_1;
      points_and_caller.points_ = 9;
      points_and_caller.winner_ = OPPONENTS_WIN;
      points_and_caller.caller_ = buffer_start;
      return points_and_caller;
      break;
    }
    
    // answer 2
    counter_1 = 0;
    printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
              players[answer_2 MINUS_ONE].name_);
    printf("                         ^     ^     ^     ^     ^     ^  \n");
    printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
    printf("---------------------------------------------------------------\n");
    
    // rewind bock
    if (hands[answer_2 MINUS_ONE][0].is_bock_ == TRUE)
    {
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_2 MINUS_ONE][counter_cards].is_bock_ = FALSE;
      }
      
    }
    
    // check position(s) of valid cards for this round
    // do not allow any other card
    for (counter = 0; counter < HAND; counter++)
      commands_3[counter] = '\0';
    for (counter = 0; counter < HAND; counter++)
    {
      if (hands[call MINUS_ONE][position_1].suit_
          == hands[answer_2 MINUS_ONE][counter].suit_)
      {
        commands_3[counter_1] = commands[counter];
        counter_1++;
      }
    }
    
    // deny wrong usage
//    commands_3[counter_1] = '\0';
    for (counter = counter_1; counter < HAND; counter++)
    {
      commands_3[counter] = '\0';
    }
    
    if (commands_3[0] == '\0')
    {
      counter = 0;
      counter_cards = 0;
      while (counter_cards < HAND)
      {
        if (commands_3_left[counter_cards] == '0')
        {
          counter_cards++;
        }
        
        if (commands_3_left[counter_cards] != '0')
        {
          commands_3[counter] = commands_3_left[counter_cards];
          counter++;
        }
        
        counter_cards++;
      }
      
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_2 MINUS_ONE][counter_cards].is_bock_ = TRUE;
      }
    }
    
    // STUCHZWANG (answer 2)
    counter_commands  = 0;
    counter_cards     = 0;
    if (!hands[answer_2 MINUS_ONE][0].is_bock_)
    {
      while (commands_3[counter_commands] != '\0')
      {
        if (hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_ >
            hands[call MINUS_ONE][position_1].value_)
        {
          if (hands[answer_1 MINUS_ONE][position_2].value_
              > hands[call MINUS_ONE][position_1].value_
              && hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_
              > hands[call MINUS_ONE][position_1].value_)
          {
            buffer_higher[counter_cards] = commands_3[counter_cards];
            buffer_higher[counter_cards ADD_ONE] = '\0';
            counter_cards++;
          }
          else
          {
            buffer_higher[counter_cards] = commands_3[counter_cards];
            buffer_higher[counter_cards ADD_ONE] = '\0';
            counter_cards++;
          }
        }
        counter_commands++;
      }
    }
    
    if (counter_cards > 0)
    {
      strcpy(commands_3, "\0\0\0\0\0\0");
      strcpy(commands_3, buffer_higher);
      strcpy(buffer_higher, "\0\0\0\0\0\0");
    }
    
    do
    {
      system ("/bin/stty raw");
      buffer = getchar();
      printf("\r");
      if (!(check = seekAndDestroy(buffer, commands_3)))
        printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
    } while (!check);
    system ("/bin/stty cooked");
    
    switch (buffer)
    {
      case 'q':
        position_3 = 0;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_3_left[0] = '0';
        break;
        
      case 'w':
        position_3 = 1;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_3_left[1] = '0';
        break;
        
      case 'e':
        position_3 = 2;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_3_left[2] = '0';
        break;
        
      case 'a':
        position_3 = 3;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_3_left[3] = '0';
        break;
        
      case 's':
        position_3 = 4;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_3_left[4] = '0';
        break;
        
      case 'd':
        position_3 = 5;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        
        commands_3_left[5] = '0';
        break;
        
      default:
        break;
    }
    
    // and now we could immediately check if caller is TOT already
    if (counter_cards > 0)  // this condition is enough
    {
      printf("FOLD!\n");
      printf("---------------------------------------------------------------\n");
//      printf("answer 2 HAT IHN SCHON!\n");
      start = answer_2;
      points_and_caller.points_ = 9;
      points_and_caller.winner_ = OPPONENTS_WIN;
      points_and_caller.caller_ = buffer_start;
      return points_and_caller;
      break;
    }
    
    // who has played the highest card
    start = highest(bool_trump, start, hands[call - 1][position_1],
                    hands[answer_1 - 1][position_2],
                    hands[answer_2 - 1][position_3]);
    
    // delete or do not display played cards anymore
    // dh. delete suits
    removeCard(&hands[call - 1][position_1]);
    removeCard(&hands[answer_1 - 1][position_2]);
    removeCard(&hands[answer_2 - 1][position_3]);
    
    // calling player must not lose one round
    // might never be executed (...)
    if (start != buffer_start)
    {
      printf("FOLD!\n");
      printf("---------------------------------------------------------------\n");
      
      points_and_caller.points_ = 9;
      points_and_caller.winner_ = OPPONENTS_WIN;
      points_and_caller.caller_ = buffer_start;
      return points_and_caller;
      
      break;
    }
  } // end for()
  
//  if (start == buffer_start)
//  {
    printf("test: land won!\n");
    points_and_caller.points_ = 9;
    points_and_caller.winner_ = buffer_start;
    points_and_caller.caller_ = buffer_start;
    return points_and_caller;
//  }
}

Points modeBauernschnapser(/*int bool_trump,*/ Card** hands, int start, int trump,
                         Player* players)
{
  int counter = 0;
  int counter_turns = 0;
  int counter_1 = 0;        // iterates players - at least during first turn
  int counter_2 = 0;        // valid cards (player 2 on first turn)
  int counter_3 = 0;
  int counter_cards = 0;
  int counter_suit = 0;
  int counter_trump = 0;
  int buffer_start = 0;
  buffer_start = start;
  char buffer = 0;
  
  char commands[7] = "qweasd";
//  long length_commands = strlen(commands);
  char commands_1[7] = "0";
  char commands_2[7] = "0";
  char commands_3[7] = "0";
  char commands_2_left[7] = "qweasd";
  char commands_3_left[7] = "qweasd";
  strcpy(commands_1, commands);
//  strcpy(commands_2, commands);
//  strcpy(commands_3, commands);
  int position_1 = 0;
  int position_2 = 0;
  int position_3 = 0;
  
  // default
  int call      = TURN_PLAYER_1;
  int answer_1  = TURN_PLAYER_2;
  int answer_2  = TURN_PLAYER_3;
  
  int player[3] = {call, answer_1, answer_2};
  char* commands_players[3] = {/*commands*/ commands_1, commands_2, commands_3};
  char* commands_players_left[3] = {"qweasd", commands_2_left, commands_3_left};
  int positions[3] = {position_1, position_2, position_3};
  
  int check       = FALSE;
  int bool_trump  = TRUE;
  
  int counter_commands  = 0;
  char buffer_higher[6] = {'\0'};
  
  Points points_and_caller = {0, 0, 0};
  
  getCall(start, &call, &answer_1, &answer_2);
  
  // Bauernschnapser   - TODO: change call, answer_1 and answer_2 to player[0, 1 & 2] (post production)
  if (/*player[0]*/start == TURN_PLAYER_1)
  {
    for (counter_turns = 0; counter_turns < MAXIMUM_TURNS; counter_turns++)
    {
      printf("TURN: %d\n", counter_turns + 1);
      
      // call
      printHand(hands[call MINUS_ONE], 6, call,
                players[call MINUS_ONE].name_);
      printf("                         ^     ^     ^     ^     ^     ^  \n");
      printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
      printf("---------------------------------------------------------------\n");
      
      do
      {
        system ("/bin/stty raw");
        buffer = getchar();
  //      position = &buffer;
        printf("\r");
        if (!(check = seekAndDestroy(buffer, commands_1)))
          printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
      } while (!check);
      system ("/bin/stty cooked");
      
      switch (buffer)
      {
        case 'q':
          position_1 = 0;
          playCard(&(hands)[call MINUS_ONE][position_1]);
          printHand(hands[call MINUS_ONE], 6, call,
                    players[call MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'w':
          position_1 = 1;
          playCard(&(hands)[call MINUS_ONE][position_1]);
          printHand(hands[call MINUS_ONE], 6, call,
                    players[call MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'e':
          position_1 = 2;
          playCard(&(hands)[call MINUS_ONE][position_1]);
          printHand(hands[call MINUS_ONE], 6, call,
                    players[call MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'a':
          position_1 = 3;
          playCard(&(hands)[call MINUS_ONE][position_1]);
          printHand(hands[call MINUS_ONE], 6, call,
                    players[call MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 's':
          position_1 = 4;
          playCard(&(hands)[call MINUS_ONE][position_1]);
          printHand(hands[call MINUS_ONE], 6, call,
                    players[call MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'd':
          position_1 = 5;
          playCard(&(hands)[call MINUS_ONE][position_1]);
          printHand(hands[call MINUS_ONE], 6, call,
                    players[call MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        default:
          break;
      }
      
      // answer 1
      counter_1 = 0;
      
      printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                players[answer_1 MINUS_ONE].name_);
      printf("                         ^     ^     ^     ^     ^     ^  \n");
      printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
      printf("---------------------------------------------------------------\n");
      
      // rewind bock
      if (hands[answer_1 MINUS_ONE][0].is_bock_ == TRUE)
      {
        for (counter_cards = 0; counter_cards < HAND; counter_cards++)
        {
          hands[answer_1 MINUS_ONE][counter_cards].is_bock_ = FALSE;
        }
      }
      
      // check position(s) of valid cards for this round
      // do not allow any other card
      for (counter = 0; counter < strlen(commands_2); counter++)
        commands_2[counter] = '\0';
      
      counter_suit = 0;
      counter_trump = 0;
      // right suit
      for (counter = 0; counter < HAND; counter++)
      {
        if (hands[call - 1][position_1].suit_
            == hands[answer_1 - 1][counter].suit_)
        {
          counter_suit++;
        }
      }
      
      // no suit but maybe trump
  //    if (counter_suit > 1)
      if (counter_suit < 1)
      {
        for (counter = 0; counter < HAND; counter++)
        {
          if (hands[answer_1 - 1][counter].suit_ == decodeSuit(trump))
          {
            counter_trump++;
          }
        }
      }
      
      // no trump || trump and sign
      if (counter_trump == 0 || (counter_trump > 0 && counter_suit > 0))
      {
        for (counter = 0; counter < HAND; counter++)
        {
          if (hands[call - 1][position_1].suit_
              == hands[answer_1 - 1][counter].suit_)
          {
            commands_2[counter_1] = commands[counter];
            counter_1++;
          }
        }
      }
      
      // trump alone
      else
      {
        for (counter = 0; counter < HAND; counter++)
        {
          if (hands[answer_1 - 1][counter].suit_ == decodeSuit(trump))
          {
            commands_2[counter_1] = commands[counter];
            counter_1++;
          }
        }
      }
      
      // deny wrong usage
      for (counter = counter_1; counter < HAND; counter++)
      {
        commands_2[counter] = '\0';
      }
      
      // no valid suit
      // no valid value
      if (commands_2[0] == '\0')
      {
  //      strcpy(commands_2, commands);
        counter = 0;
        counter_cards = 0;
        while (counter_cards < HAND)
        {
          if (commands_2_left[counter_cards] == '0')
          {
            counter_cards++;
          }
          
          if (commands_2_left[counter_cards] != '0')
          {
            commands_2[counter] = commands_2_left[counter_cards];
            counter++;
          }
          
          counter_cards++;
        }
        
        for (counter_cards = 0; counter_cards < HAND; counter_cards++)
        {
          hands[answer_1 - 1][counter_cards].is_bock_ = TRUE;
        }
      }
      
      // ---------------------
      // STUCHZWANG (answer 1)
      counter_commands  = 0;
      counter_cards     = 0;
      
      if (!hands[answer_1 MINUS_ONE][0].is_bock_)
      {
        while (commands_2[counter_commands] != '\0')
        {
          if (hands[call MINUS_ONE][position_1].suit_
              == hands[answer_1 MINUS_ONE][decomposeQWEASD(commands_2[counter_commands])].suit_)
          {
            if (hands[answer_1 MINUS_ONE][decomposeQWEASD(commands_2[counter_commands])].value_ >
                hands[call MINUS_ONE][position_1].value_)
            {
              buffer_higher[counter_cards] = commands_2[counter_cards];
              buffer_higher[counter_cards ADD_ONE] = '\0';
              counter_cards++;
            }
          }
          
          else if (hands[answer_1 MINUS_ONE][decomposeQWEASD(commands_2[counter_commands])].is_trump_)
          {
            buffer_higher[counter_cards] = commands_2[counter_cards];
            buffer_higher[counter_cards ADD_ONE] = '\0';
            counter_cards++;
          }
          counter_commands++;
        }
      }
      
      if (counter_cards > 0)
      {
        strcpy(commands_2, "\0\0\0\0\0\0");
        strcpy(commands_2, buffer_higher);
        strcpy(buffer_higher, "\0\0\0\0\0\0");
      }
      // ---------------------
      
      do
      {
        system ("/bin/stty raw");
        buffer = getchar();
        printf("\r");
        if (!(check = seekAndDestroy(buffer, commands_2)))
          printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
      } while (!check);
      system ("/bin/stty cooked");
      
      switch (buffer)
      {
        case 'q':
          position_2 = 0;
          playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
          printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                    players[answer_1 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'w':
          position_2 = 1;
          playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
          printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                    players[answer_1 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'e':
          position_2 = 2;
          playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
          printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                    players[answer_1 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'a':
          position_2 = 3;
          playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
          printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                    players[answer_1 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 's':
          position_2 = 4;
          playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
          printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                    players[answer_1 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'd':
          position_2 = 5;
          playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
          printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                    players[answer_1 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        default:
          break;
      }
      
      // answer 2
      counter_1 = 0;
      printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                players[answer_2 MINUS_ONE].name_);
      printf("                         ^     ^     ^     ^     ^     ^  \n");
      printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
      printf("---------------------------------------------------------------\n");
      
      // rewind bock
      if (hands[answer_1 - 1][0].is_bock_ == TRUE)
      {
        for (counter_cards = 0; counter_cards < HAND; counter_cards++)
        {
          hands[answer_1 - 1][counter_cards].is_bock_ = FALSE;
        }
        
      }
      
      // check position(s) of valid cards for this round
      // do not allow any other card
      for (counter = 0; counter < strlen(commands_2); counter++)
        commands_2[counter] = '\0';

      counter_suit = 0;
      counter_trump = 0;
      // right suit
      for (counter = 0; counter < HAND; counter++)
      {
        if (hands[call MINUS_ONE][position_1].suit_
            == hands[answer_2 MINUS_ONE][counter].suit_)
        {
          counter_suit++;
        }
      }
      
      // no suit but maybe trump
  //    if (counter_suit > 1)
      if (counter_suit < 1)
      {
        for (counter = 0; counter < HAND; counter++)
        {
          if (hands[answer_2 MINUS_ONE][counter].suit_ == decodeSuit(trump))
          {
            counter_trump++;
          }
        }
      }
      
      // no trump || trump and sign
      if (counter_trump == 0 || (counter_trump > 0 && counter_suit > 0))
      {
        for (counter = 0; counter < HAND; counter++)
        {
          if (hands[call MINUS_ONE][position_1].suit_
              == hands[answer_2 MINUS_ONE][counter].suit_)
          {
            commands_3[counter_1] = commands[counter];
            counter_1++;
          }
        }
      }
      
      // trump alone
      else
      {
        for (counter = 0; counter < HAND; counter++)
        {
          if (hands[answer_2 MINUS_ONE][counter].suit_ == decodeSuit(trump))
          {
            commands_3[counter_1] = commands[counter];
            counter_1++;
          }
        }
      }
      
      // deny wrong usage
      for (counter = counter_1; counter < HAND; counter++)
      {
        commands_3[counter] = '\0';
      }
      
      if (commands_3[0] == '\0')
      {
  //      strcpy(commands_3, commands);
        counter = 0;
        counter_cards = 0;
        while (counter_cards < HAND)
        {
          if (commands_3_left[counter_cards] == '0')
          {
            counter_cards++;
          }
          
          if (commands_3_left[counter_cards] != '0')
          {
            commands_3[counter] = commands_3_left[counter_cards];
            counter++;
          }
          
          counter_cards++;
        }
        
        for (counter_cards = 0; counter_cards < HAND; counter_cards++)
        {
          hands[answer_2 MINUS_ONE][counter_cards].is_bock_ = TRUE;
        }
      }
      
      // ---------------------
      // STUCHZWANG (answer 2)
      counter_commands  = 0;
      counter_cards     = 0;
      if (!hands[answer_2 MINUS_ONE][0].is_bock_)
      {
        while (commands_3[counter_commands] != '\0')
        {
          // must only match called suit
          if (hands[call MINUS_ONE][position_1].suit_
              == hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].suit_)
          {
            // case: answer 1's and answer 2's played matching suit
            if (hands[answer_1 MINUS_ONE][position_2].suit_ ==
                hands[call MINUS_ONE][position_1].suit_)
            {
              // answer 1 has already trumped
              if (hands[answer_1 MINUS_ONE][position_2].value_
                  > hands[call MINUS_ONE][position_1].value_)
              {
                // can answer 2 trump answer 1?
                if (hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_
                    > hands[answer_1 MINUS_ONE][position_2].value_)
                {
                  buffer_higher[counter_cards] = commands_3[counter_cards];
                  buffer_higher[counter_cards ADD_ONE] = '\0';
                  counter_cards++;
                }
              }
              else if (hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_
                       > hands[call MINUS_ONE][position_1].value_)
              {
                buffer_higher[counter_cards] = commands_3[counter_cards];
                buffer_higher[counter_cards ADD_ONE] = '\0';
                counter_cards++;
              }
            }
            // case: answer 1 played trump
            else
            {
              if (hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_
                  > hands[call MINUS_ONE][position_1].value_)
              {
                buffer_higher[counter_cards] = commands_3[counter_cards];
                buffer_higher[counter_cards ADD_ONE] = '\0';
                counter_cards++;
              }
            }
          }
          // else they must utilize trump - and in case trump answer 1's trump
          else if (hands[answer_1 MINUS_ONE][position_2].is_trump_)
          {
            if (hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_
                > hands[answer_1 MINUS_ONE][position_2].value_)
            {
              buffer_higher[counter_cards] = commands_3[counter_cards];
              buffer_higher[counter_cards ADD_ONE] = '\0';
              counter_cards++;
            }
          }
          counter_commands++;
        }
      }
      
      if (counter_cards > 0)
      {
        strcpy(commands_3, "\0\0\0\0\0\0");
        strcpy(commands_3, buffer_higher);
        strcpy(buffer_higher, "\0\0\0\0\0\0");
      }
      // ---------------------
      
      do
      {
        system ("/bin/stty raw");
        buffer = getchar();
        printf("\r");
        if (!(check = seekAndDestroy(buffer, commands_3)))
          printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
      } while (!check);
      system ("/bin/stty cooked");
      
      switch (buffer)
      {
        case 'q':
          position_3 = 0;
          playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
          printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                    players[answer_2 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'w':
          position_3 = 1;
          playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
          printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                    players[answer_2 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'e':
          position_3 = 2;
          playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
          printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                    players[answer_2 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'a':
          position_3 = 3;
          playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
          printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                    players[answer_2 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 's':
          position_3 = 4;
          playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
          printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                    players[answer_2 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'd':
          position_3 = 5;
          playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
          printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                    players[answer_2 MINUS_ONE].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        default:
          break;
      }
      
      // who has played the highest card
      start = highest(bool_trump, start, hands[call - 1][position_1],
                      hands[answer_1 - 1][position_2],
                      hands[answer_2 - 1][position_3]);
      
      // delete or do not display played cards anymore
      // dh. delete suits
      removeCard(&hands[call - 1][position_1]);
      removeCard(&hands[answer_1 - 1][position_2]);
      removeCard(&hands[answer_2 - 1][position_3]);
      
      // calling player must not lose one round
      if (start != buffer_start)
      {
        printf("FOLD!\n");
        printf("---------------------------------------------------------------\n");
        
        points_and_caller.points_ = 12;
        points_and_caller.winner_ = OPPONENTS_WIN;
        points_and_caller.caller_ = buffer_start;
        return points_and_caller;
        
        break;
      }
    } // end for()
    
    printf("test: bauer won!\n");
    points_and_caller.points_ = 12;
    points_and_caller.winner_ = buffer_start;
    points_and_caller.caller_ = buffer_start;
    return points_and_caller;
  }
  
  // KontraBauernschnapser
  else
  {
    printf("KONTRABAUERNSCHNAPSER!\n");
    
    for (counter_turns = 0; counter_turns < MAXIMUM_TURNS; counter_turns++)
    {
      // player 1 is first to call
      if (counter_turns == 0 && counter_1 == 0)
      {
        printf("TURN: %d\n", counter_turns + 1);

        printHand(hands[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                  players[TURN_PLAYER_1 MINUS_ONE].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");

        do
        {
          system ("/bin/stty raw");
          buffer = getchar();
    //      position = &buffer;
          printf("\r");
          if (!(check = seekAndDestroy(buffer, commands_players[TURN_PLAYER_1 MINUS_ONE])))
            printf("Gibt's nicht!\n");
        } while (!check);
        system ("/bin/stty cooked");

        switch (buffer)
        {
          case 'q':
            position_1 = 0;
            playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][/*positions[TURN_PLAYER_1 MINUS_ONE]*/position_1]);
            printHand(hands[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1 MINUS_ONE,
                      players[TURN_PLAYER_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'w':
            position_1 = 1;
            playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][position_1]);
            printHand(hands[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                      players[TURN_PLAYER_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'e':
            position_1 = 2;
            playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][position_1]);
            printHand(hands[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                      players[TURN_PLAYER_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'a':
            position_1 = 3;
            playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][position_1]);
            printHand(hands[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                      players[TURN_PLAYER_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 's':
            position_1 = 4;
            playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][position_1]);
            printHand(hands[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                      players[TURN_PLAYER_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'd':
            position_1 = 5;
            playCard(&(hands)[TURN_PLAYER_1 MINUS_ONE][position_1]);
            printHand(hands[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                      players[TURN_PLAYER_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          default:
            break;
        }
        
        counter_1++;
        positions[TURN_PLAYER_1 MINUS_ONE] = position_1;
      } // player 1 calls first should be fine

      // answer 1 - TODO: checking valid cards does not work properly
      /* else */ if (counter_turns == 0 && counter_1 == 1)
      {
        counter_2 = 0;

        printHand(hands[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                  players[TURN_PLAYER_2 MINUS_ONE].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");

        // rewind bock
        if (hands[TURN_PLAYER_2 MINUS_ONE][0].is_bock_ == TRUE)
        {
          for (counter_cards = 0; counter_cards < HAND; counter_cards++)
          {
            hands[TURN_PLAYER_2 MINUS_ONE][counter_cards].is_bock_ = FALSE;
          }
        }

        // check position(s) of valid cards for this round
        // do not allow any other card
        for (counter = 0; counter < strlen(commands_players[TURN_PLAYER_2 MINUS_ONE]); counter++)
            commands_players[TURN_PLAYER_2 MINUS_ONE][counter] = '\0';

        counter_suit  = 0;
        counter_trump = 0;
        
        // right suit
        for (counter = 0; counter < HAND; counter++)
        {
          if (hands[TURN_PLAYER_1 MINUS_ONE][positions[TURN_PLAYER_1 MINUS_ONE]].suit_
              == hands[TURN_PLAYER_2 MINUS_ONE][counter].suit_)
          {
            counter_suit++;
          }
        }

        // no suit but maybe trump
        if (counter_suit < 1)
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[TURN_PLAYER_2 MINUS_ONE][counter].suit_ == decodeSuit(trump))
            {
              counter_trump++;
            }
          }
        }

        // no trump || trump and sign
        if (counter_trump == 0 || (counter_trump > 0 && counter_suit > 0))
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[TURN_PLAYER_1 MINUS_ONE][positions[TURN_PLAYER_1 MINUS_ONE]].suit_
                == hands[TURN_PLAYER_2 MINUS_ONE][counter].suit_)
            {
              commands_players[TURN_PLAYER_2 MINUS_ONE][counter_2] = commands[counter];
              counter_2++;
            }
          }
        }

        // trump alone
        else
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[TURN_PLAYER_2 MINUS_ONE][counter].suit_ == decodeSuit(trump))
            {
              commands_players[TURN_PLAYER_2 MINUS_ONE][counter_2] = commands[counter];
              counter_2++;
            }
          }
        }

        // deny wrong usage
        for (counter = counter_2; counter < HAND; counter++)
        {
          commands_players[TURN_PLAYER_2 MINUS_ONE][counter] = '\0';
        }

        // no valid suit
        // no valid value
        if (commands_players[TURN_PLAYER_2 MINUS_ONE][0] == '\0')
        {
          counter       = 0;
          counter_cards = 0;
          
          while (counter_cards < HAND)
          {
            if (commands_players_left[TURN_PLAYER_2 MINUS_ONE][counter_cards] == '0')
            {
              counter_cards++;
            }

            if (commands_players_left[TURN_PLAYER_2 MINUS_ONE][counter_cards] != '0')
            {
              commands_players[TURN_PLAYER_2 MINUS_ONE][counter]
                = commands_2_left[counter_cards];
              counter++;
            }

            counter_cards++;
          }

          for (counter_cards = 0; counter_cards < HAND; counter_cards++)
          {
            hands[TURN_PLAYER_2 MINUS_ONE][counter_cards].is_bock_ = TRUE;
          }
        } // TODO: check if it is really completely right!
        
        // -----------------------
        // STUCHZWANG (answer 1: called this mode)
        // TODO: clean up this copy (post production)
        counter_commands  = 0;
        counter_cards     = 0;
        
        if (!hands[TURN_PLAYER_2 MINUS_ONE][0].is_bock_)
        {
          while (commands_2[counter_commands] != '\0')
          {
            if (hands[TURN_PLAYER_1 MINUS_ONE][position_1].suit_
                == hands[TURN_PLAYER_2 MINUS_ONE][decomposeQWEASD(commands_2[counter_commands])].suit_)
            {
              if (hands[TURN_PLAYER_2 MINUS_ONE][decomposeQWEASD(commands_2[counter_commands])].value_ >
                  hands[TURN_PLAYER_1 MINUS_ONE][position_1].value_)
              {
                buffer_higher[counter_cards] = commands_2[counter_cards];
                buffer_higher[counter_cards ADD_ONE] = '\0';
                counter_cards++;
              }
            }
            else if (hands[TURN_PLAYER_2 MINUS_ONE][decomposeQWEASD(commands_2[counter_commands])].is_trump_)
            {
              buffer_higher[counter_cards] = commands_2[counter_cards];
              buffer_higher[counter_cards ADD_ONE] = '\0';
              counter_cards++;
            }
            counter_commands++;
          }
        }
        
        if (counter_cards > 0)
        {
          strcpy(commands_2, "\0\0\0\0\0\0");
          strcpy(commands_2, buffer_higher);
          strcpy(buffer_higher, "\0\0\0\0\0\0");
        }
        // -----------------------

        do
        {
          system ("/bin/stty raw");
          buffer = getchar();
          printf("\r");
          if (!(check = seekAndDestroy(buffer, commands_players[TURN_PLAYER_2 MINUS_ONE])))
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (!check);
        system ("/bin/stty cooked");

        switch (buffer)
        {
          case 'q':
            position_2 = 0;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][/*positions[TURN_PLAYER_2 MINUS_ONE]*/ position_2]);
            printHand(hands[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'w':
            position_2 = 1;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][position_2]);
            printHand(hands[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'e':
            position_2 = 2;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][position_2]);
            printHand(hands[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'a':
            position_2 = 3;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][position_2]);
            printHand(hands[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 's':
            position_2 = 4;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][position_2]);
            printHand(hands[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'd':
            position_2 = 5;
            playCard(&(hands)[TURN_PLAYER_2 MINUS_ONE][position_2]);
            printHand(hands[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                      players[TURN_PLAYER_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          default:
            break;
        }
        
        counter_1++;
      } // might be alright until here

      // answer 2
      if (counter_turns == 0 && counter_1 == 2)
      {
        printHand(hands[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                  players[TURN_PLAYER_3 MINUS_ONE].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");

        // rewind bock
        if (hands[TURN_PLAYER_3 MINUS_ONE][0].is_bock_ == TRUE)
        {
          for (counter_cards = 0; counter_cards < HAND; counter_cards++)
          {
            hands[TURN_PLAYER_3 MINUS_ONE][counter_cards].is_bock_ = FALSE;
          }

        }

        // check position(s) of valid cards for this round
        // do not allow any other card
        for (counter = 0; counter < strlen(commands_3); counter++)
          commands_3[counter] = '\0';

        counter_suit = 0;
        counter_trump = 0;
        // right suit
        for (counter = 0; counter < HAND; counter++)
        {
          if (hands[TURN_PLAYER_1 MINUS_ONE][position_1].suit_
              == hands[TURN_PLAYER_3 MINUS_ONE][counter].suit_)
          {
            counter_suit++;
          }
        }

        // no suit but maybe trump
        if (counter_suit < 1)
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[TURN_PLAYER_3 MINUS_ONE][counter].suit_ == decodeSuit(trump))
            {
              counter_trump++;
            }
          }
        }

        // no trump || trump and sign
        if (counter_trump == 0 || (counter_trump > 0 && counter_suit > 0))
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[TURN_PLAYER_1 MINUS_ONE][position_1].suit_
                == hands[TURN_PLAYER_3 MINUS_ONE][counter].suit_)
            {
              commands_3[counter_3] = commands[counter];
              counter_3++;
            }
          }
        }

        // trump alone
        else
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[TURN_PLAYER_3 MINUS_ONE][counter].suit_ == decodeSuit(trump))
            {
              commands_3[counter_3] = commands[counter];
              counter_3++;
            }
          }
        }

        // deny wrong usage
        for (counter = /*counter_1*/counter_3; counter < HAND; counter++)
        {
          commands_3[counter] = '\0';
        }

        if (commands_3[0] == '\0')
        {
          counter = 0;
          counter_cards = 0;
          while (counter_cards < HAND)
          {
            if (commands_3_left[counter_cards] == '0')
            {
              counter_cards++;
            }

            if (commands_3_left[counter_cards] != '0')
            {
              commands_3[counter] = commands_3_left[counter_cards];
              counter++;
            }

            counter_cards++;
          }

          for (counter_cards = 0; counter_cards < HAND; counter_cards++)
          {
            hands[TURN_PLAYER_3 MINUS_ONE][counter_cards].is_bock_ = TRUE;
          }
        }
        
        // ---------------------------
        // STUCHZWANG (answer 2)
        counter_commands  = 0;
        counter_cards     = 0;
        if (!hands[TURN_PLAYER_3 MINUS_ONE][0].is_bock_)
        {
          while (commands_3[counter_commands] != '\0')
          {
            if (hands[TURN_PLAYER_3 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_ >
                hands[TURN_PLAYER_1 MINUS_ONE][position_1].value_
                || hands[TURN_PLAYER_3 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_ >
                    hands[TURN_PLAYER_2 MINUS_ONE][position_2].value_)
            {
              buffer_higher[counter_cards] = commands_3[counter_cards];
              buffer_higher[counter_cards ADD_ONE] = '\0';
              counter_cards++;
            }
            counter_commands++;
          }
        }
        
        if (counter_cards > 0)
        {
          strcpy(commands_3, "\0\0\0\0\0\0");
          strcpy(commands_3, buffer_higher);
          strcpy(buffer_higher, "\0\0\0\0\0\0");
        }
        // ---------------------------

        do
        {
          system ("/bin/stty raw");
          buffer = getchar();
          printf("\r");
          if (!(check = seekAndDestroy(buffer, commands_3)))
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (!check);
        system ("/bin/stty cooked");

        switch (buffer)
        {
          case 'q':
            position_3 = 0;
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position_3]);
            printHand(hands[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'w':
            position_3 = 1;
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position_3]);
            printHand(hands[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'e':
            position_3 = 2;
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position_3]);
            printHand(hands[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'a':
            position_3 = 3;
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position_3]);
            printHand(hands[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 's':
            position_3 = 4;
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position_3]);
            printHand(hands[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          case 'd':
            position_3 = 5;
            playCard(&(hands)[TURN_PLAYER_3 MINUS_ONE][position_3]);
            printHand(hands[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                      players[TURN_PLAYER_3 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;

          default:
            break;
        }
        counter_1++;
      }
      
      // ----------------------------------------------------------------------
      // rest of the game after initial call

      // who has played the highest card
      
      // first turn
//      if (counter_turns == 0)
//      {
        start = highest(bool_trump, TURN_PLAYER_1,
                        hands[TURN_PLAYER_1 MINUS_ONE][position_1],
                        hands[TURN_PLAYER_2 MINUS_ONE][position_2],
                        hands[TURN_PLAYER_3 MINUS_ONE][position_3]);
        
        removeCard(&hands[TURN_PLAYER_1 MINUS_ONE][position_1]);
        removeCard(&hands[TURN_PLAYER_2 MINUS_ONE][position_2]);
        removeCard(&hands[TURN_PLAYER_3 MINUS_ONE][position_3]);
//      }
      // following turns
//      else
//      {
//        switch (start)
//        {
//          // case TURN_PLAYER_1:  equals an usual BAUERNSCHNAPSER
//
//          case TURN_PLAYER_2:
//            start = highest(bool_trump, TURN_PLAYER_2,
//                            hands[TURN_PLAYER_2 MINUS_ONE][position_2],
//                            hands[TURN_PLAYER_3 MINUS_ONE][position_3],
//                            hands[TURN_PLAYER_1 MINUS_ONE][position_1]);
//
//            removeCard(&hands[TURN_PLAYER_1 MINUS_ONE][position_1]);
//            removeCard(&hands[TURN_PLAYER_2 MINUS_ONE][position_2]);
//            removeCard(&hands[TURN_PLAYER_3 MINUS_ONE][position_3]);
//            break;
//
//          case TURN_PLAYER_3:
//            start = highest(bool_trump, TURN_PLAYER_3,
//                            hands[TURN_PLAYER_3 MINUS_ONE][position_3],
//                            hands[TURN_PLAYER_1 MINUS_ONE][position_1],
//                            hands[TURN_PLAYER_2 MINUS_ONE][position_2]);
//
//            removeCard(&hands[TURN_PLAYER_1 MINUS_ONE][position_1]);
//            removeCard(&hands[TURN_PLAYER_2 MINUS_ONE][position_2]);
//            removeCard(&hands[TURN_PLAYER_3 MINUS_ONE][position_3]);
//            break;
//
//          default:
//            break;
//        }
//      }
      
      // following turns
      if (counter_turns == 0)
        counter_turns++;
      
      if (start == buffer_start && counter_turns > 0)
      {
        // TODO:  all turns after first one
        //        copying BAUERNSCHNAPSER might be enough
        printf("TURN: %d\n", counter_turns + 1);
        
        // call
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");
        
        do
        {
          system ("/bin/stty raw");
          buffer = getchar();
    //      position = &buffer;
          printf("\r");
          if (!(check = seekAndDestroy(buffer, /*commands_1*/ commands_players[0])))
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (!check);
        system ("/bin/stty cooked");
        
        switch (buffer)
        {
          case 'q':
            position_1 = 0;
            playCard(&(hands)[call MINUS_ONE][position_1]);
            printHand(hands[call MINUS_ONE], 6, call,
                      players[call MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'w':
            position_1 = 1;
            playCard(&(hands)[call MINUS_ONE][position_1]);
            printHand(hands[call MINUS_ONE], 6, call,
                      players[call MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'e':
            position_1 = 2;
            playCard(&(hands)[call MINUS_ONE][position_1]);
            printHand(hands[call MINUS_ONE], 6, call,
                      players[call MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'a':
            position_1 = 3;
            playCard(&(hands)[call MINUS_ONE][position_1]);
            printHand(hands[call MINUS_ONE], 6, call,
                      players[call MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 's':
            position_1 = 4;
            playCard(&(hands)[call MINUS_ONE][position_1]);
            printHand(hands[call MINUS_ONE], 6, call,
                      players[call MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'd':
            position_1 = 5;
            playCard(&(hands)[call MINUS_ONE][position_1]);
            printHand(hands[call MINUS_ONE], 6, call,
                      players[call MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          default:
            break;
        }
        
        // answer 1
        counter_1 = 1;
        counter_2 = 0;
        
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");
        
        // rewind bock
        if (hands[answer_1 MINUS_ONE][0].is_bock_ == TRUE)
        {
          for (counter_cards = 0; counter_cards < HAND; counter_cards++)
          {
            hands[answer_1 MINUS_ONE][counter_cards].is_bock_ = FALSE;
          }
        }
        
        // check position(s) of valid cards for this round
        // do not allow any other card
        for (counter = 0; counter < strlen(commands_2); counter++)
          commands_2[counter] = '\0';
        
        counter_suit = 0;
        counter_trump = 0;
        // right suit
        for (counter = 0; counter < HAND; counter++)
        {
          if (hands[call - 1][position_1].suit_
              == hands[answer_1 - 1][counter].suit_)
          {
            counter_suit++;
          }
        }
        
        // no suit but maybe trump
    //    if (counter_suit > 1)
        if (counter_suit < 1)
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[answer_1 - 1][counter].suit_ == decodeSuit(trump))
            {
              counter_trump++;
            }
          }
        }
        
        // no trump || trump and sign
        if (counter_trump == 0 || (counter_trump > 0 && counter_suit > 0))
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[call - 1][position_1].suit_
                == hands[answer_1 - 1][counter].suit_)
            {
              commands_2[counter_2] = commands[counter];
              counter_2++;
            }
          }
        }
        
        // trump alone
        else
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[answer_1 - 1][counter].suit_ == decodeSuit(trump))
            {
              commands_2[counter_2] = commands[counter];
              counter_2++;
            }
          }
        }
        
        // deny wrong usage
        for (counter = counter_2; counter < HAND; counter++)
        {
          commands_2[counter] = '\0';
        }
        
        // no valid suit
        // no valid value
        if (commands_2[0] == '\0')
        {
    //      strcpy(commands_2, commands);
          counter = 0;
          counter_cards = 0;
          while (counter_cards < HAND)
          {
            if (commands_2_left[counter_cards] == '0')
            {
              counter_cards++;
            }
            
            if (commands_2_left[counter_cards] != '0')
            {
              commands_2[counter] = commands_2_left[counter_cards];
              counter++;
            }
            
            counter_cards++;
          }
          
          for (counter_cards = 0; counter_cards < HAND; counter_cards++)
          {
            hands[answer_1 - 1][counter_cards].is_bock_ = TRUE;
          }
        }
        
        // ---------------------
        // STUCHZWANG (answer 1)
        counter_commands  = 0;
        counter_cards     = 0;
        
        if (!hands[answer_1 MINUS_ONE][0].is_bock_)
        {
          while (commands_2[counter_commands] != '\0')
          {
            if (hands[call MINUS_ONE][position_1].suit_
                == hands[answer_1 MINUS_ONE][decomposeQWEASD(commands_2[counter_commands])].suit_)
            {
              if (hands[answer_1 MINUS_ONE][decomposeQWEASD(commands_2[counter_commands])].value_ >
                  hands[call MINUS_ONE][position_1].value_)
              {
                buffer_higher[counter_cards] = commands_2[counter_cards];
                buffer_higher[counter_cards ADD_ONE] = '\0';
                counter_cards++;
              }
            }
            
            else if (hands[answer_1 MINUS_ONE][decomposeQWEASD(commands_2[counter_commands])].is_trump_)
            {
              buffer_higher[counter_cards] = commands_2[counter_cards];
              buffer_higher[counter_cards ADD_ONE] = '\0';
              counter_cards++;
            }
            counter_commands++;
          }
        }
        
        if (counter_cards > 0)
        {
          strcpy(commands_2, "\0\0\0\0\0\0");
          strcpy(commands_2, buffer_higher);
          strcpy(buffer_higher, "\0\0\0\0\0\0");
        }
        // ---------------------
        
        do
        {
          system ("/bin/stty raw");
          buffer = getchar();
          printf("\r");
          if (!(check = seekAndDestroy(buffer, commands_2)))
            printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
        } while (!check);
        system ("/bin/stty cooked");
        
        switch (buffer)
        {
          case 'q':
            position_2 = 0;
            playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
            printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                      players[answer_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'w':
            position_2 = 1;
            playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
            printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                      players[answer_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'e':
            position_2 = 2;
            playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
            printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                      players[answer_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'a':
            position_2 = 3;
            playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
            printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                      players[answer_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 's':
            position_2 = 4;
            playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
            printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                      players[answer_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'd':
            position_2 = 5;
            playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
            printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                      players[answer_1 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          default:
            break;
        }
        
        // answer 2
        counter_1 = 2;
        counter_2 = 0;
        
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");
        
        // rewind bock
        if (hands[answer_1 - 1][0].is_bock_ == TRUE)
        {
          for (counter_cards = 0; counter_cards < HAND; counter_cards++)
          {
            hands[answer_1 - 1][counter_cards].is_bock_ = FALSE;
          }
          
        }
        
        // check position(s) of valid cards for this round
        // do not allow any other card
        for (counter = 0; counter < strlen(commands_2); counter++)
          commands_2[counter] = '\0';

        counter_suit = 0;
        counter_trump = 0;
        // right suit
        for (counter = 0; counter < HAND; counter++)
        {
          if (hands[call MINUS_ONE][position_1].suit_
              == hands[answer_2 MINUS_ONE][counter].suit_)
          {
            counter_suit++;
          }
        }
        
        // no suit but maybe trump
    //    if (counter_suit > 1)
        if (counter_suit < 1)
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[answer_2 MINUS_ONE][counter].suit_ == decodeSuit(trump))
            {
              counter_trump++;
            }
          }
        }
        
        // no trump || trump and sign
        if (counter_trump == 0 || (counter_trump > 0 && counter_suit > 0))
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[call MINUS_ONE][position_1].suit_
                == hands[answer_2 MINUS_ONE][counter].suit_)
            {
              commands_3[counter_2] = commands[counter];
              counter_2++;
            }
          }
        }
        
        // trump alone
        else
        {
          for (counter = 0; counter < HAND; counter++)
          {
            if (hands[answer_2 MINUS_ONE][counter].suit_ == decodeSuit(trump))
            {
              commands_3[counter_2] = commands[counter];
              counter_2++;
            }
          }
        }
        
        // deny wrong usage
        for (counter = counter_2; counter < HAND; counter++)
        {
          commands_3[counter] = '\0';
        }
        
        if (commands_3[0] == '\0')
        {
    //      strcpy(commands_3, commands);
          counter = 0;
          counter_cards = 0;
          while (counter_cards < HAND)
          {
            if (commands_3_left[counter_cards] == '0')
            {
              counter_cards++;
            }
            
            if (commands_3_left[counter_cards] != '0')
            {
              commands_3[counter] = commands_3_left[counter_cards];
              counter++;
            }
            
            counter_cards++;
          }
          
          for (counter_cards = 0; counter_cards < HAND; counter_cards++)
          {
            hands[answer_2 MINUS_ONE][counter_cards].is_bock_ = TRUE;
          }
        }
        
        // ---------------------
        // STUCHZWANG (answer 2)
        counter_commands  = 0;
        counter_cards     = 0;
        if (!hands[answer_2 MINUS_ONE][0].is_bock_)
        {
          while (commands_3[counter_commands] != '\0')
          {
            // must only match called suit
            if (hands[call MINUS_ONE][position_1].suit_
                == hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].suit_)
            {
              // case: answer 1's and answer 2's played matching suit
              if (hands[answer_1 MINUS_ONE][position_2].suit_ ==
                  hands[call MINUS_ONE][position_1].suit_)
              {
                // answer 1 has already trumped
                if (hands[answer_1 MINUS_ONE][position_2].value_
                    > hands[call MINUS_ONE][position_1].value_)
                {
                  // can answer 2 trump answer 1?
                  if (hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_
                      > hands[answer_1 MINUS_ONE][position_2].value_)
                  {
                    buffer_higher[counter_cards] = commands_3[counter_cards];
                    buffer_higher[counter_cards ADD_ONE] = '\0';
                    counter_cards++;
                  }
                }
                else if (hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_
                         > hands[call MINUS_ONE][position_1].value_)
                {
                  buffer_higher[counter_cards] = commands_3[counter_cards];
                  buffer_higher[counter_cards ADD_ONE] = '\0';
                  counter_cards++;
                }
              }
              // case: answer 1 played trump
              else
              {
                if (hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_
                    > hands[call MINUS_ONE][position_1].value_)
                {
                  buffer_higher[counter_cards] = commands_3[counter_cards];
                  buffer_higher[counter_cards ADD_ONE] = '\0';
                  counter_cards++;
                }
              }
            }
            // else they must utilize trump - and in case trump answer 1's trump
            else if (hands[answer_1 MINUS_ONE][position_2].is_trump_)
            {
              if (hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_
                  > hands[answer_1 MINUS_ONE][position_2].value_)
              {
                buffer_higher[counter_cards] = commands_3[counter_cards];
                buffer_higher[counter_cards ADD_ONE] = '\0';
                counter_cards++;
              }
            }
            counter_commands++;
          }
        }
        
        if (counter_cards > 0)
        {
          strcpy(commands_3, "\0\0\0\0\0\0");
          strcpy(commands_3, buffer_higher);
          strcpy(buffer_higher, "\0\0\0\0\0\0");
        }
        // ---------------------
        
        do
        {
          system ("/bin/stty raw");
          buffer = getchar();
          printf("\r");
          if (!(check = seekAndDestroy(buffer, commands_3)))
            printf ("Invalid input!\n");// printf("Gibt's nicht!\n");
        } while (!check);
        system ("/bin/stty cooked");
        
        switch (buffer)
        {
          case 'q':
            position_3 = 0;
            playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
            printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                      players[answer_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'w':
            position_3 = 1;
            playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
            printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                      players[answer_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'e':
            position_3 = 2;
            playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
            printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                      players[answer_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'a':
            position_3 = 3;
            playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
            printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                      players[answer_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 's':
            position_3 = 4;
            playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
            printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                      players[answer_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          case 'd':
            position_3 = 5;
            playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
            printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                      players[answer_2 MINUS_ONE].name_);
            printf("---------------------------------------------------------------\n");
            break;
            
          default:
            break;
        }
      }
      
      else /* if (start != buffer_start) */
      {
        printf("FOLD!\n"); //printf("TOT!\n");
        printf("---------------------------------------------------------------\n");

        points_and_caller.points_ = 12;
        points_and_caller.winner_ = OPPONENTS_WIN;
        points_and_caller.caller_ = buffer_start;
        return points_and_caller;

        break;
      }
    } // end for()

    printf("test: contra bauer won!\n");
    points_and_caller.points_ = 12;
    points_and_caller.winner_ = buffer_start;
    points_and_caller.caller_ = buffer_start;
    
    return points_and_caller;
  }
}

Points modeJodler(Card** hands, int start, char* suit, Player* players)
{
  // all cards of trump
//  printf("%s\n", suit);
  
  int counter = 0;
  int counter_turns = 0;
  int counter_1 = 0;
  int counter_cards = 0;
  int counter_suits = 0;
  int counter_commands = 0;
  int buffer_start = 0;
  buffer_start = start;
  char buffer = 0;
  
  char commands[7] = "qweasd";
  char commands_1[7] = "0";
  char commands_2[7] = "0";
  char commands_3[7] = "0";
  char commands_2_left[7] = "qweasd";
  char commands_3_left[7] = "qweasd";
  strcpy(commands_1, commands);
//  strcpy(commands_2, commands);
//  strcpy(commands_3, commands);
  int position_1 = 0;
  int position_2 = 0;
  int position_3 = 0;
  
  // default
  int call      = TURN_PLAYER_1;
  int answer_1  = TURN_PLAYER_2;
  int answer_2  = TURN_PLAYER_3;
  
  int check = FALSE;
  int bool_trump = FALSE;
  
  Points points_and_caller = {0, 0, 0};
  char buffer_higher[6] = {'\0'};
  
  getCall(start, &call, &answer_1, &answer_2);
  
  for (counter = 0; counter < HAND; counter++)
  {
    if (hands[call MINUS_ONE][counter].suit_ == suit)
      counter_suits++;
  }
  
  if (counter_suits < 5)
  {
    printf("FOLD!\n"); //printf("TOT!\n");
    printf("---------------------------------------------------------------\n");
    
    points_and_caller.points_ = 18;
    points_and_caller.winner_ = OPPONENTS_WIN;
    points_and_caller.caller_ = buffer_start;
    return points_and_caller;
  }
  
  for (counter_turns = 0; counter_turns < MAXIMUM_TURNS; counter_turns++)
  {
    printf("TURN: %d\n", counter_turns + 1);
    
    // call
    printHand(hands[call MINUS_ONE], 6, call,
              players[call MINUS_ONE].name_);
    printf("                         ^     ^     ^     ^     ^     ^  \n");
    printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
    printf("---------------------------------------------------------------\n");
    
    do
    {
      system ("/bin/stty raw");
      buffer = getchar();
//      position = &buffer;
      printf("\r");
      if (!(check = seekAndDestroy(buffer, commands_1)))
        printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
    } while (!check);
    system ("/bin/stty cooked");
    
    switch (buffer)
    {
      case 'q':
        position_1 = 0;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'w':
        position_1 = 1;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'e':
        position_1 = 2;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'a':
        position_1 = 3;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 's':
        position_1 = 4;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'd':
        position_1 = 5;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      default:
        break;
    }
    
    // answer 1
    counter_1 = 0;
    
    printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
              players[answer_1 MINUS_ONE].name_);
    printf("                         ^     ^     ^     ^     ^     ^  \n");
    printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
    printf("---------------------------------------------------------------\n");
    
    // rewind bock
    if (hands[answer_1 - 1][0].is_bock_ == TRUE)
    {
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_1 - 1][counter_cards].is_bock_ = FALSE;
      }
    }
    
    // check position(s) of valid cards for this round
    // do not allow any other card
    for (counter = 0; counter < strlen(commands_2); counter++)
      commands_2[counter] = '\0';
    for (counter = 0; counter < HAND; counter++)
    {
      if (hands[call - 1][position_1].suit_
          == hands[answer_1 - 1][counter].suit_)
      {
        commands_2[counter_1] = commands[counter];
        counter_1++;
      }
//      printf("1. %s, 2. %s\n", hands[call - 1][position_1].suit_,
//             hands[answer_1 - 1][counter].suit_);
    }
    
    // deny wrong usage
    for (counter = counter_1; counter < HAND; counter++)
    {
      commands_2[counter] = '\0';
    }
    
    // no valid suit
    // no valid value
    if (commands_2[0] == '\0')
    {
//      strcpy(commands_2, commands);
      counter = 0;
      counter_cards = 0;
      while (counter_cards < HAND)
      {
        if (commands_2_left[counter_cards] == '0')
        {
          counter_cards++;
        }
        
        if (commands_2_left[counter_cards] != '0')
        {
          commands_2[counter] = commands_2_left[counter_cards];
          counter++;
        }
        
        counter_cards++;
      }
      
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_1 - 1][counter_cards].is_bock_ = TRUE;
      }
    }
    
    // STUCHZWANG
    counter_commands  = 0;
    counter_cards     = 0;
    
    if (!hands[answer_1 MINUS_ONE][0].is_bock_)
    {
      while (commands_2[counter_commands] != '\0')
      {
        if (hands[answer_1 MINUS_ONE][decomposeQWEASD(commands_2[counter_commands])].value_ >
            hands[call MINUS_ONE][position_1].value_)
        {
          buffer_higher[counter_cards] = commands_2[counter_cards];
          buffer_higher[counter_cards ADD_ONE] = '\0';
          counter_cards++;
        }
        counter_commands++;
      }
    }
    
    if (counter_cards > 0)
    {
      strcpy(commands_2, "\0\0\0\0\0\0");
      strcpy(commands_2, buffer_higher);
      strcpy(buffer_higher, "\0\0\0\0\0\0");
    }
    
    do
    {
      system ("/bin/stty raw");
      buffer = getchar();
      printf("\r");
      if (!(check = seekAndDestroy(buffer, commands_2)))
        printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
    } while (!check);
    system ("/bin/stty cooked");
    
    switch (buffer)
    {
      case 'q':
        position_2 = 0;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'w':
        position_2 = 1;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'e':
        position_2 = 2;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'a':
        position_2 = 3;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 's':
        position_2 = 4;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'd':
        position_2 = 5;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      default:
        break;
    }
    
    // answer 2
    counter_1 = 0;
    printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
              players[answer_2 MINUS_ONE].name_);
    printf("                         ^     ^     ^     ^     ^     ^  \n");
    printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
    printf("---------------------------------------------------------------\n");
    
    // rewind bock
    if (hands[answer_1 - 1][0].is_bock_ == TRUE)
    {
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_1 - 1][counter_cards].is_bock_ = FALSE;
      }
      
    }
    
    // check position(s) of valid cards for this round
    // do not allow any other card
    for (counter = 0; counter < strlen(commands_2); counter++)
      commands_2[counter] = '\0';
    for (counter = 0; counter < HAND; counter++)
    {
      if (hands[call - 1][position_1].suit_
          == hands[answer_2 - 1][counter].suit_)
      {
        commands_3[counter_1] = commands[counter];
        counter_1++;
      }
    }
    
    // deny wrong usage
    for (counter = counter_1; counter < HAND; counter++)
    {
      commands_3[counter] = '\0';
    }
    
    if (commands_3[0] == '\0')
    {
//      strcpy(commands_3, commands);
      counter = 0;
      counter_cards = 0;
      while (counter_cards < HAND)
      {
        if (commands_3_left[counter_cards] == '0')
        {
          counter_cards++;
        }
        
        if (commands_3_left[counter_cards] != '0')
        {
          commands_3[counter] = commands_3_left[counter_cards];
          counter++;
        }
        
        counter_cards++;
      }
      
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_2 - 1][counter_cards].is_bock_ = TRUE;
      }
    }
    
    // STUCHZWANG
    counter_commands  = 0;
    counter_cards     = 0;
    
    if (!hands[answer_2 MINUS_ONE][0].is_bock_)
    {
      while (commands_3[counter_commands] != '\0')
      {
        if (hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_ >
            hands[call MINUS_ONE][position_1].value_
            || hands[answer_2 MINUS_ONE][decomposeQWEASD(commands_3[counter_commands])].value_ >
                hands[answer_1 MINUS_ONE][position_2].value_)
        {
          buffer_higher[counter_cards] = commands_3[counter_cards];
          buffer_higher[counter_cards ADD_ONE] = '\0';
          counter_cards++;
        }
        counter_commands++;
      }
    }
    
    if (counter_cards > 0)
    {
      strcpy(commands_3, "\0\0\0\0\0\0");
      strcpy(commands_3, buffer_higher);
      strcpy(buffer_higher, "\0\0\0\0\0\0");
    }
    
    do
    {
      system ("/bin/stty raw");
      buffer = getchar();
      printf("\r");
      if (!(check = seekAndDestroy(buffer, commands_3)))
        printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
    } while (!check);
    system ("/bin/stty cooked");
    
    switch (buffer)
    {
      case 'q':
        position_3 = 0;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'w':
        position_3 = 1;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'e':
        position_3 = 2;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'a':
        position_3 = 3;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 's':
        position_3 = 4;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'd':
        position_3 = 5;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      default:
        break;
    }
    
    // who has played the highest card
    start = highest(bool_trump, start, hands[call - 1][position_1],
                    hands[answer_1 - 1][position_2],
                    hands[answer_2 - 1][position_3]);
    
    // delete or do not display played cards anymore
    removeCard(&hands[call - 1][position_1]);
    removeCard(&hands[answer_1 - 1][position_2]);
    removeCard(&hands[answer_2 - 1][position_3]);
    
    // calling player must not lose one round
    if (start != buffer_start)
    {
      printf("FOLD!\n"); //printf("TOT!\n");
      printf("---------------------------------------------------------------\n");
      
      points_and_caller.points_ = 18;
      points_and_caller.winner_ = OPPONENTS_WIN;
      points_and_caller.caller_ = buffer_start;
      return points_and_caller;
      break;
    }
  } // end for()
  
  printf("test: jodler won!\n");
  points_and_caller.points_ = 18;
  points_and_caller.winner_ = buffer_start;
  points_and_caller.caller_ = buffer_start;
  return points_and_caller;
}

// ----------------------------------------------------------------------------
///
/// might not even need STUCHZWANG, since at the end there will only be one card left for each player
///
//
Points modeHerrenJodler(Card** hands, int start, char* trump, Player* players)
{
  // all cards of trump
  
  int counter = 0;
  int counter_turns = 0;
  int counter_1 = 0;
  int counter_cards = 0;
  int counter_trump = 0;
  int buffer_start = 0;
  buffer_start = start;
  char buffer = 0;
  
  char commands[7] = "qweasd";
  char commands_1[7] = "0";
  char commands_2[7] = "0";
  char commands_3[7] = "0";
  char commands_2_left[7] = "qweasd";
  char commands_3_left[7] = "qweasd";
  strcpy(commands_1, commands);
//  strcpy(commands_2, commands);
//  strcpy(commands_3, commands);
  int position_1 = 0;
  int position_2 = 0;
  int position_3 = 0;
  
  // default
  int call      = TURN_PLAYER_1;
  int answer_1  = TURN_PLAYER_2;
  int answer_2  = TURN_PLAYER_3;
  
  int check = FALSE;
  int bool_trump = TRUE;
  
  Points points_and_caller = {0, 0, 0};
  
  getCall(start, &call, &answer_1, &answer_2);
  
  for (counter = 0; counter < HAND; counter++)
  {
    if (hands[call MINUS_ONE][counter].suit_ == trump)
      counter_trump++;
  }
  
  if (counter_trump < 5)
  {
    printf("FOLD!\n"); //printf("TOT!\n");
    printf("---------------------------------------------------------------\n");
    
    points_and_caller.points_ = 24;
    points_and_caller.winner_ = OPPONENTS_WIN;
    points_and_caller.caller_ = buffer_start;
    return points_and_caller;
  }
  
  for (counter_turns = 0; counter_turns < MAXIMUM_TURNS; counter_turns++)
  {
    printf("TURN: %d\n", counter_turns + 1);
    
    // call
    printHand(hands[call MINUS_ONE], 6, call,
              players[call MINUS_ONE].name_);
    printf("                         ^     ^     ^     ^     ^     ^  \n");
    printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
    printf("---------------------------------------------------------------\n");
    
    do
    {
      system ("/bin/stty raw");
      buffer = getchar();
//      position = &buffer;
      printf("\r");
      if (!(check = seekAndDestroy(buffer, commands_1)))
        printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
    } while (!check);
    system ("/bin/stty cooked");
    
    switch (buffer)
    {
      case 'q':
        position_1 = 0;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'w':
        position_1 = 1;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'e':
        position_1 = 2;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'a':
        position_1 = 3;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 's':
        position_1 = 4;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'd':
        position_1 = 5;
        playCard(&(hands)[call MINUS_ONE][position_1]);
        printHand(hands[call MINUS_ONE], 6, call,
                  players[call MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      default:
        break;
    }
    
    // answer 1
    counter_1 = 0;
    
    printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
              players[answer_1 MINUS_ONE].name_);
    printf("                         ^     ^     ^     ^     ^     ^  \n");
    printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
    printf("---------------------------------------------------------------\n");
    
    // rewind bock
    if (hands[answer_1 - 1][0].is_bock_ == TRUE)
    {
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_1 - 1][counter_cards].is_bock_ = FALSE;
      }
    }
    
    // check position(s) of valid cards for this round
    // do not allow any other card
    for (counter = 0; counter < strlen(commands_2); counter++)
      commands_2[counter] = '\0';
    for (counter = 0; counter < HAND; counter++)
    {
      if (hands[call - 1][position_1].suit_
          == hands[answer_1 - 1][counter].suit_)
      {
        commands_2[counter_1] = commands[counter];
        counter_1++;
      }
//      printf("1. %s, 2. %s\n", hands[call - 1][position_1].suit_,
//             hands[answer_1 - 1][counter].suit_);
    }
    
    // deny wrong usage
    for (counter = counter_1; counter < HAND; counter++)
    {
      commands_2[counter] = '\0';
    }
    
    // no valid suit
    // no valid value
    if (commands_2[0] == '\0')
    {
//      strcpy(commands_2, commands);
      counter = 0;
      counter_cards = 0;
      while (counter_cards < HAND)
      {
        if (commands_2_left[counter_cards] == '0')
        {
          counter_cards++;
        }
        
        if (commands_2_left[counter_cards] != '0')
        {
          commands_2[counter] = commands_2_left[counter_cards];
          counter++;
        }
        
        counter_cards++;
      }
      
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_1 - 1][counter_cards].is_bock_ = TRUE;
      }
    }
    
    do
    {
      system ("/bin/stty raw");
      buffer = getchar();
      printf("\r");
      if (!(check = seekAndDestroy(buffer, commands_2)))
        printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
    } while (!check);
    system ("/bin/stty cooked");
    
    switch (buffer)
    {
      case 'q':
        position_2 = 0;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'w':
        position_2 = 1;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'e':
        position_2 = 2;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'a':
        position_2 = 3;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 's':
        position_2 = 4;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'd':
        position_2 = 5;
        playCard(&(hands)[answer_1 MINUS_ONE][position_2]);
        printHand(hands[answer_1 MINUS_ONE], 6, answer_1,
                  players[answer_1 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      default:
        break;
    }
    
    // answer 2
    counter_1 = 0;
    printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
              players[answer_2 MINUS_ONE].name_);
    printf("                         ^     ^     ^     ^     ^     ^  \n");
    printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
    printf("---------------------------------------------------------------\n");
    
    // rewind bock
    if (hands[answer_1 - 1][0].is_bock_ == TRUE)
    {
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_1 - 1][counter_cards].is_bock_ = FALSE;
      }
      
    }
    
    // check position(s) of valid cards for this round
    // do not allow any other card
    for (counter = 0; counter < strlen(commands_2); counter++)
      commands_2[counter] = '\0';
    for (counter = 0; counter < HAND; counter++)
    {
      if (hands[call - 1][position_1].suit_
          == hands[answer_2 - 1][counter].suit_)
      {
        commands_3[counter_1] = commands[counter];
        counter_1++;
      }
    }
    
    // deny wrong usage
    for (counter = counter_1; counter < HAND; counter++)
    {
      commands_3[counter] = '\0';
    }
    
    if (commands_3[0] == '\0')
    {
//      strcpy(commands_3, commands);
      counter = 0;
      counter_cards = 0;
      while (counter_cards < HAND)
      {
        if (commands_3_left[counter_cards] == '0')
        {
          counter_cards++;
        }
        
        if (commands_3_left[counter_cards] != '0')
        {
          commands_3[counter] = commands_3_left[counter_cards];
          counter++;
        }
        
        counter_cards++;
      }
      
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[answer_2 - 1][counter_cards].is_bock_ = TRUE;
      }
    }
    
    do
    {
      system ("/bin/stty raw");
      buffer = getchar();
      printf("\r");
      if (!(check = seekAndDestroy(buffer, commands_3)))
        printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
    } while (!check);
    system ("/bin/stty cooked");
    
    switch (buffer)
    {
      case 'q':
        position_3 = 0;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'w':
        position_3 = 1;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'e':
        position_3 = 2;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'a':
        position_3 = 3;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 's':
        position_3 = 4;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      case 'd':
        position_3 = 5;
        playCard(&(hands)[answer_2 MINUS_ONE][position_3]);
        printHand(hands[answer_2 MINUS_ONE], 6, answer_2,
                  players[answer_2 MINUS_ONE].name_);
        printf("---------------------------------------------------------------\n");
        break;
        
      default:
        break;
    }
    
    // who has played the highest card
    start = highest(bool_trump, start, hands[call - 1][position_1],
                    hands[answer_1 - 1][position_2],
                    hands[answer_2 - 1][position_3]);
    
    // delete or do not display played cards anymore
    // dh. delete suits
    removeCard(&hands[call - 1][position_1]);
    removeCard(&hands[answer_1 - 1][position_2]);
    removeCard(&hands[answer_2 - 1][position_3]);
    
    // calling player must not lose one round
    if (start != buffer_start)
    {
      printf("FOLD!\n"); //printf("TOT!\n");
      printf("---------------------------------------------------------------\n");
      
      points_and_caller.points_ = 24;
      points_and_caller.winner_ = OPPONENTS_WIN;
      points_and_caller.caller_ = buffer_start;
      return points_and_caller;
      break;
    }
  } // end for()
  
  printf("test herren_jodler won.\n");
  points_and_caller.points_ = 24;
  points_and_caller.winner_ = buffer_start;
  points_and_caller.caller_ = buffer_start;
  return points_and_caller;
}

int buy(int start, Card** hands, Card* deck_dealer, int* done_1,
        int* done_2, Player* players, int* order)
{
  int buffer = 0;
  
  if (*done_1 == FALSE && *done_2 == FALSE)
  {
    printf("Player %d (%s), do you buy one?\n", start,
           players[order[start MINUS_ONE]].name_);
    printf("---------------------------------------------------------------\n");
    printHand(deck_dealer, 2, TURN_DEALER, "TALON_");
    printf("                ^     ^   'NO!'\n");
    printf("               |1|   |2|   |0|\n");
    printf("---------------------------------------------------------------\n");
    
  }
  
  if (*done_1 == TRUE || *done_2 == TRUE)
  {
    if (*done_1 == TRUE)
    {
      printf("Player %d (%s), do you buy the other one?\n", start,
             players[order[start MINUS_ONE]].name_);
      printf("---------------------------------------------------------------\n");
//      printHand(deck_dealer, 2, TURN_DEALER);
      printf("TALON:              [%s %s]\n", deck_dealer[1].sign_,
             deck_dealer[1].image_);
      printf("                      ^   'NO!'\n");
      printf("                     |2|   |0|\n");
      printf("---------------------------------------------------------------\n");
    }
    
    else /*if (*done_2 == TRUE)*/
    {
      printf("Player %d (%s), do you buy the other one?\n", start,
             players[order[start MINUS_ONE]].name_);
      printf("---------------------------------------------------------------\n");
//      printHand(deck_dealer, 2, TURN_DEALER);
      printf("TALON:        [%s %s]\n", deck_dealer[0].sign_,
             deck_dealer[0].image_);
      printf("                ^         'NO!'\n");
      printf("               |1|         |0|\n");
      printf("---------------------------------------------------------------\n");
    }
    
  }
  
  // two replacements done
  if (*done_1 == TRUE && *done_2 == TRUE)
  {
    return BREAK;
  }
  
  do
  {
    system ("/bin/stty raw");
    buffer = getchar();
    printf("\r");
    if ((buffer != '1' && buffer != '2' && buffer != '0')
        || (buffer == '1' && *done_1 == TRUE)
        || (buffer == '2' && *done_2 == TRUE))
      printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
  } while (buffer != '1' && buffer != '2' && buffer != '0');
  system ("/bin/stty cooked");
  
  if (buffer == '0' /*|| (*done_1 == TRUE && *done_2 == TRUE)*/)
  {
    return BREAK;
  }
  
  // replace with talon card 1
  else if (buffer == '1' && *done_1 == FALSE)
  {
    printf("What would you want to replace with [%s %s]\n",
           deck_dealer[0].sign_, deck_dealer[0].image_);
    *done_1 = TRUE;
    
    switch (start)
    {
      case TURN_PLAYER_1:
        printHand(hands[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                  players[order[TURN_PLAYER_1 MINUS_ONE]].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");
        break;
        
      case TURN_PLAYER_2:
        printHand(hands[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                  players[order[TURN_PLAYER_2 MINUS_ONE]].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");
        break;
        
      case TURN_PLAYER_3:
        printHand(hands[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                  players[order[TURN_PLAYER_3 MINUS_ONE]].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");
        break;
        
      default:
        break;
    }
    
    // TODO: switch
    
    do
    {
      system ("/bin/stty raw");
      buffer = getchar();
      printf("\r");
      if (buffer != 'q' && buffer != 'w' && buffer != 'e' && buffer != 'a'
          && buffer != 's' && buffer != 'd')
        printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
    } while (buffer != 'q' && buffer != 'w' && buffer != 'e' && buffer != 'a'
             && buffer != 's' && buffer != 'd');
    system ("/bin/stty cooked");
    
    switch (buffer)
    {
      case 'q':
        switchCards(&(hands)[start MINUS_ONE][0], &(deck_dealer)[0]);
//        printHand(hands[start MINUS_ONE], 6, 1,
//                  players[order[start MINUS_ONE]].name_);
        break;
        
      case 'w':
        switchCards(&(hands)[start MINUS_ONE][1], &(deck_dealer)[0]);
//        printHand(hands[start MINUS_ONE], 6, 1,
//                  players[order[start MINUS_ONE]].name_);
        break;
        
      case 'e':
        switchCards(&(hands)[start MINUS_ONE][2], &(deck_dealer)[0]);
//        printHand(hands[start MINUS_ONE], 6, 1,
//                  players[order[start MINUS_ONE]].name_);
        break;
        
      case 'a':
        switchCards(&(hands)[start MINUS_ONE][3], &(deck_dealer)[0]);
//        printHand(hands[start MINUS_ONE], 6, 1,
//                  players[order[start MINUS_ONE]].name_);
        break;
        
      case 's':
        switchCards(&(hands)[start MINUS_ONE][4], &(deck_dealer)[0]);
//        printHand(hands[start MINUS_ONE], 6, 1,
//                  players[order[start MINUS_ONE]].name_);
        break;
        
      case 'd':
        switchCards(&(hands)[start MINUS_ONE][5], &(deck_dealer)[0]);
//        printHand(hands[start MINUS_ONE], 6, 1,
//                  players[order[start MINUS_ONE]].name_);
        break;
        
      default:
        break;
    }
    
    return GO_ON;
  }
  
  // replace with talon card 2
  else if (buffer == '2' && *done_2 == FALSE)
  {
    printf("What would you want to replace with [%s %s]\n",
           deck_dealer[1].sign_, deck_dealer[1].image_);
    *done_2 = TRUE;
    
    switch (start)
    {
      case TURN_PLAYER_1:
        printHand(hands[TURN_PLAYER_1 MINUS_ONE], 6, TURN_PLAYER_1,
                  players[order[TURN_PLAYER_1 MINUS_ONE]].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");
        break;
        
      case TURN_PLAYER_2:
        printHand(hands[TURN_PLAYER_2 MINUS_ONE], 6, TURN_PLAYER_2,
                  players[order[TURN_PLAYER_2 MINUS_ONE]].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");
        break;
        
      case TURN_PLAYER_3:
        printHand(hands[TURN_PLAYER_3 MINUS_ONE], 6, TURN_PLAYER_3,
                  players[order[TURN_PLAYER_3 MINUS_ONE]].name_);
        printf("                         ^     ^     ^     ^     ^     ^  \n");
        printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
        printf("---------------------------------------------------------------\n");
        break;
        
      default:
        break;
    }
    
    do
    {
      system ("/bin/stty raw");
      buffer = getchar();
      printf("\r");
      if (buffer != 'q' && buffer != 'w' && buffer != 'e' && buffer != 'a'
          && buffer != 's' && buffer != 'd')
        printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
    } while (buffer != 'q' && buffer != 'w' && buffer != 'e' && buffer != 'a'
             && buffer != 's' && buffer != 'd');
    system ("/bin/stty cooked");
    
    switch (buffer)
    {
      case 'q':
        switchCards(&(hands)[start MINUS_ONE][0], &(deck_dealer)[1]);
        printHand(hands[start MINUS_ONE], 6, start,
                  players[order[start MINUS_ONE]].name_);
        break;
        
      case 'w':
        switchCards(&(hands)[start MINUS_ONE][1], &(deck_dealer)[1]);
        printHand(hands[start MINUS_ONE], 6, start/*1*/,
                  players[order[start MINUS_ONE]].name_);                   // better: start
        break;
        
      case 'e':
        switchCards(&(hands)[start MINUS_ONE][2], &(deck_dealer)[1]);
        printHand(hands[start MINUS_ONE], 6, start,
                  players[order[start MINUS_ONE]].name_);
        break;
        
      case 'a':
        switchCards(&(hands)[start MINUS_ONE][3], &(deck_dealer)[1]);
        printHand(hands[start MINUS_ONE], 6, start,
                  players[order[start MINUS_ONE]].name_);
        break;
        
      case 's':
        switchCards(&(hands)[start MINUS_ONE][4], &(deck_dealer)[1]);
        printHand(hands[start MINUS_ONE], 6, start,
                  players[order[start MINUS_ONE]].name_);
        break;
        
      case 'd':
        switchCards(&(hands)[start MINUS_ONE][5], &(deck_dealer)[1]);
        printHand(hands[start MINUS_ONE], 6, start,
                  players[order[start MINUS_ONE]].name_);
        break;
        
      default:
        break;
    }
    
    return GO_ON;
  }
  
  // formal "else" will never be executed - as far as I know
  else
    return GO_ON;
}

void switchCards(Card* a, Card* b)
{
  Card buffer  = {NULL, 0, NULL, NULL};
  
//  printf("|%s %s| |%s %s|\n", a->sign_, a->image_, b->sign_, b->image_);
  
  buffer     = *a;
  *a = *b;
  *b  = buffer;
}

int highest(int bool_trump, int start, Card call, Card answer_1,
            Card answer_2)
{
  if (bool_trump == FALSE)
  {
    printf("1. %d, 2. %d, 3. %d\n", call.value_, answer_1.value_,
           answer_2.value_);
    {
      if (((call.value_ >= answer_1.value_ || call.suit_ != answer_1.suit_)
          && (call.value_ >= answer_2.value_ || call.suit_ != answer_2.suit_))    // überprüfen
          || ((answer_1.is_bock_ == TRUE && answer_2.value_ <= call.value_)
          && (answer_2.is_bock_ == TRUE && answer_1.value_ <= call.value_))
          || (answer_1.is_bock_ == TRUE && answer_2.is_bock_ == TRUE))
      {
        return start;
      }
      
      else if ((call.value_ < answer_1.value_ && call.suit_ == answer_1.suit_)
          && ((answer_2.value_ < answer_1.value_
               && answer_2.suit_ == answer_1.suit_)
              || (answer_2.suit_ != answer_1.suit_)))
      {
        if (start + 1 > 3)
          return TURN_PLAYER_1;
        else
          return start + 1;
      }
          
      else /* should not require a control check */
      {
        if (start + 2 > 3)
          return TURN_PLAYER_1;
        else
          return start + 2;
      }
    }
  }
  
  else /*if (bool_trump == TRUE)*/
  {
    printf("1. %d, 2. %d, 3. %d\n", call.value_, answer_1.value_,
           answer_2.value_);
    {
//      if (((call.value_ >= answer_1.value_ || call.suit_ != answer_1.suit_)
//          && (call.value_ >= answer_2.value_ || call.suit_ != answer_2.suit_))    // überprüfen
//          || ((answer_1.is_bock_ == TRUE && answer_2.value_ <= call.value_)
//          && (answer_2.is_bock_ == TRUE && answer_1.value_ <= call.value_))
//          || (answer_1.is_bock_ == TRUE && answer_2.is_bock_ == TRUE)
//          || ((call.value_ >= answer_1.value_
//              && call.is_trump_ == answer_1.is_trump_)
//              || (call.value_ >= answer_2.value_
//                  && call.is_trump_ == answer_2.is_trump_)))
      
      // caller trumps --------------------------------------------------------
      
      // trump called
      if (call.is_trump_)
      {
        // none of the answers played/holds trump
        if (call.is_trump_ && !answer_1.is_trump_ && !answer_2.is_trump_)
        {
          return start;
        }
        // call and answer 1 played trump
        if ((call.is_trump_ && answer_1.is_trump_)
            && (call.value_ > answer_1.value_))
        {
          return start;
        }
        // call and answer 2 played trump
        if ((call.is_trump_ && answer_2.is_trump_)
            && (call.value_ > answer_2.value_))
        {
          return start;
        }
        // all three players played trump
        if ((call.is_trump_ && answer_1.is_trump_ && answer_2.is_trump_)
            &&(call.value_ > answer_1.value_ && call.value_ > answer_2.value_))
        {
          return start;
        }
      }
      // no trump called
      else /* if (!call.is_trump_) */
      {
        // all three player played the same suit
        if ((call.suit_ == answer_1.suit_ && call.suit_ == answer_2.suit_)
            && (call.value_ > answer_1.value_ && call.value_ > answer_2.value_))
        {
          return start;
        }
        // no answer played the called suit, neither did anyone play/hold the right suit
        if ((call.suit_ != answer_1.suit_ && call.suit_ != answer_2.suit_)
            && (!answer_1.is_trump_ && !answer_2.is_trump_))
        {
          return start;
        }
//        // answer 1 played called suit
//        else if (call.suit_ == answer_1.suit_ && call.value_ > answer_1.value_)
//        {
//          return start;
//        }
//        // answer 2 played called suit
//        else if (call.suit_ == answer_2.suit_ && call.value_ > answer_2.value_)
//        {
//          return start;
//        }
        if (call.suit_ == answer_1.suit_ || call.suit_ == answer_2.suit_)
        {
          if (call.value_ > answer_1.value_ && call.value_ > answer_2.value_
              && (!answer_1.is_trump_ && !answer_2.is_trump_))
          {
            return start;
          }
        }
      }
      
      // caller being trumped -------------------------------------------------
      
      // trump called
      if (call.is_trump_)
      {
        // all three played trump
        if (/* call.is_trump_ && */ answer_1.is_trump_ && answer_2.is_trump_)
        {
          // both answers trump call
          if (call.value_ < answer_1.value_ && call.value_ < answer_2.value_)
          {
            if (answer_1.value_ > answer_2.value_)
            {
              if (start + 1 > 3)
                return TURN_PLAYER_1;
              else
                return start + 1;
            }
            
            else
            {
              if (start + 2 > 3)
                return TURN_PLAYER_1;
              else
                return start + 2;
            }
          }
          // one of the answers trumps call
          else /* if (call.value_ < answer_1.value_ || call.value_ < answer_2.value_) */
          {
            // answer 1 trumps call
            if (call.value_ < answer_1.value_)
            {
              if (start + 1 > 3)
                return TURN_PLAYER_1;
              else
                return start + 1;
            }
            // answer 2 trumps call
            else
            {
              if (start + 2 > 3)
                return TURN_PLAYER_1;
              else
                return start + 2;
            }
          }
        }
        // one of the answers played trump
        else /* if ((answer_1.is_trump_ && !answer_2.is_trump_)
            || (!answer_1.is_trump_ && answer_2.is_trump_)) */
        {
          // call and answer 1 played trump
          if ((answer_1.is_trump_ && !answer_2.is_trump_)
              && (call.value_ < answer_1.value_))
          {
            if (start + 1 > 3)
              return TURN_PLAYER_1;
            else
              return start + 1;
          }
          // call and answer 2 played trump
          else /* if ((!answer_1.is_trump_ && answer_2.is_trump_)
                   && (call.value_ < answer_2.value_)) */
          {
            if (start + 2 > 3)
              return TURN_PLAYER_1;
            else
              return start + 2;
          }
        }
      }
      // no trump called
      else
      {
        // all three players played called suit
        if (call.suit_ == answer_1.suit_ && call.suit_ == answer_2.suit_)
        {
          // both answers trump call
          if (call.value_ < answer_1.value_ && call.value_ < answer_2.value_)
          {
            if (answer_1.value_ < answer_2.value_)
            {
              if (start + 2 > 3)
                return TURN_PLAYER_1;
              else
                return start + 2;
            }
            else
            {
              if (start + 1 > 3)
                return TURN_PLAYER_1;
              else
                return start + 1;
            }
          }
          // one of the answers trumps call
          else /* if (call.value_ < answer_1.value_ || call.value_ < answer_2.value_) */
          {
            if (call.value_ < answer_1.value_)
            {
              if (start + 1 > 3)
                return TURN_PLAYER_1;
              else
                return start + 1;
            }
            else
            {
              if (start + 2 > 3)
                return TURN_PLAYER_1;
              else
                return start + 2;
            }
          }
        }
        // both answers played trump
        else if (answer_1.is_trump_ && answer_2.is_trump_)
        {
          if (answer_1.value_ > answer_2.value_)
          {
            if (start + 1 > 3)
              return TURN_PLAYER_1;
            else
              return start + 1;
          }
          else
          {
            if (start + 2 > 3)
              return TURN_PLAYER_1;
            else
              return start + 2;
          }
        }
        // one has suit the other hast trump or one hast trump the other has bock
        else if ((answer_1.is_trump_ && answer_2.is_bock_)
                 || (answer_1.is_trump_ && answer_2.suit_ == call.suit_))
        {
          if (start + 1 > 3)
            return TURN_PLAYER_1;
          else
            return start + 1;
        }
        else /* if ((answer_2.is_trump_ && answer_1.is_bock_)
                 || (answer_2.is_trump_ && answer_1.is_bock_)) */
        {
          if (start + 2 > 3)
            return TURN_PLAYER_1;
          else
            return start + 2;
        }
      }
      
//      else if (((call.value_ < answer_1.value_ && call.suit_ == answer_1.suit_)
//                || ((answer_1.value_ < call.value_)
//                && (answer_1.is_trump_ == TRUE)))
//                && ((answer_2.value_ < answer_1.value_
//                && answer_2.suit_ == answer_1.suit_)
//                || (answer_2.suit_ != answer_1.suit_))
//                && (answer_1.is_trump_ == TRUE))
//      {
//        if (start + 1 > 3)
//          return TURN_PLAYER_1;
//        else
//          return start + 1;
//      }
//
//      else /* should not require a control check */
//      {
//        if (start + 2 > 3)
//          return TURN_PLAYER_1;
//        else
//          return start + 2;
//      }
    }
  }
}

void playCard(Card* card)
{
  (*card).image_  = " ";
  (*card).sign_   = " ";       // simple, but you get the picture
//  (*card).suit_   = 0;
}

void removeCard(Card* card)
{
  (*card).value_    = 0;
  (*card).suit_     = 0;
  (*card).is_trump_  = FALSE;  // this should do it
}

int seekAndDestroy(char find, char* list)
{
  int position = 0;
  int counter = 0;
  // WorkAround ---
  long length = strlen(list) /* - 1 */;                   // I glaub da scheißt mir
                                                    // die Input-Funktion rein
                                                    // und hängt da irgendwie
                                                    // ebend den gesuchten
                                                    // character an
//  list[length - 1] = '\0';
  // ---
  
//  printf("%c", *(*list) + 1);
//  printf("%s\n", list);
  
//  for (counter = 0; counter < 6; counter++)
//  {
//    buffer[counter] = (*list)[counter];
//  }
  
  for (position = 0; position < length; position++)
  {
    if ((list)[position] == find)
    {
      break;
    }
    else
    {
      continue;
    }
  }
  
  if ((list)[position] == find)
  {
    for (counter = position; counter < length - 1; counter++)
    {
      (list)[counter] = (list)[counter + 1];  // vielleicht will er da irgendeinen string operator
                                                  // und in diesem Sinne, Gute Nacht :*
    }
    list[counter] = '\0';
    
//    printf("%s\n", list);
    return TRUE;
  }
  
  else
    return FALSE;
}

void getCall(int start, int* call, int* answer_1, int* answer_2)
{
  switch (start)
  {
    case TURN_PLAYER_1:
      *call      = TURN_PLAYER_1;
      *answer_1  = TURN_PLAYER_2;
      *answer_2  = TURN_PLAYER_3;
      break;
      
    case TURN_PLAYER_2:
      *call      = TURN_PLAYER_2;
      *answer_1  = TURN_PLAYER_3;
      *answer_2  = TURN_PLAYER_1;
      break;
      
    case TURN_PLAYER_3:
      *call      = TURN_PLAYER_3;
      *answer_1  = TURN_PLAYER_1;
      *answer_2  = TURN_PLAYER_2;
      break;
      
    default:
      break;
  }
}

char* decodeSuit(int trump)
{
  switch (trump)
  {
    case 1:
      return "clubs";
      break;
      
    case 2:
      return "spades";
      break;
      
    case 3:
      return "hearts";
      break;
      
    case 4:
      return "diamonds";
      break;
      
    default:
      break;
  }
  
  return ERROR_CODE_INVALID_FILES;
}

void countPoints(int bool_trump, int start, Card call, Card answer_1,
                 Card answer_2, int* points_call, int* points_opponents)
// TODO: der Trumpf rufende Spieler bleibtn "points_call", sowie opponents
//       sich ihre Punkte teilen
{
  int buffer_start = 0;
  
  // save start
  // to remember whether calling player makes points or the opponents
  buffer_start = start;
  
  if (buffer_start == highest(bool_trump, start, call, answer_1, answer_2))
  {
    *points_call = call.value_ + answer_1.value_ + answer_2.value_;
  }
  
  else
  {
    *points_opponents = call.value_ + answer_1.value_ + answer_2.value_;
  }
}

void setTrump(Card* deck, int trump)
{
  for (int i = 0; i < HAND; i++)
  {
    if ((deck[i]).suit_ == decodeSuit(trump))
    {
//      printf("trump: %s\n", (deck[i]).sign_);
      deck[i].is_trump_ = TRUE;
    }
  }
}

void nextAndPoints(int* start, int buffer_start, Card call, Card answer_1,
                   Card answer_2, int* points_call, int* points_opponents,
                   int* points_1, int* points_2, int* points_3, int* order)
{
//  printf("1. %d, 2. %d, 3. %d\n", call.value_, answer_1.value_,
//         answer_2.value_);
  
  // not one trump
  if (call.is_trump_ == FALSE && answer_1.is_trump_ == FALSE
      && answer_2.is_trump_ == FALSE)
  {
    // all same suits
    if (call.suit_ == answer_1.suit_ && call.suit_ == answer_2.suit_)
    {
      // compare values only
      if ((call.value_ < answer_1.value_ || call.value_ < answer_2.value_)
          || (call.value_ < answer_1.value_ && call.value_ < answer_2.value_))
      {
        // opponents win
        if (answer_1.value_ < answer_2.value_)
        {
          // answer 2 wins
//          if (*start + 1 == 3)
//            *start = TURN_PLAYER_1;
//          else if (*start + 2 == 3)
//            *start = TURN_PLAYER_3;                                       // ACHTUNG !!!
//          else
//            *start = *start + 2;
          
          // warum nicht so?
          if (*start == order[TURN_PLAYER_1 MINUS_ONE])
            *start = order[TURN_PLAYER_3 MINUS_ONE];
          else if (*start == order[TURN_PLAYER_2 MINUS_ONE])
            *start = order[TURN_PLAYER_1 MINUS_ONE];
          else /* if (*start == TURN_PLAYER_3) */
            *start = order[TURN_PLAYER_2 MINUS_ONE];
          
          
//          *points_opponents += (call.value_ + answer_1.value_
//                                + answer_2.value_);
          *points_3 = (call.value_ + answer_1.value_ + answer_2.value_);   // nix +=
        }
        
        else
        {
          // answer 1 wins
          if (*start + 1 > 3)
            *start = order[TURN_PLAYER_1 MINUS_ONE];
          else
            *start = *start + 1;
          
//          *points_opponents += (call.value_ + answer_1.value_
//                                + answer_2.value_);
          *points_2 = (call.value_ + answer_1.value_ + answer_2.value_);
        }
      }
      
      else /* call.value_ > answer_1.value_ && call.value_ > answer_2.value_*/
      {
        // call wins
        *start = *start;  // naja
//        *points_call += (call.value_ + answer_1.value_ + answer_2.value_);
        *points_1 = (call.value_ + answer_1.value_ + answer_2.value_);
      }
    }
    
    // all different suits from call
    else if (call.suit_ != answer_1.suit_ && call.suit_ != answer_2.suit_)
    {
      // call wins
      *start = *start;  // naja
//      *points_call += (call.value_ + answer_1.value_ + answer_2.value_);
      *points_1 = (call.value_ + answer_1.value_ + answer_2.value_);
    }
    
    // only answer 2 has got called suit
    else if (call.suit_ != answer_1.suit_ && call.suit_ == answer_2.suit_)
    {
      if (call.value_ > answer_2.value_)
      {
        // call wins
        *start = *start;  // naja
//        *points_call += (call.value_ + answer_1.value_ + answer_2.value_);
        *points_1 = (call.value_ + answer_1.value_ + answer_2.value_);
      }
      
      else
      {
        // answer 2 wins
        if (*start + 2 > 3)
          *start = order[TURN_PLAYER_1 MINUS_ONE];
        else
          *start = *start + 2;
        
//        *points_opponents += (call.value_ + answer_1.value_
//                              + answer_2.value_);
        *points_3 = (call.value_ + answer_1.value_ + answer_2.value_);
      }
    }
    
    // only answer 1 has got called suit
    else if (call.suit_ == answer_1.suit_ && call.suit_ != answer_2.suit_)
    {
      if (call.value_ > answer_1.value_)
      {
        // call wins
        *start = *start;  // naja
//        *points_call += (call.value_ + answer_1.value_ + answer_2.value_);
        *points_1 = (call.value_ + answer_1.value_ + answer_2.value_);
      }
      
      else
      {
        // answer 1 wins
        if (*start + 1 > 3)
          *start = order[TURN_PLAYER_1 MINUS_ONE];
        else
          *start = *start + 1;
        
//        *points_opponents += (call.value_ + answer_1.value_
//                              + answer_2.value_);
        *points_2 = (call.value_ + answer_1.value_ + answer_2.value_);
      }
    }
  } // passt bis hier - glaub'
  
  // at least one trump played
  else
  {
    // all same suits means all trumps as well (in case call.is_trump_ == TRUE)
    if (call.suit_ == answer_1.suit_ && call.suit_ == answer_2.suit_)
    {
      // compare values only
      if ((call.value_ < answer_1.value_ || call.value_ < answer_2.value_)
          || (call.value_ < answer_1.value_ && call.value_ < answer_2.value_))
      {
        if (answer_1.value_ < answer_2.value_)
        {
          // answer 2 wins
          if (*start + 2 > 3)
            *start = order[TURN_PLAYER_1 MINUS_ONE];
          else
            *start = *start + 2;
          
//          *points_opponents += (call.value_ + answer_1.value_
//                                + answer_2.value_);
          *points_3 = (call.value_ + answer_1.value_ + answer_2.value_);
        }
        
        else
        {
          // answer 1 wins
          if (*start + 1 > 3)
            *start = order[TURN_PLAYER_1 MINUS_ONE];
          else
            *start = *start + 1;
          
//          *points_opponents += (call.value_ + answer_1.value_
//                                + answer_2.value_);
          *points_2 = (call.value_ + answer_1.value_ + answer_2.value_);
        }
      }
      
      else /* call.value_ > answer_1.value_ && call.value_ > answer_2.value_*/
      {
        // call wins
        *start = *start;  // naja
//        *points_call += (call.value_ + answer_1.value_ + answer_2.value_);
        *points_1 = (call.value_ + answer_1.value_ + answer_2.value_);
      }
    } // hm .. passt glaub'a ..
    
    // all different suits from call
    else if (call.suit_ != answer_1.suit_ && call.suit_ != answer_2.suit_)
    {
      /// call alone has trump
//      if (call.is_trump_ == TRUE)
      
      // call hat die einzige Fremde
      if (answer_1.is_trump_ == FALSE && answer_2.is_trump_ == FALSE)
      {
        // call wins
        *start = *start;  // naja
//        *points_call += (call.value_ + answer_1.value_ + answer_2.value_);
        *points_1 = (call.value_ + answer_1.value_ + answer_2.value_);
      }
      
      else /* call.is_trump__ == FALSE */
      {
        // call has not played trump but one or both of the answers
        if ((answer_1.is_trump_ == TRUE || answer_2.is_trump_ == TRUE)
            || (answer_1.is_trump_ == TRUE && answer_2.is_trump_ == TRUE))
        {
          // both answers have trump
          if (answer_1.is_trump_ && answer_2.is_trump_)
          {
            if (answer_1.value_ > answer_2.value_)
            {
              // answer 1 wins
              if (*start + 1 > 3)
                *start = order[TURN_PLAYER_1 MINUS_ONE];
              else
                *start = *start + 1;
              
//              *points_opponents += (call.value_ + answer_1.value_
//                                    + answer_2.value_);
              *points_2 = (call.value_ + answer_1.value_ + answer_2.value_);
            }
            
            else
            {
              // answer 2 wins
              if (*start + 2 > 3)
                *start = order[TURN_PLAYER_1 MINUS_ONE];
              else
                *start = *start + 2;
              
//              *points_opponents += (call.value_ + answer_1.value_
//                                    + answer_2.value_);
              *points_3 = (call.value_ + answer_1.value_ + answer_2.value_);
            }
          }
          
          // only answer 1 has trump
          else if (answer_1.is_trump_ == TRUE && answer_2.is_trump_ == FALSE)
          {
            // answer 1 wins
            if (*start + 1 > 3)
              *start = order[TURN_PLAYER_1 MINUS_ONE];
            else
              *start = *start + 1;
            
//            *points_opponents += (call.value_ + answer_1.value_
//                                  + answer_2.value_);
            *points_2 = (call.value_ + answer_1.value_ + answer_2.value_);
          }
          
          // only answer 2 has trump
          else
          {
            // answer 2 wins
//            if (*start + 2 > 3)
//              *start = TURN_PLAYER_2/*TURN_PLAYER_1*/;
//            else
//              *start = *start + 2;
            
            // why not like this
            if (*start == order[TURN_PLAYER_1 MINUS_ONE])
              *start = order[TURN_PLAYER_3 MINUS_ONE];
            else if (*start == order[TURN_PLAYER_2 MINUS_ONE])
              *start = order[TURN_PLAYER_1 MINUS_ONE];
            else /* if (*start == TURN_PLAYER_3) */
              *start = order[TURN_PLAYER_2 MINUS_ONE];
            
//            *points_opponents += (call.value_ + answer_1.value_
//                                  + answer_2.value_);
            *points_3 = (call.value_ + answer_1.value_ + answer_2.value_);
          }
        }
      }
    } // passt jetzt glaub'auch ..
    
    // one answer has same suit as call
    else if ((call.suit_ == answer_1.suit_ && call.suit_ != answer_2.suit_)
        || (call.suit_ != answer_1.suit_ && call.suit_ == answer_2.suit_))
    {
      if (call.is_trump_ == TRUE)
      {
        // only answer 2 has got called suit and is therefore trump
        if (call.suit_ == answer_2.suit_)
        {
          if (call.value_ > answer_2.value_)
          {
            // call wins
            *start = *start;  // naja
//            *points_call += (call.value_ + answer_1.value_ + answer_2.value_);
            *points_1 = (call.value_ + answer_1.value_ + answer_2.value_);
          }
          
          else
          {
            // answer_2 wins
            if (*start + 2 > 3)                                               // check !!
              *start = order[TURN_PLAYER_1 MINUS_ONE];
            else
              *start = *start + 2;
            
//            *points_opponents += (call.value_ + answer_1.value_
//                                  + answer_2.value_);
            *points_3 = (call.value_ + answer_1.value_ + answer_2.value_);
          }
        }
    
        // only answer 1 has got called suit and is therefore trump
        if (call.suit_ == answer_1.suit_)
        {
          if (call.value_ > answer_1.value_)
          {
            // call wins
            *start = *start;  // naja
//            *points_call += (call.value_ + answer_1.value_ + answer_2.value_);
            *points_1 = (call.value_ + answer_1.value_ + answer_2.value_);
          }
          
          else
          {
            // answer_1 wins
            if (*start + 1 > 3)
              *start = order[TURN_PLAYER_1 MINUS_ONE];
            else
              *start = *start + 1;
            
//            *points_opponents += (call.value_ + answer_1.value_
//                                  + answer_2.value_);
            *points_2 = (call.value_ + answer_1.value_ + answer_2.value_);
          }
        }
      }
      
      // one of the answers is trump and therefore wins
      else /* if (call.is_trump_ == FALSE) */
      {
        if (answer_1.is_trump_ == TRUE)
        {
          // answer_1 wins
          if (*start + 1 > 3)
            *start = order[TURN_PLAYER_1 MINUS_ONE];
          else
            *start = *start + 1;
          
//          *points_opponents += (call.value_ + answer_1.value_
//                                + answer_2.value_);
          *points_2 = (call.value_ + answer_1.value_ + answer_2.value_);
        }
        
        else /* answer_2.is_trump_ == TRUE */
        {
          // answer_2 wins
          if (*start + 2 > 3)
            *start = order[TURN_PLAYER_1 MINUS_ONE];
          else
            *start = *start + 2;
          
//          *points_opponents += (call.value_ + answer_1.value_
//                                + answer_2.value_);
          *points_3 = (call.value_ + answer_1.value_ + answer_2.value_);
        }
      } // sollt'ja a passen ..
    }
    
    // just in case I forgot something
    else
    {
      printf("SOMETHING BAD HAPPENED - I MUST HAVE MADE A MISTACE.\n");
      printf("Dieser Fall trat bisher erst EINMAL (20220402) ein .. ");
      printf("und es hat aber alles gepasst! :D\n");
      printf("Fall: Kontraschnapser (Runde 1) -> TOT\n\n  hm .. \n\n");
      printf("Zweiter Fall (20220207) ..\n");
      printf("zweite Runde im Kontraschnapser DOPPELT TOT (Farbe & Trumpf)\n\n");
      printf("(20220207): Ich glaube ich habe den Fehler entdeckt.\n");
      printf("Zumindest einen.\n\n");
    }
  }
} // Funktion passt! .. nur weiß die Funktion nicht, wer nun calling player ist
  //                    hier ist nämlich immer der "Ausspielende" call
  //                     .. und nicht der Trumpf-rufende

// determine next to call round
// return value: int next .. will be received as start in game modes
Points next(int* initial_order, int* player, Card call, Card answer_1,
          Card answer_2)
{
  Card cards_on_table[QUANTITY_PLAYERS] = {call, answer_1, answer_2}; // index corresponds to player[]
  Points next = {0, 0, 0};
  
  printf("\n Trumps on the table: %d %d %d\n", cards_on_table[0].is_trump_,
         cards_on_table[1].is_trump_, cards_on_table[2].is_trump_);
  
  // caller_mode calls (1)
  if (player[0] == initial_order[0])        // tells us who called recent round
  {
//    printf("Player %d calls mode and round", player[0]);
    
    // caller_mode called round and wins
    if (player[0] == highestCard(cards_on_table))
    {
//      printf(" and wins!\n");
      
      next.winner_ = initial_order[0]; // player[0];
    }

    // caller_mode called round and loses
    else /* if () */
    {
//      printf(" and loses");

      // opponent_1 (2) wins
//      if (player[1] == highestCard(cards_on_table))
      if (highestCard(cards_on_table) == TURN_PLAYER_2)
      {
//        printf(" against Player %d.\n", player[1]);
        
//        next.winner_ = initial_order[1]; // player[1];
        next.winner_ = initial_order[1];
      }

      // opponent_2 (3) wins
      else
      {
//        printf(" against Player %d.\n", player[2]);
        
        next.winner_ = initial_order[2]; // player[2];
      }
    }
  }
  
  // TODO: opponent_1 (2) called round
  else if (player[0] == initial_order[1])    // tells us who called recent round
  {
//    printf("Player %d calls round", player[0]);
    
    // caller_round wins (= opponent_1 (2))
    if (player[0] == highestCard(cards_on_table))
    {
//      printf(" and wins!\n");
      
      next.winner_ = initial_order[0]; // player[0];
    }
    
    else
    {
      // player 3
      if (highestCard(cards_on_table) == TURN_PLAYER_3)
      {
        next.winner_ = initial_order[0];
      }
      // player 1
      else /* if (highestCard(cards_on_table) == TURN_PLAYER_2) */
      {
        next.winner_ = initial_order[2];
      }
    }
  }
  
  // TODO: opponent_2 (3) called round
  else /* if (player[0] == initial_order[2]) */
  {
//    printf("Player %d calls round", player[0]);
    
    // caller_round wins (= opponent_2 (3))
    if (player[0] == highestCard(cards_on_table))
    {
//      printf(" and wins!\n");
      
//      next.winner_ = initial_order[0]; // player[0];
      next.winner_ = player[2];
    }
    
    else
    {
//      printf(" and loses against");
      
      // caller_mode wins (= initial_order[0]) (1)
//      if (player[1] == highestCard(cards_on_table))
      if (highestCard(cards_on_table) == TURN_PLAYER_2)
      {
//        printf(" the player who called mode (%d).", player[1]);
        
        next.winner_ = initial_order[0]; // player[1];
      }
      
      // other opponent wins (= opponent_1 (2))
      else /* if (highestCard(cards_on_table) == TURN_PLAYER_3) */
      {
//        printf(" Player %d.\n", player[2]);
        
//        next.winner_ = initial_order[1]; // player[2];
        next.winner_ = player[1];
      }
    }
  }
  
  // next to call always receives recent round's points
  next.points_ = call.value_ + answer_1.value_ + answer_2.value_;
  
  return next;
}

// returns index of the player who has the highest card on the table
int highestCard(Card* cards)
{
  int index = 0;
  int count_trump = 0;
  int counter = 0;
  int has_trump[QUANTITY_PLAYERS] = {FALSE, FALSE, FALSE};
  
  for (counter = 0; counter < QUANTITY_PLAYERS; counter++)
  {
    if (cards[counter].is_trump_)
    {
      has_trump[counter] = TRUE;
      count_trump++;
    }
  }
  
//  printf("%d %d %d\n", cards[0].value_, cards[1].value_, cards[2].value_);
//  printf("%d trumps on table.\n", count_trump);
  
  // TODO: check highest card and do not forget about trump
  
  // no trump or all trump
//  if (!(cards[0].is_trump_ && cards[1].is_trump_ && cards[3].is_trump_)
//      || (cards[0].is_trump_ && cards[1].is_trump_ && cards[3].is_trump_))
  if (count_trump == 0 || count_trump == QUANTITY_PLAYERS)
  {
//    printf("NONE OR ALL TRUMP ON THE TABLE\n");
    // TODO: check highest card's index and pass
    if (cards[0].value_ > cards[1].value_ && cards[0].value_ > cards[2].value_)
    {
      index = TURN_PLAYER_1;
    }
    
    else if (cards[1].value_ > cards[0].value_ && cards[1].value_ > cards[2].value_)
    {
      index = TURN_PLAYER_2;
    }
    
    else /* if (cards[2].value_ > cards[0].value_ && cards[2].value_ > cards[1].value_) */
    {
      index = TURN_PLAYER_3;
    }
  }
  
  // trump
  else
  {
//    printf("AT LEAST ONE TRUMP ON THE TABLE\n");
    // TODO: check highest trump's index and pass
    if (count_trump == 1)
    {
      for (counter = 0; counter < QUANTITY_PLAYERS; counter++)
      {
        if (has_trump[counter] == TRUE)
        {
          index = counter ADD_ONE;
        }
      }
    }
    
    else /* if (count_trump == 2) */
    {
      if (has_trump[0] && has_trump[1])
      {
        if (cards[0].value_ > cards[1].value_)
          index = TURN_PLAYER_1;
        
        else
          index = TURN_PLAYER_2;
      }
        
      else if (has_trump[0] && has_trump[2])
      {
        if (cards[0].value_ > cards[2].value_)
          index = TURN_PLAYER_1;
        
        else
          index = TURN_PLAYER_3;
      }
        
      else /* if (has_trump[1] && has_trump[2]) */
      {
        if (cards[1].value_ > cards[2].value_)
          index = TURN_PLAYER_2;
        
        else
          index = TURN_PLAYER_3;
      }
    }
  }
  
  return index;
}

void sortHands(Card** hands, char** suits)
{
//  printf("first in hands[0]: %s\n", hands[0]->image_);
//  printf("first in hands[1]: %s\n", hands[1]->image_);
//  printf("first in hands[2]: %s\n", hands[2]->image_);
  
  // access like this
  // (hands[1])[0].image_ = (hands[1])[2].image_;
  int counter_hands   = 0;
  int counter_cards   = 0;
  int counter_suits   = 0;
  int counter_values  = 0;
  int values[QUANTITY_VALUES] = {ACE, TEN, KING, QUEEN, JACK};
  Card buffer_hand[HAND] = {};    // one hand
  int quantity        = 0;
  int positions[6]   = {-1, -1, -1, -1, -1, -1};    // gibt's sicher einen
                                                    // saubereren/eleganteren
                                                    // Schmeè
  int position_hand = 0;
  
  // loop hands
  for (counter_hands = 0; counter_hands < QUANTITY_PLAYERS; counter_hands++)
  {
    // buffer hand - one after the other
    
    // loop suits
    while (counter_suits < QUANTITY_SUITS)
    {
//      // order: hearts, spades, clubs, diamonds
//      printf("suit: %s\n", suits[counter_suits]);
//
//      // find position(s)
//      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
//      {
//        if (buffer_hand[counter_cards].suit_ == suits[counter_suits])
//        {
//          quantity++;
//          positions[counter_cards] = counter_cards;
//          printf("%d\n", counter_cards + 1);
//        }
//      }
      // loop values
//      while (counter_values < QUANTITY_VALUES)  // better while (counter_suits < QUANTITY_SUITS)
//      while (counter_suits < QUANTITY_SUITS)
//      {
        // check order
        sort(positions, hands[counter_hands], quantity, &position_hand,
             buffer_hand, suits[counter_suits]);
        
        counter_values++;
        counter_suits++;
      
//      }
      
//      counter_values  = 0;
//      counter_suits   = 0;
    }
    
    counter_values  = 0;
    counter_suits   = 0;
    position_hand   = 0;
    
    // write buffer (= sorted hand) onto hand
    
//    hands[counter_hands] = buffer_hand;         // geht (wohl) nicht
    
    // now applay the order to all three hands
    // could be outsources as a separate function
    // falls amol geil bist
    for (int i = 0; i < 6; i++)
    {
  //      hands[counter_hands][position_hand] = buffer_hand[position_hand];
      hands[counter_hands][i].image_ = buffer_hand[i].image_;
      hands[counter_hands][i].value_ = buffer_hand[i].value_;
      hands[counter_hands][i].suit_ = buffer_hand[i].suit_;
      hands[counter_hands][i].sign_ = buffer_hand[i].sign_;
      hands[counter_hands][i].is_bock_ = buffer_hand[i].is_bock_;
      hands[counter_hands][i].is_trump_ = buffer_hand[i].is_trump_;
    }
    
  } // end for (hands)
}

void sort(int* positions, Card* hand, int quantity, int* position_hand,
          Card* buffer, char* suit)
{
  int counter   = 0;
  
  for (counter = 0; counter < HAND; counter++)
  {
    
    if (hand[counter].value_ == ACE && hand[counter].suit_ == suit)
    {
      // place
//      buffer[*position_hand] = hand[counter];   // das geht glaub'halt wirklich nicht
      
      buffer[*position_hand].image_ = hand[counter].image_;
      buffer[*position_hand].value_ = hand[counter].value_;
      buffer[*position_hand].suit_ = hand[counter].suit_;
      buffer[*position_hand].sign_ = hand[counter].sign_;
      buffer[*position_hand].is_bock_ = hand[counter].is_bock_;
      buffer[*position_hand].is_trump_ = hand[counter].is_trump_;
      
      (*position_hand)++;
    }
  }
  
  for (counter = 0; counter < HAND; counter++)
  {
    
    if (hand[counter].value_ == TEN && hand[counter].suit_ == suit)
    {
      // place
//      buffer[*position_hand] = hand[counter];
      
      buffer[*position_hand].image_ = hand[counter].image_;
      buffer[*position_hand].value_ = hand[counter].value_;
      buffer[*position_hand].suit_ = hand[counter].suit_;
      buffer[*position_hand].sign_ = hand[counter].sign_;
      buffer[*position_hand].is_bock_ = hand[counter].is_bock_;
      buffer[*position_hand].is_trump_ = hand[counter].is_trump_;
      
      (*position_hand)++;
    }
  }
  
  for (counter = 0; counter < HAND; counter++)
  {
    
    if (hand[counter].value_ == KING && hand[counter].suit_ == suit)
    {
      // place
//      buffer[*position_hand] = hand[counter];
      
      buffer[*position_hand].image_ = hand[counter].image_;
      buffer[*position_hand].value_ = hand[counter].value_;
      buffer[*position_hand].suit_ = hand[counter].suit_;
      buffer[*position_hand].sign_ = hand[counter].sign_;
      buffer[*position_hand].is_bock_ = hand[counter].is_bock_;
      buffer[*position_hand].is_trump_ = hand[counter].is_trump_;
      
      (*position_hand)++;
    }
  }
  
  for (counter = 0; counter < HAND; counter++)
  {
    
    if (hand[counter].value_ == QUEEN && hand[counter].suit_ == suit)
    {
      // place
//      buffer[*position_hand] = hand[counter];
      
      buffer[*position_hand].image_ = hand[counter].image_;
      buffer[*position_hand].value_ = hand[counter].value_;
      buffer[*position_hand].suit_ = hand[counter].suit_;
      buffer[*position_hand].sign_ = hand[counter].sign_;
      buffer[*position_hand].is_bock_ = hand[counter].is_bock_;
      buffer[*position_hand].is_trump_ = hand[counter].is_trump_;
      
      (*position_hand)++;
    }
  }
  
  for (counter = 0; counter < HAND; counter++)
  {
    
    if (hand[counter].value_ == JACK && hand[counter].suit_ == suit)
    {
      // place
//      buffer[*position_hand] = hand[counter];
      
      buffer[*position_hand].image_ = hand[counter].image_;
      buffer[*position_hand].value_ = hand[counter].value_;
      buffer[*position_hand].suit_ = hand[counter].suit_;
      buffer[*position_hand].sign_ = hand[counter].sign_;
      buffer[*position_hand].is_bock_ = hand[counter].is_bock_;
      buffer[*position_hand].is_trump_ = hand[counter].is_trump_;
      
      (*position_hand)++;
    }
  }
}

int highlight(Card* hand, int player, int* positions)   // why player?
                                                        // maybe I had an idea
                                                        // of only checking
                                                        // player[i == 0]
{
//  int positions[3]    = {0};    // position of Queen
  int counter         = 0;
  int counter_pairs   = 0;
  
  // find pair
  for (counter = 1; counter < HAND; counter++)
  {
    if ((hand[counter].value_ == 3 && hand[counter MINUS_ONE].value_ == 4)
        && hand[counter].suit_ == hand[counter MINUS_ONE].suit_)
    {
//      printf("Found a pair!\n");
      positions[counter_pairs] = counter;
      counter_pairs++;
    }
  }
  
  if (counter_pairs > 0) // has at least one pair
  {
    if (counter_pairs == 3) /* three pairs */
    {
      if (hand[positions[0]].is_trump_ == FALSE
          && hand[positions[1]].is_trump_ == FALSE
          && hand[positions[2]].is_trump_ == FALSE) /* no 40 */
      {
        printf("                         __2_0__     __2_0__     __2_0__  \n");
      }
      
      else /* three pairs with 40 */
      {
        if (hand[0].is_trump_ == TRUE)
          printf("                         __4_0__     __2_0__     __2_0__  \n");
        
        if (hand[2].is_trump_ == TRUE)
          printf("                         __2_0__     __4_0__     __2_0__  \n");
        
        if (hand[4].is_trump_ == TRUE)
          printf("                         __2_0__     __2_0__     __4_0__  \n");
      }
    }
    
    else if (counter_pairs == 2) /* two pairs */
    {
      if (hand[positions[0]].is_trump_ == FALSE
          && hand[positions[1]].is_trump_ == FALSE) /* no 40 */
      {
        if (positions[0] == 0 ADD_ONE)
        {
          if (positions[1] == 2 ADD_ONE)
            printf("                         __2_0__     __2_0__              \n");
          
          if (positions[1] == 3 ADD_ONE)
            printf("                         __2_0__           __2_0__        \n");
          
          if (positions[1] == 4 ADD_ONE)
            printf("                         __2_0__                 __2_0__  \n");
        }
        
        else if (positions[0] == 1 ADD_ONE)
        {
          if (positions[1] == 4 ADD_ONE)
            printf("                               __2_0__           __2_0__  \n");
          
          else
            printf("                               __2_0__     __2_0__        \n");
        }
        
        else
          printf("                                     __2_0__     __2_0__  \n");    // ?
      }
      
      else /* with 40 */
      {
//        if (((hand[positions[0]].is_trump_ == TRUE && positions[0] < 3 ADD_ONE)
//            && (hand[positions[1]].is_trump_ == FALSE && positions[1] >= 3 ADD_ONE))
//            || ((hand[positions[1]].is_trump_ == TRUE && positions[0] < 3 ADD_ONE)
//                && (hand[positions[0]].is_trump_ == FALSE
//                    && positions[1] >= 3 ADD_ONE)))
        
        if (hand[positions[0]].is_trump_ == TRUE)   // reicht wahrscheinlich
        {
          if (positions[0] == 0 ADD_ONE)
          {
            if (positions[1] == 2 ADD_ONE)
              printf("                         __4_0__     __2_0__              \n");
            
            if (positions[1] == 3 ADD_ONE)
              printf("                         __4_0__           __2_0__        \n");
            
            if (positions[1] == 4 ADD_ONE)
              printf("                         __4_0__                 __2_0__  \n");
          }
          
          else if (positions[0] == 1 ADD_ONE)
          {
            if (positions[1] == 4 ADD_ONE)
              printf("                               __4_0__           __2_0__  \n");
            
            else
              printf("                               __4_0__     __2_0__        \n");
          }
          
          else
            printf("                                     __4_0__     __2_0__  \n");      // ?
        }
        
        else
        {
          if (positions[0] == 0 ADD_ONE)
          {
            if (positions[1] == 2 ADD_ONE)
              printf("                         __2_0__     __4_0__              \n");
            
            if (positions[1] == 3 ADD_ONE)
              printf("                         __2_0__           __4_0__        \n");
            
            if (positions[1] == 4 ADD_ONE)
              printf("                         __2_0__                 __4_0__  \n");
          }
          
          else if (positions[0] == 1 ADD_ONE)
          {
            if (positions[1] == 4 ADD_ONE)
              printf("                               __2_0__           __4_0__  \n");
            else
              printf("                               __2_0__     __4_0__        \n");
          }
          
          else
            printf("                                     __2_0__     __4_0__  \n");      // ?
        }
      }
    }
    
    else /* one pair */
    {
      for (counter = 0; counter < counter_pairs; counter++)
      {
        if (hand[positions[0]].is_trump_ == FALSE) // 20
        {
          // where is the king of the pair?
          switch (positions[0] MINUS_ONE)
          {
            case 0:
              printf("                         __2_0__                          \n");
              break;
              
            case 1:
              printf("                               __2_0__                    \n");
              break;
              
            case 2:
              printf("                                     __2_0__              \n");
              break;
              
            case 3:
              printf("                                           __2_0__        \n");
              break;
              
            case 4:
              printf("                                                 __2_0__  \n");
              break;
              
            default:
              break;
          }
        }
        
        else // 40
        {
          // where is the king of the pair?
          switch (positions[0] MINUS_ONE)
          {
            case 0:
              printf("                         __4_0__                          \n");
              break;
              
            case 1:
              printf("                               __4_0__                    \n");
              break;
              
            case 2:
              printf("                                     __4_0__              \n");
              break;
              
            case 3:
              printf("                                           __4_0__        \n");
              break;
              
            case 4:
              printf("                                                 __4_0__  \n");
              break;
              
            default:
              break;
          }
        }
      }
    }
  }
  
  return counter_pairs;
}

int resetPairs(int* position)
{
  int counter = 0;
  
  for (counter = 0; counter < 3; counter++)
  {
    position[counter] = 0;
  }
  
  return 0;
}

int pairHandler(Pair* handle_pairs, int position, int points_to_add,
                 int* i, int* points_call, int* points_opponents,
                 Card* hand, int is_caller)
{
//  handle_pairs[i].points_ = points_to_add;
//  handle_pairs[i].position_played_ = position;
  
  // QUEEN played and KING has not been played or
  // KING played and QUEEN has not been played or
  if ((hand[position].value_ == 3 && hand[position MINUS_ONE].value_ != 0)
      || (hand[position MINUS_ONE].value_ == 4 && hand[position].value_ != 0)
      || (hand[position].value_ == 4 && hand[position ADD_ONE].value_ != 0))
  {
    handle_pairs[*i].points_ = points_to_add;
    
    // exit condition   -   20 or 40 will only be shown; game ends
    if (is_caller == TRUE)    // trump caller plays 20 || 40
    {
      if (*points_call + points_to_add >= 66)
      {
        *points_call += points_to_add;
        printf("It is enough to show.\n"); //printf("Brauch I nit ausspielen!\n");
        *i = ENOUGH;    // nenn'ma's a'foch 5
      }
    }
    
    else /* if ( one of the opponents calls 20 || 40 ) */
    {
      if (*points_opponents + points_to_add >= 66)
      {
        *points_opponents += points_to_add;
        printf("No need to go on.\n"); //printf("Brauch'ma nit ausspielen!\n");
        *i = ENOUGH;    // nenn'ma's a'foch 5
      }
    }
    
    return TRUE;
  }
  
  else
  {
    handle_pairs[*i].points_ = 0;
    return FALSE;
  }
}

void setOrder(Card* deck, int test_case, int trump)
{
//  "♥", "♠", "♣", "♦"
  
  switch (test_case)
  {
  // case: three pairs
    case 0:
      deck[0].value_  = 4;
      deck[0].sign_   = "♥";
      deck[0].suit_   = "hearts";
      deck[0].image_  = "K";
      
      deck[1].value_  = 3;
      deck[1].sign_   = "♥";
      deck[1].suit_   = "hearts";
      deck[1].image_  = "Q";
      
      deck[2].value_  = 4;
      deck[2].sign_   = "♣";
      deck[2].suit_   = "clubs";
      deck[2].image_  = "K";
      
      deck[3].value_  = 3;
      deck[3].sign_   = "♣";
      deck[3].suit_   = "clubs";
      deck[3].image_  = "Q";
      
      deck[4].value_  = 4;
      deck[4].sign_   = "♦";
      deck[4].suit_   = "diamonds";
      deck[4].image_  = "K";
      
      deck[5].value_  = 3;
      deck[5].sign_   = "♦";
      deck[5].suit_   = "diamonds";
      deck[5].image_  = "Q";
      
      break;
  
  // case: pair with K on position: 0
    case 1:
      deck[0].value_  = 4;
      deck[0].sign_   = "♥";
      deck[0].suit_   = "hearts";
      deck[0].image_  = "K";
      
      deck[1].value_  = 3;
      deck[1].sign_   = "♥";
      deck[1].suit_   = "hearts";
      deck[1].image_  = "Q";
      break;
  
  // case: pair with Q on position: 5
    case 2:
      deck[4].value_  = 4;
      deck[4].sign_   = "♦";
      deck[4].suit_   = "diamonds";
      deck[4].image_  = "K";
      deck[4].is_trump_= FALSE;
      
      deck[5].value_  = 3;
      deck[5].sign_   = "♦";
      deck[5].suit_   = "diamonds";
      deck[5].image_  = "Q";
      deck[5].is_trump_= FALSE;
      break;
      
    case 10:
      deck[1].value_    = 4;
      deck[1].sign_     = "♥";
      deck[1].suit_     = "hearts";
      deck[1].image_    = "K";
      deck[1].is_trump_ = FALSE;
      break;
      
    case 11:
      deck[3].value_    = 10;
      deck[3].sign_     = "♥";
      deck[3].suit_     = "hearts";
      deck[3].image_    = "10";
      deck[3].is_trump_ = FALSE;
      
      deck[2].value_    = 4;
      deck[2].sign_     = "♣";
      deck[2].suit_     = "clubs";
      deck[2].image_    = "K";
      deck[2].is_trump_ = FALSE;
      break;
      
    case 12:
      deck[0].value_    = 11;
      deck[0].sign_     = "♥";
      deck[0].suit_     = "hearts";
      deck[0].image_    = "A";
      deck[0].is_trump_ = FALSE;
      
      deck[2].value_    = 2;
      deck[2].sign_     = "♥";
      deck[2].suit_     = "hearts";
      deck[2].image_    = "J";
      deck[2].is_trump_ = FALSE;
      break;
      
    case 13:
      deck[0].value_    = 3;
      deck[0].sign_     = "♦";
      deck[0].suit_     = "diamonds";
      deck[0].image_    = "J";
      deck[0].is_trump_ = FALSE;
      break;
      
    case 20:
      deck[1].value_    = 4;
      deck[1].sign_     = "♥";
      deck[1].suit_     = "hearts";
      deck[1].image_    = "K";
      deck[1].is_trump_ = FALSE;
      break;
      
    case 21:
      deck[3].value_    = 10;
      deck[3].sign_     = "♥";
      deck[3].suit_     = "hearts";
      deck[3].image_    = "10";
      deck[3].is_trump_ = FALSE;
      
      deck[2].value_    = 4;
      deck[2].sign_     = "♣";
      deck[2].suit_     = "clubs";
      deck[2].image_    = "K";
      deck[2].is_trump_ = FALSE;
      break;
      
    case 22:
      deck[0].value_    = 3;
      deck[0].sign_     = "♣";
      deck[0].suit_     = "clubs";
      deck[0].image_    = "J";
      deck[0].is_trump_ = FALSE;
      
      deck[2].value_    = 4;
      deck[2].sign_     = "♦";
      deck[2].suit_     = "diamonds";
      deck[2].image_    = "K";
      deck[2].is_trump_ = FALSE;
      break;
      
    case 23:
      deck[0].value_    = 11;
      deck[0].sign_     = "♥";
      deck[0].suit_     = "hearts";
      deck[0].image_    = "A";
      deck[0].is_trump_ = FALSE;
      
      deck[1].value_    = 2;
      deck[1].sign_     = "♥";
      deck[1].suit_     = "hearts";
      deck[1].image_    = "J";
      deck[1].is_trump_ = FALSE;
      break;
      
    case 30:
      deck[1].value_    = 4;
      deck[1].sign_     = "♥";
      deck[1].suit_     = "hearts";
      deck[1].image_    = "K";
      deck[1].is_trump_ = FALSE;
      break;
      
    case 31:
      deck[3].value_    = 0;
      deck[3].sign_     = "0";
      deck[3].suit_     = "0";
      deck[3].image_    = "0";
      deck[3].is_trump_ = FALSE;
      
      deck[2].value_    = 4;
      deck[2].sign_     = "♣";
      deck[2].suit_     = "clubs";
      deck[2].image_    = "Q";
      deck[2].is_trump_ = FALSE;
      
      deck[5].value_    = 0;
      deck[5].sign_     = "0";
      deck[5].suit_     = "0";
      deck[5].image_    = "0";
      deck[5].is_trump_ = FALSE;
      break;
      
    case 32:
      deck[0].value_    = 3;
      deck[0].sign_     = "♣";
      deck[0].suit_     = "clubs";
      deck[0].image_    = "J";
      deck[0].is_trump_ = FALSE;
      
      deck[2].value_    = 4;
      deck[2].sign_     = "♦";
      deck[2].suit_     = "diamonds";
      deck[2].image_    = "K";
      deck[2].is_trump_ = FALSE;
      break;
      
    case 33:
      deck[0].value_    = 11;
      deck[0].sign_     = "♥";
      deck[0].suit_     = "hearts";
      deck[0].image_    = "A";
      deck[0].is_trump_ = FALSE;
      
      deck[1].value_    = 2;
      deck[1].sign_     = "♥";
      deck[1].suit_     = "hearts";
      deck[1].image_    = "J";
      deck[1].is_trump_ = FALSE;
      break;
      
    case 40:
      deck[1].value_    = 4;
      deck[1].sign_     = "♥";
      deck[1].suit_     = "hearts";
      deck[1].image_    = "K";
      deck[1].is_trump_ = FALSE;
      break;
      
    case 41:
      deck[3].value_    = 0;
      deck[3].sign_     = "0";
      deck[3].suit_     = "0";
      deck[3].image_    = "0";
      deck[3].is_trump_ = FALSE;
      
      deck[2].value_    = 3;
      deck[2].sign_     = "♣";
      deck[2].suit_     = "clubs";
      deck[2].image_    = "Q";
      deck[2].is_trump_ = FALSE;
      
      deck[5].value_    = 0;
      deck[5].sign_     = "0";
      deck[5].suit_     = "0";
      deck[5].image_    = "0";
      deck[5].is_trump_ = FALSE;
      break;
      
    case 42:
      deck[0].value_    = 4;
      deck[0].sign_     = "♣";
      deck[0].suit_     = "clubs";
      deck[0].image_    = "K";
      deck[0].is_trump_ = FALSE;
      
      deck[2].value_    = 2;
      deck[2].sign_     = "♣";
      deck[2].suit_     = "club";
      deck[2].image_    = "J";
      deck[2].is_trump_ = FALSE;
      break;
      
    case 43:
      deck[0].value_    = 11;
      deck[0].sign_     = "♥";
      deck[0].suit_     = "hearts";
      deck[0].image_    = "A";
      deck[0].is_trump_ = FALSE;
      
      deck[1].value_    = 2;
      deck[1].sign_     = "♥";
      deck[1].suit_     = "hearts";
      deck[1].image_    = "J";
      deck[1].is_trump_ = FALSE;
      break;
      
      default:
        break;
  }
  // set trump
  for (int i = 0; i < 6; i++)
  {
    if (deck[i].suit_ == decodeSuit(trump))
      deck[i].is_trump_ = TRUE;
  }
}

void resetHandlePairs(Pair* handle_pairs)
{
  int counter = 0;
  
  for (counter = 0; counter < 3; counter++)
  {
    handle_pairs[counter].points_     = 0;
    handle_pairs[counter].bool_pair_  = FALSE;
//    handle_pairs[counter].bool_trick_ = FALSE;
  }
}

int checkContinue(char* player)
{
  int buffer = 0;
  printf("---------------------------------------------------------------\n");
  printf("%s, would you like to play another Bummerl?\n", player);
  printf("y       YES\n");
  printf("n       NO\n");
  printf("---------------------------------------------------------------\n");
  
  do
  {
    system ("/bin/stty raw");
    buffer = getchar();
    printf("\r");
    if (buffer != 'y' && buffer != 'n')
      printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
  } while (buffer != 'y' && buffer != 'n');
  system ("/bin/stty cooked");
  
  if (buffer == 'y')
  {
    return TRUE;
  }
  
  else
  {
    return FALSE;
  }
}

void swapOrder(Player* players)
{
  Player buffer = {"Hausdepp", 555};      // maybe an unprofessional choice
  int counter = 0;
  
  strcpy(buffer.name_, players[0].name_);
  buffer.points_  = players[0].points_;
  
  for (counter = 0; counter < 2; counter++)
  {
    strcpy(players[counter].name_, players[counter ADD_ONE].name_);
    players[counter].points_ = players[counter ADD_ONE].points_;
  }
  
  strcpy(players[counter].name_, buffer.name_);
  players[counter].points_ = buffer.points_;
}

// stupid helper function but I'm tired
int decomposeQWEASD (char character)
{
  switch (character)
  {
    case 'q':
      return 0;
      break;
      
    case 'w':
      return 1;
      break;
      
    case 'e':
      return 2;
      break;
      
    case 'a':
      return 3;
      break;
      
    case 's':
      return 4;
      break;
      
    case 'd':
      return 5;
      break;
      
    default:
      return ERROR_CODE_WRONG_COMMAND;
      break;
  }
}

char randomCall(int instance)
{
  srand((unsigned int)clock());
  
  switch (instance)
  {
    case INSTANCE_CARD:
      return COMMANDS_CARDS[rand() % INSTANCE_CARD];
      break;
      
    case INSTANCE_POLAR:
      return COMMANDS_POLAR[rand() % INSTANCE_POLAR];
      break;
      
    case INSTANCE_TRUMP:
      return COMMANDS_TRUMP[rand() % INSTANCE_TRUMP];
      break;
      
    default:
      return HUMAN;
      break;
  }
}

int validCommandCPU(char to_compare, char* with)
{
  for (int i = 0; i < strlen(with); i++)
  {
    if (to_compare == with[i])
    {
      return to_compare;
    }
  }
  return RETRY;
}

char checkCPU(Player to_check, int instance, char* commands_possible)
{
  int call_CPU = RETRY;
  
  if (to_check.CPU_bool_ == TRUE)
  {
    call_CPU = validCommandCPU(randomCall(instance), commands_possible);
    
    return call_CPU;
  }
  
  else
  {
    return RETRY;
  }
}

void printTable(Player players)
{
//  for (int i = 0; i < 3; i++)
//    printf("[%s %s] ", players[i].sign_, deck[counter].image_);
}

// switchRufer()
// sets order and calls modeRufer()
// call like this:
//                next_and_points
//                  = switchRufer(hands, start, decodeSuit(*trump), players, order)
//
Points switchRufer(Card** hands, int start, char* trump, Player* players,
                  int* order)
{
  Points next_and_points    = {start, 0, start};
  Points points_and_caller  = {start, 0, start};
  int points_caller         = 0;
  int points_opponents      = 0;
  int counter               = 0;
  Card* initial_hands[3]    = {};
  int counter_hand          = 0;
  Player initial_players[3] = {"", 0, FALSE};
  
  char commands[7] = "qweasd";
  char commands_1[7] = "\0";
  char commands_2[7] = "\0";
  char commands_3[7] = "\0";
  strcpy(commands_1, commands);
  strcpy(commands_2, commands);
  strcpy(commands_3, commands);
  char* players_commands[3] = {commands_1, commands_2, commands_3};
  int initial_commands[3] = {1, 2, 3};
  char* current_commands[3] = {commands, commands, commands};
  
  // who called mode - who is opponent
  
  // Player 1 called mode
  if (order[0] ADD_ONE == TURN_PLAYER_1)
  {
    next_and_points.caller_ = TURN_PLAYER_1;
    initial_commands[0] = 1;
    initial_commands[1] = 2;
    initial_commands[2] = 3;
    initial_players[0] = players[0];
    initial_players[1] = players[1];
    initial_players[2] = players[2];
    
    for (counter_hand = 0; counter_hand < HAND; counter_hand++)
    {
      initial_hands[counter_hand] = hands[counter_hand];
    }
    
    while (points_caller < MAXIMUM_POINTS
           && points_opponents < MAXIMUM_POINTS
           && counter < MAXIMUM_TURNS)
    {
      printf("TURN: %d\n", counter + 1);
      
      // modeRufer() will therefore be just one turn
      
      // sort hands prior calling modeRufer()
      sortOrderHands(/* next_and_points.caller_ */ next_and_points.winner_, hands, initial_hands);
      sortOrderPlayers(/* next_and_points.caller_ */ next_and_points.winner_, players, initial_players);
//      sortOrderCommands(next_and_points.winner_, players_commands, initial_commands);
      
      // silly work around - since I am tired
//      strcpy(current_commands[0], players_commands[0]);
//      strcpy(current_commands[1], players_commands[1]);
//      strcpy(current_commands[2], players_commands[2]);
      
      next_and_points
        = modeRufer(hands, next_and_points.caller_, trump, players, players_commands);
      
//      strcpy(players_commands[0], current_commands[0]);
//      strcpy(players_commands[1], current_commands[1]);
//      strcpy(players_commands[2], current_commands[2]);
      
      // next to call
      next_and_points.caller_ = next_and_points.winner_;
      
      // handle points
      // caller wins turn
      if (next_and_points.winner_ == TURN_PLAYER_1)
      {
        points_caller += next_and_points.points_;
      }
      
      // opponents win turn
      else
      {
        points_opponents += next_and_points.points_;
      }
      
      // remove played cards from hands
//      removeCard(&hands[0][position[0]]);
//      removeCard(&hands[1][position[1]]);
//      removeCard(&hands[2][position[2]]);
      // -------------------------------------------
      // write all left over cards onto commands
      
      counter++;
    } // end of while()
    
    // game points
    distributePoints(points_caller, points_opponents, &points_and_caller);
  }
  
  // Player 2 called mode - TODO: clone "Player 2 called mode"
  else if (order[0] ADD_ONE == TURN_PLAYER_2)
  {
    next_and_points.caller_ = TURN_PLAYER_2;
    // start = TURN_PLAYER_2;
    
    // and here we will call the mode itself
    next_and_points
      = modeRufer(hands, next_and_points.caller_, trump, players, players_commands);
    
    // caller wins
    if (next_and_points.winner_ == TURN_PLAYER_2)
    {
      
    }
    
    // opponents win
    else
    {
      
    }
  }
  
  // Player 3 called mode - TODO: clone "Player 3 called mode"
  else /* if (order[0] ADD_ONE == TURN_PLAYER_3) */
  {
    next_and_points.caller_ = TURN_PLAYER_3;
    // start = TURN_PLAYER_3;
    
    // and here we will call the mode itself
    next_and_points
      = modeRufer(hands, next_and_points.caller_, trump, players, players_commands);
    
    // caller wins
    if (next_and_points.winner_ == TURN_PLAYER_3)
    {
      
    }
    
    // opponents win
    else
    {
      
    }
  }
  
  return points_and_caller; // { winner_ = OPPONENTS_WIN || TURN_PLAYER_1
                            //   points_ = 1, 2 or 3 }
}

// One turn only, so it has to be called in a loop. Maybe this way I can clean
// it up a bit and finally make it work
Points modeRufer(Card** hands, int start, char* trump, Player* players, char** players_commands)
{
  int player[3] = {start, 0, 0};
  getCall(start, &player[0], &player[1], &player[2]);
  Points points_and_next = {start, 0, 0};
  int counter = 0;
  
//  printHand(hands[0], 6, 1, "test");
//  printHand(hands[1], 6, 2, "test");
//  printHand(hands[2], 6, 3, "test");
  
  // -----------
  
//  int counter_turns = 0;
  int counter_cards = 0;
  int counter_position = 0;
  int counter_suit = 0;
  int counter_trump = 0;
  int counter_hand = 0;
  int counter_command = 0;
  const char commands[7] = "qweasd";
//  char commands_1[7] = "\0";
//  char commands_2[7] = "\0";
//  char commands_3[7] = "\0";
//  strcpy(commands_1, commands);
//  strcpy(commands_2, commands);
//  strcpy(commands_3, commands);
  
  printf("1. %s\n2. %s\n3. %s\n", players_commands[0], players_commands[1], players_commands[2]);
  
  // default
//  int call      = TURN_PLAYER_1;
//  int answer_1  = TURN_PLAYER_2;
//  int answer_2  = TURN_PLAYER_3;
  int call      = player[0];
  int answer_1  = player[1];
  int answer_2  = player[2];
  int initial_order[3] = {call, answer_1, answer_2};   // initial order
//  char* players_commands[3] = {commands_1, commands_2, commands_3};
  int position[3]   = {0};
  int position_Q[3] = {0};
  int pairs         = 0;
  int check = FALSE;
  int buffer = 0;
//  int counter_Q = 0;
  int points_call = 0;
  int points_opponents = 0;
  int points_pair = 0;
  int buffer_start = 0;
//  int bool_trump = TRUE;
//  int points_1 = 0;
//  int points_2 = 0;
//  int points_3 = 0;
//  int points[3] = {points_1, points_2, points_3};
  int count_bock    = 0;   // bock
  int count_suit    = 0;   // right suit
  int count_trump   = 0;   // trump
  int count_permit  = 0;
//  int count_valid   = 0;
  Pair handle_pairs[3] = {0, 0, 0};
//  Turn buffer_turn[3] = {{{NULL}, {0}, "", ' '}, ' ', 0};
  getCall(start, &call, &answer_1, &answer_2);
  buffer_start = start;
//  Points points_and_caller = {0, 0, 0};
  char buffer_higher[6] = {'\0'};
//  char input_CPU = '\0';
//  Points next_and_points = {0, 0, 0};
  int bool_trumped_already = FALSE;
  
  for (counter = 0; counter < QUANTITY_PLAYERS; counter++)
  {
//    printHand(hands[player[counter] MINUS_ONE], HAND, player[counter],
//              players[player[counter] MINUS_ONE].name_);
    printHand(hands[counter], HAND, player[counter],
              players[counter].name_);
    printf("                         ^     ^     ^     ^     ^     ^  \n");
    printf("                        |Q|   |W|   |E|   |A|   |S|   |D| \n");
    
    // highlight 20 & 40
    if (counter == 0)
      pairs = highlight(hands[counter], player[counter], position_Q);
    
    printf("---------------------------------------------------------------\n");
    
    // caller's input - may call arbitrary
    if (counter == 0)
    {
      do
      {
        system ("/bin/stty raw");
        buffer = getchar();
        printf("\r");
        if (!(check = seekAndDestroy(buffer, players_commands[counter])))
          printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
      } while (!check);
      system ("/bin/stty cooked");
      
      switch (buffer)
      {
        case 'q':
          position[counter] = 0;
          
          // check for 20 or 40
          if (pairs > 0)
          {
            // at this position we will only find a KING
            if (position[counter] == position_Q[0] MINUS_ONE
                 && hands[counter][position_Q[0]].value_ == QUEEN)
            {
              // 40
              if (hands[counter][position_Q[0]].is_trump_ == TRUE)
              {
                points_pair = 40;
                
                handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                         position_Q[0] MINUS_ONE,
                                                         points_pair, &counter, &points_call,
                                                         &points_opponents,
                                                         hands[counter],
                                                         (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("K 40?\n");
                  pairs--;
                  // sollte ich hier nun auch theoretische positions_Q
                  // "hochrutschen"? - 1 auf 0, 2 auf 1
                  //
                  // Ja sicher!
                  position_Q[0] = position_Q[1];                            // naja, hardcoded
                  position_Q[1] = position_Q[2];                            // oba viel besseres
                  position_Q[2] = 0;                                        // fällt mir hier nit ein
                }
              }
              // 20
              else /* is_trump == FALSE */
              {
                points_pair = 20;
                handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                         position_Q[0],
                                                         points_pair,
                                                         &counter, &points_call,
                                                         &points_opponents,
                                                         hands[counter],
                                                         (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("K 20?\n");
                  pairs--;
                  // sollte ich hier nun auch theoretische positions_Q
                  // "hochrutschen"? - 1 auf 0, 2 auf 1
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
              }
            }
          }
          
          if (counter == ENOUGH)
          {
            break;
          }
          
          else
          {
            playCard(&(hands)[counter][position[counter]]);
            printHand((hands)[counter], 6, player[counter],
                      players[counter].name_);
            printf("---------------------------------------------------------------\n");
            break;
          }
          
        case 'w':
          position[counter] = 1;
          
          // check for 20 or 40
          if (pairs > 0)
          {
            // second card = QUEEN
            if (position[counter] == position_Q[0]
                 && hands[counter][position_Q[0]].value_ == QUEEN)
            {
              // 40
              if (hands[counter][position_Q[0]].is_trump_ == TRUE)
              {
                points_pair = 40;
                
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[0],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[player[counter] MINUS_ONE],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 40?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
//                  printf("check Q 40?\n");
              }
              // 20
              else /* is_trump == FALSE */
              {
                points_pair = 20;
                
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[0],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[counter],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 20?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
//                  printf("check Q 20?\n");
              }
            }
            // second card = KING
            else if (position[counter] == position_Q[0] MINUS_ONE
                     && hands[counter][position_Q[0]].value_ == QUEEN)
            {
              // 40
              if (hands[counter][position_Q[0]].is_trump_ == TRUE)
              {
                points_pair = 40;
                
                handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                         position_Q[0] MINUS_ONE,
                                                         points_pair, &counter,
                                                         &points_call,
                                                         &points_opponents,
                                                         hands[counter],
                                                         (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("K 40?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];                            // naja, hardcoded
                  position_Q[1] = position_Q[2];                            // oba viel besseres
                  position_Q[2] = 0;                                        // fällt mir hier nit ein
                }
//                  printf("check K 40?\n");
              }
              // 20
              else /* is_trump == FALSE */
              {
                points_pair = 20;
                handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                         position_Q[0],
                                                         points_pair,
                                                         &counter, &points_call,
                                                         &points_opponents,
                                                         hands[counter],
                                                         (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("K 20?\n");
                  pairs--;
                  // sollte ich hier nun auch theoretische positions_Q
                  // "hochrutschen"? - 1 auf 0, 2 auf 1
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
//                  printf("check K 20?\n");
              }
            }
          }
          
          if (counter == ENOUGH)
          {
            break;
          }
          
          else
          {
            playCard(&(hands)[counter][position[counter]]);
            printHand((hands)[counter], 6, player[counter],
                      players[counter].name_);
            printf("---------------------------------------------------------------\n");
            break;
          }
          
        case 'e':
          position[counter] = 2;
          
          // check for 20 or 40
          if (pairs > 0)
          {
            // third card = QUEEN
            if (position[counter] == position_Q[0]
                 && hands[counter][position_Q[0]].value_ == QUEEN)
            {
              // 40
              if (hands[counter][position_Q[0]].is_trump_ == TRUE)
              {
                points_pair = 40;
                
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[0],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[counter],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 40?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check Q 40?\n");
              }
              // 20
              else /* is_trump == FALSE */
              {
                points_pair = 20;
                
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[0],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[counter],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 20?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check Q 20?\n");
              }
            }
            // third card = KING
            else if ((position[counter] == position_Q[0] MINUS_ONE
                     && hands[counter][position_Q[0]].value_ == QUEEN)
                     || (position[counter] == position_Q[1] MINUS_ONE
                         && hands[counter][position_Q[1]].value_ == QUEEN))
            {
              // 40
              if (hands[counter][position_Q[0]].is_trump_ == TRUE)
              {
                points_pair = 40;
                
                handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                         position_Q[0] MINUS_ONE,
                                                         points_pair, &counter,
                                                         &points_call,
                                                         &points_opponents,
                                                         hands[counter],
                                                         (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("K 40?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];                            // naja, hardcoded
                  position_Q[1] = position_Q[2];                            // oba viel besseres
                  position_Q[2] = 0;                                        // fällt mir hier nit ein
                }
                
//                  printf("check K 40?\n");
              }
              // 40
              else if (hands[counter][position_Q[1]].is_trump_ == TRUE)
              {
                points_pair = 40;
                
                handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                         position_Q[1] MINUS_ONE,
                                                         points_pair, &counter,
                                                         &points_call,
                                                         &points_opponents,
                                                         hands[counter],
                                                         (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("K 40?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];                            // naja, hardcoded
                  position_Q[1] = position_Q[2];                            // oba viel besseres
                  position_Q[2] = 0;                                        // fällt mir hier nit ein
                }
                
//                  printf("check K 40?\n");
              }
              // 20
              else if (hands[counter][position_Q[0]].is_trump_ != TRUE)
              {
                points_pair = 20;
                
                handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                         position_Q[0],
                                                         points_pair,
                                                         &counter, &points_call,
                                                         &points_opponents,
                                                         hands[counter],
                                                         (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("K 20?\n");
                  pairs--;
                  // sollte ich hier nun auch theoretische positions_Q
                  // "hochrutschen"? - 1 auf 0, 2 auf 1
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check K 20?\n");
              }
              // 20
              else if (hands[counter][position_Q[1]].is_trump_ != TRUE)
              {
                points_pair = 20;
                
                handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                         position_Q[1],
                                                         points_pair,
                                                         &counter, &points_call,
                                                         &points_opponents,
                                                         hands[counter],
                                                         (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("K 20?\n");
                  pairs--;
                  // sollte ich hier nun auch theoretische positions_Q
                  // "hochrutschen"? - 1 auf 0, 2 auf 1
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check K 20?\n");
              }
            }
          }
          
          if (counter == ENOUGH)
          {
            break;
          }
          
          else
          {
            playCard(&(hands)[counter][position[counter]]);
            printHand((hands)[counter], 6, player[counter],
                      players[counter].name_);
            printf("---------------------------------------------------------------\n");
            break;
          }
          
        case 'a':
          position[counter] = 3;
          
          // check for 20 or 40
          if (pairs > 0)
          {
            // fourth card = QUEEN
            if ((position[counter] == position_Q[0]
                 && hands[counter][position_Q[0]].value_ == QUEEN)
                || ((position[counter] == position_Q[1]
                     && hands[counter][position_Q[1]].value_ == QUEEN)))
            {
              // 40
              if (hands[counter][position_Q[0]].is_trump_ == TRUE)
              {
                points_pair = 40;
                
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[0],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[counter],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 40?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check Q 40?\n");
              }
              // 40
              else if (hands[counter][position_Q[1]].is_trump_ == TRUE)
              {
                points_pair = 40;
                
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[1],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[counter],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 40?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check Q 40?\n");
              }
              // 20
              else if (hands[counter][position_Q[0]].is_trump_ != TRUE)
              {
                points_pair = 20;
                
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[0],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[counter],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 20?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check Q 20?\n");
              }
              // 20
              else if (hands[counter][position_Q[1]].is_trump_ != TRUE)
              {
                points_pair = 20;
                
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[1],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[counter],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 20?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check Q 20?\n");
              }
            }
            // fourth card = KING
            else if ((position[counter] == position_Q[0] MINUS_ONE
                     && hands[counter][position_Q[0]].value_ == QUEEN)
                     || (position[counter] == position_Q[1] MINUS_ONE
                         && hands[counter][position_Q[1]].value_ == QUEEN))
            {
              if (position_Q[0] > position[0])  // first 20 or 40
              {
                // 40
                if (hands[counter][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0] MINUS_ONE,
                                                           points_pair, &counter, &points_call,
                                                           &points_opponents,
                                                           hands[counter],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[counter].bool_pair_ == TRUE)
                  {
//                      printf("K 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                  
//                    printf("check K 40?\n");
                }
                // 20
                else if (hands[counter][position_Q[0]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0],
                                                           points_pair,
                                                           &counter, &points_call,
                                                           &points_opponents,
                                                           hands[counter],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[counter].bool_pair_ == TRUE)
                  {
//                      printf("K 20?\n");
                    pairs--;
                    // sollte ich hier nun auch theoretische positions_Q
                    // "hochrutschen"? - 1 auf 0, 2 auf 1
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                    printf("check K 20?\n");
                }
              }
              
              else /* if (position_Q[0] < position[0]) */ // second 20 or 40
              {
                // 40
                if (hands[counter][position_Q[1]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[1] MINUS_ONE,
                                                           points_pair, &counter, &points_call,
                                                           &points_opponents,
                                                           hands[counter],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[counter].bool_pair_ == TRUE)
                  {
//                      printf("K 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                  
//                    printf("check K 40?\n");
                }
                // 20
                else if (hands[counter][position_Q[1]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[1],
                                                           points_pair,
                                                           &counter, &points_call,
                                                           &points_opponents,
                                                           hands[counter],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[counter].bool_pair_ == TRUE)
                  {
//                      printf("K 20?\n");
                    pairs--;
                    // sollte ich hier nun auch theoretische positions_Q
                    // "hochrutschen"? - 1 auf 0, 2 auf 1
                    position_Q[0] = position_Q[1];
                    position_Q[1] = position_Q[2];
                    position_Q[2] = 0;
                  }
                  
//                    printf("check K 20?\n");
                }
              }
            }
          }
          
          if (counter == ENOUGH)
          {
            break;
          }
          
          else
          {
            playCard(&(hands)[counter][position[counter]]);
            printHand((hands)[counter], 6, player[counter],
                      players[counter].name_);
            printf("---------------------------------------------------------------\n");
            break;
          }
          
        case 's':
          position[counter] = 4;
          
          // check for 20 or 40
          if (pairs > 0)
          {
            // fifth card = QUEEN
            if ((position[counter] == position_Q[0]
                 && hands[counter][position_Q[0]].value_ == QUEEN)
                || ((position[counter] == position_Q[1]
                     && hands[counter][position_Q[1]].value_ == QUEEN)))
            {
              // 40
              if (hands[counter][position_Q[0]].is_trump_ == TRUE)
              {
                points_pair = 40;
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[0],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[counter],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 40?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check Q 40?\n");
              }
              // 40
              else if (hands[counter][position_Q[1]].is_trump_ == TRUE)
              {
                points_pair = 40;
                
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[1],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[counter],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 40?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check Q 40?\n");
              }
              // 20
              else if (hands[counter][position_Q[0]].is_trump_ != TRUE)
              {
                points_pair = 20;
                
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[0],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[counter],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 20?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check Q 20?\n");
              }
              // 20
              else if (hands[counter][position_Q[1]].is_trump_ != TRUE)
              {
                points_pair = 20;
                
                handle_pairs[counter].bool_pair_
                  = pairHandler(handle_pairs, position_Q[1],
                                points_pair, &counter, &points_call,
                                &points_opponents,
                                hands[counter],
                                (player[0] == buffer_start));
                if (handle_pairs[counter].bool_pair_ == TRUE)
                {
//                    printf("Q 20?\n");
                  
                  pairs--;
                  
                  position_Q[0] = position_Q[1];
                  position_Q[1] = position_Q[2];
                  position_Q[2] = 0;
                }
                
//                  printf("check Q 20?\n");
              }
            }
            // fifth card = KING
            else if ((position[counter] == position_Q[0] MINUS_ONE
                     && hands[counter][position_Q[0]].value_ == QUEEN)
                     || (position[counter] == position_Q[1] MINUS_ONE
                         && hands[counter][position_Q[1]].value_ == QUEEN)
                     || (position[counter] == position_Q[2] MINUS_ONE
                         && hands[counter][position_Q[2]].value_ == QUEEN))
            {
              // first 20 or 40
              if (position_Q[0] > position[0])
              {
                // 40
                if (hands[counter][position_Q[0]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0] MINUS_ONE,
                                                           points_pair, &counter, &points_call,
                                                           &points_opponents,
                                                           hands[counter],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[counter].bool_pair_ == TRUE)
                  {
//                      printf("K 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                  
//                    printf("check K 40?\n");
                }
                // 20
                else if (hands[counter][position_Q[0]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[0] MINUS_ONE,
                                                           points_pair, &counter, &points_call,
                                                           &points_opponents,
                                                           hands[counter],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[counter].bool_pair_ == TRUE)
                  {
//                      printf("K 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                  
//                    printf("check K 20?\n");
                }
              }
              // third 20 or 40
              else if (position_Q[2] MINUS_ONE == 4)      // only possible
                                                          // on position 4
              {
                // 40
                if (hands[counter][position_Q[2]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[2] MINUS_ONE,
                                                           points_pair, &counter, &points_call,
                                                           &points_opponents,
                                                           hands[counter],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[counter].bool_pair_ == TRUE)
                  {
//                      printf("K 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                  
//                    printf("check K 40?\n");
                }
                // 20
                else if (hands[counter][position_Q[2]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  
                  handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[2] MINUS_ONE,
                                                           points_pair, &counter, &points_call,
                                                           &points_opponents,
                                                           hands[counter],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[counter].bool_pair_ == TRUE)
                  {
//                      printf("K 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                  
//                    printf("check K 20?\n");
                }
              }
              // second 20 or 40
              else /* best work around yet :D */
              {
                // 40
                if (hands[counter][position_Q[1]].is_trump_ == TRUE)
                {
                  points_pair = 40;
                  
                  handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[1] MINUS_ONE,
                                                           points_pair, &counter, &points_call,
                                                           &points_opponents,
                                                           hands[counter],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[counter].bool_pair_ == TRUE)
                  {
//                      printf("K 40?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                  
//                    printf("check K 40?\n");
                }
                // 20
                else if (hands[counter][position_Q[1]].is_trump_ != TRUE)
                {
                  points_pair = 20;
                  handle_pairs[counter].bool_pair_ = pairHandler(handle_pairs,
                                                           position_Q[1] MINUS_ONE,
                                                           points_pair, &counter, &points_call,
                                                           &points_opponents,
                                                           hands[counter],
                                                           (player[0] == buffer_start));
                  if (handle_pairs[counter].bool_pair_ == TRUE)
                  {
//                      printf("K 20?\n");
                    
                    pairs--;
                    
                    position_Q[0] = position_Q[1];                            // naja, hardcoded
                    position_Q[1] = position_Q[2];                            // oba viel besseres
                    position_Q[2] = 0;                                        // fällt mir hier nit ein
                  }
                  
//                    printf("check K 20?\n");
                }
              }
            }
          }
          
          if (counter == ENOUGH)
          {
            break;
          }
          
          else
          {
            playCard(&(hands)[counter][position[counter]]);
            printHand((hands)[counter], 6, player[counter],
                      players[counter].name_);
            printf("---------------------------------------------------------------\n");
            break;
          }
          
        case 'd':
          position[counter] = 5;
          
          // check for 20 or 40
          if (pairs > 0)
          {
            // at this position we will only find a QUEEN
            if ((position[counter] == position_Q[0]
                 && hands[counter][position_Q[0]].value_ == QUEEN)
                || (position[counter] == position_Q[1]
                    && hands[counter][position_Q[1]].value_ == QUEEN)
                || (position[counter] == position_Q[2]
                    && hands[counter][position_Q[2]].value_ == QUEEN))
              
              /* QUICK AND DIRTY FIX .. IMPROVE IN "POST PRODUCTION" (; */
              
            {
              for (int j = 0; j < 3; j++)                       // sowas tu'I normalerwiese NIE
              {
                if (position_Q[j] == position[counter] /* 5 */)
                {
                  // 40
                  if (hands[counter][position_Q[j]].is_trump_ == TRUE)
                  {
                    points_pair = 40;
                    
                    handle_pairs[counter].bool_pair_
                      = pairHandler(handle_pairs, position_Q[j],
                                    points_pair, &counter, &points_call,
                                    &points_opponents,
                                    hands[counter],
                                    (player[0] == buffer_start));
                    if (handle_pairs[counter].bool_pair_ == TRUE)
                    {
//                        printf("Q 40?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                      printf("check Q 40?\n");
                  }
                  // 20
                  else /* is_trump == FALSE */
                  {
                    points_pair = 20;
                    
                    handle_pairs[counter].bool_pair_
                      = pairHandler(handle_pairs, position_Q[j],
                                    points_pair, &counter, &points_call,
                                    &points_opponents,
                                    hands[counter],
                                    (player[0] == buffer_start));
                    if (handle_pairs[counter].bool_pair_ == TRUE)
                    {
//                        printf("Q 20?\n");
                      
                      pairs--;
                      
                      position_Q[0] = position_Q[1];
                      position_Q[1] = position_Q[2];
                      position_Q[2] = 0;
                    }
                    
//                      printf("check Q 20?\n");
                  }
                }
              }
            }
          }
          
          if (counter == ENOUGH)
          {
            break;
          }
          
          else
          {
            playCard(&(hands)[counter][position[counter]]);
            printHand((hands)[counter], 6, player[counter],
                      players[counter].name_);
            printf("---------------------------------------------------------------\n");
            break;
          }
          
        default:
          break;
      }
    }
    
    // opponents' inputs - valid: suit, trump, bock
    
    // rewind bock
    if (hands[counter][0].is_bock_ == TRUE) // hard coded 0 sollte passen
                                                        // zumal: ein bock: alle bock
    {
      for (counter_cards = 0; counter_cards < HAND; counter_cards++)
      {
        hands[counter][counter_cards].is_bock_ = FALSE;
      }
    }

    // check position(s) of valid cards for this round
    // do not allow any other card
    // delete non valid characters from commands
    count_bock    = 0;   // bock
    count_suit    = 0;   // right suit
    count_trump   = 0;   // trump
    
    // seek
    for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
         counter_cards++)
    {
      // have trump
      if ((hands)[counter][counter_cards].is_trump_ == TRUE)
      {
//            if ((hands)[player[i] MINUS_ONE][counter_cards].is_trump_)
        count_trump++;
      }
    }
    
    
//    // opponent_1 must trump call
//    if (counter == 1)
//    {
//
//    }
//
//    // opponent_2 must trump call if opponent_1 could not but does not have to
//    // trump opponent_1 if they have already trumped call
//    if (counter == 2)
//    {
//
//    }
    
    
    // further playing
    if (counter != 0 && counter < 3)
    {
      for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
           counter_cards++)
      {
        // right suit
        if (counter == 1)
        {
          if ((hands)[counter][counter_cards].suit_
              == (hands)[counter - 1][position[counter - 1]].suit_)
          {
            // right suit is trump (: trump has been played )
            if (hands[counter][counter_cards].is_trump_ == TRUE)
            {
              count_trump--;
            }
            count_suit++;
          }
        }
        
        else /* if (i == 2) */
        {
          if ((hands)[counter][counter_cards].suit_
              == (hands)[counter - 2][position[counter - 2]].suit_)
          {
            // rigtht suit is trump (: trump has been played )
            if (hands[counter][counter_cards].is_trump_ == TRUE)
            {
              count_trump--;
            }
            count_suit++;
          }
        }
      } // end: for()
      
      for (counter_cards = 0; counter_cards < (HAND/* - counter_turns*/);
           counter_cards++)
      {
        // no trump && no suit = all bock
        if (counter == 1)
        {
          if ((hands)[counter][counter_cards].is_trump_ == FALSE
              && (hands)[counter][counter_cards].suit_
              != (hands)[counter - 1][position[counter - 1]].suit_)
          {
            count_bock++;
          }
        }
        
        else /* if (i == 2) */
        {
          if ((hands)[counter][counter_cards].is_trump_ == FALSE
              && (hands)[counter][counter_cards].suit_
              != (hands)[counter - 2][position[counter - 2]].suit_)
          {
            count_bock++;
          }
        }
      } // end: for()
      
      // .. and adjust ------------
      strcpy(players_commands[counter], "000000");    // maybe there is a better
                                                // more professinal way
      
      counter_trump   = 0;
      counter_suit    = 0;
      counter_cards   = 0;
      counter_hand    = 0;
      counter_command = 0;
      
      // all bock - no matter which answer
      if ((count_suit == 0 && count_trump == 0))
      {
        // according to turn, -1
//          printf("Alles Bock!\n");
        while (counter_hand < (HAND))
        {
          if ((hands)[counter][counter_hand].suit_ != NULL)
          {
            players_commands[counter][counter_cards] = commands[counter_command];
            counter_command++;
            counter_cards++;
          }
          
          else
          {
            counter_command++;
          }
          
          counter_hand++;
        }
      }
      
      // no valid suit but at least one trump
      else if (count_suit == 0 && count_trump > 0)
      {
        counter_trump   = 0;
        counter_suit    = 0;
        counter_cards   = 0;
        counter_hand    = 0;
        counter_command = 0;
        // must hold trump
        // allow trump(s)
//          printf("Muaßt an Trumpf spiel'n!\n");

        while (counter_hand < (HAND/* - counter_turns*/)/* eben nicht strlen(players_commands[i])*/)
        {
          // differenciate between answer 1 and answer 2
          // answer 1 must trump call & answer 2 must trump either
          if ((hands)[counter][counter_hand].suit_ != NULL
              && hands[counter][counter_hand].is_trump_ == TRUE)
          {
            players_commands[counter][counter_cards] = commands[counter_command];
            counter_command++;
            counter_cards++;
          }
          
          else
          {
            counter_command++;
          }
          
          counter_hand++;
        }
        
        // remove right
        for (counter_cards = count_trump; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
        {
          players_commands[counter][counter_cards] = '\0';
        }
      } // end else if (must play trump)
      
      else /* must hold suit, at leas one */
      {
//          printf("Muaßt Farb' spielen!\n");
        counter_cards = 0;
        counter_command = 0;
        
        // remove left
        for (counter_hand = 0; counter_hand < (HAND/* - counter_turns*/); counter_hand++)
        {
          // suit found
          if (counter == 1)   // first to answer
          {
            if ((hands)[counter][counter_hand].suit_
                == (hands)[counter - 1][position[counter - 1]].suit_)
            {
//                printf("gleiche Farbe @ %d\n", counter_hand);
//                printf("%c = %c\n", players_commands[i][counter_hand], commands[counter_command]);
              players_commands[counter][counter_cards] = commands[counter_command];
              counter_command++;
              counter_cards++;
            }
            else
            {
              counter_command++;
            }
          }
          
          else /* (i == 2) */   // second to answer
          {
            if ((hands)[counter][counter_hand].suit_
                == (hands)[counter - 2][position[counter - 2]].suit_)
            {
//                printf("gleiche Farbe @ %d\n", counter_hand);
              players_commands[counter][counter_cards] = commands[counter_command];
              counter_command++;
              counter_cards++;
            }
            else
            {
//                players_commands[i][counter_hand] = commands[counter_command];
              counter_command++;
            }
          }
        }
        
        // remove right
        for (counter_cards = count_suit; counter_cards < (HAND/* - counter_turns*/); counter_cards++)
        {
          players_commands[counter][counter_cards] = '\0';
        }
      } // end else (must play suit)
      
      // now that we have found valid cards, go on and only allow permitted cards
      // we do not have to mind case:     all bock
      // but                              trump (in case trump has been played)
      //                                  suit
      // what do we need? - (answer 1): called card's value
      //                    (answer 2): called card's value and answer 1's value
      
      if (counter == 1)
      {
        // if there is at least one valid card to answer with
        if (!(count_suit == 0 && count_trump == 0))
        {
          // compare values and only allow weaker cards if there is not one
          // which could trump call
          counter_command = 0;
          counter_cards   = 0;
          count_permit    = 0;
          
          if (count_suit > 0)
          {
            while (players_commands[counter][counter_command] != '\0')
            {
              if ((hands)[counter][counter_cards].value_ >
                  /* called card */ hands[0][position[0]].value_)
              {
                printf("%s %d - %s %d\n", hands[0][position[0]].suit_,
                       hands[0][position[0]].value_,
                       (hands)[counter][counter_cards].suit_,
                       (hands)[counter][counter_cards].value_);
                buffer_higher[counter_cards] = players_commands[counter][counter_cards];
                buffer_higher[counter_cards ADD_ONE] = '\0';
                counter_cards++;
                
                if (player[0] == initial_order[0])
                {
                  bool_trumped_already = TRUE;
                }
              }
              counter_command++;
            }
          }
          
          if (counter_cards > 0)
          {
            strcpy(players_commands[counter], "\0\0\0\0\0\0");
            strcpy(players_commands[counter], buffer_higher);
            strcpy(buffer_higher, "\0\0\0\0\0\0");
          }
        }
      }
      
      else /* if (i == 2) */
      {
        // if there is at least one valid card to answer with
        if (!(count_suit == 0 && count_trump == 0))
        {
          // compare values and only allow weaker cards if there is not one
          // which could trump any (call || answer 1)
          counter_command = 0;
          counter_cards   = 0;
          count_permit    = 0;
          
          if (count_suit > 0)
          {
            // case: first answer could not trump call
            if (bool_trumped_already == FALSE)
            {
              // TODO: this!
              printf("TODO!\n");
//              while (players_commands[counter][counter_command] != '\0')
//              {
//                // has to trump caller if possible
//                if ((hands)[counter][counter_command].value_ <
//                    /* called card */ hands[player[0] MINUS_ONE][position[0]].value_
//                    || (hands)[counter][counter_cards].value_ <
//                    /* answer 1's card */ hands[player[counter - 1] MINUS_ONE][position[1/*0*/]].value_)
//                {
//                  // does not work properly
//                }
//                else
//                {
//                  buffer_higher[counter_cards] = players_commands[counter][counter_cards];
//                  buffer_higher[counter_cards ADD_ONE] = '\0';
//                  counter_cards++;
//                }
//
//              counter_command++;
//              }
            }

            else
            {
              // no need to trump caller
            }
          }
          
          else if (count_trump > 0)
          {
//              printf("%d\n", (hands)[player[i] MINUS_ONE][position[i MINUS_ONE]].is_trump_);
            if ((hands)[counter][position[counter MINUS_ONE]].is_trump_)
            {
              while (players_commands[counter][counter_command] != '\0')
              {
                // only need to compare trump's value with answer 1
                if ((hands)[counter][counter_cards].value_ <
                    /* answer 1's card */ hands[player[counter MINUS_ONE] MINUS_ONE][position[counter MINUS_ONE]].value_)
                {
                  printf("\n!!! 16504 !!!\n");
                }
                else
                {
                  buffer_higher[counter_cards] = players_commands[counter][counter_cards];
                  buffer_higher[counter_cards ADD_ONE] = '\0';
                  counter_cards++;
                }
                counter_command++;
              }
            }
          }
          
          if (counter_cards > 0)
          {
            strcpy(players_commands[counter], "\0\0\0\0\0\0");
            strcpy(players_commands[counter], buffer_higher);
            strcpy(buffer_higher, "\0\0\0\0\0\0");
          }
        }
      }
      
      // -----------

      do
      {
        system ("/bin/stty raw");
        buffer = getchar();
        printf("\r");
        if (!(check = seekAndDestroy(buffer, players_commands[counter])))
          printf("Invalid input!\n"); //printf("Gibt's nicht!\n");
      } while (!check);
      system ("/bin/stty cooked");
      
      switch (buffer)
      {
        case 'q':
          position[counter] = 0;
          playCard(&(hands)[counter][position[counter]]);
//          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
          printHand((hands)[counter], 6, player[counter],
                    players[counter].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'w':
          position[counter] = 1;
          playCard(&(hands)[counter][position[counter]]);
//          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
          printHand((hands)[counter], 6, player[counter],
                    players[counter].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'e':
          position[counter] = 2;
          playCard(&(hands)[counter][position[counter]]);
//          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
          printHand((hands)[counter], 6, player[counter],
                    players[counter].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'a':
          position[counter] = 3;
          playCard(&(hands)[counter][position[counter]]);
//          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
          printHand((hands)[counter], 6, player[counter],
                    players[counter].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 's':
          position[counter] = 4;
          playCard(&(hands)[counter][position[counter]]);
//          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
          printHand((hands)[counter], 6, player[counter],
                    players[counter].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        case 'd':
          position[counter] = 5;
          playCard(&(hands)[counter][position[counter]]);
//          playCard(&(buffer_turn)[i].card_);
//            printHand((hands)[player[i] MINUS_ONE], 6, turn[i],
//                      players[turn[i] MINUS_ONE].name_);
          printHand((hands)[counter], 6, player[counter],
                    players[counter].name_);
          printf("---------------------------------------------------------------\n");
          break;
          
        default:
          break;
      }
    }
  }
  
  // ------------------
  
  // --------------------------------------------------------------------------------
//  if (i > 3) // exit condition: 20 or 40 shown only - enough & exit
//  {
//    printf("points caller: %d\n", points_call);
//    printf("points opponents: %d\n", points_opponents);
//    break;
//  }

  // ------------------
  
  counter = 0;
  
  printf("Table:\n[[%s %d] [%s %d] [%s %d]\n",
         hands[counter][position[counter]].suit_,
         hands[counter][position[counter]].value_,
         (hands)[counter + 1][position[counter + 1]].suit_,
         (hands)[counter + 1][position[counter + 1]].value_,
         (hands)[counter + 2][position[counter + 2]].suit_,
         (hands)[counter + 2][position[counter + 2]].value_);

  points_and_next = next(initial_order, player,
                         hands[counter][position[counter]],
                         (hands)[counter + 1][position[counter + 1]],
                         (hands)[counter + 2][position[counter + 2]]);
  
  printf("Player %d, won %d points.\n", points_and_next.winner_,
                                        points_and_next.points_);
  
  removeCard(&hands[0][position[0]]);
  removeCard(&hands[1][position[1]]);
  removeCard(&hands[2][position[2]]);
  
  // remove used commands
  printf("1. %s\n2. %s\n3. %s\n", players_commands[0], players_commands[1], players_commands[2]);
  
  // player 1
  counter_position = 0 ;
  for (counter_cards = 0; counter_cards < HAND; counter_cards++)
  {
    printf("1. %s\n", players_commands[0]);
    if ((hands)[0][counter_cards].suit_ != NULL)
    {
      players_commands[0][counter_position] = commands[counter_cards];
      counter_position++;
    }
  }
  
  players_commands[0][counter_position] = '\0';
  
  // player 2
  counter_position = 0 ;
  for (counter_cards = 0; counter_cards < HAND; counter_cards++)
  {
    printf("2. %s\n", players_commands[1]);
    if ((hands)[1][counter_cards].suit_ != NULL)
    {
      players_commands[1][counter_position] = commands[counter_cards];
      counter_position++;
    }
  }
  
  players_commands[1][counter_position] = '\0';
  
  // player 3
  counter_position = 0 ;
  for (counter_cards = 0; counter_cards < HAND; counter_cards++)
  {
    printf("3. %s\n", players_commands[2]);
    if ((hands)[player[2] MINUS_ONE][counter_cards].suit_ != NULL)
    {
      players_commands[2][counter_position] = commands[counter_cards];
      counter_position++;
    }
  }
  
  players_commands[2][counter_position] = '\0';
  
  printf("1. %s\n2. %s\n3. %s\n", players_commands[0], players_commands[1], players_commands[2]);
  
  return points_and_next;
}

void printBummerl(Player* players)
{
  // all different
  if (players[0].points_ != players[1].points_
      && players[1].points_ != players[2].points_
      && players[0].points_ != players[2].points_)
  {
    // Bummerl goes to player 1
    if (players[0].points_ < players[1].points_
        && players[0].points_ < players[2].points_)
    {
      if (players[0].points_ == 0)
      {
        printf("%s gains two Bummerl.\n", players[0].name_);
        // add two Bummerl to score_bummerl
      }
      else
      {
        printf("%s gains a Bummerl.\n", players[0].name_);
        // add a Bummerl to score_bummerl
      }
    }
    
    // Bummerl goes to player 2
    else if (players[1].points_ < players[2].points_
             && players[1].points_ < players[0].points_)
    {
      if (players[1].points_ == 0)
      {
        printf("%s gains two Bummerl.\n", players[1].name_);
        // add two Bummerl to score_bummerl
      }
      
      else
      {
        printf("%s gains a Bummerl.\n", players[1].name_);
        // add a Bummerl to score_bummerl
      }
    }
    
    // Bummerl goes to player 3
    else /* if (players[2].points_ < players[0].points_
             && players[2].points_ < players[1].points_) */
    {
      if (players[2].points_ == 0)
      {
        printf("%s gains two Bummerl.\n", players[2].name_);
        // add two Bummerl to score_bummerl
      }
      
      else
      {
        printf("%s gains a Bummerl.\n", players[2].name_);
        // add a Bummerl to score_bummerl
      }
    }
  }
  
  // player 1 and player 2 have identical points
  else if (players[0].points_ == players[1].points_)
  {
    if (players[0].points_ < players[2].points_)
    {
      if (players[0].points_ == 0)
      {
        printf("%s and %s gain two Bummerl each.\n",
               players[0].name_, players[1].name_);
        // add two Bummerl to their scores
      }
      
      else
      {
        printf("%s and %s gain a Bummerl each.\n",
               players[0].name_, players[1].name_);
        // add a Bummerl to their scores
      }
    }
    else
    {
      if (players[2].points_ == 0)
      {
        printf("%s gains two Bummerl.\n", players[2].name_);
        // add two Bummerl to score_bummerl
      }
      
      else
      {
        printf("%s gains a Bummerl.\n", players[2].name_);
        // add a Bummerl to score_bummerl
      }
    }
  }
  
  // player 1 and player 3 have identical points
  else if (players[0].points_ == players[2].points_)
  {
    if (players[0].points_ < players[1].points_)
    {
      if (players[0].points_ == 0)
      {
        printf("%s and %s gain two Bummerl each.\n",
               players[0].name_, players[2].name_);
        // TODO
      }
      
      else
      {
        printf("%s and %s gain a Bummerl each.\n",
               players[0].name_, players[2].name_);
        // TODO
      }
    }
    else
    {
      if (players[1].points_ == 0)
      {
        printf("%s gains two Bummerl.\n", players[1].name_);
        // TODO
      }
      
      else
      {
        printf("%s gains a Bummerl.\n", players[1].name_);
        // TODO
      }
    }
  }
  
  // player 2 and player 3 have identical points
  else /* if (players[1].points_ == players[2].points_) */
  {
    if (players[1].points_ < players[0].points_)
    {
      if (players[1].points_ == 0)
      {
        printf("%s and %s gain two Bummerl each.\n",
               players[1].name_, players[2].name_);
        // TODO
      }
      
      else
      {
        printf("%s and %s gain a Bummerl each.\n",
               players[1].name_, players[2].name_);
        // TODO
      }
    }
    else
    {
      if (players[0].points_ == 0)
      {
        printf("%s gains two Bummerl.\n", players[0].name_);
        // TODO
      }
      
      else
      {
        printf("%s gains a Bummerl.\n", players[0].name_);
        // TODO
      }
    }
  }
}

void distributePoints(int points_call, int points_opponents,
                        Points* points_and_caller)
{
  if (points_call < 66)
  {
    if (points_call == 0)
    {
      (*points_and_caller).points_ = 3;
      // points_and_caller.caller_ = IRGENDWER;
      (*points_and_caller).winner_ = OPPONENTS_WIN;
      printf("Opponents win 3 points!\n");
      printf("---------------------------------------------------------------\n");
      // return *points_and_caller;
    }
    
    else if (points_call < 33)
    {
      (*points_and_caller).points_ = 2;
      // points_and_caller.caller_ = IRGENDWER;
      (*points_and_caller).winner_ = OPPONENTS_WIN;
      printf("Opponents win 2 points!\n");
      printf("---------------------------------------------------------------\n");
      // return *points_and_caller;
    }
    
    else /*if (points_call > 33)*/
    {
      (*points_and_caller).points_ = 1;
      // points_and_caller.caller_ = IRGENDWER;
      (*points_and_caller).winner_ = OPPONENTS_WIN;
      printf("Opponents win 1 points!\n");
      printf("---------------------------------------------------------------\n");
      // return *points_and_caller;
    }
  }
  
  else /* caller wins (modeGame() = player 1) */
  {
    if (points_opponents == 0)
    {
      (*points_and_caller).points_ = 3;
      (*points_and_caller).winner_ = TURN_PLAYER_1 /*buffer_start*/;
      printf("Player %d wins 3 points!\n", (*points_and_caller).winner_);
      printf("---------------------------------------------------------------\n");
      // return *points_and_caller;
    }
    
    else if (points_opponents < 33)
    {
      (*points_and_caller).points_ = 2;
      (*points_and_caller).winner_ = TURN_PLAYER_1;
      printf("Player %d wins 2 points!\n", (*points_and_caller).winner_);
      printf("---------------------------------------------------------------\n");
      // return *points_and_caller;
    }
    
    else /*if (points_opponent > 33)*/
    {
      (*points_and_caller).points_ = 1;
      (*points_and_caller).winner_ = TURN_PLAYER_1;
      printf("Player %d wins 1 points!\n", (*points_and_caller).winner_);
      printf("---------------------------------------------------------------\n");
      // return *points_and_caller;
    }
  }
}

void sortOrderHands(int start, Card** hands, Card** initial_hands)
{
  Card buffer[6] = {"", 0, "", "", 0, 0};
  Card buffer_initial[6] = {"", 0, "", "", 0, 0};
  int counter_cards = 0;
  
  if (start == TURN_PLAYER_1)
  {
    hands[0] = initial_hands[0];
    hands[1] = initial_hands[1];
    hands[2] = initial_hands[2];
  }
  
  else if (start == TURN_PLAYER_2)
  {
    hands[0] = initial_hands[1];
    hands[1] = initial_hands[2];
    hands[2] = initial_hands[0];
  }
  
  else /* if (start == TURN_PLAYER_3) */
  {
    hands[0] = initial_hands[2];
    hands[1] = initial_hands[0];
    hands[2] = initial_hands[1];
  }
}

void sortOrderPlayers(int start, Player* players, Player* initial_order)
{
  if (start == TURN_PLAYER_1)
  {
     players[0] = initial_order[0];
     players[1] = initial_order[1];
     players[2] = initial_order[2];
  }
  
  else if (start == TURN_PLAYER_2)
  {
    players[0] = initial_order[1];
    players[1] = initial_order[2];
    players[2] = initial_order[0];
  }

  else /* if (start == TURN_PLAYER_3)*/
  {
    players[0] = initial_order[2];
    players[1] = initial_order[0];
    players[2] = initial_order[1];
  }
}

void sortOrderCommands(int start, char** players_commands, int* initial_commands)
{
  
}
