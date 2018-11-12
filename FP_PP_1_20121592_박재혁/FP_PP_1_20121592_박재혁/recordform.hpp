#pragma once
#include <string>
#include <iostream>

// ������ ���ڵ��� ������ ���� Ŭ���� ����
#define RECORD_PAGE 16

// ���ڵ带 �о��� ��, ����/����� Ȥ�� ������ ��Ÿ���� enum code
enum DeletedRecord_ErrCode
{
	dr_error = -1,
	dr_nondeleted = 0,
	dr_deleted = 1,
};

// Record�� Head �κ�
class DeletedRecordHead
{
	int delim; // Head�� �������ִ� ������
	bool deleted; //�����ƴ��� �� �ƴ��� ����
	int page_num; //���� ���ڵ尡 �����ϰ� �ִ� page number

	const static int head_size = 7; //head ũ��
	char buffer[head_size] = {}; //buffer

public:
	DeletedRecordHead(char delim ='|', bool deleted = false, int page_num = 1)
	{
		this->deleted = deleted;
		this->delim = delim;
		this->page_num = page_num;
		makeHead();
	}
	DeletedRecordHead(const DeletedRecordHead& d)
	{
		this->delim = d.delim;
		this->page_num = d.page_num;
		memcpy(this->buffer, d.buffer, head_size);
	}
	//Buffer�� Data�� size�� ���� header byte�� �����Ͽ� �ʿ��� Page�� ������ ����Ѵ�. 
	static int calPageNum(int buffer_size)
	{
		int whole_size = buffer_size + head_size;
		int remainder = whole_size % RECORD_PAGE;
		return whole_size / RECORD_PAGE + (remainder ? 1 : 0);
	}

	//Record�� ����, �̻��� ����
	bool getDeleted() const ;

	//������ get/set
	void setDelim(char delim);
	const char getDelim() const;

	//head buffer�� get�� head buffer�� size
	const char* getHead() const;
	const int getHeadSize() const;

	//record�� ���̰� �ִ� page�� ���� get/set
	void setPageNum(int page_num);
	const int getPageNum() const;
	// record ��ü���� ����ϰ� �ִ� byte ����
	const int getPageSize() const;
	// record���� header �κ��� �� ������ �κ��� byte
	const int getDataSize() const;

	//����ü -> buffer�� ��ȯ
	void makeHead();
	//buffer -> ����ü�� ��ȯ
	DeletedRecord_ErrCode extractHead(const char* buffer);
};

//Deleted Record�� ��ü �κ�
class DeletedRecord
{
protected:
	DeletedRecordHead head;
	char buffer[RECORD_PAGE+1];
	int prevaddr; //����Ű�� �ִ� ���� record�� ��ġ
	int nextaddr; //����Ű�� �ִ� ���� record�� ��ġ

	//����ü -> buffer�� ��ȯ
	void makeRecord();
	//buffer -> ����ü�� ��ȯ
	DeletedRecord_ErrCode extractRecord(const char* buffer);

public:
	DeletedRecord(char _delim, int _prevaddr = -1, int _nextaddr = -1, int _num = 1) : prevaddr(_prevaddr), nextaddr(_nextaddr)
	{
		this->head = DeletedRecordHead(_delim, true, _num);
		makeRecord();
	}

	DeletedRecord(const DeletedRecord& d)
	{
		this->prevaddr = d.prevaddr;
		this->nextaddr = d.nextaddr;
		this->head = d.head;
		makeRecord();
	}

	DeletedRecord& operator=(const DeletedRecord& d)
	{
		this->prevaddr = d.prevaddr;
		this->nextaddr = d.nextaddr;
		this->head = d.head;
		makeRecord();
		return *this;
	}

	// ����ü�� Setting
	DeletedRecord_ErrCode set(int _prevaddr, int _nextaddr, int _num);
	// buffer -> ����ü�� ��ȯ
	DeletedRecord_ErrCode set(const char* buffer);
	// DeletedRecord�� buffer�� ���·� return�Ѵ�.
	const char* get() const;
	// DeletedRecord�� buffer ũ�⸦ return�Ѵ�.
	const int getBufferSize() const;

	// private member data�� ���� �������� get/set�Լ�.
	const int getPrevaddr() const;
	void setPrevaddr(int addr);
	const int getNextaddr() const;
	void setNextaddr(int addr);

	// DeltedRecordHeader�� �ִ� member data�� ���� �������� get/set �Լ�.
	void setDelim(char delim);
	const char getDelim() const;
	const char* getHead() const;
	const int getHeadSize() const;
	const int getPageNum() const;
	const int getPageSize() const;
	void setPageNum(int num);
	const int getDataSize() const;
	
	// stream -> ����ü/buffer�� ��ȯ
	DeletedRecord_ErrCode read(std::istream& stream);
	// ����ü/buffer -> stream���� ��ȯ
	const int write(std::ostream& stream) const;
};

//Deleted Record�� linked list�� �ش��ϴ� ����ü��.
class DeletedRecordList : public DeletedRecord
{
	//Record File ���ο��� �����ϴ� Deleted Record�� head�� �ּ�
	int header_addr;
public:
	DeletedRecordList(char delim = '|') : DeletedRecord(delim)
	{
		makeRecord();
	}
	const int get_headaddr() const
	{
		return header_addr;
	}
	void set_headaddr(int addr)
	{
		header_addr = addr;
	}
	int ReadHeader(std::istream& stream)
	{
		header_addr = stream.tellg();
		stream.read(buffer, RECORD_PAGE-1);
		if (!stream.good()) return false;
		this->extractRecord(buffer);
		return true;
	}
	int WriteHeader(std::ostream& stream) const
	{
		stream.write(this->buffer, RECORD_PAGE);
		if (!stream.good()) return false;
		return true;
	}
	
	int pop(std::iostream& stream, const DeletedRecord& temp);
	int push(std::iostream& stream, DeletedRecord& temp, int addr);
};



