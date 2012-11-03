#ifndef MD52_H
#define MD52_H

#include <string>
#include <fstream>

/* Type define */
typedef unsigned char byte;
typedef unsigned int uint32;

using std::string;
using std::ifstream;

/* MD5 declaration. */
class MD52 {
public:
	MD52();
	MD52(const void* input, size_t length);
	MD52(const string& str);
	MD52(ifstream& in);
	MD52(std::wstring& filename);
	void update(const void* input, size_t length);
	void update(const string& str);
	void update(ifstream& in);
	void update(std::wstring& filename);
	const byte* digest();
	string toString();
	void reset();

private:
	void update(const byte* input, size_t length);
	void final();
	void transform(const byte block[64]);
	void encode(const uint32* input, byte* output, size_t length);
	void decode(const byte* input, uint32* output, size_t length);
	string bytesToHexString(const byte* input, size_t length);

	/* class uncopyable */
	MD52(const MD52&);
	MD52& operator=(const MD52&);

private:
	uint32 _state[4];	/* state (ABCD) */
	uint32 _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
	byte _buffer[64];	/* input buffer */
	byte _digest[16];	/* message digest */
	bool _finished;		/* calculate finished ? */

	static const byte PADDING[64];	/* padding for calculate */
	static const char HEX[16];
	enum { BUFFER_SIZE = 1024 };
};


#endif /*MD5_H*/
