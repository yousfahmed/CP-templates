/* the max number of characters starting from the position  
i that coincide with the first characters of s  */
vector<int> Z(const string &s) {
  const int n = s.size();
  vector<int> z(n);
  for (int i = 1; i < n; i++) {
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
      z[i]++;
    }
  }
  return z;
}