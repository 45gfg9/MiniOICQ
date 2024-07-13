#include "misc.h"

namespace MINIOICQ
{

QColor themeColor = QColor(0, 188, 212);
QPalette themePalette = QPalette();
QFont themeFont = QFont("Roboto");
QMargins zeroMargins = QMargins(0, 0, 0, 0);
QString themeStyleSheetDeep =
    QString("background-color: %1;").arg(themeColor.name()) +
    QString("color: white;");
QString themeStyleSheetLight = QString("background-color: white;") +
                               QString("color: %1;").arg(themeColor.name());
QString themeStyleSheetDim = QString("color: gray;");

void initTheme() { themePalette.setColor(QPalette::Background, Qt::white); }

} // namespace MINIOICQ
