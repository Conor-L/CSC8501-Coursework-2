#include <chrono>
#include <thread>
#include "user.h"
#include "maze.h"

using namespace std;

User::User(string username) {
	this->username = username;
}

Maze* User::generate_maze(int h, int w, int e) {
	// Make a call to the Maze class
	Maze* maze_gen = new Maze(h, w, e);
	maze_gen->print_maze();

	return maze_gen;
}

Maze* User::generate_shortest_route(Maze* m) {
	m->generate_route(m->find_closest_exit(m->get_exits()));
	m->print_maze();

	return m;
}

Maze* User::generate_all_routes(Maze* m) {
	m->generate_all_routes(m->get_exits());
	m->print_maze();

	return m;
}

void User::save_maze(Maze* m, string f) {
	m->save_maze(m, f);
}

void User::load_maze(Maze* m, string filename) {
	m->load_maze(filename);
}

string User::check_string(string s) {	

	for (int i = 0; i < s.size(); i++) {
		int found = ALLOWED_CHARACTERS.find(s[i]);
		if (!(found < ALLOWED_CHARACTERS.size())) {
			s.erase(s.begin() + i);
			i--; // If we erase something here then we need to decrement i because the size will be reduced by one.
		}
	}

	return s;
}

int User::check_integer_input(int input) {	

	while (!(cin >> input)) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Error: input not of type 'int': please retry" << endl;
		cout << "->";
	}
	return input;
}

int main() {
	User* maze_user = new User("User1");
	Maze* generated_maze = new Maze();
	int input = -1;

	int width = 35;
	int height = 12;
	int exits = 1;
	string filename;
	string username;

	cout << "Enter your username (special characters will be removed!): ";
	cin >> username;

	username = maze_user->check_string(username);

	if (username.empty() == true) {
		cout << "Due to false input username will be set to User1." << endl;
	}
	else {
		maze_user->set_username(username);
	}

	cout << "Files will be saved under " << maze_user->get_username() << " followed by the file name you provide." << endl;
	cout << "Due to screen resolution and route generation time, the maximum width is 150 and the maximum height is 75." << endl;	

	bool keep_running = true;

	while (keep_running) {
		cout << "Choose an option using one of the numbers below: " << endl;
		cout << "(1) Generate a Maze" << endl;
		cout << "(2) Load a previous Maze" << endl;
		cout << "(3) Exit the program" << endl;
		cout << "-> ";

		input = maze_user->check_integer_input(input);

		switch (input) {
			case 1:
				// Generate a Maze
				cout << "How wide do you want the maze to be ? (Min: 35, Max: 200): ";
				width = maze_user->check_integer_input(input);

				width < 35 ? width = 35 : width;
				width > 200 ? width = 200 : width;

				cout << "How high do you want the maze to be ? (Min: 12, Max: 100): ";
				height = maze_user->check_integer_input(input);

				height < 12 ? height = 12 : height;
				height > 100 ? height = 100 : height;
				
				cout << "How many exits do you want ? (Min: 1, Sensible: 5~10, Not-So-Sensible: 20+): ";
				exits = maze_user->check_integer_input(input);

				exits < 1 ? exits = 1 : exits;
				exits > height* width ? exits = 10 : exits;

				if (height * width * exits > maze_user->get_warning_limit()) {
					cout << endl;
					cout << "- - - - - - - - - - - - - - -" << endl;
					cout << "WARNING: the dimensions you have provided (width/height/exits) results in a huge coefficient which will result in slow route generation times" << endl;
					cout << "You may only want to generate the shortest route otherwise it will take too long!" << endl;
					cout << "Resuming in 5 seconds." << endl;
					cout << "- - - - - - - - - - - - - - -" << endl;
					cout << endl;
					this_thread::sleep_for(chrono::seconds(5));
				}

				cout << "Generating Maze of width: " << width << " and height: " << height << " with " << exits << " exits." << endl;
				generated_maze = maze_user->generate_maze(height, width, exits);

				cout << "Do you want to generate routes for this maze? " << endl;
				cout << "(1) Yes " << endl;
				cout << "(2) No " << endl;
				cout << "-> ";

				input = maze_user->check_integer_input(input);

				switch (input) {
					case 1:
						cout << "Do you want to generate routes to every exit or just to the closest?" << endl;
						cout << "(1) All routes" << endl;
						cout << "(2) Shortest" << endl;
						cout << "-> ";

						input = maze_user->check_integer_input(input);

						switch (input) {
							case 1:
								// generate all routes
								generated_maze = maze_user->generate_all_routes(generated_maze);
								break;
							case 2:
								// generate the shortest route
								generated_maze = maze_user->generate_shortest_route(generated_maze);
								break;
							default:
								cout << "That was not a valid option. Please try again." << endl;
								break;
						}

						break;

					case 2:
						cout << "Routes will not be generated." << endl;
						break;

					default:
						cout << "That was not one of the options - Try again" << endl;
						break;

				}

				cout << "Do you want to save this maze? " << endl;
				cout << "(1) Yes " << endl;
				cout << "(2) No " << endl;
				cout << "-> ";

				input = maze_user->check_integer_input(input);

				switch (input) {
					case 1:
						cout << "Please enter a suitable filename - do not include special characters or spaces (they will be removed!): ";
						cin >> filename;
						filename = maze_user->check_string(filename);

						if (filename.empty() == true) {
							cout << "A unique name will be generated due to lack of characters" << endl;
							filename = "maze" + to_string(generated_maze->generate_random_number(50000, 0));
						}

						filename = maze_user->get_username() + "-" + filename;

						maze_user->save_maze(generated_maze, filename);
						break;
					case 2:
						cout << "Saving will not occur." << endl;
						break;
					default:
						cout << "That was not an option - saving will not occur" << endl;
						break;
				}

				break;

			case 2:
				// Load a Previous Maze
				cout << "Please enter the file name that you wish to load: ";
				cin >> filename;
				maze_user->load_maze(generated_maze, filename);
				break;
			case 3:
				// Exit the program
				cout << "Goodbye!" << endl;
				keep_running = false;
				break;
			default :
				// Try again
				cout << "That was not one of the options - try again." << endl;
				break;
		}
	}

	// Clean up memory
	delete maze_user;
	delete generated_maze;

}