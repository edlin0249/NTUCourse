#include "bits/stdc++.h"
#define MAX 101
using namespace std;

typedef struct Block {
    int len;
    char c;
} Block;
Block dp[MAX][MAX];

char curStr[MAX];
vector<char*> ans;

void initDP() {                         
    for (int j = 0; j < MAX; j++) 
        dp[0][j].len = 0;
    for (int i = 1; i < MAX; i++)
        dp[i][0].len = 0;          
}                                      

void buildLCS(char A[], char B[], int n, int m) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (A[i - 1] == B[j - 1]) {
                dp[i][j].len = dp[i - 1][j - 1].len + 1;
                dp[i][j].c = A[i - 1];
            }
            else if (dp[i - 1][j].len > dp[i][j - 1].len)
                dp[i][j].len = dp[i - 1][j].len;
            else
                dp[i][j].len = dp[i][j - 1].len;
        }
    }
}

void getLCS(int idxA, int idxB, int len, char A[], char B[], int n, int m) {
    if (len == dp[n][m].len) {
        curStr[len] = '\0';
        char* tmpStr = (char *)malloc(sizeof(char) * (len + 1));
        strcpy(tmpStr, curStr);
        ans.push_back(tmpStr);
        return;
    }

    if (idxA == n || idxB == m) return;
    for (int i = idxA; i < n; i++) 
        for (int j = idxB; j < m; j++) 
            if (A[i] == B[j] && dp[i][j].len == len) {
                curStr[len] = A[i];
                getLCS(i + 1, j + 1, len + 1, A, B, n, m);
            }
}

void printAns(int len, int size) {
    printf("%d %d\n", len, size);
    for (int i = 0; i < size; i++)
        printf("%s\n", ans[i]);
}

int dictCmp(char* A, char* B) { return strcmp(A, B) < 0; }

int main() {
    char A[MAX], B[MAX];
    scanf("%s%s", A, B);
    int n = strlen(A);
    int m = strlen(B);

    initDP();
    buildLCS(A, B, n, m);
    getLCS(0, 0, 0, A, B, n, m);
    sort(ans.begin(), ans.end(), dictCmp);
    printAns(dp[n][m].len, ans.size());

    return 0;
}