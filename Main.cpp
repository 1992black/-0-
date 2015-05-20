#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

void JustACout(vector<vector<string> > m_vvsWhatEver, vector<int> m_vnSupportCount)
{
	cout << endl;
	for (int i = 0; i < m_vvsWhatEver.size(); i++)
	{
		for (int j = 0; j < m_vvsWhatEver.at(i).size(); j++)
		{
			cout << m_vvsWhatEver.at(i).at(j) << ' ';
		}
		cout << m_vnSupportCount.at(i) << endl;
	}
	cout << endl;
}
//删除最小次数的数
void DelMinNum(vector<int> *m_lnSupportCount, vector<vector<string> > *m_llsSupport, int m_nThreshold)
{
/*
	int m_nMinNum = m_lnSupportCount->at(0);

	for (int i = 1; i < m_lnSupportCount->size(); i++)
	{
		m_nMinNum = m_nMinNum > m_lnSupportCount->at(i) ? m_lnSupportCount->at(i) : m_nMinNum;
	}
*/
	for (int i = 0; i < m_lnSupportCount->size(); i++)
	{
		if (m_lnSupportCount->at(i) < m_nThreshold)
		{
			m_lnSupportCount->erase(m_lnSupportCount->begin() + i);
			m_llsSupport->erase(m_llsSupport->begin() + i);
			i--;
		}
	}
	return;
}

//将数据整合到一个vector内方便做全排列
void ChangeItemAlone(vector<string> * m_lsTempSupport, vector<vector<string> > *m_llsSupport)
{
	m_lsTempSupport->clear();
	for (int i = 0; i < m_llsSupport->size(); i++)
	{
		for (int j = 0; j < m_llsSupport->at(i).size(); j++)
		{
			bool isExist = false;
			for (int k = 0; k < m_lsTempSupport->size(); k++)
			{
				if (m_llsSupport->at(i).at(j) == m_lsTempSupport->at(k))
				{
					isExist = true;
					break;
				}
			}
			if (!isExist)
			{
				m_lsTempSupport->push_back(m_llsSupport->at(i).at(j));
			}
		}
	}
	return;
}

//全排列找到C
void FullArray(int m_nStartPosition, int m_nItemCount, vector<string> *m_lsTempSupport, vector<string> *m_lsTempOriginal, vector<vector<string> > *m_llsItems)
{
	if (m_nItemCount == m_lsTempOriginal->size())
	{
		m_llsItems->push_back((*m_lsTempOriginal));
		return;
	}

	for (int i = m_nStartPosition; i < m_lsTempSupport->size(); i++)
	{
		m_lsTempOriginal->push_back(m_lsTempSupport->at(i));
		m_lsTempSupport->erase(m_lsTempSupport->begin() + i);
		FullArray(i, m_nItemCount, m_lsTempSupport, m_lsTempOriginal, m_llsItems);
		m_lsTempSupport->insert(m_lsTempSupport->begin() + i, m_lsTempOriginal->back());
		m_lsTempOriginal->pop_back();
	}
	return;
}

//扫描C2来获得未精简的L2
void ScannerItems(vector<string> *m_lsTempSupport, vector<vector<string> > *m_llsOriginal, vector<vector<string> > *m_llsItems, vector<vector<string> > *m_llsSupport, vector<int> *m_lsSupportCount)
{
	m_llsSupport->clear();
	m_lsSupportCount->clear();
	m_lsTempSupport->clear();

	for (int i = 0; i < m_llsItems->size(); i++)
	{
		for (int oi = 0; oi < m_llsOriginal->size(); oi++)
		{
			int m_nCount = 0;
			for (int j = 0; j < m_llsItems->at(i).size(); j++)
			{
				for (int oj = 0; oj < m_llsOriginal->at(oi).size(); oj++)
				{
					if (m_llsItems->at(i).at(j) == m_llsOriginal->at(oi).at(oj))
					{
						m_nCount++;
						m_lsTempSupport->push_back(m_llsItems->at(i).at(j));
						break;
					}
				}
			}
			if (m_nCount == m_llsItems->at(i).size())
			{
				bool m_bHasItem = false;
				for (int si = 0; si < m_llsSupport->size(); si++)
				{
					int m_nIsSameCount = 0;
					for (int sj = 0; sj < m_llsSupport->at(si).size(); sj++)
					{
						for (int ti = 0; ti < m_lsTempSupport->size(); ti++)
						{
							if (m_llsSupport->at(si).at(sj) == m_lsTempSupport->at(ti))
							{
								m_nIsSameCount++;
								break;
							}
						}
					}
					if (m_nIsSameCount == m_llsSupport->at(si).size())
					{
						m_bHasItem = true;
						m_lsSupportCount->at(si)++;
						break;
					}
				}
				if (!m_bHasItem)
				{
					m_llsSupport->push_back((*m_lsTempSupport));
					m_lsSupportCount->push_back(1);
				}
			}
			m_lsTempSupport->clear();
		}
	}
}

int main()
{

	char m_cFilePath[1000];
	cout << "Please Input File Path" << endl;
	gets(m_cFilePath);
	ifstream m_fsIn(m_cFilePath, ios::in);
	if(!m_fsIn.is_open())
	{
		cout << "error" << endl;
		return 0;
	}
	char m_cLine[1000] = {0};
	string m_strTemp;

	int m_nTempCount = 0, m_nCount = 0, m_nThreshold;
	string str;
	vector<int> m_lnSupportCount;
	vector<string> m_lsTempOriginal, m_lsTempSupport;
	vector<vector<string> > m_llsOriginal, m_llsItems, m_llsSupport;
	cout << "Please Input Minspu:" << endl;
	cin >> m_nThreshold;
	time_t start, end, time;
	start = clock();
	//cout << '1' << endl;
	while(m_fsIn.getline(m_cLine, sizeof(m_cLine)))
	{
		stringstream m_ssWord(m_cLine);
		//cout << m_ssWord.width();
		while(m_ssWord)
		{
			m_ssWord >> m_strTemp;
		/*	bool m_bExist = false;
			for(int i = 0; i < m_llsSupport.size(); i++)
			{
				for (int j = 0; j < m_llsSupport.at(i).size(); j++)
				{
					if (m_strTemp == m_llsSupport.at(i).at(j))
					{
						m_lnSupportCount.at(i)++;
						m_bExist = true;
						break;
					}
				}

				vector<int>::iterator m_viResult = find(m_llsSupport.at(i).begin(), m_llsSupport.at(i).end(), m_strTemp);
				if(m_viResult != m_llsSupport.at(i).end())
				{
					m_lnSupportCount.at(i)++;
					m_bExist = true;
					break;
				}

			}
			if(!m_bExist)
			{
				m_lsTempSupport.push_back(m_strTemp);
				m_llsSupport.push_back(m_lsTempSupport);
				m_lnSupportCount.push_back(1);
				m_lsTempSupport.clear();
			}
*/
			m_lsTempOriginal.push_back(m_strTemp);
		}
		m_lsTempOriginal.pop_back();
		m_llsOriginal.push_back(m_lsTempOriginal);
		m_lsTempOriginal.clear();
	}
	//cout << 2 << endl;
/*
	m_lsTempSupport.push_back(0);
	m_lnSupportCount.push_back(0);
	m_llsSupport.push_back(m_lsTempSupport);
*/
//	cout << "i am here " << endl;
//	ScannerItems(&m_lsTempSupport, &m_llsOriginal, &m_llsItems, &m_llsSupport, &m_lnSupportCount);
	for(int k = 0; k < m_llsOriginal.size(); k++)
	{
		for(int z = 0; z < m_llsOriginal.at(k).size(); z++)
		{

			bool m_bExist = false;

			for(int i = 0; i < m_llsSupport.size(); i++)
			{
				for (int j = 0; j < m_llsSupport.at(i).size(); j++)
				{
					//cout << 3 << endl;
					if (m_llsOriginal.at(k).at(z) == m_llsSupport.at(i).at(j))
					{
						m_lnSupportCount.at(i)++;
						m_bExist = true;
						break;
					}
				}
			}
			if(!m_bExist)
			{
				m_lsTempSupport.push_back(m_llsOriginal.at(k).at(z));
				m_llsSupport.push_back(m_lsTempSupport);
				m_lnSupportCount.push_back(1);
				m_lsTempSupport.clear();
			}
		}
	}
	//cout << m_llsOriginal.size()<< endl;
	/*for(int i = 0; i < m_llsOriginal.size(); i++)
	{
		for(int j = 0; j < m_llsOriginal.at(i).size(); j++)
		{
			cout << m_llsOriginal.at(i).at(j) << ' ';
		}
		cout << endl;
	}	*/

/*
	while (cin >> m_nCount)
	{
		if (m_nCount != m_nTempCount)
		{
			m_llsOriginal.push_back(m_lsTempOriginal);
			m_lsTempOriginal.clear();
			m_nTempCount = m_nCount;
		}
		cin >> str;
		m_lsTempOriginal.push_back(str);

		bool isExist = false;

		for (int i = 0; i < m_llsSupport.size(); i++)
		{
			for (int j = 0; j < m_llsSupport.at(i).size(); j++)
			{
				if (str == m_llsSupport.at(i).at(j))
				{
					m_lnSupportCount.at(i)++;
					isExist = true;
					break;
				}
			}
		}
		if (!isExist)
		{
			m_lsTempSupport.push_back(str);
			m_llsSupport.push_back(m_lsTempSupport);
			m_lnSupportCount.push_back(1);
			m_lsTempSupport.clear();
		}
	}

	m_llsOriginal.push_back(m_lsTempOriginal);
	m_lsTempOriginal.clear();
	m_nTempCount = m_nCount;
*/
	//JustACout(m_llsOriginal, m_lnSupportCount);
	/*for(int i = 0; i < m_llsOriginal.size(); i++)
	{
		for(int j = 0; j < m_llsOriginal.at(i).size(); j++)
		{
			cout << m_llsOriginal.at(i).at(j) << ' ';
		}
		cout << endl;
	}*/
	//JustACout(m_llsSupport, m_lnSupportCount);
	DelMinNum(&m_lnSupportCount, &m_llsSupport, m_nThreshold);
/*
	for (int i = 0; i < m_lsTempSupport.size(); i++)
	{
		cout << m_lsTempSupport.at(i) << endl;
	}
	for (int i = 0; i < m_llsSupport.size(); i++)
	{
		for (int j = 0; j < m_llsSupport.at(i).size(); j++)
		{
			cout << m_llsSupport.at(i).at(j) << ' ';
		}
		cout << m_lnSupportCount.at(i) << endl;
	}
*/
	JustACout(m_llsSupport, m_lnSupportCount);
	int m_nLength = 2;
	//cout << endl << m_llsSupport.size() << endl;
	while (m_llsSupport.size() > 0)
	{
		ChangeItemAlone(&m_lsTempSupport, &m_llsSupport);

		m_lsTempOriginal.clear();
		m_llsItems.clear();
		FullArray(0, m_nLength++, &m_lsTempSupport, &m_lsTempOriginal, &m_llsItems);
		ScannerItems(&m_lsTempSupport, &m_llsOriginal, &m_llsItems, &m_llsSupport, &m_lnSupportCount);


		DelMinNum(&m_lnSupportCount, &m_llsSupport, m_nThreshold);
		JustACout(m_llsSupport, m_lnSupportCount);
		//cout << m_llsSupport.size() << endl;
/*
		for (int i = 0; i < m_lsTempSupport.size(); i++)
			{
			cout << m_lsTempSupport.at(i) << endl;
		}
		for (int i = 0; i < m_llsSupport.size(); i++)
		{
			for (int j = 0; j < m_llsSupport.at(i).size(); j++)
			{
				cout << m_llsSupport.at(i).at(j) << ' ';
			}
			cout << m_lnSupportCount.at(i) << endl;
		}
		cout << endl;
*/
		//JustACout(m_llsSupport, m_lnSupportCount);
		if (m_llsSupport.size() < 2)
		{
			break;
		}
	}


	/*for (int i = 0; i < m_lsTempSupport.size(); i++)
	{
		cout << m_lsTempSupport.at(i) << endl;
	}
	for (int i = 0; i < m_llsSupport.size(); i++)
	{
		for (int j = 0; j < m_llsSupport.at(i).size(); j++)
		{
			cout << m_llsSupport.at(i).at(j) << ' ';
		}
	cout << m_lnSupportCount.at(i) << endl;
	}*/

	//JustACout(m_llsItems);
	end = clock();
	//time = end - time;
	//cout << "time is " << time << endl;
	cout << start << ' ' << end << ' ' << end - start << endl;
	system("pause");
	return 0;
}
