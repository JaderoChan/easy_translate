# Easy Translate

[[简体中文](./README.md) | **English**]

## Introduction

This is a mini library written in C++11 that helps you quickly implement UI text translation in GUI programs.

## Features

- Small and simple: This is a header-only library, which means you can easily integrate it into any C++ project.
- Transparent language files, no compilation needed: Translation files are stored as json text files, which means they require no compilation, are easy to modify, and are human-readable.
- Automatic **Translation ID** extraction: Provides an interface and a python script to help developers extract the **Translation IDs** used in the code (translations are retrieved via **Translation ID**).

## Drawbacks

- No dynamic loading of translation files; all translations for the target language are loaded into memory when switching languages.

## Dependencies

[Nlohmann/Json](https://github.com/nlohmann/json)

## Overview

### Language Mapping

This class stores the mapping between language IDs and **Translation Mapping File** paths.

Its basic structure is a `{Language ID (key, string) : Translation mapping path (value, string)}` pair, where `Language ID` represents the language ID, such as `EN`, `ZH`, etc., and `Translation mapping path` represents the **Translation Mapping File** path corresponding to the language ID.

### Translation Mapping

This class stores the mapping between translation IDs and translation texts.

Its basic structure is a `{Translation ID (key, string) : Translation text (value, string)}` pair, where `Translation ID` represents the translation ID, and `Translation text` represents the translation text corresponding to the translation ID.

## Standard Usage Flow

1. Load a **Language Mapping File** via `setLanguageMapping` to obtain the mapping between language IDs and **Translation Mapping File** paths.
2. Change the current language ID via `setLanguage`, so that the target **Translation Mapping File** can be loaded via the language ID.
3. Wrap the translation IDs in your program with the `EASYTR` macro (or call `translate`) to get the corresponding translation text for the specified translation ID in the current language.

## Extracting Translation IDs

In most cases, manually extracting translation IDs is quite tedious.

Therefore, after the main loop of the program ends, you can use `collectedTranslationIds()`/`dumpCollectedTranslationIds()` to retrieve all translation IDs used in the program.

The `EASY_TRANSLATE_COLLECT_TRANSLATION_ID` macro must be defined to use the above two functions.

When the `EASY_TRANSLATE_COLLECT_TRANSLATION_ID` macro is defined, each translation ID read will be stored internally by the instance, which increases memory usage. The correct approach is to define the `EASY_TRANSLATE_COLLECT_TRANSLATION_ID` macro only when you need to extract translation IDs (during the development phase).

## Example

See `example/base_example`, which provides an example program project implemented under the Qt framework.
