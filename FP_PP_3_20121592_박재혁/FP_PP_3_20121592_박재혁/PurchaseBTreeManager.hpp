#pragma once
#include "RecordManager.hpp"
//#include "InversedIndex.hpp"

class PurchaseBTreeManager : public RecordManager<Purchase>
{
private:
	char lastid[2];
	void setLastKey()
	{
		lastid[0] = 0;
		lastid[1] = 0;
		Purchase target;

		std::string data_filename = std::string(filename) + ".dat";
		if (this->file->Open((char *)data_filename.c_str(), ios::in) == FALSE)
			return;
		
		while (file->Read(target) != -1)
		{
			if (target.getKey(0)[0] > lastid[0])
				lastid[0] = target.getKey(0)[0];
		}

		this->file->Close();

		lastid[0] += 1;
		return;
	}

	int find_addr;
	int find_nextaddr;
	void refreshRedundant() { find_addr = -1; find_nextaddr = -1; }
	bool findRedundant(const Purchase& source, Purchase& dest, int seed)
	{
		std::string data_filename = std::string(filename) + ".dat";
		if (this->file->Open((char *)data_filename.c_str(), ios::in) == FALSE)
			return false;

		bool result;

		//ó���� �ƴϸ� �̾ ã�´�.
		if (find_addr != -1)
			find_addr = find_nextaddr;

		while (true)
		{
			//�д´�.
			find_addr = file->Read(dest, find_addr);
			// ����� �� �о����� ó������ ���ư���.
			if (find_addr == -1)
			{
				find_nextaddr = -1;
				result = false;
				break;
			}
			else
			{
				find_nextaddr = file->NextRecord();
				// ã������ ���� �ּҸ� find_addr�� �����Ѵ�.
				if (dest.checkRedundant(source, seed))
				{
					result = true;
					break;
				}
				// �� ã���� ���� �ּҸ� ã�´�.
				find_addr = find_nextaddr;
			}
		}

		this->file->Close();

		return result;
	}
	
public:
	PurchaseBTreeManager(char* filename, BTreeFile<Purchase>* file, RecordFile<Purchase>* datafile) : RecordManager<Purchase>(filename, file)
	{
		this->file = datafile;
	}

	// ��ȸ
	virtual RM_errcode retrieve(std::vector<Purchase>& view);
	// Ž��
	virtual RM_errcode search(const Purchase& source, Purchase& dest);
	// ����
	virtual RM_errcode update(const Purchase& source, int recaddr = -1);
	// �߰�
	virtual RM_errcode insert(const Purchase& source);
	// ����
	virtual RM_errcode remove(const Purchase& source, int recaddr = -1);

	RM_errcode cascade_remove(const Member& m);
	RM_errcode cascade_remove(const Lecture& l);

	// �Ϲ� ����� ����
	RM_errcode my_retrieve(const Member& profile, std::vector<Purchase>& view);
	RM_errcode my_search(const Member& profile, const Lecture& lecture, Purchase& dest);
	RM_errcode my_update(const Member& profile, const Purchase& purchase, int recaddr = -1);
	RM_errcode my_insert(const Member& profile, const Purchase& purchase, int recaddr = -1);
	RM_errcode my_remove(const Member& profile, const Purchase& purchase, int recaddr = -1);
};