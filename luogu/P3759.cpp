// https://www.luogu.org/problemnew/show/P3759
// [TJOI2017]不勤劳的图书管理员

#include <bits/stdc++.h>
using namespace std;

using PII = pair<int, int>;

struct ChairmanInFenwick {
    int N;  // number of positions, index is 1-based
    int M;  // number of weights, index is 0-based
    int T_cnt;
    int16_t *root;
    int *val;
    int16_t *lc, *rc, *cnt;

    // cap is the number of elements to be added
    ChairmanInFenwick(int N_, int M_, int cap): N(N_), M(M_), T_cnt(1) {
        root = new int16_t[N+1]{};
        const int numOfNodes = cap * int(log2(N)+2) * int(log2(M)+2);
        val = new int[numOfNodes]{};
        lc = new int16_t[numOfNodes]{};
        rc = new int16_t[numOfNodes]{};
        cnt = new int16_t[numOfNodes]{};
    }

    ~ChairmanInFenwick() {
        delete[] root;
        delete[] val;
        delete[] lc;
        delete[] rc;
        delete[] cnt;
    }

    // add value t to weight node w at position x
    void add(int x, int w, int t) {
        for (int i = x; i <= N; i += lowbit(i)) {
            update(root[i], w, t, 0, M-1);
        }
    }

    // weight sum [l, r] for position [1, k]
    PII query(int k, int l, int r) {
        if (l > r) return make_pair(0, 0);
        int sum = 0;
        int c = 0;
        for (int j = k; j; j -= lowbit(j)) {
            auto p = querySingleTree(root[j], 0, M-1, l, r);
            sum += p.first;
            c += p.second;
        }
        return make_pair(sum, c);
    }

private:
    int lowbit(int x) {return x & -x;}

    void update(int16_t &now, int w, int t, int16_t l, int16_t r) {
        if (!now) now = T_cnt++;
        val[now] += t;
        cnt[now] += (t > 0 ? 1 : -1);
        if (l == r) return;
        int mid = (l + r) / 2;
        if (w <= mid) update(lc[now], w, t, l, mid);
        else          update(rc[now], w, t, mid+1, r);
    }

    PII querySingleTree(int node, int L, int R, int l, int r) {
        if (L >= l && R <= r) return make_pair(val[node], cnt[node]);
        if (L > r || R < l) return make_pair(0, 0);
        int sum = 0, c = 0;
        int mid = (L + R) / 2;
        if (l <= mid) {
            auto p = querySingleTree(lc[node], L, mid, l, r);
            sum += p.first;
            c += p.second;
        }
        if (mid < r) {
            auto p = querySingleTree(rc[node], mid+1, R, l, r);
            sum += p.first;
            c += p.second;
        }
        return make_pair(sum, c);
    }
};

const int MAXN = 50010;
int N, M;
int V[MAXN];
int pos[MAXN]; // book->position
int book[MAXN]; // position->book
int64_t res;

void update(ChairmanInFenwick &cf, int x, int y, int lo, int hi,
            int b, int f) {
   auto p = cf.query(y, lo, hi);
   auto q = cf.query(x, lo, hi);
   int d = p.first - q.first;
   int c = p.second - q.second;
   int up = d + c * b;
   res += f * up;
}


void solve(ChairmanInFenwick &cf, int x, int y) {
    int bx = book[x];
    int by = book[y];

    update(cf, x-1, y-1, 1, bx-1, bx, -1);
    update(cf, x-1, y-1, bx+1, N, bx, 1);
    update(cf, x, y-1, by+1, N, by, -1);
    update(cf, x, y-1, 1, by-1, by, 1);
    if (bx < by) res += V[bx] + V[by];
    else res -= V[bx] + V[by];
    cf.add(x, bx, -V[bx]);
    cf.add(y, by, -V[by]);
    cf.add(x, by, V[by]);
    cf.add(y, bx, V[bx]);
    swap(pos[bx], pos[by]);
    swap(book[x], book[y]);
    printf("%lld\n", res);
}

int main() {
    scanf("%d%d", &N, &M);
    for (int i = 1; i <= N; i++) {
        int a, v;
        scanf("%d%d", &a, &v);
        V[a] = v;
        pos[a] = i;
        book[i] = a;
    }
    ChairmanInFenwick cf(N, N+1, N+M);
    for (int i = 1; i <= N; i++) {
        cf.add(pos[i], i, V[i]);
    }
    for (int i = 0; i < M; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        if (x > y) swap(x, y);
        solve(cf, x, y);
    }
    return 0;
}
