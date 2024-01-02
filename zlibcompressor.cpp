#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QCoreApplication>

#include "zlibcompressor.h"
#include "zlib.h"

#define CHUNK 8192
static char raw_buf[CHUNK];
static char compress_buf[CHUNK];

int zlibCompressor::compress(const string& sourceFile, const string& compressFile)
{
    int flush;
    unsigned have;
    uint32_t compress_size = 0;
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    const QString CompressFileName = QString::fromStdString(compressFile);
    const QString sourceFileName = QString::fromStdString(sourceFile);
    QFile srcFile(sourceFileName);
    QFile newFile(CompressFileName);

    deflateInit(&strm, Z_BEST_COMPRESSION);

    if (srcFile.open(QFile::ReadOnly) && newFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        do {
            strm.avail_in = srcFile.read(raw_buf, CHUNK);
            flush = strm.avail_in ? Z_NO_FLUSH:Z_FINISH;
            strm.next_in = (z_const Bytef *)raw_buf;
            do {
                strm.avail_out = CHUNK;
                strm.next_out = (z_const Bytef *)compress_buf;
                deflate(&strm, flush);
                have = CHUNK - strm.avail_out;
                if (newFile.write((const char *)compress_buf, have) != have)
                {
                    (void)deflateEnd(&strm);
                    return Z_ERRNO;
                }
                compress_size += have;
            } while (strm.avail_out == 0);
            /* done when last data in file processed */
        } while (flush != Z_FINISH);
    }

    (void)deflateEnd(&strm);
    return compress_size;
}
