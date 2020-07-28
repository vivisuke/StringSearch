#include "StrSearch.h"

using namespace std;

StrSearch::StrSearch()
{
}
StrSearch::~StrSearch()
{
}
bool StrSearch::setup(cchar *pat, /*int sz,*/ uint opt, uchar algorithm)
{
	m_pat = pat;
	const int plen = m_plen = m_pat.size();
	m_ignoreCase = (opt & IGNORE_CASE) != 0;
	const int WORD_SIZE = 0x10000;
	//m_pat = string(pat, pat+sz);
	switch( m_algorithm = algorithm ) {
	case STRSTR:
		if( (opt & IGNORE_CASE) == 0 )
			m_doSearchFunc = &StrSearch::strstr;		//	英字大文字小文字区別
		else
			m_doSearchFunc = &StrSearch::stristr;		//	英字大文字小文字同一視
		break;
	case STRCHRSTR:
		if( (opt & IGNORE_CASE) == 0 )
			m_doSearchFunc = &StrSearch::strchrstr;		//	英字大文字小文字区別
		else
			m_doSearchFunc = &StrSearch::stristr;		//	英字大文字小文字同一視
		break;
	case SHIFT_AND:
		if( plen > 32 ) return false;;
		for(int i = 0; i < 0x100; ++i)					//	skip テーブル初期化
			m_skipTable[i] = m_rskipTable[i] = plen;
		m_CV = new RegType[WORD_SIZE]();		//	() for 0 で初期化
		if( !m_ignoreCase ) {
			for(int i = 0, mask = 1; i < plen; ++i, mask<<=1)
				m_CV[pat[i]] |= mask;
			for(int i = 0; i < plen; ++i)					//	for 順方向検索
				m_skipTable[(uchar)pat[i]] = plen - i - 1;
		} else {
			for(int i = 0, mask = 1; i < plen; ++i, mask<<=1) {
				m_CV[tolower(pat[i])] |= mask;
				m_CV[toupper(pat[i])] |= mask;
			}
			for(int i = 0; i < plen; ++i) {					//	for 順方向検索
				m_skipTable[(uchar)tolower(pat[i])] = plen - i - 1;
				m_skipTable[(uchar)toupper(pat[i])] = plen - i - 1;
			}
		}
		//if( algorithm == SHIFT_AND )
			m_doSearchFunc = &StrSearch::a_bitmap_strstr;
		break;
	case BNDM: 
		if( plen > 32 ) return false;;
		m_CV = new RegType[WORD_SIZE]();		//	() for 0 で初期化
		if( !m_ignoreCase ) {
			for(int i = plen - 1, mask = 1; i >= 0; --i, mask<<=1)
				m_CV[pat[i]] |= mask;
		} else {
			for(int i = plen - 1, mask = 1; i >= 0; --i, mask<<=1) {
				m_CV[tolower(pat[i])] |= mask;
				m_CV[toupper(pat[i])] |= mask;
			}
		}
		m_doSearchFunc = &StrSearch::a_BNDM;
		break;
	default:
		m_doSearchFunc = nullptr;
		return false;
	}
	return true;
}
cchar* StrSearch::doSearch(cchar* text)		//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
{
	if( m_doSearchFunc == nullptr )
		return nullptr;
	return (this->*m_doSearchFunc)(text);
}
#if	0
cchar* StrSearch::doSearch(cchar* from, cchar* to)		//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
{
	if( m_doSearchFunc == nullptr )
		return nullptr;
	return (this->*m_doSearchFunc)(from, to);
}
#endif

bool StrSearch::isMatch(cchar* text)
{
	cchar* pat = &m_pat[0];
	char ch;
	while( (ch = *pat++) != '\0' ) {
		if( *text++ != ch )		//	文字不一致
			return false;
		//if( *text == '\0' )		//	検索テキスト末尾
		//	return false;
	}
	return true;
}
bool StrSearch::isMatchIC(cchar* text)		//	大文字小文字同一視 マッチチェック
{
	cchar* pat = &m_pat[0];
	char ch;
	while( (ch = *pat++) != '\0' ) {
		if( tolower(*text++) != tolower(ch) )		//	文字不一致
			return false;
		//if( *text == '\0' )		//	検索テキスト末尾
		//	return false;
	}
	return true;
}
cchar* StrSearch::strchrstr(cchar*text)			//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
{
	cchar *ptr = strchr(text, m_pat[0]);
	while( ptr != nullptr ) {
		if( !strncmp(ptr, &m_pat[0], m_pat.size()) )
			return ptr;
		ptr = strchr(ptr+1, m_pat[0]);
	}
	return ptr;
}
cchar* StrSearch::strstr(cchar* text)		//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
{
	while( *text != '\0' ) {
		if( isMatch(text) )		//	m_pat とマッチするか？
			return text;
		++text;
	}
	return nullptr;
}
cchar* StrSearch::stristr(cchar* text)		//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
{
	while( *text != '\0' ) {
		if( isMatchIC(text) )		//	m_pat とマッチするか？
			return text;
		++text;
	}
	return nullptr;
}
cchar* StrSearch::strstr(cchar* from, cchar* to)		//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
{
	while( from < to ) {
		cchar* ptr = from;
		cchar* pat = &m_pat[0];
		for (;;) {
			char ch;
			if( (ch = *pat++) == '\0' )		//	パターン末尾に達した場合
				return from;
			if( ptr >= to )				//	被検索テキスト末尾に達した場合
				return nullptr;
			if( *ptr++ != ch )
				break;	//	不一致
		}
		++from;
	}
	return nullptr;
}
cchar* StrSearch::stristr(cchar* from, cchar* to)	//	マッチした場合はその先頭アドレスを返す、マッチしない場合は nullptr を返す
{
	while( from < to ) {
		cchar* ptr = from;
		cchar* pat = &m_pat[0];
		for (;;) {
			char ch;
			if( (ch = *pat++) == '\0' )		//	パターン末尾に達した場合
				return from;
			if( ptr >= to )				//	被検索テキスト末尾に達した場合
				return nullptr;
			if( tolower(*ptr++) != tolower(ch) )	//	大文字・小文字同一視比較
				break;	//	不一致
		}
		++from;
	}
	return nullptr;
}
cchar* StrSearch::a_bitmap_strstr(cchar* text)
{
	RegType R = 0;
	const int plen = m_pat.size();
	RegType Matched = 1 << (plen - 1);
	while( *text != '\0' ) {
		R = ((R<<1) + 1) & m_CV[(uchar)*text++];
		if( (R & Matched) ) return text - 1;
	}
	return nullptr;
}
cchar* StrSearch::a_BNDM(cchar* text)
{
	cchar* tlast = std::strchr(text, '\0');
	cchar* tend = tlast - m_plen;
	while( text <= tend ) {
		int i = m_plen - 1;
		int last = m_plen;
		RegType d = ~0;
		while( i >= 0 && d != 0 ) {
			d &= m_CV[(uchar)text[i]];
			--i;
			if( d != 0 ) {
				if( i >= 0 )
					last = i + 1;
				else
					return text;
			}
			d <<= 1;
		}
		text += last;
	}
	return nullptr;
}
cchar* StrSearch::a_quick_search(cchar* text)
{
	return nullptr;
}
cchar* StrSearch::a_quick_search_ic(cchar* text)
{
	return nullptr;
}
