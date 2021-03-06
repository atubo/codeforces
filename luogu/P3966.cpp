// https://www.luogu.org/problemnew/show/P3966
// [TJOI2013]单词

#include <bits/stdc++.h>
using namespace std;

class SuffixAutomaton {
public:
    static const int CHARSET_SIZE = 27;
    static const int MAXN = 1000210;
    struct Node {
        Node *ch[CHARSET_SIZE], *next;
        int max, posCnt;

        int getMin() {
            return next->max + 1;
        }
    private:
        Node(int max = 0, bool newSuffix = false):
            ch(), next(NULL), max(max), posCnt(newSuffix) {}
        friend SuffixAutomaton;
    };

    Node *_pool;
    Node *start, *last, *_curr;
    vector<Node*> topo;

    SuffixAutomaton() {
        _pool = new Node[MAXN * 2 + 1];
        _curr = _pool;
        start = last = alloc(0, false);
    }

    ~SuffixAutomaton() {
        delete[] _pool;
    }

    void build(const string &s) {
        for (char c: s) {
            extend(c - 'a');
        }
    }

    void calc() {
        toposort();
        for (int i = topo.size()-1; i > 0; i--) {
            Node *v = topo[i];
            v->next->posCnt += v->posCnt;
        }
    }

private:
    Node *alloc(int max, bool newSuffix) {
        return new (_curr++)Node(max, newSuffix);
    }

    Node *extend(int c) {
        Node *u = alloc(last->max + 1, true), *v = last;

        for (; v && !v->ch[c]; v = v->next) v->ch[c] = u;

        if (!v) {
            u->next = start;
        } else if (v->ch[c]->max == v->max + 1) {
            u->next = v->ch[c];
        } else {
            Node *n = alloc(v->max + 1, false), *o = v->ch[c];
            std::copy(o->ch, o->ch + CHARSET_SIZE, n->ch);
            n->next = o->next;
            o->next = u->next = n;
            for (; v && v->ch[c] == o; v = v->next) v->ch[c] = n;
        }
        last = u;
        return u;
    }

    void toposort() {
        static int buc[MAXN * 2 + 1];
        int max = 0;
        for (Node *p = _pool; p != _curr; p++) {
            max = std::max(max, p->max);
            buc[p->max]++;
        }
        for (int i = 1; i <= max; i++) buc[i] += buc[i-1];
        topo.clear();
        topo.resize(_curr - _pool);
        for (Node *p = _pool; p != _curr; p++) {
            topo[--buc[p->max]] = p;
        }
        std::fill(buc, buc + max + 1, 0);
    }
};

int N;

int main() {
    cin >> N;
    vector<string> words(N);
    string s;
    for (int i = 0; i < N; i++) {
        cin >> words[i];
        s += words[i];
        s.push_back('{');
    }
    SuffixAutomaton sam;
    sam.build(s);
    sam.calc();

    for (const string &w: words) {
        SuffixAutomaton::Node *node = sam.start;
        for (char c: w) {
            node = node->ch[c-'a'];
        }
        cout << node->posCnt << endl;
    }

    return 0;
}
