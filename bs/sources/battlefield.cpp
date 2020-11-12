#include "headers/includes.h"

#include "headers/player.h"

/**
 *Change the state of a certain unit of the battlefield  
 */
void Player::BattleField::SeUnitState(
  /** parameters:
   * reference to a field object, whose unit state will be changed,
   */
  std::array < Unit, 100 > & BattleField,

  /**
   *constant references to the coordinates of the cell whose state must be changed
   */
  const uint_fast16_t & Absciss,
    const uint_fast16_t & Ordinate,

      /**
       * constant reference to a "Unit" object that conditionally displays the state to which a certain cell must be transferred
       */
      const Unit & UnitState
) {
  BattleField[((Ordinate * 10) + Absciss) - 1] = UnitState;
}



