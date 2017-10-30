#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN (102)

typedef struct Node {
	int x;
	int y;
	int score;
	int edge[MAXLEN];
	int nEdge;
} Node;

typedef struct Answer {
	char str[MAXLEN];
	int count;
} Answer;

char s[MAXLEN], t[MAXLEN];
int ls, lt;
int L[MAXLEN][MAXLEN] = {0};

Node *stack[MAXLEN];
int stackL = 0;

Node bucket[MAXLEN][MAXLEN];
int nBucket[MAXLEN] = {0};

Answer answer[MAXLEN];
int nAns = 0, realnAns = 0;

int Max(int a, int b){
	return (a > b)? a: b;
}

int compare(const void *a, const void *b){
	Answer *A = (Answer *)a;
	Answer *B = (Answer *)b;
	return strcmp(A->str, B->str);
}

int main(){
	scanf("%s", s);
	scanf("%s", t);
	ls = strlen(s);
	lt = strlen(t);
	for(int i=1; i<=ls; i++){
		for(int j=1; j<=lt; j++){
			int l = 0;
			if(s[i-1] == t[j-1]) l = 1;
			L[i][j] = Max( Max(L[i-1][j], L[i][j-1]), l + L[i-1][j-1]);
		}
	}

	for(int score=L[ls][lt]; score>=1; score--){
		for(int i=1; i<=ls; i++){
			for(int j=1; j<=lt; j++){
				if(L[i][j]!=score || s[i-1]!=t[j-1]) continue;
				int flag = 0;
				if(!nBucket[score+1]){
					bucket[score][nBucket[score]].x = i;
					bucket[score][nBucket[score]].y = j;
					bucket[score][nBucket[score]].score = score;
					bucket[score][nBucket[score]].nEdge = 0;
					flag = 1;
				}
				for(int k=0; k<nBucket[score+1]; k++){
					if(i<bucket[score+1][k].x && j<bucket[score+1][k].y){
						if(flag==0){
							bucket[score][nBucket[score]].x = i;
							bucket[score][nBucket[score]].y = j;
							bucket[score][nBucket[score]].score = score;
							bucket[score][nBucket[score]].edge[0] = k;
							bucket[score][nBucket[score]].nEdge = 1;
							flag++;
						}
						else{
							int tmp = bucket[score][nBucket[score]].nEdge;
							bucket[score][nBucket[score]].edge[tmp] = k;
							bucket[score][nBucket[score]].nEdge++;
						}
					}
				}
				if(flag) nBucket[score]++;
			}
		}
	}

	stackL = -1;
	for(int i=0; i<nBucket[1]; i++){
		stackL++;
		stack[stackL] = &bucket[1][i];
	}
	while(stackL >= 0){
		Node *current = stack[stackL];
		stackL--;
		answer[nAns].str[current->score - 1] = s[current->x - 1];
		if(current->score==L[ls][lt]){
			realnAns++;
			int flag = 0;
			answer[nAns].str[L[ls][lt]] = '\0';
			for(int i=0; i<nAns; i++){
				if(strcmp(answer[i].str, answer[nAns].str)==0){
					flag = 1;
					answer[i].count++;
					break;
				}
			}
			if(!flag){
				answer[nAns].count = 1;
				nAns++;
				strcpy(answer[nAns].str, answer[nAns-1].str);
			}
			continue;
		}

		for(int i=0; i<current->nEdge; i++){
			stackL++;
			stack[stackL] = &bucket[current->score+1][current->edge[i]];
		}
	}

	qsort(answer, nAns, sizeof(answer[0]), compare);
	printf("%d %d\n", L[ls][lt], realnAns);
	for(int i=0; i<nAns; i++){
		for(int j=0; j<answer[i].count; j++){
			puts(answer[i].str);
		}
	}
	return 0;
}