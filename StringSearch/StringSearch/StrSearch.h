#pragma once

#include <string>
#include <functional>

typedef const char cchar;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef uint RegType;

class StrSearch {
public:
	enum {
		//	検索オプション
		IGNORE_CASE = 0x01,
		//WHOLE_WORD_ONLY = 0x02,					//	単語単位
		REGEXP = 0x04,

		STRSTR = 0,
		QUICK_SEARCH,
		SHIFT_AND,			//	bitap
		//SAKUSAKU,
		BNDM,
		//FORWARD_SBNDM,
		//STD_REGEX,
	};
	
public:
	StrSearch();
	~StrSearch();
	
public:
	void	clear();
	bool	setup(cchar *pat, uint opt = 0, uchar algorithm = STRSTR);
	//bool	setup(cchar *pat, int sz, uint opt = 0, uchar algorithm = STRSTR);

public:
	cchar*	doSearch(cchar* from, cchar* to);	//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
	cchar*	doSearch(cchar* text);				//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
	
protected:
	cchar*	strstr(cchar*, cchar*);		//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
	cchar*	stristr(cchar*, cchar*);	//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
	cchar*	strstr(cchar*);			//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
	cchar*	stristr(cchar*);		//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
	bool	isMatch(cchar*);
	bool	isMatchIC(cchar*);		//	大文字小文字同一視 マッチチェック
	cchar*	a_bitmap_strstr(cchar*);
	cchar*	a_BNDM(cchar*);
	cchar*	a_quick_search(cchar*);
	cchar*	a_quick_search_ic(cchar*);
	
private:
	uint	m_opt;
	int		m_plen;
	bool	m_ignoreCase;
	bool	m_wholeWordOnly;		//	単語単位
	uchar	m_algorithm;
	RegType	*m_CV;
	std::string		m_pat;			//	検索パターン
	cchar*	(StrSearch::*m_doSearchFunc)(cchar*);
	//cchar*	(StrSearch::*m_doSearchFunc)(cchar*, cchar*);
	//std::function<cchar* (cchar*, cchar*)>	m_doSearchFunc;
	ushort 	m_skipTable[0x100];			//	スキップテーブル for 0x00～0xff
	ushort 	m_rskipTable[0x100];
};
