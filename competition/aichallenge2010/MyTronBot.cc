#include "Map.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <cmath>
#include <queue>
#include <vector>
#include <cstdio>
#define DEBUG 0
#define INFO 1
#define INF  100000
#define NINF -100000

#ifdef _WIN32
#include <sys/timeb.h>
#include <sys/types.h>
#include <winsock.h>
#else
#include <sys/time.h>
#include <time.h>
#include <cstdio>
#endif

using namespace std;

double getTimeSec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + (double) tv.tv_usec * 1e-6;
}

typedef vector<vector<bool> > Wall;

static const int xmv[4] = {-1, 1, 0, 0};
static const int ymv[4] = { 0, 0,-1, 1};

struct P
{
  P(): x(0), y(0) {}
  P(int x, int y): x(x), y(y) {}
  int x, y;
};

struct N {
  int x, y, w;
  N(int x, int y, int w) : x(x), y(y), w(w) { }
  bool operator< (const N& lhs) const {
    if (lhs.w < w) return true;
    if (lhs.w != w) return false;
    if (lhs.x < x) return true;
    return false;
  }
};

static bool use_deeping = false;
static bool deeping_stop = false;
static double begin_time;
const double timeout = 0.91;

void bfs(
    const Wall& w,
    P& p,
    vector<vector<int> >& tb)
{
  const int width  = w.size();
  const int height = w[0].size();
  Wall visited(width, vector<bool>(height, false));
  priority_queue<N> pq;
  pq.push(N(p.x, p.y, 0));
  while (!pq.empty()) {
    N n = pq.top(); pq.pop();
    if (visited[n.x][n.y]) continue;
    visited[n.x][n.y] = true;
    for (int i=0; i<4; i++) {
      int xnew, ynew;
      xnew = n.x + xmv[i];
      ynew = n.y + ymv[i];
      if (visited[xnew][ynew]) continue;
      if (xnew < 0 || xnew >= width) continue;
      if (ynew < 0 || ynew >= height) continue;
      if (w[xnew][ynew]) continue;
      pq.push(N(xnew, ynew, n.w + 1));
      tb[xnew][ynew] = n.w + 1;
    }
  }
}

int evalSep(Wall& wall, P& me, int& phase)
{
  const int width = wall.size();
  const int height = wall[0].size();
  vector<vector<int> > meTb(width, vector<int>(height, 0));
  bfs(wall, me, meTb);
  int score = 0;
  for (int i=0; i<width; i++)
    for (int j=0; j<height; j++) {
      if (wall[i][j]) continue;
      if (i == me.x && j == me.y) continue;
      if (meTb[i][j] > 0) score++;
    }

  return score;
}

int eval(Wall& wall, P& me, P& op, int& phase)
{
  const int width = wall.size();
  const int height = wall[0].size();
  vector<vector<int> > meTb(width, vector<int>(height, 0));
  vector<vector<int> > opTb(width, vector<int>(height, 0));
  bfs(wall, me, meTb);
  bfs(wall, op, opTb);
  int score = 0;
  for (int i=0; i<width; i++)
    for (int j=0; j<height; j++) {
      if (i == me.x && j == me.y) continue;
      if (i == op.x && j == op.y) continue;
      if (wall[i][j]) continue;
      if (meTb[i][j] == 0 && opTb[i][j] >  0) { score--; continue; }
      if (meTb[i][j] >  0 && opTb[i][j] == 0) { score++; continue; }
      if (meTb[i][j] < opTb[i][j]) { score++; }
      else if (meTb[i][j] > opTb[i][j]) { score--; }
    }

  return score;
  //  return phase;
}

bool checkSeparated(const Map& m)
{
  const int width = m.Width();
  const int height = m.Height();
  vector<vector<bool> > wall(width, vector<bool>(height, false));
  for (int i=0; i<width; i++)
    for (int j=0; j<height; j++)
      if (m.IsWall(i, j)) wall[i][j] = true;
  P me(m.MyX(), m.MyY()), op(m.OpponentX(), m.OpponentY());
  vector<vector<int> > meTb(width, vector<int>(height, 0));
  vector<vector<int> > opTb(width, vector<int>(height, 0));
  bfs(wall, me, meTb);
  bfs(wall, op, opTb);
  for (int i=0; i<width; i++)
    for (int j=0; j<height; j++)
      if (meTb[i][j] != 0 && opTb[i][j] != 0) return false;

  return true;
}

int getMovable(Wall& wall, P& me, P& op)
{
  const int width = wall.size();
  const int height = wall[0].size();
  vector<vector<int> > meTb(width, vector<int>(height, 0));
  vector<vector<int> > opTb(width, vector<int>(height, 0));
  bfs(wall, me, meTb);
  bfs(wall, op, opTb);
  int cnt = 0;
  for (int i=0; i<width; i++)
    for (int j=0; j<height; j++) {
      if (i == me.x && j == me.y) continue;
      if (i == op.x && j == op.y) continue;
      if (wall[i][j]) continue;
      if (meTb[i][j] == 0 && opTb[i][j] >  0) { cnt++; continue; }
      if (meTb[i][j] >  0 && opTb[i][j] == 0) { cnt++; continue; }
      if (meTb[i][j] < opTb[i][j]) cnt++;
      else if (meTb[i][j] > opTb[i][j]) cnt++;
    }

  return cnt;
}

// upper bound for traverse depth
int countMovableWithP(const Map& m, const P& p)
{
  const int width = m.Width();
  const int height = m.Height();
  vector<vector<bool> > w(width, vector<bool>(height, false));
  for (int i=0; i<width; i++)
    for (int j=0; j<height; j++)
      if (m.IsWall(i, j)) w[i][j] = true;
  Wall visited(width, vector<bool>(height, false));

  int cnt = 0;
  priority_queue<N> pq;
  pq.push(N(p.x, p.y, 0));
  while (!pq.empty()) {
    N n = pq.top(); pq.pop();
    if (visited[n.x][n.y]) continue;
    visited[n.x][n.y] = true;
    for (int i=0; i<4; i++) {
      int xnew, ynew;
      xnew = n.x + xmv[i];
      ynew = n.y + ymv[i];
      if (visited[xnew][ynew]) continue;
      if (xnew < 0 || xnew >= width) continue;
      if (ynew < 0 || ynew >= height) continue;
      if (w[xnew][ynew]) continue;
      pq.push(N(xnew, ynew, n.w + 1));
      cnt++;
    }
  }

  return cnt;
}

int countMovable(const Map& m)
{
  const int width = m.Width();
  const int height = m.Height();
  vector<vector<bool> > wall(width, vector<bool>(height, false));
  for (int i=0; i<width; i++)
    for (int j=0; j<height; j++)
      if (m.IsWall(i, j)) wall[i][j] = true;
  P me(m.MyX(), m.MyY()), op(m.OpponentX(), m.OpponentY());
  return getMovable(wall, me, op);
}

bool isCollide(P& me, P& op)
{
  return (me.x == op.x && me.y == op.y);
}

int minmaxMin(Wall&, P&, P&, P&, int, int&, int&);

// me -> max
int minmaxMax(
    Wall& wall,
    P& me, P& me_pre, P& op, int depth,
    int& phase,
    int& beta)
{
  int xnew, ynew, tmp_score = 0;
  int score = NINF + phase;
  int alpha = score;

  if (DEBUG) cerr << "MINMAX MAX for[] me=(" << me.x << "," << me.y << ")" << endl;
  for (int i=0; i<4; i++) {
    xnew = me.x; ynew = me.y;
    xnew += xmv[i]; ynew += ymv[i];
    P me_next(xnew, ynew);
    P tmp_me = me;
    P tmp_me_pre = me_pre;
    me = me_next;
    me_pre = tmp_me;
    if (DEBUG) cerr << "MINMAX MAX mv=" << i << " >>>>>>>>>>>>>>>>>>>>>>>" << endl;
    tmp_score = minmaxMin(wall, me, me_pre, op, depth, phase, alpha);
    if (DEBUG) cerr << "MINMAX MAX return val of minimize: <<<<<<<<<<<< mv=" << i << ", score=" << tmp_score << "" << endl;
    me = tmp_me;
    me_pre = tmp_me_pre;

    if (tmp_score > beta) return tmp_score; // purning


    if (tmp_score > score) {
      score = tmp_score;
      alpha = score;
    }
  }

  return score;
}

// op -> min
int minmaxMin(
    Wall& wall,
    P& me, P& me_pre, P& op_pre, int depth,
    int& phase,
    int& alpha)
{
  int xnew, ynew, mv = 0, tmp_score = 0;
  int score = INF - phase;
  int beta = score;

  P op;
  for (int i=0; i<4; i++) {
    xnew = op_pre.x; ynew = op_pre.y;
    xnew += xmv[i]; ynew += ymv[i];
    op.x = xnew; op.y = ynew;

    if (depth <= 0) {

      // deeping
      if (use_deeping && getTimeSec() - begin_time > timeout) {
        deeping_stop = true;
        return -1;
      }

      wall[me_pre.x][me_pre.y] = true;
      wall[op_pre.x][op_pre.y] = true;
      // draw: collide me and op
      if (isCollide(me, op)) tmp_score = 0;
      // draw: me -> wall, op -> wall
      else if (wall[me.x][me.y] && wall[op.x][op.y]) tmp_score = 0;
      // win: me -> !wall, op -> wall
      else if (!wall[me.x][me.y] && wall[op.x][op.y]) tmp_score = INF - phase;
      // lose: me -> wall, op -> !wall
      else if (wall[me.x][me.y] && !wall[op.x][op.y]) tmp_score = NINF + phase;
      // no win, no lose, no draw
      else tmp_score = eval(wall, me, op, phase);

      // leaf node debug print
      if (DEBUG) cerr << "DEPTH ZERO " << tmp_score << "me=(" << me.x << "," << me.y << ")" << endl;
      wall[me_pre.x][me_pre.y] = false;
      wall[op_pre.x][op_pre.y] = false;
      // eval
    } else {
      wall[me_pre.x][me_pre.y] = true;
      wall[op_pre.x][op_pre.y] = true;

      // ===== end conditions =====
      // draw: collide me and op
      if (isCollide(me, op)) tmp_score = 0;
      // draw: me -> wall, op -> wall
      else if (wall[me.x][me.y] && wall[op.x][op.y]) tmp_score = 0;
      // win: me -> !wall, op -> wall
      else if (!wall[me.x][me.y] && wall[op.x][op.y]) tmp_score = INF - phase;
      // lose: me -> wall, op -> !wall
      else if (wall[me.x][me.y] && !wall[op.x][op.y]) tmp_score = NINF + phase;
      // traverse
      else {
        phase++;
        P tmp_op_pre = op_pre;
        op_pre = op;

        if (DEBUG) cerr << "MINMAX MIN mv=" << i << " >>>>>>>>>>>>>>>>>>>>>>>" << endl;
        tmp_score = minmaxMax(wall, me, me_pre, op_pre, depth - 1, phase, beta);
        if (use_deeping && deeping_stop) return -1; // deeping

        if (DEBUG) cerr << "MINMAX MIN return val of maximize: <<<<<<< mv=" << i << ", score=" << tmp_score << "" << endl;
        op_pre = tmp_op_pre;
        phase--;
      }
      wall[me_pre.x][me_pre.y] = false;
      wall[op_pre.x][op_pre.y] = false;
    }

    if (tmp_score < alpha) return tmp_score;

    if (tmp_score < score) {
      score = tmp_score;
      beta  = score;
    }
  }
  if (DEBUG) cerr << "MINMAX MIN == minscore: (mv, score) = " << mv << ", " << score << endl;

  return score;
}

// me -> max
int engine(Map& map, int& phase, int depth, int& score)
{
  int xnew, ynew, mv = 0, tmp_score = 0, tmp_distance;
  Wall wall(map.Width(), vector<bool>(map.Height(), false));
  for (int i=0; i<map.Width(); i++)
    for (int j=0; j<map.Height(); j++)
      if (map.IsWall(i,j)) wall[i][j] = true;

  P me;
  P me_pre(map.MyX(), map.MyY());
  P op_pre(map.OpponentX(), map.OpponentY());
  int distance = INF;
  tmp_distance = distance;

  score = NINF + phase;
  int alpha = score;
  for (int i=0; i<4; i++) {
    xnew = me_pre.x; ynew = me_pre.y;
    xnew += xmv[i]; ynew += ymv[i];
    me.x = xnew; me.y = ynew;
    distance = abs(me.x - op_pre.x) + abs(me.y - op_pre.y);

    if (DEBUG) cerr << "ENGINE mv=" << i << " >>>>>>>>>>>>>>>>>>>>>>>" << endl;
    tmp_score = minmaxMin(wall, me, me_pre, op_pre, depth - 1, phase, alpha);
    if (use_deeping && deeping_stop) return -1; // deeping
    if (DEBUG) cerr << "ENGINE return val of minimize: mv=" << i << ", score=" << tmp_score << "<<<<<<<<<<<<" << endl;

    if (tmp_score == score && tmp_distance > distance) {
      tmp_distance = distance;
      mv = i;
      if (DEBUG) cerr << "ENGINE rewrite score: (mv, score) = " << mv << ", " << score << endl;
    }

    if (tmp_score > score) {
      tmp_distance = distance;
      score = tmp_score;
      alpha = score;
      mv    = i;
      if (DEBUG) cerr << "ENGINE rewrite score: (mv, score) = " << mv << ", " << score << endl;
    }
  }

  if (DEBUG) cerr << "ENGINE == maxscore: (mv, score) = " << mv << ", " << score << endl;
  return mv;
}

int maxmax(Wall& wall, P& me, int depth, int& phase)
{
  int xnew, ynew, mv = 0, tmp_score = 0;
  int score = NINF + phase;
  for (int i=0; i<4; i++) {
    wall[me.x][me.y] = true;
    me.x += xmv[i]; me.y += ymv[i];
    phase++;

    if (depth <= 0) {

      // deeping
      if (use_deeping && getTimeSec() - begin_time > timeout) {
        deeping_stop = true;
        return -1;
      }

      if (wall[me.x][me.y]) tmp_score = NINF + phase;
      else tmp_score = evalSep(wall, me, phase);
    } else {
      if (wall[me.x][me.y]) tmp_score = NINF + phase;
      else tmp_score = maxmax(wall, me, depth - 1, phase);

      if (use_deeping && deeping_stop) return -1; // deeping
    }

    phase--;
    me.x -= xmv[i]; me.y -= ymv[i];
    wall[me.x][me.y] = false;
    if (tmp_score > score) {
      score = tmp_score;
      mv    = i;
    }
  }

  return score;
}

int engineSep(Map& map, int& phase, int depth, int& score)
{
  int xnew, ynew, mv = 0, tmp_score = 0;
  Wall wall(map.Width(), vector<bool>(map.Height(), false));
  for (int i=0; i<map.Width(); i++)
    for (int j=0; j<map.Height(); j++)
      if (map.IsWall(i,j)) wall[i][j] = true;

  P me(map.MyX(), map.MyY());
  score = NINF + phase;
  for (int i=0; i<4; i++) {
    wall[me.x][me.y] = true;
    me.x += xmv[i]; me.y += ymv[i];
    phase++;

    if (depth <= 0) {
      if (wall[me.x][me.y]) tmp_score = NINF + phase;
      else tmp_score = evalSep(wall, me, phase);
    } else {
      if (wall[me.x][me.y]) tmp_score = NINF + phase;
      else {
        tmp_score = maxmax(wall, me, depth - 1, phase);
        if (use_deeping && deeping_stop) return -1; // deeping
      }
    }

    phase--;
    me.x -= xmv[i]; me.y -= ymv[i];
    wall[me.x][me.y] = false;

    if (tmp_score > score) {
      score = tmp_score;
      mv    = i;
    }
  }

  return mv;
}

// iterative deeping with timeout limitation
int engineDeeping(Map& map, int& phase, int depth, int& score)
{
  int maxmv = 0, mv, tmp_score, upper_bound;
  begin_time = getTimeSec();
  use_deeping = true;
  deeping_stop = false;

  while (true) {
    if (INFO) cerr << "DEEPING: " << depth << " begin - ";
    mv = engine(map, phase, depth, score);
    double end_time = getTimeSec();
    if (INFO) cerr << " end";
    if (INFO) cerr.setf(std::ios::fixed, std::ios::floatfield);
    if (INFO) cerr.precision(3);
    if (INFO) cerr << " (" << end_time - begin_time<< " sec)" << endl;
    if (deeping_stop) break;

    tmp_score = score;
    maxmv = mv;
    depth++;

    if (end_time - begin_time > timeout) break;

    P me(map.MyX(), map.MyY());
    upper_bound = countMovableWithP(map, me);
    if (depth > upper_bound) break;
  }

  score = tmp_score; // last score
  use_deeping = false;
  return maxmv;
}

int engineDeepingSep(Map& map, int& phase, int depth, int& score)
{
  int maxmv = 0, mv, tmp_score, upper_bound;
  begin_time = getTimeSec();
  use_deeping = true;
  deeping_stop = false;
  P me(map.MyX(), map.MyY());
  upper_bound = countMovableWithP(map, me);

  while (true) {
    if (INFO) cerr << "DEEPING: " << depth << " begin - ";
    mv = engineSep(map, phase, depth, score);
    double end_time = getTimeSec();
    if (INFO) cerr << " end";
    if (INFO) cerr.setf(std::ios::fixed, std::ios::floatfield);
    if (INFO) cerr.precision(3);
    if (INFO) cerr << " (" << end_time - begin_time<< " sec)" << endl;

    if (deeping_stop) break;
    tmp_score = score;
    maxmv = mv;
    depth++;
    if (end_time - begin_time > timeout) break;

    P me(map.MyX(), map.MyY());
    upper_bound = countMovableWithP(map, me);
    if (depth > upper_bound) break;
  }

  score = tmp_score;
  use_deeping = false;
  return maxmv;
}

void showInfo(int depth, int movable_cnt, int phase, bool separated)
{
  cerr << "depth:         " << depth << endl;
  cerr << "movable_cnt:   " << movable_cnt << endl;
}

void showPostInfo(string mv, int score, double t)
{
  cerr << "score:         " << score << endl;
  cerr << "move:          " << mv << endl;
  cerr.setf(std::ios::fixed, std::ios::floatfield);
  cerr.precision(3);
  cerr << "time:          " << t << endl;
}

int getDepth(int mcnt, bool separated)
{
  int n = 3;
  if      (mcnt > 400) n=2;
  else if (mcnt > 200) n=3;
  else if (mcnt > 170) n=3;
  else if (mcnt > 150) n=4;
  else if (mcnt > 100) n=5;
  else if (mcnt > 70)  n=6;
  else if (mcnt > 30)  n=6;
  else                 n=6;
  if (separated) n *= 2;
  return n;
}

string MakeMove(Map& map, int& phase)
{
  double begin_t, end_t;
  if (INFO) begin_t = getTimeSec();

  string mv;
  int score;
  int movable_cnt = countMovable(map);
  bool sep = checkSeparated(map);
  int depth = getDepth(movable_cnt, sep);
  if (INFO) showInfo(depth, movable_cnt, phase, false);

  /*
  // normal alpha-beta purning
  int maxmove = sep ?
      engineSep(map, phase, depth, score)
      : engine(map, phase, depth, score);
  */

  // with iterative deeping
  int maxmove = sep ?
      engineDeepingSep(map, phase, 2, score)
      : engineDeeping(map, phase, 2, score);

  switch (maxmove) {
    case WEST:  mv="WEST"; break;
    case EAST:  mv="EAST"; break;
    case NORTH: mv="NORTH"; break;
    default:    mv="SOUTH";
  }
  if (INFO) end_t = getTimeSec();
  if (INFO) showPostInfo(mv, score, end_t - begin_t);
  return mv;
}

int main() {
  int i = 1;
  Map map;

  while (true) {
    string mv = MakeMove(map, i);
    Map::MakeMove(mv);
    Map map_; map = map_;
    i++;
  }

  return 0;
}
