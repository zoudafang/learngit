#pragma once
#include<set>
#include<unordered_map>
#include <random>
#include <ctime>
#include<iostream>

using namespace std;

struct information
{
	uint64_t fullkey[2];//完整特征值
	uint16_t location = 111;//存储位置
	//uint64_t count;//频率
};
class containers
{
public:
	static int keybit;
	static int hammdist;
	static int sub_index_num;
	int sub_keybit;
	int sub_hammdist;
	static int initialize_size;
	static int test_size;
	int successful_num = 0;

	set<uint32_t> candidate;
	unordered_map<uint32_t, information> full_index;//完整索引
	unordered_map<uint32_t, uint32_t>sub_index1;//四个子索引
	unordered_map<uint32_t, uint32_t>sub_index2;
	unordered_map<uint32_t, uint32_t>sub_index3;
	unordered_map<uint32_t, uint32_t>sub_index4;
	vector<uint32_t>C_0_TO_subhammdis;//用于与特征段做异或运算的所有数字的容器
	set<pair<uint64_t, uint64_t>>test_pool;
	containers();
	void prepare();
	void initialize();
	void find_sim(uint64_t query[]);
	void test();
};
