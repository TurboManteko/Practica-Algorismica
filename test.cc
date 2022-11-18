
Dir bfs_nearest_better_cell(int i, int j) {
    VVI dist(board_rows(), VI(board_cols(), -1));
    VVI directions = { {1, -1}, {1, -1} };
    queue<Aresta> Q;
    dist[i][j] = 0;
    Aresta minim;
    minim.x = random(-1, 1) + i;
    minim.y = random(-1, 1) + j;
    minim.c = INF;
    while (not Q.empty()) {
        int x = Q.front().first;
        int y = Q.front().second;
        int c = Q.front().c;
        if (cell(x, y).owner != me()) minim = Q.front();
        if (pos_ok(x + 1, y) and cell(x + 1, j).owner != me() and dist[x][y] == -1) {
            dist[x + 1][y] = 1 + dist[x][y];
            Q.push(P(x + 1, y));
        }
        if (pos_ok(x - 1, y) and cell(x - i, y).owner != me() and dist[x][y] == -1) {
            dist[x - 1][y] = 1 + dist[x][y];
        }
    }
}


Dir decide(int i, int j) {
    int n = 60;
    int m = 60;
    if (i < n - 1 and cell(i + 1, j).owner != me()) return dirs[1];
    if (i > 0 and cell(i - 1, j).owner != me()) return dirs[0];
    if (j < m - 1 and cell(i, j + 1).owner != me()) return dirs[3];
    if (j > 0 and cell(i, j - 1).owner != me()) return dirs[2];
    Dir d = dirs[random(0, dirs.size() - 1)];
    Pos new_pos = unit(id).pos + d;
    while (not pos_ok(new_pos) and cell(new_pos).type == Waste)
        d = dirs[random(0, dirs.size() - 1)];
    return dirs[random(0, dirs.size() - 1)];
}
