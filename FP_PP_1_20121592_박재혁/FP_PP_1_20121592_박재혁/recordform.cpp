#pragma once
#include "recordform.hpp"
void DeletedRecordHead::setDeleted(bool offset)
{
	this->deleted = offset;
	makeHead();
}
bool DeletedRecordHead::getDeleted() const
{
	return this->deleted;
}
void DeletedRecordHead::setDelim(char delim = '|')
{
	this->delim = delim;
	makeHead();
}
const char DeletedRecordHead::getDelim() const
{
	return this->delim;
}
const char* DeletedRecordHead::getHead() const
{
	return buffer;
}
const int DeletedRecordHead::getHeadSize() const
{
	return head_size;
}

void DeletedRecordHead::setPageNum(int page_num)
{
	this->page_num = page_num;
	makeHead();
}
const int DeletedRecordHead::getPageNum() const
{
	return page_num;
}
const int DeletedRecordHead::getPageSize() const
{
	return page_num * RECORD_PAGE;
}
const int DeletedRecordHead::getDataSize() const
{
	return page_num * RECORD_PAGE - head_size;
}
void DeletedRecordHead::makeHead()
{
	buffer[0] = this->deleted + '0';
	buffer[1] = delim;
	memcpy(buffer + 2, (char *)(&page_num), sizeof(int));
	buffer[6] = delim;
}
// 1 : deleted, 0 : non_deleted -1 : error
DeletedRecord_ErrCode DeletedRecordHead::extractHead(const char* buffer)
{
	switch (buffer[0])
	{
	case '0': this->deleted = false; break;
	case '1': this->deleted = true; break;
	default: return dr_error;
	}

	if (buffer[1] != delim)
		return dr_error;

	memcpy((void *)(&this->page_num), (char *)(buffer + 2), sizeof(int));
	
	if (buffer[6] != delim)
		return dr_error;
	
	return deleted ? dr_deleted : dr_nondeleted;
}

/*****************************************************************/


void DeletedRecord::makeRecord()
{
	int step = 0;
	memset(buffer, 0, RECORD_PAGE);

	memcpy(buffer, this->head.getHead(), this->head.getHeadSize());
	step += this->head.getHeadSize();
	memcpy(buffer + step, (char *)(&prevaddr), sizeof(int));
	step += sizeof(int);
	memcpy(buffer + step, (char *)(&nextaddr), sizeof(int));
	buffer[RECORD_PAGE - 1] = this->head.getDelim();
}
DeletedRecord_ErrCode DeletedRecord::extractRecord(const char* buffer)
{
	DeletedRecord_ErrCode result = this->head.extractHead(buffer);
	if (result == dr_deleted)
	{
		int step = this->head.getHeadSize();
		memcpy((void *)(&prevaddr), (char *)(buffer + step), sizeof(int));
		step += sizeof(int);
		memcpy((void *)(&nextaddr), (char *)(buffer + step), sizeof(int));
		return dr_deleted;
	}
	return result;
}
DeletedRecord_ErrCode DeletedRecord::set(int _prevaddr, int _nextaddr, int _num)
{
	this->prevaddr = _prevaddr;
	this->nextaddr = _nextaddr;
	this->head.setPageNum(_num);
	makeRecord();
	return dr_deleted;
}
DeletedRecord_ErrCode DeletedRecord::set(const char* buffer)
{
	DeletedRecord_ErrCode result = extractRecord(buffer);
	if (result == dr_deleted)
	{
		this->head.makeHead();
		makeRecord();
	}
	else if (result == dr_nondeleted)
	{
		this->setDeleted(true);
		makeRecord();
	}
	return result;
}

const char* DeletedRecord::get() const
{
	return buffer;
}
const int DeletedRecord::getBufferSize() const
{
	return this->head.getPageSize();
}
void DeletedRecord::setDeleted(bool offset)
{
	this->head.setDeleted(offset);
}
bool DeletedRecord::getDeleted() const
{
	return this->head.getDeleted();
}

void DeletedRecord::setDelim(char delim = '|')
{
	this->head.setDelim(delim);
	makeRecord();
}
const char DeletedRecord::getDelim() const
{
	return this->head.getDelim();
}
const int DeletedRecord::getPrevaddr() const
{
	return this->prevaddr;
}
void DeletedRecord::setPrevaddr(int addr)
{
	this->prevaddr = addr;
	makeRecord();
}
const int DeletedRecord::getNextaddr() const
{
	return this->nextaddr;
}
void DeletedRecord::setNextaddr(int addr)
{
	this->nextaddr = addr;
	makeRecord();
}
const char* DeletedRecord::getHead() const
{
	return this->head.getHead();
}
const int DeletedRecord::getHeadSize() const
{
	return this->head.getHeadSize();
}

const int DeletedRecord::getPageNum() const
{
	return this->head.getPageNum();
}
const int DeletedRecord::getPageSize() const
{
	return this->head.getPageSize();
}
void DeletedRecord::setPageNum(int num)
{
	this->head.setPageNum(num);
	makeRecord();
}

const int DeletedRecord::getDataSize() const
{
	return this->head.getDataSize();
}


DeletedRecord_ErrCode DeletedRecord::read(std::istream& stream)
{
	//stream -> buffer
	int readaddr = (int)stream.tellg();
	stream.read(buffer, RECORD_PAGE);
	if (!stream.good())
	{
		stream.clear();
		return dr_error;
	}
	//buffer -> ����ü
	DeletedRecord_ErrCode result =  this->set(buffer);
	
	// Record�� ���¸� ��� ������ ���� Record�� �Ѿ��.
	if(result == dr_deleted || result == dr_nondeleted)
		stream.seekg(readaddr + this->getPageSize());

	return result;
}
const int DeletedRecord::write(std::ostream& stream) const
{
	// �� ó�� �κ��� Deleted Record �κ����� ä���.
	int recaddr = (int)stream.tellp();
	stream.write(this->get(), RECORD_PAGE);
	if (!stream) return -1;

	//�������� �� �������� ä���.
	char blank[RECORD_PAGE] = { 0, };
	for (int i = 0; i < this->getPageNum() - 1;i++)
		stream.write(blank, RECORD_PAGE);
	if (!stream.good()) return -1;

	return recaddr;
};

/******************************************************************************************/

int DeletedRecordList::pop(std::iostream& stream, const DeletedRecord& temp)
{
	//1. ���� �����ϰ� �ִ� ��Ʈ�� �ּҸ� �����Ѵ�.
	int now_addr = (int)stream.tellg();

	//2. ����Ʈ���� �����ϰ��� �ϴ� ���ڵ忡�� ���� ���ڵ��� �ּҸ� �����´�.
	int prev_addr = temp.getPrevaddr();
	int next_addr = temp.getNextaddr();

	//3. ���� ���ڵ带 ���� ��
	stream.seekg(prev_addr);
	DeletedRecord prev(temp.getDelim());
	if (prev.read(stream) != dr_deleted)
		return -1;
	//4. ���� ���ڵ带 ���� ���ڵ�� �����ϰ�
	prev.setNextaddr(next_addr);
	//5. �ٽ� ��Ʈ���� �����Ѵ�.
	stream.seekp(prev_addr);
	if (prev.write(stream) == -1)
		return -1;

	//6. ���� ���ڵ尡 ������ ���
	DeletedRecord next(temp.getDelim());
	if (next_addr != -1)
	{
		//7. ���� ���ڵ带 �о�鿩��
		stream.seekg(next_addr);
 		if (next.read(stream) != dr_deleted)
			return -1;
		//8. ���� ���ڵ�� ���� ��
		next.setPrevaddr(prev_addr);
		//9. ��Ʈ���� �����Ѵ�.
		stream.seekp(next_addr);
		if (next.write(stream) == -1)
			return -1;
	}

	//10. ���� �����ϰ� �ִ� ��Ʈ�� �ּҸ� �ҷ��´�.
	stream.seekg(now_addr);
	return 1;
}
int DeletedRecordList::push(std::iostream& stream, DeletedRecord& temp, int addr)
{
	//1. HEAD�� �����Ͽ� ���������� ������.
	stream.seekg(this->get_headaddr());
	if (this->read(stream) != dr_deleted)
		return -1;
	
	int prev_addr = this->get_headaddr();
	int next_addr = this->getNextaddr();
	
	//2. HEAD���� ���� ������ ���ڵ带 �����ϰ� ��Ʈ���� ����.
	this->setNextaddr(addr);
	stream.seekp(prev_addr);
	if (this->write(stream) == -1)
		return -1;

	//3. ���� HEAD�� ������� ������,
	if (next_addr != -1)
	{
		//4. �ش� ���ڵ带 ������ ���� ������ ���ڵ带 �����Ѵ�.
		stream.seekg(next_addr);
		DeletedRecord next(temp.getDelim());
		if (next.read(stream) != dr_deleted)
			return -1;
		next.setPrevaddr(addr);

		//5. �׸��� ���ڵ忡 ����.
		stream.seekp(next_addr);
		if (next.write(stream) == -1)
			return -1;
	}
		
	//6. ���� ������ ���ڵ�� HEAD�� ���� HEAD�� ����ƴ� ���ڵ带 �����ϰ�
	temp.setPrevaddr(prev_addr);
	temp.setNextaddr(next_addr);

	//7. ��Ʈ���� ����.
	stream.seekg(addr);
	stream.seekp(addr);
	if (temp.write(stream) == -1)
		return -1;

	return 1;
}