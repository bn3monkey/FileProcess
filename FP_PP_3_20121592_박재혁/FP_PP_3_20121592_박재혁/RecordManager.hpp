#pragma once
#include <vector>
#include "Indfile.h"
#include "btreefile.h"
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
enum RM_filemode
{
	RM_Record,
	RM_Indexed,
	RM_BTreed,
};
template <class RecType>
class RecordManager
{
protected:
	RM_filemode file_mode;
	RecordFile<RecType>* file;
	TextIndexedFile<RecType>* indfile;
	BTreeFile<RecType>* btreefile;
	RecType current;
	char* filename;
	int view_addr;

	inline int Open(int MODE) 
	{
		switch (file_mode)
		{
		case RM_Record:
			return this->file->Open(this->filename, MODE);
		case RM_Indexed:
			return this->indfile->Open(this->filename, MODE);
		case RM_BTreed:
			return this->btreefile->Open(this->filename, MODE);
		}
		return FALSE;
	}
	inline int Open(std::string filename, int MODE) 
	{
		char name[50];
		memcpy(name, filename.c_str(), strlen(this->filename));
		switch (file_mode)
		{
		case RM_Record:
			return this->file->Open(name, MODE);
		case RM_Indexed:
			return this->indfile->Open(name, MODE);
		case RM_BTreed:
			return this->btreefile->Open(name, MODE);
		}
		return FALSE;
	}
	inline int Close() 
	{
		switch (file_mode)
		{
		case RM_Record:
			return this->file->Close();
		case RM_Indexed:
			return this->indfile->Close(); 
		case RM_BTreed:
			return this->btreefile->Close();
		}
		return 0;
	}

public:
	RecordManager(char* _filename, RecordFile<RecType>* _file) : filename(_filename) , file(_file), view_addr(-1) 
	{
		file = _file;
		file_mode = RM_Record;
	}
	RecordManager(char* _filename, TextIndexedFile<RecType>* _file) : filename(_filename), view_addr(-1)
	{
		indfile = _file;
		file_mode = RM_Indexed;
	}
	RecordManager(char* _filename, BTreeFile<RecType>* _file) : filename(_filename), view_addr(-1)
	{
		btreefile = _file;
		file_mode = RM_BTreed;
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
	std::string data_filename = std::string(filename) + ".dat";
	if (this->file->Open((char *)data_filename.c_str(), ios::in) == FALSE)
		return RM_cannot_read;

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

	this->file->Close();

	return RM_valid;
}
// Ž��
template <class RecType>
RM_errcode RecordManager<RecType>::search(const RecType& source, RecType& dest)
{
	std::string data_filename = std::string(filename) + ".dat";
	if (this->file->Open((char *)data_filename.c_str(), ios::in) == FALSE)
		return RM_cannot_read;

	int readaddr = file->Find(source);
	if (readaddr == -1)
	{
		this->file->Close();
		return RM_not_found;
	}
	int result = file->Read(dest, readaddr);
	if (result == -1)
	{
		this->file->Close();
		return RM_cannot_read;
	}
	this->file->Close();
	return RM_valid;
}
// ����
template <class RecType>
RM_errcode RecordManager<RecType>::update(const RecType& source, int recaddr)
{
	std::string data_filename = std::string(filename) + ".dat";
	if (this->file->Open((char *)data_filename.c_str(), ios::in) == FALSE)
		return RM_cannot_read;

	int readaddr = file->Update(source, recaddr);
	if (readaddr == -1)
	{
		this->file->Close();
		return RM_not_found;
	}
	this->file->Close();
	return RM_valid;
}
// �߰�
template <class RecType>
RM_errcode RecordManager<RecType>::insert(const RecType& source)
{
	std::string data_filename = std::string(filename) + ".dat";
	if (this->file->Open((char *)data_filename.c_str(), ios::in) == FALSE)
		return RM_cannot_read;

	int readaddr = file->Find(source);
	if (readaddr != -1)
	{
		this->file->Close();
		return RM_redundant;
	}
	readaddr = file->Insert(source);
	if (readaddr == -1)
	{
		this->file->Close();
		return RM_cannot_write;
	}
	this->file->Close();
	return RM_valid;

}
// ����
template <class RecType>
RM_errcode RecordManager<RecType>::remove(const RecType& source, int recaddr = -1)
{
	std::string data_filename = std::string(filename) + ".dat";
	if (this->file->Open((char *)data_filename.c_str(), ios::in) == FALSE)
		return RM_cannot_read;

	int readaddr = file->Remove(source, recaddr);
	if (readaddr == -1)
	{
		this->file->Close();
		return RM_not_found;
	}
	this->file->Close();
	return RM_valid;
}
