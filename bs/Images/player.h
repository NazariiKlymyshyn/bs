#ifndef PLAYER_H
#define PLAYER_H
#include "includes.h"
#include "server.h"

const uint_fast16_t  FIELD_DIM   = 10;
const uint_fast16_t WATER_SYMB = 0;
const uint_fast16_t SHIP_SYMB   =  1 ;

/////////////////////////////////////////////////////////////////////////
//"main", the most important class, the instance of which represents the concept  //
//of a player and a game, interacting with a graphical and network interface,        //
//encapsulating the rest of things in itself.                                                            //
/////////////////////////////////////////////////////////////////////////
/// \brief The Player class

class Player{
public:
    ////////////////////////////////////////////////////////////////////////
    //structure that unites data and displays the state of each cell of the battlefield //
    ////////////////////////////////////////////////////////////////////////
    struct Unit{
        struct Pos{
            uint_fast16_t x;
            uint_fast16_t y;
        }position;
        uint_fast8_t ship : 1 ;
        uint_fast8_t miss : 1 ;
        uint_fast8_t hit : 1 ;
        uint_fast8_t destroyed : 1;
        uint_fast8_t r: 4;
    };
 ///////////////////////////////////////////////////////////////////
 //structure in which to find flags displaying the current state of the player //
  //////////////////////////////////////////////////////////////////
    struct CurrentProgramState{
        uint_fast8_t main_menu : 1;
        uint_fast8_t ships_placing : 1;
        uint_fast8_t answer_await : 1;
        uint_fast8_t turn_await : 1;
        uint_fast8_t pve_game : 1;
        uint_fast8_t opponent_field_set : 1;
        uint_fast8_t r : 2;
    } ;
    ///////////////////////////////////////////////////////////////////////////////
    //structure of objects that programmatically represent representations of game textures //
    ///////////////////////////////////////////////////////////////////////////////
    struct Image{
        Image();
        /////////////////////////////////////////////////////////////////////////////
        //in order to avoid loud chains of calls of an encapsulated similar class method Image, //
        // this public "wrapper" has been introduced providing similar functionality to parts     //
        //of the program that do not have access to the class                                                  //
        /////////////////////////////////////////////////////////////////////////////
        const QImage& GetImage(const QString & PathToImage ) const;
        QMap<QString,QImage> image;
    } ;
    //////////////////////////////////////////////////////////////////
    //structure eating all the objects that are necessary for the presentation  //
    //of each of the battlefields of both players                                              //
    //////////////////////////////////////////////////////////////////
    struct BattleField{
        Player::Unit state= { {0,0 },0,0,0,0,0};
        void SeUnitState(std::array<Unit,100>&BattleField ,const uint_fast16_t & Absciss ,const uint_fast16_t & Ordinate, const Unit & UnitState);
        std::array<Unit,100> battle_field = { state };
        const  Player::Image picture;
    } ;
    Player (const Player &) = delete;
    Player & operator=(const Player &) = delete ;
private:
    ///////////////////////////////////////////////////////////////////////
    //Primary initialization of Player::Unit::Position type object located into Player //
    //type object in order to provide proper sequence of positions [0;0] [9;9]          //
    ///////////////////////////////////////////////////////////////////////
    explicit Player();

    Player::BattleField first_player_battle_field_ , second_player_battle_field_;
    CurrentProgramState state_= { 0,0,0,0,0,0,0 };// initial state


    ////////////////////////////////////////////////////////////////////
    // algorithm - the second part of the chain of functions for determining the //
    // validity of the custom arrangement of ships                                             //
    // on the battle field, counting the number of ships of each type                 //
    ///////////////////////////////////////////////////////////////////
    const uint_fast16_t shipNum( const uint_fast16_t size ) const;
    ////////////////////////////////////////////////////////////////////
    // algorithm - the third part of the chain of functions for determining the     //
    // validity of the custom arrangement of ships                                             //
    // on the battle field, sorting out all the units of the playing field                //
    ///////////////////////////////////////////////////////////////////
    const uint_fast8_t isShip( const uint_fast16_t size, const uint_fast16_t x, const uint_fast16_t y ) const;
    ///////////////////////////////////////////////////////////////////////
    //private overload providing simplified functionality for obtaining the state        //
    //of the battlefield unit for parts of the program that have access to all private //
    //fields of the class                                                                                           //
    ///////////////////////////////////////////////////////////////////////
    const Player::Unit GetSingleUnitState(const uint_fast16_t &Absciss ,const uint_fast16_t &Ordinate) const ;

    void SetAIFieldState(const std::array<Player::Unit,100> &Field) ;
    bool  ShipIsGood(int size,  bool   is_horiz,  int   row_top,  int col_left,const char field[][FIELD_DIM]);
    //////////////////////////////////////////////////////////////////////////////
    // function initializing the launch of a chain of functions to pseudo random initialization //
    //by ships of the battlefield of artificial intelligence when the pve game mode is selected//
    //////////////////////////////////////////////////////////////////////////////
    void  SetShip(int  size, char  field[FIELD_DIM][FIELD_DIM]);
    void  SetShips(char  field[FIELD_DIM][FIELD_DIM]);
    void  GetAIField(char  field[FIELD_DIM][FIELD_DIM]);
    void  SetWater(char  field[FIELD_DIM][FIELD_DIM]);

public:
    ////////////////////////////////////////////////////////////////
    // Filling first player's battlefield with pseudo random ships placement   //
    //(used same as for any ai battlefield algorithm)                                    //
    ////////////////////////////////////////////////////////////////
    void AutoShipsPlace();
    ///////////////////////////////////////////////////////////////////////////////// ////
    // In order to the need to prohibit the creation of many objects of Player type due to the lack of //
    // possible correct use of them in a 1x1 game, a programming singleton template was used          //
    // This method will provide the only existing object of type Player in  part of the program.          //
    //////////////////////////////////////////////////////////////////////////////////////
    static Player& GetPlayerInstance();
    ///////////////////////////////////////////////////////////////////////////////
    // A function that iterates through collections containing data on the state of the playing //
    // fields of each of the players and when determining whether a player won by                //
    // destroying all the enemy�s corpses, transmits the corresponding signal to the               //
    // part of the program responsible for the graphical interface                                          //
    ///////////////////////////////////////////////////////////////////////////////
     const int_fast8_t IsWin();
     /////////////////////////////////////////////////////////////////////////////
     //in order to avoid loud chains of calls of an encapsulated similar class method Image, //
     // this public "wrapper" has been introduced providing similar functionality to parts     //
     //of the program that do not have access to the class                                                  //
     /////////////////////////////////////////////////////////////////////////////
    const QImage& GetImage(const QString & PathToImage ) const;
    ////////////////////////////////////////////////
    //Returns the state of a certain unit of the battlefield//
    ////////////////////////////////////////////////
    const Player::Unit GetSingleUnitState(const std::array<Player::Unit,100>&BattleField ,const uint_fast16_t &Absciss ,const uint_fast16_t &Ordinate) const ;
    const std::array<Player::Unit,100> GetWholeFieldState( const Player::BattleField& Field) const;
    /////////////////////////////////////////////////////////////////////
    // in order to avoid loud chains of calls in any programs part a functional        //
    // divided into three functions is provided for obtaining an object of the type //
    // that conditionally displays the state of each unit of the battlefield             //
    /////////////////////////////////////////////////////////////////////
    const Player::BattleField GetFirstPlayerField(const Player& p) const;
    const Player::BattleField GetSecondPlayerField(const Player& p) const;
    ////////////////////////////////////////////////////////////////////////////
    //function returning an object of type conditionally displaying the state of the object //
    ////////////////////////////////////////////////////////////////////////////
    const Player::CurrentProgramState GetCurrentProgramState( const Player& p) const ;
    ////////////////////////////////////////////////////////////////////
    // function initializing the launch of a chain of functions to determine the   //
    // validity of the custom arrangement of ships on the playing field               //
    ///////////////////////////////////////////////////////////////////
    const uint_fast8_t checkMyField() const;
    /////////////////////////////////////////////////
    // open to other parts of the program functionality to  //
    // change the current state of the program                 //
    /////////////////////////////////////////////////
    void SetPlayerState( Player& p , const Player::CurrentProgramState & st) ;
    ///////////////////////////////////////////////////////////////////////
    // The tool made it possible to determine the correctness of clicking the            //
    // user�s mouse with respect to the current state of the program and its minor   //
    // changes in accordance with the result                                                            //
    ///////////////////////////////////////////////////////////////////////
    void ProcessClick( const uint_fast16_t & Absciss,const uint_fast16_t & Ordinate) ;
} ;

#endif // PLAYER_H








