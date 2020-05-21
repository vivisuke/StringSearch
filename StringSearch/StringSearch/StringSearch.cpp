#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <string.h>
#include "StrSearch.h"

using namespace std;

random_device g_rd;
mt19937 g_mt(g_rd());

void test_searchRandom();
void test_search_aaaaZ();
void test_search_aaaaaaaaaZ();

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
#if	1
	test_searchRandom();
	//test_search_aaaaZ();
	//test_search_aaaaaaaaaZ();
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
    std::cout << "OK\n";
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
