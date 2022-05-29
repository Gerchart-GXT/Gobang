#include "aiJudge.h"

//黑、白棋的zobrist表，内存64bit随机数
ULL zobrist_black[N][N],zobrist_white[N][N];

//Key-val分值表
std::vector<Pattern> patterns = {
    { "11111",  50000 },
    { "011110", 4320 },
    { "011100", 720 },
    { "001110", 720 },
    { "011010", 720 },
    { "010110", 720 },
    { "11110",  720 },
    { "01111",  720 },
    { "11011",  720 },
    { "10111",  720 },
    { "11101",  720 },
    { "001100", 120 },
    { "001010", 120 },
    { "010100", 120 },
    { "000100", 20 },
    { "001000", 20 },
};

//zobrist-list
std::unordered_map<ULL,int> zobrist_list;
//Ac自动机初始化
AcString* acs = new AcString(patterns);

aiJudge::aiJudge(std::vector<point>& state, bool nextcolor,int depthmax)
{
    aistep  = alphaBetaPlay(state, nextcolor, 0, -INF, INF,depthmax);
}

aiJudge::~aiJudge()
{
}

int aiJudge::evluateState(std::vector<point>& state, bool color)
{
    //获取当前局面的key
    ULL nowkey = getZobristKey(state);
    //如果已有cache就直接返回
    if(zobrist_list.count(nowkey)){
        return color ? zobrist_list[nowkey]:-zobrist_list[nowkey];
    }
    //初始化棋盘记录数组g
    int nowcolor = color ? 1 : -1;
    int g[N][N];
    memset(g, 0, sizeof g);

    for (auto i : state)
        if (i.color)
            g[i.x][i.y] = 1;
        else if (!i.color)
            g[i.x][i.y] = -1;

    //对于新加入的点，先使用以新点为中心的8联通优化
    std::vector<point> befState;
    befState.assign(state.begin(),state.end()-1);
    //获取未加入新点前的key
    ULL befkey = getZobristKey(befState);
    auto nowpoint = state.back();
    //如果为加入的点已被cache，就在其基础上仅改动其8联通方向的分值
    if(zobrist_list.count(befkey)){
        int befval = color ? zobrist_list[befkey]:-zobrist_list[befkey];
        int x = nowpoint.x,y = nowpoint.y;
        int dir = -1;
        while (++dir < 4) {
            int sign = -1;
            for (int i = 1; i < 8; ++i) {
                int tx = x + sign * i * dx[dir], ty = y + sign * i * dy[dir];
                if (!posIsLegal(tx, ty)){
                    break;
                }
                if(g[tx][ty] == 0)
                    continue;
                if (g[tx][ty] == -nowcolor) {
                    befval=befval+getScoreInDir(!color,tx,ty,befState,dir)-getScoreInDir(!color,tx,ty,state,dir);
                }
                if (g[tx][ty] == nowcolor)
                    befval=befval-getScoreInDir(color,tx,ty,befState,dir)+getScoreInDir(color,tx,ty,state,dir);
            }
            sign = -sign;
            for (int i = 1; i < 8; ++i) {
                int tx = x + sign * i * dx[dir], ty = y + sign * i * dy[dir];
                if (!posIsLegal(tx, ty)){
                    break;
                }
                if(g[tx][ty] == 0)
                    continue;
                if (g[tx][ty] == -nowcolor) {
                    befval=befval+getScoreInDir(!color,tx,ty,befState,dir)-getScoreInDir(!color,tx,ty,state,dir);
                }
                if (g[tx][ty] == nowcolor)
                    befval=befval-getScoreInDir(color,tx,ty,befState,dir)+getScoreInDir(color,tx,ty,state,dir);
            }
        }
        befval+=getScore(color,nowpoint.x,nowpoint.y,g);
        if (color)
            zobrist_list[nowkey] = befval;
        else{
            zobrist_list[nowkey] = -befval;
        }
        return befval;
    }

    //如果未记录，就评估去全局
    int pointBLK = 0, pointWIT =0;

    for (auto i : state) {
        if (i.color == 1)
            pointBLK += getScore(1, i.x, i.y, g);
    }
    for (auto i : state) {
        if (i.color == 0)
            pointWIT += getScore(0, i.x, i.y, g);
    }
    int val = 0;
    if (color){
        val = pointBLK- pointWIT;
        zobrist_list[nowkey] = val;
    }
    else{
        val = pointWIT- pointBLK;
        zobrist_list[nowkey] = -val;
    }
    return val;
}
bool aiJudge::posIsLegal(int x, int y)
{
    return !(x < 1 || x > M || y < 1 || y > M);
}

//以下方法已被弃用
int aiJudge::evluateStep(std::vector<point>& state, bool nowcolor)
{
    int g[N][N];
    int check[N][N];
    memset(g, 0, sizeof g);
    memset(check, 0, sizeof check);
    for (auto i : state)
        if (i.color)
            g[i.x][i.y] = 1;
        else if (!i.color)
            g[i.x][i.y] = -1;
    auto t = state.back();
    return getScore(nowcolor, t.x, t.y, g);
}

int aiJudge::getScoreInDir(bool color,int x,int y,std::vector<point> state,int dir)
{
    int g[N][N];
    memset(g, 0, sizeof g);
    for (auto i : state)
        if (i.color)
            g[i.x][i.y] = 1;
        else if (!i.color)
            g[i.x][i.y] = -1;

    int ans = 0;
    int nowcolor = color ? 1 : -1;
    std::deque<int> S;
    S.push_back(1);
    int sign = -1;
    //获取该点dir方向的棋子排布
    for (int i = 1; i < 4; ++i) {
        int tx = x + sign * i * dx[dir], ty = y + sign * i * dy[dir];
        if (!posIsLegal(tx, ty)){
            S.push_front(2);
            break;
        }
        if (g[tx][ty] == 0) {
            S.push_front(0);
        }
        if (g[tx][ty] == nowcolor)
            S.push_front(1);
        if (g[tx][ty] == -nowcolor) {
            S.push_front(2);
            break;
        }
    }
    sign = -sign;
    for (int i = 1; i <4; ++i) {
        int tx = x + sign * i * dx[dir], ty = y + sign * i * dy[dir];
        if (!posIsLegal(tx, ty)){
            S.push_back(2);
            break;
        }
        if (g[tx][ty] == 0) {
            S.push_back(0);
        }
        if (g[tx][ty] == nowcolor)
            S.push_back(1);
        if (g[tx][ty] == -nowcolor) {
            S.push_back(2);
            break;
        }
    }
    //转为String
    std::string s = "";
    for(auto i:S){
        s+='0'+i;
    }
    //使用Ac自动机进行匹配
    std::vector<int> tmp = acs->ACSearch(s);
    for (int j = 0; j <(int)tmp.size(); j++) {
        ans += patterns[tmp[j]].score;
    }
    return ans;
}
int aiJudge::getScore(bool color, int x, int y, int g[][N])
{
    int ans = 0;
    int dir = -1;
    int nowcolor = color ? 1 : -1;
    //获取8联通方向的棋子排布
    while (++dir < 4) {
        std::deque<int> S;
        S.push_back(1);
        int sign = -1;
        for (int i = 1; i < 4; ++i) {
            int tx = x + sign * i * dx[dir], ty = y + sign * i * dy[dir];
            if (!posIsLegal(tx, ty)){
                S.push_front(2);
                break;
            }
            if (g[tx][ty] == 0) {
                S.push_front(0);
            }
            if (g[tx][ty] == nowcolor)
                S.push_front(1);
            if (g[tx][ty] == -nowcolor) {
                S.push_front(2);
                break;
            }
        }
        sign = -sign;
        for (int i = 1; i <4; ++i) {
            int tx = x + sign * i * dx[dir], ty = y + sign * i * dy[dir];
            if (!posIsLegal(tx, ty)){
                S.push_back(2);
                break;
            }
            if (g[tx][ty] == 0) {
                S.push_back(0);
            }
            if (g[tx][ty] == nowcolor)
                S.push_back(1);
            if (g[tx][ty] == -nowcolor) {
                S.push_back(2);
                break;
            }
        }
        std::string s = "";
        for(auto i:S){
            s+='0'+i;
        }
        //Ac自动机匹配
        std::vector<int> tmp = acs->ACSearch(s);
        for (int j = 0; j <(int)tmp.size(); j++) {
            ans += patterns[tmp[j]].score;
        }
    }

    return ans;
}
std::stack<aiJudge::point> aiJudge::getPossibleNextStep(std::vector<point>& state, bool nowcolor)
{
    const int dtx[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    const int dty[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
    std::set<PII> S, A;
    //将已有点存入集合A
    for (auto i : state)
        A.insert({ i.x, i.y });
    for (auto t : state) {
        for (int i = 0; i < 8; i++) {
            int tx = t.x + dtx[i], ty = t.y + dty[i];
            if (!posIsLegal(tx, ty))
                continue;
            if (A.count({ tx, ty }))
                continue;
            A.insert({ tx, ty });
            state.push_back({ nowcolor, tx, ty });
            //让下一步的当前子的val尽可能的大
            S.insert({ evluateState(state, nowcolor), tx * N + ty });
            state.pop_back();
        }
    }
    //将最后结果加入stack中（从小到大）
    std::stack<point> ans;
    for (auto i : S) {
        int x = i.se / N, y = i.se % N;
        ans.push({ nowcolor, x, y });
    }
    return ans;
}
aiJudge::point aiJudge::alphaBetaPlay(std::vector<point>& state, bool nowcolor, int depth, int alpha, int beta,int maxdepth)
{
    //到达底层返回
    if(depth == maxdepth){
        point nextpt = state[state.size()-maxdepth];
        nextpt.score = evluateState(state,nextpt.color);
        return nextpt;
    }
    //获取最优点
    std::stack<point> nextPossibleStep = getPossibleNextStep(state,nowcolor);
    point maxV = {nowcolor,-1,-1,-INF},minV= {nowcolor,-1,-1,INF};
    int nowval = evluateState(state,nowcolor);
    int cnt = 0;
    while(nextPossibleStep.size()){
        cnt++;
        if(cnt>10&&maxdepth>4)
            break;
        point tpstep = nextPossibleStep.top();
        nextPossibleStep.pop();
        state.push_back(tpstep);
        int tkey = getZobristKey(state);
        //更新zobrist——list
        if(!zobrist_list.count(tkey)){
            if(nowcolor)
                zobrist_list[tkey] = nowval^zobrist_black[tpstep.x][tpstep.y];
            else
                zobrist_list[tkey] = nowval^zobrist_white[tpstep.x][tpstep.y];
        }
        point val = alphaBetaPlay(state,!nowcolor,depth+1,alpha,beta,maxdepth);
        state.pop_back();
        //Alpha-beta剪枝
        if(depth%2 == 0){//对应Max层
            if(val.score>maxV.score){
                maxV = val;
            }
            alpha = std::max(alpha,maxV.score);
        }else{//对应Min层
            if(val.score<minV.score){
                minV = val;
            }
            beta = std::min(beta,minV.score);
        }
        //alpha>=beta进行剪枝操作
        if(alpha>=beta)
            break;
    }
    //max层返回maxv，min层返回minv
    if(depth%2 == 0)
        return maxV;
    else
        return minV;
}
//获取64bit随机数
ULL getRandom64() {
    return (ULL)rand() | ((ULL)rand() << 15) | ((ULL)rand() << 30) | ((ULL)rand() << 45) | ((ULL)rand() << 60);
}
//获取state局面的ZobristKey
ULL aiJudge::getZobristKey(std::vector<aiJudge::point> state)
{
    ULL key = 0;
    for(auto i:state){
        int x = i.x,y = i.y;
        int color = i.color;
        if(color){
            key = key^zobrist_black[x][y];
        }else{
            key = key^zobrist_white[x][y];
        }
    }
    return key;
}

void initZobrist()
{
    for(int i = 1;i<=M;i++)
        for(int j = 1;j<=M;j++){
            zobrist_black[i][j] = getRandom64();
            zobrist_white[i][j] = getRandom64();
        }
}
