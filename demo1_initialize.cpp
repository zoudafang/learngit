#include <string>
#include <unordered_map>
//#include <map>
#include <iostream> 
#include <set>
#include <vector>
//#include <queue>
#include <random>
#include <ctime>

using namespace std;

#define keybit 128    //����ֵ��λ��
#define hammdist 8    //��ΪͼƬ���Ƶ����������
#define sub_index_num 4   //������������
#define sub_keybit keybit/sub_index_num   //����ֵ�ε�λ��
#define sub_hammdist  hammdist/sub_index_num    //��ѡֵ�ı�׼
#define K 100 //�涨��part_index�Ĵ�С

int x = 0;

struct uint128_t
{
	uint64_t fullkey[2];//��������ֵ
};
//����������ָ�������
struct information
{
	uint64_t fullkey[2];//��������ֵ
	//uint64_t count;//Ƶ��
	uint16_t location=111;//�洢λ��
};
/*
struct cmp
{
	bool operator()(information a, information b)
	{
		return a.count > b.count;
	}
};
*/

set<uint32_t> candidate;//��ѡ��
unordered_map<uint32_t, information> full_index;//��������
unordered_map<uint32_t, uint32_t>sub_index1;//�ĸ�������
unordered_map<uint32_t, uint32_t>sub_index2;
unordered_map<uint32_t, uint32_t>sub_index3;
unordered_map<uint32_t, uint32_t>sub_index4;
vector<uint32_t>C_0_TO_subhammdis;//�����������������������������ֵ�����
set<pair<uint64_t, uint64_t>>test_pool;

//׼���������������������������֣�ȫ������C_0_TO_subhammdis�У�����ֻд�������λ��ͬ�������
//�����ܱ���������������û������������
void prepare()
{
	uint32_t tmp1 = 1;
	uint32_t tmp2 = 1;
	uint32_t tmp = 0;
	C_0_TO_subhammdis.push_back(0);
	switch (sub_hammdist)
	{
		case 2:
			for (int i = 0; i < 31; i++)
			{
				tmp1 = 0x00000001 << i;
				for (int j = 1+i; j < 32; j++)
				{
					tmp2 = 0x00000001 << j;
					tmp = tmp1 + tmp2;
					C_0_TO_subhammdis.push_back(tmp);
				}
			}
		case 1:
		{
			for (int x = 0; x < 32; x++)
			{
				tmp = 0x00000001 << x;
				C_0_TO_subhammdis.push_back(tmp);
			}
			break;
		}
		default:
			break;
	}
}

void initialize()
{
	uint64_t temp_key[2] = { 0 };
	uint64_t temp_keyx[2] = { 0 };
	uint32_t out_id = 0;
	uint32_t sub1, sub2, sub3, sub4 = 0;
	uint128_t temp_full_key;
	information temp_information;
	
	
	random_device rd;
	default_random_engine r_eng(rd());
	
	while(sub_index1.size()<20)
	{
		temp_key[0] = (uint64_t)r_eng();
		temp_key[0] = temp_key[0] << 32;
		temp_key[0] = temp_key[0] + (uint64_t)r_eng();
		temp_key[1] = (uint64_t)r_eng();
		temp_key[1] = temp_key[1] << 32;
		temp_key[1] = temp_key[1] + (uint64_t)r_eng();
		temp_keyx[0] = temp_key[0];
		temp_keyx[1] = temp_key[1];
		temp_information.fullkey[0] = temp_key[0];
		temp_information.fullkey[1] = temp_key[1];

		sub1 = temp_key[0] & 0xffffffff;
		temp_key[0] = temp_key[0] >> 32;
		sub2 = temp_key[0] & 0xffffffff;
		sub3 = temp_key[1] & 0xffffffff;
		temp_key[1] = temp_key[1] >> 32;
		sub4 = temp_key[1] & 0xffffffff;
		temp_full_key.fullkey[0] = temp_keyx[0];
		temp_full_key.fullkey[1] = temp_keyx[1];
		if ((sub_index1.find(sub1) == sub_index1.end()) && (sub_index2.find(sub2) == sub_index2.end()) && (sub_index3.find(sub3) == sub_index3.end()) && (sub_index4.find(sub4) == sub_index4.end()))
		{
			sub_index1.insert(std::pair<uint32_t, uint32_t>(sub1, out_id));
			sub_index2.insert(std::pair<uint32_t, uint32_t>(sub2, out_id));
			sub_index3.insert(std::pair<uint32_t, uint32_t>(sub3, out_id));
			sub_index4.insert(std::pair<uint32_t, uint32_t>(sub4, out_id));
			full_index.insert(std::pair<uint32_t, information>(out_id, temp_information));
			test_pool.insert(std::pair< uint64_t, uint64_t>(temp_keyx[0], temp_keyx[1]));
			out_id++;
		}
	}
	return;
}

void find_sim(uint64_t query[2])
{
	//ͨ��λ���㣬��ȡ����ֵ��
	uint64_t tmpquery1 = query[0];
	uint64_t tmpquery2 = query[1];
	uint32_t sub1 = tmpquery1 & 0xffffffff;
	tmpquery1 = tmpquery1 >> 32;
	uint32_t sub2 = tmpquery1 & 0xffffffff;
	uint32_t sub3 = tmpquery2 & 0xffffffff;
	tmpquery2 = tmpquery2 >> 32;
	uint32_t sub4 = tmpquery2 & 0xffffffff;

	//�����ĵ�����
	std::unordered_map<uint32_t, uint32_t>::const_iterator got1;
	std::unordered_map<uint32_t, uint32_t>::const_iterator got2;
	std::unordered_map<uint32_t, uint32_t>::const_iterator got3;
	std::unordered_map<uint32_t, uint32_t>::const_iterator got4;
	std::set<uint32_t>::iterator it;
	std::vector<uint32_t>::iterator its;

	//Ѱ�Һ�ѡ������
	uint32_t tmpsub1, tmpsub2, tmpsub3, tmpsub4 = 0;
	for (its= C_0_TO_subhammdis.begin(); its != C_0_TO_subhammdis.end(); its++)
	{
		//��������㣬�ҳ�ֻ�����λ��ͬ�Ŀ��ܺ�ѡֵ
		tmpsub1 = sub1 ^ *its;
		tmpsub2 = sub2 ^ *its;
		tmpsub3 = sub3 ^ *its;
		tmpsub4 = sub4 ^ *its;

		//�ֱ���4���������в��Һ�ѡֵ
		got1 = sub_index1.find(tmpsub1);
		if (got1 != sub_index1.end())//���������ܵĺ�ѡֵ�Ƿ����
		{
			it = candidate.find(got1->second);//����ʶ���ں�ѡ�����Ƿ��Ѿ�����
			if (it == candidate.end())
				candidate.insert(got1->second);
		}
		got2 = sub_index2.find(tmpsub2);
		if (got2 != sub_index2.end())
		{
			it = candidate.find(got2->second);
			if (it == candidate.end())
				candidate.insert(got2->second);
		}
		got3 = sub_index3.find(tmpsub3);
		if (got3 != sub_index3.end())
		{
			it = candidate.find(got3->second);
			if (it == candidate.end())
				candidate.insert(got3->second);
		}
		got4 = sub_index3.find(tmpsub4);
		if (got4 != sub_index3.end())
		{
			it = candidate.find(got4->second);
			if (it == candidate.end())
				candidate.insert(got4->second);
		}
	}

	//�Ժ�ѡ�����ν���ɸѡ
	uint64_t cmp_hamm[2] = { 0 };
	int count = 0;

	std::unordered_map<uint32_t, information>::const_iterator got_out;
	for (it = candidate.begin(); it != candidate.end(); ++it)
	{
		got_out = full_index.find(*it);
		if (got_out != full_index.end())
		{
			cmp_hamm[0] = query[0] ^ (got_out->second.fullkey[0]);
			cmp_hamm[1] = query[1] ^ (got_out->second.fullkey[1]);
			//����ͳ�Ʋ�ͬλ�ĸ���
			count = 0;
			while (cmp_hamm[0]) {
				count += cmp_hamm[0] & 1;
				cmp_hamm[0] = cmp_hamm[0] >> 1;
			}
			while (cmp_hamm[1]) {
				count += cmp_hamm[1] & 1;
				cmp_hamm[1] = cmp_hamm[1] >> 1;
			}
			if (count <= hammdist)
			{
				x++;
				//printf("Successfully found similar photos! x=%d\n",x);
			}
		}
	}
	candidate.clear();
}

void test()
{
	int m = 0;
	uint64_t temp_key[2] = { 0 };
	std::set<pair<uint64_t, uint64_t >>::iterator itx;
	int x = 0, y = 0;
	
	//for (itx = test_pool.begin(); itx != test_pool.end(); ++itx)
	for (itx = test_pool.begin(); m<20; ++itx)
	{
		x = rand() % 3;
		
		temp_key[0] = itx->first;
		temp_key[1] = itx->second;
		for (int i = 0; i < x; i++)
		{
			y = rand() % 32;
			temp_key[0] = temp_key[0] ^ (0x0000000000000001 << y);
			temp_key[1] = temp_key[1] ^ (0x0000000000000001 << y);
		}
		find_sim(temp_key);
		m++;
	}
}

void main() 
{
	//��ʼ�����ݣ�
	//���������ѯ��
	prepare();
	initialize();
	test();
	printf("%d", x);
	//�������ѯ������ֵ
}
