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
    //  型変換
    template<typename T>
    inline void convert(const std::string& s, T& val) {
        if (s.empty()) return;
        if constexpr (std::is_same_v<T, int>) val = std::stoi(s);
        else if constexpr (std::is_same_v<T, double>) val = std::stod(s);
        else if constexpr (std::is_same_v<T, std::string>) val = s;
    }
    //  行を分解
    template<typename T, typename... Args>
    inline void parseLine(std::stringstream& ss, T& first, Args&... rest) {
        std::string field;
        if (std::getline(ss, field, ',')) {
            convert(field, first);
            if constexpr (sizeof...(rest) > 0) parseLine(ss, rest...);
        }
    }

    //  定義用のマクロ
#define CSV_BINDABLE(StructName, ...) \
    friend std::istream& operator>>(std::istream& is, StructName& obj) { \
        std::string line; \
        if (std::getline(is, line)) { \
            if (line.empty()) return is; \
            std::stringstream ss(line); \
            parseLine(ss, __VA_ARGS__); \
        } \
        return is; \
    }

    template <typename T>
    class CSVLoader {
    private:
        std::vector<T> mDataList;
        std::unordered_map<int, size_t> mIdToIndex; 
        bool mIsLoaded = false;

    public:
        CSVLoader() = default;
        virtual~CSVLoader() = default;

        // ファイルから読み込み（成功したらtrue）
        bool Load(const std::string& filename) 
        {
            std::ifstream file(filename);
            if (!file.is_open()) return false;

            mDataList.clear();
            mIdToIndex.clear();

            std::string header;
            std::getline(file, header);

            T item;
            while (file >> item) {
                // item.id が存在することを前提とする
                mIdToIndex[item.id] = mDataList.size();
                mDataList.push_back(item);
            }

            mIsLoaded = true;
            return true;
        }

        // 全データ取得
        const std::vector<T>& GetAll() const { return mDataList; }

        // IDで検索（ポインタを返すことで、見つからない場合はnullptrを返せるようにする）
        const T* Find(int id) const {
            auto it = mIdToIndex.find(id);
            if (it != mIdToIndex.end()) {
                return &mDataList[it->second];
            }
            return nullptr;
        }

        size_t Count() const { return mDataList.size(); }
        bool Empty() const { return !mIsLoaded || mDataList.empty(); }
    };
};

