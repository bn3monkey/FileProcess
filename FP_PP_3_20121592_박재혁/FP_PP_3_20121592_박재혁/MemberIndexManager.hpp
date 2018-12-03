#pragma once
#include "MemberManager.hpp"
#include "indfile.h"
class MemberIndexManager : public RecordManager<Member>
{
private:
	Authority auth;
	Member profile;
	PurchaseManager* pPurchaseManager;
	
public:
	MemberIndexManager(char* filename, TextIndexedFile<Member>* file) : RecordManager(filename, file)
	{
		auth = auth_noneid;
	}
	~MemberIndexManager()
	{
		pPurchaseManager = nullptr;
	}
	inline void setPurchaseManager(PurchaseManager& pm)
	{
		pPurchaseManager = &pm;
	}

	Authority verify(std::string id, std::string password);
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

	// �Ϲ� ����� ����
	RM_errcode my_retrieve(Member& dest);
	RM_errcode my_update(const Member& source);
	//����ڰ� �����Ǵ� ���̹Ƿ�, �ٷ� System�� ����ǵ��� ��ġ�Ѵ�.
	RM_errcode my_remove();
};