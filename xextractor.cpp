/* Copyright (c) 2022-2024 hors<horsicq@gmail.com>
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
    listResult.append(XBinary::FT_CUR);
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
    // listResult.append(XBinary::FT_SIGNATURE); // TODO

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
    // result.listFileTypes.append(XBinary::FT_SIGNATURE);

    result.bDeepScan = true;
    result.bHeuristicScan = true;

    return result;
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

void XExtractor::handleSearch(XBinary *pBinary, XBinary::_MEMORY_MAP *pMemoryMap, XBinary::FT fileType, const QString &sSignature, qint32 nDelta, QVariant varExtra,
                              const QString &sInfo, const QString &sInfo2)
{
    Q_UNUSED(varExtra)
    Q_UNUSED(sInfo)
    Q_UNUSED(sInfo2)

    if (g_pData->options.listFileTypes.contains(fileType)) {
        XBinary::setPdStructStatus(g_pPdStruct, g_nFreeIndex, XBinary::fileTypeIdToString(fileType));

        qint64 nOffset = 0;

        qint32 _nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
        XBinary::setPdStructInit(g_pPdStruct, _nFreeIndex, pBinary->getSize());

        qint32 nFound = 0;

        while (!(g_pPdStruct->bIsStop)) {
            nOffset = pBinary->find_signature(pMemoryMap, nOffset, -1, sSignature, nullptr, g_pPdStruct);

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

                    SubDevice subdevice(g_pDevice, _nOffset, -1);

                    if (subdevice.open(QIODevice::ReadOnly)) {
                        XBinary::FILEFORMATINFO formatInfo = XFormats::getFileFormatInfo(fileType, &subdevice, false, -1, g_pPdStruct);

                        if (formatInfo.bIsValid) {
                            if (g_pData->options.bHeuristicScan) {
                                QSet<XBinary::FT> stFT = XFormats::getFileTypes(&subdevice, true, g_pPdStruct);
                                XBinary::FT _fileType = XBinary::_getPrefFileType(&stFT);

                                XBinary::FILEFORMATINFO _formatInfo = XFormats::getFileFormatInfo(_fileType, &subdevice, false, -1, g_pPdStruct);

                                if (_formatInfo.bIsValid) {
                                    formatInfo = _formatInfo;
                                }
                            }

                            RECORD record = {};

                            record.nOffset = _nOffset;
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

                                nFound++;
                            }

                            nResSize = record.nSize;
                        }

                        subdevice.close();
                    }

                    if (nResSize == 0) {
                        nResSize = 1;
                    }

                    //    if ((g_pData->options.bDeepScan) && (fileType != XBinary::FT_ZIP)) {
                    //        nResult = 1;
                    //    }

                    if (g_pData->options.bDeepScan) {
                        nResSize = 1;
                    }

                    nOffset += nResSize;
                }

            } else {
                break;
            }

            if ((g_pData->options.nLimit > 0) && (nFound >= g_pData->options.nLimit)) {
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

    qint32 nSearchCount = g_pData->options.listFileTypes.count();

    if (g_pData->options.listFileTypes.contains(XBinary::FT_ICO)) {
        nSearchCount++;
    }

    if (g_pData->options.listFileTypes.contains(XBinary::FT_MACHO)) {
        nSearchCount += 3;
    }

    if (g_pData->options.listFileTypes.contains(XBinary::FT_MACHOFAT)) {
        nSearchCount++;
    }

    if (g_pData->options.listFileTypes.contains(XBinary::FT_TIFF)) {
        nSearchCount++;
    }

    // if (g_pData->options.listFileTypes.contains(XBinary::FT_RIFF)) {
    //     nSearchCount += 2;
    // }

    if (g_pData->options.listFileTypes.contains(XBinary::FT_AMIGAHUNK)) {
        nSearchCount += 2;
    }

    // TODO signatures

    g_nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
    XBinary::setPdStructInit(g_pPdStruct, g_nFreeIndex, nSearchCount);

    XBinary binary(g_pDevice);

    XBinary::_MEMORY_MAP memoryMap = binary.getMemoryMap(g_pData->options.mapMode, g_pPdStruct);

    connect(&binary, SIGNAL(errorMessage(QString)), this, SIGNAL(errorMessage(QString)));

    handleSearch(&binary, &memoryMap, XBinary::FT_PE, "'MZ'", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_ELF, "7F'ELF'", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_7Z, "'7z'BCAF271C", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_ZIP, "'PK'0304", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_RAR, "'Rar!'1A07", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_GZIP, "1F8B08", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_ZLIB, "785E", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_ZLIB, "789C", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_ZLIB, "78DA", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_DEX, "'dex\n'", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_PDF, "'%PDF'", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_PNG, "89'PNG\r\n'1A0A", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_JPEG, "FFD8FF", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_CAB, "'MSCF'", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_ICO, "00000100", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_CUR, "00000200", 0);  // CUR
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHO, "FEEDFACE", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHO, "CEFAEDFE", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHO, "FEEDFACF", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHO, "CFFAEDFE", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHOFAT, "CAFEBABE", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_MACHOFAT, "BEBAFECA", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_BMP, "'BM'", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_GIF, "'GIF8'", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_TIFF, "'MM'002A", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_TIFF, "'II'2A00", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_MP3, "'ID3'", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_MP4, "'ftyp'", -4);  // 000000..'ftyp'
    handleSearch(&binary, &memoryMap, XBinary::FT_RIFF, "'RIFF'", 0);
    // handleSearch(&binary, &memoryMap, XBinary::FT_RIFF, "'RIFX'", 0);
    // handleSearch(&binary, &memoryMap, XBinary::FT_RIFF, "'AIFF'", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_NE, "'MZ'", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_LE, "'MZ'", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_AMIGAHUNK, "000003F3", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_AMIGAHUNK, "000003E7", 0);
    handleSearch(&binary, &memoryMap, XBinary::FT_JAVACLASS, "CAFEBABE", 0);

    // TODO LE/BE
    handleSearch(&binary, &memoryMap, XBinary::FT_SIGNATURE, "00000000", -4, 0, "CRC32", "Test");
    // TODO more

    XBinary::setPdStructFinished(g_pPdStruct, g_nFreeIndex);

    emit completed(scanTimer.elapsed());
}
