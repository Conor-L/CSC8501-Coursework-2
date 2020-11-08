#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stack>

struct Cell { 
	char value = ' '; 
	int x = 0; 
	int y = 0; 
	bool visited = false; 
	bool isWall = false;
	Cell* up_neighbour = nullptr;
	Cell* down_neighbour = nullptr;
	Cell* left_neighbour = nullptr;
	Cell* right_neighbour = nullptr;
};

struct Node {
	Cell* current_cell;
	Node* parent_cell = nullptr;
	double f = FLT_MAX;
	double g = FLT_MAX;
	double h = FLT_MAX;
};

struct Entrance {
	Cell* current_cell;
	Cell* available_neighbour;
};

struct Player {
	// TODO Clear the memory of these
	Cell* current_cell;
	Cell* previous_cell;
	char value = 'P';

	Entrance* my_entrance;
	std::stack<Node*> my_route;

	std::vector<Node*> open;
	std::vector<Node*> closed;
	std::vector<Node*> path;
	std::stack<Node*> stack_path;

	bool found_exit = false;
};



class Maze{	
	public:
		Maze();
		Maze(int dim_x, int dim_y, int num_exits, int num_players); // Parameterised Constructor where the user can provide the information needed
		Maze(const Maze& rhs);
		~Maze();

		// Maze initialisation functions
		void fill_maze();
		void set_walls();
		void set_neighbours();
		void generate_maze(Cell initial);
		std::vector<Cell*> get_neighbours(Cell current_cell);
		bool check_space(Cell cell_to_check);
		void generate_maze_centre();
		void print_maze();

		// Maze helper functions
		void place_entrance(int num_entrances);
		void place_finish(int finishx, int finishy);
		int generate_random_number(int upper_limit, int lower_limit);

		// Save and Load functions
		void save_maze(Maze* maze, std::string filename);
		void save_progression(Maze* m, std::string filename);
		void write_file(Maze* m, std::string filename);
		void write_progression(Maze* m, std::string filename);
		Maze* load_maze(std::string filename);
		void load_progression(std::string filename);

		// A* Algorithm route generation
		void generate_route(Node* dest, Player* player, Node* starting_node);
		bool node_is_dest(int x, int y, Node* dest);
		bool check_closed_list(Node* n, Player* player);
		void set_next_node(Player* player, Node* next_node, Node* q, Cell* c, Node* dest);
		double calculate_heuristic(int x, int y, Node* dest);
		void create_path(std::vector<Node*> path, Node* dest, Node* initial, Player* player);

		// A*  Algorithm helper functions
		void generate_all_routes(std::vector<Cell*> exit_vector);

		// Player placements and tracking
		void place_players(int num_players);
		Cell* find_available_neighbour(Cell* entrance);
		void update_player_position(Player* player, Cell* new_cell);
		bool step_through_movements(std::vector<Player*> active_players);
		void convert_path_vector(std::vector<Player*> players);

		// Used for retrieving the exit vector needed for assisting the User Input
		std::vector<Cell*> get_entrances();
		std::vector<Player*> get_players();

		// Get the array index
		int get_array_index(int x, int y);

	private:
		Cell* maze;
		int maze_x_size = 0;
		int maze_y_size = 0;
		int num_entrances = 2;
		int num_players = 2;
		int turn = 0;

		Cell* finishing_cell;
		std::vector<Cell*> entrance_cells;
		std::vector<Entrance*> entrance_vector;
		std::vector<Player*> active_players;

		int players_finished = 0;
};