// https://www.luogu.org/problem/show?pid=2224
// [HNOI2001]产品加工

#include <bits/stdc++.h>
using namespace std;

int dp[30010];
int t[6010][3];
int mx;

int readInt () {
	bool minus = false;
	int result = 0;
	char ch;
	ch = getchar();
	while (true) {
		if (ch == '-') break;
		if (ch >= '0' && ch <= '9') break;
		ch = getchar();
	}
	if (ch == '-') minus = true; else result = ch-'0';
	while (true) {
		ch = getchar();
		if (ch < '0' || ch > '9') break;
		result = result*10 + (ch - '0');
	}
	if (minus)
		return -result;
	else
		return result;
}

int main() {
    int N;
    //scanf("%d", &N);
    N = readInt();
    memset(dp, 0x7F, sizeof(dp));
    dp[0] = 0;
    for (int i = 0; i < N; i++) {
        t[i][0] = readInt();
        t[i][1] = readInt();
        t[i][2] = readInt();
    }
    for (int i = 0; i < N; i++) {
        int a, b, c;
        a = t[i][0] == 0 ? INT_MAX: t[i][0];
        b = t[i][1] == 0 ? INT_MAX: t[i][1];
        c = t[i][2] == 0 ? INT_MAX: t[i][2];
        //scanf("%d %d %d", &a, &b, &c);
        mx += min(min(a, b), c);
        for (int j = mx; j >= 0; j--) {
            if (b != INT_MAX) dp[j] += b;
            else dp[j] = 0x7F7F7F7F;
            if (j >= a) dp[j] = min(dp[j], dp[j-a]);
            if (j >= c) dp[j] = min(dp[j], dp[j-c]+c);
        }
    }

    int ans = INT_MAX;
    for (int i = 0; i <= mx; i++) {
        ans = min(ans, max(i, dp[i]));
    }
    printf("%d\n", ans);
    return 0;
}
