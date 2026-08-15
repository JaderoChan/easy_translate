#include <qapplication.h>

#include <easy_translate.hpp>

#include "mainwidget.h"
#include "language.h"

#define APP_LANGUAGES_MAPPING_FILEPATH "languages/languages_mapping.json"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    easytr::setLanguagesMapping(easytr::LanguagesMapping::fromFile(APP_LANGUAGES_MAPPING_FILEPATH));
    setLanguage(currentLanguage());

    MainWidget w;
    w.show();

    int ret = a.exec();

    easytr::updateTranslationsMappingFiles();

    return ret;
}
