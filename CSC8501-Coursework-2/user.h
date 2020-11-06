#pragma once
#include <iostream>
#include "maze.h"

class User : public Maze {
	public:
		User(std::string username);

		Maze* generate_maze(int height, int width, int exits);
		Maze* generate_shortest_route(Maze* m);
		Maze* generate_all_routes(Maze* m);
		void save_maze(Maze* maze, std::string filename);
		void load_maze(Maze* maze, std::string filename);

		std::string check_string(std::string s);
		int check_integer_input(int input);

		std::string get_username() { return username; }
		void set_username(std::string s) { this->username = s; }

		int get_warning_limit() { return WRN_LIMIT; }

	private:
		std::string username;
		std::string const ALLOWED_CHARACTERS = "abcdefghijklmnopqrstuvwxyz-ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
		int const WRN_LIMIT = 5000;
};