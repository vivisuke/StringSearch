#pragma once

#include <string>

typedef const char cchar;
typedef unsigned char uchar;
typedef unsigned int uint;

class StrSearch {
public:
	enum {
		//	検索オプション
		IGNORE_CASE = 0x01,
		//WHOLE_WORD_ONLY = 0x02,					//	単語単位
		REGEXP = 0x04,

		STRSTR = 0,
		//QUICK_SEARCH,
		//SHIFT_AND,
		//SAKUSAKU,
		//BNDM,
		//FORWARD_SBNDM,
		//STD_REGEX,
	};
	
public:
	StrSearch();
	~StrSearch();
	
public:
	void	clear();
	bool	setup(cchar *pat, int sz, uint opt = 0, uchar algorithm = STRSTR);

public:
	cchar*	strstr(cchar*, cchar*);		//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
	
private:
	uint	m_opt;
	bool	m_ignoreCase;
	bool	m_wholeWordOnly;		//	単語単位
	uchar	m_algorithm;
	std::string		m_pat;			//	検索パターン
	cchar*	(StrSearch::*m_strstrAFunc)(cchar*, cchar*);
};
