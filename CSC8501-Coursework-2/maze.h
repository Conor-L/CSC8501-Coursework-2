#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

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

class Maze{	
	public:
		Maze();
		Maze(int dim_x, int dim_y, int num_exits); // Parameterised Constructor where the user can provide the information needed
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
		void place_exit(int num_exits);
		void place_start(int startx, int starty);
		int generate_random_number(int upper_limit, int lower_limit);

		// Save and Load functions
		void save_maze(Maze* maze, std::string filename);
		void write_file(Maze* m, std::string filename);
		Maze* load_maze(std::string filename);

		// A* Algorithm route generation
		void generate_route(Node* dest);
		bool node_is_dest(int x, int y, Node* dest);
		bool check_closed_list(Node* n);
		void set_next_node(Node* next_node, Node* q, Cell* c, Node* dest);
		double calculate_heuristic(int x, int y, Node* dest);
		void create_path(std::vector<Node*> path, Node* dest, Node* initial);

		// A*  Algorithm helper functions
		std::vector<Cell*> generate_travsersible_cells();
		Node* find_closest_exit(std::vector<Cell*> exit_vector);
		void generate_all_routes(std::vector<Cell*> exit_vector);

		// Used for retrieving the exit vector needed for assisting the User Input
		std::vector<Cell*> get_exits();

	private:
		Cell** maze;
		int maze_x_size = 0;
		int maze_y_size = 0;
		int num_exits = 1;

		Cell* starting_cell;
		std::vector<Cell*> exit_vector;
		std::vector<Cell*> traversible_cells;

		// Vectors for the A* Algorithm
		std::vector<Node*> open;
		std::vector<Node*> closed;
		std::vector<Node*> path;

};