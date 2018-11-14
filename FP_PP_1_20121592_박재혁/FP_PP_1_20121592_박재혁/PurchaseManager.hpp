#pragma once
#include "RecordManager.hpp"

class PurchaseManager : public RecordManager<Purchase>
{
private:
	int find_addr;
	void refreshRedundant() { find_addr = -1; }
	bool findRedundant(const Purchase& source, int seed)
	{
		this->Open(ios::in);

		bool result;
		Purchase target = Purchase();
		
		//ó���� �ƴϸ� �̾ ã�´�.
		if(find_addr != -1)
			find_addr = file->NextRecord();
		
		while(true)
		{
			//�д´�.
			find_addr = file->Read(target, find_addr);
			// ����� �� �о����� ó������ ���ư���.
			if (find_addr == -1)
			{
				result = false;
				break;
			}
			else
			{
				// ã������ ���� �ּҸ� find_addr�� �����Ѵ�.
				if (target.checkRedundant(source, seed))
				{
					result = true;
					break;
				}
				// �� ã���� ���� �ּҸ� ã�´�.
				find_addr = file->NextRecord();
			}
		}

		this->Close();

		return result;
	}
	
public:
	PurchaseManager(char* filename, RecordFile<Purchase>* file) : RecordManager<Purchase>(filename, file) 
	{
		find_addr = -1;
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
};