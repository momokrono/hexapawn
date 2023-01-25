#include <algorithm>

#include "engine.hpp"


Engine::Engine()
{
    _state[2] = { -1, -1, -1 };
    _state[1] = {  0,  0,  0 };
    _state[0] = {  1,  1,  1 };

    _player = Player::white;
    _winner = std::nullopt;
}


auto Engine::game_ended() -> std::optional<Player>
{
    // check if we reached the other side
    if ( std::ranges::find(_state[2], 1) != _state[2].end() ) {
        _winner = Player::white;
        return Player::white;
    }

    // check if we reached the other side
    if ( std::ranges::find(_state[0], -1) != _state[0].end() ) {
        _winner = Player::black;
        return Player::black;
    }

    generate_moves();
    if ( _valid_moves.empty() ) {
        _winner = _player == Player::white ? Player::black : Player::white;
        return _winner;
    }

    return std::nullopt;
}


auto Engine::generate_moves() -> void
{
    std::vector<Move> valid_moves;

    if ( _player == Player::white ) {
        for ( auto line = 0; line < 3; ++line ) {
            for ( auto square = 0; square < 3; ++square ) {
                if ( _state[line][square] == 1 ) {
                    // we check if we can push forward
                    if ( _state[line + 1][square] == 0 ) {
                        valid_moves.emplace_back(Move{{line, square}, {line + 1, square}});
                    }

                    // now we check for captures
                    // DO NOT use an else statement otherwise the second 'if' will not execute if the first does
                    // left to right
                    if ( square + 1 < 3 and _state[line + 1][square + 1] == -1 ) {
                        valid_moves.emplace_back(Move{{line, square}, {line + 1, square + 1}});
                    }
                    // now right to left
                    if ( square - 1 > -1 and _state[line + 1][square - 1] == -1 ) {
                        valid_moves.emplace_back(Move{{line, square}, {line + 1, square - 1}});
                    }
                }
            }
        }
    } else {
        for ( auto line = 0; line < 3; ++line ) {
            for ( auto square = 0; square < 3; ++square ) {
                if ( _state[line][square] == -1 ) {
                    // we check if we can push forward
                    if ( _state[line - 1][square] == 0 ) {
                        valid_moves.emplace_back(Move{{line, square}, {line - 1, square}});
                    }

                    // now we check for captures
                    // DO NOT use an else statement otherwise the second 'if' will not execute if the first does
                    // left to right
                    if ( square + 1 < 3 and _state[line - 1][square + 1] == 1 ) {
                        valid_moves.emplace_back(Move{{line, square}, {line - 1, square + 1}});
                    }
                    // now right to left
                    if ( square - 1 > -1 and _state[line - 1][square - 1] == 1 ) {
                        valid_moves.emplace_back(Move{{line, square}, {line - 1, square - 1}});
                    }
                }
            }
        }
    }

    _valid_moves = valid_moves;
}


auto Engine::get_char( int x, int y ) const -> char
{
    if ( _state[x][y] ==  1 ) { return 'X'; }
    if ( _state[x][y] == -1 ) { return 'O'; }
    return ' ';
}


auto Engine::dump_board() const -> void
{
    for ( auto i = 0; i < 3; ++i ) {
        fmt::print("[ {} ][ {} ][ {} ]\n", get_char( 2 - i, 0 ), get_char( 2 - i, 1 ), get_char( 2 - i, 2 ));
    }
    fmt::print("\n");
}


auto Engine::play() -> void
{
    srand(time(nullptr));
    while ( game_ended() == std::nullopt ) {
        dump_board();
        fmt::print("{} to move:\n\n", _player == Player::white ? "white" : "black");

        if ( _player == Player::white ) {
            exec_move(pick_random_move(_valid_moves));
        } else {
            auto [m, move] = ab_min(-2, 2);
            auto [x, y] = move._from;
            auto [i, j] = move._to;
            fmt::print("after search, best move is [{}][{}] -> [{}][{}]\n\n", x, y, i, j);
            exec_move(move);
        }
    }
    dump_board();
    fmt::print("winner is {}\n", _winner == Player::white ? "white" : "black");
}


auto Engine::pick_random_move(std::vector<Move> const & moves) -> Move
{
    return moves[rand() % moves.size()];
}


auto Engine::ab_max( int alpha, int beta ) -> std::pair<int, Move >
{
    auto max_v = -2;
    auto max_move = Move{};

    auto result = game_ended();

    if ( result == Player::white ) {
        return { 1, {} };
    }
    if ( result == Player::black ) {
        return { -1, {} };
    }

    auto saved_moves = _valid_moves;

    for ( auto & move : saved_moves ) {
        auto save_state = _state;
        exec_move(move);
        auto [m, min_move] = ab_min( alpha, beta );
        if ( m > max_v ) {
            max_v = m;
            max_move = move;
        }
        _state = save_state;
        _player = Player::white;

        if ( max_v >= beta ) {
            return { max_v, move };
        }
        if ( max_v > alpha ) {
            alpha = max_v;
        }
    }
    return { max_v, max_move };
}


auto Engine::ab_min( int alpha, int beta ) -> std::pair<int , Move >
{
    auto min_v = 2;
    auto min_move = Move{};

    auto result = game_ended();

    if ( result == Player::white ) {
        return { 1, {} };
    }
    if ( result == Player::black ) {
        return { -1, {} };
    }

    auto saved_moves = _valid_moves;

    for ( auto & move : saved_moves ) {
        auto save_state = _state;
        exec_move(move);
        auto [m, max_move] = ab_max( alpha, beta );
        if ( m < min_v ) {
            min_v = m;
            min_move = move;
        }
        _state = save_state;
        _player = Player::black;

        if ( min_v <= alpha ) {
            return { min_v, move };
        }
        if ( min_v < beta ) {
            beta = min_v;
        }
    }
    return { min_v, min_move };
}


auto Engine::exec_move(Move move) -> void
{
    auto [x, y] = move._from;
    auto [i, j] = move._to;
    _state[x][y] = 0;
    _state[i][j] = _player == Player::white ? 1 : -1;

    _player = _player == Player::white ? Player::black : Player::white;
}
