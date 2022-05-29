#include <string>
#include <unordered_map>
#include <vector>

// key-val字典
struct Pattern {
    std::string pattern;
    int score;
};

// Tire树节点
struct ACNode {
    ACNode(int p, char c)
        : parent(p)
        , ch(c)
        , fail(-1)
    {
    }
    char ch;
    std::unordered_map<char, int> sons;
    int fail;
    std::vector<int> output;
    int parent;
};

// Ac自动机类（oop封装）
class AcString {
public:
    //对外接口——传入待匹配String，返回对应的匹配成功的结果
    std::vector<int> ACSearch(const std::string& text);

    //传入Key-val字典初始化
    AcString(std::vector<Pattern> patterns);
    ~AcString(); //析构
    void LoadPattern(const std::vector<std::string>& paterns);
    void BuildGotoTable();
    void BuildFailTable();

private:
    int maxState;
    std::vector<ACNode> nodes;
    std::vector<std::string> paterns;
    //Tire树添加新节点
    void AddState(int parent, char ch);
};
