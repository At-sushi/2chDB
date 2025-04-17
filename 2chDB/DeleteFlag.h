#pragma once
// 2chDB/DeleteCache.h : DeleteCacheクラスのヘッダーファイル

#include <iostream>
#include <string>
#include <unordered_set>
#include <mutex>
#include <filesystem>
#include <format>
#include <string_view>

class DeleteFlag
{
private:
    /* data */
    struct BBSKey {
        std::string bbs;
        std::string key;
    
        auto operator <=> (const BBSKey&) const = default;
    
        std::size_t hash() const noexcept {
            return std::hash<std::string>{}(bbs) ^ std::hash<std::string>{}(key);
        }
    };

    std::unordered_set<BBSKey, decltype([](const BBSKey& a){ return a.hash(); })> m_cache; // キャッシュのセット
    std::mutex m_mutex; // スレッドセーフのためのミューテックス

public:
    DeleteFlag() = default;
    ~DeleteFlag() {
        try {
            flush(); // キャッシュをフラッシュ
        }
        catch(const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl; // エラーメッセージを表示
            std::cerr << "Error while flushing cache." << std::endl; // エラーメッセージを表示
        }
    }

    void erase(const std::string& bbs, const std::string& key) {
        std::lock_guard<std::mutex> lock(m_mutex); // スレッドセーフにするためのロック

        m_cache.erase({bbs, key}); // キャッシュから削除
    }

    void add(const std::string& bbs, const std::string& key) {
        std::lock_guard<std::mutex> lock(m_mutex); // スレッドセーフにするためのロック

        m_cache.emplace(bbs, key); // キャッシュに追加
    }

    void flush() {
        std::lock_guard<std::mutex> lock(m_mutex); // スレッドセーフにするためのロック

        for (const auto& bbsKey : m_cache) {
            // キャッシュをフラッシュする処理をここに追加
            // 例: データベースから削除するなど
            const std::filesystem::path path = create_fname(bbsKey.bbs, bbsKey.key);

            if (std::filesystem::file_size(path) == 0) // ファイルが空の場合
                std::filesystem::remove(path); // ファイルを削除
            else if (std::filesystem::exists(path)) // ファイルが存在する場合
                std::cerr << "File exists: " << path << std::endl; // エラーメッセージを表示
            else // ファイルが存在しない場合
                std::cerr << "File not found: " << path << std::endl; // エラーメッセージを表示
        }

        m_cache.clear(); // キャッシュをクリア
    }
};
