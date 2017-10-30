#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
 
#define eps 1e-5
#define boundary 1e6
 
typedef struct Node {
    float a, b, c;
    struct Node *next;
} Node;

Node *I[3]; // I[0] = I_0; I[1] = I_-; I[2] = I_+
 
Node *myInit(int a, int b, int c){
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->a = a;
    newNode->b = b;
    newNode->c = c;
    newNode->next = NULL;
    return newNode;
}
 
int isEqual(float a, float b){
    if(fabs(a - b)<eps) return 1;
    return 0;
}
 
void myInsert(int set, float a, float b, float c){
    I[set]->a++;
    Node *newNode = myInit(a, b, c);
    newNode->next = I[set]->next;
    I[set]->next = newNode;
    return;
}

void myDelete(int set, Node **ppre, Node **pre, Node **cur, int index){
    if(index == 0){ // Delete pre
    	(*ppre)->next = (*pre)->next;
		*pre = *ppre;
    }
    else{ // Delete cur
    	(*pre)->next = (*cur)->next;
		*cur = *pre, *pre = *ppre;
    }
	I[set]->a--;
    return;
}
 
bool myCompare2 (float i, float j){ return (i < j); }

int FindInter(float a1, float b1, float c1, float a2, float b2, float c2, float *x, float *y){
    float tmp = a1 * b2 - b1 * a2;
    if(!isEqual(tmp, 0)) *x = (b1 * c2 - c1 * b2) / tmp;
    else return 0;
 
    *y = (-1 * a1 * (*x) - c1) / b1;
    return 1;
}
 
float FindY(float x){
	float minY = -boundary-1, maxY = boundary+1;
	
    Node *cur = I[1]->next;
	if(cur != NULL) minY = (-1*cur->a*x - cur->c) / cur->b;

	cur = I[2]->next;
	if(cur != NULL) maxY = (-1*cur->a*x - cur->c) / cur->b;

	if(minY > maxY) return boundary + 1;
	else return minY;
}
 
float myPrune(float Xl, float Xr){
    float intersec[1<<14] = {0};
    int n = 0;
    Node *ppre, *pre, *cur;
    for(int i=1; i<3; i++){
        ppre = NULL, pre = I[i], cur = I[i]->next;
        if(cur==NULL) continue;
        while(cur->next!=NULL){
            ppre = pre, pre = cur, cur = cur->next;
            float x, y;
            if(FindInter(pre->a, pre->b, pre->c, cur->a, cur->b, cur->c, &x, &y)){
	    		int flag = -1*pre->a/pre->b > -1*cur->a/cur->b;
            	if(x > Xl && x < Xr) intersec[n++] = x;
				else if(x <= Xl){
					if(i==1){
						if(flag) myDelete(1, &ppre, &pre, &cur, 1);
						else myDelete(1, &ppre, &pre, &cur, 0);
					}
					else{ // i==2
						if(flag) myDelete(2, &ppre, &pre, &cur, 0);
						else myDelete(2, &ppre, &pre, &cur, 1);
					}
				}
				else{ // x >= Xr
					if(i==1){
						if(flag) myDelete(1, &ppre, &pre, &cur, 0);
						else myDelete(1, &ppre, &pre, &cur, 1);
					}
					else{ // i==2
						if(flag) myDelete(2, &ppre, &pre, &cur, 1);
						else myDelete(2, &ppre, &pre, &cur, 0);
					}
				}
       	    }
			else{
	    		int flag = -1*pre->c/pre->b > -1*cur->c/cur->b;
	    		if(i==1){
					if(flag) myDelete(1, &ppre, &pre, &cur, 1);
					else myDelete(1, &ppre, &pre, &cur, 0);
				}
				else{ // i==2
					if(flag) myDelete(2, &ppre, &pre, &cur, 0);
					else myDelete(2, &ppre, &pre, &cur, 1);
				}
			}
		}
    }

	if(I[1]->a <= 1 && I[2]->a <= 1){ // terminated
		if(Xr < Xl) return boundary + 1;
		float minY = boundary + 1;
		float x, y;
		int flag;

		if(I[1]->a==1 && I[2]->a==1){
			Node *line1 = I[1]->next, *line2 = I[2]->next;
			if(FindInter(line1->a, line1->b, line1->c, line2->a, line2->b, line2->c, &x, &y)){
				if(x > Xl && x < Xr){
					if(y < minY) minY = y;
				}
			}
		}

		y = FindY(Xr);
		if(y < minY) minY = y;
 
		y = FindY(Xl);
		if(y < minY) minY = y;
 
		return minY;
    }

    std::nth_element(intersec, intersec+n/2, intersec+n, myCompare2);
    float Xm = intersec[n/2];
    float Ax = Xm, Bx = Xm;
    float Ay = -boundary, By = boundary;
    float Smax = -boundary, Smin = boundary;
    float Tmax = -boundary, Tmin = boundary;
    for(cur=I[1]->next; cur!=NULL; cur=cur->next){
        float tmpY = (-1*cur->a*Xm - cur->c) / cur->b;
        if(tmpY > Ay){
            Ay = tmpY;
            Smax = Smin = -1*cur->a / cur->b;
        }
        else if(isEqual(tmpY, Ay)){
            float tmpM = -1*cur->a / cur->b;
            if(tmpM > Smax) Smax = tmpM;
            if(tmpM < Smin) Smin = tmpM;
        }
    }
    for(cur=I[2]->next; cur!=NULL; cur=cur->next){
        float tmpY = (-1*cur->a*Xm - cur->c) / cur->b;
        if(tmpY < By){
            By = tmpY;
            Tmax = Tmin = -1*cur->a / cur->b;
        }
        else if(isEqual(tmpY,By)){
            float tmpM = -1*cur->a / cur->b;
            if(tmpM > Tmax) Tmax = tmpM;
            if(tmpM < Tmin) Tmin = tmpM;
        }
    }
 
    if(Ay <= By || isEqual(Ay, By)){
        if((Smin <= 0 && 0 <= Smax) || isEqual(Smin, 0) || isEqual(Smax, 0)) return Ay;
        else if(Smax < 0) Xl = Xm;
        else if(Smin > 0) Xr = Xm;
    }
    else{
        if((Smax >= Tmin && Smin <= Tmax) || (isEqual(Smax, Tmin) && isEqual(Smin, Tmax))) return boundary + 1;
        else if(Smax < Tmin) Xl = Xm;
        else if(Smin > Tmax) Xr = Xm;
    }

    return myPrune(Xl, Xr);
}
 
int main(){
    int N;
    while(scanf("%d", &N)!=EOF){
        for(int i=0; i<3; i++) I[i] = myInit(0, 0, 0);
        while(N--){
            float a, b, c;
            scanf("%f %f %f", &a, &b, &c);
            if(b==0) myInsert(0, a, b, -1*c);
            else if(b<0) myInsert(1, a, b, -1*c);
            else myInsert(2, a, b, -1*c);
        }
 
        float Xl=-boundary, Xr=boundary;
        Node *cur;
        for(cur=I[0]->next; cur!=NULL; cur=cur->next){
            if(cur->a > 0){
                if(-1*cur->c / cur->a < Xr) Xr = -1*cur->c / cur->a;
            }
            else{
                if(-1*cur->c / cur->a > Xl) Xl = -1*cur->c / cur->a;
            }
        }
 
        float ans = myPrune(Xl, Xr);
        if(ans > boundary) printf("NA\n");
        else if(ans < -boundary) printf("-INF\n");
        else printf("%.6f\n", ans);
    }
    return 0;
}