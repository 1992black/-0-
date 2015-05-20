#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<cmath>
#include<algorithm>
#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<queue>
#include<stack>
#include<list>
#include<map>
#include<set>
#include<hash_map>

#define TEST

#define LL long long
#define Mt(f, x) memset(f, x, sizeof(f));
#define rep(i, s, e) for(int i = (s); i <= (e); ++i)
#ifdef TEST
    #define See(a) cout << #a << " = " << a << endl;
    #define See2(a, b) cout << #a << " = " << a << ' ' << #b << " = " << b << endl;
    #define debug(a, s, e) rep(_i, s, e) {cout << a[_i] << ' ';} cout << endl;
    #define debug2(a, s, e, ss, ee) rep(i_, s, e) {debug(a[i_], ss, ee)}
#else
    #define See(a)
    #define See2(a, b)
    #define debug(a, s, e)
    #define debug2(a, s, e, ss, ee)
#endif // TEST

const int MAX = 2e9;
const int MIN = -2e9;
const double eps = 1e-8;
const double PI = acos(-1.0);

using namespace std;
using namespace __gnu_cxx;

namespace __gnu_cxx
{
    template<> struct hash<const string>
    {
        size_t operator()(const string &s) const
        {
            return hash<const char*>()(s.c_str());
        }
    };
    template<> struct hash<string>
    {
        size_t operator() (const string &s) const
        {
            return hash<const char*>()(s.c_str());
        }
    };
}

const int IN = 105;
const int TN = 3005;

bitset<TN> I[IN];//记录矩阵
bitset<TN> INote[IN];//记录原始矩阵
int n;//T的数量
int iNum;//I的数量
string iSet, dfsSet;//iSet, 现有的集合，dfsSet搜索时候用的集合
string tSet;//T的集合
hash_map<string, bitset<TN> > hashNote;
int iCount[IN];
int tCount[TN];
int iPs;//I的频数
int tPs;//T的频数
int xs;//项数
int topNum;//每次组合的个数（每次+1）
int nowNum;//符合条件的项数

void init()
{
    iSet = "";
    for(int i = 1; i <= iNum; ++i)
    {
        iSet += i + '0';
    }
    dfsSet = iSet;
    tSet = "";
    for(int i = 1; i <= n; ++i)
    {
        tSet += '0' + i;
    }
}

void in()//输入转换为矩阵
{
    scanf("%d%d", &n, &iNum);
    for(int i = 1; i <= n; ++i)
    {
        int m;
        scanf("%d", &m);
        while(m--)
        {
            int a;
            scanf("%d", &a);
            I[a][i] = 1;
            INote[a][i] = 1;
        }
    }
}

void addICount(const string &s)
{
    for(int i = 0; i < s.length(); ++i)
    {
        iCount[s[i] - '0']++;
    }
}

void addTCount(const bitset<TN> &bit)
{
    for(int i = 1; i <= n; ++i)
    {
        if(bit[i])
        {
            int temp = tSet[i - 1] - '0';
            tCount[temp]++;
        }
    }
}

void add(const string s, const bitset<TN> &bit)//向hashNote加入I1，I2和它的bit, 并统计iCount, tCount
{
    hashNote.insert(pair<string, bitset<TN> > (s, bit));
    addTCount(bit);
    if(bit.count() >= iPs)
    {
        nowNum++;
        addICount(s);
    }
}

void dfs(int x, int num, int topNum, bitset<TN> bit)//查找所有组合
{
    printf("x = %d num = %d topNum = %d bit = ", x, num, topNum);
    debug(bit, 0, 5);
    printf("\n");
    if(num == topNum)
    {
        add(dfsSet, bit);
        return ;
    }
    for(int i = x; i <= iNum; ++i)
    {
//        if(n - i >= topNum - num)//剪枝，如果剩下的搜索项少于还需要的搜索项，就剪枝
        {
            dfsSet[num] = iSet[i - 1];
            bitset<TN> nextBit = bit & I[iSet[i - 1] - '0'];
            dfs(i + 1, num + 1, topNum, nextBit);
        }
    }
}

void testOut()
{
    hash_map<string, bitset<TN> >::iterator it = hashNote.begin();
    while(it != hashNote.end())
    {
        if(it -> second.count() >= iPs)
        {
            cout << it -> first << ' ';
            bitset<TN> bit = it -> second;
            cout << " bit.count() = " << bit.count() << "   ";
            debug(bit, 1, n);
            cout << endl;
        }
        it++;
    }
}

void updateISet()//更新I集合
{
    string temISet = "";
    for(int i = 1; i <= iNum; ++i)
    {
        if(iCount[i] >= iPs)
        {
            temISet += iSet[i - 1];
        }
    }
    iSet = temISet;
    iNum = iSet.length();
}

void updateTSet()
{
    string temTSet = "";
    for(int i = 1; i <= n; ++i)
    {
        if(tCount[i] >= tPs)
        {
            temTSet += tSet[i - 1];
        }
    }
    tSet = temTSet;
    n = tSet.length();
}

void updateI()
{
    for(int i = 0; i < iSet.length(); ++i)
    {
        bitset<TN> temBit(0);;
        for(int k = 0; k < tSet.length(); ++k)
        {
            temBit[k + 1] = I[i + 1][tSet[k] - '0'];
        }
        I[i + 1] = temBit;
    }
    for(int i = 1; i <= n; ++i)
    {
        debug(I[i], 1, iNum);
    }
}

int main()
{
    in();
    init();
    //
    iPs = 2;
    tPs = 2;
    xs = 3;
    topNum = 2;
    while(true)
    {
        hashNote.clear();
        bitset<TN> a;
        a.flip();//置成全1(全部取反)
        dfsSet = "";
        for(int i = 0; i < topNum; ++i)
        {
            dfsSet += "1";
        }
        memset(iCount, 0, sizeof(iCount));
        memset(tCount, 0, sizeof(tCount));
        nowNum = 0;
        dfs(1, 0, topNum, a);
        cout << "iSet = " << iSet << endl;
        cout << "tSet = " << tSet << endl;
        cout << "hashNote.size() = " << hashNote.size() << endl;
        if(nowNum < xs)
        {
            break;
        }
        topNum++;
        testOut();
        debug(iCount, 1, iNum);
        debug(tCount, 1, n);
        updateISet();
        updateTSet();
        updateI();
    }
    printf("The answer is :\n");
    testOut();
    //
    return 0;
}
/*
10 5
2 1 2
2 2 3
2 1 2
4 1 2 3 4
3 1 2 3
3 1 3 5
2 2 3
3 1 3 4
1 4
2 3 5
*/

