/* Copyright (c) 2022-2025 hors<horsicq@gmail.com>
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

XBinary::XCONVERT _TABLE_XExtractor_EMODE[] = {
    {XExtractor::EMODE_UNKNOWN, "Unknown", QObject::tr("Unknown")}, {XExtractor::EMODE_RAW, "Raw", QObject::tr("Raw")},
    {XExtractor::EMODE_FORMAT, "Format", QObject::tr("Format")},    {XExtractor::EMODE_HEURISTIC, "Heuristic", QObject::tr("Heuristic")},
    {XExtractor::EMODE_UNPACK, "Unpack", QObject::tr("Unpack")},
};

bool compareXExtractor(const XExtractor::RECORD &a, const XExtractor::RECORD &b)
{
    return a.nOffset < b.nOffset;
}

XExtractor::XExtractor(QObject *pParent) : XThreadObject(pParent)
{
    m_pDevice = nullptr;
    m_pData = nullptr;
    m_pPdStruct = nullptr;
}

void XExtractor::setData(QIODevice *pDevice, DATA *pData, XBinary::PDSTRUCT *pPdStruct)
{
    m_pDevice = pDevice;
    m_pData = pData;
    m_pPdStruct = pPdStruct;
}

bool XExtractor::processFile(const QString &sFileName, DATA *pData, XBinary::PDSTRUCT *pPdStruct)
{
    bool bResult = false;

    QFile file(sFileName);

    if (file.open(QIODevice::ReadOnly)) {
        setData(&file, pData, pPdStruct);
        process();

        bResult = true;

        file.close();
    }

    return bResult;
}

QList<XBinary::FT> XExtractor::getAvailableFileTypes(EMODE emode)
{
    QList<XBinary::FT> listResult;

    if ((emode == EMODE_RAW) || (emode == EMODE_FORMAT)) {
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
        listResult.append(XBinary::FT_GZIP);
        listResult.append(XBinary::FT_ZLIB);
        listResult.append(XBinary::FT_7Z);
        listResult.append(XBinary::FT_CAB);
        listResult.append(XBinary::FT_MP3);
        listResult.append(XBinary::FT_MP4);
        listResult.append(XBinary::FT_RIFF);
        listResult.append(XBinary::FT_LE);
        listResult.append(XBinary::FT_NE);
        listResult.append(XBinary::FT_AMIGAHUNK);
        listResult.append(XBinary::FT_JAVACLASS);
        listResult.append(XBinary::FT_SZDD);
        listResult.append(XBinary::FT_BZIP2);
        listResult.append(XBinary::FT_LHA);
        listResult.append(XBinary::FT_DJVU);
    }

    if (emode == EMODE_FORMAT) {
        listResult.append(XBinary::FT_BINARY);
    }

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
    result.listFileTypes.append(XBinary::FT_GZIP);
    result.listFileTypes.append(XBinary::FT_ZLIB);
    result.listFileTypes.append(XBinary::FT_PDF);
    result.listFileTypes.append(XBinary::FT_7Z);
    result.listFileTypes.append(XBinary::FT_PNG);
    result.listFileTypes.append(XBinary::FT_JPEG);
    // result.listFileTypes.append(XBinary::FT_TIFF);
    result.listFileTypes.append(XBinary::FT_BMP);
    result.listFileTypes.append(XBinary::FT_GIF);
    result.listFileTypes.append(XBinary::FT_CAB);
    //    result.listFileTypes.append(XBinary::FT_ICO);
    result.listFileTypes.append(XBinary::FT_DEX);
    result.listFileTypes.append(XBinary::FT_MP3);
    result.listFileTypes.append(XBinary::FT_MP4);
    result.listFileTypes.append(XBinary::FT_RIFF);
    // result.listFileTypes.append(XBinary::FT_LE);
    // result.listFileTypes.append(XBinary::FT_NE);
    // result.listFileTypes.append(XBinary::FT_AMIGAHUNK);
    // result.listFileTypes.append(XBinary::FT_JAVACLASS);
    // result.listFileTypes.append(XBinary::FT_SIGNATURE);
    // result.listFileTypes.append(XBinary::FT_OTHER);

    result.bDeepScan = true;
    result.bAnalyze = true;
    // result.bHeuristicScan = true;

    result.nBufferSize = 2 * 1024 * 1024;

    return result;
}

QAbstractItemModel *XExtractor::createModelFromRecords(DATA *pData)
{
    return 0;
}

QString XExtractor::extractorModeToString(EMODE mode)
{
    return XBinary::XCONVERT_idToTransString(mode, _TABLE_XExtractor_EMODE, sizeof(_TABLE_XExtractor_EMODE) / sizeof(XBinary::XCONVERT));
}

bool XExtractor::isFormatModeAvailable(XBinary::FT fileType)
{
    bool bResult = false;

    if ((fileType == XBinary::FT_ZIP) || (fileType == XBinary::FT_JAR) || (fileType == XBinary::FT_APK) || (fileType == XBinary::FT_APKS) ||
        (fileType == XBinary::FT_PDF) || (fileType == XBinary::FT_TAR) || (fileType == XBinary::FT_PE32) || (fileType == XBinary::FT_PE64)) {
        bResult = true;
    }

    return bResult;
}

bool XExtractor::isUnpackModeAvailable(XBinary::FT fileType)
{
    bool bResult = false;

    if ((fileType == XBinary::FT_ZIP) || (fileType == XBinary::FT_JAR) || (fileType == XBinary::FT_APK) || (fileType == XBinary::FT_APKS) ||
        (fileType == XBinary::FT_PDF) || (fileType == XBinary::FT_TAR)) {
        bResult = true;
    }

    return bResult;
}

XExtractor::EMODE XExtractor::ftStringToExtractorMode(QString sString)
{
    return (XExtractor::EMODE)XBinary::XCONVERT_ftStringToId(sString, _TABLE_XExtractor_EMODE, sizeof(_TABLE_XExtractor_EMODE) / sizeof(XBinary::XCONVERT));
}

QVector<XExtractor::RECORD> XExtractor::scanDevice(QIODevice *pDevice, OPTIONS options, XBinary::PDSTRUCT *pPdStruct)
{
    DATA _data = {};
    _data.options = options;

    XExtractor _extractor;
    _extractor.setData(pDevice, &_data, pPdStruct);
    _extractor.process();

    return _data.listRecords;
}

void XExtractor::handleSearch(qint32 nGlobalIndex, XBinary *pBinary, DATA *pData, XBinary::FT fileType, const QString &sSignature, qint32 nDelta,
                              XBinary::PDSTRUCT *pPdStruct)
{
    if (m_pData->options.listFileTypes.contains(fileType) || m_pData->options.bAllTypes) {
        XBinary::setPdStructStatus(pPdStruct, nGlobalIndex, XBinary::fileTypeIdToString(fileType));

        bool bNextByte = true;
        // mb TODO all archives!
        if (fileType == XBinary::FT_LHA) {
            bNextByte = false;
        }

        qint64 nOffset = 0;

        qint32 _nFreeIndex = XBinary::getFreeIndex(pPdStruct);
        XBinary::setPdStructInit(pPdStruct, _nFreeIndex, pBinary->getSize());

        qint32 nFound = 0;

        while (XBinary::isPdStructNotCanceled(pPdStruct)) {
            nOffset = pBinary->find_signature(&(pData->memoryMap), nOffset, -1, sSignature, nullptr, pPdStruct);

            if (nOffset != -1) {
                qint64 _nOffset = nOffset + nDelta;

                if (fileType == XBinary::FT_SIGNATURE) {
                    // TODO
                    /*
                    CRC8 0x07
                    CRC8_SAE_J1850 0x1D
                    CRC8_SAE_J1850_ZERO 0x1D
                    CRC8_8H2F 0x2F
                    CRC8_CDMA2000 0x9B
                    CRC8_DARC 0x39
                    CRC8_DVB_S2 0xD5
                    CRC8_EBU 0x1D
                    CRC8_ICODE 0x1D
                    CRC8_ITU 0x07
                    CRC8_MAXIM 0x31
                    CRC8_ROHC 0x07
                    CRC8_WCDMA 0x9B

                    CRC16_CCIT_ZERO 0x1021
                    CRC16_ARC 0x8005
                    CRC16_AUG_CCITT 0x1021
                    CRC16_BUYPASS 0x8005
                    CRC16_CCITT_FALSE 0x1021
                    CRC16_CDMA2000 0xC867
                    CRC16_DDS_110 0x8005
                    CRC16_DECT_R 0x0589
                    CRC16_DECT_X 0x0589
                    CRC16_DNP 0x3D65
                    CRC16_EN_13757 0x3D65
                    CRC16_GENIBUS 0x1021
                    CRC16_MAXIM 0x8005
                    CRC16_MCRF4XX 0x1021
                    CRC16_RIELLO 0x1021
                    CRC16_T10_DIF 0x8BB7
                    CRC16_TELEDISK 0xA097
                    CRC16_TMS37157 0x1021
                    CRC16_USB 0x8005
                    CRC16_A 0x1021
                    CRC16_KERMIT 0x1021
                    CRC16_MODBUS 0x8005
                    CRC16_X_25 0x1021
                    CRC16_XMODEM 0x1021

                    CRC32 0x04C11DB7
                    CRC32_BZIP2 0x04C11DB7
                    CRC32_C 0x1EDC6F41
                    CRC32_D 0xA833982B
                    CRC32_MPEG2 0x04C11DB7
                    CRC32_POSIX 0x04C11DB7
                    CRC32_Q 0x814141AB
                    CRC32_JAMCRC 0x04C11DB7
                    CRC32_XFER 0x000000AF

                    CRC64_ECMA_182 0x42F0E1EBA9EA3693
                    CRC64_GO_ISO 0x000000000000001B
                    CRC64_WE 0x42F0E1EBA9EA3693
                    CRC64_XZ 0x42F0E1EBA9EA3693

                    */

                    nOffset += 1;
                } else {
                    qint64 nResSize = 0;
                    XBinary::FT _fileType = XBinary::FT_UNKNOWN;
                    qint64 nFileFormatSize = 0;

                    {
                        SubDevice subdevice(m_pDevice, _nOffset, -1);

                        if (subdevice.open(QIODevice::ReadOnly)) {
                            if (XFormats::isValid(fileType, &subdevice, false, -1, pPdStruct)) {
                                QSet<XBinary::FT> stFT = XFormats::getFileTypes(&subdevice, true, pPdStruct);
                                _fileType = XBinary::_getPrefFileType(&stFT);
                                nFileFormatSize = XFormats::getFileFormatSize(_fileType, &subdevice, false, -1, pPdStruct);
                            }

                            subdevice.close();
                        }
                    }

                    if (nFileFormatSize) {
                        SubDevice subdevice(m_pDevice, _nOffset, nFileFormatSize);

                        if (subdevice.open(QIODevice::ReadOnly)) {
                            XBinary::FILEFORMATINFO formatInfo = XFormats::getFileFormatInfo(_fileType, &subdevice, false, -1, pPdStruct);

                            if (formatInfo.bIsValid) {
                                RECORD record = {};

                                record.mapProperties.insert(XBinary::FPART_PROP_COMPRESSMETHOD, XArchive::COMPRESS_METHOD_STORE);
                                record.nOffset = _nOffset;
                                record.nSize = nFileFormatSize;

                                if (record.nSize) {
                                    record.sString = XBinary::getFileFormatString(&formatInfo);
                                    record.sExt = formatInfo.sExt;
                                    record.fileType = formatInfo.fileType;

                                    // // Fix if more than the device size
                                    // if ((record.nOffset + record.nSize) > m_pDevice->size()) {
                                    //     record.nSize = (m_pDevice->size() - record.nOffset);
                                    // }

                                    record.nCRC = XBinary::_getCRC32(&subdevice, pPdStruct);

                                    m_pData->listRecords.append(record);

                                    nFound++;
                                }

                                nResSize = record.nSize;
                            }

                            subdevice.close();
                        }
                    }

                    if (nResSize == 0) {
                        nResSize = 1;
                    }

                    //    if ((m_pData->options.bDeepScan) && (fileType != XBinary::FT_ZIP)) {
                    //        nResult = 1;
                    //    }

                    if (m_pData->options.bDeepScan) {
                        if (bNextByte) {
                            nResSize = 1;
                        }
                    }

                    nOffset += nResSize;
                }

            } else {
                break;
            }

            if ((m_pData->options.nLimit > 0) && (nFound >= m_pData->options.nLimit)) {
                break;
            }

            XBinary::setPdStructCurrent(pPdStruct, _nFreeIndex, nOffset);
        }

        XBinary::setPdStructFinished(pPdStruct, _nFreeIndex);
        XBinary::setPdStructCurrentIncrement(pPdStruct, nGlobalIndex);
    }
}

void XExtractor::handleRaw()
{
    m_pData->emode = XExtractor::EMODE_RAW;
    m_pData->listRecords.clear();

    qint32 nSearchCount = 0;

    if (m_pData->options.bAllTypes) {
        nSearchCount = getAvailableFileTypes(XExtractor::EMODE_RAW).count();
    } else {
        nSearchCount = m_pData->options.listFileTypes.count();
    }

    if (m_pData->options.listFileTypes.contains(XBinary::FT_ICO) || (m_pData->options.bAllTypes)) {
        nSearchCount++;
    }

    if (m_pData->options.listFileTypes.contains(XBinary::FT_MACHO) || (m_pData->options.bAllTypes)) {
        nSearchCount += 3;
    }

    if (m_pData->options.listFileTypes.contains(XBinary::FT_MACHOFAT) || (m_pData->options.bAllTypes)) {
        nSearchCount++;
    }

    if (m_pData->options.listFileTypes.contains(XBinary::FT_TIFF) || (m_pData->options.bAllTypes)) {
        nSearchCount++;
    }

    // if (m_pData->options.listFileTypes.contains(XBinary::FT_RIFF)) {
    //     nSearchCount += 2;
    // }

    if (m_pData->options.listFileTypes.contains(XBinary::FT_AMIGAHUNK) || (m_pData->options.bAllTypes)) {
        nSearchCount++;
    }

    if (m_pData->options.listFileTypes.contains(XBinary::FT_DJVU) || (m_pData->options.bAllTypes)) {
        nSearchCount++;
    }

    if (m_pData->options.listFileTypes.contains(XBinary::FT_LHA) || (m_pData->options.bAllTypes)) {
        nSearchCount += 2;
    }

    if (m_pData->options.listFileTypes.contains(XBinary::FT_BZIP2) || (m_pData->options.bAllTypes)) {
        nSearchCount++;
    }

    qint32 nGlobalIndex = XBinary::getFreeIndex(m_pPdStruct);
    XBinary::setPdStructInit(m_pPdStruct, nGlobalIndex, nSearchCount);

    // TODO signatures
    XBinary binary(m_pDevice);

    connect(&binary, SIGNAL(errorMessage(QString)), this, SIGNAL(errorMessage(QString)));

    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_PE, "'MZ'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_ELF, "7F'ELF'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_7Z, "'7z'BCAF271C", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_ZIP, "'PK'0304", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_RAR, "'Rar!'1A07", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_GZIP, "1F8B08", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_ZLIB, "785E", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_ZLIB, "789C", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_ZLIB, "78DA", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_DEX, "'dex\n'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_PDF, "'%PDF'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_PNG, "89'PNG\r\n'1A0A", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_JPEG, "FFD8FF", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_CAB, "'MSCF'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_ICO, "00000100", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_CUR, "00000200", 0, m_pPdStruct);  // CUR
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_MACHO, "FEEDFACE", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_MACHO, "CEFAEDFE", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_MACHO, "FEEDFACF", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_MACHO, "CFFAEDFE", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_MACHOFAT, "CAFEBABE", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_MACHOFAT, "BEBAFECA", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_BMP, "'BM'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_GIF, "'GIF8'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_TIFF, "'MM'002A", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_TIFF, "'II'2A00", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_MP3, "'ID3'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_MP4, "'ftyp'", -4, m_pPdStruct);  // 000000..'ftyp'
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_RIFF, "'RIFF'", 0, m_pPdStruct);
    // handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_RIFF, "'RIFX'", 0, m_pPdStruct);
    // handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_RIFF, "'AIFF'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_NE, "'MZ'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_LE, "'MZ'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_AMIGAHUNK, "000003F3", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_AMIGAHUNK, "000003E7", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_JAVACLASS, "CAFEBABE", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_DJVU, "'AT&TFORM'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_DJVU, "'SDJVFORM'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_SZDD, "'SZDD'88F027'3A'", 0, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_LHA, "'-lh'..2d'", -2, m_pPdStruct);  // "....'-lh'..2d"
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_LHA, "'-lz'..2d'", -2, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_LHA, "'-pm'..2d'", -2, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_BZIP2, "314159265359", -4, m_pPdStruct);
    handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_BZIP2, "17724538509000000000", -4, m_pPdStruct);  // Empty

    // TODO LE/BE
    // handleSearch(nGlobalIndex, &binary, m_pData, XBinary::FT_SIGNATURE, "00000000", -4, 0, "CRC32", "Test");
    // TODO more

    std::sort(m_pData->listRecords.begin(), m_pData->listRecords.end(), compareXExtractor);

    XBinary::setPdStructFinished(m_pPdStruct, nGlobalIndex);
}

void XExtractor::handleFormatAndUnpack(XBinary::FT fileType, bool bUnpack)
{
    m_pData->emode = (bUnpack ? EMODE_UNPACK : EMODE_FORMAT);
    m_pData->listRecords.clear();

    QList<XBinary::FPART> listParts;

    if ((fileType == XBinary::FT_ZIP) || (fileType == XBinary::FT_JAR) || (fileType == XBinary::FT_APK) || (fileType == XBinary::FT_APKS) ||
        (fileType == XBinary::FT_PDF) || (fileType == XBinary::FT_TAR)) {
        listParts = XFormats::getFileParts(fileType, m_pDevice, XBinary::FILEPART_STREAM, -1, false, -1, m_pPdStruct);
    } else if ((fileType == XBinary::FT_PE32) || (fileType == XBinary::FT_PE64)) {
        listParts = XFormats::getFileParts(fileType, m_pDevice, XBinary::FILEPART_RESOURCE, -1, false, -1, m_pPdStruct);
    }

    qint32 nNumberOfParts = listParts.count();

    if (nNumberOfParts > 0) {
        qint32 nGlobalIndex = XBinary::getFreeIndex(m_pPdStruct);
        XBinary::setPdStructInit(m_pPdStruct, nGlobalIndex, nNumberOfParts);

        for (qint32 i = 0; (i < nNumberOfParts) && XBinary::isPdStructNotCanceled(m_pPdStruct); i++) {
            XBinary::FPART fpart = listParts.at(i);

            QString sPrefName = fpart.mapProperties.value(XBinary::FPART_PROP_ORIGINALNAME).toString();

            if (sPrefName == "") {
                sPrefName = fpart.sName;
            }

            XBinary::setPdStructStatus(m_pPdStruct, nGlobalIndex, sPrefName);

            bool bAdd = false;

            if (fpart.nFileSize > 0) {
                RECORD record = {};
                record.nOffset = fpart.nFileOffset;
                record.nSize = fpart.nFileSize;
                record.mapProperties = fpart.mapProperties;
                record.sName = sPrefName;

                if (bUnpack) {
                    bAdd = true;
                } else {
                    QSet<XBinary::FT> stFileTypes;
                    XBinary::FT fileTypePref = (XBinary::FT)(fpart.mapProperties.value(XBinary::FPART_PROP_FILETYPE, XBinary::FT_UNKNOWN).toUInt());
                    XBinary::HANDLE_METHOD handleMethod =
                        (XBinary::HANDLE_METHOD)(fpart.mapProperties.value(XBinary::FPART_PROP_HANDLEMETHOD, XBinary::HANDLE_METHOD_UNKNOWN).toUInt());

                    if (handleMethod != XBinary::HANDLE_METHOD_UNKNOWN) {
                        stFileTypes.insert(fileTypePref);

                        record.sExt = fpart.mapProperties.value(XBinary::FPART_PROP_EXT, QString()).toString();
                        record.fileType = fileTypePref;
                        record.handleMethod = handleMethod;
                        record.sString = fpart.mapProperties.value(XBinary::FPART_PROP_INFO, false).toString();
                    } else {
                        XCompressedDevice compressedDevice;
                        compressedDevice.setData(m_pDevice, fpart, m_pPdStruct);

                        if (compressedDevice.open(QIODevice::ReadOnly)) {
                            stFileTypes = XFormats::getFileTypes(&compressedDevice, true, m_pPdStruct);
                            fileTypePref = XBinary::_getPrefFileType(&stFileTypes);

                            XBinary::FILEFORMATINFO formatInfo = XFormats::getFileFormatInfo(fileTypePref, &compressedDevice, false, -1, m_pPdStruct);

                            record.sExt = formatInfo.sExt;
                            record.fileType = formatInfo.fileType;
                            record.sString = XBinary::getFileFormatString(&formatInfo);
                            record.nCRC = XBinary::_getCRC32(&compressedDevice, m_pPdStruct);

                            if ((fileType == XBinary::FT_APK) || (fileType == XBinary::FT_APKS)) {
                                if ((record.fileType == XBinary::FT_ANDROIDASRC) || (record.fileType == XBinary::FT_ANDROIDXML)) {
                                    record.fileType = XBinary::FT_XML;
                                    record.handleMethod = XBinary::HANDLE_METHOD_ANDROID_XML;
                                }
                            }

                            compressedDevice.close();
                        }
                    }

                    if (m_pData->options.bAllTypes) {
                        bAdd = true;
                    } else {
                        qint32 nNumberOfFileTypes = m_pData->options.listFileTypes.count();

                        for (qint32 j = 0; j < nNumberOfFileTypes; j++) {
                            XBinary::FT _fileType = m_pData->options.listFileTypes.at(j);

                            if (stFileTypes.contains(_fileType)) {
                                bAdd = true;
                                break;
                            }
                        }
                    }
                }

                if (bAdd) {
                    // Fix if more than the device size
                    if ((record.nOffset + record.nSize) > m_pDevice->size()) {
                        record.nSize = (m_pDevice->size() - record.nOffset);
                    }

                    m_pData->listRecords.append(record);
                }
            }

            XBinary::setPdStructCurrentIncrement(m_pPdStruct, nGlobalIndex);
        }

        XBinary::setPdStructFinished(m_pPdStruct, nGlobalIndex);
    }
}

void XExtractor::process()
{
    qint32 _nFreeIndex = XBinary::getFreeIndex(m_pPdStruct);
    XBinary::setPdStructInit(m_pPdStruct, _nFreeIndex, 1);

    bool bInvalidMode = false;

    XBinary::FT fileType = m_pData->options.fileType;

    if (fileType == XBinary::FT_UNKNOWN) {
        QSet<XBinary::FT> stFileTypes = XFormats::getFileTypes(m_pDevice, true);
        fileType = XBinary::_getPrefFileType(&stFileTypes);
    }

    if (m_pData->options.bAnalyze) {
        if (m_pData->options.emode == EMODE_HEURISTIC) {
            if (isFormatModeAvailable(fileType)) {
                handleFormatAndUnpack(fileType, false);
            } else {
                handleRaw();
            }
        } else if (m_pData->options.emode == EMODE_FORMAT) {
            if (isFormatModeAvailable(fileType)) {
                handleFormatAndUnpack(fileType, false);
            } else {
                bInvalidMode = true;
            }
        } else if (m_pData->options.emode == EMODE_RAW) {
            handleRaw();
        } else if (m_pData->options.emode == EMODE_UNPACK) {
            if (isUnpackModeAvailable(fileType)) {
                handleFormatAndUnpack(fileType, true);
            } else {
                bInvalidMode = true;
            }
        }
    }

    if (bInvalidMode) {
#ifdef QT_DEBUG
        qDebug() << "Invalid mode" << XBinary::fileTypeIdToString(fileType) << m_pData->options.emode;
#endif
        emit errorMessage(tr("Mode is not available for this file type"));
    }

    if (m_pData->options.bExtract) {
        if (m_pData->emode == EMODE_UNPACK) {
            XFormats xformats;
            _connect(&xformats);
            QString sOutputDirectory = m_pData->options.sOutputDirectory + QDir::separator() + XBinary::getDeviceFileBaseName(m_pDevice);
            xformats.unpackDeviceToFolder(fileType, m_pDevice, sOutputDirectory, m_pPdStruct);
        } else if ((m_pData->emode == EMODE_FORMAT) || (m_pData->emode == EMODE_RAW)) {
            QList<XBinary::ARCHIVERECORD> listArchiveRecords;

            qint32 nFreeIndex = XBinary::getFreeIndex(m_pPdStruct);
            qint32 nNumberOfRecords = m_pData->listRecords.count();
            XBinary::setPdStructInit(m_pPdStruct, nFreeIndex, nNumberOfRecords);

            for (qint32 i = 0; (i < nNumberOfRecords) && XBinary::isPdStructNotCanceled(m_pPdStruct); i++) {
                QString sName = XBinary::convertFileNameSymbols(m_pData->listRecords.at(i).sName);
                QString sExt = QFileInfo(sName).completeSuffix();

                if (sExt == "") {
                    sExt = m_pData->listRecords.at(i).sExt;
                }

                if (sName != "") {
                    sName += "_";
                }
                sName += XBinary::valueToHex(m_pData->listRecords.at(i).nCRC);

                if (sExt != "") {
                    sName += ".";
                    sName += sExt;
                }

                sName = XBinary::fileTypeIdToFtString(m_pData->listRecords.at(i).fileType) + QDir::separator() + sName;

                XBinary::ARCHIVERECORD archiveRecord = {};
                archiveRecord.mapProperties = m_pData->listRecords.at(i).mapProperties;
                archiveRecord.nStreamOffset = m_pData->listRecords.at(i).nOffset;
                archiveRecord.nStreamSize = m_pData->listRecords.at(i).nSize;
                archiveRecord.mapProperties.insert(XBinary::FPART_PROP_ORIGINALNAME, m_pData->listRecords.at(i).sString);
                // part.sExt = m_pData->listRecords.at(i).sExt;
                // part.fileType = m_pData->listRecords.at(i).fileType;

                listArchiveRecords.append(archiveRecord);
            }

            if (listArchiveRecords.count()) {
                XFormats xformats;
                _connect(&xformats);

                xformats.extractArchiveRecordsToFolder(&listArchiveRecords, m_pDevice, m_pData->options.sOutputDirectory, m_pPdStruct);
            }

            XBinary::setPdStructFinished(m_pPdStruct, nFreeIndex);
        }
    }

    XBinary::setPdStructFinished(m_pPdStruct, _nFreeIndex);
}
