#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME TurboMantek1
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
    const VI idir = {1, -1, 0, 0};
    const VI jdir = {0, 0, 1, -1};
    /*const VI idird = {1, -1, 0, 0, -1, 1, 1, -1};
    const VI jdird = {0, 0, 1, -1, -1, 1, -1, 1};*/

    void bfs_zombies(int x, int y, VVI& Dist) {
	//cerr << "zombie at: " << x << ' ' << y << endl;
	VVI dist(board_rows(), VI(board_cols(), -1));
	queue<P> Q;
	dist[x][y] = 0;
	Dist[x][y] = 0;
	Q.push(P(x, y));
	PP minim = PP(P(x, y), INF);
	while (not Q.empty() and dist[Q.front().F][Q.front().S] < ZOMBIE) {
	    int i = Q.front().F;
	    int j = Q.front().S;
	    Q.pop();
	    if (cell(i, j).id != -1 and unit(cell(i, j).id).player == me() and dist[i][j] < minim.S) {
		if (Dist[i][j] != -1) Dist[i][j] = min(Dist[i][j], dist[i][j]);
		minim = PP(P(i, j), dist[i][j]);
	    }
	    for (int d = 0; d < 4; ++d) {
		int di = i + idir[d];
		int dj = j + jdir[d];
		if (pos_ok(di, dj) and cell(di, dj).type != Waste and dist[di][dj] == -1) {
		    dist[di][dj] = dist[i][j] + 1;
		    if (Dist[di][dj] == -1) Dist[di][dj] = 1 + Dist[i][j];
		    else Dist[di][dj] = min(Dist[di][dj], dist[di][dj]);
		    Q.push(P(di, dj));
		}
	    }
	}
	/*cerr << "minim: " << minim.F.F << ' ' << minim.F.S << ' ' << minim.S << ' ';
	cerr << "Dist: " << Dist[minim.F.F][minim.F.S] << endl; */
    }

    void bfs_food(int x, int y, VVI& Dist) {
	//cerr << "food at: " << x << ' ' << y << endl;
	VVI dist(board_rows(), VI(board_cols(), -1));
	queue<P> Q;
	dist[x][y] = 0;
	Dist[x][y] = 0;
	Q.push(P(x, y));
	PP minim = PP(P(x, y), INF);
	while (not Q.empty() and dist[Q.front().F][Q.front().S] < FOOD) {
	    int i = Q.front().F;
	    int j = Q.front().S;
	    Q.pop();
	    if (cell(i, j).id != -1 and unit(cell(i, j).id).player == me() and dist[i][j] < minim.S) {
		if (Dist[i][j] != -1) Dist[i][j] = min(Dist[i][j], dist[i][j]);
		minim = PP(P(i, j), dist[i][j]);
	    }
	    for (int d = 0; d < 4; ++d) {
		int di = i + idir[d];
		int dj = j + jdir[d];
		if (pos_ok(di, dj) and cell(di, dj).type != Waste and dist[di][dj] == -1) {
		    dist[di][dj] = dist[i][j] + 1;
		    if (Dist[di][dj] == -1) Dist[di][dj] = 1 + Dist[i][j];
		    else Dist[di][dj] = min(Dist[di][dj], dist[di][dj]);
		    Q.push(P(di, dj));
		}
	    }
	}
	/*cerr << "minim: " << minim.F.F << ' ' << minim.F.S << ' ' << minim.S << ' ';
	cerr << "Dist: " << Dist[minim.F.F][minim.F.S] << endl; */
    }

    void bfs_enemies(int x, int y, VVI& Dist) {
	//cerr << "enemie of team " << unit(cell(x, y).id).player << "at: " << x << ' ' << y << endl;
	VVI dist(board_rows(), VI(board_cols(), -1));
	queue<P> Q;
	dist[x][y] = 0;
	Dist[x][y] = 0;
	Q.push(P(x, y));
	PP minim = PP(P(x, y), INF);
	while (not Q.empty() and dist[Q.front().F][Q.front().S] < ENEMY) {
	    int i = Q.front().F;
	    int j = Q.front().S;
	    Q.pop();
	    if (cell(i, j).id != -1 and unit(cell(i, j).id).player == me() and dist[i][j] < minim.S) {
		if (Dist[i][j] != -1) Dist[i][j] = min(Dist[i][j], dist[i][j]);
		minim = PP(P(i, j), dist[i][j]);
	    }
	    for (int d = 0; d < 4; ++d) {
		int di = i + idir[d];
		int dj = j + jdir[d];
		if (pos_ok(di, dj) and cell(di, dj).type != Waste and dist[di][dj] == -1) {
		    dist[di][dj] = dist[i][j] + 1;
		    if (Dist[di][dj] == -1) Dist[di][dj] = 1 + Dist[i][j];
		    else Dist[di][dj] = min(Dist[di][dj], dist[di][dj]);
		    Q.push(P(di, dj));
		}
	    }
	}
	/*cerr << "minim: " << minim.F.F << ' ' << minim.F.S << ' ' << minim.S << ' ';
	cerr << "Dist: " << Dist[minim.F.F][minim.F.S] << endl; */
    }
    
    bool is_nearest_cell(int x, int y, int a, int b, int distan) {
	VVI dist(board_rows(), VI(board_cols(), -1));
	queue<P> Q;
	dist[x][y] = 0;
	Q.push(P(x, y));
	PP minim = PP(P(x, y), INF);
	while (not Q.empty() and dist[Q.front().F][Q.front().S] < NEAREST_CELL) {
	    int i = Q.front().F;
	    int j = Q.front().S;
	    Q.pop();
	    if (cell(i, j).owner != me() and dist[i][j] < minim.S)
		minim = PP(P(i, j), dist[i][j]);
	    for (int d = 0; d < 4; ++d) {
		int di = i + idir[d];
		int dj = j + jdir[d];
		if (pos_ok(di, dj) and cell(di, dj).type != Waste and dist[di][dj] == -1) {
		    dist[di][dj] = dist[i][j] + 1;
		    Q.push(P(di, dj));
		}
	    }
	}
        return dist[a][b] < distan;
    }

    void bfs_nearest_cell(int x, int y) {
	VVI dist(board_rows(), VI(board_cols(), -1));
	queue<P> Q;
	dist[x][y] = 0;
	Q.push(P(x, y));
	PP minim = PP(P(x, y), INF);
	while (not Q.empty() and dist[Q.front().F][Q.front().S] < NEAREST_CELL) {
	    int i = Q.front().F;
	    int j = Q.front().S;
	    Q.pop();
	    if (cell(i, j).owner != me() and dist[i][j] < minim.S)
		minim = PP(P(i, j), dist[i][j]);
	    for (int d = 0; d < 4; ++d) {
		int di = i + idir[d];
		int dj = j + jdir[d];
		if (pos_ok(di, dj) and cell(di, dj).type != Waste and dist[di][dj] == -1) {
		    dist[di][dj] = dist[i][j] + 1;
		    Q.push(P(di, dj));
		}
	    }
	}
        cerr << "player at: " << x << ' ' << y;
        cerr << " minim: " << minim.F.F << ' ' << minim.F.S << ' ' << minim.S << ' ' << dist[minim.F.F][minim.F.S] << endl;
        for (Dir d : dirs) {
            VI rand = random_permutation(4);
            for (int i = 0; i < 4; ++i) cerr << rand[i] << ' ';
            cerr << endl;
            for (int k = 0; k < 4; ++k) {
                Pos np = Pos(x, y) + dirs[rand[k]];
                int ni = np.i;
                int nj = np.j;
                if (pos_ok(ni, nj) and cell(ni, nj).type != Waste and is_nearest_cell(ni, nj, minim.F.F, minim.F.S, minim.S)) {
                    cerr << "embeng" << endl;
                    cerr << x << ' ' << y << " Dir: " << dirs[rand[k]] << endl;
                    move(cell(x, y).id, dirs[rand[k]]); return;
                }
            }
        }
    }



    /**
     * Play method, invoked once per each round.
     */
    virtual void play () {
        double st = status(me());
        if (st >= 0.9) return;

        vector<int> Alive = alive_units(me());
        vector<int> Zombies = zombies();
        vector<Unit> units(Alive.size());
        vector<P> Food;
        for (unsigned int i = 0; i < Alive.size(); ++i)
            units[i] = unit(Alive[i]);
        for (int i = 0; i < board_rows(); ++i)
            for (int j = 0; j < board_cols(); ++j)
                if (cell(i, j).food) Food.push_back(P(i, j));

        VVI dist(board_rows(), VI(board_cols(), -1));
        //cerr << "passive " << passive << endl;
        for (P p : Food) bfs_food(p.F, p.S, dist);
        for (int id : Zombies) bfs_zombies(unit(id).pos.i, unit(id).pos.j, dist);
        if (Alive.size() > 35) {
            for (int k = 0; k < 4; ++k)
                if (k == me()) continue;
                else for (int id : alive_units(k)) bfs_enemies(unit(id).pos.i, unit(id).pos.j, dist);
        }

        /*for (int i = 0; i < 60; ++i) {
          for (int j = 0; j < 60; ++j) cerr << setw(3) << dist[i][j];
          cerr << endl;
          }*/

        for (int id : Alive) {
            int i = unit(id).pos.i;
            int j = unit(id).pos.j;
            vector<int> rand = random_permutation(4);
            if (dist[i][j] > NEAREST_CELL) bfs_nearest_cell(i, j);
            else {
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
        }
    }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
