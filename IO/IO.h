#pragma once
#include "Head.h"
#include "StringUtils.h"

class IO {
public:
	static const int IO_ERR = -1;
	static const std::size_t kBufferSize = 1 << 18;

	IO() :  read_bytes_count_(0), read_bytes_offset_(0), current_char_(0), eof_(false)/* ,
			read_buffer_(kBufferSize), write_buffer_(kBufferSize) */ {
		static_assert(kBufferSize > 32, "Size of a buffer must be greater than 32 due to comparison in IO::Flush() method.");
	}

	~IO() {
		Flush();
	}

	IO(const IO&) = delete;
	IO& operator=(const IO&) = delete;
	IO(const IO&&) = delete;
	IO& operator=(const IO&&) = delete;

	bool eof() const {
		return eof_;
	}

	inline void ShiftChar();
	inline void SkipWS();
	inline void SkipToEndl();

	inline int NextInt();
	inline unsigned int NextUint();
	inline long long NextLong();
	inline unsigned long long NextUlong();
	inline char NextCharWS();
	inline char NextChar();
	inline void NextString(char* s);
	inline void NextLine(char* s);

	IO& operator >>(int& x);
	IO& operator >>(unsigned int& x);
	IO& operator >>(long long& x);
	IO& operator >>(unsigned long long& x);
	IO& operator >>(char& x);
	IO& operator >>(char* s);

	inline void Flush();
	inline void NewLine();

	inline void PrintInt(const int& x);
	inline void PrintUint(const unsigned int& x);
	inline void PrintLong(const long long& x);
	inline void PrintUlong(const unsigned long long& x);
	inline void PrintChar(const char& ch);
	inline void PrintString(const char* s);
	//inline void PrintString(const std::string& s);
	inline void PrintLine(const char* s);
	//inline void PrintLine(const std::string& s);

	IO& operator <<(const int& x);
	IO& operator <<(const unsigned int& x);
	IO& operator <<(const long long& x);
	IO& operator <<(const unsigned long long& x);
	IO& operator <<(const char& x);
	IO& operator <<(const char* s);
	IO& operator <<(const std::string& s);
	IO& operator <<(std::ostream& (*fn)(std::ostream&));

	static void assignFiles(const std::string& task);
	static void assignFiles(const std::string& inputFile, const std::string& outputFile);
	static void assignFilesInputTxt();

private:
	using Buffer = std::array<char, kBufferSize>;
	Buffer read_buffer_;
	Buffer write_buffer_;
	std::size_t read_bytes_count_;
	std::size_t read_bytes_offset_;
	std::size_t write_bytes_offset_;
	bool eof_;
	char current_char_;

	inline void UpdateBuffer();

	template<typename T>
	T ReadNumber();

	template<typename T>
	void PrintNumber(const T& value);

};

extern bool useFastIO;
extern istream * pin;
extern ostream * pout;
extern IO io;

inline int NextInt();
inline unsigned int NextUint();
inline long long NextLong();
inline unsigned long long NextUlong();

inline void SkipWS();
inline void SkipToEndl();

inline char NextCharWS();
inline char NextChar();
inline void NextString(char * s);
inline void NextLine(char * s);

inline void Flush();
inline void NewLine();

inline void PrintInt(const int& x);
inline void PrintUint(const unsigned int& x);
inline void PrintLong(const long long& x);
inline void PrintUlong(const unsigned long long& x);
inline void PrintChar(const char& x);
inline void PrintString(const char* s);
//inline void PrintString(const std::string& s);
inline void PrintLine(const char* s);
//inline void PrintLine(const std::string& s);
