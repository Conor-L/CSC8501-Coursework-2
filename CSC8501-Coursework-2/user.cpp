#include <chrono>
#include <thread>
#include "user.h"
#include "maze.h"

using namespace std;

User::User(string username) {
	this->username = username;
}

Maze* User::generate_maze(int h, int w, int e) {
	Maze* maze_gen = new Maze(h, w, e, e);
	maze_gen->print_maze();

	return maze_gen;
}

Maze* User::generate_all_routes(Maze* m) {
	m->generate_all_routes(m->get_entrances());
	cout << "Player routes have been Calculated!" << endl;

	return m;
}

Maze* User::generate_random_mazes(Maze* m, int w, int h, int p) {
	m = generate_maze(m->generate_random_number(h, height_lower_limit), m->generate_random_number(w, width_lower_limit), m->generate_random_number(p, player_lower_limit));
	generate_all_routes(m);
	return m;
}

bool User::step_through(Maze* m) {
	return m->step_through_movements(m->get_players());
}

bool User::step_through_automatically(Maze* m) {
	bool stop = false;

	while (!stop) {
		stop = step_through(m);
		this_thread::sleep_for(chrono::milliseconds(400)); // Makes the console more visible
	}

	return stop;
}

void User::save_maze(Maze* m, string f) {
	m->save_maze(m, f);
}

void User::save_progression(Maze* m, string f) {
	m->save_progression(m, f);
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
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer to avoid tainted input
		cout << "Error: input not of type 'int': please retry" << endl;
		cout << "->";
	}
	return input;
}

int main() {
	User* maze_user = new User("User1");
	Maze* generated_maze = new Maze();
	int input = -1;

	int width = maze_user->get_width_l();
	int height = maze_user->get_height_l();
	int entrances = maze_user->get_players_l();

	int width_u = maze_user->get_width_u();
	int height_u = maze_user->get_height_u();
	int player_u = maze_user->get_players_u();

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
	cout << "Due to screen resolution and route generation time, the maximum width is 200 and the maximum height is 100." << endl;

	bool keep_running = true;
	int finish_looping = false;

	while (keep_running) {
		bool finished_stepping = false;
		cout << "Choose an option using one of the numbers below: " << endl;
		cout << "(1) Generate a Maze" << endl;
		cout << "(2) Load a previous Maze" << endl;
		cout << "(3) Load a series of progression mazes" << endl;
		cout << "(4) Information and Explanation" << endl;
		cout << "(5) Generate a series of random mazes (Warning: this will take a long time.)" << endl;
		cout << "(6) Exit the program" << endl;
		cout << "-> ";

		input = maze_user->check_integer_input(input);

		switch (input) {
		case 1:
			// Generate a Maze
			cout << "How wide do you want the maze to be ? (Min: 35, Max: 200): ";
			width = maze_user->check_integer_input(input);

			width < maze_user->get_width_l() ? width = maze_user->get_width_l() : width;
			width > maze_user->get_width_u() ? width = maze_user->get_width_u() : width;

			cout << "How high do you want the maze to be ? (Min: 12, Max: 100): ";
			height = maze_user->check_integer_input(input);

			height < maze_user->get_height_l() ? height = maze_user->get_height_l() : height;
			height > maze_user->get_height_u() ? height = maze_user->get_height_u() : height;

			cout << "How many entrances do you want? The same number or players will be created. (Min: 2, Sensible: 5~10, Not-So-Sensible: 20+): ";
			entrances = maze_user->check_integer_input(input);

			entrances < maze_user->get_players_l() ? entrances = maze_user->get_players_l() : entrances;
			entrances > height* width ? entrances = maze_user->get_players_u() : entrances;

			if (height * width * entrances > maze_user->get_warning_limit()) {
				cout << endl;
				cout << "- - - - - - - - - - - - - - -" << endl;
				cout << "WARNING: the dimensions you have provided (width/height/entrances) results in a huge coefficient which could result in slow route generation times" << endl;
				cout << "Resuming in 5 seconds." << endl;
				cout << "- - - - - - - - - - - - - - -" << endl;
				cout << endl;
				this_thread::sleep_for(chrono::seconds(5));
			}

			cout << "Generating Maze of width: " << width << " and height: " << height << " with " << entrances << " entrances." << endl;
			generated_maze = maze_user->generate_maze(height, width, entrances);
			generated_maze = maze_user->generate_all_routes(generated_maze);

			cout << "Do you want to start stepping through the players moving? " << endl;
			cout << "(1) Yes " << endl;
			cout << "(2) Save the progression to a file instead" << endl;
			cout << "(3) No " << endl;
			cout << "-> ";

			input = maze_user->check_integer_input(input);

			switch (input) {
			case 1:
				while (finished_stepping != true) {
					cout << "Step?" << endl;
					cout << "(1) Yes" << endl;
					cout << "(2) No" << endl;
					cout << "-> ";

					input = maze_user->check_integer_input(input);

					switch (input) {
					case 1:
						finished_stepping = maze_user->step_through_automatically(generated_maze);
						break;
					case 2:
						finished_stepping = true;
						cout << "User has stopped stepping!" << endl;
						break;
					default:
						cout << "That was not a valid option. Please try again." << endl;
						break;
					}

				}
				break;

			case 2:
				cout << "Please enter a suitable filename - do not include special characters or spaces (they will be removed!): ";
				cin >> filename;
				filename = maze_user->check_string(filename);

				if (filename.empty() == true) {
					cout << "A unique name will be generated due to lack of characters" << endl;
					filename = "maze" + to_string(generated_maze->generate_random_number(50000, 0));
				}

				filename = maze_user->get_username() + "-" + filename;

				maze_user->save_progression(generated_maze, filename);
				break;

			case 3:
				cout << "Routes will not be generated." << endl;
				break;

			default:
				cout << "That was not one of the options - Try again" << endl;
				break;

			}

			cout << "Do you want to save this maze? " << endl;
			cout << "(1) Save this maze " << endl;
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
			// Load a progression Maze
			cout << "Please enter the file name that you wish to load: ";
			cin >> filename;
			maze_user->load_progression(filename);
			break;
		case 4:
			cout << "This is the information section. Here you can learn more about how the maze is traversed." << endl;
			cout << "Choose an option to learn more:" << endl;
			cout << "(1) Deadlock and Livelock scenarios" << endl;
			cout << "(2) Maze and Player combinations" << endl;
			cout << "(3) Return" << endl;
			cout << "-> ";
			input = maze_user->check_integer_input(input);

			switch (input) {
			case 1:
				cout << "- Deadlock and Livelock scenarios -" << endl;
				cout << "As the maze generation and player movement currently works, there are no deadlock scenarios." << endl;
				cout << "A player will never move into a corner and therefore there is no scenario where another player may follow them and trap them." << endl;
				cout << "To prevent players from \"colliding\", if a player comes across another and cannot get past them to continue their route, they will skip their turn." << endl;
				cout << "Due to how the A* algorithm works with each player, players have no battle for nodes." << endl;
				cout << "Therefore, players will consider every traversible node in the maze regardless of where other players have been." << endl;
				cout << "This means that there is no possible livelock scenario" << endl << "- - - - - - - - - -" << endl;

				cout << "Return?" << endl;
				cout << "(1) Yes" << endl;
				cout << "-> ";

				input = maze_user->check_integer_input(input);

				switch (input) {
				case 1:
					cout << "Returning to main menu." << endl;
				default:
					break;
				}

				break;

			case 2:
				cout << "- Maze and Player combinations -" << endl;
				cout << "An interesting combination for the mazes and players is when two entrances appear directly next to each other." << endl;
				cout << "Each player generates their own route to the finish (as expected) but sometimes the routes will overlap." << endl;
				cout << "If the routes are indeed the same, then the player who is one place behind will always lose to the player in front." << endl;
				cout << "This is not only because they will get there first, but because one player will have to skip a turn as they cannot move past another player" << endl;
				cout << "- - - - - - - - - -" << endl;
				cout << "The maze generation creates an interesting property. Any open space on the maze is reachable from any other open position." << endl;
				cout << "This means you could be stood in one corner and reach another corner from at least one route as long as both corners are open cells" << endl;
				cout << "This therefore means that no matter what, regardless of mathematical proof, every generated maze is solvable." << endl;
				cout << "- - - - - - - - - -" << endl;
				cout << "In addition to the maze being solvable every time, it is noteworthy that the smallest maze has one general solution." << endl;
				cout << "No matter where a player starts, it will merge into the same solution. However, if the maze is much much larger, more tunnels are created." << endl;
				cout << "This means that there are more solutions for the players to explore based on where they are on the maze" << endl;
				cout << "- - - - - - - - - -" << endl;

				cout << "Return?" << endl;
				cout << "(1) Yes" << endl;
				cout << "-> ";

				input = maze_user->check_integer_input(input);

				switch (input) {
				case 1:
					cout << "Returning to main menu." << endl;
				default:
					break;
				}

				break;

			case 3:
				cout << "Returning to main menu." << endl;
				break;

			default:
				break;
			}

			break;

		case 5:
			// generate random mazes
			cout << "Please provide a number for the width upper limit between 35 and 200: ";
			width_u = maze_user->check_integer_input(input);
			width_u < maze_user->get_width_l() ? width_u = maze_user->get_width_l() : width_u;
			width_u > maze_user->get_width_u() ? width_u = maze_user->get_width_u() : width_u;

			cout << "Please provide a number for the height upper limit between 12 and 100: ";
			height_u = maze_user->check_integer_input(input);
			height_u < maze_user->get_height_l() ? height_u = maze_user->get_height_l() : height_u;
			height_u > maze_user->get_height_u() ? height_u = maze_user->get_height_u() : height_u;

			cout << "Please provide a number for the player upper limit between 2 and 40: ";
			player_u = maze_user->check_integer_input(input);
			player_u < maze_user->get_players_l() ? player_u = maze_user->get_players_l() : player_u;
			player_u > maze_user->get_players_u() ? player_u = maze_user->get_players_u() : player_u;

			for (int i = 0; i < maze_user->get_maze_gen_amount(); i++) {
				generated_maze = maze_user->generate_random_mazes(generated_maze, width_u, height_u, player_u);
				finish_looping = maze_user->step_through_automatically(generated_maze);
				cout << "Starting next maze." << endl;
				this_thread::sleep_for(chrono::milliseconds(500));

			}

			break;

		case 6:
			// Exit the program
			cout << "Goodbye!" << endl;
			keep_running = false;
			break;
		default:
			// Try again
			cout << "That was not one of the options - try again." << endl;
			break;
		}
	}

	// Clean up memory
	delete maze_user;
	delete generated_maze;

}

std::string User::get_username() { return username; }
int User::get_warning_limit(){ return WRN_LIMIT; }
int User::get_height_l() { return height_lower_limit; };
int User::get_width_l() { return width_lower_limit; };
int User::get_players_l() { return player_lower_limit; };
int User::get_height_u() { return maze_width_upper; };
int User::get_width_u() { return maze_height_upper; };
int User::get_players_u() { return player_upper; };
int User::get_maze_gen_amount() { return MZE_GEN_AMNT; }