// https://www.luogu.org/problem/show?pid=2280
// [HNOI2003]激光炸弹

#include <bits/stdc++.h>
using namespace std;


template<typename V, typename D=V>
class SegmentTreeTD {
    struct Node {
        Node(): value(0), update(0),count(0) {}
        V value;
        D update;
        int count;
    };

public:
    SegmentTreeTD(const vector<V>& A, V zero_, D zeroUpdate_,
                  function<V(V, V)> combine_,
                  function<D(D, D)> accu_,
                  function<V(V, D, int)> apply_):
        N(A.size()), zero(zero_), zeroUpdate(zeroUpdate_),
        combine(combine_), accu(accu_), apply(apply_) {
        int nlog = 0;
        int n = N;
        while (n > 0) {
            n = (n >> 1);
            nlog++;
        }
        int msize = 2 * (1 << (nlog+1)) + 1;
        M = new Node[msize];
        for (int i = 1; i < msize; i++) M[i].update = zeroUpdate;
        initialize(1, 0, N-1, A);
    }

    ~SegmentTreeTD() {
        delete[] M;
    }

    void initialize(int node, int b, int e, const vector<V>& A) {
        if (b == e) {
            M[node].value = A[b];
            M[node].count = 1;
        }
        else {
            initialize(2*node, b, (b+e)/2, A);
            initialize(2*node+1, (b+e)/2+1, e, A);
            M[node].value = combine(eval(M[2*node]), eval(M[2*node+1]));
            M[node].count = M[2*node].count + M[2*node+1].count;
        }
    }

    // i, j inclusive
    V query(int i, int j) const {
        return query(1, 0, N-1, i, j);
    }

    void update(int i, int j, D d) {
        update(1, 0, N-1, i, j, d);
    }

private:
    const int N;
    const V zero;   // zero element for combine
    const D zeroUpdate; // zero for update

    function<V(V, V)> combine;
    function<D(D, D)> accu;
    function<V(V, D, int)> apply;

    Node* M;

    V eval(const Node& node) const {
        return apply(node.value, node.update, node.count);
    }

    V query(int node, int b, int e, int i, int j) const {
        assert(b <= e);
        if (i > e || j < b) {
            return zero;
        }

        if (M[node].update != zeroUpdate) {
            M[node].value = eval(M[node]);
            if (b != e) {
                M[2*node].update = accu(M[node].update, M[2*node].update);
                M[2*node+1].update = accu(M[node].update, M[2*node+1].update);
            }
            M[node].update = zeroUpdate;
        }

        if (b >= i && e <= j) {
            return M[node].value;
        }

        V p1 = query(2*node, b, (b+e)/2, i, j);
        V p2 = query(2*node+1, (b+e)/2+1, e, i, j);

        return combine(p1, p2);
    }

    void update(int node, int b, int e, int i, int j, D d) {
        if (i > e || j < b) {
            return;
        }
        if (b >= i && e <= j) {
            M[node].update = accu(d, M[node].update);
            return;
        }
        if (M[node].update != zeroUpdate) {
            M[2*node].update = accu(M[node].update, M[2*node].update);
            M[2*node+1].update = accu(M[node].update, M[2*node+1].update);
        }

        update(2*node, b, (b+e)/2, i, j, d);
        update(2*node+1, (b+e)/2+1, e, i, j, d);
        M[node].value = combine(eval(M[2*node]), eval(M[2*node+1]));
        M[node].update = zeroUpdate;
    }
};

using TI = tuple<int, int, int, int>;   // x, type, y, value

int main() {
    int N, R;
    scanf("%d%d", &N, &R);
    vector<TI> events;
    for (int i = 0; i < N; i++) {
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);
        events.push_back(make_tuple(max(0, x-R+1), 1, y, z));
        events.push_back(make_tuple(x+1, 0, y, z));
    }
    sort(events.begin(), events.end());
    vector<int> A(5010);

    auto comb = [](int x, int y) {return max(x, y);};
    auto accu = [](int x, int y) {return x + y;};
    auto appl = [](int x, int y, int s) {return x + y;};
    SegmentTreeTD<int, int> st(A, 0, 0, comb, accu, appl);

    int ret = 0;
    for (const TI &e : events) {
        int x, t, y, v;
        tie(x, t, y, v) = e;
        int lo = max(0, y-R+1);
        int hi = y;
        if (t == 0) {
            st.update(lo, hi, -v);
        } else {
            st.update(lo, hi, v);
            ret = max(ret, st.query(0, 5009));
        }
    }
    printf("%d\n", ret);
    return 0;
}
