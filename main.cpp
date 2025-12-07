//
// Created by Manju Muralidharan on 11/22/25.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Directions for DFS (students must use these)
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

// ----------------------------------------------------------
// DO NOT MODIFY: Maze generation
// ----------------------------------------------------------
void generateMaze(vector<vector<int>>& maze, int N, int M) {
    srand(time(0));

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            int roll = rand() % 100;
            maze[r][c] = (roll < 70) ? 0 : 1;   // 0 = open, 1 = wall
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Choose a random boundary cell that is open
// ----------------------------------------------------------
pair<int,int> chooseBoundaryCell(const vector<vector<int>>& maze) {
    int N = maze.size();
    int M = maze[0].size();

    while (true) {
        int side = rand() % 4;
        int r, c;

        if (side == 0) {          // top row
            r = 0;
            c = rand() % M;
        } else if (side == 1) {   // bottom row
            r = N - 1;
            c = rand() % M;
        } else if (side == 2) {   // left column
            r = rand() % N;
            c = 0;
        } else {                  // right column
            r = rand() % N;
            c = M - 1;
        }

        if (maze[r][c] == 0) {
            return {r, c};
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the maze with S and E markers
// ----------------------------------------------------------
void printMaze(const vector<vector<int>>& maze,
               int ent_r, int ent_c,
               int exit_r, int exit_c)
{
    int N = maze.size();
    int M = maze[0].size();

    cout << "\nMaze:\n";
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            if (r == ent_r && c == ent_c) {
                cout << "S ";
            } else if (r == exit_r && c == exit_c) {
                cout << "E ";
            } else {
                cout << maze[r][c] << " ";
            }
        }
        cout << "\n";
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the reconstructed path
// Students must fill parent[][] correctly during DFS
// ----------------------------------------------------------
void printPath(pair<int,int> exitcell,
               const vector<vector<int>>& parent_r,
               const vector<vector<int>>& parent_c,
               int ent_r, int ent_c)
{
    int r = exitcell.first;
    int c = exitcell.second;

    vector<pair<int,int>> path;

    // Walk backward from exit to entrance
    while (!(r == ent_r && c == ent_c)) {
        path.push_back({r, c});
        int pr = parent_r[r][c];
        int pc = parent_c[r][c];
        r = pr;
        c = pc;
    }
    path.push_back({ent_r, ent_c});

    cout << "\nPath from entrance to exit:\n";
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << "(" << path[i].first << ", " << path[i].second << ")\n";
    }
}

// ----------------------------------------------------------
// STUDENTS IMPLEMENT DFS HERE
// Add arguments, return type, and logic
// ----------------------------------------------------------

bool dfs(int r, int c, const vector<vector<int>>& maze, vector<vector<bool>>& visited, vector<vector<int>>& parent_r,
vector<vector<int>>& parent_c, int exit_r, int exit_c) {

    //base case (exit found)
    //needs to be checked first because the recursive call steps into a new cell (I think)
    //and that cell could be in the exit already
    //can't do maze[nr][nc] == maze[exit_r][exit_c] because nr and nc aren't created until the loop
    if (r == exit_r && c == exit_c) {
        return true;
    }

    visited[r][c] = true; //marks the cells in general, and also marks the starting cell first

    //get new row and column combo to check
    //moves through all the possible directions, and updates nr and nc accordingly based on
    //values for each move in the dr and dc arrays
    for (int i = 0; i < 4; i++) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        //out of bounds check
        if (nr < 0 || nr >= maze.size() || nc < 0 || nc >= maze[0].size()) {
            continue; //do nothing and go to next i/try another direction
        }
        //^have to use maze.size() and maze[0].size(), not just maze.size() because
        //the number of rows and columns might not be equal, but each row will always be the
        //same length, so maze[0] gets that columns bound

        //already have a line for marking, but this will check for visited cells
        if (visited[nr][nc] == true) {
            continue;  //do nothing and go to next i/try another direction
        }

        //hit a wall
        if (maze[nr][nc] == 1) {
            continue; //do nothing and go to next i/try another direction
        }

        //helps keep track of how DFS reached a cell --> used to backtrack from exit to entrance
        parent_r[nr][nc] = r;
        parent_c[nr][nc] = c;

        //if the exit is found during the recursive call, this conditional will be true, and return true
        //which is passed to the dfs call stack
        if (dfs(nr, nc, maze, visited, parent_r, parent_c, exit_r, exit_c)) {
            return true;
        }
        //can't just call dfs(...) because then there's no check if the recursive dfs call
        //found the exit, and while there's an exit check, there's no check that the dfs call
        //itself returned true
    }
        return false; //returns false when the cell is surrounded by walls/after going
                      //up, down, left, and right only a wall or out of bounds was reached
    }

// ----------------------------------------------------------
// MAIN PROGRAM (students add DFS calls and logic)
// ----------------------------------------------------------
int main() {
    int N, M;

    cout << "Enter maze dimensions N M: ";
    cin >> N >> M;

    vector<vector<int>> maze(N, vector<int>(M));
    generateMaze(maze, N, M);

    // Pick entrance and exit
    pair<int,int> entrance = chooseBoundaryCell(maze);
    pair<int,int> exitcell = chooseBoundaryCell(maze);

    while (exitcell == entrance) {
        exitcell = chooseBoundaryCell(maze);
    }

    int ent_r = entrance.first;
    int ent_c = entrance.second;
    int exit_r = exitcell.first;
    int exit_c = exitcell.second;

    // Display the maze
    printMaze(maze, ent_r, ent_c, exit_r, exit_c);

    // Students must use these
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    vector<vector<int>> parent_r(N, vector<int>(M, -1));
    vector<vector<int>> parent_c(N, vector<int>(M, -1));

    // ------------------------------------------------------
    // STUDENT WORK:
    // Call your DFS, track visited, and fill parent_r and parent_c
    // ------------------------------------------------------
       bool found = dfs(ent_r, ent_c, maze, visited, parent_r, parent_c, exit_r, exit_c);

    // ------------------------------------------------------
    // STUDENT WORK:
    // If found, print the path
    // ------------------------------------------------------
       if (found) {
           printPath(exitcell, parent_r, parent_c, ent_r, ent_c);
       } else {
           cout << "\nNo path exists.\n";
       }

    return 0;
}