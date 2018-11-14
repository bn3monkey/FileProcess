#include "MemberManager.hpp"

Authority MemberManager::verify(std::string id, std::string password)
{
	Member m;
	m.setKey(id.c_str());
	if (this->search(m, m) == RM_not_found)
	{
		auth = auth_noneid;
		return auth_noneid;
	}
	else
	{
		if (!password.compare(m.getPassword()))
		{
			profile = m;
			auth = (Authority)m.getLevel();
		}
		else
			auth = auth_nonepw;	
		return auth;
	}
}
// ��ȸ
RM_errcode MemberManager::retrieve(std::vector<Member>& view)
{
	return RecordManager::retrieve(view);
}
// Ž��
RM_errcode MemberManager::search(const Member& source, Member& result)
{
	return RecordManager::search(source, result);
}
// ����
RM_errcode MemberManager::update(const Member& source)
{
	return RecordManager::update(source);
}
// �߰�
RM_errcode MemberManager::insert(const Member& source)
{
	return RecordManager::insert(source);
}
// ����
RM_errcode MemberManager::remove(const Member& source)
{
	RM_errcode errcode = RecordManager::remove(source);
	if (errcode == RM_valid)
	{
		errcode = pPurchaseManager->cascade_remove(source);
	}
	if (errcode == RM_valid)
	{
		//���� ����ڸ� �������� ���, ������ �� �ֵ��� return ���� �Ѱ��ش�.
		if (!strcmp(source.getKey(), profile.getKey()))
		{
			auth = auth_noneid;
			return RM_noauth;
		}
	}
	return errcode;
}

RM_errcode MemberManager::my_retrieve(Member& dest)
{
	if (auth <= auth_nonepw)
		return RM_not_found;
	dest = profile;
	return RM_valid;
}
RM_errcode MemberManager::my_update(const Member& source)
{
	if (strcmp(source.getKey(), profile.getKey()))
		return RM_not_found;
	profile = source;
	return this->update(profile);
}
RM_errcode MemberManager::my_remove()
{
	return this->remove(profile);
}