#pragma once
#include <string>
#include <vector>

class InversedIndex
{
	//Member ID Ȥ�� Lecture ID�� �����ϴ� �����̴�.
	std::vector<std::string> secondarys;
	// �ش� Member ID�� �ش��ϴ� Purchase�� primary key�� �����ϴ� �����̴�.
	std::vector<std::vector<char>> primarys;

public:
	InversedIndex()
	{

	}
	
	int find(std::string secondary)
	{
		for (size_t i = 0; i<secondarys.size(); i++)
		{
			if (!secondary.compare(secondarys[i]))
				return (int)i;
		}
		return -1;
	}
	int find(int secondary_idx, char primary)
	{
		for (size_t i = 0; i<primarys[secondary_idx].size(); i++)
		{
			if (primarys[secondary_idx][i] == primary)
				return (int)i;
		}
		return -1;
	}
	int find(std::string secondary, char primary)
	{
		int result;
		result = find(secondary);
		if (result == -1) return -1;
		result = find(result, primary);
		if (result == -1) return -1;
		return result;
	}
	bool insert(std::string secondary, char primary)
	{
		//secondary key�� secondary_index ��Ͽ� �����ϴ��� Ȯ���Ѵ�.
		int idx = find(secondary);
		// ������ ���� �ִ´�.
		if (idx < 0)
		{
			// secondary index ��Ͽ� ���� index �߰�
			secondarys.push_back(secondary);
			// primary ��� �����ϰ� ���� key �߰�
			std::vector<char> temp;
			temp.push_back(primary);
			// primary key ����� ��Ͽ� ���� ��� �߰�.
			primarys.push_back(temp);
		}
		else
		{
			//�̹� priamry key�� ������ insert�� �����Ѵ�.
			if (find(idx, primary) != -1)
				return false;
			//������ primary�� �ִ´�.
			primarys[idx].push_back(primary);
		}
		return true;
	}
	bool remove(std::string secondary, char primary)
	{
		//secondary key�� secondary_index ��Ͽ� �����ϴ��� Ȯ���Ѵ�.
		int idx = find(secondary);
		// ������ index ��Ͽ� ����.
		if (idx < 0)
			return false;
		// ��Ͽ� ������, remove�� �����Ѵ�.
		idx = find(idx, primary);
		if (idx == -1)
			return false;
		// ��Ͽ� ������, �����.
		std::vector<char>& keys = primarys[idx];
		keys.erase(keys.begin() + idx);
		return true;
	}
	// secondary key�� ������ �ִ� primary key�� �� �ϳ��� �̰�, ��Ͽ��� �����Ѵ�.
	char pop(std::string secondary)
	{
		char result;
		int idx = find(secondary);
		if (idx < 0)
			return -1;
		std::vector<char>& keys = primarys[idx];
		result = keys.back();
		keys.pop_back();

		//secondary�� �ִ� primary key�� ��� �̾�����, secondary key ��Ͽ��� ���� secondary�� �����Ѵ�.
		if (keys.empty())
		{
			primarys.erase(primarys.begin() + idx);
			secondarys.erase(secondarys.begin() + idx);
		}
	}

	//���� �� ó���� ����, primary key return
	bool getPrimarys(std::string secondary, std::vector<char>& primarys)
	{
		int idx = find(secondary);
		if (idx == -1) return false;
		primarys = this->primarys[idx];
		return true;
	}
};