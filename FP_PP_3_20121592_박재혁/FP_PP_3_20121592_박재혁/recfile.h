#ifndef RECFILE_H
#define RECFILE_H

#include "buffile.h"
#include "iobuffer.h"
// template class to support direct read and write of records
// The template parameter RecType must support the following
//	int Pack (BufferType &); pack record into buffer
//	int Unpack (BufferType &); unpack record from buffer

template <class RecType>
class RecordFile: public BufferFile
{public:
	int Read (RecType & record, int recaddr = -1);
	int Write (const RecType & record, int recaddr = -1);
	int Append (const RecType & record, int recaddr = -1);

	/* for project 1 */
	// return the record address corresponding to key 
	int Find(const RecType& record);
	int Remove(const RecType& record, int recaddr = -1);
	int Insert(const RecType& record);
	int Update(const RecType& record, int recaddr = -1);

	inline int NextRecord() { return (int)File.tellg();  }

	RecordFile (IOBuffer & buffer): BufferFile (buffer) {}
};

// template method bodies
template <class RecType>
int RecordFile<RecType>::Read (RecType & record, int recaddr = -1)
{
	int readAddr, result;
	readAddr = BufferFile::Read (recaddr);
	if (readAddr==-1) 
		return -1;
	result = record . Unpack (Buffer);
	if (!result) 
		return -1;
	return readAddr;
}

template <class RecType>
int RecordFile<RecType>::Write (const RecType & record, int recaddr = -1)
{
	int result;
	result = record . Pack (Buffer);
	if (!result) return -1;
	return BufferFile::Write (recaddr);
}

template <class RecType>
int RecordFile<RecType>::Remove(const RecType& record, int recaddr = -1)
{
	int removeAddr;
	if (recaddr == -1)
	{
		removeAddr = Find(record);
		if (removeAddr == -1) return -1;
	}
	else
		removeAddr = recaddr;
	int result = BufferFile::Remove(removeAddr);
	BufferFile::Rewind();
	return result;
}

template <class RecType>
int RecordFile<RecType>::Append (const RecType & record, int recaddr = -1)
{
	int result;
	result = record . Pack (Buffer);
	if (!result) return -1;
	return BufferFile::Append ();
}

template <class RecType>
int RecordFile<RecType>::Insert(const RecType & record)
{
	int result;
	result = record.Pack(Buffer);
	if (!result) return -1;
	result = BufferFile::Insert();
	BufferFile::Rewind();
	return result;
}

template <class RecType>
int RecordFile<RecType>::Find(const RecType& record)
{
	int result;
	result = record.Pack(Buffer);
	if (!result) return -1;
	//const char* key = record.getKey(seed);

	//1. Start the beginning of file
	BufferFile::Rewind();

	RecType target = RecType();
	//2. Read sequentially and compare with record
	for (result = this->Read(target); result != EOF; result = this->Read(target))
	{
		// if you find, break
		if (target == record)
			break;
	}

	//3. Rewind the file
	BufferFile::Rewind();
	return result;
}
template <class RecType>
int RecordFile<RecType>::Update(const RecType & record, int recaddr = -1)
{
	int result;
	result = this->Remove(record, recaddr);
	if (result == -1) return -1;
	result = this->Insert(record);
	return result;
}

#endif