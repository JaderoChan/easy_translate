#include "language.h"

#include <qapplication.h>
#include <qevent.h>

#include <easy_translate.hpp>

static LanguageID gCurrentLanguage = LANG_EN;

QString getLanguageIdStr(LanguageID langId)
{
    switch (langId)
    {
        case LANG_EN: return "EN";
        case LANG_ZH: return "ZH";
        default:      return "";
    }
}

void setLanguage(LanguageID langId)
{
    bool ret = easytr::setCurrentLanguage(getLanguageIdStr(langId).toStdString());
    if (!ret)
    {
        qDebug() << "Failed to set the current language to: " << getLanguageIdStr(langId);
        return;
    }
    gCurrentLanguage = langId;
    QEvent event(QEvent::LanguageChange);
    qApp->sendEvent(qApp, &event);
}

LanguageID currentLanguage()
{
    return gCurrentLanguage;
}
