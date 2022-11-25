#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME TurboMantek2
#define F first
#define S second

const int INF = 1e9;
const int ZOMBIE = 100;
const int FOOD  = 100;
const int ENEMY = 100;
const int NEAREST_CELL = 17;

using P = pair<int, int>;
using PP = pair<P, int>;
using VI = vector<int>;
using VVI = vector<VI>;
using VVVI = vector<VVI>;


struct PLAYER_NAME : public Player {

    /**
     * Factory: returns a new instance of this class.
     * Do not modify this function.
     */
    static Player* factory () {
        return new PLAYER_NAME;
    }

    /**
     * Types and attributes for your player can be defined here.
     */
    const vector<Dir> dirs = {Up, Down, Left, Right};
    const VI idir = {-1, 1, 0, 0};
    const VI jdir = {0, 0, -1, 1};
    /*const VI idird = {1, -1, 0, 0, -1, 1, 1, -1};
      const VI jdird = {0, 0, 1, -1, -1, 1, -1, 1};*/

    void bfs(VVI& Dist, queue<P> Q, int RANGE) {
        VVI dist(board_rows(), VI(board_cols(), -1));

        while (not Q.empty() and dist[Q.front().F][Q.front().S] < RANGE) {
            int i = Q.front().F;
            int j = Q.front().S;
            if (Dist[i][j] == 0) dist[i][j] = 0;
            Q.pop();
            VI rand = random_permutation(4);
            for (int d = 0; d < 4; ++d) {
                int ni = i + idir[rand[d]];
                int nj = j + jdir[rand[d]];
                if (pos_ok(ni, nj) and cell(ni, nj).type != Waste and (cell(ni, nj).id == -1 or unit(cell(ni, nj).id).type != Dead) and dist[ni][nj] == -1) {
                    dist[ni][nj] = 1 + dist[i][j];
                    if (Dist[ni][nj] == -1) Dist[ni][nj] = dist[ni][nj];
                    else Dist[ni][nj] = min(Dist[ni][nj], dist[ni][nj]);
                    Q.push(P(ni, nj));
                }
            }
        }
    }

    void optimal_cell(int x, int y) {
	cerr << " so " << x << ' ' << y << " goes to: ";
	VVI dist(board_rows(), VI(board_cols(), -1));
	dist[x][y] = 0;
	queue<PP> Q;
	Q.push(PP(P(x, y), -1));
	int Dir;
	while (not Q.empty() and dist[Q.front().F.F][Q.front().F.S] < 10) {
	    int i = Q.front().F.F;
	    int j = Q.front().F.S;
	    int dir = Q.front().S;
	    Q.pop();
	    VI rand = random_permutation(4);
	    if (dir != -1 and cell(i, j).owner != me()) cerr << "dir " << dirs[dir] << endl;
	    for (int d = 0; d < 4; ++d) {
		int ni = i + idir[rand[d]];
		int nj = j + jdir[rand[d]];
		if (pos_ok(ni, nj) and cell(ni, nj).type != Waste and dist[ni][nj] == -1) {
		    dist[ni][nj] = 1 + dist[i][j];
		    Q.push(PP(P(ni, nj), rand[d]));
		}
	    }
	}
	cerr << string(50, '-') << endl;
	for (int i = 0; i < 60; ++i) {
	    for (int x : dist[i]) cerr << setw(3) << x;
	    cerr << endl;
	}
	cerr << string(50, '-') << endl;
    }

    void nearest_cell(int x, int y) {
        cerr << "unit at: " << x << ' ' << y << "   ";

        VI rand = random_permutation(4);
        for (int d = 0; d < 4; ++d) {
            int i = x + idir[rand[d]];
            int j = y + jdir[rand[d]];
            if (pos_ok(i, j) and cell(i, j).type != Waste and cell(i, j).owner != me()) {
                cerr << x << ' ' << y << " to " << i << ' ' << j << " dir: " << dirs[rand[d]] << endl;
                move(cell(x, y).id, dirs[rand[d]]);
                return;
            }
        }
	cerr << "unluigi ";
	optimal_cell(x, y);
    }


    /**
     * Play method, invoked once per each round.
     */
    virtual void play () {
        double st = status(me());
        if (st >= 0.9) return;

        vector<int> Alive = alive_units(me());
        queue<P> Zombies, Food, Enemies;
        VVI dist(board_rows(), VI(board_cols(), -1));
        for (int i = 0; i < board_rows(); ++i)
            for (int j = 0; j < board_cols(); ++j)
                if (cell(i, j).food) {
                    dist[i][j] = 0;
                    Food.push(P(i, j));
                }
                else if (cell(i, j).id != -1 and unit(cell(i, j).id).player == -1) {
                    dist[i][j] = 0;
                    Zombies.push(P(i, j));
                }
                else if (cell(i, j).id != -1 and unit(cell(i, j).id).player != me()) {
                    dist[i][j] = 0;
                    Enemies.push(P(i, j));
                }
        int FOOD_RANGE = 1;
        int ZOMBIE_RANGE = 1;
        int ENEMY_RANGE = 1;

        bfs(dist, Food, FOOD_RANGE);
        bfs(dist, Zombies, ZOMBIE_RANGE);
        bfs(dist, Enemies, ENEMY_RANGE);
        for (int i = 0; i < 60; ++i) {
            for (int j = 0; j < 60; ++j) cerr << setw(3) << dist[i][j];
            cerr << endl;
        }

        for (int id : Alive) {
            int i = unit(id).pos.i;
            int j = unit(id).pos.j;
            VI rand = random_permutation(4);
            for (int k = 0; k < 4; ++k) {
                Pos np = Pos(unit(id).pos) + dirs[rand[k]];
                int ni = np.i;
                int nj = np.j;
                if (pos_ok(ni, nj) and cell(ni, nj).type != Waste and dist[ni][nj] < dist[i][j]) {
                    //cerr << i << ' ' << j << " Dir: " << dirs[rand[k]] << endl;
                    move(id, dirs[rand[k]]); break;
                }
            }
        }

        for (int id : Alive) {
            int i = unit(id).pos.i;
            int j = unit(id).pos.j;
            nearest_cell(i, j);
        }
    }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
