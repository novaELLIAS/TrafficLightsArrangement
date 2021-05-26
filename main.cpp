#pragma GCC optimize ("Ofast", 3)
#pragma pack (4)

//#define ALL_STATUS_CALC
#define USING_CROSSING_STRATEGY

#include <bits/stdc++.h>
#include <bits/extc++.h>

using namespace std;

class EdgeTable {
public:
    int *head, *too, *nxt, cnt=0;
    int *ind;

    inline void init (int n) {
        int tp = (n+1)*(n+1)<<2|1; ind = new int[tp];
        head = new int[tp], nxt = new int[tp*tp<<1], too = new int[tp*tp<<1], cnt=0;
        for (int i=0; i<tp; ++ i) head[i] = ind[i] = 0;
    }
    ~EdgeTable() {delete []head; delete []nxt; delete []too; delete []ind;}

    inline void addEdge (int fr, int to) {
        nxt[++ cnt] = head[fr], head[fr] = cnt, too[cnt] = to;
        nxt[++ cnt] = head[to], head[to] = cnt, too[cnt] = fr;
        ++ ind[fr]; ++ ind[to];
    }

};

class Graphic {
private:
    int n, *ioVia, *col, *ans;
    int *que, queLen;
    int solCnt=0, maxCol=0x7f, useCol;

    EdgeTable e;

    inline int getIndex(int a, int b) {return a*(n+1)+b;}
    inline bool isVaild (int x);
    inline void generateMap1();
    inline void generateMap2();
    inline bool junctionJudge(int fr, int to);

public:
    ~Graphic() {
        delete []ioVia; delete []col; delete []que; delete []ans;
    }

    inline void init (int x);
    void work (int x);
    inline void printSolution();
    inline bool getSortVal(int a, int b);
    inline bool solutionSortCmp(int a, int b);

} g;

inline bool Graphic::getSortVal(int a, int b) {
    return e.ind[que[a]] > e.ind[que[b]];
}

inline bool Graphic::solutionSortCmp(int a, int b) {
    return (ans[a]^ans[b])? (ans[a]<ans[b]):(a/(n+1)<b/(n+1));
}

inline bool cmp (int a, int b) {return g.getSortVal(a, b);}
inline bool solutionSort (int a, int b) {return g.solutionSortCmp(a, b);}

inline void Graphic::printSolution() {
    printf("Solution %d: \n", solCnt);
    int fr, to;
    std::sort(que+1, que+queLen+1, solutionSort);
    for (int i=1; i<=queLen; ++ i) {
        fr = que[i]/(n+1), to = que[i]%(n+1);
        if (to==fr+1 || (fr==n && to==1)) continue;
        printf("%d->%d: %d\n", fr, to, ans[que[i]]);
    } putchar('\n');
}

inline void Graphic::work (int x=1) {
    //printf("work: %d\n", x);
    if (x>queLen) {
        ++ solCnt;
        if (useCol<maxCol) {
            maxCol = useCol;
            for (int i=1; i<=queLen; ++ i) ans[que[i]] = col[que[i]];
        }
#ifdef ALL_STATUS_CALC
        return;
#else
        printSolution();
        exit(0);
#endif
    } int tmpCol;
    for (int i=1; i<=n; ++ i) {
        col[que[x]] = i, tmpCol = useCol, useCol = max(i, useCol);
        if (isVaild(x)) work(x+1);
        col[que[x]] = 0, useCol = tmpCol;
    }
}

inline void Graphic::generateMap2() {
    int fr1, to1, fr2, to2;
    for (auto i=0; i<queLen; ++ i) {
        for (auto j=i+1; j<queLen; ++ j) {
            fr1=que[i]/(n+1), to1=que[i]%(n+1), fr2=que[j]/(n+1), to2=que[j]%(n+1);
            if (junctionJudge(que[i], que[j])) {
                e.addEdge(que[i], que[j]);
            }
        }
    } std::sort(que+1, que+queLen+1, cmp);
    std::cout << "eCnt: " << e.cnt << endl;
}

inline bool Graphic::junctionJudge(int fr, int to) {
    double fr1=(double)((int)(fr/(n+1)))+0.5, to1=fr%(n+1);
    double fr2=(double)((int)(to/(n+1)))+0.5, to2=to%(n+1);
    if (fr1>to1) swap(fr1, to1); if (fr2>to2) swap(fr2, to2);
    if (fr1>fr2) swap(fr1, fr2), swap(to1, to2);
    return fr2<to1 && to2>to1;
}

inline void Graphic::generateMap1() {
    for (int fr=1; fr<=n; ++ fr) {
        for (int rs=fr+1; rs<=n; ++ rs) {
            for (int to=rs+1; to<=n; ++ to) {
                for (int ls=to+1; ls<=n; ++ ls) {
                    for (int i=0; i<4; ++ i) {
                        if (ioVia[fr]&1 && ioVia[to]&2) {
                            if (ioVia[ls]&1 && ioVia[rs]&2) e.addEdge(getIndex(fr, to), getIndex(ls, rs));
                            //if (ioVia[ls]&2 && ioVia[rs]&1) e.addEdge(getIndex(fr, to), getIndex(rs, ls));
                        }
                        if (ioVia[fr]&1 && ioVia[ls]&2) {
                            if (ioVia[fr]&2) {
                                if (ioVia[rs]&1) e.addEdge(getIndex(fr, ls), getIndex(rs, fr));
                                if (ioVia[to]&1) e.addEdge(getIndex(fr, ls), getIndex(to, fr));
                            }
                            if (ioVia[ls]&1) {
                                if (ioVia[rs]&2) e.addEdge(getIndex(fr, ls), getIndex(ls, rs));
                                if (ioVia[to]&2) e.addEdge(getIndex(fr, ls), getIndex(ls, to));
                            }
                        } swap(fr, rs), swap(fr, to), swap(fr, ls);
                    }
                }
            }
        }
    } std::sort(que+1, que+queLen+1, cmp);
    std::cout << "eCnt: " << e.cnt << endl;
}

inline void Graphic::init (int x) {
    n = x, ioVia = new int[n+1], col = new int[(n+1)*(n+1)<<1], e.init(n);
    ans = new int[(n+1)*(n+1)<<1], que = new int[(n+1)*(n+1)<<1], queLen = 0;
    for (int i=1; i<=n; ++ i) scanf("%d ", &ioVia[i]);
    for (int i=1; i<=n; ++ i) printf("%d %d; ", ioVia[i]&1, ioVia[i]&2);
    for (int i=1; i<=n; ++ i) {
        for (int j = i+1; j<=n; ++j) {
            if (ioVia[i]&1 && ioVia[j]&2) que[++queLen] = getIndex(i, j);
            if (ioVia[i]&2 && ioVia[j]&1) que[++queLen] = getIndex(j, i);
        }
    }
    cout << "quelen: " << queLen << endl;
    for (int i=1; i<queLen; ++ i) cout << que[i] << " "; puts("");

#ifdef USING_CROSSING_STRATEGY
    generateMap1();
#else
    generateMap2();
#endif
}

inline bool Graphic::isVaild (int x) {
    for (int i=e.head[que[x]]; i; i=e.nxt[i]) {
        //cout << "isValid: " << e.too[i] << endl;
        if (!(col[que[x]] ^ col[e.too[i]])) return false;
    } return true;
}

signed main () {
    freopen("../in.txt", "r", stdin);
    int n; scanf("%d", &n);
    g.init(n);
    puts("QwQ init fin.");
    g.work();
    g.printSolution();
    fclose(stdin);
}