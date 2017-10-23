// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
// std::make_shared
#define _VARIADIC_MAX	10
// Windows
#include <windows.h>
#include <CommDlg.h>
#include <uxtheme.h>
#include <vssym32.h>
#pragma comment(lib, "Uxtheme.lib")
#include <WinGDI.h>
#pragma comment(lib, "msimg32.lib")
#include <OleIdl.h>
#include <shlwapi.h>
#pragma comment (lib, "shlwapi.lib")
// C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <string.h>

// C++
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <deque>
#include <vector>
#include <algorithm>
#include <functional>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <tuple>
#include <typeinfo>
#include <unordered_set>
#include <array>
#include <exception>
//#include <thread>
//#include <mutex>

// Boost
#include <boost/program_options.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>

//MultiIndex
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <boost/signals2/signal.hpp>
//
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
//
#define _SCL_SECURE_NO_WARNINGS
#define D_SCL_SECURE_NO_WARNINGS
#include <boost/multi_array.hpp>
#include <boost/tokenizer.hpp>

//Serialization
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp> 
#include <boost/archive/text_iarchive.hpp> 
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_woarchive.hpp>
#include <boost/archive/xml_wiarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/split_member.hpp>

//Spirit
#pragma warning(disable: 4819)
#include <boost/spirit/include/qi.hpp>
#pragma warning(default: 4819)
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>

//OpenGL
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")