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
	//m_pat = string(pat, pat+sz);
	switch( m_algorithm = algorithm ) {
	case STRSTR:
		if( (opt & IGNORE_CASE) == 0 )
			m_doSearchFunc = &StrSearch::strstr;		//	英字大文字小文字区別
		else
			m_doSearchFunc = &StrSearch::stristr;		//	英字大文字小文字同一視
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
		if( *text == '\0' )		//	検索テキスト末尾
			return false;
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
		if( *text == '\0' )		//	検索テキスト末尾
			return false;
	}
	return true;
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
