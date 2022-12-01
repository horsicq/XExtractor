/* Copyright (c) 2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "xextractor.h"

XExtractor::XExtractor(QObject *pParent) : QObject(pParent)
{
    g_pDevice = nullptr;
    g_pData = nullptr;
    g_pPdStruct = nullptr;
    g_nFreeIndex = 0;
}

void XExtractor::setData(QIODevice *pDevice, DATA *pData, XBinary::PDSTRUCT *pPdStruct)
{
    g_pDevice = pDevice;
    g_pData = pData;
    g_pPdStruct = pPdStruct;
}

QList<XBinary::FT> XExtractor::getAvailableFileTypes()
{
    QList<XBinary::FT> listResult;

    listResult.append(XBinary::FT_PE);
    listResult.append(XBinary::FT_ELF);
    listResult.append(XBinary::FT_MACHOFAT);
    listResult.append(XBinary::FT_MACHO);
    listResult.append(XBinary::FT_PDF);
    listResult.append(XBinary::FT_PNG);
    listResult.append(XBinary::FT_JPEG);
    listResult.append(XBinary::FT_TIFF);
    listResult.append(XBinary::FT_BMP);
    listResult.append(XBinary::FT_GIF);
    listResult.append(XBinary::FT_ICO);
    listResult.append(XBinary::FT_DEX);
    listResult.append(XBinary::FT_ZIP);
    listResult.append(XBinary::FT_RAR);
    listResult.append(XBinary::FT_7Z);
    listResult.append(XBinary::FT_CAB);

    return listResult;
}

XExtractor::OPTIONS XExtractor::getDefaultOptions()
{
    XExtractor::OPTIONS result = {};

    result.listFileTypes.append(XBinary::FT_PE);
    result.listFileTypes.append(XBinary::FT_ELF);
    result.listFileTypes.append(XBinary::FT_MACHOFAT);
    result.listFileTypes.append(XBinary::FT_MACHO);
    result.listFileTypes.append(XBinary::FT_ZIP);
    result.listFileTypes.append(XBinary::FT_RAR);
    result.listFileTypes.append(XBinary::FT_PDF);
    result.listFileTypes.append(XBinary::FT_7Z);
    result.listFileTypes.append(XBinary::FT_PNG);
    result.listFileTypes.append(XBinary::FT_JPEG);
    result.listFileTypes.append(XBinary::FT_TIFF);
    result.listFileTypes.append(XBinary::FT_BMP);
    result.listFileTypes.append(XBinary::FT_GIF);
    result.listFileTypes.append(XBinary::FT_CAB);
    result.listFileTypes.append(XBinary::FT_ICO);
    result.listFileTypes.append(XBinary::FT_DEX);

    result.bDeepScan = true;

    return result;
}

qint64 XExtractor::tryToAddRecord(qint64 nOffset, XBinary::FT fileType)
{
    qint64 nResult = 0;

    SubDevice subevice(g_pDevice, nOffset, -1);

    if (subevice.open(QIODevice::ReadOnly)) {
        XBinary::FILEFORMATINFO formatInfo = XFormats::getFileFormatInfo(fileType, &subevice);

        if (formatInfo.bIsValid) {
            RECORD record = {};

            record.nOffset = nOffset;
            record.nSize = formatInfo.nSize;

            if (record.nSize) {
                record.sString = formatInfo.sString;
                record.sExt = formatInfo.sExt;
                record.fileType = formatInfo.fileType;

                // Fix if more than the device size
                if ((record.nOffset + record.nSize) > g_pDevice->size()) {
                    record.nSize = (g_pDevice->size() - record.nOffset);
                }

                g_pData->listRecords.append(record);
            }

            nResult = record.nSize;
        }

        subevice.close();
    }

    if (nResult == 0) {
        nResult = 1;
    }

    //    if ((g_pData->options.bDeepScan) && (fileType != XBinary::FT_ZIP)) {
    //        nResult = 1;
    //    }

    if (g_pData->options.bDeepScan) {
        nResult = 1;
    }

    return nResult;
}

void XExtractor::handleSearch(XBinary *pBinary, XBinary::_MEMORY_MAP *pMemoryMap, XBinary::FT fileType, QString sSignature)
{
    if (g_pData->options.listFileTypes.contains(fileType)) {
        qint64 nOffset = 0;

        qint32 _nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
        XBinary::setPdStructInit(g_pPdStruct, _nFreeIndex, pBinary->getSize());

        while (!(g_pPdStruct->bIsStop)) {
            nOffset = pBinary->find_signature(pMemoryMap, nOffset, -1, sSignature, nullptr, g_pPdStruct);

            if (nOffset != -1) {
                nOffset += tryToAddRecord(nOffset, fileType);
            } else {
                break;
            }

            XBinary::setPdStructCurrent(g_pPdStruct, _nFreeIndex, nOffset);
        }

        XBinary::setPdStructFinished(g_pPdStruct, _nFreeIndex);

        XBinary::setPdStructCurrentIncrement(g_pPdStruct, g_nFreeIndex);
    }
}

void XExtractor::process()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    g_pData->listRecords.clear();

    g_nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
    XBinary::setPdStructInit(g_pPdStruct, g_nFreeIndex, g_pData->options.listFileTypes.count());

    XBinary binary(g_pDevice);

    XBinary::_MEMORY_MAP memoryMap = binary.getMemoryMap();

    connect(&binary, SIGNAL(errorMessage(QString)), this, SIGNAL(errorMessage(QString)));

    handleSearch(&binary, &memoryMap, XBinary::FT_PE, "'MZ'");
    handleSearch(&binary, &memoryMap, XBinary::FT_ELF, "7F'ELF'");
    handleSearch(&binary, &memoryMap, XBinary::FT_7Z, "'7z'BCAF271C");
    handleSearch(&binary, &memoryMap, XBinary::FT_ZIP, "'PK'0304");
    handleSearch(&binary, &memoryMap, XBinary::FT_RAR, "'Rar!'1A07");
    handleSearch(&binary, &memoryMap, XBinary::FT_DEX, "'dex\n'");
    handleSearch(&binary, &memoryMap, XBinary::FT_PDF, "'%PDF'");
    handleSearch(&binary, &memoryMap, XBinary::FT_PNG, "89'PNG\r\n'1A0A");
    handleSearch(&binary, &memoryMap, XBinary::FT_JPEG, "FFD8FF");
    handleSearch(&binary, &memoryMap, XBinary::FT_CAB, "'MSCF'");
    handleSearch(&binary, &memoryMap, XBinary::FT_ICO, "00000100");
    handleSearch(&binary, &memoryMap, XBinary::FT_ICO, "00000200");  // CUR
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHO, "FEEDFACE");
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHO, "CEFAEDFE");
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHO, "FEEDFACF");
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHO, "CFFAEDFE");
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHOFAT, "CAFEBABE");
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHOFAT, "BEBAFECA");
    handleSearch(&binary, &memoryMap, XBinary::FT_BMP, "'BM'");
    handleSearch(&binary, &memoryMap, XBinary::FT_GIF, "'GIF8'");
    handleSearch(&binary, &memoryMap, XBinary::FT_TIFF, "'MM'002A");
    handleSearch(&binary, &memoryMap, XBinary::FT_TIFF, "'II'2A00");
    // TODO more

    XBinary::setPdStructFinished(g_pPdStruct, g_nFreeIndex);

    emit completed(scanTimer.elapsed());
}
