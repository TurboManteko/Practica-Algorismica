#include "Player.hh"
/* 
 * Falta acabar de poner lo de matar siempre al zombie incluso si estas al lado de una unidad muerta
 * Falta añadir que huya de los que se estan conviertiendo
*/

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME TurboMantek2
#define F first
#define S second

const int INF = 1e9;
const int NEAREST_CELL = 30;

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
    const VI idird = {1, -1, 0, 0, -1, 1, 1, -1};
    const VI jdird = {0, 0, 1, -1, -1, 1, -1, 1};
    VVI dia_dist;


    bool is_ally(int i, int j) {
        return (cell(i, j).id != -1 and unit(cell(i, j).id).player == me());
    }

    bool posok(int i, int j) {
        return (pos_ok(i, j) and cell(i, j).type != Waste);
    }

    bool is_dead(int i, int j) {
        return (cell(i, j).id != -1 and unit(cell(i, j).id).type == Dead);
    }

    bool adjacent_zombie(int i, int j) {
        for (int d = 0; d < 8; ++d) {
            int ni = i + idird[d];
            int nj = j + jdird[d];
            if (pos_ok(ni, nj) and cell(ni, nj).id != -1 and unit(cell(ni, nj).id).type == Zombie) return true;
        }
        return false;
    }

    bool adjacent_to_anyone(int i, int j) {
        for (int d = 0; d < 8; ++d) {
            int ni = i + idird[d];
            int nj = j + jdird[d];
            if (pos_ok(ni, nj) and cell(ni, nj).id != -1 and not is_dead(ni, nj) and not is_ally(ni, nj)) return true;
        }
        return false;
    }

    bool adjacent_dead(int i, int j) {
        for (int d = 0; d < 8; ++d) {
            int ni = i + idird[d];
            int nj = j + jdird[d];
            if (pos_ok(ni, nj) and cell(ni, nj).id != -1 and unit(cell(ni, nj).id).type == Dead) return true;
        }
        return false;
    }

    bool adjacent_to_me(int i, int j, const VI& units, int& u) {
        for (int id : units) {
            Pos np = Pos(unit(id).pos);
            int x = np.i;
            int y = np.j;
            if (abs(x - i) <= 1 and abs(y - j) <= 1) {
                u = id;
                return true;
            }
        }
        return false;
    }

    void bfs(VVI& Dist, queue<P> Q, int RANGE) {
        VVI dist(board_rows(), VI(board_cols(), -1));
        while (not Q.empty() and dist[Q.front().F][Q.front().S] < RANGE) {
            int i = Q.front().F;
            int j = Q.front().S;
            if (Dist[i][j] == 0) dist[i][j] = 0;
            Q.pop();
            for (int d = 0; d < 4; ++d) {
                int ni = i + idir[d];
                int nj = j + jdir[d];
                if (pos_ok(ni, nj) and cell(ni, nj).type != Waste and dist[ni][nj] == -1) {
                    dist[ni][nj] = 1 + dist[i][j];
                    if (Dist[ni][nj] == -1) Dist[ni][nj] = dist[ni][nj];
                    else Dist[ni][nj] = min(Dist[ni][nj], dist[ni][nj]);
                    Q.push(P(ni, nj));
                }
            }
        }
    }

    void kill_zombies(VVI& Dist, queue<P> Q, int RANGE) {
        VVI dist(board_rows(), VI(board_cols(), -1));
        while (not Q.empty() and dist[Q.front().F][Q.front().S] < RANGE) {
            int i = Q.front().F;
            int j = Q.front().S;
            if (Dist[i][j] == 0) dist[i][j] = 0;
            Q.pop();
            for (int d = 0; d < 8; ++d) {
                int ni = i + idird[d];
                int nj = j + jdird[d];
                if (pos_ok(ni, nj) and cell(ni, nj).type != Waste and dist[ni][nj] == -1) {
                    dist[ni][nj] = 1 + dist[i][j];
                    if (Dist[ni][nj] == -1) Dist[ni][nj] = dist[ni][nj];
                    else Dist[ni][nj] = min(Dist[ni][nj], dist[ni][nj]);
                    Q.push(P(ni, nj));
                }
            }
        }
    }

    void fastest_path(int x, int y, int xfi, int yfi) {
        VVI dist(board_rows(), VI(board_cols(), -1));
        dist[xfi][yfi] = 0;
        queue<P> Q;
        Q.push(P(xfi, yfi));
        while (not Q.empty() and dist[Q.front().F][Q.front().S] < NEAREST_CELL) {
            int i = Q.front().F;
            int j = Q.front().S;
            Q.pop();
            for (int d = 0; d < 4; ++d) {
                int ni = i + idir[d];
                int nj = j + jdir[d];
                if (pos_ok(ni, nj) and cell(ni, nj).type != Waste and dist[ni][nj] == -1) {
                    dist[ni][nj] = 1 + dist[i][j];
                    Q.push(P(ni, nj));
                }
            }
        }
        VI rand = random_permutation(4);
        for (int k = 0; k < 4; ++k) {
            Pos np = Pos(unit(cell(x, y).id).pos) + dirs[rand[k]];
            int ni = np.i;
            int nj = np.j;
            if (pos_ok(ni, nj) and cell(ni, nj).type != Waste and dist[ni][nj] < dist[x][y]) {
                cerr << "finally: unit at: " << x << ' ' << y << " moves to: " << dirs[rand[k]] << ' ' << idir[rand[k]] << ' ' << jdir[rand[k]] << endl;
                move(cell(x, y).id, dirs[rand[k]]);
                break;
            }
        }
    }

    void optimal_cell(int x, int y) {
        cerr << " so " << x << ' ' << y << " goes to: ";
        VVI dist(board_rows(), VI(board_cols(), -1));
        dist[x][y] = 0;
        queue<P> Q;
        Q.push(P(x, y));
        PP minim = PP(P(x, y), INF);
        while (not Q.empty() and dist[Q.front().F][Q.front().S] < NEAREST_CELL) {
            int i = Q.front().F;
            int j = Q.front().S;
            Q.pop();
            if (dist[i][j] != -1 and cell(i, j).owner != me() and dist[i][j] < minim.S)
                minim = PP(P(i, j), dist[i][j]);
            VI rand = random_permutation(4);
            for (int d = 0; d < 4; ++d) {
                int ni = i + idir[rand[d]];
                int nj = j + jdir[rand[d]];
                if (pos_ok(ni, nj) and cell(ni, nj).type != Waste and dist[ni][nj] == -1) {
                    dist[ni][nj] = 1 + dist[i][j];
                    Q.push(P(ni, nj));
                }
            }
        }
        cerr << "minim: " << minim.F.F << ' ' << minim.F.S << ' ' << minim.S << endl;
        fastest_path(x, y, minim.F.F, minim.F.S);
    }

    void nearest_cell(int x, int y) {
        cerr << "unit at: " << x << ' ' << y << "   ";

        VI rand = random_permutation(4);
        for (int d = 0; d < 4; ++d) {
            int i = x + idir[rand[d]];
            int j = y + jdir[rand[d]];
            if (pos_ok(i, j) and cell(i, j).type != Waste and cell(i, j).owner != me()) {
                cerr << x << ' ' << y << " to " << i << ' ' << j << " dir: " << dirs[rand[d]] << ' ' << rand[d] << endl;
                move(cell(x, y).id, dirs[rand[d]]);
                return;
            }
        }
        cerr << "unluigi ";
        optimal_cell(x, y);
    }

    void outplay_zombie(int i, int j, int id, const VVI& dist) {
        VI rand = random_permutation(4);
        for (int k = 0; k < 4; ++k) {
            Pos np = Pos(unit(id).pos) + dirs[rand[k]];
            int ni = np.i;
            int nj = np.j;
            cerr << k << ' ' << rand[k] << endl;
            if (posok(ni, nj) and not is_ally(ni, nj) and not is_dead(ni, nj)) {
                if (dia_dist[i][j] == 1 and dist[i][j] == 1 and dist[ni][nj] == 0) {
                    cerr << boolalpha << adjacent_zombie(i, j) << endl;
                    cerr << "zomb 1: unit at: " << i << ' ' << j << "move to: " << dirs[rand[k]] << endl;
                    move(id, dirs[rand[k]]);
                    break;
                }
                if (dia_dist[i][j] == 1 and dist[i][j] == 2 and dist[ni][nj] >= 2) {
                    cerr << "zomb 2: unit at: " << i << ' ' << j << "move to: " << dirs[rand[k]] << endl;
                    move(id, dirs[rand[k]]);
                    break;
                }
                if (dia_dist[i][j] == 2 and dist[i][j] == 2 and dist[ni][nj] > dist[i][j]) {
                    cerr << "zomb 3: unit at: " << i << ' ' << j << "move to: " << dirs[rand[k]] << endl;
                    move(id, dirs[rand[k]]);
                    break;
                }
                if (dia_dist[i][j] == 2 and dia_dist[ni][nj] >= 2 and dist[ni][nj] >= dist[i][j]) {
                    cerr << "zomb 4: unit at: " << i << ' ' << j << "move to: " << dirs[rand[k]] << endl;
                    move(id, dirs[rand[k]]);
                    break;
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
        queue<P> Zombies, Food, Enemies;
        VVI dist(board_rows(), VI(board_cols(), -1));
        dia_dist = VVI(board_rows(), VI(board_cols(), -1));
	bool passive = true;
        for (int i = 0; i < board_rows(); ++i)
            for (int j = 0; j < board_cols(); ++j)
                if (cell(i, j).food) {
                    dist[i][j] = 0;
                    Food.push(P(i, j));
                }
                else if (cell(i, j).id != -1 and unit(cell(i, j).id).player == -1) {
                    dist[i][j] = 0;
                    dia_dist[i][j] = 0;
                    Zombies.push(P(i, j));
                }
                else if (cell(i, j).id != -1 and unit(cell(i, j).id).player != me()) {
		    for (int k = 0; k < 4; ++k) {
			if (k == me()) continue;
			if (Alive.size() < 30 + alive_units(k).size()) passive = true;
			else { passive = false; break; }
		    }
		    if (not passive) {
			int id = -1;
			if (unit(cell(i, j).id).type == Dead and adjacent_to_me(i, j, Alive, id)) {
			    int x = unit(id).pos.i;
			    int y = unit(id).pos.j;
			    if (adjacent_to_anyone(x, y)) {
				cerr << "siuu " << x << ' ' << y << endl;
				continue;
			    }
			}
			dist[i][j] = 0;
			Enemies.push(P(i, j));
		    }
		    else {
			dist[i][j] = 99;
			Enemies.push(P(i, j));
		    }
		}
	int FOOD_RANGE = 20;
	int ZOMBIE_RANGE = 15;
	int ENEMY_RANGE = 2;

	cerr << boolalpha << "passive: " << passive << endl;
	bfs(dist, Food, FOOD_RANGE);
	bfs(dist, Zombies, ZOMBIE_RANGE);
	bfs(dist, Enemies, ENEMY_RANGE);
	kill_zombies(dia_dist, Zombies, ZOMBIE_RANGE);

	for (int i = 0; i < 60; ++i) {
	  for (int j = 0; j < 60; ++j) cerr << setw(3) << dist[i][j];
	  cerr << endl;
	  }
	  cerr << string(50, '-') << endl;
	  /*

	  for (int i = 0; i < 60; ++i) {
	  for (int j = 0; j < 60; ++j) cerr << setw(3) << dia_dist[i][j];
	  cerr << endl;
	  }*/

	for (int id : Alive) {
	    int i = unit(id).pos.i;
	    int j = unit(id).pos.j;
	    VI rand = random_permutation(4);
	    if (unit(id).rounds_for_zombie == -1 and dia_dist[i][j] <= 2 and dia_dist[i][j] != -1)
		outplay_zombie(i, j, id, dist);
	    for (int k = 0; k < 4; ++k) {
		Pos np = Pos(unit(id).pos) + dirs[rand[k]];
		int ni = np.i;
		int nj = np.j;
		if (posok(ni, nj) and not is_ally(ni, nj)) {
		    if (dist[ni][nj] < dist[i][j]) {
			cerr << "food unit at: " << i << ' ' << j << "move to: " << dirs[rand[k]] << endl;
			move(id, dirs[rand[k]]);
			break;
		    }
		}
	    }
	    nearest_cell(i, j);
	}
    }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
