#include "ota.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QCoreApplication>
#include "compressor.h"

typedef enum compress_type
{
    OTA_LZ4,
    OTA_ZIP,
    OTA_FASTLZ
}compress_type_t;

typedef enum digest_type
{
    OTA_MD5 = 0,
    OTA_SHA1,
    OTA_SHA256,
}digest_type_t;

struct ota_head
{
    char magic[4];                           /* magic: 'uota' */
    uint32_t image_size;                     /* 镜像大小, 包括头大小 */
    char image_version[8];                   /* 镜像版本号 */
    char image_partition[8];                 /* 镜像所在的分区名字 */
    unsigned char image_digest[32];          /* 镜像摘要 */
    uint32_t image_digest_len;               /* 摘要实际长度 */
    digest_type_t digest_type;               /* 采用的摘要算法 */
    compress_type_t compress_type;           /* 采用的压缩算法 */
    uint32_t raw_size;                       /* 镜像原始尺寸 */
};

static uint32_t hash_len[3] = {16, 20, 32};

OTA::OTA(QWidget *parent, const compressFactory& factory) : QWidget(parent), compressorFactory(factory)
{
    QVBoxLayout *layout = new QVBoxLayout(parent);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(10);

    QPushButton *selectFileButton = new QPushButton("选择文件", this);
    connect(selectFileButton, &QPushButton::clicked, this, &OTA::selectFile);
    layout->addWidget(selectFileButton);

    versionLineEdit = new QLineEdit(this);
    versionLineEdit->setMaxLength(5);
    versionLineEdit->setPlaceholderText("输入版本号");
    layout->addWidget(versionLineEdit);

    outputFileNameLineEdit = new QLineEdit(this);
    outputFileNameLineEdit->setPlaceholderText("输出文件名");
    layout->addWidget(outputFileNameLineEdit);

    partitionLineEdit = new QLineEdit(this);
    partitionLineEdit->setMaxLength(8);
    partitionLineEdit->setPlaceholderText("输入镜像分区名");
    layout->addWidget(partitionLineEdit);

    hashAlgorithmComboBox = new QComboBox(this);
    hashAlgorithmComboBox->addItem("MD5");
    hashAlgorithmComboBox->addItem("SHA-1");
    hashAlgorithmComboBox->addItem("SHA-256");
    layout->addWidget(hashAlgorithmComboBox);

    compressionAlgorithmComboBox = new QComboBox(this);
    compressionAlgorithmComboBox->addItem("LZ4");
    compressionAlgorithmComboBox->addItem("ZLIB");
    compressionAlgorithmComboBox->addItem("FASTLZ");
    layout->addWidget(compressionAlgorithmComboBox);

    QPushButton *packButton = new QPushButton("打包", this);
    connect(packButton, &QPushButton::clicked, this, &OTA::package);
    layout->addWidget(packButton);
}

void OTA::selectFile()
{
    filePath = QFileDialog::getOpenFileName(this, "选择文件");
    qDebug() << "选择的文件路径：" << filePath;
}

void OTA::package()
{
    struct ota_head header;
    memset(&header, 0, sizeof(header));

    if(filePath.isEmpty())
    {
        QMessageBox::warning(this, "警告", "请选择要打包的文件");
        return;
    }
    else if(outputFileNameLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "警告", "清输入待打包的文件名");
        return;
    }

    header.magic[0] = 'u';
    header.magic[1] = 'o';
    header.magic[2] = 't';
    header.magic[3] = 'a';

    int hashCurrentIndex = hashAlgorithmComboBox->currentIndex();
    int compressCurrentIndex = compressionAlgorithmComboBox->currentIndex();
    qDebug() << hashCurrentIndex;

    QCryptographicHash hash(QCryptographicHash::Algorithm(QCryptographicHash::Md5 + (hashCurrentIndex == 2 ? hashCurrentIndex + 1 : hashCurrentIndex)));

    qDebug() << compressionAlgorithmComboBox->currentText();

    Compressor* compressor = compressorFactory.compressorCreate(compressionAlgorithmComboBox->currentText().toStdString());

    QString currentPath = QCoreApplication::applicationDirPath();
    QString newFilePath = currentPath + "/" + outputFileNameLineEdit->text();

    int comressFileSize = compressor->compress(filePath.toStdString(), newFilePath.toStdString());

    qstrncpy(header.image_version, versionLineEdit->text().toUtf8().data(), sizeof(header.image_version));
    qstrncpy(header.image_partition, partitionLineEdit->text().toUtf8().data(), sizeof(header.image_partition));

    QFile newFile(newFilePath);
    QFile srcFile(filePath);

    srcFile.open(QFile::ReadOnly);
    newFile.open(QIODevice::ReadWrite);

    header.image_digest_len = hash_len[hashCurrentIndex];
    header.digest_type = (digest_type_t)hashCurrentIndex;
    header.compress_type = (compress_type_t)compressCurrentIndex;
    header.image_size = comressFileSize + sizeof(header);
    header.raw_size = srcFile.size();

    qDebug() << "header.raw_size" << header.raw_size;

    QByteArray existingData = newFile.readAll();
    /* write header, Prepare calcu hash */
    newFile.seek(0);
    newFile.write((const char *)&header, sizeof(header));
    newFile.write(existingData);
    newFile.flush();
    newFile.close();

    /* reopen, calculate hash */
    newFile.open(QIODevice::ReadOnly);
    hash.addData(&newFile);
    newFile.close();

    /* reopen, uodate  */
    newFile.open(QFile::ReadWrite);
    QByteArray hashResult = hash.result();
    memcpy(&header.image_digest, hashResult.constData(), hashResult.size());
    QString md5Hash = QString(hashResult.toHex());
    newFile.seek(0);
    newFile.write((const char *)&header, sizeof(header));

    newFile.close();
    srcFile.close();
    compressorFactory.compressorDestory(compressionAlgorithmComboBox->currentText().toStdString(), compressor);
}

OTA::~OTA()
{
    qDebug() << "byby：";
}
