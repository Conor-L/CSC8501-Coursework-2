#pragma once
#include <iostream>
#include "maze.h"

class User : public Maze {
	public:
		User(std::string username);

		Maze* generate_maze(int height, int width, int exits);
		Maze* generate_all_routes(Maze* m);
		Maze* generate_random_mazes(Maze* m, int w, int h, int p);
		bool step_through(Maze* m);
		bool step_through_automatically(Maze* m);
		void save_maze(Maze* maze, std::string filename);
		void save_progression(Maze* maze, std::string filename);
		void load_maze(Maze* maze, std::string filename);

		std::string check_string(std::string s);
		int check_integer_input(int input);

		std::string get_username() { return username; }
		void set_username(std::string s) { this->username = s; }

		int get_warning_limit() { return WRN_LIMIT; }

		int get_height_l() { return height_lower_limit; };
		int get_width_l() { return width_lower_limit; };
		int get_players_l() { return player_lower_limit; };
		int get_height_u() { return maze_width_upper; };
		int get_width_u() { return maze_height_upper; };
		int get_players_u() { return player_upper; };

		int get_maze_gen_amount() { return MZE_GEN_AMNT; }

	private:
		std::string username;
		std::string const ALLOWED_CHARACTERS = "abcdefghijklmnopqrstuvwxyz-ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
		int const WRN_LIMIT = 5000;

		int height_lower_limit = 12;
		int width_lower_limit = 35;
		int player_lower_limit = 2;

		int maze_width_upper = 200;
		int maze_height_upper = 100;
		int player_upper = 40;

		int const MZE_GEN_AMNT = 100;

};