/*

	The entire project is available on my github at: https://github.com/Conor-L/CSC8501-Coursework-2
	
*/


#include <time.h>
#include "maze.h"
#include <Windows.h>

using namespace std;
Maze::Maze() {
	srand(time(0));

	maze = new Cell*[12];
	for (int i = 0; i < 12; i++) {
		maze[i] = new Cell[35];
	}

	maze_x_size = 11;
	maze_y_size = 34;

	fill_maze();
	set_walls();
	place_finish(maze_x_size / 2, maze_y_size / 2);
	set_neighbours();

	generate_maze(*finishing_cell);
	generate_maze_centre();
	place_entrance(num_entrances);

}

Maze::Maze(int dim_x, int dim_y, int num_entrances, int num_players) {
	srand(time(0));

	maze_x_size = dim_x - 1;
	maze_y_size = dim_y - 1;
	this->num_entrances = num_entrances;
	this->num_players = num_players;

	maze = new Cell*[dim_x];
	for (int i = 0; i < dim_x; i++) {
		maze[i] = new Cell[dim_y];
	}

	fill_maze();
	set_walls();
	place_finish(maze_x_size / 2, maze_y_size / 2);

	set_neighbours();
	generate_maze(*finishing_cell);
	generate_maze_centre();
	place_entrance(num_entrances);
	place_players(num_players);

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
			int random = generate_random_number(available_neighbours.size() - 1, 0); 
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
	
	if (!(current.up_neighbour == nullptr) && (current.up_neighbour->visited == false) && (current.up_neighbour->isWall == false)) {
		available_neighbours.emplace_back(maze[current.x][current.y].up_neighbour);
	}
	
	if (!(current.down_neighbour == nullptr) && (current.down_neighbour->visited == false) && (current.down_neighbour->isWall == false)) {
		available_neighbours.emplace_back(maze[current.x][current.y].down_neighbour);
	}

	if (!(current.right_neighbour == nullptr) && (current.right_neighbour->visited == false) && (current.right_neighbour->isWall == false)) {
		available_neighbours.emplace_back(maze[current.x][current.y].right_neighbour);
	}
	
	if (!(current.left_neighbour == nullptr) && (current.left_neighbour->visited == false) && (current.left_neighbour->isWall == false)) {
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
	int startx = finishing_cell->x;
	int starty = finishing_cell->y;

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

			else if (maze[i][j].value == 'F') {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << maze[i][j].value;
			}

			else if (maze[i][j].value == 'P') {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
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

void Maze::place_entrance(int num_entrances) {
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

	for (int k = 0; k < num_entrances; k++) {
		int random_exit = generate_random_number(confirmed_exit_positions.size(), 0);
		Cell* c = confirmed_exit_positions.at(random_exit);		
		
		if (c->value != 'E') {
			c->value = 'E';
			entrance_cells.emplace_back(c);

			Entrance* new_entrance = new Entrance;
			new_entrance->current_cell = c;
			new_entrance->available_neighbour = find_available_neighbour(c);

			entrance_vector.emplace_back(new_entrance);
		}

		else {
			// Go back and attempt to re-place that entrance
			k--;
			continue;
		}
	}
	
}

void Maze::place_finish(int finishx, int finishy) {
	maze[finishx][finishy].value = 'F';
	finishing_cell = &maze[finishx][finishy];
}

int Maze::generate_random_number(int upper_limit, int lower_limit) {
	int random = lower_limit + (rand() % (upper_limit - lower_limit + 1));
	return random;
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
	ostream << m->num_entrances << endl;
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
		cout << "There was an issue opening this file: File does not exist." << endl;
	}

	else {

		int search_valid;
		int entrance_counter = 0;
		int split = -1;
		int height = -1;
		int width = -1;
	
		string dimensions;
		istream >> dimensions;

		search_valid = dimensions.find('|');

		if (search_valid < dimensions.size()) {
			split = dimensions.find('|');
			height = stoi(dimensions.substr(0, split));
			width = stoi(dimensions.substr(split + 1));
		}

		else {
			cout << "-Error:" << endl;
			cout << "The file that you have attempted to load a maze does not contain maze dimensions." << endl;
			cout << "You can fix this by adding the dimensions to the file yourself:" << endl;
			cout << "To add the dimensions, load the file and type <dim_x>|<dim_y> on the first line and press enter; where <dim_x> and <dim_y> are the height and width." << endl;
			cout << "For example: 12|35" << endl;
			cout << "- - - - - - - - - - - - - -" << endl;
			return nullptr;
		}		

		string entrances_s;
		int entrances;
		istream >> entrances_s;

		search_valid = entrances_s.find('X');

		if (search_valid < entrances_s.size()) {
			cout << "-Error:" << endl;
			cout << "The file that you have attempted to load a maze does not contain maze entrances." << endl;
			cout << "You can fix this by adding the entrances to the file yourself:" << endl;
			cout << "To add the entrances, load the file and type the number of entrances on the second line and press enter." << endl;
			cout << "- - - - - - - - - - - - - -" << endl;
			return nullptr;
		}

		else {
			entrances = stoi(entrances_s);
		}

		Maze* new_maze = nullptr;

		try {
			new_maze = new Maze(height, width, entrances, entrances);
		}
		catch (const std::bad_array_new_length e) {
			cout << "- - - - - - - - - " << endl;
			cout << "An exception has occured attempting to generate your maze: " << e.what() << endl;
			cout << "Check: " << endl;
			cout << "(1) The dimensions that you have entered are actually correct." << endl;
			cout << "(2) The number of exits that you have entered are actually correct." << endl;
			cout << "- - - - - - - - - " << endl;
			return nullptr;
		}

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				char c;
				c = istream.get();

				if (c == 'E') {
					entrance_counter++;
				}

				if (c == '\n') {
					c = istream.get();
				}

				new_maze->maze[i][j].value = c;

			}
		}

		char eof;
		istream >> eof; // This will get the file end character after the for loop

		if (istream.eof() == 0) {
			cout << "- - - - - - - - - " << endl;
			cout << "There was an issue reading this file: end of file not reached." << endl;
			cout << "Check the maze dimensions and make sure that they correct in order to read whole file." << endl;
			cout << "- - - - - - - - - " << endl;
			return nullptr;
		}

		if (entrance_counter != entrances) {
			cout << "- - - - - - - - - " << endl;
			cout << "There was an issue reading this file: entrances inconsistent." << endl;
			cout << "Check the maze entrance dimension to make sure it is correct (it should equal how many entrances actually exist)." << endl;
			cout << "- - - - - - - - - " << endl;
			return nullptr;
		}

		new_maze->print_maze();

		if (!istream && (istream.eof() == 0)) {
			cout << "There was an issue reading this file. Please check your spelling and try again!" << endl;
		}

		istream.close();
		return new_maze;
	}
	return nullptr;
}

void Maze::generate_route(Node* dest, Player* player, Node* starting_node) {

	while (!(player->open.empty())) {
		Node* q = new Node;
		int index_of_q = 0;
		double lowest_f = 9999;
		for (int i = 0; i < player->open.size(); i++) {
			if (player->open.at(i)->f < lowest_f) {
				lowest_f = player->open.at(i)->f;
				q = player->open.at(i);
				index_of_q = i;
				
			}
		}

		player->open.erase(player->open.begin() + index_of_q);
		player->closed.emplace_back(q);

		// North

		if (q != nullptr && (q->current_cell->up_neighbour != nullptr && q->current_cell->up_neighbour->value != 'X')) {
			Cell* c = q->current_cell->up_neighbour;
			Node* n = new Node;
			n->current_cell = c;

			if (node_is_dest(c->x, c->y, dest) == true) {
				dest->parent_cell = q;
				player->path.emplace_back(dest);
				break;
			}

			else if (!(check_closed_list(n, player))) {
				set_next_node(player, n, q, c, dest);
			}
		}	

		// South

		if (q != nullptr && (q->current_cell->down_neighbour != nullptr && q->current_cell->down_neighbour->value != 'X')) {
			Cell* c = q->current_cell->down_neighbour;
			Node* n = new Node;
			n->current_cell = c;

			if (node_is_dest(c->x, c->y, dest) == true) {
				dest->parent_cell = q;
				player->path.emplace_back(dest);
				break;
			}

			else if (!(check_closed_list(n, player))) {
				set_next_node(player, n, q, c, dest);
			}
		}

		// East

		if (q != nullptr && (q->current_cell->right_neighbour != nullptr && q->current_cell->right_neighbour->value != 'X')) {
			Cell* c = q->current_cell->right_neighbour;
			Node* n = new Node;
			n->current_cell = c;

			if (node_is_dest(c->x, c->y, dest) == true) {
				dest->parent_cell = q;
				player->path.emplace_back(dest);
				break;
			}

			else if (!(check_closed_list(n, player))) {
				set_next_node(player, n, q, c, dest);
			}
		}

		// West

		if (q != nullptr && (q->current_cell->left_neighbour != nullptr && q->current_cell->left_neighbour->value != 'X')) {
			Cell* c = q->current_cell->left_neighbour;
			Node* n = new Node;
			n->current_cell = c;

			if (node_is_dest(c->x, c->y, dest) == true) {
				dest->parent_cell = q;
				player->path.emplace_back(dest);
				break;
			}

			else if (!(check_closed_list(n, player))) {
				set_next_node(player, n, q, c, dest);
			}
		}
	}
	create_path(player->path, dest, starting_node, player);
}

bool Maze::check_closed_list(Node* n, Player* player) {
	bool dupe_check = false;
	for (int i = 0; i < player->closed.size(); i++) {
		Cell* ct = player->closed.at(i)->current_cell;
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

void Maze::set_next_node(Player* player, Node* next_node, Node* q, Cell* c, Node* dest) {
	double new_g, new_f, new_h;
	bool dupe_check = false;

	new_g = q->g + 1.0;
	new_h = calculate_heuristic(c->x, c->y, dest);
	new_f = new_g + new_h;

	for (int i = 0; i < player->open.size(); i++) {
		Cell* ct = player->open.at(i)->current_cell;
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

		player->open.emplace_back(next_node);
		player->path.emplace_back(next_node);
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

void Maze::create_path(vector<Node*> path, Node* dest, Node* initial, Player* player) {

	stack<Node*> complete;
	stack<Node*> reversed_stack;
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

	player->my_route = complete;
	player->my_route.pop();

}

void Maze::generate_all_routes(vector<Cell*> entrances) {
	Node* entrance_node = new Node;
	Node* starting_node = new Node;
	starting_node->f = 0;
	starting_node->g = 0;
	starting_node->h = 0;
	Node* finishing_node = new Node;
	finishing_node->current_cell = finishing_cell;
	Player* current_player;

	for (int k = 0; k < active_players.size(); k++) {
		current_player = active_players.at(k);
		starting_node->current_cell = current_player->my_entrance->current_cell;			
		active_players.at(k)->open.emplace_back(starting_node);
		active_players.at(k)->path.emplace_back(starting_node);
		generate_route(finishing_node, current_player, starting_node);
	}

	//convert_path_vector(active_players);

}

void Maze::place_players(int num_players) {
	for (int i = 0; i < num_players; i++) {
		Entrance* curr_e = entrance_vector.at(i);
		Player* new_player = new Player;
		new_player->current_cell = curr_e->available_neighbour;
		new_player->current_cell->value = 'P';
		new_player->my_entrance = curr_e;
		active_players.emplace_back(new_player);
	}
}

Cell* Maze::find_available_neighbour(Cell* e) {
	if (e->up_neighbour != nullptr && e->up_neighbour->value == ' ') {
		return e->up_neighbour;
	}

	if (e->down_neighbour != nullptr && e->down_neighbour->value == ' ') {
		return e->down_neighbour;
	}

	if (e->left_neighbour != nullptr && e->left_neighbour->value == ' ') {
		return e->left_neighbour;
	}

	if (e->right_neighbour != nullptr && e->right_neighbour->value == ' ') {
		return e->right_neighbour;
	}

	return nullptr;
}

void Maze::update_player_position(Player* player,  Cell* n) {

	player->previous_cell = player->current_cell;
	player->current_cell = n;
	player->current_cell->value = 'P';
	player->previous_cell->value = 'o';
	
}

bool Maze::step_through_movements(vector<Player*> players) {
	if (!(players.at(turn)->my_route.empty())) {

		Node* next_step = players.at(turn)->my_route.top();
		if (next_step->current_cell->value != 'P' || next_step->current_cell == finishing_cell) {
			players.at(turn)->my_route.pop();
			update_player_position(players.at(turn), next_step->current_cell);
		}

		else {
			cout << "A Player has been blocked by another Player!" << endl;
			cout << "Their turn has been skipped." << endl;
		}

		if (players.at(turn)->my_route.empty()) {
			cout << "A Player has finished!" << endl;
			players_finished++;
		}
	}

	if (players_finished == num_entrances) {
		cout << "All Players have finished!" << endl;
		return true;
	}
	
	if (turn != players.size() - 1) {
		turn++;
	}

	else {
		turn = 0;
	}

	print_maze();
	return false;
}

void Maze::convert_path_vector(vector<Player*> players) {
	stack<Node*> reversed_stack;
	for (int i = 0; i < players.size(); i++) {
		for (int j = 0; j < players.at(i)->path.size(); j++) {
			players.at(i)->stack_path.push(players.at(i)->path.at(j));
		}

		for (int k = 0; k < players.at(i)->stack_path.size(); k++) {
			reversed_stack.push(players.at(i)->stack_path.top());
			players.at(i)->stack_path.pop();
		}
		players.at(i)->stack_path = reversed_stack;
	}

	
}

vector<Cell*> Maze::get_entrances() {
	return entrance_cells;
}

vector<Player*> Maze::get_players() {
	return active_players;
}