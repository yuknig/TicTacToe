#include "Svgs.h"
#include <QXmlStreamReader>
#include <QPainter>
#include <QSvgRenderer>

namespace {

const char* const XoSvgText = R"(
<svg fill="#000000" height="200px" width="200px" version="1.1" id="Capa_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 455.896 455.896" xml:space="preserve">
    <g id="X">
        <path d="M27.652,106.497l25.235-25.241l25.235,25.241c3.11,3.11,7.184,4.666,11.26,4.666c4.075,0,8.149-1.556,11.26-4.666 c6.222-6.213,6.222-16.299,0-22.519L75.408,58.735l25.233-25.243c6.222-6.221,6.222-16.306,0-22.519 c-6.221-6.221-16.299-6.221-22.519,0L52.887,36.214L27.652,10.973c-6.221-6.221-16.299-6.221-22.519,0 c-6.222,6.213-6.222,16.299,0,22.519l25.233,25.243L5.133,83.977c-6.222,6.221-6.222,16.307,0,22.519 c3.11,3.11,7.184,4.666,11.26,4.666C20.468,111.163,24.542,109.607,27.652,106.497z"/>
    </g>
    <g id="O">
        <path d="M228.072,117.47c32.389,0,58.735-26.346,58.735-58.735C286.807,26.346,260.461,0,228.072,0 c-32.389,0-58.735,26.346-58.735,58.735C169.337,91.123,195.684,117.47,228.072,117.47z M228.072,31.852 c14.822,0,26.883,12.061,26.883,26.883c0,14.822-12.061,26.883-26.883,26.883c-14.822,0-26.883-12.061-26.883-26.883 C201.189,43.913,213.25,31.852,228.072,31.852z"/>
    </g>
</svg>
)";

QPixmap RenderSvg(const QAnyStringView& svgContent, const QSize& dstSize, const QString& id)
{
    QXmlStreamReader xmlReader(svgContent);
    QSvgRenderer renderer(&xmlReader);

    QPixmap pixmap(dstSize);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    QRectF rect(QRect({}, dstSize));
    renderer.render(&painter, id, rect);
    return pixmap;
}

} // namespace anonymous


QPixmap GetIconX(QSize size)
{
    return RenderSvg(XoSvgText, size, "X");
}

QPixmap GetIconO(QSize size)
{
    return RenderSvg(XoSvgText, size, "O");
}
