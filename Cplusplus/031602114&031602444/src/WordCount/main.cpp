#include"pch.h"
#include"CharCount.h"
#include"WordCount.h"
#include"LineCount.h"
#include"SortTopN.h"
#include"WeightTypeNE.h"
#include"Word_Group_Cnt.h"
#include <iostream>
#include<string>
#include<stdio.h>
#include<cstdlib>
#include<fstream>
#include<algorithm>
#include<vector>
#include<map>

using namespace std;

int main(int argc, char *argv[])						//int argc, char *argv[]
{
	int charCnt = 0;									//计算字符数（包括空格，换行符，制表符，排除序号和Title： 及Abstract： ）
	int wordCnt = 0;									//计算有效单词数（排除Title： 及Abstract： ）
	int lineCnt = 0;									//计算有效行数（排除序号）
	int weight_Type = -1;								//权重
	int word_Group_Len = -1;							//词组长度
	int top_Num = -1;									//词频前N
	char* input_Filename = argv[0];
	char* output_Filename = argv[0];

	if (argc < 7 || argc > 11)							// 输入必须符合题目要求格式
	{
		printf("Please input in a right format!\n");
		return -1;
	}

	for (int i = 1; i < argc; i += 2)
	{
		string para_Type(argv[i]);						//参数含义，转换成string类型
		if (para_Type == "-i")							//-i参数设定输入文件input_Filename；
		{
			input_Filename = argv[i + 1];
		}
		else if (para_Type == "-o")						//-o参数设定输出文件output_Filename；
		{
			output_Filename = argv[i + 1];
		}
		else if (para_Type == "-w")						//-w参数设定不同权重计算单词词频；（0：Title和Abstract字段的权重都为1，1：Title字段权重为10，Abstract字段权重为1）
		{
			weight_Type = atoi(argv[i + 1]);
		}
		else if (para_Type == "-m")						//-m参数设定统计的词组长度；
		{
			word_Group_Len = atoi(argv[i + 1]);
		}
		else if (para_Type == "-n")						//-n参数设定输出的单词（词组）数量；
		{
			top_Num = atoi(argv[i + 1]);
		}
	}

	charCnt = CharCount(input_Filename);
	wordCnt = WordCount(input_Filename);
	lineCnt = LineCount(input_Filename);

	ofstream out_file;
	out_file.open(output_Filename);
	if (!out_file)
	{
		cout << "Can't open file :result.txt. " << "\nUsage:countch filename" << endl;
		exit(0);
	}
	out_file << "characters: " << charCnt << endl;
	out_file << "words: " << wordCnt << endl;					
	out_file << "lines: " << lineCnt << endl;

	//判断输入参数中是否有-m 和 -n决定后续输出top_Num个单词（词组）
	if (word_Group_Len == -1 && top_Num == -1)
	{
		//输出默认的前十个的单词函数；
		vector<pair<string, int>> x1;
		map<string, int> word_Map;

		WeightTypeNE(input_Filename, word_Map, weight_Type);
		SortTopN(10, word_Map, x1);
		Display(x1, out_file);
	}

	else if (word_Group_Len != -1 && top_Num == -1)
	{
		//输出默认的前十个长度为word_Group_Len的词组函数；
		vector<pair<string, int>> x1;
		map<string, int> group_Map;

		Cut_Ttl_Abs(word_Group_Len, input_Filename, group_Map, weight_Type);
		SortTopN(10, group_Map, x1);
		Display(x1, out_file);
	}

	else if (word_Group_Len == -1 && top_Num != -1)
	{
		//输出top_Num个单词函数；
		vector<pair<string, int>> x1;
		map<string, int> word_Map;

		WeightTypeNE(input_Filename, word_Map, weight_Type);
		SortTopN(top_Num, word_Map, x1);
		Display(x1, out_file);
	}

	else if (word_Group_Len != -1 && top_Num != -1)
	{
		//输出top_Num个长度为word_Group_Len的词组函数；
		vector<pair<string, int>> x1;
		map<string, int> group_Map;

		Cut_Ttl_Abs(word_Group_Len, input_Filename, group_Map, weight_Type);
		SortTopN(top_Num, group_Map, x1);
		Display(x1, out_file);
	}

	out_file.close();
	return 0;
}