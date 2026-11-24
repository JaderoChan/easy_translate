# Easy Translate

[**简体中文** | [English](./README_EN.md)]

## 介绍

这是一个能够帮助你快速完成 GUI 程序中 UI 文本翻译的 mini 库，使用 C++11 编写。

## 特点

- 小且简单：这是一个 Header-Only 库，这意味着你可以很轻松的将它应用在任何 C++ 项目中。
- 语言文件透明，无需编译：译文文件使用 json 格式文本文件存储，这意味着它无需编译，易于修改，且对人类阅读友好。
- 自动提取 **译文ID**：提供接口与 python 脚本帮助开发者提取代码中用到的 **译文ID**（通过 **译文ID** 获取译文）。

## 缺点

- 无译文文件动态载入功能，在切换语言时会将目标语言的所有译文载入至内存。

## 依赖

[Nlohmann/Json](https://github.com/nlohmann/json)

## 大纲

### Language Mapping

此类存储了语言ID与 **译文映射文件** URL 的映射。

其基本结构是 `{Language ID (key, string) : Translation mapping URL (value, string)}` 对。其中 `Language ID` 表示语言ID，如 `EN`、`ZH` 等，而 `Translation mapping URL` 表示语言ID对应的 **译文映射文件** URL。

### Translation Mapping

此类存储了译文ID与译文的映射。

其基本结构是 `{Translation ID (key, string) : Translation text (value, string)}` 对，其中 `Translation ID` 表示译文ID，而 `Translation text` 表示译文ID对应的译文。

## 标准使用流程

1. 通过 `setLanguageMapping` 载入一个 **语言映射文件**，得到语言ID与 **译文映射文件** URL 的映射。
2. 通过 `setLanguage` 改变当前语言ID，这样便能通过语言ID将目标 **译文映射文件** 载入。
3. 通过 `EASYTR` 宏对程序中的译文ID进行包含（或者调用 `translate`）以获取当前语言下指定译文ID的对应译文。

## 提取译文ID

大多数时候，手动提取译文ID是十分繁琐的。

故可以在程序主循环结束后通过 `collectedTranslationIds()`/`dumpCollectedTranslationIds()` 来获取所有程序中使用到的译文ID。

使用以上两个函数时必须定义 `EASY_TRANSLATE_COLLECT_TRANSLATION_ID` 宏。

在定义了 `EASY_TRANSLATE_COLLECT_TRANSLATION_ID` 宏时，实例内部会对每一个读取到的译文ID进行存储，此行为会增加内存占用。正确的做法应该是在需要提取译文ID时（开发阶段）才定义 `EASY_TRANSLATE_COLLECT_TRANSLATION_ID` 宏。

## 示例

参见 `example/base_example`，其提供了一个在 Qt 框架下实现的示例程序项目。
