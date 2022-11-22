#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME TurboMantek0
#define F first
#define S second

const int INF = 1e9;
const int ZOMBIE = 100;
const int FOOD  = 100;
const int ENEMY = 100;

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

    void bfs_zombies(int x, int y, VVI& Dist) {
	//cerr << "zombie at: " << x << ' ' << y << endl;
	VVI dist(board_rows(), VI(board_cols(), -1));
	queue<P> Q;
	dist[x][y] = 0;
	Dist[x][y] = 0;
	Q.push(P(x, y));
	PP minim = PP(P(x, y), INF);
	while (not Q.empty() and dist[Q.front().F][Q.front().S] < 100) {
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
	while (not Q.empty() and dist[Q.front().F][Q.front().S] < 100) {
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
	while (not Q.empty() and dist[Q.front().F][Q.front().S] < 100) {
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



    /**
     * Play method, invoked once per each round.
     */
    virtual void play () {
	double st = status(me());
	if (st >= 0.9) return;

	vector<int> Alive = alive_units(me());
	vector<int> Zombies = zombies();
	vector<Unit> units(Alive.size());
	//vector<Unit> zombies_units(Z_id.size());
	vector<P> Food;
	for (unsigned int i = 0; i < Alive.size(); ++i)
	    units[i] = unit(Alive[i]);
	/*for (unsigned int i = 0; i < Zombies.size(); ++i)
	  zombies[i] = unit(Z_id[i]);
	  */
	for (int i = 0; i < board_rows(); ++i)
	    for (int j = 0; j < board_cols(); ++j)
		if (cell(i, j).food) Food.push_back(P(i, j));

	//cerr << "At round" << round() << " player " << me() << " has " << Alive.size() << " alive units: ";
	/*for (auto id : Alive)
	    cerr << id << " at pos " << unit(id).pos << ", ";
	cerr << endl;*/

	bool passive = true;
	VVI dist(board_rows(), VI(board_cols(), -1));
	for (int d = 0; d < 4; ++d) {
	    if (d == me()) continue;
	    if (alive_units(d).size() + 40 < Alive.size()) passive = false;
	    else { passive = true; break; }
	}
	//cerr << "passive " << passive << endl;
	if (passive) {
	    for (P p : Food) bfs_food(p.F, p.S, dist);
	    for (int id : Zombies) bfs_zombies(unit(id).pos.i, unit(id).pos.j, dist);
	}
	else {
	    for (int k = 0; k < 4; ++k)
		if (k == me()) continue;
		else for (int id : alive_units(k)) bfs_enemies(unit(id).pos.i, unit(id).pos.j, dist);
	}

	for (int id : Alive) {
	    int i = unit(id).pos.i;
	    int j = unit(id).pos.j;
	    vector<int> rand = random_permutation(4);
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
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
