#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <assert.h>
#include "StrSearch.h"

using namespace std;

typedef const char cchar;

random_device g_rd;
#if	1
auto g_seed = g_rd();
mt19937 g_mt(g_seed);
//mt19937 g_mt(g_rd());
#else
int g_seed = 3516212911;
mt19937 g_mt(g_seed);
#endif

int g_plen;			//	検索文字列長
cchar* g_pat;		//	検索文字列
std::string g_lowerPat;		//	小文字変換した検索文字列
int g_BMH[256];		//	スキップテーブル

void test_searchRandom();
void test_search_aaaaZ();
void test_search_aaaaaaaaaZ();
void test_my_strstr();
void test_BMH();
void measureTime(const int PLEN = 5, bool bWorst = false, int LEN = 1024*1024*100);

int strSearchIC(const std::string& text, int pos, const std::string& pat)
{
  auto itr = std::search(
    text.begin() + pos, text.end(),
    pat.begin(),   pat.end(),
    [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
  );
  if( itr != text.end() )
  	return itr - text.begin();
  else
  	return -1;
}
int main()
{
#if	0
	StrSearch src;
	src.setup("hoge", 0, StrSearch::STRCHRSTR);
	string buffer = "abcdefgHOGEhohohogeaccdzzz";
	cchar* ptr = src.doSearch(&buffer[0]);
#endif
#if	1
	//test_my_strstr();
	//test_BMH();
	//test_searchRandom();
	//test_search_aaaaZ();
	//test_search_aaaaaaaaaZ();
#if 0
	measureTime(5, /*worst:*/false);		//	pat：５文字
	measureTime(10, /*worst:*/false);
	measureTime(20, /*worst:*/false);
#else
	measureTime(5, /*worst:*/true);
	measureTime(10, /*worst:*/true);
	measureTime(20, /*worst:*/true);
#endif
#endif
#if	0
	StrSearch src;
	src.setup("hoge");
	string buffer = "abcdefgHOGEhohohogeaccdzzz";
	cchar *ptr = src.doSearch(&buffer[0]);
	//cchar *ptr = src.doSearch(&buffer[0], &buffer[0]+buffer.size());
	if( ptr != nullptr )
		cout << "ptr = " << ptr << "\n";
	//
	src.setup("hoge", StrSearch::IGNORE_CASE);
	cchar* ptr2 = src.doSearch(&buffer[0]);
	//cchar* ptr2 = src.doSearch(&buffer[0], &buffer[0] + buffer.size());
	if (ptr2 != nullptr)
		cout << "ptr2 = " << ptr2 << "\n";
#endif
#if	0
	StrSearch src;
	src.setup("aaaaZ");
	string buffer = "aaaaaZ";
	cchar *ptr = src.doSearch(&buffer[0]);
#endif
	//
    std::cout << "\nOK\n";
}
void test_searchRandom()
{
	cout << "test_searchRandom():\n";
	StrSearch src;
	string buf;
	buf.resize(1000*1000);		//	100万文字
	for(auto& x: buf) x = 'a' + g_mt() % 16;
	//
	string pat;
	const int PAT_LEN = 4;
	for (int i = 0; i < PAT_LEN; ++i) pat.push_back('a' + g_mt() % 16);
	cout << "pat = '" << pat << "'\n";
	if (true) {
		cout << "std::strstr():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = std::strstr(ptr, &pat[0]);
				//ptr = strcasestr(ptr, &pat[0]);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "std::search(), opt = ignore_case:\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			int pos = 0;
			for(;;) {
				pos = strSearchIC(buf, pos, pat);
				if( pos < 0 ) break;
				++nMatch;
				++pos;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
#if	0
	if (true) {
		cout << "std::strcasestr():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			cchar* ptr = &buf[0];
			for (;;) {
				ptr = strcasestr(ptr, &pat[0]);
				if (ptr == nullptr) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
#endif
	if (true) {
		cout << "StrSearch::strchrstr():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], 0, StrSearch::STRCHRSTR);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::strstr():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0]);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::stristr():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], StrSearch::IGNORE_CASE);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::bitap():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], 0, StrSearch::SHIFT_AND);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::bitap(), opt = ignore_case:\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], StrSearch::IGNORE_CASE, StrSearch::SHIFT_AND);
			cchar* ptr = &buf[0];
			for (;;) {
				ptr = src.doSearch(ptr);
				if (ptr == nullptr) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::BNDM():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], 0, StrSearch::BNDM);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	//
	cout << "\n";
}
void test_search_aaaaZ()
{
	cout << "test_search_aaaaZ():\n";
	StrSearch src;
	string buf(1000*1000, 'a');		//	100万文字の 'a'
	buf.back() = 'Z';				//	最後の文字のみ 'Z'
	string pat = "aaaaZ";
	//
	if (true) {
		cout << "std::strstr():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = std::strstr(ptr, &pat[0]);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::strstr():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0]);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::stristr():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], StrSearch::IGNORE_CASE);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::bitap():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], 0, StrSearch::SHIFT_AND);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::BNDM():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], 0, StrSearch::BNDM);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	//
	cout << "\n";
}
void test_search_aaaaaaaaaZ()
{
	cout << "test_search_aaaaaaaaaZ():\n";
	StrSearch src;
	string buf(1000*1000, 'a');		//	100万文字の 'a'
	buf.back() = 'Z';				//	最後の文字のみ 'Z'
	string pat = "aaaaaaaaaZ";
	//
	if (true) {
		cout << "std::strstr():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = std::strstr(ptr, &pat[0]);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::strstr():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0]);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::stristr():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], StrSearch::IGNORE_CASE);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::bitap():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], 0, StrSearch::SHIFT_AND);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	if (true) {
		cout << "StrSearch::BNDM():\n";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], 0, StrSearch::BNDM);
			cchar* ptr = &buf[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout << "nMatch = " << nMatch << ", ";
		cout << "dur = " << msec << "msec\n";
	}
	//
	cout << "\n";
}
bool startsWithIC(cchar* text, cchar* pat)
{
	while( *pat != '\0' ) {
		if( toupper(*pat++) != toupper(*text++) )		//	ポインタを進めながら、文字が一致するかチェック
			return false;			//	不一致の場合は false を返す
	}
	return true;	//	パターン末尾まで一致した場合は true を返す
}
cchar* my_strcasestr(cchar* text, cchar* pat)		//	大文字小文字同一視
{
	while( *text != '\0' ) {	//	末尾でない間ループ
		if( startsWithIC(text, pat) )	//	text 位置に pat があるかチェック（大文字小文字同一視）
			return text;		//	あればマッチなので、その場所（text）を返す
		++text;		//	被検索文字列位置をひとつ進める
	}
	return nullptr;
}
bool startsWith(cchar* text, cchar* pat)
{
	while( *pat != '\0' ) {
		if( *pat++ != *text++ )		//	ポインタを進めながら、文字が一致するかチェック
			return false;			//	不一致の場合は false を返す
	}
	return true;	//	パターン末尾まで一致した場合は true を返す
}
cchar* my_strstr(cchar* text, cchar* pat)
{
	while( *text != '\0' ) {	//	末尾でない間ループ
		if( startsWith(text, pat) )	//	text 位置に pat があるかチェック
			return text;		//	あればマッチなので、その場所（text）を返す
		++text;		//	被検索文字列位置をひとつ進める
	}
	return nullptr;
}
cchar* my_strstr2(cchar* text, cchar* pat)		//	strchr を使用する版
{
	while ((text = strchr(text, pat[0])) != nullptr) {
		if( startsWith(text+1, pat+1) )	//	text 位置に pat があるかチェック
			return text;		//	あればマッチなので、その場所（text）を返す
		++text;		//	被検索文字列位置をひとつ進める
	}
	return nullptr;
}
void test_my_strstr()
{
	string buffer;
	string pat;
	if (true) {
		buffer = "abcabcabcd";
		pat = "abc";
		cchar* ptr = my_strstr(&buffer[0], &pat[0]);
		assert(ptr == &buffer[0]);
		//
		pat = "abcd";
		ptr = my_strstr(&buffer[0], &pat[0]);
		assert(ptr == &buffer[0] + 6);
	}
	if( true ) {
		buffer.resize(1024*1024);			//	1メガバイト
		for(auto& x: buffer) x = (g_mt() % 26) + 'a';		//	'a' - 'z'
		const int PLEN = 4;		//	パターン長
		pat.resize(PLEN);
		for (int i = 0; i != PLEN; ++i) pat[i] = (g_mt() % 26) + 'a';		//	'a' - 'z'
		vector<cchar*> lst;		//	マッチ箇所リスト
		cchar *ptr = strstr(&buffer[0], &pat[0]);
		while( ptr != nullptr ) {
			lst.push_back(ptr);
			ptr = strstr(ptr+1, &pat[0]);
		}
		cout << "lst.size() = " << lst.size() << "\n";
		//
		ptr = &buffer[0];
		for (int i = 0; i != lst.size(); ++i) {
			ptr = my_strstr(ptr, &pat[0]);
			assert( ptr == lst[i] );
			++ptr;
		}
		ptr = my_strstr(ptr, &pat[0]);
		assert( ptr == nullptr );
		cout << "OK\n";
	}
}
bool setupIcBMH(cchar* pat)		//	大文字小文字同一視版
{
	if( !(g_plen = strlen(g_pat = pat)) )
		return false;		//	検索文字列が空の場合
	g_lowerPat.resize(g_plen);
	for(int i = 0; i != g_plen; ++i) {
		g_lowerPat[i] = tolower(pat[i]);
	}
	for(auto& x: g_BMH) x = g_plen;	//	テーブル要素をすべて検索文字列長に設定
	for(int i = 0; i != g_plen - 1; ++i) {
		uchar ch = *pat++;
		g_BMH[tolower(ch)] = g_BMH[toupper(ch)] = g_plen - i - 1;		//	文字 *pat でのスキップ可能長
	}
	//	パターン最後の文字がそれ以前に無い場合は g_BMH[ch] = g_plaen のまま
	//if( g_BMH[*pat] == g_plaen )
	//if( g_BMH[(uchar)*--pat] == 0 )			//	最後の文字がそれ以前にない
	//	g_BMH[(uchar)*pat] = g_plen;		//	文字 *pat でのスキップ可能長
	return true;
}
bool setupBMH(cchar* pat)
{
	if( !(g_plen = strlen(g_pat = pat)) )
		return false;		//	検索文字列が空の場合
	for(auto& x: g_BMH) x = g_plen;	//	テーブル要素をすべて検索文字列長に設定
	for(int i = 0; i != g_plen - 1; ++i)
		g_BMH[(uchar)*pat++] = g_plen - i - 1;		//	文字 *pat でのスキップ可能長
	//	パターン最後の文字がそれ以前に無い場合は g_BMH[ch] = g_plaen のまま
	//if( g_BMH[*pat] == g_plaen )
	//if( g_BMH[(uchar)*--pat] == 0 )			//	最後の文字がそれ以前にない
	//	g_BMH[(uchar)*pat] = g_plen;		//	文字 *pat でのスキップ可能長
	return true;
}
bool startsWithBkw(cchar* text, cchar* pat, int plen)		//	末尾からｎ文字比較
{
	while( --plen >= 0 ) {
		if( text[plen] != pat[plen] )
			return false;		//	不一致
	}
	return true;	//	一致
}
bool startWithIcBkw(cchar* text, cchar* pat, int plen)		//	末尾からｎ文字比較
{
	while( --plen >= 0 ) {
		if( tolower(text[plen]) != pat[plen] )
			return false;		//	不一致
	}
	return true;	//	一致
}
cchar* strstrBMH(cchar* text0, cchar* text, cchar *endptr)
{
	while( text + g_plen <= endptr ) {	//	検索パターン分の文字列が残っている間ループ
		if( !strncmp(text, g_pat, g_plen) )		//	検索パーンと一致するか？
			return text;				//	一致すれば一致アドレスを返す
		auto skip = g_BMH[(uchar)text[g_plen-1]];		//	for Test, Debug
		text += g_BMH[(uchar)text[g_plen-1]];	//	スキップテーブルの値だけ、text を進める
		//auto ix = text - text0;
		//if( ix >= 6209900 && ix < 6209999 )
		//	cout << ix << " ";
	}
	return nullptr;
}
cchar* strstrBMH_fwd(cchar* text, cchar* endptr)
{
	while (text + g_plen <= endptr) {	//	検索パターン分の文字列が残っている間ループ
		if (!strncmp(text, g_pat, g_plen))		//	検索パーンと一致するか？（大文字小文字同一視）
			return text;				//	一致すれば一致アドレスを返す
		//auto skip = g_BMH[(uchar)text[g_plen-1]];		//	for Test, Debug
		text += g_BMH[(uchar)text[g_plen - 1]];	//	スキップテーブルの値だけ、text を進める
	}
	return nullptr;
}
cchar* strstrBMH(cchar* text, cchar* endptr)
{
	while (text + g_plen <= endptr) {	//	検索パターン分の文字列が残っている間ループ
		if (startsWithBkw(text, g_pat, g_plen))		//	検索パーンと一致するか？（大文字小文字同一視）
			return text;				//	一致すれば一致アドレスを返す
		//auto skip = g_BMH[(uchar)text[g_plen-1]];		//	for Test, Debug
		text += g_BMH[(uchar)text[g_plen - 1]];	//	スキップテーブルの値だけ、text を進める
	}
	return nullptr;
}
cchar* strcasestrBMH_fwd(cchar* text, cchar *endptr)
{
	while( text + g_plen <= endptr ) {	//	検索パターン分の文字列が残っている間ループ
		if( !_strnicmp(text, g_pat, g_plen) )		//	検索パーンと一致するか？（大文字小文字同一視）
			return text;				//	一致すれば一致アドレスを返す
		//auto skip = g_BMH[(uchar)text[g_plen-1]];		//	for Test, Debug
		text += g_BMH[(uchar)text[g_plen-1]];	//	スキップテーブルの値だけ、text を進める
	}
	return nullptr;
}
cchar* strcasestrBMH(cchar* text, cchar *endptr)
{
	while( text + g_plen <= endptr ) {	//	検索パターン分の文字列が残っている間ループ
		if( startWithIcBkw(text, &g_lowerPat[0], g_plen) )		//	検索パーンと一致するか？（大文字小文字同一視）
			return text;				//	一致すれば一致アドレスを返す
		//auto skip = g_BMH[(uchar)text[g_plen-1]];		//	for Test, Debug
		text += g_BMH[(uchar)text[g_plen-1]];	//	スキップテーブルの値だけ、text を進める
	}
	return nullptr;
}
void test_BMH()
{
	string buffer;
	string pat;
	if( true ) {
		buffer = "fyefkkdtbefnwciooysojgjyjkbrixzjsejoxyawtq";
		//buffer = "sxlbvruqnbomizjyhqvsgghmnmcuxbeckebuarfyefkkdtbefnwciooysojgjyjkbrixzjsejoxyawtq";
		pat = "yefkk";
		if( !setupBMH(&pat[0]) ) assert(0);
		cchar* ptr = strstrBMH(&buffer[0], &buffer[0] + buffer.size());
		assert( ptr == &buffer[39] );
	}
	if( true ) {
		buffer = "abcabcabcd";
		pat = "abc";
		if( !setupBMH(&pat[0]) ) assert(0);
		cchar* ptr = strstrBMH(&buffer[0], &buffer[0] + buffer.size());
		assert( ptr == &buffer[0] );
		//
		pat = "abcd";
		if( !setupBMH(&pat[0]) ) assert(0);
		ptr = strstrBMH(&buffer[0], &buffer[0] + buffer.size());
		assert( ptr == &buffer[0] + 6);
	}
	if( true ) {
		buffer.resize(1024*1024);			//	1メガバイト
		for(auto& x: buffer) x = (g_mt() % 26) + 'a';		//	'a' - 'z'
		const int PLEN = 4;		//	パターン長
		pat.resize(PLEN);
		for (int i = 0; i != PLEN; ++i) pat[i] = (g_mt() % 26) + 'a';		//	'a' - 'z'
		vector<cchar*> lst;		//	マッチ箇所リスト
		cchar *ptr = strstr(&buffer[0], &pat[0]);
		while( ptr != nullptr ) {
			lst.push_back(ptr);
			ptr = strstr(ptr+1, &pat[0]);
		}
		cout << "lst.size() = " << lst.size() << "\n";
		//
		setupBMH(&pat[0]);
		ptr = &buffer[0];
		cchar* endptr = &buffer[0] + buffer.size();
		for (int i = 0; i != lst.size(); ++i) {
			ptr = strstrBMH(ptr, endptr);
			assert( ptr == lst[i] );
			++ptr;
		}
		ptr = strstrBMH(ptr, endptr);
		assert( ptr == nullptr );
		cout << "OK\n";
	}
#if	0
	buffer.resize(1024*1024);			//	1メガバイト
	//buffer.resize(1024*1024*100);		//	100メガバイト
	//buffer = "ababcab";
	//buffer = "axcababcab";
	//buffer = "abababcab";
	for(auto& x: buffer) x = (g_mt() % 26) + 'a';		//	'a' - 'z'
	const int PLEN = 3;		//	パターン長
	for (int i = 0; i != PLEN; ++i) pat.push_back((g_mt() % 26) + 'a');		//	'a' - 'z'
	//pat = "abc";
	auto p = strstr(&buffer[0], &pat[0]);
	cout << "&buffer[0] = " << (int)&buffer[0] << "\n";
	cout << "p = " << (int)p << "\n";
	setupBMH(&pat[0]);
	cchar *ptr = &buffer[0];
	cchar *endptr = &buffer[0] + buffer.size();
	for (;;) {
		ptr = strstrBMH(ptr, endptr);
		cout << "ptr = " << (int)ptr << "\n";
		if( ptr == nullptr )
			break;
		++ptr;
	}
#endif
}
void measureTime(const int PLEN, bool bWorst, const int LEN)
{
	cout << "\n\nseed = " << g_seed << "\n";
	//
	StrSearch src;
	string buffer;
	string pat;
	//const int PLEN = 5;		//	パターン長
	pat.resize(PLEN);
	if( !bWorst ) {
		for (int i = 0; i != PLEN; ++i) pat[i] = (g_mt() % 26) + 'a';		//	'a' - 'z'
	} else {
		for (int i = 0; i != PLEN - 1; ++i) pat[i] = 'a';
		pat.back() = 'Z';
	}
	cout << "pat = '" << pat << "'\n\n";
	//buffer.resize(1024*1024);			//	1メガバイト
	//buffer.resize(1024*1024*10);		//	10メガバイト
	buffer.resize(1024*1024*100);		//	100メガバイト（1億）
	//buffer.resize(1024*1024*200);		//	200メガバイト（2億）
	//buffer.resize(1024*1024*500);		//	500メガバイト（5億）
	if( !bWorst ) {
		for(auto& x: buffer) x = (g_mt() % 26) + 'a';		//	'a' - 'z'
		for (int i = 0; i < 3; ++i) {
			int ix = g_mt() % (buffer.size() - PLEN);
			for (int k = 0; k < pat.size(); ++k) {
				buffer[ix+k] = pat[k];
			}
		}
	} else {
		for(auto& x: buffer) x = 'a';
		buffer.back() = 'Z';
	}
	//cout << "text = '";
	//for (int i = 0; i < 80; ++i) { cout << buffer[6209900+i]; } cout << "'\n";
	//
	if( true ) {
		cout << "std::strstr():\t";
		//vector<int> lst;
		cchar *ptr = &buffer[0];
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for(;;) {
			ptr = strstr(ptr, &pat[0]);
			if( ptr == nullptr ) break;
			//lst.push_back(ptr - &buffer[0]);
			++nMatch;
			++ptr;
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout /*<< "dur = "*/ << msec << "msec,\t";
		cout << "nMatch = " << nMatch << "\n";
		//for(auto ix: lst) { cout << ix << " "; } cout << "\n";
	}
	if( true ) {
		cout << "my_strstr():\t";
		//vector<int> lst;
		cchar *ptr = &buffer[0];
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for(;;) {
			ptr = my_strstr(ptr, &pat[0]);
			if( ptr == nullptr ) break;
			//lst.push_back(ptr - &buffer[0]);
			++nMatch;
			++ptr;
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout /*<< "dur = "*/ << msec << "msec,\t";
		cout << "nMatch = " << nMatch << "\n";
		//for(auto ix: lst) { cout << ix << " "; } cout << "\n";
	}
	//
	if( true ) {
		cout << "my_strstr2():\t";
		//vector<int> lst;
		cchar *ptr = &buffer[0];
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for(;;) {
			ptr = my_strstr2(ptr, &pat[0]);
			if( ptr == nullptr ) break;
			//lst.push_back(ptr - &buffer[0]);
			++nMatch;
			++ptr;
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout /*<< "dur = "*/ << msec << "msec,\t";
		cout << "nMatch = " << nMatch << "\n";
		//for(auto ix: lst) { cout << ix << " "; } cout << "\n";
	}
	//
	if( true ) {
		cout << "my_strcasestr():\t";
		//vector<int> lst;
		cchar *ptr = &buffer[0];
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for(;;) {
			ptr = my_strcasestr(ptr, &pat[0]);
			if( ptr == nullptr ) break;
			//lst.push_back(ptr - &buffer[0]);
			++nMatch;
			++ptr;
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout /*<< "dur = "*/ << msec << "msec,\t";
		cout << "nMatch = " << nMatch << "\n";
		//for(auto ix: lst) { cout << ix << " "; } cout << "\n";
	}
	//
	if( true ) {
		cout << "BMH_fwd:\t";
		//vector<int> lst;
		setupBMH(&pat[0]);
		cchar *ptr = &buffer[0];
		cchar* endptr = &buffer[0] + buffer.size();
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for(;;) {
			ptr = strstrBMH_fwd(/*&buffer[0],*/ ptr, endptr);
			if( ptr == nullptr ) break;
			//lst.push_back(ptr - &buffer[0]);
			++nMatch;
			++ptr;
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout /*<< "dur = "*/ << msec << "msec,\t";
		cout << "nMatch = " << nMatch << "\n";
		//for(auto ix: lst) { cout << ix << " "; } cout << "\n";
	}
	if( true ) {
		cout << "BMH_bkw:\t";
		//vector<int> lst;
		setupBMH(&pat[0]);
		cchar *ptr = &buffer[0];
		cchar* endptr = &buffer[0] + buffer.size();
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for(;;) {
			ptr = strstrBMH(/*&buffer[0],*/ ptr, endptr);
			if( ptr == nullptr ) break;
			//lst.push_back(ptr - &buffer[0]);
			++nMatch;
			++ptr;
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout /*<< "dur = "*/ << msec << "msec,\t";
		cout << "nMatch = " << nMatch << "\n";
		//for(auto ix: lst) { cout << ix << " "; } cout << "\n";
	}
	if( true ) {
		cout << "BMH ignore case:\t";
		//vector<int> lst;
		setupIcBMH(&pat[0]);
		cchar *ptr = &buffer[0];
		cchar* endptr = &buffer[0] + buffer.size();
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		for(;;) {
			ptr = strcasestrBMH(/*&buffer[0],*/ ptr, endptr);
			if( ptr == nullptr ) break;
			//lst.push_back(ptr - &buffer[0]);
			++nMatch;
			++ptr;
		}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout /*<< "dur = "*/ << msec << "msec,\t";
		cout << "nMatch = " << nMatch << "\n";
		//for(auto ix: lst) { cout << ix << " "; } cout << "\n";
	}
	if (true) {
		cout << "StrSearch::bitap():\t";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		//for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], 0, StrSearch::SHIFT_AND);	//	o for 大文字小文字区別
			cchar* ptr = &buffer[0];
			for(;;) {
				ptr = src.doSearch(ptr);
				if( ptr == nullptr ) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		//}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout /*<< "dur = "*/ << msec << "msec,\t";
		cout << "nMatch = " << nMatch << "\n";
	}
	if (true) {
		cout << "StrSearch::bitap(), opt = ignore_case:\t";
		int nMatch = 0;
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		//for (int cnt = 0; cnt != 100; ++cnt) {
			src.setup(&pat[0], StrSearch::IGNORE_CASE, StrSearch::SHIFT_AND);
			cchar* ptr = &buffer[0];
			for (;;) {
				ptr = src.doSearch(ptr);
				if (ptr == nullptr) break;
				++nMatch;
				++ptr;		//	次の位置から検索
			}
		//}
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		auto dur = end - start;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		cout /*<< "dur = "*/ << msec << "msec,\t";
		cout << "nMatch = " << nMatch << "\n";
	}
}
