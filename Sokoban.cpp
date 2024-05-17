#define UP "w"
#define DOWN "s"
#define LEFT "a"
#define RIGHT "d"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class coordinate {
	public:
		int x;
		int y;
};

class Player {
	public:
		coordinate pos;
};

class Box {
	public:
		coordinate pos;
		bool complete;
};

class End {
	public:
		coordinate pos;
};

class direction {
	public:
		int x;
		int y;
};

direction which_direction(string button) {
	direction dir;
	if (button == UP) {
		dir.x = -1;
		dir.y = 0;
	}
	else if (button == DOWN) {
		dir.x = 1;
		dir.y = 0;
	}
	else if (button == LEFT) {
		dir.x = 0;
		dir.y = -1;
	}
	else if (button == RIGHT) {
		dir.x = 0;
		dir.y = 1;
	}

	return dir;
}

int main() {
	bool first_line = true;
	int width, height;
	vector<string> map;

	string filename;
	cin >> filename;

	ifstream file;
	file.open(filename);
	while (!file.eof()) {
		string line;
		getline(file, line);
		if (first_line) {
			height = stoi(line.substr(0, line.find(' ')));
			width = stoi(line.substr(line.find(' ') + 1));
			first_line = false;
		}
		else {
			if (map.size() < height) {
				map.push_back(line);
			}
		}
	}
	file.close();

	Player player;
	vector<Box> boxes;
	vector<End> ends;
	vector<vector<bool>> is_this_end;
	vector<bool> temp;

	for (int a = 0; a < height; a++) {
		for (int b = 0; b < width; b++) {
			temp.push_back(false);
		}
		is_this_end.push_back(temp);
	}
	for (int a = 0; a < height; a++) {
		for (int b = 0; b < width; b++) {
			if (map[a][b] == '0') {
				player.pos.x = a;
				player.pos.y = b;
			}
			if (map[a][b] == '1') {
				boxes.push_back({ {a, b}, false });
			}
			if (map[a][b] == '2') {
				ends.push_back({ a, b });
				is_this_end[a][b] = true;
			}
		}
	}

	bool goal = false;
	string button;
	for (int j = 0; j < map.size(); j++) {
		cout << map[j] << endl;
	}
	cin >> button;
	while ((button != UP) && (button != DOWN) && (button != LEFT) && (button != RIGHT)) {
		cout << "Invalid input!" << endl;
		cin >> button;
	}

	while (!goal) {
		if (map[player.pos.x + which_direction(button).x][player.pos.y + which_direction(button).y] == '1') {
			// box
			int k = 0;
			for (k; k < boxes.size(); k++) {
				if ((player.pos.x + which_direction(button).x == boxes[k].pos.x) && (player.pos.y + which_direction(button).y == boxes[k].pos.y)) {
					break;
				}
			}	// k is the box's index
			if ((map[boxes[k].pos.x + which_direction(button).x][boxes[k].pos.y + which_direction(button).y] == '-') || (map[boxes[k].pos.x + which_direction(button).x][boxes[k].pos.y + which_direction(button).y] == '2')) {
				// if the box can be pushed
				if (map[boxes[k].pos.x + which_direction(button).x][boxes[k].pos.y + which_direction(button).y] == '2') {
					int t = 0;	// push to end
					for (t; t < ends.size(); t++) {
						if ((boxes[k].pos.x + which_direction(button).x == ends[t].pos.x) && (boxes[k].pos.y + which_direction(button).x == ends[t].pos.y)) {
							break;
						}
					}	// t is the end's index
					boxes[k].complete = true;
				}
				else if (map[boxes[k].pos.x + which_direction(button).x][boxes[k].pos.y + which_direction(button).y] == '-') {
					if (boxes[k].complete) {
						// push the box away from end
						boxes[k].complete = false;
					}
				}
				if (is_this_end[player.pos.x][player.pos.y]) {
					map[player.pos.x][player.pos.y] = '2';
				}
				else {
					map[player.pos.x][player.pos.y] = '-';
				}
				map[player.pos.x + which_direction(button).x][player.pos.y + which_direction(button).y] = '0';
				map[player.pos.x + 2 * which_direction(button).x][player.pos.y + 2 * which_direction(button).y] = '1';
				player.pos.x = player.pos.x + which_direction(button).x;
				player.pos.y = player.pos.y + which_direction(button).y;
				boxes[k].pos.x = boxes[k].pos.x + which_direction(button).x;
				boxes[k].pos.y = boxes[k].pos.y + which_direction(button).y;
			}
		}
		else if (map[player.pos.x + which_direction(button).x][player.pos.y + which_direction(button).y] != '/') {
			// road or end (not pushing a box)
			if (is_this_end[player.pos.x][player.pos.y]) {
				map[player.pos.x + which_direction(button).x][player.pos.y + which_direction(button).y] = '0';
				map[player.pos.x][player.pos.y] = '2';
				player.pos.x = player.pos.x + which_direction(button).x;
				player.pos.y = player.pos.y + which_direction(button).y;
			}
			else {
				map[player.pos.x + which_direction(button).x][player.pos.y + which_direction(button).y] = '0';
				map[player.pos.x][player.pos.y] = '-';
				player.pos.x = player.pos.x + which_direction(button).x;
				player.pos.y = player.pos.y + which_direction(button).y;
			}
		}
		/*
		else{		// wall (nothing to do)
		
		}
		*/

		system("cls");
		for (int j = 0; j < map.size(); j++) {
			cout << map[j] << endl;
		}
		bool all_complete = true;
		for (int p = 0; p < boxes.size(); p++) {
			if (boxes[p].complete == false) {
				all_complete = false;
				break;
			}
		}
		if (all_complete) {
			goal = true;
		}
		if (!goal) {
			cin >> button;
			while ((button != UP) && (button != DOWN) && (button != LEFT) && (button != RIGHT)) {
				cout << "Invalid input!" << endl;
				cin >> button;
			}
		}
	}

	cout << "You win!";
	return 0;
}