#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME TurboMantek1
#define F first
#define S second

const int INF = 1e9;

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

    Dir bfs_food_zombies(int x, int y) {
        cerr << "zombie at: " << x << ' ' << y << endl;
        VVI dist(board_rows(), VI(board_cols(), -1));
        queue<P> Q;
        dist[x][y] = 0;
        Q.push(P(x, y));
        PP minim = PP(P(x, y), INF);
        while (not Q.empty()) {
            int i = Q.front().F;
            int j = Q.front().S;
            if (cell(i, j).id != -1 and unit(cell(i, j).id).player == me() and dist[i][j] < minim.S)
                minim = PP(P(i, j), dist[i][j]);
            for (int d = 0; d < 4; ++d) {
                int di = i + idir[d];
                int dj = j + jdir[d];
                if (pos_ok(di, dj) and cell(di, dj).type != Waste and dist[di][dj] == -1)
                    dist[di][dj] = dist[i][j] + 1;
            }
        }
        cerr << minim.F.F << ' ' << minim.F.S << ' ' << minim.S << endl;
        for (int k = 0; k < 4; ++k) {
            Pos new_pos = Pos(minim.F.F, minim.F.S) + dirs[k];
            int i = new_pos.i; int j = new_pos.j;
            if (pos_ok(new_pos) and cell(i, j).type != Waste and dist[i][j] < minim.S)
                return dirs[k];
        }
        cerr << "sadge" << endl;
        return dirs[0];
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
        cerr << "Food at: " << endl;
        for (int i = 0; i < (int)Food.size(); ++i)
            cerr << Food[i].F << ' ' << Food[i].S << endl;
        cerr << string(20, '-') << endl;

        cerr << "At round" << round() << " player " << me() << " has " << Alive.size() << " alive units: ";
        for (auto id : Alive)
            cerr << id << " at pos " << unit(id).pos << ", ";
        cerr << endl;

        for (int id : Zombies) {
            Dir d = bfs_food_zombies(unit(id).pos.i, unit(id).pos.j);
            move(id, d);
        }
    }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
