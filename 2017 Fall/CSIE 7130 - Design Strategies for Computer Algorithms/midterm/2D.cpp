#include "bits/stdc++.h"
using namespace std;
#define ERR 0.000001

vector<double> xs;
double xl = INT_MIN, xr = INT_MAX;

struct Node {
    double max;
    double min;
};

struct Point {
    double x;
    double y;
};

class Line {
public:
    int a;                             
    int b;
    int c;
    double m;
    class Line* prev;
    class Line* next;
    friend class DLinkedList;
};

Point intersect(Line* p, Line* q) {
    Point point, emptyPoint = {0.0, 0.0};
    double det = p->a * q->b - q->a * p->b;
    point.x = (p->c * q->b - q->c * p->b) / det;
    point.y = (p->a * q->c - q->a * p->c) / det;

    if (det != 0) return point;
    else return emptyPoint;
}

class DLinkedList {
public:
    DLinkedList();                              // constructor
    ~DLinkedList();                             // destructor
    bool empty() const;                         // is list empty?
    void add(Line* v, int a, int b, int c);     // insert new line before v
    void addFront(int a, int b, int c);         // add to front of list
    void addBack(int a, int b, int c);          // add to back of list
    void remove(Line* v);                       // remove line v
    void removeFront();                         // remove from front
    void removeBack();                          // remove from back
    Line* getFront();                           // get front element
    Line* getBack();                            // get back element    
    void printLine();                           // print the list
private:                                        // list sentinels
    Line* head;
    Line* tail;
};

DLinkedList::DLinkedList() {                    // constructor
    head = new Line;                            // create sentinels
    tail = new Line;
    head->next = tail;                          // have them point to each other
    tail->prev = head;  
}

DLinkedList::~DLinkedList() {                   // destructor
    while (!empty()) removeFront();             // remove all but sentinels
    delete head;                                // remove the sentinels
    delete tail;
}

bool DLinkedList::empty() const { return (head->next == tail); }

void DLinkedList::add(Line* v, int a, int b, int c) {
    Line* u = new Line; 
    u->a = a; 
    u->b = b; 
    u->c = c; 
    u->m = (double) -a / b;
    u->next = v;
    u->prev = v->prev;
    v->prev->next = u;
    v->prev = u;
}

void DLinkedList::addFront(int a, int b, int c) { add(head->next, a, b, c); }
void DLinkedList::addBack(int a, int b, int c) { add(tail, a, b, c); }

void DLinkedList::remove(Line* v) {
    Line* u = v->prev;
    Line* w = v->next;
    u->next = w;
    w->prev = u;
    delete v;
}

void DLinkedList::removeFront() { remove(head->next); }
void DLinkedList::removeBack() { remove(tail->prev); }

Line* DLinkedList::getFront() { return head->next; }
Line* DLinkedList::getBack() { return tail->prev; }    

void DLinkedList::printLine() {
    Line *now = head->next;
    while (now != tail) { 
        printf("%3dx + %3dy <= %3d, m = %2.4f\n", now->a, now->b, now->c, now->m);
        now = now->next;
    }
    printf("=====End of printLine()=====\n");
}

void prune(DLinkedList& lines, char c) {
    Line* now = lines.getFront();
    while (now != lines.getBack()->next && now->next != lines.getBack()->next) {
        Line* curr = now;
        if (now->next->next != lines.getBack()->next) now = now->next->next;
        else now = lines.getBack()->next;

        Point p = intersect(curr, curr->next);
        if (p.x && p.x >= xl && p.x <= xr) 
            xs.push_back(p.x);
        if (c == 'A') {
            if (p.x >= xr) {                                                                  // Delete the line with larger m
                if (curr->m < curr->next->m) lines.remove(curr->next);
                else if (curr->m > curr->next->m) lines.remove(curr);  
            } else if (p.x <= xl) {                                                           // Delete the line with less m
                if (curr->m > curr->next->m) lines.remove(curr->next);
                else if (curr->m < curr->next->m) lines.remove(curr);         
            }  
        } else if (c == 'B') {
            if (p.x >= xr) {                                                                  // Delete the line with less m
                if (curr->m > curr->next->m) lines.remove(curr->next);      
                else if (curr->m < curr->next->m) lines.remove(curr);            
            } else if (p.x <= xl) {                                                           // Delete the line with larger m
                if (curr->m < curr->next->m) lines.remove(curr->next);
                else if (curr->m > curr->next->m) lines.remove(curr);     
            }  
        }
    }
}

double assignValue(DLinkedList& lines, double xm, double val, char c) {
    Line *now = lines.getFront();
    double tmp = val;
    while (now != lines.getBack()->next) {
        double y = (now->c - now->a * xm) / now->b;
        if (y > tmp && c == 'A') tmp = y;
        if (y < tmp && c == 'B') tmp = y; 
        now = now->next;
    }
    return tmp;
}

Node getSlope(DLinkedList& lines, double x, double y) {
    Node tmp = {INT_MIN, INT_MAX};
    Line *now = lines.getFront();
    while (now != lines.getBack()->next) {
        if (abs(now->a * x + now->b * y - now->c) <= ERR) {
            if (now->m > tmp.max) tmp.max = now->m;
            if (now->m < tmp.min) tmp.min = now->m;
        }
        now = now->next;
    }
    return tmp;
}

int main() {
    int n;
    scanf("%d", &n);
    
    DLinkedList linesA;
    DLinkedList linesB;

    for (int i = 0; i < n; i++) {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);

        if (b == 0 && a > 0 && a < xr) xr = (double) c / a;
        else if (b == 0 && a < 0 && a > xl) xl = (double) c / a;
        else if (b < 0) linesA.addBack(a, b, c);
        else if (b > 0) linesB.addBack(a, b, c);
    }

    while (true) {
        // printf("==========Round[%d]==========\n", cnt);
        
        if (xr < xl) { printf("NA\n"); break; }
        if (linesA.empty()) { printf("empty: -INF\n"); break; }
        
        if (linesA.getFront() == linesA.getBack() && linesB.getFront() == linesB.getBack()) {
            Line* lastA = linesA.getFront();
            Line* lastB = linesB.getFront();
            Point p = intersect(lastA, lastB);
            if (lastA->m > 0) {
                if (lastA->m > lastB->m) {
                    if (p.x > xl) {
                        if (xl == INT_MIN) { printf("-INF\n"); break; }
                        else {
                            double y = (lastA->c - lastA->a * xl) / lastA->b;
                            printf("%d\n", (int) round(y));
                            break;
                        }
                    } else if (p.x < xl) { printf("NA\n"); break; }
                } else if (lastA->m < lastB->m) {
                    if (p.x < xl) {
                        double y = (lastA->c - lastA->a * xl) / lastA->b;
                        printf("%d\n", (int) round(y));
                        break;
                    } else if (p.x > xr) { printf("NA\n"); break; }
                    else { printf("%d\n", (int) round(p.y)); break; }
                } else if (lastA->m == lastB->m) {
                    double yA = lastA->c / lastA->b;
                    double yB = lastB->c / lastB->b;
                    if (yA > yB) { printf("NA\n"); break; }
                    else if (xl == INT_MIN) { printf("-INF\n"); break; }
                    else {
                        double y = (lastA->c - lastA->a * xl) / lastA->b;
                        printf("%d\n", (int) round(y));
                        break;
                    }
                }
            } else if (lastA->m < 0) {
                if (lastA->m < lastB->m) {
                    if (p.x < xr) {
                        if (xr == INT_MAX) { printf("-INF\n"); break; }
                        else {
                            double y = (lastA->c - lastA->a * xr) / lastA->b;
                            printf("%d\n", (int) round(y));
                            break;
                        }
                    } else if (p.x > xr) { printf("NA\n"); break; }
                } else if (lastA->m > lastB->m) {
                    if (p.x > xr) {
                        double y = (lastA->c - lastA->a * xr) / lastA->b;
                        printf("%d\n", (int) round(y));
                        break;
                    } else if (p.x < xl) { printf("NA\n"); break; }
                    else { printf("%d\n", (int) round(p.y)); break; }
                } else if (lastA->m == lastA->m) {
                    double yA = lastA->c / lastA->b;
                    double yB = lastB->c / lastB->b;
                    if (yA > yB) { printf("NA\n"); break; }
                    else if (xr == INT_MAX) { printf("-INF\n"); break; }
                    else {
                        double y = (lastA->c - lastA->a * xr) / lastA->b;
                        printf("%d\n", (int) round(y));
                        break;
                    }
                }
            }
        } 

        if (linesA.getFront() == linesA.getBack() && linesB.empty()) {
            Line* lastA = linesA.getFront();
            Line* lastB = linesB.getFront();
            Point p = intersect(lastA, lastB);
            if (lastA->m > 0) {
                if (xl == INT_MIN) { printf("-INF\n"); break; }
                else {
                    double y = (lastA->c - lastA->a * xl) / lastA->b;
                    printf("%d\n", (int) round(y));
                    break;
                }
            } else if (lastA->m < 0) {
                if (xr == INT_MAX) { printf("-INF\n"); break; }
                else {
                    double y = (lastA->c - lastA->a * xr) / lastA->b;
                    printf("%d\n", (int) round(y));
                    break;
                }
            }
        }

        double alpha_y = INT_MIN;
        double beta_y = INT_MAX;
        Node s = {0.0, 0.0};
        Node t = {0.0, 0.0};

        prune(linesA, 'A');
        prune(linesB, 'B');

        nth_element(xs.begin(), xs.begin() + xs.size() / 2, xs.end());
        double xm = xs[xs.size() / 2];
    
        alpha_y = assignValue(linesA, xm, alpha_y, 'A');
        beta_y = assignValue(linesB, xm, beta_y, 'B');

        s = getSlope(linesA, xm, alpha_y);
        t = getSlope(linesB, xm, beta_y);

        if (alpha_y <= beta_y && s.min <= s.max && s.max < 0) { xl = xm; }
        if (alpha_y <= beta_y && s.max >= s.min && s.min > 0) { xr = xm; }
        if (alpha_y <= beta_y && s.min <= 0 && 0 <= s.max) { printf("%d\n", (int) round(alpha_y)); break; }
        if (alpha_y > beta_y && s.max < t.min) { xl = xm; }
        if (alpha_y > beta_y && s.min > t.max) { xr = xm; }
        if (alpha_y > beta_y && s.max >= t.min && s.min <= t.max) { printf("NA\n"); break; }

        xs.clear();
        linesA.printLine();
        linesB.printLine();
    }
    return 0;
}