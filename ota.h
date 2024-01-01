#ifndef OTA_H
#define OTA_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include "compressfactory.h"

class OTA : public QWidget
{
    Q_OBJECT

public:
    OTA(QWidget *parent, const compressFactory& factory);
    ~OTA();

private:
    QString filePath;
    QComboBox *hashAlgorithmComboBox;
    QComboBox *compressionAlgorithmComboBox;
    QLineEdit *versionLineEdit;
    QLineEdit *partitionLineEdit;
    QLineEdit *outputFileNameLineEdit;
    const compressFactory& compressorFactory;

private slots:
    void selectFile();
    void package();
};

#endif // OTA_H
