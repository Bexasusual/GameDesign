#pragma once
#include "stdafx.h"
#include "time.h"
#include "Menu.h"
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <iostream>
using namespace std;
class Score {
public:
	Score():score_number(0) {};
	Score(string m, string n) :score_number(0)
	{
		order = m;
		for (unsigned int i = 0; i < n.size(); i++) {
			score_number *= 10;
			score_number += (n[i] - '0');
		}
		if (score_number < 0)score_number = 0;
	}
	Score(string m, int num)
	{
		order = m;
		if (num < 0)score_number = 0;
		else score_number = num;
	}
	~Score() {};

	bool operator()(const Score& a, const Score& b)	//用该函数实现降序sort
	{
		return a.score_number > b.score_number;
	}

	string order;	//排序标号
	int score_number;	//成绩

};