#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME TurboManteko

const int INF = 1e9;

using P = pair<int, int>;
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

    
    /*
    P dfs_cell(int i, int j) {
        VVI dist(board_rows(), VI(board_cols(), -1));
        VVI directions = { {1, -1}, {1, -1} };
        queue<P> Q;
        dist[i][j] = 0;
        while (not Q.empty()) {
            int x = Q.front().first;
            int y = Q.front().second; Q.pop();
            if (pos_ok(i + 1, j)) {
                Cell cela = cell(i + 1, j);
                if (cela.type != Waste and cela.owner != me() and dist[i][j] == -1) {
                    dist[i + 1][j] = 1 + dist[i][j];
                    Q.push(P(i + 1, j));
                }
                if (cela.type !=
            }
        }
    }
    */

    Dir decide(int i, int j) {
        int n = 60;
        int m = 60;
        if (i < n - 1 and cell(i + 1, j).type != Waste and cell(i + 1, j).owner != me()) return dirs[1];
        if (i > 0 and cell(i - 1, j).type != Waste and cell(i - 1, j).owner != me()) return dirs[0];
        if (j < m - 1 and cell(i, j + 1).type != Waste and cell(i, j + 1).owner != me()) return dirs[3];
        if (j > 0 and cell(i, j - 1).type != Waste and cell(i, j - 1).owner != me()) return dirs[2];
        Dir d = dirs[random(0, dirs.size() - 1)];
        Pos new_pos = Pos(i, j) + d;
        while (not pos_ok(new_pos) and cell(new_pos).type == Waste) {
            d = dirs[random(0, dirs.size() - 1)];
            new_pos = Pos(i , j) + d;
        }
        return d;
    }

    /**
     * Play method, invoked once per each round.
     */
    virtual void play () {
        double st = status(me());
        if (st >= 0.9) return;

        vector<int> alive = alive_units(me());
        vector<Unit> units(alive.size());
        for (unsigned int i = 0; i < alive.size(); ++i)
            units[i] = unit(alive[i]);

        cerr << "At round" << round() << " player " << me() << " has " << alive.size() << " alive units: ";
        for (auto id : alive)
            cerr << id << " at pos " << unit(id).pos << ", ";
        cerr << endl;

        for (int id : alive) {
            Unit u = unit(id);
            int i = u.pos.i;
            int j = u.pos.j;
            Dir d = decide(i, j);
            move(id, d);
        }


    }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
