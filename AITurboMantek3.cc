#include "Player.hh"
#include <algorithm>

/* Falta rellenar mapa
 * Falta kill zombies siempre
 * Falta prioridades bien hechas
*/

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME TurboMantek3
#define F first
#define S second
#define MP make_pair
#define PB push_back

const int INF = 1e9;
const int RANGE = 20;

struct Node {
    int i, j;
    int dist;
    Dir dir;
};

using P = pair<int, int>;
using PN = pair<int, Node>;
using PP = pair<P, int>;
using VI = vector<int>;
using VVI = vector<VI>;
using VVVI = vector<VVI>;
using VP = vector<P>;
using VVP = vector<VP>;



struct PLAYER_NAME : public Player {

    /**
     * Factory: returns a new instance of this class.
     * Do not modify this function.
     */
    static Player* factory () {
        return new PLAYER_NAME;
    }

    bool posok(int i, int j) {
        return pos_ok(i, j) and cell(i, j).type != Waste;
    }

    bool is_dead(int i, int j) {
        return cell(i, j).id != -1 and unit(cell(i, j).id).type == Dead;
    }
    
    bool is_food(int i, int j) {
        return cell(i, j).food;
    }

    bool is_zombie(int i, int j) {
        return cell(i, j).id != -1 and unit(cell(i, j).id).type == Zombie;
    }

    bool is_enemy(int i, int j) {
        return cell(i, j).id != -1 and unit(cell(i, j).id).type != Zombie and unit(cell(i, j).id).player != me();
    }

    bool is_ally(int i, int j) {
        return cell(i, j).id != -1 and unit(cell(i, j).id).player == me();
    }

    bool is_cnv_enemy(int i, int j) {
        return is_enemy(i, j) and unit(cell(i, j).id).rounds_for_zombie != -1;
    }

    /**
     * Types and attributes for your player can be defined here.
     */
    const vector<Dir> dirs = {Up, Down, Left, Right};
    const VI idir = {-1, 1, 0, 0};
    const VI jdir = {0, 0, -1, 1};
    const VI idird = {1, -1, 0, 0, -1, 1, 1, -1};
    const VI jdird = {0, 0, 1, -1, -1, 1, -1, 1};
    VVP being_taken;

    static bool myorder(PN a, PN b) {
        if (a.F == b.F) return a.S.dist > b.S.dist;
        return a.F > b.F;
    }
    
    void bfs(int x, int y, int id, map<int, Dir>& commands) {
        cerr << x << ' ' << y << endl;
        VVI dist(board_rows(), VI(board_cols(), -1));
        queue<Node> Q;
        dist[x][y] = 0;
        bool first = true;
        Q.push({x, y, 0, dirs[1]});
        vector<PN> PQ;
        int minim_cell = INF;
        int minim_food = INF;
        while (not Q.empty() and Q.front().dist < RANGE) {
            Node node = Q.front();
            int i = Q.front().i;
            int j = Q.front().j;
            int distan = Q.front().dist;
            Dir dir = Q.front().dir;
            Q.pop();
            if (is_food(i, j) and dist[i][j] < minim_food) {
                if (being_taken[i][j].F == -1) {
                    being_taken[i][j].F = id;
                    being_taken[i][j].S = dist[i][j];
                    minim_food = dist[i][j];
                    PQ.PB(MP(0, node));
                }
                else if (id != being_taken[i][j].F) {
                    cerr << "ya ta pillao por: " <<  unit(being_taken[i][j].F).pos.i;
                    cerr << ' ' << unit(being_taken[i][j].F).pos.j << " at dist: " << being_taken[i][j].S << endl;
                    if (being_taken[i][j].S > dist[i][j]) {
                        cerr << "y encima lo cambio pringao por: " << x << ' '<< y << " at dist: " << dist[i][j] << endl;
                        int id_r = being_taken[i][j].F;
                        being_taken[i][j].F = id;
                        being_taken[i][j].S = minim_food = dist[i][j];
                        bfs(unit(id_r).pos.i, unit(id_r).pos.j, id_r, commands);
                        PQ.PB(MP(0, node));
                    }
                    else if (being_taken[i][j].S == dist[i][j]) PQ.PB(MP(0, node));
                }
            }
            else if (is_zombie(i, j)) PQ.PB(MP(1, node));
            else if (is_enemy(i, j)) PQ.PB(MP(2, node));
            else if (cell(i, j).owner != me() and dist[i][j] < minim_cell) {
                minim_cell = dist[i][j];
                PQ.PB(MP(3, node));
            }
            VI rand = random_permutation(4);
            for (int d = 0; d < 4; ++d) {
                int ni = i + idir[rand[d]];
                int nj = j + jdir[rand[d]];
                if (posok(ni, nj) and not is_ally(ni, nj) and dist[ni][nj] == -1) {
                    dist[ni][nj] = 1 + dist[i][j];
                    if (first) Q.push({ni, nj, dist[ni][nj], dirs[rand[d]]});
                    else Q.push({ni, nj, dist[ni][nj], dir});
                }
            }
            first = false;
        }


        sort(PQ.begin(), PQ.end(), myorder);
        for (int d = 0; d < (int)PQ.size(); ++d) {
            cerr << PQ[d].F << ' ' << PQ[d].S.i << ' ' << PQ[d].S.j << ' ' << PQ[d].S.dist << ' ' << PQ[d].S.dir << endl;
        }
        cerr << endl;

        pair<int, Dir> p;
        while (PQ.size() != 0) {
            int n = PQ.size();
            int k = PQ[n - 1].F;
            Node node = PQ[n - 1].S;
            cerr << PQ[n - 1].F << ' ' << PQ[n - 1].S.i << ' ' << PQ[n - 1].S.j << ' ' << PQ[n - 1].S.dist << ' ' << PQ[n - 1].S.dir << endl;
            PQ.pop_back();
            if (k == 1 and node.dist <= 12) {
                cerr << "zomb: unit at: " << x << ' ' << y << "moves to: " << node.i << ' ' << node.j << ' ' << node.dir << endl;
                commands[id] = node.dir;
                break;
            }
            else if (k == 2 and node.dist <= 2) {
                cerr << "enemy: unit at: " << x << ' ' << y << "moves to: " << node.i << ' ' << node.j << ' ' << node.dir << endl;
                commands[id] = node.dir;
                break;
            }
            else if (k == 0 and node.dist <= 20) {
                cerr << "food: unit at: " << x << ' ' << y << "moves to: " << node.i << ' ' << node.j << ' ' << node.dir << endl;
                commands[id] = node.dir;
                break;
            }
            else if (k == 3) {
                cerr << "fill: unit at: " << x << ' ' << y << " moves to: " << node.i << ' ' << node.j << ' ' << node.dir << endl;
                commands[id] = node.dir;
                break;
            }
        }
        cerr << string(20, '-') << endl;
        /*for (int i = 0; i < 60; ++i) {
            for (int j = 0; j < 60; ++j) cerr << setw(3) << dist[i][j];
            cerr <<  endl;
        }*/
    }

    /**
     * Play method, invoked once per each round.
     */
    virtual void play () {
        double st = status(me());
        if (st >= 0.9) return;

        VI Alive = alive_units(me());
        map<int, Dir> commands;
        being_taken = VVP(board_rows(), VP(board_cols(), P(-1, -1)));
        int FOOD_RANGE = 30;
        int ZOMBIE_RANGE = 12;
        int ENEMY_RANGE = 12;


        for (int id : Alive) {
            int i = unit(id).pos.i;
            int j = unit(id).pos.j;
            bfs(i, j, id, commands);
        }
        for (pair<int, Dir> p : commands) {
            cerr << "move: " << unit(p.F).pos.i << ' ' << unit(p.F).pos.j << " to: " << p.S << endl;
            move(p.F, p.S);
        }
        for (int i = 0; i < board_rows(); ++i) {
            for (int j = 0; j < board_rows(); ++j) {
                if (cell(i, j).food) {
                    cerr << "food at: " << i << ' ' << j << " being taken by: ";
                    if (being_taken[i][j].F != -1) cerr << unit(being_taken[i][j].F).pos.i << ' ' << unit(being_taken[i][j].F).pos.j << endl;
                    else cerr << "no one" << endl;
                }
            }
        }
    }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
