#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME TurboManteko
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

    Dir better_cell(int x, int y) {
        VVI dist(board_rows(), VI(board_cols(), -1));
        queue<P> Q;
        dist[x][y] = 0;
        Q.push(P(x, y));
        PP minim = PP(P(x, y), INF);
        while (not Q.empty() and minim.S > 100) {
            int i = Q.front().F;
            int j = Q.front().S; Q.pop();
            if (pos_ok(i, j) and (cell(i, j).owner != me() or (cell(i, j).id != -1 and unit(cell(i, j).id).player != me())) and dist[i][j] < minim.S)
                minim = PP(P(i, j), dist[i][j]);
            if (pos_ok(i + 1, j) and cell(i + 1, j).type != Waste and dist[i + 1][j] == -1) {
                dist[i + 1][j] = 1 + dist[i][j];
                Q.push(P(i + 1, j));
            }
            if (pos_ok(i - 1, j) and cell(i - 1, j).type != Waste and dist[i - 1][j] == -1) {
                dist[i - 1][j] = 1 + dist[i][j];
                Q.push(P(i - 1, j));
            }
            if (pos_ok(i, j + 1) and cell(i, j + 1).type != Waste and dist[i][j + 1] == -1) {
                dist[i][j + 1] = 1 + dist[i][j];
                Q.push(P(i, j + 1));
            }
            if (pos_ok(i, j - 1) and cell(i, j - 1).type != Waste and dist[i][j - 1] == -1) {
                dist[i][j - 1] = 1 + dist[i][j];
                Q.push(P(i, j - 1));
            }
        }
        cerr << "bettercell " << x << ' ' << y << ' ' << minim.F.F << ' ' << minim.F.S;
        cerr << " minim.S " << minim.S << endl;
        int best = abs(minim.F.F - x) + abs(minim.F.S - y);
        for (Dir d : dirs) {
            Pos new_pos = Pos(x, y) + d;
            if (pos_ok(new_pos))
                if (cell(new_pos.i, new_pos.j).type != Waste)
                    if (best > abs(new_pos.i - minim.F.F) + abs(new_pos.j - minim.F.S)) {
                        cerr << "dir " << d << endl;
                        return d;
                    }
        }
        return dirs[0];
    }


    Dir bfs_food_zombies(int x, int y) {
        VVI dist(board_rows(), VI(board_cols(), -1));
        queue<P> Q;
        dist[x][y] = 0;
        Q.push(P(x, y));
        PP minim = PP(P(x, y), INF);
        while (not Q.empty()) {
            int i = Q.front().F;
            int j = Q.front().S; Q.pop();
            Cell c = cell(i, j);
            if (pos_ok(i, j) and (c.food or (c.id != -1 and unit(c.id).type != Dead and 
                            unit(c.id).type == Zombie)) and dist[i][j] < minim.S) 
                minim = PP(P(i, j), dist[i][j]);
            if (pos_ok(i + 1, j) and cell(i + 1, j).type != Waste and dist[i + 1][j] == -1) {
                dist[i + 1][j] = 1 + dist[i][j];
                Q.push(P(i + 1, j));
            }
            if (pos_ok(i - 1, j) and cell(i - 1, j).type != Waste and dist[i - 1][j] == -1) {
                dist[i - 1][j] = 1 + dist[i][j];
                Q.push(P(i - 1, j));
            }
            if (pos_ok(i, j + 1) and cell(i, j + 1).type != Waste and dist[i][j + 1] == -1) {
                dist[i][j + 1] = 1 + dist[i][j];
                Q.push(P(i, j + 1));
            }
            if (pos_ok(i, j - 1) and cell(i, j - 1).type != Waste and dist[i][j - 1] == -1) {
                dist[i][j - 1] = 1 + dist[i][j];
                Q.push(P(i, j - 1));
            }
        }
        cerr << "embeng " << x << ' ' << y << ' ' << minim.F.F << ' ' << minim.F.S << ' ';
        cerr << minim.S << ' ';
        if (cell(minim.F.F, minim.F.S).id != -1) cerr << unit(cell(minim.F.F, minim.F.S).id).player << endl;
        else cerr << "food" << endl;

        int best = abs(minim.F.F - x) + abs(minim.F.S - y);
        if (best < 12) {
            for (Dir d : dirs) {
                Pos new_pos = Pos(x, y) + d;
                if (pos_ok(new_pos)) {
                    int id_in_cell = cell(new_pos.i, new_pos.j).id;
                    if (cell(new_pos.i, new_pos.j).type != Waste)
                        if (best > abs(new_pos.i - minim.F.F) + abs(new_pos.j - minim.F.S)) {
                            cerr << "dir " << d << endl;
                            return d;
                        }
                }
            }
        }
        return better_cell(x, y);
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
            Dir d = bfs_food_zombies(unit(id).pos.i, unit(id).pos.j);
            move(id, d);
        }
    }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
