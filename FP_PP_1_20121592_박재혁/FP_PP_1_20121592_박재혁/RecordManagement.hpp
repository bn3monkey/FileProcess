#pragma once
#include "recfile.h"

enum RM_errcode
{
	RM_valid, //Ÿ���� ��
	RM_not_found, //�ش� Ű�� ���ڵ忡 ���� ������ ã�� �� ���� ��
	RM_redundant, //�ش� Ű�� �̹� ������ ��
	RM_cannot_read, //���Ͽ��� �о�� �� ���� ��
	RM_cannot_write, //���Ͽ� �� �� ���� ��
};
template <class RecType>
class RecordManagement
{
	RecordFile<RecType>* file;

public:
	RecordManagement(RecordFile<RecType>* _file = nullptr) : file(_file) {}

	int Open(char * filename, int MODE); 
	int Close();

	// Ž��
	int search(const char* key, RecType& result);
	// ����
	int update(const RecType& source, RecType& result);
	// �߰�
	int insert(const RecType& source, RecType& result);
	// ����
	int remove(const char* key);
};