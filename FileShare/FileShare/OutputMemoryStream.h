#pragma once
class OutputMemoryStream
{
private:
	char *m_buffer;

	unsigned m_head;

	unsigned m_capacity;

	void reallocate(int newLength);

public:
	OutputMemoryStream();

	~OutputMemoryStream();

	const char *getBufferPtr() const;

	int getLength() const;

	void write(const void *data, unsigned byteCount);

	void write(int data);

	void write(unsigned data);
};

