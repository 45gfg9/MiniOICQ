#ifndef MISC_H
#define MISC_H

#include <QColor>
#include <QPalette>
#include <QFont>
#include <QMargins>
#include <QImage>
#include <QByteArray>
#include <QBuffer>

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
void Error(QString message);
void Warning(QString message);
QByteArray toByteArray(QImage& image);
QByteArray toByteArray(QImage image);

} // namespace MINIOICQ

#endif
