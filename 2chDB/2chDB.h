// 2chDB.h : 標準のシステム インクルード ファイル用のインクルード ファイル、
// または、プロジェクト専用のインクルード ファイル。

#pragma once

#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <format>
#include <ranges>
#include <concepts>

// TODO: プログラムに必要な追加ヘッダーをここで参照します。

void init();
void testWrite(const char *bbs, const char *key, const char *source);
const char *queryFromReadCGI(const char *bbs, const char *key);

template <typename T>
    requires std::is_convertible_v<T, std::string_view>
inline static constexpr std::string create_fname(const T &bbs, const T &key)
{
    // TODO: sanitize bbs and key
    return std::format("{}/dat/{}.dat", bbs, key);
}
