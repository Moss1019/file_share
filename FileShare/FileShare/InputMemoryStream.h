#pragma once
class InputMemoryStream
{
private:
	char *m_buffer;

	unsigned m_head;

	unsigned m_capacity;

public:
	InputMemoryStream(char *buffer, int byteCount);

	~InputMemoryStream();

	unsigned getRemainingSize() const;

	void read(void *data, unsigned byteCount);

	void read(int &data);

	void read(unsigned &data);
};

