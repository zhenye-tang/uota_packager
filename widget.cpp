#include "widget.h"
#include "ota.h"
#include "compressfactory.h"
#include "lz4compressor.h"
#include "QDebug"

/* user code */
static Compressor *lz4CompressorCreate()
{
    Compressor *obj = new Lz4Compressor;
    qDebug() << obj;
    return obj;
}

static void lz4CompressorDestory(Compressor *compressor)
{
    qDebug() << compressor;
    delete compressor;
}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(200,200);
    setWindowTitle("uota");

    const compressFactory factory;
    factory.compressorRegister("LZ4", lz4CompressorCreate, lz4CompressorDestory);
    OTA *ota_tool = new OTA(this, factory);
}

Widget::~Widget()
{

}
