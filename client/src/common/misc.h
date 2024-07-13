#ifndef MISC_H
#define MISC_H

#include <QColor>
#include <QPalette>
#include <QFont>
#include <QMargins>

namespace MINIOICQ
{

extern QColor themeColor;
extern QPalette themePalette;
extern QFont themeFont;
extern QMargins zeroMargins;
extern QString themeStyleSheetDeep;
extern QString themeStyleSheetLight;
extern QString themeStyleSheetDim;

void initTheme();

} // namespace MINIOICQ

#endif
