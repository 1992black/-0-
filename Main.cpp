#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<cmath>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<sstream>
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

typedef pair<bitset<TN>, vector<int> > BitType;
typedef hash_map<string, BitType> HashType;

bitset<TN> I[IN];//记录矩阵
bitset<TN> INote[IN];//记录原始矩阵
int n;//T的数量
int iNum;//I的数量
vector<int> iSet, dfsSet;//iSet现有的集合，dfsSet搜索时候用的集合
vector<int> tSet;//T的集合
HashType hashNote;
int iCount[IN];
int tCount[TN];
int iPs;//I的频数
int tPs;//T的频数
int xs;//项数
int topNum;//每次组合的个数（每次+1）
int nowNum;//符合条件的项数

string intToStr(int n)
{
    string ret = "";
    while(n)
    {
        ret += (n % 10 + '0');
        n /= 10;
    }
    ret.reserve(ret.length());
    return ret;
}

string vecToStr(const vector<int> &vec)
{
    string ret = "";
    for(int i = 0; i < vec.size(); ++i)
    {
        ret += intToStr(vec[i]);
    }
    return ret;
}

void init()
{
    iSet.clear();
    for(int i = 1; i <= iNum; ++i)
    {
        iSet.push_back(i);
        dfsSet.push_back(i);
    }
    tSet.clear();
    for(int i = 1; i <= n; ++i)
    {
        tSet.push_back(i);
    }
}

void in()//输入转换为矩阵
{
    char m_cFilePath[1000];
	cout << "Please Input File Path" << endl;
	gets(m_cFilePath);
	cout << "Please Input Threshold Value" << endl;
	cin >> iPs;
	tPs = iPs;
	ifstream m_fsIn(m_cFilePath, ios::in);
	if(!m_fsIn.is_open())
	{
		cout << "error" << endl;
		return ;
	}
	char m_cLine[1000] = {0};
	int m_strTemp;
	int i = 1;
	n = 0;
    iNum = -1;
	while(m_fsIn.getline(m_cLine, sizeof(m_cLine)))
	{
		stringstream m_ssWord(m_cLine);
		//cout << m_ssWord.width();
		while(m_ssWord)
		{

			m_ssWord >> m_strTemp;
            //cout << m_strTemp << endl;
            iNum = max(iNum, m_strTemp);
			I[m_strTemp][i] = 1;
            INote[m_strTemp][i] = 1;
		}
		i++;
		n++;
	}
}

void addICount(const vector<int> &vec)//
{
    for(int i = 0; i < vec.size(); ++i)
    {
        iCount[vec[i]]++;
    }
}

void addTCount(const bitset<TN> &bit)
{
    for(int i = 1; i <= n; ++i)
    {
        if(bit[i])
        {
            int temp = tSet[i - 1];
            tCount[temp]++;
        }
    }
}

void add(const vector<int> &vec, const bitset<TN> &bit)//向hashNote加入I1，I2和它的bit, 并统计iCount, tCount
{
    string s = vecToStr(vec);
    hashNote[s] = BitType(bit, vec);
//    addTCount(bit);
    if(bit.count() >= iPs)
    {
        nowNum++;
        addICount(vec);
    }
}

void dfs(int x, int num, int topNum, bitset<TN> bit)//查找所有组合
{
//    printf("x = %d num = %d topNum = %d bit = ", x, num, topNum);
//    debug(bit, 0, 5);
//    printf("\n");
    if(num == topNum)
    {
        add(dfsSet, bit);
        return ;
    }
    for(int i = x; i <= iNum; ++i)
    {
//        printf("*****x = %d num = %d topNum = %d\n", x, num, topNum);
//        See(i);
//        See2(n - i, topNum - num);
        if(iNum - i + 1 >= topNum - num)//剪枝，如果剩下的搜索项少于还需要的搜索项，就剪枝
        {
            dfsSet[num] = iSet[i - 1];
            bitset<TN> nextBit = bit & INote[iSet[i - 1]];
            dfs(i + 1, num + 1, topNum, nextBit);
        }
    }
}

void testOut()
{
    HashType::iterator it = hashNote.begin();
    while(it != hashNote.end())
    {
        BitType bitPair = it -> second;
        bitset<TN> bit = bitPair.first;
        if(bit.count() >= iPs)
        {
            //
            const vector<int> vec = bitPair.second;
            for(int i = 0; i < vec.size(); ++i)
            {
                printf("%d ", vec[i]);
            }
            //

//            cout << it -> first << ' ';
            cout << " bit.count() = " << bit.count() << "   " << endl;;
//            debug(bit, 1, n);
//            printf("\n");
//            for(int i = 1; i <= n; ++i)
//            {
//                printf("%d\t", bit[i] ? 1 : 0);
//            }
//            cout << endl;
        }
        it++;
    }
}

void testOutI()
{
    for(int i = 1; i <= iNum; ++i)
    {
        for(int k = 1; k <= n; ++k)
        {
            printf("%d ", I[i][k] ? 1 : 0);
        }
        printf("\n");
    }
}

void updateISet()//更新I集合
{
    vector<int> temISet;
    for(int i = 1; i <= iNum; ++i)
    {
        if(iCount[i] >= iPs)
        {
            temISet.push_back(iSet[i - 1]);
        }
    }
    iSet = temISet;
    iNum = iSet.size();
}

void screenTSet()
{
    HashType::iterator it = hashNote.begin();
    while(it != hashNote.end())
    {
        BitType bitPair = it -> second;
        bitset<TN> bit = bitPair.first;
        if(bit.count() >= iPs)
        {
            addTCount(bit);
        }
        it++;
    }
}

void updateTSet()//更新T集合
{
//    printf("There is updateTSet():\n");
    vector<int> temTSet;
    for(int i = 1; i <= n; ++i)
    {
        if(tCount[i] >= tPs)
        {
//            See(i);
            temTSet.push_back(tSet[i - 1]);
        }
    }
    tSet = temTSet;
//    See(tSet.size());
    n = tSet.size();
}

void updateI()//更新矩阵
{
    for(int i = 0; i < iSet.size(); ++i)
    {
        bitset<TN> temBit(0);;
        for(int k = 0; k < tSet.size(); ++k)
        {
            temBit[k + 1] = I[iSet[i]][tSet[k]];
        }
        I[i + 1] = temBit;
    }
//    for(int i = 1; i <= n; ++i)
//    {
//        debug(I[i], 1, iNum);
//    }
}

int main()
{
    in();
    init();
    xs = 3;
    topNum = 2;
//    cout << "iSet = " << endl;
//    int iLen = iSet.size();
//    debug(iSet, 0, iLen - 1);
    printf("topNum = 1\n");
    for(int i = 1; i <= iNum; ++i)
    {
        printf("%d bit.count() = %d\n", i, I[i].count());
    }
    while(true)
    {
        hashNote.clear();
        bitset<TN> a;
        a.flip();//置成全1(全部取反)
        dfsSet.clear();
        for(int i = 0; i < topNum; ++i)
        {
            dfsSet.push_back(i + 1);
        }
        memset(iCount, 0, sizeof(iCount));
        memset(tCount, 0, sizeof(tCount));
        nowNum = 0;
//        See("Begin");
        dfs(1, 0, topNum, a);
        See(topNum);
        topNum++;
//        printf("This is iCount \n");
//        debug(iCount, 1, iNum);
//        printf("This is tCount \n");
//        debug(tCount, 1, n);
//        testOutI();
        testOut();
        updateISet();
        screenTSet();
        updateTSet();
        updateI();
//        cout << "iSet = " << endl;
//        int iLen = iSet.size();
//        debug(iSet, 0, iLen - 1);
//        cout << "tSet = " << endl;
//        int tLen = tSet.size();
//        debug(tSet, 0, tLen - 1);
//        cout << "hashNote.size() = " << hashNote.size() << endl;
        if(nowNum < xs)
        {
            break;
        }
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
/*
4 5
3 1 3 4
3 2 3 5
4 1 2 3 5
2 2 5
*/
/*
10 5
3 1 2 5
2 2 4
2 2 3
3 1 2 4
2 1 3
2 2 3
2 1 3
4 1 2 3 5
3 1 2 3
3 3 4 5
*/
