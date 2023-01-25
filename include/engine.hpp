#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <array>
#include <vector>
#include <optional>
#include <fmt/format.h>
#include <fmt/ranges.h>

struct Move
{
    std::pair<int, int> _from, _to;
    Move() = default;
    Move(std::pair<int, int> from,std::pair<int, int> to) : _from{std::move(from)}, _to{std::move(to)} {}
};


enum class Player{ white, black };


class Engine
{
public:
    Engine();

    auto generate_moves() -> void;

    static auto pick_random_move(std::vector<Move> const & moves) -> Move;
    auto exec_move(Move move) -> void;

    [[nodiscard]] auto ab_max( int alpha, int beta ) -> std::pair<int, Move>;
    [[nodiscard]] auto ab_min( int alpha, int beta ) -> std::pair<int, Move>;

    auto play() -> void;
    [[nodiscard]] auto game_ended() -> std::optional<Player>;

    auto dump_board() const -> void;
    [[nodiscard]] auto get_char( int x, int y ) const -> char;

private:
    Player _player;
    std::array<std::array<int, 3>, 3> _state{};
    std::vector<Move> _valid_moves;
    std::optional<Player> _winner;
};


#endif
