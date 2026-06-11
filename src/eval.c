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

#include "bitboard.h"
#include "game.h"
#include "eval_config.h"
#include "pawn_eval.h"
#include "phash.h"
#include "position.h"
#include "tables.h"

#define SAFE_CHECK_BONUS          3
#define PUSHED_PASSERS_BONUS      9
#define BEHIND_PAWN_BONUS         9
#define PAWN_ATTACK_BONUS         2
#define K_SQ_ATTACK               2
#define K_CNT_LIMIT               8

#define OUTPOST_BONUS             16
#define OUTPOST_BONUS_END         12
#define PHASE_SHIFT               7
#define TOTAL_PHASE               (1 << PHASE_SHIFT)
#define TEMPO                     10

const int k_cnt_mul[K_CNT_LIMIT] = { 0, 3, 7, 12, 16, 18, 19, 20 };

static inline int _mobility_scale(int piece, int for_side) {
  if (for_side == WHITE)
    switch (piece) {
      case KNIGHT: return eval_config.knight_mobility_scale;
      case BISHOP: return eval_config.bishop_mobility_scale;
      case ROOK: return eval_config.rook_mobility_scale;
      case QUEEN: return eval_config.queen_mobility_scale;
      default: return 128;
    }
  else
    switch (piece) {
      case KNIGHT: return eval_config.knight_opp_mobility_scale;
      case BISHOP: return eval_config.bishop_opp_mobility_scale;
      case ROOK: return eval_config.rook_opp_mobility_scale;
      case QUEEN: return eval_config.queen_opp_mobility_scale;
      default: return 128;
    }
}

int eval(position_t *pos)
{
  int side, score, score_mid, score_end, pcnt, sq, k_sq_f, k_sq_o,
      piece_o, open_file, initiative_bonus, k_score[N_SIDES], k_cnt[N_SIDES],
      outpost_cnt[N_SIDES];
  uint64_t b, b0, b1, k_zone, occ, occ_f, occ_o, occ_o_np, occ_o_nk, occ_x,
           p_occ, p_occ_f, p_occ_o, n_att, b_att, r_att, pushed_passers, safe_area,
           p_safe_att, p_pushed[N_SIDES], mob_area[N_SIDES], att_area[N_SIDES],
           d_att_area[N_SIDES], checks[N_SIDES], piece_att[N_SIDES][N_PIECES];
  phash_data_t phash_data;

  phash_data = pawn_eval(pos);
  score_mid = (pos->score_mid + phash_data.score_mid) * eval_config.material_scale / 128;
  score_end = (pos->score_end + phash_data.score_end) * eval_config.material_scale / 128;

  p_occ = pos->piece_occ[PAWN];
  occ = _occ(pos);
  pushed_passers = phash_data.pushed_passers;

  for (side = WHITE; side < N_SIDES; side ++)
  {
    p_occ_f = p_occ & pos->occ[side];
    p_pushed[side] = pushed_pawns(p_occ_f, ~occ, side);
    piece_att[side][PAWN] = pawn_attacks(p_occ_f, side);
  }

  for (side = WHITE; side < N_SIDES; side ++)
  {
    k_sq_f = pos->k_sq[side];
    k_sq_o = pos->k_sq[side ^ 1];
    k_zone = _b_king_zone[k_sq_o];

    occ_f = pos->occ[side];
    occ_o = pos->occ[side ^ 1];
    p_occ_f = p_occ & occ_f;
    p_occ_o = p_occ & occ_o;
    occ_o_nk = occ_o & ~_b(k_sq_o);
    occ_x = occ ^ pos->piece_occ[QUEEN];

    n_att = knight_attack(occ, k_sq_o);
    b_att = bishop_attack(occ_x, k_sq_o);
    r_att = rook_attack(occ_x, k_sq_o);

    mob_area[side] = ~(p_occ_f | piece_att[side ^ 1][PAWN] | _b(k_sq_f));

    piece_att[side][KING] = _b_piece_area[KING][pos->k_sq[side]];
    att_area[side] = piece_att[side][PAWN] | piece_att[side][KING];
    d_att_area[side] = piece_att[side][PAWN] & piece_att[side][KING];

    checks[side] = 0;
    k_score[side] = k_cnt[side] = outpost_cnt[side] = 0;

    #define _score_rook_open_files                                             \
      b1 = _b_file[_file(sq)];                                                 \
      if (!(b1 & p_occ_f))                                                     \
      {                                                                        \
        open_file = !(b1 & p_occ_o);                                           \
        score_mid += rook_file_bonus[PHASE_MID][open_file] * eval_config.rook_open_file_scale / 128;                    \
        score_end += rook_file_bonus[PHASE_END][open_file] * eval_config.rook_open_file_scale / 128;                    \
      }

    #define _score_threats(piece)                                              \
      b1 = b & occ_o_nk;                                                       \
      _loop(b1)                                                                \
      {                                                                        \
        piece_o = _to_white(pos->board[_bsf(b1)]);                             \
        score_mid += threats[PHASE_MID][piece][piece_o] * eval_config.threat_scale / 128;                       \
        score_end += threats[PHASE_END][piece][piece_o] * eval_config.threat_scale / 128;                       \
      }
    #define _nop(...)

    #define _score_piece(piece, method, att, _threats, _rook_bonus)            \
      piece_att[side][piece] = 0;                                              \
      b0 = pos->piece_occ[piece] & occ_f;                                      \
      _loop(b0)                                                                \
      {                                                                        \
        sq = _bsf(b0);                                                         \
        b = method(occ_x, sq);                                                 \
        piece_att[side][piece] |= b;                                           \
        d_att_area[side] |= att_area[side] & b;                                \
        att_area[side] |= b;                                                   \
        b &= mob_area[side];                                                   \
                                                                               \
        _threats(piece);                                                       \
                                                                               \
        /* mobility */                                                         \
        pcnt = _popcnt(b);                                                     \
        score_mid += mobility[PHASE_MID][piece][pcnt] * _mobility_scale(piece, side) / 128;                         \
        score_end += mobility[PHASE_END][piece][pcnt] * _mobility_scale(piece, side) / 128;                         \
                                                                               \
        /* king safety */                                                      \
        b &= k_zone | att;                                                     \
        if (b)                                                                 \
        {                                                                      \
          k_cnt[side] ++;                                                      \
          k_score[side] += _popcnt(b & k_zone);                                \
                                                                               \
          checks[side] |= b &= att;                                            \
          k_score[side] += _popcnt(b);                                         \
        }                                                                      \
                                                                               \
        _rook_bonus                                                            \
      }

    _score_piece(KNIGHT, knight_attack, n_att, _score_threats, );
    _score_piece(BISHOP, bishop_attack, b_att, _score_threats, );

    b_att |= r_att;
    _score_piece(QUEEN, queen_attack, b_att, _nop, );

    occ_x ^= pos->piece_occ[ROOK] & occ_f & ~(side == WHITE ? _B_RANK_1 : _B_RANK_8);
    _score_piece(ROOK, rook_attack, r_att, _score_threats, _score_rook_open_files);

    // passer protection/attacks
    score_end += _popcnt(att_area[side] & pushed_passers) * PUSHED_PASSERS_BONUS * eval_config.passed_pawn_scale / 128;

    // threat by king
    if(piece_att[side][KING] & mob_area[side] & occ_o)
    {
      score_mid += threat_king[PHASE_MID] * eval_config.threat_king_scale / 128;
      score_end += threat_king[PHASE_END] * eval_config.threat_king_scale / 128;
    }

    // N/B behind pawns
    b = (side == WHITE ? p_occ << 8 : p_occ >> 8) & occ_f &
        (pos->piece_occ[KNIGHT] | pos->piece_occ[BISHOP]);
    score_mid += _popcnt(b) * BEHIND_PAWN_BONUS * eval_config.behind_pawn_bonus / 128;

    // knight/bishop outposts
    {
      uint64_t enemy_half = side == WHITE
        ? (_B_RANK_8|_B_RANK_7|(_B_RANK_7<<8)|_B_RANK_5)
        : (_B_RANK_4|(_B_RANK_4<<8)|_B_RANK_2|_B_RANK_1);
      uint64_t outpost_sq = pawn_attacks(p_occ_f, side) & enemy_half
                          & ~pawn_attacks(p_occ_o, side ^ 1);
      uint64_t outpost_pieces = (pos->piece_occ[KNIGHT] | pos->piece_occ[BISHOP])
                              & occ_f & outpost_sq;
      outpost_cnt[side] = _popcnt(outpost_pieces);
      score_mid += outpost_cnt[side] * OUTPOST_BONUS * eval_config.outpost_scale / 128;
      score_end += outpost_cnt[side] * OUTPOST_BONUS_END * eval_config.outpost_scale / 128;
    }

    // bishop pair bonus
    if (_popcnt(pos->piece_occ[BISHOP] & occ_f) >= 2)
    {
      score_mid += bishop_pair[PHASE_MID] * eval_config.bishop_pair_scale / 128;
      score_end += bishop_pair[PHASE_END] * eval_config.bishop_pair_scale / 128;
    }

    score_mid = -score_mid;
    score_end = -score_end;
  }

  // use precalculated attacks (a separate loop is required)
  for (side = WHITE; side < N_SIDES; side ++)
  {
    occ_f = pos->occ[side];
    occ_o = pos->occ[side ^ 1];
    p_occ_f = p_occ & occ_f;
    p_occ_o = p_occ & occ_o;
    occ_o_np = occ_o & ~p_occ_o;
    safe_area = att_area[side] | ~att_area[side ^ 1];

    // pawn mobility
    pcnt = _popcnt(p_pushed[side] & safe_area);
    score_mid += pcnt * pawn_mobility[PHASE_MID] * eval_config.pawn_mobility_scale / 128;
    score_end += pcnt * pawn_mobility[PHASE_END] * eval_config.pawn_mobility_scale / 128;

    // pawn attacks on the king zone
    p_safe_att = pawn_attacks(p_occ_f & safe_area, side);
    k_score[side] +=
      _popcnt(p_safe_att & _b_king_zone[pos->k_sq[side ^ 1]]) * PAWN_ATTACK_BONUS;

    // threats by protected pawns
    pcnt = _popcnt(p_safe_att & occ_o_np);
    score_mid += pcnt * threat_protected_pawn[PHASE_MID] * eval_config.threat_protected_pawn_scale / 128;
    score_end += pcnt * threat_protected_pawn[PHASE_END] * eval_config.threat_protected_pawn_scale / 128;

    // threats by protected pawns (after push)
    pcnt = _popcnt(pawn_attacks(p_pushed[side] & safe_area, side) & occ_o_np);
    score_mid += pcnt * threat_protected_pawn_push[PHASE_MID] * eval_config.threat_protected_pawn_scale / 128;
    score_end += pcnt * threat_protected_pawn_push[PHASE_END] * eval_config.threat_protected_pawn_scale / 128;

    // bonus for safe checks
    b = checks[side] & ~occ_f;
    b &= ~att_area[side ^ 1] |
        (d_att_area[side] & ~d_att_area[side ^ 1] &
        (piece_att[side ^ 1][KING] | piece_att[side ^ 1][QUEEN]));
    if (b)
    {
      k_cnt[side] ++;
      k_score[side] += _popcnt(b) * SAFE_CHECK_BONUS;
    }

    // attacked squares next to the king
    b = piece_att[side ^ 1][KING] & att_area[side] & ~d_att_area[side ^ 1];
    k_score[side] += _popcnt(b) * K_SQ_ATTACK;

    // scale king safety
    score_mid += _sqr(k_score[side] * eval_config.king_safety_scale / 128) * k_cnt_mul[_min(k_cnt[side], K_CNT_LIMIT - 1)] / 8;

    // potential threats on the opponent's queen
    #define _score_threats_on_queen(piece, method)                             \
      b = piece_att[side][piece] & method(occ, sq) & safe_area;                \
      if (piece != KNIGHT) b &= d_att_area[side];                              \
      if (b)                                                                   \
      {                                                                        \
        pcnt = _popcnt(b);                                                     \
        score_mid += pcnt * threats_on_queen[piece][PHASE_MID] * eval_config.threats_on_queen_scale / 128;                \
        score_end += pcnt * threats_on_queen[piece][PHASE_END] * eval_config.threats_on_queen_scale / 128;                \
      }

    b = pos->piece_occ[QUEEN] & occ_o;
    if (b)
    {
      sq = _bsf(b);
      safe_area =
        mob_area[side] &
        (~att_area[side ^ 1] | (d_att_area[side] & ~d_att_area[side ^ 1]));

      _score_threats_on_queen(KNIGHT, knight_attack);
      _score_threats_on_queen(BISHOP, bishop_attack);
      _score_threats_on_queen(ROOK, rook_attack);
    }

    score_mid = -score_mid;
    score_end = -score_end;
  }

  // exchange sacrifice compensation: amplify positional score when
  // a side has traded a rook for a minor piece
  {
    int stm = pos->side;
    int opp = stm ^ 1;
    int rooks_stm = _popcnt(pos->piece_occ[ROOK] & pos->occ[stm]);
    int rooks_opp = _popcnt(pos->piece_occ[ROOK] & pos->occ[opp]);
    int minors_stm = _popcnt((pos->piece_occ[KNIGHT]|pos->piece_occ[BISHOP]) & pos->occ[stm]);
    int minors_opp = _popcnt((pos->piece_occ[KNIGHT]|pos->piece_occ[BISHOP]) & pos->occ[opp]);

    int raw_mat = 0;
    for (int p = PAWN; p < KING; p++)
      raw_mat += (_popcnt(pos->piece_occ[p] & pos->occ[stm])
                - _popcnt(pos->piece_occ[p] & pos->occ[opp])) * piece_value[p];
    raw_mat = raw_mat * eval_config.material_scale / 128;

    if ((rooks_stm == rooks_opp - 1 && minors_stm == minors_opp + 1) ||
        (rooks_opp == rooks_stm - 1 && minors_opp == minors_stm + 1))
    {
      int pos_mid = score_mid - raw_mat;
      int pos_end = score_end - raw_mat;
      if (pos_mid > 0) score_mid += pos_mid * eval_config.exchange_sac_scale / 128;
      if (pos_end > 0) score_end += pos_end * eval_config.exchange_sac_scale / 128;
    }
  }

  if (pos->side == BLACK)
  {
    score_mid = -score_mid;
    score_end = -score_end;
  }

  // initiative
  initiative_bonus =
    (initiative[0] * _popcnt(p_occ) +
    initiative[1] * ((p_occ & _B_Q_SIDE) && (p_occ & _B_K_SIDE)) +
    initiative[2] * (_popcnt(occ & ~p_occ) == 2) -
    initiative[3]) * eval_config.initiative_scale / 128;

  score_end += _sign(score_end) * _max(initiative_bonus, -_abs(score_end));

  // score interpolation
  if (pos->phase >= TOTAL_PHASE)
    score = score_end;
  else
    score = ((score_mid * (TOTAL_PHASE - pos->phase)) +
             (score_end * pos->phase)) >> PHASE_SHIFT;

  return score + TEMPO * eval_config.tempo / 128;
}
