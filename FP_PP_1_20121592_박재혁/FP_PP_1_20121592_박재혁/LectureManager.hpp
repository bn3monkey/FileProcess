#pragma once
#pragma once
#include "PurchaseManager.hpp"

class LectureManager : public RecordManager<Lecture>
{
private:
	PurchaseManager* pPurchaseManager;
public:
	LectureManager(char* filename, RecordFile<Lecture>* file) : RecordManager<Lecture>(filename, file) {}
	inline void setPurchaseManager(PurchaseManager& pm)
	{
		pPurchaseManager = &pm;
	}
	
	// ��ȸ
	virtual RM_errcode retrieve(std::vector<Lecture>& view);
	// Ž��
	virtual RM_errcode search(const Lecture& source, Lecture& dest);
	// ����
	virtual RM_errcode update(const Lecture& source);
	// �߰�
	virtual RM_errcode insert(const Lecture& source);
	// ����
	virtual RM_errcode remove(const Lecture& source);
};