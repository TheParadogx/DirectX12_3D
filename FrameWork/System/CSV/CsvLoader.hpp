#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <type_traits>
#include <unordered_map>
#include <memory>

namespace Engine::System
{
    /* 読込用変換 */
    template<typename T>
    inline void Convert(const std::string& s, T& val) {
        if (s.empty()) return;
        if constexpr (std::is_same_v<T, int>) val = std::stoi(s);
        else if constexpr (std::is_same_v<T, double>) val = std::stod(s);
        else if constexpr (std::is_same_v<T, std::string>) val = s;
    }

    /* 行を分解 */
    template<typename T, typename... Args>
    inline void ParseLine(std::stringstream& ss, T& first, Args&... rest) {
        std::string field;
        if (std::getline(ss, field, ',')) {
            Convert(field, first); // Convertに統一
            if constexpr (sizeof...(rest) > 0) ParseLine(ss, rest...);
        }
    }

    /* 書き出し用シリアライズ */
    template<typename T>
    inline void SerializeLine(std::ostream& os, const T& last) {
        os << last;
    }
    template<typename T, typename... Args>
    inline void SerializeLine(std::ostream& os, const T& first, const Args&... rest) {
        os << first << ",";
        Engine::System::SerializeLine(os, rest...);
    }

    /* バインド用マクロ（読み書き両対応版） */
#define CSV_BINDABLE(StructName, ...) \
    friend std::istream& operator>>(std::istream& is, StructName& obj) { \
        std::string line; \
        if (std::getline(is, line)) { \
            if (line.empty()) return is; \
            std::stringstream ss(line); \
            Engine::System::ParseLine(ss, __VA_ARGS__); \
        } \
        return is; \
    } \
    friend std::ostream& operator<<(std::ostream& os, const StructName& obj) { \
        Engine::System::SerializeLine(os, __VA_ARGS__); \
        return os; \
    }

    template <typename T>
    class CSVLoader {
    private:
        std::vector<T> mDataList;
        std::unordered_map<int, size_t> mIdToIndex;
        std::string mCachedHeader;
        bool mIsLoaded = false;

    public:
        CSVLoader() = default;
        virtual ~CSVLoader() = default;

        /* ファイルへ保存 */
        bool Save(const std::string& filename, const std::string& header = "") {
            std::ofstream file(filename);
            if (!file.is_open()) return false;

            std::string finalHeader = header.empty() ? mCachedHeader : header;

            if (!finalHeader.empty()) {
                file << finalHeader << "\n";
            }

            for (const auto& item : mDataList) {
                file << item << "\n"; // マクロで定義した operator<< が呼ばれる
            }
            return true;
        }

        /* 読込 */
        bool Load(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) return false;

            mDataList.clear();
            mIdToIndex.clear();

            // 1行目をキャッシュに保存
            if (!std::getline(file, mCachedHeader)) {
                mCachedHeader = "";
            }

            T item;
            while (file >> item) {
                mIdToIndex[item.id] = mDataList.size();
                mDataList.push_back(item);
            }
            mIsLoaded = true;
            return true;
        }

        /* 検索 読み取り用 */
        const T* Find(int id) const {
            auto it = mIdToIndex.find(id);
            return (it != mIdToIndex.end()) ? &mDataList[it->second] : nullptr;
        }

        /* 検索 書き換え用*/
        T* FindMutable(int id) {
            auto it = mIdToIndex.find(id);
            return (it != mIdToIndex.end()) ? &mDataList[it->second] : nullptr;
        }

        std::vector<T>& GetRawData() { return mDataList; }
        const std::vector<T>& GetAll() const { return mDataList; }
        size_t Count() const { return mDataList.size(); }
        bool Empty() const { return !mIsLoaded || mDataList.empty(); }
    };
}