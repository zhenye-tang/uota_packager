#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QCoreApplication>
#include "compressor.h"

#include "lz4compressor.h"
#include "lz4.h"

enum {
    BLOCK_BYTES = 1024 * 8,
};

static char inpBuf[2][BLOCK_BYTES];
static char cmpBuf[LZ4_COMPRESSBOUND(BLOCK_BYTES)];

int Lz4Compressor::compress(const string& sourceFile, const string& compressFile)
{
    int file_size = 0;
    LZ4_stream_t *lz4Stream = LZ4_createStream();
    const QString CompressFileName = QString::fromStdString(compressFile);
    const QString sourceFileName = QString::fromStdString(sourceFile);

    QFile srcFile(sourceFileName);
    QFile newFile(CompressFileName);

    LZ4_initStream(lz4Stream, sizeof (*lz4Stream));

    int inpBufIndex = 0;
    int cmpBytes = 0;
    if (srcFile.open(QFile::ReadOnly) && newFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        for(;;)
        {
            char* const inpPtr = inpBuf[inpBufIndex];
            const int inpBytes = (int) srcFile.read((char *)inpPtr, BLOCK_BYTES);
            if(0 >= inpBytes)
            {
                break;
            }

            {
                cmpBytes = LZ4_compress_fast_continue(
                    lz4Stream, inpPtr, cmpBuf, inpBytes, sizeof(cmpBuf), 1);
                if(cmpBytes <= 0)
                {
                    break;
                }
                newFile.write((const char *)&cmpBytes, 4);
                newFile.write((const char *)cmpBuf, cmpBytes);
            }

            inpBufIndex = (inpBufIndex + 1) % 2;
            file_size += cmpBytes + 4;
        }

        srcFile.close();
        newFile.close();
    }

    qDebug() << "lz4 compress : " << file_size;
    return file_size;
}
