// 2chDB.h : 標準のシステム インクルード ファイル用のインクルード ファイル、
// または、プロジェクト専用のインクルード ファイル。

#pragma once

#include <iostream>
#include <cstdint>
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
inline std::uint16_t get_hash(const T &key)
{
    std::uint16_t hash = 5381;
    for (auto &&i : key)
        hash = (hash << 5) + hash + i;
    return hash;
}

template <typename T>
    requires std::is_convertible_v<T, std::string_view>
inline std::string create_fname(const T &bbs, const T &key)
{
    // TODO: sanitize bbs and key
    return std::format("{}/dat/{0:8x}/{}.dat", bbs, get_hash(key), key);
}
