// The "easy translate" library for C++.
//
// Repository: https://github.com/JaderoChan/easy_translate
// Contact email: c_dl_cn@outlook.com

// MIT License
//
// Copyright (c) 2024 頔珞JaderoChan

/// @version 1.1.0
/// @date 2026-08-15
/// @author 𬱖珞 JaderoChan

#ifndef EASY_TRANSLATE_HPP
#define EASY_TRANSLATE_HPP

#include <cstddef>              // size_t
#include <string>               // string
#include <vector>               // vector
#include <set>                  // set
#include <map>                  // map
#include <fstream>              // ifstream, ofstream

#include <nlohmann/json.hpp>    // json

/// @def EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES
/// @brief Define this macro to enable the `easytr::updateTranslationsMappingFiles()` function.
/// @note When this macro is defined, the `easytr::TranslateManager::translate()` function will store
/// all `Translation ID`s in memory for potential updates to `Translations mapping files`.

// Translate function
//   - Usage: EASYTR("Translation ID")
//   - Returns the `Translation text` for the given `Translation ID` in the current language.
//   - If the `Translation ID` does not exist in the current language, returns the `Translation ID` itself.
#define EASYTR(x) easytr::translate(x)

// Below is an example directory structure and content format for
// the `Languages mapping file` and `Translations mapping file`:
//
// languages_mapping.json (Languages mapping file)
//   - en_US (Language ID) : en_US.json (Translations mapping filepath)
//   - zh_CN (Language ID) : zh_CN.json (Translations mapping filepath)
//   - ...
//   - ja_JP (Language ID) : ja_JP.json (Translations mapping filepath)
//   - fr_FR (Language ID) : fr_FR.json (Translations mapping filepath)
//
// en_US.json (Translations mapping file)
//   - App.Title  (Translation ID) : Easy Translation (Translation text)
//   - App.Author (Translation ID) : JaderoChan       (Translation text)
//   - ...
//
// zh_CN.json (Translations mapping file)
//   - App.Title  (Translation ID) : 轻松翻译 (Translation text)
//   - App.Author (Translation ID) : 頔珞     (Translation text)
//   - ...

// Languages mapping file
//   - Language ID : Translations mapping filepath
//   - ...
//
// Translations mapping file
//   - Translation ID : Translation text
//   - ...

// Note:
// The `Languages mapping file` (e.g., languages_mapping.json) and `Translations mapping files`
// (e.g., en_US.json, zh_CN.json) should be saved using UTF-8 encoding.

namespace easytr
{

class LanguagesMapping
{
    friend class TranslateManager;

public:
    LanguagesMapping() = default;

    LanguagesMapping(const std::map<std::string, std::string>& langsMapping) : data_(langsMapping) {}

    /// @brief Load `LanguagesMapping` from a JSON string.
    /// @note If the JSON is invalid, the `LanguagesMapping` object will be empty.
    static LanguagesMapping fromJson(const std::string& json)
    {
        using Json = nlohmann::json;

        Json j = Json::parse(json, nullptr, false, true);
        if (j.is_discarded())
            return LanguagesMapping();

        std::map<std::string, std::string> list;
        for (const auto& var : j.items())
            list.insert({var.key(), var.value()});

        return LanguagesMapping(list);
    }

    /// @brief Load `LanguagesMapping` from a JSON file.
    /// @note If the JSON is invalid, the `LanguagesMapping` object will be empty.
    static LanguagesMapping fromFile(const std::string& filepath)
    {
        using Json = nlohmann::json;

        std::ifstream ifs(filepath);
        if (!ifs.is_open())
            return LanguagesMapping();

        Json j = Json::parse(ifs, nullptr, false, true);
        if (j.is_discarded())
        {
            ifs.close();
            return LanguagesMapping();
        }

        std::map<std::string, std::string> list;
        ifs.close();
        for (const auto& var : j.items())
            list.insert({var.key(), var.value()});

        return LanguagesMapping(list);
    }

    /// @brief Get the JSON representation as a string.
    std::string toJson() const
    {
        nlohmann::json j;
        for (const auto& var : data_)
            j[var.first] = var.second;
        return j.dump(2);
    }

    /// @brief Write the `LanguagesMapping` to a JSON file.
    /// @return Returns false if writing fails, otherwise returns true.
    bool toFile(const std::string& filepath = "languages_mapping.json") const
    {
        std::ofstream ofs(filepath);
        if (!ofs.is_open())
            return false;
        ofs << toJson();
        ofs.close();
        return true;
    }

    /// @brief Get the `Translations mapping filepath` for the given `Language ID`.
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
    std::vector<std::string> getIds() const
    {
        std::vector<std::string> ids;
        for (const auto& var : data_)
            ids.push_back(var.first);
        return ids;
    }

    /// @brief Add a pair of `Language ID` and `Translations mapping filepath`.
    /// @note If the `Language ID` already exists, no action is taken.
    void add(const std::string& languageId, const std::string& translationsFilepath)
    {
        if (!has(languageId))
            data_.insert({languageId, translationsFilepath});
    }

    /// @brief Remove a `Language ID` and its corresponding `Translations mapping filepath`.
    void remove(const std::string& languageId)
    {
        if (has(languageId))
            data_.erase(languageId);
    }

    /// @brief Remove all `Language ID`s and their corresponding `Translations mapping filepath`s.
    void clear() { data_.clear(); }

private:
    // {Language ID : Translations mapping filepath}
    std::map<std::string, std::string> data_;
};

class TranslationsMapping
{
    friend class TranslateManager;

public:
    TranslationsMapping() = default;

    TranslationsMapping(const std::map<std::string, std::string>& transMapping) : data_(transMapping) {}

    /// @brief Load `TranslationsMapping` from a JSON string.
    /// @note If the JSON is invalid, the `TranslationsMapping` object will be empty.
    static TranslationsMapping fromJson(const std::string& json)
    {
        using Json = nlohmann::json;

        Json j = Json::parse(json, nullptr, false, true);
        if (j.is_discarded())
            return TranslationsMapping();

        std::map<std::string, std::string> list;
        for (const auto& var : j.items())
            list.insert({var.key(), var.value()});

        return TranslationsMapping(list);
    }

    /// @brief Load `TranslationsMapping` from a JSON file.
    /// @note If the JSON is invalid, the `TranslationsMapping` object will be empty.
    static TranslationsMapping fromFile(const std::string& filepath)
    {
        using Json = nlohmann::json;

        std::ifstream ifs(filepath);
        if (!ifs.is_open())
            return TranslationsMapping();

        Json j = Json::parse(ifs, nullptr, false, true);
        if (j.is_discarded())
        {
            ifs.close();
            return TranslationsMapping();
        }

        std::map<std::string, std::string> list;
        ifs.close();
        for (const auto& var : j.items())
            list.insert({var.key(), var.value()});

        return TranslationsMapping(list);
    }

    /// @brief Get the JSON representation as a string.
    std::string toJson() const
    {
        nlohmann::json j;
        for (const auto& var : data_)
            j[var.first] = var.second;
        return j.dump(2);
    }

    /// @brief Write the `TranslationsMapping` to a JSON file.
    /// @return Returns false if writing fails, otherwise returns true.
    bool toFile(const std::string& filepath) const
    {
        std::ofstream ofs(filepath);
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
    std::vector<std::string> getIds() const
    {
        std::vector<std::string> ids;
        for (const auto& var : data_)
            ids.push_back(var.first);
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
    static TranslateManager& getInstance()
    {
        static TranslateManager instance;
        return instance;
    }

    /// @brief Get the `Translation text` for the given `Translation ID` in the current language.
    /// @note If the `Translation ID` does not exist in the current language, returns the `Translation ID` itself.
#ifndef EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES
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
#endif // EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES

    /// @brief Set the `LanguagesMapping` and reset the current language.
    void setLanguagesMapping(const LanguagesMapping& langsMapping)
    { langsMapping_ = langsMapping; currentLanguage_.clear(); }

    /// @brief Get the `Language ID` of the current language.
    const char* currentLanguage() const
    { return currentLanguage_.c_str(); }

    /// @brief Set the current language by `Language ID`.
    /// @return Returns true if successful, otherwise returns false.
    bool setCurrentLanguage(const std::string& languageId)
    {
        if (!hasLanguage(languageId))
            return false;

    #ifdef EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES
        bool isFirst = currentLanguage_.empty();
    #endif // EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES
        currentLanguage_ = languageId;
        transMapping_ = TranslationsMapping::fromFile(langsMapping_.at(languageId));

    #ifdef EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES
        if (isFirst)
        {
            for (const auto& var : transMapping_.data_)
                tranIds_.insert(var.first);
        }
    #endif // EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES

        return true;
    }

    const LanguagesMapping& languagesMapping() const { return langsMapping_; }

    const TranslationsMapping& translationsMapping() const { return transMapping_; }

    /// @brief Get the number of `Language ID`s.
    size_t languageCount() const { return langsMapping_.count(); }

    /// @brief Get the number of `Translation ID`s in the current language.
    size_t translationCount() const { return transMapping_.count(); }

    /// @brief Check if the given `Language ID` exists.
    bool hasLanguage(const std::string& languageId) const { return langsMapping_.has(languageId); }

    /// @brief Check if the given `Translation ID` exists in the current language.
    bool hasTranslation(const std::string& tranId) const { return transMapping_.has(tranId); }

    /// @brief Update all `Translations mapping files` (add new `Translation ID`s with empty `Translation text`).
    /// @return The number of files updated.
    /// @note New `Translation ID`s are collected from all calls to `translate()` in the program.
    /// @note This function helps to easily obtain all `Translation ID`s that need translation.
    /// @attention Call this function after all `translate()` calls to ensure a complete `Translation ID` list.
    /// @attention This function has no effect when the macro `EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES`
    ///            is undefined.
    size_t updateTranslationsMappingFiles() const
    {
    #ifndef EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES
        return 0;
    #else
        using Json = nlohmann::json;

        size_t updated = 0;
        for (const auto& languageId : langsMapping_.getIds())
        {
            std::string filepath = langsMapping_.at(languageId);
            std::ifstream ifs(filepath);
            Json j;
            if (!ifs.is_open())
            {
                for (const auto& tranId : tranIds_)
                    j[tranId] = "";
            }
            else
            {
                j = Json::parse(ifs, nullptr, false, true);
                if (j.is_discarded())
                {
                    j = Json();
                    for (const auto& tranId : tranIds_)
                        j[tranId] = "";
                }
                else
                {
                    std::map<std::string, std::string> map; // For sorting
                    for (const auto& tranId : tranIds_)
                        j.contains(tranId) ? map.insert({tranId, j[tranId]}) : map.insert({tranId, ""});

                    j.clear();
                    for (const auto& var : map)
                        j[var.first] = var.second;
                }

                ifs.close();
            }

            std::ofstream ofs(filepath);
            if (!ofs.is_open())
                continue;

            ofs << j.dump(2);
            ofs.close();
            updated++;
        }

        return updated;
    #endif // EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES
    }

private:
    TranslateManager() = default;

    ~TranslateManager() = default;

    TranslateManager(const TranslateManager&) = delete;

    TranslateManager& operator=(const TranslateManager&) = delete;

#ifdef EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES
    std::set<std::string> tranIds_;
#endif // EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES
    std::string currentLanguage_;
    LanguagesMapping langsMapping_;
    TranslationsMapping transMapping_;
};

// Convenience functions

inline TranslateManager& getTranslateManager()
{ return TranslateManager::getInstance(); }

/// @brief Get the `Translation text` for the given `Translation ID` in the current language.
/// @note If the `Translation ID` does not exist in the current language, returns the `Translation ID` itself.
inline const char* translate(const std::string& tranId)
{ return getTranslateManager().translate(tranId); }

/// @brief Set the `LanguagesMapping`.
inline void setLanguagesMapping(const LanguagesMapping& langs)
{ getTranslateManager().setLanguagesMapping(langs); }

inline const char* currentLanguage()
{ return getTranslateManager().currentLanguage(); }

/// @brief Set the current language by `Language ID`.
/// @return Returns true if successful, otherwise returns false.
inline bool setCurrentLanguage(const std::string& languageId)
{ return getTranslateManager().setCurrentLanguage(languageId); }

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

inline const LanguagesMapping& languagesMapping()
{ return getTranslateManager().languagesMapping(); }

inline const TranslationsMapping& translationsMapping()
{ return getTranslateManager().translationsMapping(); }

/// @brief Update all `Translations mapping files` (add new `Translation ID`s with empty `Translation text`).
/// @return The number of files updated.
/// @note New `Translation ID`s are collected from all calls to `translate()` in the program.
/// @note This function helps to easily obtain all `Translation ID`s that need translation.
/// @attention Call this function after all `translate()` calls to ensure a complete `Translation ID` list.
/// @attention This function has no effect when the macro `EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES`
///            is undefined.
inline size_t updateTranslationsMappingFiles()
{ return getTranslateManager().updateTranslationsMappingFiles(); }

} // namespace easytr

#endif // !EASY_TRANSLATE_HPP
