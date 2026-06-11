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

  // Outpost
  eval_config.outpost_scale = 128;

  // Exchange sacrifice
  eval_config.exchange_sac_scale = 128;

  // Opponent mobility
  eval_config.knight_opp_mobility_scale = 128;
  eval_config.bishop_opp_mobility_scale = 128;
  eval_config.rook_opp_mobility_scale = 128;
  eval_config.queen_opp_mobility_scale = 128;

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
    case PERSONALITY_STEINITZ:
      // Positional pioneer, prophylaxis, slow buildup
      eval_config.material_scale = 120;
      eval_config.king_safety_scale = 200;
      eval_config.connected_pawn_scale = 220;
      eval_config.doubled_pawn_penalty = 220;
      eval_config.backward_pawn_penalty = 220;
      eval_config.isolated_pawn_penalty = 220;
      eval_config.passed_pawn_scale = 180;
      eval_config.pawn_shield_scale = 200;
      eval_config.threat_scale = 40;
      eval_config.threat_king_scale = 40;
      eval_config.initiative_scale = 30;
      eval_config.rook_open_file_scale = 180;
      eval_config.bishop_pair_scale = 160;
      eval_config.tempo = 60;
      eval_config.outpost_scale = 180;
      eval_config.exchange_sac_scale = 60;
      eval_config.knight_opp_mobility_scale = 180;
      eval_config.bishop_opp_mobility_scale = 180;
      eval_config.rook_opp_mobility_scale = 180;
      eval_config.queen_opp_mobility_scale = 180;
      break;


    case PERSONALITY_LASKER:
      // Psychological fighter, tricky, never gives up
      eval_config.material_scale = 100;
      eval_config.king_safety_scale = 180;
      eval_config.threat_scale = 150;
      eval_config.threat_king_scale = 170;
      eval_config.initiative_scale = 160;
      eval_config.pawn_storm_scale = 150;
      eval_config.knight_mobility_scale = 170;
      eval_config.bishop_mobility_scale = 170;
      eval_config.rook_mobility_scale = 160;
      eval_config.queen_mobility_scale = 170;
      eval_config.rook_open_file_scale = 170;
      eval_config.tempo = 150;
      eval_config.contempt = 50;
      eval_config.outpost_scale = 160;
      eval_config.exchange_sac_scale = 150;
      eval_config.knight_opp_mobility_scale = 120;
      eval_config.bishop_opp_mobility_scale = 120;
      eval_config.rook_opp_mobility_scale = 120;
      eval_config.queen_opp_mobility_scale = 120;
      break;


    case PERSONALITY_CAPABLANCA:
      // Endgame virtuoso, clarity, technique, avoids complications
      eval_config.material_scale = 115;
      eval_config.king_safety_scale = 180;
      eval_config.connected_pawn_scale = 180;
      eval_config.doubled_pawn_penalty = 180;
      eval_config.backward_pawn_penalty = 180;
      eval_config.isolated_pawn_penalty = 180;
      eval_config.passed_pawn_scale = 220;
      eval_config.pawn_shield_scale = 180;
      eval_config.threat_scale = 60;
      eval_config.threat_king_scale = 60;
      eval_config.initiative_scale = 80;
      eval_config.rook_open_file_scale = 180;
      eval_config.bishop_pair_scale = 170;
      eval_config.tempo = 90;
      eval_config.outpost_scale = 180;
      eval_config.exchange_sac_scale = 40;
      eval_config.knight_opp_mobility_scale = 160;
      eval_config.bishop_opp_mobility_scale = 160;
      eval_config.rook_opp_mobility_scale = 160;
      eval_config.queen_opp_mobility_scale = 160;
      break;


    case PERSONALITY_ALEKHINE:
      // Fierce attacker, combinatorial genius, sacrifices
      eval_config.material_scale = 76;
      eval_config.king_safety_scale = 320;
      eval_config.threat_scale = 190;
      eval_config.threat_king_scale = 220;
      eval_config.initiative_scale = 210;
      eval_config.pawn_storm_scale = 190;
      eval_config.connected_pawn_scale = 70;
      eval_config.doubled_pawn_penalty = 50;
      eval_config.isolated_pawn_penalty = 50;
      eval_config.knight_mobility_scale = 190;
      eval_config.bishop_mobility_scale = 190;
      eval_config.rook_mobility_scale = 170;
      eval_config.queen_mobility_scale = 190;
      eval_config.rook_open_file_scale = 180;
      eval_config.tempo = 190;
      eval_config.outpost_scale = 180;
      eval_config.exchange_sac_scale = 220;
      eval_config.knight_opp_mobility_scale = 80;
      eval_config.bishop_opp_mobility_scale = 80;
      eval_config.rook_opp_mobility_scale = 80;
      eval_config.queen_opp_mobility_scale = 80;
      break;


    case PERSONALITY_EUWE:
      // Solid, scientific, well-rounded
      eval_config.material_scale = 100;
      eval_config.king_safety_scale = 160;
      eval_config.connected_pawn_scale = 150;
      eval_config.doubled_pawn_penalty = 150;
      eval_config.backward_pawn_penalty = 150;
      eval_config.isolated_pawn_penalty = 150;
      eval_config.passed_pawn_scale = 160;
      eval_config.pawn_shield_scale = 150;
      eval_config.threat_scale = 120;
      eval_config.threat_king_scale = 120;
      eval_config.initiative_scale = 120;
      eval_config.knight_mobility_scale = 140;
      eval_config.bishop_mobility_scale = 140;
      eval_config.rook_mobility_scale = 140;
      eval_config.queen_mobility_scale = 140;
      eval_config.rook_open_file_scale = 150;
      eval_config.bishop_pair_scale = 140;
      eval_config.tempo = 110;
      eval_config.outpost_scale = 150;
      eval_config.exchange_sac_scale = 100;
      eval_config.knight_opp_mobility_scale = 130;
      eval_config.bishop_opp_mobility_scale = 130;
      eval_config.rook_opp_mobility_scale = 130;
      eval_config.queen_opp_mobility_scale = 130;
      break;


    case PERSONALITY_BOTVINNIK:
      // Scientific, deep calculation, endgame technique
      eval_config.material_scale = 100;
      eval_config.king_safety_scale = 180;
      eval_config.connected_pawn_scale = 160;
      eval_config.doubled_pawn_penalty = 160;
      eval_config.backward_pawn_penalty = 160;
      eval_config.isolated_pawn_penalty = 160;
      eval_config.passed_pawn_scale = 200;
      eval_config.pawn_shield_scale = 160;
      eval_config.threat_scale = 130;
      eval_config.threat_king_scale = 140;
      eval_config.initiative_scale = 130;
      eval_config.rook_open_file_scale = 170;
      eval_config.bishop_pair_scale = 150;
      eval_config.tempo = 120;
      eval_config.outpost_scale = 160;
      eval_config.exchange_sac_scale = 100;
      eval_config.knight_opp_mobility_scale = 140;
      eval_config.bishop_opp_mobility_scale = 140;
      eval_config.rook_opp_mobility_scale = 140;
      eval_config.queen_opp_mobility_scale = 140;
      break;


    case PERSONALITY_SMYSLOV:
      // Harmonious play, endgame artist, bishop pair lover
      eval_config.material_scale = 105;
      eval_config.king_safety_scale = 160;
      eval_config.connected_pawn_scale = 160;
      eval_config.doubled_pawn_penalty = 160;
      eval_config.backward_pawn_penalty = 160;
      eval_config.isolated_pawn_penalty = 160;
      eval_config.passed_pawn_scale = 200;
      eval_config.pawn_shield_scale = 160;
      eval_config.threat_scale = 100;
      eval_config.threat_king_scale = 100;
      eval_config.initiative_scale = 100;
      eval_config.rook_open_file_scale = 170;
      eval_config.bishop_pair_scale = 200;
      eval_config.tempo = 100;
      eval_config.outpost_scale = 160;
      eval_config.exchange_sac_scale = 100;
      eval_config.knight_opp_mobility_scale = 140;
      eval_config.bishop_opp_mobility_scale = 140;
      eval_config.rook_opp_mobility_scale = 140;
      eval_config.queen_opp_mobility_scale = 140;
      break;


    case PERSONALITY_TAL:
      // Magician from Riga, extreme sacrifices, king hunt
      eval_config.material_scale = 66;
      eval_config.king_safety_scale = 380;
      eval_config.threat_scale = 220;
      eval_config.threat_king_scale = 260;
      eval_config.initiative_scale = 230;
      eval_config.pawn_storm_scale = 220;
      eval_config.connected_pawn_scale = 50;
      eval_config.doubled_pawn_penalty = 30;
      eval_config.isolated_pawn_penalty = 30;
      eval_config.knight_mobility_scale = 210;
      eval_config.bishop_mobility_scale = 210;
      eval_config.rook_mobility_scale = 190;
      eval_config.queen_mobility_scale = 210;
      eval_config.rook_open_file_scale = 190;
      eval_config.tempo = 210;
      eval_config.outpost_scale = 160;
      eval_config.exchange_sac_scale = 250;
      eval_config.knight_opp_mobility_scale = 60;
      eval_config.bishop_opp_mobility_scale = 60;
      eval_config.rook_opp_mobility_scale = 60;
      eval_config.queen_opp_mobility_scale = 60;
      break;


    case PERSONALITY_PETROSIAN:
      // Iron Tigran, prophylaxis, exchange sacs, hard to beat
      eval_config.material_scale = 120;
      eval_config.king_safety_scale = 250;
      eval_config.connected_pawn_scale = 200;
      eval_config.doubled_pawn_penalty = 200;
      eval_config.backward_pawn_penalty = 200;
      eval_config.isolated_pawn_penalty = 200;
      eval_config.passed_pawn_scale = 180;
      eval_config.pawn_shield_scale = 220;
      eval_config.threat_scale = 30;
      eval_config.threat_king_scale = 30;
      eval_config.initiative_scale = 20;
      eval_config.rook_open_file_scale = 140;
      eval_config.bishop_pair_scale = 160;
      eval_config.tempo = 50;
      eval_config.outpost_scale = 200;
      eval_config.exchange_sac_scale = 200;
      eval_config.knight_opp_mobility_scale = 200;
      eval_config.bishop_opp_mobility_scale = 200;
      eval_config.rook_opp_mobility_scale = 200;
      eval_config.queen_opp_mobility_scale = 200;
      break;


    case PERSONALITY_SPASSKY:
      // Universal, dynamic, powerful attacker when unleashed
      eval_config.material_scale = 105;
      eval_config.king_safety_scale = 220;
      eval_config.threat_scale = 160;
      eval_config.threat_king_scale = 180;
      eval_config.initiative_scale = 170;
      eval_config.pawn_storm_scale = 160;
      eval_config.connected_pawn_scale = 130;
      eval_config.doubled_pawn_penalty = 130;
      eval_config.isolated_pawn_penalty = 130;
      eval_config.knight_mobility_scale = 170;
      eval_config.bishop_mobility_scale = 170;
      eval_config.rook_mobility_scale = 160;
      eval_config.queen_mobility_scale = 170;
      eval_config.rook_open_file_scale = 170;
      eval_config.bishop_pair_scale = 160;
      eval_config.tempo = 160;
      eval_config.outpost_scale = 160;
      eval_config.exchange_sac_scale = 150;
      eval_config.knight_opp_mobility_scale = 110;
      eval_config.bishop_opp_mobility_scale = 110;
      eval_config.rook_opp_mobility_scale = 110;
      eval_config.queen_opp_mobility_scale = 110;
      break;


    case PERSONALITY_FISCHER:
      // Precision, bishops, fighting spirit, never draws
      eval_config.material_scale = 98;
      eval_config.king_safety_scale = 240;
      eval_config.threat_scale = 170;
      eval_config.threat_king_scale = 190;
      eval_config.initiative_scale = 190;
      eval_config.pawn_storm_scale = 160;
      eval_config.connected_pawn_scale = 140;
      eval_config.doubled_pawn_penalty = 140;
      eval_config.isolated_pawn_penalty = 140;
      eval_config.passed_pawn_scale = 180;
      eval_config.knight_mobility_scale = 170;
      eval_config.bishop_mobility_scale = 190;
      eval_config.rook_mobility_scale = 170;
      eval_config.queen_mobility_scale = 180;
      eval_config.rook_open_file_scale = 190;
      eval_config.bishop_pair_scale = 220;
      eval_config.tempo = 170;
      eval_config.contempt = 100;
      eval_config.outpost_scale = 180;
      eval_config.exchange_sac_scale = 140;
      eval_config.knight_opp_mobility_scale = 120;
      eval_config.bishop_opp_mobility_scale = 140;
      eval_config.rook_opp_mobility_scale = 120;
      eval_config.queen_opp_mobility_scale = 120;
      break;


    case PERSONALITY_KARPOV:
      // Boa constrictor, positional squeeze, blockade
      eval_config.material_scale = 110;
      eval_config.king_safety_scale = 200;
      eval_config.connected_pawn_scale = 220;
      eval_config.doubled_pawn_penalty = 220;
      eval_config.backward_pawn_penalty = 220;
      eval_config.isolated_pawn_penalty = 220;
      eval_config.passed_pawn_scale = 200;
      eval_config.pawn_shield_scale = 200;
      eval_config.threat_scale = 50;
      eval_config.threat_king_scale = 50;
      eval_config.initiative_scale = 40;
      eval_config.rook_open_file_scale = 180;
      eval_config.bishop_pair_scale = 180;
      eval_config.tempo = 70;
      eval_config.outpost_scale = 200;
      eval_config.exchange_sac_scale = 50;
      eval_config.knight_opp_mobility_scale = 200;
      eval_config.bishop_opp_mobility_scale = 200;
      eval_config.rook_opp_mobility_scale = 200;
      eval_config.queen_opp_mobility_scale = 200;
      break;


    case PERSONALITY_KASPAROV:
      // Dynamic attacker, deep prep, initiative above all
      eval_config.material_scale = 88;
      eval_config.king_safety_scale = 280;
      eval_config.threat_scale = 180;
      eval_config.threat_king_scale = 200;
      eval_config.initiative_scale = 230;
      eval_config.pawn_storm_scale = 180;
      eval_config.connected_pawn_scale = 80;
      eval_config.doubled_pawn_penalty = 60;
      eval_config.isolated_pawn_penalty = 60;
      eval_config.knight_mobility_scale = 180;
      eval_config.bishop_mobility_scale = 180;
      eval_config.rook_mobility_scale = 170;
      eval_config.queen_mobility_scale = 180;
      eval_config.rook_open_file_scale = 190;
      eval_config.tempo = 190;
      eval_config.outpost_scale = 170;
      eval_config.exchange_sac_scale = 180;
      eval_config.knight_opp_mobility_scale = 80;
      eval_config.bishop_opp_mobility_scale = 80;
      eval_config.rook_opp_mobility_scale = 80;
      eval_config.queen_opp_mobility_scale = 80;
      break;


    case PERSONALITY_KRAMNIK:
      // Berlin wall, solid, endgame technique, hard to beat
      eval_config.material_scale = 110;
      eval_config.king_safety_scale = 180;
      eval_config.connected_pawn_scale = 180;
      eval_config.doubled_pawn_penalty = 180;
      eval_config.backward_pawn_penalty = 180;
      eval_config.isolated_pawn_penalty = 180;
      eval_config.passed_pawn_scale = 200;
      eval_config.pawn_shield_scale = 180;
      eval_config.threat_scale = 80;
      eval_config.threat_king_scale = 80;
      eval_config.initiative_scale = 90;
      eval_config.rook_open_file_scale = 200;
      eval_config.bishop_pair_scale = 160;
      eval_config.tempo = 100;
      eval_config.outpost_scale = 170;
      eval_config.exchange_sac_scale = 180;
      eval_config.knight_opp_mobility_scale = 160;
      eval_config.bishop_opp_mobility_scale = 160;
      eval_config.rook_opp_mobility_scale = 160;
      eval_config.queen_opp_mobility_scale = 160;
      break;


    case PERSONALITY_ANAND:
      // Lightning calculator, universal, tactical, fast
      eval_config.material_scale = 105;
      eval_config.king_safety_scale = 220;
      eval_config.threat_scale = 160;
      eval_config.threat_king_scale = 180;
      eval_config.initiative_scale = 170;
      eval_config.pawn_storm_scale = 160;
      eval_config.knight_mobility_scale = 190;
      eval_config.bishop_mobility_scale = 190;
      eval_config.rook_mobility_scale = 180;
      eval_config.queen_mobility_scale = 190;
      eval_config.rook_open_file_scale = 180;
      eval_config.bishop_pair_scale = 170;
      eval_config.tempo = 170;
      eval_config.outpost_scale = 160;
      eval_config.exchange_sac_scale = 140;
      eval_config.knight_opp_mobility_scale = 110;
      eval_config.bishop_opp_mobility_scale = 110;
      eval_config.rook_opp_mobility_scale = 110;
      eval_config.queen_opp_mobility_scale = 110;
      break;


    case PERSONALITY_CARLSEN:
      // Endgame monster, grinder, patient, universal
      eval_config.material_scale = 105;
      eval_config.king_safety_scale = 160;
      eval_config.connected_pawn_scale = 160;
      eval_config.doubled_pawn_penalty = 160;
      eval_config.backward_pawn_penalty = 160;
      eval_config.isolated_pawn_penalty = 160;
      eval_config.passed_pawn_scale = 230;
      eval_config.pawn_shield_scale = 160;
      eval_config.threat_scale = 90;
      eval_config.threat_king_scale = 90;
      eval_config.initiative_scale = 120;
      eval_config.rook_open_file_scale = 190;
      eval_config.bishop_pair_scale = 170;
      eval_config.tempo = 110;
      eval_config.outpost_scale = 160;
      eval_config.exchange_sac_scale = 100;
      eval_config.knight_opp_mobility_scale = 140;
      eval_config.bishop_opp_mobility_scale = 140;
      eval_config.rook_opp_mobility_scale = 140;
      eval_config.queen_opp_mobility_scale = 140;
      break;


    case PERSONALITY_THEBUTCHER:
      // Extreme materialist, hoards every pawn, never sacs
      eval_config.material_scale = 200;
      eval_config.king_safety_scale = 60;
      eval_config.connected_pawn_scale = 250;
      eval_config.doubled_pawn_penalty = 250;
      eval_config.backward_pawn_penalty = 250;
      eval_config.isolated_pawn_penalty = 250;
      eval_config.passed_pawn_scale = 250;
      eval_config.pawn_shield_scale = 50;
      eval_config.threat_scale = 20;
      eval_config.threat_king_scale = 20;
      eval_config.initiative_scale = 10;
      eval_config.pawn_storm_scale = 20;
      eval_config.knight_mobility_scale = 60;
      eval_config.bishop_mobility_scale = 60;
      eval_config.rook_mobility_scale = 60;
      eval_config.queen_mobility_scale = 60;
      eval_config.rook_open_file_scale = 60;
      eval_config.bishop_pair_scale = 60;
      eval_config.behind_pawn_bonus = 200;
      eval_config.tempo = 30;
      eval_config.outpost_scale = 100;
      eval_config.exchange_sac_scale = 20;
      eval_config.knight_opp_mobility_scale = 180;
      eval_config.bishop_opp_mobility_scale = 180;
      eval_config.rook_opp_mobility_scale = 180;
      eval_config.queen_opp_mobility_scale = 180;
      break;


    case PERSONALITY_THEGAMBLER:
      // Sacs everything, all-out attack, 19th century on steroids
      eval_config.material_scale = 30;
      eval_config.king_safety_scale = 500;
      eval_config.threat_scale = 300;
      eval_config.threat_king_scale = 350;
      eval_config.initiative_scale = 300;
      eval_config.pawn_storm_scale = 300;
      eval_config.connected_pawn_scale = 30;
      eval_config.doubled_pawn_penalty = 10;
      eval_config.isolated_pawn_penalty = 10;
      eval_config.knight_mobility_scale = 250;
      eval_config.bishop_mobility_scale = 250;
      eval_config.rook_mobility_scale = 220;
      eval_config.queen_mobility_scale = 250;
      eval_config.rook_open_file_scale = 220;
      eval_config.tempo = 250;
      eval_config.outpost_scale = 180;
      eval_config.exchange_sac_scale = 300;
      eval_config.knight_opp_mobility_scale = 40;
      eval_config.bishop_opp_mobility_scale = 40;
      eval_config.rook_opp_mobility_scale = 40;
      eval_config.queen_opp_mobility_scale = 40;
      break;


    case PERSONALITY_THEWALL:
      // Pure defense, never attacks, draws everything
      eval_config.material_scale = 140;
      eval_config.king_safety_scale = 300;
      eval_config.connected_pawn_scale = 250;
      eval_config.doubled_pawn_penalty = 250;
      eval_config.backward_pawn_penalty = 250;
      eval_config.isolated_pawn_penalty = 250;
      eval_config.passed_pawn_scale = 200;
      eval_config.pawn_shield_scale = 250;
      eval_config.threat_scale = 10;
      eval_config.threat_king_scale = 10;
      eval_config.initiative_scale = 5;
      eval_config.pawn_storm_scale = 10;
      eval_config.knight_mobility_scale = 60;
      eval_config.bishop_mobility_scale = 60;
      eval_config.rook_mobility_scale = 60;
      eval_config.queen_mobility_scale = 60;
      eval_config.rook_open_file_scale = 100;
      eval_config.bishop_pair_scale = 100;
      eval_config.tempo = 20;
      eval_config.contempt = -500;
      eval_config.outpost_scale = 140;
      eval_config.exchange_sac_scale = 30;
      eval_config.knight_opp_mobility_scale = 200;
      eval_config.bishop_opp_mobility_scale = 200;
      eval_config.rook_opp_mobility_scale = 200;
      eval_config.queen_opp_mobility_scale = 200;
      break;


    case PERSONALITY_THEBERSERKER:
      // King walks, all-out attack, no defense, pure violence
      eval_config.material_scale = 42;
      eval_config.king_safety_scale = 450;
      eval_config.threat_scale = 280;
      eval_config.threat_king_scale = 320;
      eval_config.initiative_scale = 280;
      eval_config.pawn_storm_scale = 280;
      eval_config.connected_pawn_scale = 40;
      eval_config.doubled_pawn_penalty = 20;
      eval_config.isolated_pawn_penalty = 20;
      eval_config.knight_mobility_scale = 240;
      eval_config.bishop_mobility_scale = 240;
      eval_config.rook_mobility_scale = 220;
      eval_config.queen_mobility_scale = 240;
      eval_config.rook_open_file_scale = 220;
      eval_config.tempo = 240;
      eval_config.outpost_scale = 180;
      eval_config.exchange_sac_scale = 280;
      eval_config.knight_opp_mobility_scale = 40;
      eval_config.bishop_opp_mobility_scale = 40;
      eval_config.rook_opp_mobility_scale = 40;
      eval_config.queen_opp_mobility_scale = 40;
      break;


    case PERSONALITY_THESWINDLER:
      // Tricky, traps, compensation, never resigns
      eval_config.material_scale = 100;
      eval_config.king_safety_scale = 200;
      eval_config.threat_scale = 200;
      eval_config.threat_king_scale = 200;
      eval_config.initiative_scale = 200;
      eval_config.pawn_storm_scale = 180;
      eval_config.connected_pawn_scale = 100;
      eval_config.doubled_pawn_penalty = 100;
      eval_config.isolated_pawn_penalty = 100;
      eval_config.passed_pawn_scale = 180;
      eval_config.knight_mobility_scale = 190;
      eval_config.bishop_mobility_scale = 190;
      eval_config.rook_mobility_scale = 180;
      eval_config.queen_mobility_scale = 190;
      eval_config.rook_open_file_scale = 180;
      eval_config.bishop_pair_scale = 180;
      eval_config.tempo = 200;
      eval_config.contempt = 200;
      eval_config.outpost_scale = 160;
      eval_config.exchange_sac_scale = 200;
      eval_config.knight_opp_mobility_scale = 100;
      eval_config.bishop_opp_mobility_scale = 100;
      eval_config.rook_opp_mobility_scale = 100;
      eval_config.queen_opp_mobility_scale = 100;
      break;


    case PERSONALITY_THEPACIFIST:
      // Avoids all conflict, draws at any cost, extreme safety
      eval_config.material_scale = 150;
      eval_config.king_safety_scale = 300;
      eval_config.connected_pawn_scale = 250;
      eval_config.doubled_pawn_penalty = 250;
      eval_config.backward_pawn_penalty = 250;
      eval_config.isolated_pawn_penalty = 250;
      eval_config.passed_pawn_scale = 200;
      eval_config.pawn_shield_scale = 250;
      eval_config.threat_scale = 5;
      eval_config.threat_king_scale = 5;
      eval_config.initiative_scale = 5;
      eval_config.pawn_storm_scale = 5;
      eval_config.knight_mobility_scale = 50;
      eval_config.bishop_mobility_scale = 50;
      eval_config.rook_mobility_scale = 50;
      eval_config.queen_mobility_scale = 50;
      eval_config.rook_open_file_scale = 80;
      eval_config.bishop_pair_scale = 80;
      eval_config.tempo = 10;
      eval_config.contempt = -800;
      eval_config.outpost_scale = 120;
      eval_config.exchange_sac_scale = 20;
      eval_config.knight_opp_mobility_scale = 200;
      eval_config.bishop_opp_mobility_scale = 200;
      eval_config.rook_opp_mobility_scale = 200;
      eval_config.queen_opp_mobility_scale = 200;
      break;


    case PERSONALITY_THEYOUTH:
      // Computer-like, no fear, extreme mobility, fast
      eval_config.material_scale = 93;
      eval_config.king_safety_scale = 260;
      eval_config.threat_scale = 200;
      eval_config.threat_king_scale = 220;
      eval_config.initiative_scale = 220;
      eval_config.pawn_storm_scale = 200;
      eval_config.connected_pawn_scale = 100;
      eval_config.doubled_pawn_penalty = 80;
      eval_config.isolated_pawn_penalty = 80;
      eval_config.passed_pawn_scale = 200;
      eval_config.knight_mobility_scale = 230;
      eval_config.bishop_mobility_scale = 230;
      eval_config.rook_mobility_scale = 210;
      eval_config.queen_mobility_scale = 230;
      eval_config.rook_open_file_scale = 210;
      eval_config.bishop_pair_scale = 180;
      eval_config.tempo = 220;
      eval_config.outpost_scale = 160;
      eval_config.exchange_sac_scale = 160;
      eval_config.knight_opp_mobility_scale = 80;
      eval_config.bishop_opp_mobility_scale = 80;
      eval_config.rook_opp_mobility_scale = 80;
      eval_config.queen_opp_mobility_scale = 80;
      break;


    case PERSONALITY_TOPALOV:
      // Aggressive attacker, exchange sac specialist, dynamic
      eval_config.material_scale = 90;
      eval_config.king_safety_scale = 250;
      eval_config.threat_scale = 180;
      eval_config.threat_king_scale = 200;
      eval_config.initiative_scale = 200;
      eval_config.pawn_storm_scale = 180;
      eval_config.connected_pawn_scale = 120;
      eval_config.doubled_pawn_penalty = 120;
      eval_config.isolated_pawn_penalty = 120;
      eval_config.passed_pawn_scale = 180;
      eval_config.pawn_shield_scale = 180;
      eval_config.knight_mobility_scale = 190;
      eval_config.bishop_mobility_scale = 190;
      eval_config.rook_mobility_scale = 180;
      eval_config.queen_mobility_scale = 190;
      eval_config.rook_open_file_scale = 190;
      eval_config.bishop_pair_scale = 160;
      eval_config.tempo = 180;
      eval_config.outpost_scale = 180;
      eval_config.exchange_sac_scale = 200;
      eval_config.knight_opp_mobility_scale = 90;
      eval_config.bishop_opp_mobility_scale = 90;
      eval_config.rook_opp_mobility_scale = 90;
      eval_config.queen_opp_mobility_scale = 90;
      break;

    case PERSONALITY_THEVETERAN:
      // Slow, safe, solid, experience over calculation
      eval_config.material_scale = 115;
      eval_config.king_safety_scale = 200;
      eval_config.connected_pawn_scale = 200;
      eval_config.doubled_pawn_penalty = 200;
      eval_config.backward_pawn_penalty = 200;
      eval_config.isolated_pawn_penalty = 200;
      eval_config.passed_pawn_scale = 200;
      eval_config.pawn_shield_scale = 200;
      eval_config.threat_scale = 60;
      eval_config.threat_king_scale = 60;
      eval_config.initiative_scale = 60;
      eval_config.pawn_storm_scale = 60;
      eval_config.knight_mobility_scale = 100;
      eval_config.bishop_mobility_scale = 100;
      eval_config.rook_mobility_scale = 100;
      eval_config.queen_mobility_scale = 100;
      eval_config.rook_open_file_scale = 160;
      eval_config.bishop_pair_scale = 160;
      eval_config.tempo = 80;
      break;
      eval_config.outpost_scale = 150;
      eval_config.exchange_sac_scale = 60;
      eval_config.knight_opp_mobility_scale = 150;
      eval_config.bishop_opp_mobility_scale = 150;
      eval_config.rook_opp_mobility_scale = 150;
      eval_config.queen_opp_mobility_scale = 150;


    default:
      break;
  }
}