// The "easy translate" library for C++.
//
// Repository: https://github.com/JaderoChan/easy_translate
// Contact email: c_dl_cn@outlook.com

// MIT License
//
// Copyright (c) 2024 頔珞JaderoChan

/// @version 1.3.1
/// @date 2026-11-25
/// @author 𬱖珞 JaderoChan

#ifndef EASY_TRANSLATE_HPP
#define EASY_TRANSLATE_HPP

#include <stddef.h> // size_t
#include <string>   // string
#include <set>      // set
#include <map>      // map
#include <istream>  // istream
#include <fstream>  // ifstream, ofstream

#include <nlohmann/json.hpp> // json

/// @def EASY_TRANSLATE_COLLECT_TRANSLATION_ID
/// @brief Define this macro to enable the `easytr::collectedTranslationIds()` function.
/// @note When this macro is defined, the `easytr::translate()` function will store
/// all `Translation ID`s in memory for dump them.

/// Translate function
///   - Usage: EASYTR("Translation ID")
///   - Returns the `Translation text` for the given `Translation ID` in the current language.
///   - If the `Translation ID` does not exist in the current language, returns the `Translation ID` itself.
#define EASYTR(x) easytr::translate(x)

namespace easytr
{

/// A mapping table used to record the `Language ID` and the `Translation mapping path`.
class LanguageMapping
{
    friend class TranslateManager;

    using Json = nlohmann::json;

public:
    LanguageMapping() = default;

    LanguageMapping(const std::map<std::string, std::string>& langMapping) : data_(langMapping) {}

    /// @brief Load `Language mapping` from a json string.
    /// @note If the json is invalid, the `Language mapping` will be empty.
    static LanguageMapping fromJson(const char* json)
    {
        Json j = Json::parse(json, nullptr, false, true);
        if (j.is_discarded())
            return LanguageMapping();

        std::map<std::string, std::string> list;
        for (const auto& var : j.items())
            list.insert({var.key(), var.value()});

        return LanguageMapping(list);
    }

    /// @brief Load `Language mapping` from a stream.
    /// @overload
    static LanguageMapping fromStream(std::istream& is)
    {
        Json j = Json::parse(is, nullptr, false, true);
        if (j.is_discarded())
            return LanguageMapping();

        std::map<std::string, std::string> list;
        for (const auto& var : j.items())
            list.insert({var.key(), var.value()});

        return LanguageMapping(list);
    }

    /// @brief Load `Language mapping` from a json file.
    /// @overload
    static LanguageMapping fromFile(const std::string& filePath)
    {
        std::ifstream ifs(filePath);
        if (!ifs.is_open())
            return LanguageMapping();
        return fromStream(ifs);
    }

    /// @brief Get the json representation as a string.
    std::string toJson() const
    {
        nlohmann::json j;
        for (const auto& var : data_)
            j[var.first] = var.second;
        return j.dump(2);
    }

    /// @brief Write the `Language mapping` to a json file.
    /// @return Returns false if writing fails, otherwise returns true.
    bool toFile(const std::string& filePath = "language_mapping.json") const
    {
        std::ofstream ofs(filePath);
        if (!ofs.is_open())
            return false;
        ofs << toJson();
        ofs.close();
        return true;
    }

    /// @brief Get the `Translation mapping path` for the given `Language ID`.
    const char* at(const std::string& languageId) const
    { return data_.at(languageId).c_str(); }

    /// @brief Get the number of `Language ID`s.
    size_t count() const { return data_.size(); }

    /// @brief Check if there are no `Language ID`s.
    bool empty() const { return count() == 0; }

    /// @brief Check if the given `Language ID` exists.
    bool has(const std::string& languageId) const
    { return data_.find(languageId) != data_.end(); }

    /// @brief Get all `Language ID`s.
    std::set<std::string> getIds() const
    {
        std::set<std::string> ids;
        for (const auto& var : data_)
            ids.insert(var.first);
        return ids;
    }

    /// @brief Add a pair of `Language ID` and `Translation mapping path`.
    /// @note If the `Language ID` already exists, no action is taken.
    void add(const std::string& languageId, const std::string& path)
    {
        if (!has(languageId))
            data_.insert({languageId, path});
    }

    /// @brief Remove a `Language ID` and its corresponding `Translation mapping path`.
    void remove(const std::string& languageId)
    {
        if (has(languageId))
            data_.erase(languageId);
    }

    /// @brief Remove all `Language ID`s and their corresponding `Translation mapping path`s.
    void clear() { data_.clear(); }

private:
    // {Language ID : Translation mapping path}
    std::map<std::string, std::string> data_;
};

/// A mapping table used to record the `Translation ID` and the `Translation text`.
class TranslationMapping
{
    friend class TranslateManager;

    using Json = nlohmann::json;

public:
    TranslationMapping() = default;

    TranslationMapping(const std::map<std::string, std::string>& transMapping) : data_(transMapping) {}

    /// @brief Load `Translation mapping` from a json string.
    /// @note If the json is invalid, the `TranslationMapping` will be empty.
    static TranslationMapping fromJson(const std::string& json)
    {
        Json j = Json::parse(json, nullptr, false, true);
        if (j.is_discarded())
            return TranslationMapping();

        std::map<std::string, std::string> list;
        for (const auto& var : j.items())
            list.insert({var.key(), var.value()});

        return TranslationMapping(list);
    }

    /// @brief Load `Translation mapping` from a stream.
    /// @overload
    static TranslationMapping fromStream(std::istream& is)
    {
        Json j = Json::parse(is, nullptr, false, true);
        if (j.is_discarded())
            return TranslationMapping();

        std::map<std::string, std::string> list;
        for (const auto& var : j.items())
            list.insert({var.key(), var.value()});

        return TranslationMapping(list);
    }

    /// @brief Load `Translation mapping` from a json file.
    /// @overload
    static TranslationMapping fromFile(const std::string& filePath)
    {
        std::ifstream ifs(filePath);
        if (!ifs.is_open())
            return TranslationMapping();
        return fromStream(ifs);
    }

    /// @brief Get the json representation as a string.
    std::string toJson() const
    {
        nlohmann::json j;
        for (const auto& var : data_)
            j[var.first] = var.second;
        return j.dump(2);
    }

    /// @brief Write the `Translation mapping` to a json file.
    /// @return Returns false if writing fails, otherwise returns true.
    bool toFile(const std::string& filePath) const
    {
        std::ofstream ofs(filePath);
        if (!ofs.is_open())
            return false;
        ofs << toJson();
        ofs.close();
        return true;
    }

    /// @brief Get the `Translation text` for the given `Translation ID`.
    /// @note If the `Translation ID` does not exist, returns the `Translation ID` itself.
    const char* at(const std::string& tranId) const
    {
        if (!has(tranId))
            return tranId.c_str();
        return data_.at(tranId).c_str();
    }

    /// @brief Get the number of `Translation ID`s.
    size_t count() const { return data_.size(); }

    /// @brief Check if there are no `Translation ID`s.
    bool empty() const { return count() == 0; }

    /// @brief Check if the given `Translation ID` exists.
    bool has(const std::string& tranId) const
    { return data_.find(tranId) != data_.end(); }

    /// @brief Get all `Translation ID`s.
    std::set<std::string> getIds() const
    {
        std::set<std::string> ids;
        for (const auto& var : data_)
            ids.insert(var.first);
        return ids;
    }

    /// @brief Add a pair of `Translation ID` and `Translation text`.
    /// @note If the `Translation ID` already exists, no action is taken.
    void add(const std::string& tranId, const std::string& translation)
    {
        if (!has(tranId))
            data_.insert({tranId, translation});
    }

    /// @brief Remove a `Translation ID` and its corresponding `Translation text`.
    void remove(const std::string& tranId)
    {
        if (has(tranId))
            data_.erase(tranId);
    }

    /// @brief Remove all `Translation ID`s and their corresponding `Translation text`s.
    void clear() { data_.clear(); }

private:
    // {Translation ID : Translation text}
    std::map<std::string, std::string> data_;
};

// Singleton class
class TranslateManager
{
public:
    using TranslationMappingLoader = TranslationMapping (*)(const std::string& path);

    static TranslateManager& instance()
    {
        static TranslateManager instance;
        return instance;
    }

    /// @brief Set the custom Translation mapping loader for handle `Translation mapping path` in `Language mapping`.
    /// @note Default is nullptr and handle `Translation mapping path` as local file path.
    void setTranslationMappingLoader(TranslationMappingLoader loader = nullptr)
    {
        loader_ = loader;
    }

    /// @brief Get the `Translation text` for the given `Translation ID` in the current language.
    /// @note If the `Translation ID` does not exist in the current language, returns the `Translation ID` itself.
#ifndef EASY_TRANSLATE_COLLECT_TRANSLATION_ID
    const char* translate(const std::string& tranId) const
    {
        return transMapping_.at(tranId);
    }
#else
    const char* translate(const std::string& tranId)
    {
        tranIds_.insert(tranId);
        return transMapping_.at(tranId);
    }
#endif // EASY_TRANSLATE_COLLECT_TRANSLATION_ID

    /// @brief Set the `Language mapping` and reset the current language.
    void setLanguageMapping(const LanguageMapping& langMapping)
    { langMapping_ = langMapping; currentLanguage_.clear(); }

    /// @brief Get the `Language ID` of the current language.
    const char* currentLanguage() const
    { return currentLanguage_.c_str(); }

    /// @brief Set the current language by `Language ID`.
    /// @return Returns true if successful, otherwise returns false.
    bool setCurrentLanguage(const std::string& languageId)
    {
        if (!hasLanguage(languageId))
            return false;

    #ifdef EASY_TRANSLATE_COLLECT_TRANSLATION_ID
        bool isFirst = currentLanguage_.empty();
    #endif // EASY_TRANSLATE_COLLECT_TRANSLATION_ID
        currentLanguage_ = languageId;

        transMapping_ = loader_
            ? loader_(langMapping_.at(languageId))
            : TranslationMapping::fromFile(langMapping_.at(languageId));

    #ifdef EASY_TRANSLATE_COLLECT_TRANSLATION_ID
        if (isFirst)
        {
            for (const auto& var : transMapping_.data_)
                tranIds_.insert(var.first);
        }
    #endif // EASY_TRANSLATE_COLLECT_TRANSLATION_ID

        return true;
    }

    const LanguageMapping& languageMapping() const { return langMapping_; }

    const TranslationMapping& translationMapping() const { return transMapping_; }

    /// @brief Get the number of `Language ID`s.
    size_t languageCount() const { return langMapping_.count(); }

    /// @brief Get the number of `Translation ID`s in the current language.
    size_t translationCount() const { return transMapping_.count(); }

    /// @brief Check if the given `Language ID` exists.
    bool hasLanguage(const std::string& languageId) const { return langMapping_.has(languageId); }

    /// @brief Check if the given `Translation ID` exists in the current language.
    bool hasTranslation(const std::string& tranId) const { return transMapping_.has(tranId); }

#ifdef EASY_TRANSLATE_COLLECT_TRANSLATION_ID
    /// @brief Get all collected `Translation ID`s.
    /// @note New `Translation ID`s are collected from all calls to `translate()` in the program.
    /// @note This function helps to easily obtain all `Translation ID`s that need translation.
    /// @attention Call this function after all `translate()` calls to ensure a complete `Translation ID` list.
    /// @sa dumpCollectedTranslationIds()
    std::set<std::string> collectedTranslationIds() const
    {
        return tranIds_;
    }

    /// @brief Dump all collected `Translation ID`s to special file.
    /// @return The number of the collected translations IDs, if dump fail return 0.
    /// @sa collectedTranslationIds()
    size_t dumpCollectedTranslationIds(const std::string& filePath) const
    {
        using Json = nlohmann::json;

        Json j = Json::object();
        for (const auto& tranId : tranIds_)
            j[tranId] = std::string();

        std::ofstream ofs(filePath);
        if (!ofs.is_open())
            return 0;

        ofs << j.dump(2);
        return tranIds_.size();
    }
#endif // EASY_TRANSLATE_COLLECT_TRANSLATION_ID

private:
    TranslateManager() = default;

    ~TranslateManager() = default;

    TranslateManager(const TranslateManager&) = delete;

    TranslateManager& operator=(const TranslateManager&) = delete;

#ifdef EASY_TRANSLATE_COLLECT_TRANSLATION_ID
    std::set<std::string>    tranIds_;
#endif // EASY_TRANSLATE_COLLECT_TRANSLATION_ID
    std::string              currentLanguage_;
    LanguageMapping          langMapping_;
    TranslationMapping       transMapping_;
    TranslationMappingLoader loader_ = nullptr;
};

// Convenience functions

inline TranslateManager& getTranslateManager()
{ return TranslateManager::instance(); }

/// @brief Set the custom Translation mapping loader for handle `Translation mapping path` in `Language mapping`.
/// @note Default is nullptr and handle `Translation mapping path` as local file path.
inline void setTranslationMappingLoader(TranslateManager::TranslationMappingLoader loader = nullptr)
{ getTranslateManager().setTranslationMappingLoader(loader); }

/// @brief Get the `Translation text` for the given `Translation ID` in the current language.
/// @note If the `Translation ID` does not exist in the current language, returns the `Translation ID` itself.
inline const char* translate(const std::string& tranId)
{ return getTranslateManager().translate(tranId); }

/// @brief Set the `Language mapping` and reset the current language.
inline void setLanguageMapping(const LanguageMapping& langs)
{ getTranslateManager().setLanguageMapping(langs); }

/// @brief Get the `Language ID` of the current language.
inline const char* currentLanguage()
{ return getTranslateManager().currentLanguage(); }

/// @brief Set the current language by `Language ID`.
/// @return Returns true if successful, otherwise returns false.
inline bool setCurrentLanguage(const std::string& languageId)
{ return getTranslateManager().setCurrentLanguage(languageId); }

inline const LanguageMapping& languageMapping()
{ return getTranslateManager().languageMapping(); }

inline const TranslationMapping& translationMapping()
{ return getTranslateManager().translationMapping(); }

/// @brief Get the number of `Language ID`s.
inline size_t languageCount()
{ return getTranslateManager().languageCount(); }

/// @brief Get the number of `Translation ID`s in the current language.
inline size_t translationCount()
{ return getTranslateManager().translationCount(); }

/// @brief Check if the given `Language ID` exists.
inline bool hasLanguage(const std::string& languageId)
{ return getTranslateManager().hasLanguage(languageId); }

/// @brief Check if the given `Translation ID` exists in the current language.
inline bool hasTranslation(const std::string& tranId)
{ return getTranslateManager().hasTranslation(tranId); }

#ifdef EASY_TRANSLATE_COLLECT_TRANSLATION_ID
/// @brief Get all collected `Translation ID`s.
/// @note New `Translation ID`s are collected from all calls to `translate()` in the program.
/// @note This function helps to easily obtain all `Translation ID`s that need translation.
/// @attention Call this function after all `translate()` calls to ensure a complete `Translation ID` list.
/// @sa dumpCollectedTranslationIds()
inline std::set<std::string> collectedTranslationIds()
{ return getTranslateManager().collectedTranslationIds(); }

/// @brief Dump all collected `Translation ID`s to special file.
/// @return The number of the collected translations IDs, if dump fail return 0.
/// @sa collectedTranslationIds()
inline size_t dumpCollectedTranslationIds(const std::string& filePath)
{ return getTranslateManager().dumpCollectedTranslationIds(filePath); }
#endif // EASY_TRANSLATE_COLLECT_TRANSLATION_ID

} // namespace easytr

#endif // !EASY_TRANSLATE_HPP
