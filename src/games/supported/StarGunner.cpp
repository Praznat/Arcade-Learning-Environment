/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2012 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "StarGunner.hpp"

#include "../RomUtils.hpp"


StarGunnerSettings::StarGunnerSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* StarGunnerSettings::clone() const { 
    
    RomSettings* rval = new StarGunnerSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void StarGunnerSettings::step(const System& system) {

    // update the reward
    int lower_digit = readRam(&system, 0x83) & 0x0F;
    if (lower_digit == 10) lower_digit = 0;
    int middle_digit = readRam(&system, 0x84) & 0x0F;
    if (middle_digit == 10) middle_digit = 0;
    int higher_digit = readRam(&system, 0x85) & 0x0F;
    if (higher_digit == 10) higher_digit = 0;
    int digit_4 = readRam(&system, 0x86) & 0x0F;
    if (digit_4 == 10) digit_4 = 0;
    int score = lower_digit + 10 * middle_digit + 100 * higher_digit + 1000 * digit_4;
    score *= 100;
    int reward = score - m_score;
    m_reward = reward;
    m_score = score;

    // update terminal status
    int lives = readRam(&system, 0x87); 
    m_terminal = lives == 0; 
}


/* is end of game */
bool StarGunnerSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t StarGunnerSettings::getReward() const { 

    return m_reward; 
}


/* is an action part of the minimal set? */
bool StarGunnerSettings::isMinimal(const Action &a) const {

    switch (a) {
        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_UP:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        case PLAYER_A_DOWN:
        case PLAYER_A_UPRIGHT:
        case PLAYER_A_UPLEFT:
        case PLAYER_A_DOWNRIGHT:
        case PLAYER_A_DOWNLEFT:
        case PLAYER_A_UPFIRE:
        case PLAYER_A_RIGHTFIRE:
        case PLAYER_A_LEFTFIRE:
        case PLAYER_A_DOWNFIRE:
        case PLAYER_A_UPRIGHTFIRE:
        case PLAYER_A_UPLEFTFIRE:
        case PLAYER_A_DOWNRIGHTFIRE:
        case PLAYER_A_DOWNLEFTFIRE:
            return true;
        default:
            return false;
    }   
}


/* reset the state of the game */
void StarGunnerSettings::reset() {
    
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
}
        
/* saves the state of the rom settings */
void StarGunnerSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void StarGunnerSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
}

