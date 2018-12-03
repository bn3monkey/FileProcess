#pragma once
#include <vector>
#include "Indfile.h"
#include "BasicClassHeader.h"

enum Authority
{
	auth_noneid,
	auth_nonepw,
	auth_admin = '1',
	auth_normal = '9',
};
enum RM_errcode
{
	RM_valid, //Ÿ���� ��
	RM_not_found, //�ش� Ű�� ���ڵ忡 ���� ������ ã�� �� ���� ��
	RM_redundant, //�ش� Ű�� �̹� ������ ��
	RM_cannot_read, //���Ͽ��� �о�� �� ���� ��
	RM_cannot_write, //���Ͽ� �� �� ���� ��
	RM_noauth, //Member ���Ͽ��� ���� ����ڰ� �����Ǿ� �ý����� ���� �����ؾ��� ��
	RM_not_match, //My �Լ��� �䱸�ϴ� element�� ���� ����� ������ ��ġ���� ���� ��
};
template <class RecType>
class RecordManager
{
protected:
	int is_indexed;
	RecordFile<RecType>* file;
	TextIndexedFile<RecType>* indfile;
	RecType current;
	char* filename;
	int view_addr;

	inline int Open(int MODE) 
	{
		if(is_indexed)
			return this->indfile->Open(this->filename, MODE);
		else
			return this->file->Open(this->filename, MODE);
	}
	inline int Open(std::string filename, int MODE) 
	{
		char name[50];
		memcpy(name, this->filename, strlen(this->filename));
		if (is_indexed)
			return this->indfile->Open(name, MODE);
		else
			return this->file->Open(this->filename, MODE);
	}
	inline int Close() 
	{
		if (is_indexed)
			return this->indfile->Close();
		else
			return this->file->Close(); 
	}

public:
	RecordManager(char* _filename, RecordFile<RecType>* _file) : filename(_filename) , file(_file), view_addr(-1) 
	{
		file = _file;
		is_indexed = 0;
	}
	RecordManager(char* _filename, TextIndexedFile<RecType>* _file) : filename(_filename), view_addr(-1)
	{
		indfile = _file;
		is_indexed = 1;
	}
	~RecordManager() { 
	}

	// ��ȸ
	inline void refresh() { this->view_addr = -1; }
	virtual RM_errcode retrieve(std::vector<RecType>& view);
	// Ž��
	virtual RM_errcode search(const RecType& source, RecType& dest);
	// ����
	virtual RM_errcode update(const RecType& source, int recaddr = -1);
	// �߰�
	virtual RM_errcode insert(const RecType& source);
	// ����
	virtual RM_errcode remove(const RecType& source, int recaddr = -1);
};

// ��ȸ
template <class RecType>
RM_errcode RecordManager<RecType>::retrieve(std::vector<RecType>& view)
{
	this->Open(ios::in);

	view.clear();
	RecType m;
	while(true)
	{
		view_addr = file->Read(m, view_addr);
		if (view_addr == -1)
			break;
		else	
			view_addr = file->NextRecord();
		view.push_back(m);
	}

	this->Close();

	return RM_valid;
}
// Ž��
template <class RecType>
RM_errcode RecordManager<RecType>::search(const RecType& source, RecType& dest)
{
	this->Open(ios::in);

	int readaddr = file->Find(source);
	if (readaddr == -1)
	{
		this->Close();
		return RM_not_found;
	}
	int result = file->Read(dest, readaddr);
	if (result == -1)
	{
		this->Close();
		return RM_cannot_read;
	}
	this->Close();
	return RM_valid;
}
// ����
template <class RecType>
RM_errcode RecordManager<RecType>::update(const RecType& source, int recaddr)
{
	this->Open(ios::in | ios::out);

	int readaddr = file->Update(source, recaddr);
	if (readaddr == -1)
	{
		this->Close();
		return RM_not_found;
	}
	this->Close();
	return RM_valid;
}
// �߰�
template <class RecType>
RM_errcode RecordManager<RecType>::insert(const RecType& source)
{
	this->Open(ios::in | ios::out);

	int readaddr = file->Find(source);
	if (readaddr != -1)
	{
		this->Close();
		return RM_redundant;
	}
	readaddr = file->Insert(source);
	if (readaddr == -1)
	{
		this->Close();
		return RM_cannot_write;
	}
	this->Close();
	return RM_valid;

}
// ����
template <class RecType>
RM_errcode RecordManager<RecType>::remove(const RecType& source, int recaddr = -1)
{
	this->Open(ios::in | ios::out);

	int readaddr = file->Remove(source, recaddr);
	if (readaddr == -1)
	{
		this->Close();
		return RM_not_found;
	}
	this->Close();
	return RM_valid;
}
