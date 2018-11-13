// deltext.cc
#include "delim.h"
#include <string.h>

//class DelimFieldBuffer

// public members

DelimFieldBuffer :: DelimFieldBuffer(char delim, int maxBytes)
 // construct with a maximum of maxFields
: VariableLengthBuffer (maxBytes)
{
	Init (delim);
}

void DelimFieldBuffer :: Clear ()
// clear fields from buffer
{
	VariableLengthBuffer::Clear();
}

int DelimFieldBuffer :: Pack (const void * field, int size)
// set the value of the next field of the buffer;
// if size = -1 (default) use strlen(field) as Delim of field
// return number of bytes packed, -1 if error
{
	// look for delimeter in field!
	short len; // length of string to be packed
	if (size >= 0) len = size;
	else len = strlen ((char*)field);
	if (len > (short)strlen((char*)field)) // field is too short!
		return -1;
	int start = NextByte; // first character to be packed
	NextByte += len + 1;
	if (NextByte > MaxBytes) return -1;
	memcpy (&Buffer[start], field, len);
	Buffer [start+len] = Delim; // add delimeter
	BufferSize = NextByte;
	return len;
}

int DelimFieldBuffer :: Unpack (void * field, int maxBytes)
// extract the value of the next field of the buffer
// return the number of bytes extracted, -1 if error
{
	int len = -1; // length of packed string
	int start = NextByte; // first character to be unpacked
	for (int i = start; i < BufferSize; i++)
		if (Buffer[i] == Delim) 
			{len = i - start; break;}
	if (len == -1) return -1; // delimeter not found
	NextByte += len + 1;
	if (NextByte > BufferSize) return -1;
	// check for maxBytes
	memcpy (field, &Buffer[start], len);
	if (maxBytes > len || maxBytes == -1)
		((char*)field) [len] = 0; // zero termination for string 
	return len;
}

int DelimFieldBuffer::Read(istream &stream)
{
	// ���� Stream�� ���� �� �ִ��� üũ�Ѵ�.
	if (stream.eof()) return -1;
	int recaddr = (int)stream.tellg();
	Clear();
	
	// �������� ���� Record�� ���� ������ �д´�.
	DeletedRecord temp(this->Delim);
	DeletedRecord_ErrCode result;
	for (result = temp.read(stream); result == dr_deleted; result = temp.read(stream))
		recaddr = (int)stream.tellg();

	// ���� record�� �������� ���� Record��
	if(result == dr_nondeleted)
	{
		stream.seekg(recaddr + temp.getHeadSize());
		// Data �κ��� Buffer�� �о�´�.
		stream.read(Buffer, temp.getDataSize());
		BufferSize = strlen(Buffer);
		if (!stream.good()) 
		{
			stream.clear(); 
			return -1; 
		}

		// ���� record���� ���۽�Ű��, ���� �����ϰ� �ִ� record�� �ּҸ� return�Ѵ�.
		stream.seekg(recaddr + temp.getPageSize());
		return recaddr;
	}
	return -1;
}
int DelimFieldBuffer::Write(ostream &stream) const
{
	int recaddr = (int)stream.tellp();

	//1. ���� record�� head�� �����Ѵ�.
	int pageNum = DeletedRecordHead::calPageNum(BufferSize);
	DeletedRecordHead ndrhead(Delim, false, pageNum);
	//2. head�� record�� ����.
	stream.write(ndrhead.getHead(), ndrhead.getHeadSize());
	if (!stream) return -1;
	//3. data�� record�� ����.
	stream.write(Buffer, ndrhead.getDataSize());
	if (!stream.good()) return -1;
	
	return recaddr;
}
int DelimFieldBuffer::Remove(iostream &stream)
{
	//check the stream
	if (stream.eof()) return -1;
	
	//1.���� ��ġ�� record�� �о ���� �������� ���� ���ڵ����� Ȯ���Ѵ�.
	int readaddr = (int)stream.tellg();
	DeletedRecord temp(this->Delim);
	if (temp.read(stream) != dr_nondeleted)
		return -1;
	
	//2. ������ �ٷ� �� record�� deleted_record���� Ȯ��
	DeletedRecord next(this->Delim);
	int nextaddr = readaddr + temp.getPageSize();
	stream.seekg(nextaddr);
	while(next.read(stream) == dr_deleted)
	{
		// 2 - 1. ������ linked_list���� pop�ϰ� ���� deleted_record�� ����
		if (dList.pop(stream, next) == -1)
			return -1;
		
		int page_num = temp.getPageNum() + next.getPageNum();
		temp.setPageNum(page_num);

		nextaddr = (int)stream.tellg();
	}
	
	//3. �ϼ��� record�� linked_list�� �߰��Ѵ�.
	if (dList.push(stream, temp, readaddr) == -1)
		return -1;

	return readaddr;
}
int DelimFieldBuffer::Insert(iostream &stream)
{
	//1. ������ �ϴ� ���ڵ��� page number�� ���Ѵ�,
	int pageNum = DeletedRecordHead::calPageNum(BufferSize);
	DeletedRecord temp(this->Delim);
	
	int removeAddr;
	//2. ������ ���ڵ� ����Ʈ���� ���� ���ڵ�� �Ѿ��.
	for (removeAddr = dList.getNextaddr(); removeAddr != -1; removeAddr = temp.getNextaddr())
	{
		stream.seekg(removeAddr);
		if (temp.read(stream) != dr_deleted)
			return -1;

		//3-1. �ƴϸ� �ٷ� �� �������� ������ ���ڵ尡 �ִ��� Ȯ���Ѵ�.
		DeletedRecord next(this->Delim);
		int nextaddr = (int)stream.tellg();
		int whole_pagenum = temp.getPageNum();
		while (next.read(stream) == dr_deleted)
		{
			//3-1. ������ �� ���ڵ带 ���� ������ ���ڵ� �������� �����ϰ� �ݺ��Ѵ�.
			if (dList.pop(stream, next) == -1)
				return -1;

			//3-1-2. page number�� ���������� �ٲ��.
			whole_pagenum = temp.getPageNum() + next.getPageNum();
			
			// 3-1-3. pop ���� �� temp�� ����� ���ڵ��� �ּҰ� �ٲ���� �� �����Ƿ�
			//temp�� record�� �����´�.

			nextaddr = (int)stream.tellg();
			stream.seekg(removeAddr);
			if (temp.read(stream) != dr_deleted)
				return -1;
			stream.seekg(nextaddr);
		}
		temp.setPageNum(whole_pagenum);

		//3 - 2. ������ ���ڵ� ������ ũ�� >= ������ �ϴ� ���ڵ� ũ���̸�
		if (pageNum <= temp.getPageNum())
		{
			// ���� ���� ���ڵ带 ������ ���ڵ� ����Ʈ���� �����Ѵ�.
			stream.seekp(removeAddr);
			if (dList.pop(stream, temp) == -1)
				return -1;

			// ������ ���ڵ� �������� ������ �ϴ� ���ڵ带 �߰��Ѵ�.
			stream.seekp(removeAddr);
			if (this->Write(stream) == -1)
				return -1;

			// ���� ���ڵ� ����� ��� ������ ���ڵ� ����Ʈ�� �߰��Ѵ�.
			int rest_pageNum = temp.getPageNum() - pageNum;
			if (rest_pageNum > 0)
			{
				temp.setPageNum(rest_pageNum);
				if (dList.push(stream, temp, removeAddr + RECORD_PAGE*pageNum) == -1)
					return -1;
			}
			
			return removeAddr;
		}
	}
	//4. ������ ���ڵ� ����Ʈ���� ������ ������, ���ڵ� �� �ڿ� ����.
	stream.seekp(0, ios::end);
	if (this->Write(stream) == -1)
		return -1;

	return removeAddr;
}
int DelimFieldBuffer :: ReadHeader (istream & stream)
// read header: contains delimeter character
{
	char ch;
	int result;
	result = VariableLengthBuffer::ReadHeader (stream);
	if (!result) return FALSE;
	result = dList.ReadHeader(stream);
	if (!result) return FALSE;

	stream . get (ch);
	if (!Initialized)
	{
		SetDefaultDelim(ch);
		return TRUE;
	}
	if (ch != Delim) return FALSE;

	return (int)stream . tellg ();
}

int DelimFieldBuffer :: WriteHeader (ostream & stream) const
// write header into file: contains the delimeter character
{
	if (!Initialized) return FALSE;
	int result;
	result = VariableLengthBuffer::WriteHeader (stream);
	if (!result) return FALSE;
	
	result = dList.WriteHeader(stream);
	if (!result) return FALSE;
	//stream . put (Delim);
	
	return (int)stream . tellp ();
}

void DelimFieldBuffer :: Print (ostream & stream) const
{
	VariableLengthBuffer::Print (stream);
	stream << " Delimeter '"<<Delim<<"'"<<endl;
}

// Protected members

int DelimFieldBuffer :: Init (char delim)
 // construct with a maximum of maxFields
{
	Initialized = TRUE;
	Clear ();
	if (delim == -1) Delim = DefaultDelim;
	else Delim = delim;

	return TRUE;
}

void DelimFieldBuffer :: SetDefaultDelim (char delim)
{
	DefaultDelim = delim;
}

// initialize static protected element

char DelimFieldBuffer :: DefaultDelim = 0;