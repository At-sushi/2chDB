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
#include <filesystem>
#include <concepts>

// TODO: プログラムに必要な追加ヘッダーをここで参照します。

constexpr int DEFAULT_PORT = 8082;

void init();
void testWrite(const char *bbs, const char *key, const char *source, bool isAppend = false);
const char *queryFromReadCGI(const char *bbs, const char *key);

inline constexpr std::uint16_t get_hash(const std::string_view &key)
{
    std::uint16_t hash = 5381;
    for (const auto i : key)
        hash = (hash << 5) + hash + i;
    return hash;
}

template <typename T>
    requires std::is_convertible_v<T, std::string_view>
inline constexpr std::string create_fname(const T &bbs, const T &key)
{
    // TODO: sanitize bbs and key
    return std::format("{}/dat/{:04x}/{}.dat", bbs, get_hash(key), key);
}
