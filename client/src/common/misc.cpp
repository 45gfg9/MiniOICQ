#include "misc.h"
#include <QDebug>

namespace MINIOICQ
{

void Error(QString message)
{
    qCritical() << "\033[1;31m[Error]\033[0m " + message;
    abort();
}

void Warning(QString message)
{
    qWarning() << "\033[1;33m[Warning]\033[0m " + message;
}

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

QByteArray toByteArray(QImage& image)
{
    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");
    return arr;
}

QByteArray toByteArray(QImage image)
{
    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");
    if(arr.size() == 0)
    {
        Warning("toByteArray zero");
    }
    return arr;
}

void initTheme() { themePalette.setColor(QPalette::Window, Qt::white); }

} // namespace MINIOICQ
