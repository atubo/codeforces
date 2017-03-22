// http://poj.org/problem?id=1149
// PIGS

#include <cstdio>
#include <cmath>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;
typedef long long LL;

struct Edge {
	int from, to, cap, flow, index;
	Edge(int from_, int to_, int cap_, int flow_, int index_) :
		from(from_), to(to_), cap(cap_), flow(flow_), index(index_) {}
};

struct PushRelabel {
	int N;
	vector<vector<Edge> > G;
	vector<LL> excess;
	vector<int> dist, active, count;
	queue<int> Q;

	PushRelabel(int N_) : N(N_), G(N_), excess(N_), dist(N_), active(N_), count(2*N_) {}

	void AddEdge(int from, int to, int cap) {
		G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
		if (from == to) G[from].back().index++;
		G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
	}

	void Enqueue(int v) { 
		if (!active[v] && excess[v] > 0) { active[v] = true; Q.push(v); } 
	}

	void Push(Edge &e) {
		int amt = int(min(excess[e.from], LL(e.cap - e.flow)));
		if (dist[e.from] <= dist[e.to] || amt == 0) return;
		e.flow += amt;
		G[e.to][e.index].flow -= amt;
		excess[e.to] += amt;    
		excess[e.from] -= amt;
		Enqueue(e.to);
	}

	void Gap(int k) {
		for (int v = 0; v < N; v++) {
			if (dist[v] < k) continue;
			count[dist[v]]--;
			dist[v] = max(dist[v], N+1);
			count[dist[v]]++;
			Enqueue(v);
		}
	}

	void Relabel(int v) {
		count[dist[v]]--;
		dist[v] = 2*N;
		for (int i = 0; i < (int)G[v].size(); i++) 
			if (G[v][i].cap - G[v][i].flow > 0)
				dist[v] = min(dist[v], dist[G[v][i].to] + 1);
		count[dist[v]]++;
		Enqueue(v);
	}

	void Discharge(int v) {
		for (int i = 0; excess[v] > 0 && i < (int)G[v].size(); i++) Push(G[v][i]);
		if (excess[v] > 0) {
			if (count[dist[v]] == 1) 
				Gap(dist[v]); 
			else
				Relabel(v);
		}
	}

	LL GetMaxFlow(int s, int t) {
		count[0] = N-1;
		count[N] = 1;
		dist[s] = N;
		active[s] = active[t] = true;
		for (int i = 0; i < (int)G[s].size(); i++) {
			excess[s] += G[s][i].cap;
			Push(G[s][i]);
		}

		while (!Q.empty()) {
			int v = Q.front();
			Q.pop();
			active[v] = false;
			Discharge(v);
		}

		LL totflow = 0;
		for (int i = 0; i < (int)G[s].size(); i++) totflow += G[s][i].flow;
		return totflow;
	}
};

const int MAXN = 110, MAXM = 1010;
int adj[MAXN][MAXN], house[MAXM], last[MAXM];
int M, N;
const int INF = 300000000;

int main() {
    scanf("%d %d", &M, &N);
    for (int i = 1; i <= M; i++) {
        scanf("%d", &house[i]);
    }
    int s = 0, t = N + 1;
    for (int i = 1; i <= N; i++) {
        int n;
        scanf("%d", &n);
        for (int j = 1; j <= n; j++) {
            int k;
            scanf("%d", &k);
            if (last[k] == 0) {
                adj[s][i] += house[k];
            } else {
                adj[last[k]][i] = INF;
            }
            last[k] = i;
        }
        int b;
        scanf("%d", &b);
        adj[i][t] = b;
    }

    PushRelabel pr(N+2);
    for (int i = 0; i <= N+1; i++) {
        for (int j = 0; j <= N+1; j++) {
            if (adj[i][j] > 0) {
                pr.AddEdge(i, j, adj[i][j]);
            }
        }
    }

    printf("%lld\n", pr.GetMaxFlow(0, N+1));

    return 0;
}
