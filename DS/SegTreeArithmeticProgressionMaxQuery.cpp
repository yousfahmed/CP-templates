const int inf = 1e18;

struct line {
  int id, val;

  line(int i = 0, int val = inf) : id(i), val(val) {}

  int cross_after(const line &l) const {
    int up = l.val - val;
    int d = id - l.id;
    if (d < 0) {
      up *= -1, d *= -1;
    }
    int inter = up >= 0 ? (up + d - 1) / d : -((-up) / d);
    return inter > 0 ? inter : inf;
  }

  bool operator<(const line &other) const {
    if (val != other.val)
      return val < other.val;
    return id < other.id;
  }
};

struct kinetic_segment_tree {
#define LF (x*2+1)
#define RT (x*2+2)
#define md ((lx+rx) >> 1)
  int n;
  vector<int> melt;
  vector<line> t;
  vector<array<int, 2>> d;

  void upd(int x) {
    t[x] = max(t[LF], t[RT]);
    melt[x] = min({
                      melt[LF],
                      melt[RT],
                      t[LF].cross_after(t[RT])
                  });
  }

  void init(int x, int lx, int rx) {
    if (lx == rx) {
      t[x] = {lx, 0};
      return;
    }
    init(LF, lx, md);
    init(RT, md + 1, rx);
    upd(x);
  }

  kinetic_segment_tree(int n) : n(n) {
    t.resize(4 * n, {});
    d.resize(4 * n, {});
    melt.resize(4 * n, inf);
    init(0, 0, n - 1);
  }

  void push(int x, int lx, int rx) {
    if (lx != rx) {
      d[LF][0] += d[x][0];
      d[RT][0] += d[x][0];
      d[LF][1] += d[x][1];
      d[RT][1] += d[x][1];
    }
    t[x].val += t[x].id * d[x][0] + d[x][1];
    melt[x] -= d[x][0];
    d[x] = {0, 0};
  }

  void propagate(int x, int lx, int rx) {
    push(x, lx, rx);
    if (melt[x] > 0) {
      return;
    }
    if (lx != rx) {
      propagate(LF, lx, md);
      propagate(RT, md + 1, rx);
      upd(x);
    }
  }


public:

  void update(int l, int r, int a, int delta, int x = 0, int lx = 0, int rx = -1) {
    if (rx == -1)rx = n - 1;
    push(x, lx, rx);
    if (r < lx or rx < l) {
      return;
    }
    if (l <= lx and rx <= r) {
      d[x][0] = delta;
      d[x][1] = a;
      propagate(x, lx, rx);
      return;
    }
    int m = (lx + rx) / 2;
    update(l, r, a, delta, LF, lx, m);
    update(l, r, a, delta, RT, m + 1, rx);
    upd(x);
  }

  line query(int l, int r, int x = 0, int lx = 0, int rx = -1) {
    if (rx == -1)rx = n - 1;
    push(x, lx, rx);
    if (r < lx or rx < l) {
      return line(0, -inf);
    }
    if (l <= lx && rx <= r) {
      return t[x];
    }
    int m = (lx + rx) / 2;
    return max(
        query(LF, lx, m, l, r),
        query(RT, m + 1, rx, l, r)
    );
  }

};
