
const int N = 2e5 + 5;

int n, q, val[N];
vector<int> adj[N];

int sz[N], p[N], dep[N];
int in[N], tp[N];
int timer = 1;

void dfs_sz(int u, int par) {
  sz[u] = 1, p[u] = par;
  for (int &v: adj[u])
    if (v != par) {
      dep[v] = dep[u] + 1;
      dfs_sz(v, u);
      sz[u] += sz[v];
    }
}

void dfs_hld(int u, int par, int top) {
  in[u] = timer++;
  tp[u] = top;
  seg.update(in[u], val[u]); /// update point
  int bg = -1, mx = -1;
  for (int v: adj[u]) {
    if (v != par and sz[v] > mx)
      mx = sz[v], bg = v;
  }

  if (bg == -1) return;

  dfs_hld(bg, u, top);

  for (int v: adj[u])
    if (v != par and v != bg)
      dfs_hld(v, u, v);
}

int query_path(int x, int y) {
  int ret = 0;
  while (tp[x] != tp[y]) {
    if (dep[tp[x]] < dep[tp[y]])
      swap(x, y);
    /// query range
    ret = max(ret, seg.query(in[tp[x]], in[x])); /// TODO
    x = p[tp[x]];
  }
  if (dep[x] > dep[y])
    swap(x, y);
  /// query range
  ret = max(ret, seg.query(in[x], in[y])); /// TODO
  return ret;
}

void update_path(int x, int y, int v) {
  while (tp[x] != tp[y]) {
    if (dep[tp[x]] < dep[tp[y]])
      swap(x, y);
    /// update range
    seg.update(in[tp[x]], in[x], v);
    x = p[tp[x]];
  }
  if (dep[x] > dep[y])
    swap(x, y);
  /// update range
  seg.update(in[x], in[y], v);
}
