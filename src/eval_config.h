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

#ifndef EVAL_CONFIG_H
#define EVAL_CONFIG_H

// Personality presets
enum {
  PERSONALITY_NONE,
  PERSONALITY_AGGRESSIVE,   // Tal-like: sacs, attacks, king safety priority
  PERSONALITY_POSITIONAL,   // Karpov-like: blockade, pawn structure, space
  PERSONALITY_SOLID,        // Carlsen-like: endgame, universal, patient
  PERSONALITY_ATTACKER,     // Kasparov-like: initiative, dynamic play
  PERSONALITY_DEFENSIVE,    // Petrosian-like: prophylaxis, exchange sacs
  PERSONALITY_ROMANTIC,     // Wild attacker: extreme sacrifices
  PERSONALITY_GRINDER,      // Korchnoi-like: never gives up, endgame
  N_PERSONALITIES
};

// Eval config struct - all scale factors are per128 (128 = 1.0x)
typedef struct {
  // Personality
  int personality;

  // Material (per128)
  int material_scale;        // Overall material multiplier
  int pawn_value;            // Pawn base value
  int knight_value;          // Knight base value
  int bishop_value;          // Bishop base value
  int rook_value;            // Rook base value
  int queen_value;           // Queen base value

  // PST (per128)
  int pst_scale;             // Piece-square table scale

  // Pawn structure (per128)
  int connected_pawn_scale;
  int doubled_pawn_penalty;
  int backward_pawn_penalty;
  int isolated_pawn_penalty;
  int passed_pawn_scale;
  int pawn_shield_scale;
  int pawn_storm_scale;
  int pawn_mobility_scale;

  // Mobility (per128)
  int knight_mobility_scale;
  int bishop_mobility_scale;
  int rook_mobility_scale;
  int queen_mobility_scale;

  // King safety (per128)
  int king_safety_scale;

  // Threats (per128)
  int threat_scale;
  int threat_king_scale;
  int threat_protected_pawn_scale;
  int threats_on_queen_scale;

  // Rook (per128)
  int rook_open_file_scale;

  // Other (per128)
  int bishop_pair_scale;
  int behind_pawn_bonus;
  int initiative_scale;
  int tempo;
  int contempt;
} eval_config_t;

extern eval_config_t eval_config;

void init_eval_config();
void apply_personality(int personality);

#endif