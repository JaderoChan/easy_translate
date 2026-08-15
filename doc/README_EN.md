# Easy Translate

[[简体中文](./README_ZH.md) | **English**]

## Introduction

This is a mini library written in C++11 that helps you quickly implement UI text translation in GUI applications.

## Features

- Small and simple: This is a Header-Only library, which means you can easily integrate it into any C++ project.
- Transparent language files, no compilation required: Translation files are stored as JSON text files, which means they require no compilation, are easy to modify, and are human-readable.
- Automatic **Translation ID** extraction: Provides an interface and a Python script to help developers extract **Translation IDs** used in the code (translations are retrieved via **Translation IDs**).

## Limitations

- No dynamic loading of translation files; switching languages will load all translations for the target language into memory.
- Strictly dependent on the file system, meaning this library only accepts translation files in file form.

## Dependencies

[Nlohmann/Json](https://github.com/nlohmann/json)

## Overview

### Languages Mapping

This class stores the mapping between language IDs and the paths to their corresponding **Translations Mapping** files.

Its basic structure is `{Language ID (key, string) : Translations mapping filepath (value, string)}` pairs. Here, `Language ID` represents identifiers such as `EN`, `ZH`, etc., and `Translations mapping filepath` is the path to the **Translations Mapping** file for that language ID.

### Translations Mapping

This class stores the mapping between translation IDs and their translations.

Its basic structure is `{Text ID (key, string) : Translation (value, string)}` pairs, where `Text ID` is the translation ID and `Translation` is the corresponding translation for that ID.

## Standard Usage Workflow

1. Load a **Languages Mapping** file via `setLanguagesMapping` to obtain the mapping between language IDs and **Translations Mapping** file paths.
2. Change the current language ID via `setLanguage`, which loads the target **Translations Mapping** file for that language ID.
3. Wrap translation IDs in your code using the `EASYTR` macro (or call `translate`) to retrieve the translation for the specified ID in the current language.

## Extracting Translation IDs

Manually extracting translation IDs is often tedious.

You can call `updateTranslationsMappingFiles` after the main loop ends to update translation IDs. This will add new key-value pairs to the translation files for any newly discovered translation IDs, with the key set to the new ID and the value (translation) set to an empty string, and it will also re-sort the translation IDs.

The macro `EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES` must be defined when using `updateTranslationsMappingFiles`; otherwise, it does nothing.

When `EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES` is defined, every translation ID that is read will be stored internally so that the **Translations Mapping** files can be updated when `updateTranslationsMappingFiles` is called. This behavior incurs additional overhead. The recommended practice is to define `EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES` only when you need to extract translation IDs (i.e., during development).

## Example

See `example/base_example`, which provides a sample project implemented with the Qt framework.
