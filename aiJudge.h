#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include "AcString.h"
#define fi first
#define se second

typedef  long long ULL;
typedef std::pair<int, int> PII;
const int INF = 0x3f3f3f3f;
const int N = 20, M = 15;
const int dx[] = { 1, 0, 1, 1 }, dy[] = { 0, 1, 1, -1 };//8联通方向偏移

//Zobrist初始化
void initZobrist();

class aiJudge {
public:
    typedef struct point {
        bool color;// 1--black,0--white
        int x, y,score;
        //有/无分数构造
        point(bool color, int x, int y)
        {
            this->color = color;
            this->x = x;
            this->y = y;
            this->score = 0;
        }
        point(bool color, int x, int y,int score)
        {
            this->color = color;
            this->x = x;
            this->y = y;
            this->score = score;
        }
    } point;

private:
    //攻防系数（1～10），默认1
    int ad_cfg;
    //ai下一波结果
    point aistep = {0,0,0};
    //Alpha-Beta剪枝的Max-Min博弈树搜索
    aiJudge::point alphaBetaPlay(std::vector<point>& state, bool nowcolor, int depth, int alpha, int beta,int maxdepth);
    //局面评估
    int evluateState(std::vector<point>& state, bool nextcolor);
    //获取某点在局面为g某点的8联通方向分数
    int getScore(bool color, int x, int y, int g[][N]);
    //获取某点的dir联通方向分数
    int getScoreInDir(bool color,int x,int y,std::vector<point> state,int dir);
    //判断坐标是否合法
    bool posIsLegal(int x, int y);
    //以下两方法用于启发式搜索
    int evluateStep(std::vector<point>& state, bool nowcolor);//usedful，but outdated！(5.0版本已经弃用)
    //用于获得当前层最优可落点并返回按分值从大到小排序后的节点
    std::stack<point> getPossibleNextStep(std::vector<point>& state, bool nowcolor);
    //获得当前点的ZorbristKey
    static ULL getZobristKey(std::vector<point> state);
public:
    //初始化，传入当前局面，该走的颜色，搜索深度
    aiJudge(std::vector<point>& state, bool nextcolor,int depthmax);
    ~aiJudge();
    //获取ai的走法
    point getStep(){
        return this->aistep;
    }
    void Ad_cfg(int x)
    {
        this->ad_cfg = x;
    }
};

