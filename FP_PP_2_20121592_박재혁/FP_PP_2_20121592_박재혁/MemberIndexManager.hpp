#pragma once
#include "RecordManager.hpp"
#include "indfile.h"
class MemberIndexManager : public RecordManager<Member>
{
private:
	
public:
	MemberIndexManager(char* filename, TextIndexedFile<Member>* file) : RecordManager(filename, file) {}
	~MemberIndexManager()
	{
	}


	// ��ȸ
	virtual RM_errcode retrieve(std::vector<Member>& view);
	// Ž��
	virtual RM_errcode search(const Member& source, Member& dest);
	// ����
	virtual RM_errcode update(const Member& source);
	// �߰�
	virtual RM_errcode insert(const Member& source);
	// ����
	virtual RM_errcode remove(const Member& source);

};