#pragma once
#include "LectureManager.hpp"
#include "indfile.h"
class LectureIndexManager : public RecordManager<Lecture>
{
private:
	PurchaseManager* pPurchaseManager;
public:
	LectureIndexManager(char* filename, TextIndexedFile<Lecture>* file) : RecordManager(filename, file) {}
	~LectureIndexManager()
	{
		pPurchaseManager = nullptr;
	}
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