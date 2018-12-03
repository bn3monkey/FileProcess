#include "LectureManager.hpp"

// ��ȸ
RM_errcode LectureManager::retrieve(std::vector<Lecture>& view)
{
	return RecordManager::retrieve(view);
}
// Ž��
RM_errcode LectureManager::search(const Lecture& source, Lecture& result)
{
	return RecordManager::search(source, result);
}
// ����
RM_errcode LectureManager::update(const Lecture& source)
{
	return RecordManager::update(source);
}
// �߰�
RM_errcode LectureManager::insert(const Lecture& source)
{
	return RecordManager::insert(source);
}
// ����
RM_errcode LectureManager::remove(const Lecture& source)
{
	RM_errcode errcode = RecordManager::remove(source);
	pPurchaseManager->cascade_remove(source);
	return errcode;
}