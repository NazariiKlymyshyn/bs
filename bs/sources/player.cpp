#include "headers/player.h"

#include "headers/includes.h"

/**
 *Primary initialization of Player::Unit::Position type object located into Player    
 *type object in order to provide proper sequence of positions [0;0] [9;9]             
 */
Player::Player() {
  for (uint_fast16_t Ordinate = 0; Ordinate != 10; ++Ordinate)
    for (uint_fast16_t Absciss = 0; Absciss != 10; ++Absciss) this -> first_player_battle_field_.battle_field[((Ordinate * 10) + Absciss) - 1].position.y = Ordinate, first_player_battle_field_.battle_field[((Ordinate * 10) + Absciss) - 1].position.x = Absciss;
}

/**                                                                                                                  
 *function returning an object of type conditionally displaying the state of the object    
 */
const Player::CurrentProgramState Player::GetCurrentProgramState(
  const Player & p
) const {
  return p.state_;
}

/**
 *In order to the need to prohibit the creation of many objects of Player type due to the lack of    
 *possible correct use of them in a 1x1 game, a programming singleton template was used             
 *This method will provide the only existing object of type Player in  part of the program.             
 */
Player & Player::GetPlayerInstance() {
  static Player instance_;
  return instance_;
}

/**
 *in order to avoid loud chains of calls of an encapsulated similar class method Image,    
 *this public "wrapper" has been introduced providing similar functionality to parts        
 *of the program that do not have access to the class                                                     
 */
const QImage & Player::GetImage(
  const QString & PathToImage
) const {
  return Player::GetPlayerInstance().first_player_battle_field_.picture.GetImage(PathToImage);
}

/**
 *Returns the state of a certain unit of the battlefield   
 */
const Player::Unit Player::GetSingleUnitState(
  /** reference to a field object, whose unit state will be changed */
  const std::array < Player::Unit, 100 > & BattleField,

    /**    constant references to the coordinates of the cell whose state must be changed */
    const uint_fast16_t & Absciss,
      const uint_fast16_t & Ordinate
) const {
  return BattleField[((Ordinate * 10) + Absciss)];
}

/**
 * private overload providing simplified functionality for obtaining the state           
 * of the battlefield unit for parts of the program that have access to all private    
 * fields of the class                                                                                              
 */
const Player::Unit Player::GetSingleUnitState(
  /**     reference to a field object, whose unit state will be changed */
  const uint_fast16_t & Absciss,
    const uint_fast16_t & Ordinate
) const {
  return this -> first_player_battle_field_.battle_field[((Ordinate * 10) + Absciss)];
}

/**
 *in order to avoid loud chains of calls in any programs part a functional
 *divided into three functions is provided for obtaining an object of the
 *type that conditionally displays the state of each unit of the battlefield
 */
const Player::BattleField Player::GetFirstPlayerField(
  const Player & p
) const {
  return p.first_player_battle_field_;
}

const Player::BattleField Player::GetSecondPlayerField(
  const Player & p
) const {
  return p.second_player_battle_field_;
}

const std::array < Player::Unit, 100 > Player::GetWholeFieldState(
  const Player::BattleField & Field
) const {
  return Field.battle_field;
}

/**
 *function initializing the launch of a chain of functions to determine the      
 *validity of the custom arrangement of ships on the playing field                  
 */
const uint_fast8_t Player::checkMyField() const {
  return (
    (4 == shipNum(1)) &&
    (3 == shipNum(2)) &&
    (2 == shipNum(3)) &&
    (1 == shipNum(4))
  );
}

/**                                                                                                      
 *algorithm - the second part of the chain of functions for determining the    
 *validity of the custom arrangement of ships                                                
 *on the battle field, counting the number of ships of each type                    
 */
const uint_fast16_t Player::shipNum(
  uint_fast16_t size
) const {
  return std::count_if(
    this -> first_player_battle_field_.battle_field.begin(),
    this -> first_player_battle_field_.battle_field.end(),
    [ & ]
    (
      auto & pos
    ) {
      return isShip(size,
        pos.position.x,
        pos.position.y
      );
    }
  );
}
/**
 *algorithm - the third part of the chain of functions for determining the        
 *validity of the custom arrangement of ships                                                
 *on the battle field, sorting out all the units of the playing field                   
 */
const uint_fast8_t Player::isShip(const uint_fast16_t size,
  const uint_fast16_t x,
    const uint_fast16_t y) const {
  if (

    /**
     *checking changes in the coordinates of the cell of the playing field
     *in order to avoid going beyond the boundaries of the playing field and data that represent its state
     */
    ((x > 0) &&
      (y > 0) &&
      (Player::GetPlayerInstance().GetSingleUnitState(x - 1, y - 1).ship != 0)) ||
    ((y > 0) &&
      (10 > x) &&
      (Player::GetPlayerInstance().GetSingleUnitState(x + 1, y - 1).ship != 0)) ||
    ((10 > y) &&
      (10 > x) && (Player::GetPlayerInstance().GetSingleUnitState(x + 1, y + 1).ship != 0)) ||
    ((x > 0) &&
      (10 > y) &&
      (Player::GetPlayerInstance().GetSingleUnitState(x - 1, y + 1).ship != 0)) ||
    ((x > 0) &&
      (Player::GetPlayerInstance().GetSingleUnitState(x - 1, y).ship != 0)) ||
    ((y > 0) &&
      (Player::GetPlayerInstance().GetSingleUnitState(x, y - 1).ship != 0))
  )
    return false;
  if (
    (y > 0) &&
    (Player::GetPlayerInstance().GetSingleUnitState(x, y - 1).ship != 0)
  ) return false;
  if (Player::GetPlayerInstance().GetSingleUnitState(x, y).ship == 0)
    return false;
  uint_fast8_t tmp = x, num = 0;
  while (
    (Player::GetPlayerInstance().GetSingleUnitState(tmp, y).ship != 0) &&
    (tmp < 10)) {
    tmp++, num++;
  }
  if (num == size) {
    if (Player::GetPlayerInstance().GetSingleUnitState(x, y + 1).ship != 0)
      return false;
    return true;
  }
  tmp = y;
  num = 0;
  while (
    (Player::GetPlayerInstance().GetSingleUnitState(x, tmp).ship != 0) &&
    (tmp < 10)) {
    tmp++;
    num++;
  }
  if (num == size) {
    if (Player::GetPlayerInstance().GetSingleUnitState(x + 1, y).ship != 0)
      return false;
    return true;
  }
  return false;
}

/**
 *open to other parts of the program functionality to     
 *change the current state of the program                    
 */
void Player::SetPlayerState(Player & p,
  const Player::CurrentProgramState & st) {
  p.state_ = st;
}

/**
 *The tool made it possible to determine the correctness of clicking the               
 *user’s mouse with respect to the current state of the program and its minor      
 *changes in accordance with the result                                                               
 */
void Player::ProcessClick(const uint_fast16_t & Absciss,
  const uint_fast16_t & Ordinate) {
  if (this -> state_.main_menu || this -> state_.answer_await) return;
  if (this -> state_.ships_placing && (33 < Absciss) && (410 > Absciss) && (32 < Ordinate) && (410 > Ordinate)) this -> first_player_battle_field_.battle_field[(Absciss - 33) / 38 + ((Ordinate - 33) / 38) * 10].ship ? this -> first_player_battle_field_.battle_field[(Absciss - 33) / 38 + ((Ordinate - 33) / 38) * 10].ship = 0 : this -> first_player_battle_field_.battle_field[(Absciss - 33) / 38 + ((Ordinate - 33) / 38) * 10].ship = 1;
  if ((1 == this -> state_.turn_await) && (499 < Absciss) && (874 > Absciss) && (32 < Ordinate) && (410 > Ordinate)) {
    if ((1 == this -> second_player_battle_field_.battle_field[(Absciss - 499) / 38 + ((Ordinate - 33) / 38) * 10].ship) && (0 == this -> second_player_battle_field_.battle_field[(Absciss - 499) / 38 + ((Ordinate - 33) / 38) * 10].hit) && (0 == this -> second_player_battle_field_.battle_field[(Absciss - 499) / 38 + ((Ordinate - 33) / 38) * 10].destroyed)) this -> second_player_battle_field_.battle_field[(Absciss - 499) / 38 + ((Ordinate - 33) / 38) * 10].ship = 0, this -> second_player_battle_field_.battle_field[(Absciss - 499) / 38 + ((Ordinate - 33) / 38) * 10].hit = 1;
    else if ((0 == this -> second_player_battle_field_.battle_field[(Absciss - 499) / 38 + ((Ordinate - 33) / 38) * 10].ship) && (0 == this -> second_player_battle_field_.battle_field[(Absciss - 499) / 38 + ((Ordinate - 33) / 38) * 10].hit) && (0 == this -> second_player_battle_field_.battle_field[(Absciss - 499) / 38 + ((Ordinate - 33) / 38) * 10].destroyed) && (0 == this -> second_player_battle_field_.battle_field[(Absciss - 499) / 38 + ((Ordinate - 33) / 38) * 10].miss)) this -> second_player_battle_field_.battle_field[(Absciss - 499) / 38 + ((Ordinate - 33) / 38) * 10].miss = 1;
    else return;
    if ((1 == this -> state_.pve_game) && (1 == this -> state_.opponent_field_set)) {
      static uint8_t i = rand() % 100;
      ++i %= 100;
      if ((1 == this -> first_player_battle_field_.battle_field[i].ship) && (0 == this -> first_player_battle_field_.battle_field[i].hit) && (0 == this -> first_player_battle_field_.battle_field[i].destroyed)) this -> first_player_battle_field_.battle_field[i].ship = 0, this -> first_player_battle_field_.battle_field[i].hit = 1;
      else if ((0 == this -> first_player_battle_field_.battle_field[i].ship) && (0 == this -> first_player_battle_field_.battle_field[i].hit) && (0 == this -> first_player_battle_field_.battle_field[i].destroyed)) this -> first_player_battle_field_.battle_field[i].miss = 1;
    }
  }
  if ((1 == this -> state_.pve_game) && (0 == this -> state_.opponent_field_set)) {
    srand(static_cast < unsigned > (time(0)));
    char field[FIELD_DIM][FIELD_DIM];
    SetWater(field), SetShips(field), GetAIField(field);
    this -> state_.opponent_field_set = 1, state_.turn_await = 1;
  }
}

/**
 *function initializing the launch of a chain of functions to pseudo random initialization    
 *by ships of the battlefield of artificial intelligence when the pve game mode is selected   
 */
void Player::SetShip(int size, char field[FIELD_DIM][FIELD_DIM]) {
  bool is_horiz = rand() % 2 == 0;
  int row_top = 0, col_left = 0;
  do {
    do row_top = rand() % FIELD_DIM;
    while (!is_horiz && row_top > FIELD_DIM - size);
    do col_left = rand() % FIELD_DIM;
    while (is_horiz && col_left > FIELD_DIM - size);
  } while (!ShipIsGood(size, is_horiz, row_top, col_left, field));
  if (is_horiz)
    for (int j = col_left; j < col_left + size; ++j) field[row_top][j] = SHIP_SYMB;
  else
    for (int i = row_top; i < row_top + size; ++i) field[i][col_left] = SHIP_SYMB;
}

void Player::SetShips(char field[FIELD_DIM][FIELD_DIM]) {
  for (uint_fast8_t i = 0; i < 1; ++i) SetShip(4, field);
  for (uint_fast8_t i = 0; i < 2; ++i) SetShip(3, field);
  for (uint_fast8_t i = 0; i < 3; ++i) SetShip(2, field);
  for (uint_fast8_t i = 0; i < 4; ++i) SetShip(1, field);
}

void Player::GetAIField(char field[FIELD_DIM][FIELD_DIM]) {
  for (int i = 0; i < FIELD_DIM; ++i)
    for (int j = 0; j < FIELD_DIM; ++j) this -> second_player_battle_field_.battle_field[i + 10 * j].ship = field[i][j];
}

void Player::SetWater(char field[FIELD_DIM][FIELD_DIM]) {
  for (int i = 0; i < FIELD_DIM; ++i)
    for (int j = 0; j < FIELD_DIM; ++j) field[i][j] = WATER_SYMB;
}

bool Player::ShipIsGood(int size, bool is_horiz, int row_top, int col_left,
  const char field[][FIELD_DIM]) {
  if (is_horiz) {
    for (int i = std::max(0, row_top - 1); i <= std::min(static_cast < int > (FIELD_DIM - 1), row_top + 1); ++i)
      for (int j = std::max(0, col_left - 1); j <= std::min(static_cast < int > (FIELD_DIM) - 1, col_left + size); ++j)
        if (field[i][j] == SHIP_SYMB) return false;
    return true;
  } else {
    for (int i = std::max(0, row_top - 1); i <= std::min(static_cast < int > (FIELD_DIM) - 1, row_top + size); ++i)
      for (int j = std::max(0, col_left - 1); j <= std::min(static_cast < int > (FIELD_DIM) - 1, col_left + 1); ++j)
        if (field[i][j] == SHIP_SYMB) return false;
    return true;
  }
}

/**
 *A function that iterates through collections containing data on the state of the playing    
 *fields of each of the players and when determining whether a player won by                   
 *destroying all the enemy’s corpses, transmits the corresponding signal to the                  
 *part of the program responsible for the graphical interface                                             
 */
const int_fast8_t Player::IsWin() {
  uint_fast8_t c = 0;
  /**     passage of the collection */
  for (auto & e: this -> first_player_battle_field_.battle_field) {

    /** search for the number of "hits" in the ship of a specific player */
    if (e.hit == 1) c++;
    if (c == 20) {
      Unit h = {
        0,
        0,
        0,
        0,
        0,
        0,
        0
      };

      /**  "cleans" the game field in connection with the victory of one of the players and the subsequent completion of the match */
      for (auto & e: this -> first_player_battle_field_.battle_field) e = h;
      for (auto & e: this -> second_player_battle_field_.battle_field) e = h;
      return 1;
    }
  }
  c = 0;

  /**     passage of the collection */
  for (auto & e: this -> second_player_battle_field_.battle_field) {

    /**    search for the number of "hits" in the ship of a specific player */
    if (e.hit == 1) c++;
    if (c == 20) {
      Unit g = {
        0,
        0,
        0,
        0,
        0,
        0,
        0
      };

      /** "cleans" the game field in connection with the victory of one of the players and the subsequent completion of the match */
      for (auto & e: this -> first_player_battle_field_.battle_field) e = g;
      for (auto & e: this -> second_player_battle_field_.battle_field) e = g;
      return -1;
    }
  }
  return 0;
}

/**
 *Filling first player's battlefield with pseudo random ships placement     
 *(used same as for any ai battlefield algorithm)                                       
 */
void Player::AutoShipsPlace() {
  srand(static_cast < unsigned > (time(0)));
  char field[FIELD_DIM][FIELD_DIM];
  SetWater(field), SetShips(field);
  for (int i = 0; i < FIELD_DIM; ++i)
    for (int j = 0; j < FIELD_DIM; ++j) this -> first_player_battle_field_.battle_field[i + 10 * j].ship = field[i][j];
}
