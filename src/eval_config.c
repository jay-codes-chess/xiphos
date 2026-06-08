/*
  Xiphos, a UCI chess engine
  Copyright (C) 2018, 2019 Milos Tatarevic

  Xiphos is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Xiphos is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "eval_config.h"

eval_config_t eval_config;

void init_eval_config()
{
  eval_config.personality = PERSONALITY_NONE;

  // Material - default piece values (per128)
  eval_config.material_scale = 128;
  eval_config.pawn_value = 128;
  eval_config.knight_value = 128;
  eval_config.bishop_value = 128;
  eval_config.rook_value = 128;
  eval_config.queen_value = 128;

  // PST
  eval_config.pst_scale = 128;

  // Pawn structure
  eval_config.connected_pawn_scale = 128;
  eval_config.doubled_pawn_penalty = 128;
  eval_config.backward_pawn_penalty = 128;
  eval_config.isolated_pawn_penalty = 128;
  eval_config.passed_pawn_scale = 128;
  eval_config.pawn_shield_scale = 128;
  eval_config.pawn_storm_scale = 128;
  eval_config.pawn_mobility_scale = 128;

  // Mobility
  eval_config.knight_mobility_scale = 128;
  eval_config.bishop_mobility_scale = 128;
  eval_config.rook_mobility_scale = 128;
  eval_config.queen_mobility_scale = 128;

  // King safety
  eval_config.king_safety_scale = 128;

  // Threats
  eval_config.threat_scale = 128;
  eval_config.threat_king_scale = 128;
  eval_config.threat_protected_pawn_scale = 128;
  eval_config.threats_on_queen_scale = 128;

  // Rook
  eval_config.rook_open_file_scale = 128;

  // Other
  eval_config.bishop_pair_scale = 128;
  eval_config.behind_pawn_bonus = 128;
  eval_config.initiative_scale = 128;
  eval_config.tempo = 128;
  eval_config.contempt = 0;  // centipawns, not per128
}

void apply_personality(int personality)
{
  // Reset to defaults first
  init_eval_config();
  eval_config.personality = personality;

  switch (personality)
  {
    case PERSONALITY_AGGRESSIVE:
      // Tal-like: sacrifices material for attack, king safety priority
      eval_config.material_scale = 82;     // 0.64x - happy to sac
      eval_config.king_safety_scale = 256;  // 2.0x - king attack priority
      eval_config.threat_scale = 180;       // 1.4x
      eval_config.threat_king_scale = 200;  // 1.56x
      eval_config.initiative_scale = 180;   // 1.4x
      eval_config.pawn_storm_scale = 180;   // 1.4x
      eval_config.connected_pawn_scale = 80; // 0.625x - less careful with pawns
      eval_config.doubled_pawn_penalty = 60; // 0.47x - doesn't care about doubled pawns
      eval_config.isolated_pawn_penalty = 60;
      eval_config.knight_mobility_scale = 180;
      eval_config.bishop_mobility_scale = 180;
      eval_config.rook_mobility_scale = 160;
      eval_config.queen_mobility_scale = 180;
      eval_config.rook_open_file_scale = 160;
      eval_config.tempo = 180;              // 1.4x - values the initiative
      break;

    case PERSONALITY_POSITIONAL:
      // Karpov-like: blockade, pawn structure, space, avoids risk
      eval_config.material_scale = 110;     // 0.86x - values material slightly
      eval_config.king_safety_scale = 180;  // 1.4x - protects own king
      eval_config.connected_pawn_scale = 200;  // 1.56x
      eval_config.doubled_pawn_penalty = 200;  // 1.56x - hates doubled pawns
      eval_config.backward_pawn_penalty = 200;
      eval_config.isolated_pawn_penalty = 200;
      eval_config.passed_pawn_scale = 180;
      eval_config.pawn_shield_scale = 180;
      eval_config.threat_scale = 60;        // 0.47x - doesn't overvalue threats
      eval_config.threat_king_scale = 60;
      eval_config.initiative_scale = 60;    // 0.47x - patient, doesn't press
      eval_config.rook_open_file_scale = 180;
      eval_config.bishop_pair_scale = 180;
      eval_config.tempo = 80;               // 0.625x - doesn't rush
      break;

    case PERSONALITY_SOLID:
      // Carlsen-like: universal, endgame, patient, hard to beat
      eval_config.material_scale = 105;     // slightly values material
      eval_config.king_safety_scale = 160;
      eval_config.connected_pawn_scale = 150;
      eval_config.doubled_pawn_penalty = 150;
      eval_config.isolated_pawn_penalty = 150;
      eval_config.passed_pawn_scale = 200;  // endgame monster
      eval_config.initiative_scale = 120;
      eval_config.threat_scale = 80;
      eval_config.rook_open_file_scale = 180;
      eval_config.bishop_pair_scale = 160;
      eval_config.tempo = 100;
      break;

    case PERSONALITY_ATTACKER:
      // Kasparov-like: dynamic, initiative, deep preparation
      eval_config.material_scale = 88;      // 0.69x - willing to sac for initiative
      eval_config.king_safety_scale = 220;  // 1.72x
      eval_config.threat_scale = 160;
      eval_config.threat_king_scale = 180;
      eval_config.initiative_scale = 200;   // 1.56x - initiative is everything
      eval_config.pawn_storm_scale = 160;
      eval_config.knight_mobility_scale = 160;
      eval_config.bishop_mobility_scale = 160;
      eval_config.rook_mobility_scale = 150;
      eval_config.queen_mobility_scale = 160;
      eval_config.rook_open_file_scale = 180;
      eval_config.tempo = 160;
      break;

    case PERSONALITY_DEFENSIVE:
      // Petrosian-like: prophylaxis, exchange sacs, hard to beat
      eval_config.material_scale = 115;     // values material
      eval_config.king_safety_scale = 220;  // very protective of own king
      eval_config.connected_pawn_scale = 180;
      eval_config.doubled_pawn_penalty = 180;
      eval_config.isolated_pawn_penalty = 180;
      eval_config.pawn_shield_scale = 200;
      eval_config.threat_scale = 50;        // doesn't overvalue own threats
      eval_config.threat_king_scale = 50;
      eval_config.initiative_scale = 40;    // patient, waits for opponent mistakes
      eval_config.rook_open_file_scale = 140;
      eval_config.tempo = 70;
      break;

    case PERSONALITY_ROMANTIC:
      // Wild attacker: extreme sacrifices, 19th century style
      eval_config.material_scale = 50;      // 0.39x - throws material around
      eval_config.king_safety_scale = 400;  // 3.125x - all about the king hunt
      eval_config.threat_scale = 250;
      eval_config.threat_king_scale = 300;
      eval_config.initiative_scale = 250;
      eval_config.pawn_storm_scale = 250;
      eval_config.connected_pawn_scale = 50;
      eval_config.doubled_pawn_penalty = 30;
      eval_config.isolated_pawn_penalty = 30;
      eval_config.knight_mobility_scale = 220;
      eval_config.bishop_mobility_scale = 220;
      eval_config.rook_mobility_scale = 200;
      eval_config.queen_mobility_scale = 220;
      eval_config.rook_open_file_scale = 200;
      eval_config.tempo = 220;
      break;

    case PERSONALITY_GRINDER:
      // Korchnoi-like: never gives up, endgame specialist, fighting
      eval_config.material_scale = 110;
      eval_config.king_safety_scale = 140;
      eval_config.connected_pawn_scale = 160;
      eval_config.doubled_pawn_penalty = 160;
      eval_config.isolated_pawn_penalty = 160;
      eval_config.passed_pawn_scale = 220;  // endgame monster
      eval_config.initiative_scale = 140;
      eval_config.rook_open_file_scale = 200;
      eval_config.bishop_pair_scale = 160;
      eval_config.tempo = 120;
      break;

    default:
      break;
  }
}