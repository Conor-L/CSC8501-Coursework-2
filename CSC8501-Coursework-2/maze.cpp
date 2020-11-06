/*

	The entire project is available on my github at: https://github.com/Conor-L/CSC8501-Coursework-1
	
*/


#include <time.h>
#include "maze.h"
#include <stack>
#include <Windows.h>

using namespace std;
Maze::Maze() {
	srand(time(NULL));

	maze = new Cell*[12];
	for (int i = 0; i < 12; i++) {
		maze[i] = new Cell[35];
	}

	maze_x_size = 11;
	maze_y_size = 34;

	fill_maze();
	set_walls();
	place_start(maze_x_size / 2, maze_y_size / 2);
	set_neighbours();

	generate_maze(*starting_cell);
	generate_maze_centre();
	place_exit(num_exits);

}

Maze::Maze(int dim_x, int dim_y, int num_exits) {
	srand(time(NULL));

	maze_x_size = dim_x - 1;
	maze_y_size = dim_y - 1;
	this->num_exits = num_exits;

	maze = new Cell*[dim_x];
	for (int i = 0; i < dim_x; i++) {
		maze[i] = new Cell[dim_y];
	}

	fill_maze();
	set_walls();
	place_start(maze_x_size / 2, maze_y_size / 2);

	set_neighbours();
	generate_maze(*starting_cell);
	generate_maze_centre();
	place_exit(num_exits);

}
	
Maze::~Maze() {
	// Clean up
	for (int i = 0; i < maze_x_size + 1; i++) {
		delete[] maze[i];
	}
	delete[] maze;
}

void Maze::fill_maze() {
	for (int i = 0; i < (maze_x_size + 1); i++) {
		for (int j = 0; j < (maze_y_size + 1); j++) {
			maze[i][j].value = 'X';
			maze[i][j].x = i;
			maze[i][j].y = j;
			
		}
	}
}

void Maze::set_walls() {
	for (int i = 0; i < (maze_x_size + 1); i++) {
		for (int j = 0; j < (maze_y_size + 1); j++) {

			if ((j == 0) || (j == maze_y_size)) {
				maze[i][j].isWall = true;
			}

			else if ((i == 0) || (i == maze_x_size)) {
				maze[i][j].isWall = true;
			}
		}
	}
}

void Maze::set_neighbours() {
	for (int i = 0; i < (maze_x_size + 1); i++) {
		for (int j = 0; j < (maze_y_size + 1); j++) {
			if (j == 0 || j != maze_y_size) {
				maze[i][j].right_neighbour = &maze[i][j + 1];
			}

			if (j != 0) {
				maze[i][j].left_neighbour = &maze[i][j - 1];
			}

			if (i == 0 || i != maze_x_size) {
				maze[i][j].down_neighbour = &maze[i + 1][j];
			}

			if (i != 0) {
				maze[i][j].up_neighbour = &maze[i - 1][j];
			}
		}
	}
}

void Maze::generate_maze(Cell c) {
	Cell* initial_cell = &maze[c.x][c.y];
	stack<Cell*> path_stack;

	initial_cell->visited = true;
	path_stack.push(initial_cell);
	
	while (!path_stack.empty()) {
		Cell* current_cell = path_stack.top();
		path_stack.pop();
		vector<Cell*> available_neighbours;

		if (!check_space(*current_cell)) {
			available_neighbours = get_neighbours(*current_cell);
		}

		while (!available_neighbours.empty()) {
			int random = generate_random_number(available_neighbours.size(), 0); 
			Cell* next_cell = available_neighbours.at(random);
			if (!check_space(*next_cell)) {
				path_stack.push(current_cell);
				next_cell->value = ' ';
				next_cell->visited = true;
				path_stack.push(next_cell);
			}
			available_neighbours.erase(available_neighbours.begin() + random);
		}
	}
	
}

vector<Cell*> Maze::get_neighbours(Cell current) {
	vector<Cell*> available_neighbours;
	
	if ((current.up_neighbour->visited == false) && (current.up_neighbour->isWall == false)) {
		available_neighbours.emplace_back(maze[current.x][current.y].up_neighbour);
	}
	
	if ((current.down_neighbour->visited == false) && (current.down_neighbour->isWall == false)) {
		available_neighbours.emplace_back(maze[current.x][current.y].down_neighbour);
	}

	if ((current.right_neighbour->visited == false) && (current.right_neighbour->isWall == false)) {
		available_neighbours.emplace_back(maze[current.x][current.y].right_neighbour);
	}
	
	if ((current.left_neighbour->visited == false) && (current.left_neighbour->isWall == false)) {
		available_neighbours.emplace_back(maze[current.x][current.y].left_neighbour);
	}
	
	return available_neighbours;

}

bool Maze::check_space(Cell check) {
	int space_count = 0;

	if (check.up_neighbour->visited == true || check.up_neighbour->isWall == true) {
		space_count++;
	}

	if (check.down_neighbour->visited == true || check.down_neighbour->isWall == true) {
		space_count++;
	}

	if (check.left_neighbour->visited == true || check.left_neighbour->isWall == true) {
		space_count++;
	}

	if (check.right_neighbour->visited == true || check.right_neighbour->isWall == true) {
		space_count++;
	}

	if (check.up_neighbour->isWall == true || check.down_neighbour->isWall == true || check.left_neighbour->isWall == true || check.right_neighbour->isWall == true) {
		return (space_count > 2 ? true : false);
	}

	else {
		return (space_count > 1 ? true : false);
	}

}

void Maze::generate_maze_centre() {
	int startx = starting_cell->x;
	int starty = starting_cell->y;

	for (int i = starty - 2; i < starty + 3; i++) {
		maze[startx - 1][i].value = ' ';
		maze[startx - 1][i].visited = true;
	}

	for (int i = starty - 2; i < starty + 3; i++) {
		if (i == starty) {
			continue;
		}
		maze[startx][i].value = ' '; 
		maze[startx - 1][i].visited = true;
	}

	for (int i = starty - 2; i < starty + 3; i++) {
		maze[startx + 1][i].value = ' '; 
		maze[startx - 1][i].visited = true;
	}

}

void Maze::print_maze() {
	for (int i = 0; i < (maze_x_size + 1); i++) {
		for (int j = 0; j < (maze_y_size + 1); j++) {
			if (maze[i][j].value == 'o') {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				cout << maze[i][j].value;				
			}

			else if (maze[i][j].value == 'E') {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << maze[i][j].value;
			}

			else if (maze[i][j].value == 'S') {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << maze[i][j].value;
			}

			else {
				cout << maze[i][j].value;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		cout << endl;
	}
	cout << endl;
}

void Maze::place_exit(int num_exits) {
	vector<Cell*> possible_exit_positions;
	vector<Cell*> confirmed_exit_positions;
	
	for (int i = 0; i < maze_x_size + 1; i++) {
		for (int j = 0; j < maze_y_size + 1; j++) {
			if (!(i == 0 && j == 0) && !(i == 0 && j == maze_y_size) && !(i == maze_x_size && j == 0) && !(i == maze_x_size && j == maze_y_size)) {
				// Ignore corners
				possible_exit_positions.emplace_back(&maze[i][j]);
			}
		}
	}

	// Check the wall positions for a clear neighbour slot otherwise Exit will be innaccessible
	for (int v = 0; v < possible_exit_positions.size(); v++) {
		Cell* current_cell = possible_exit_positions.at(v);

		if ((current_cell->x > 0 && current_cell->y == 0)) {
			if (current_cell->right_neighbour->value == ' ') {
				confirmed_exit_positions.emplace_back(current_cell);
			}
		}

		else if ((current_cell->x > 0 && current_cell->y == maze_y_size)) {
			if (current_cell->left_neighbour->value == ' ') {
				confirmed_exit_positions.emplace_back(current_cell);
			}
		}

		else if ((current_cell->x == 0 && current_cell->y > 0)) {
			if (current_cell->down_neighbour->value == ' ') {
				confirmed_exit_positions.emplace_back(current_cell);
			}
		}

		else if ((current_cell->x == maze_x_size && current_cell->y > 0)) {
			if (current_cell->up_neighbour->value == ' ') {
				confirmed_exit_positions.emplace_back(current_cell);
			}
		}

	}

	for (int k = 0; k < num_exits; k++) {
		int random_exit = generate_random_number(confirmed_exit_positions.size(), 0);
		confirmed_exit_positions.at(random_exit)->value = 'E';
		exit_vector.emplace_back(confirmed_exit_positions.at(random_exit));
	}
	
}

void Maze::place_start(int startx, int starty) {
	maze[startx][starty].value = 'S';
	starting_cell = &maze[startx][starty];
}

int Maze::generate_random_number(int upper_limit, int lower_limit) {
	return(rand() % upper_limit + lower_limit);
}

void Maze::save_maze(Maze* maze, string filename) {
	cout << "A txt file and an rtf file will be generated - the rtf file is readable but the txt file is only meant for the computer." << endl;
	string txt_f = filename + ".txt";
	string rtf_f = filename + "-rtf.rtf";

	write_file(maze, txt_f);
	write_file(maze, rtf_f);

}

void Maze::write_file(Maze* m, string f) {
	ofstream ostream;

	ostream.open(f);
	if (!ostream) {
		cout << "There was an issue opening this file." << endl;
	}

	ostream << m->maze_x_size + 1 << "|" << m->maze_y_size + 1 << endl;
	ostream << m->num_exits << endl;
	for (int i = 0; i < (m->maze_x_size + 1); i++) {
		for (int j = 0; j < (m->maze_y_size + 1); j++) {
			ostream << m->maze[i][j].value;
		}
		ostream << endl;
	}

	if (!ostream) {
		cout << "There was an issue writing to this file." << endl;
	}

	ostream.close();
}

Maze* Maze::load_maze(string filename) {	
	ifstream istream;

	istream.open(filename + ".txt");

	if (!istream) {
		cout << "There was an issue opening this file." << endl;
	}

	else {
	
		string dimensions;
		istream >> dimensions;

		int split = dimensions.find('|');
		int height = stoi(dimensions.substr(0, split));
		int width = stoi(dimensions.substr(split + 1));

		int exits;
		istream >> exits;

		Maze* new_maze = new Maze(height, width, exits);

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				char c;
				c = istream.get();

				if (c == '\n') {
					c = istream.get();
				}

				new_maze->maze[i][j].value = c;

			}
		}

		new_maze->print_maze();

		if (!istream) {
			cout << "There was an issue reading this file. Please check your spelling and try again!" << endl;
		}

		istream.close();
		return new_maze;
	}
	return nullptr;
}

void Maze::generate_route(Node* dest) {
	closed.clear();
	open.clear();
	path.clear();

	vector<Cell*> copy_cells = generate_travsersible_cells();
	Cell* starting_cell = nullptr;
	for (int i = 0; i < copy_cells.size(); i++) {
		if (copy_cells.at(i)->value == 'S') {
			starting_cell = copy_cells.at(i);
		}
	}
	Node* starting_node = new Node; 
	starting_node->current_cell = starting_cell;
	starting_node->f = 0;
	starting_node->g = 0;
	starting_node->h = 0;

	open.emplace_back(starting_node);
	path.emplace_back(starting_node);

	bool dest_found = false;

	while (open.empty() != true) {
		Node* q = new Node;
		int index_of_q = 0;
		double lowest_f = 9999;
		for (int i = 0; i < open.size(); i++) {
			if (open.at(i)->f < lowest_f) {
				lowest_f = open.at(i)->f;
				q = open.at(i);
				index_of_q = i;
				
			}
		}

		open.erase(open.begin() + index_of_q);
		closed.emplace_back(q);

		// North

		if (q != nullptr && (q->current_cell->up_neighbour != nullptr && q->current_cell->up_neighbour->value != 'X')) {
			Cell* c = q->current_cell->up_neighbour;
			Node* n = new Node;
			n->current_cell = c;

			if (node_is_dest(c->x, c->y, dest) == true) {
				dest->parent_cell = q;				
				dest_found = true;
				path.emplace_back(dest);
				break;
			}

			else if (!(check_closed_list(n))) {
				set_next_node(n, q, c, dest);
			}
		}	

		// South

		if (q != nullptr && (q->current_cell->down_neighbour != nullptr && q->current_cell->down_neighbour->value != 'X')) {
			Cell* c = q->current_cell->down_neighbour;
			Node* n = new Node;
			n->current_cell = c;

			if (node_is_dest(c->x, c->y, dest) == true) {
				dest->parent_cell = q;				
				dest_found = true;
				path.emplace_back(dest);
				break;
			}

			else if (!(check_closed_list(n))) {
				set_next_node(n, q, c, dest);
			}
		}

		// East

		if (q != nullptr && (q->current_cell->right_neighbour != nullptr && q->current_cell->right_neighbour->value != 'X')) {
			Cell* c = q->current_cell->right_neighbour;
			Node* n = new Node;
			n->current_cell = c;

			if (node_is_dest(c->x, c->y, dest) == true) {
				dest->parent_cell = q;				
				dest_found = true;
				path.emplace_back(dest);
				break;
			}

			else if (!(check_closed_list(n))) {
				set_next_node(n, q, c, dest);
			}
		}

		// West

		if (q != nullptr && (q->current_cell->left_neighbour != nullptr && q->current_cell->left_neighbour->value != 'X')) {
			Cell* c = q->current_cell->left_neighbour;
			Node* n = new Node;
			n->current_cell = c;

			if (node_is_dest(c->x, c->y, dest) == true) {
				dest->parent_cell = q;				
				dest_found = true;
				path.emplace_back(dest);
				break;
			}

			else if (!(check_closed_list(n))) {
				set_next_node(n, q, c, dest);
			}
		}
	}
	create_path(path, dest, starting_node);
}

bool Maze::check_closed_list(Node* n) {
	bool dupe_check = false;
	for (int i = 0; i < closed.size(); i++) {
		Cell* ct = closed.at(i)->current_cell;
		if (n->current_cell == ct) {
			dupe_check = true;
			return dupe_check;
		}

		else {
			dupe_check = false;
		}
	}
	return dupe_check;
}

void Maze::set_next_node(Node* next_node, Node* q, Cell* c, Node* dest) {
	double new_g, new_f, new_h;
	bool dupe_check = false;

	new_g = q->g + 1.0;
	new_h = calculate_heuristic(c->x, c->y, dest);
	new_f = new_g + new_h;

	for (int i = 0; i < open.size(); i++) {
		Cell* ct = open.at(i)->current_cell;
		if (next_node->current_cell == ct) {
			dupe_check = true;
			break;
		}

		else {
			dupe_check = false;
		}
	}

	if ((next_node->f == FLT_MAX || next_node->f > new_f) && !dupe_check) {
		next_node->f = new_f;
		next_node->g = new_g;
		next_node->h = new_h;
		next_node->parent_cell = q;

		open.emplace_back(next_node);
		path.emplace_back(next_node);
	}
}

bool Maze::node_is_dest(int x, int y, Node* dest) {
	if (x == dest->current_cell->x && y == dest->current_cell->y) {
		return true;
	}
	return false;
}

double Maze::calculate_heuristic(int x, int y, Node* dest) {
	return ((double)sqrt(pow((x - dest->current_cell->x), 2) + pow((y - dest->current_cell->y), 2)));
}

void Maze::create_path(vector<Node*> path, Node* dest, Node* initial) {

	stack<Node*> complete;
	Node* current_node = dest;
	while (!(current_node->current_cell == initial->current_cell)) {
		complete.push(current_node);
		for (int i = 0; i < path.size(); i++) {
			if (current_node->parent_cell->current_cell == path.at(i)->current_cell) {
				current_node = path.at(i);
				break;
			}
		}
	}

	complete.push(dest);
	while (!complete.empty()) {
		Node* current_pop = complete.top();
		complete.pop();
		if (current_pop->current_cell->value == ' ' || current_pop->current_cell->value == 'o') {
			// Checking if a node is already 'o' is necessary if you are generating loads of exits.
			maze[current_pop->current_cell->x][current_pop->current_cell->y].value = 'o';
		}
	}
}

vector<Cell*> Maze::generate_travsersible_cells() {
	for (int i = 0; i < maze_x_size + 1; i++) {
		for (int j = 0; j < maze_y_size + 1; j++) {
			if (maze[i][j].value == ' ' || maze[i][j].value == 'E' || maze[i][j].value == 'S') {
				traversible_cells.emplace_back(&maze[i][j]);
			}
		}
	}
	return traversible_cells;
}

Node* Maze::find_closest_exit(vector<Cell*> exits) {
	Node* closest_node = new Node;

	for (int i = 0; i < exits.size(); i++) {
		Node* current_exit = new Node;
		current_exit->current_cell = exits.at(i);

		double new_h = calculate_heuristic(starting_cell->x, starting_cell->y, current_exit);

		if (new_h < closest_node->h) {
			closest_node = current_exit;
		}
	}

	return closest_node;

}

void Maze::generate_all_routes(vector<Cell*> exits) {

	Node* exit_node = new Node;
	for (int i = 0; i < exits.size(); i++) {
		exit_node->current_cell = exits.at(i);
		generate_route(exit_node);
	}

}

vector<Cell*> Maze::get_exits() {
	return exit_vector;
}