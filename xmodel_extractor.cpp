/* Copyright (c) 2025 hors<horsicq@gmail.com>
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
#include "xmodel_extractor.h"

XModel_Extractor::XModel_Extractor(XExtractor::DATA *pData, QObject *pParent) : XModel(pParent)
{
    m_pData = pData;

    _setRowCount(pData->listRecords.count());

    if (pData->emode == XExtractor::EMODE_FORMAT) {
        _setColumnCount(__COLUMN_FORMAT_SIZE);
    } else if (pData->emode == XExtractor::EMODE_RAW) {
        _setColumnCount(__COLUMN_RAW_SIZE);
    } else {
        _setColumnCount(__COLUMN_GENERIC_SIZE);
    }

    m_modeAddress = XBinary::getWidthModeFromSize_32_64(pData->memoryMap.nModuleAddress + pData->memoryMap.nImageSize);
    m_modeOffset = XBinary::getWidthModeFromSize_32_64(pData->memoryMap.nBinarySize);

    setColumnAlignment(COLUMN_GENERIC_NUMBER, Qt::AlignVCenter | Qt::AlignRight);
    setColumnAlignment(COLUMN_GENERIC_OFFSET, Qt::AlignVCenter | Qt::AlignRight);
    setColumnAlignment(COLUMN_GENERIC_ADDRESS, Qt::AlignVCenter | Qt::AlignRight);
    setColumnAlignment(COLUMN_GENERIC_REGION, Qt::AlignVCenter | Qt::AlignLeft);
    setColumnAlignment(COLUMN_GENERIC_SIZE, Qt::AlignVCenter | Qt::AlignRight);
    setColumnAlignment(COLUMN_GENERIC_METHOD, Qt::AlignVCenter | Qt::AlignLeft);

    if (pData->emode == XExtractor::EMODE_FORMAT) {
        setColumnAlignment(COLUMN_FORMAT_TYPE, Qt::AlignVCenter | Qt::AlignLeft);
        setColumnAlignment(COLUMN_FORMAT_INFO, Qt::AlignVCenter | Qt::AlignLeft);
        setColumnAlignment(COLUMN_FORMAT_NAME, Qt::AlignVCenter | Qt::AlignLeft);
    } else if (pData->emode == XExtractor::EMODE_RAW) {
        setColumnAlignment(COLUMN_RAW_TYPE, Qt::AlignVCenter | Qt::AlignLeft);
        setColumnAlignment(COLUMN_RAW_INFO, Qt::AlignVCenter | Qt::AlignLeft);
    }

    setColumnSymbolSize(COLUMN_GENERIC_NUMBER, QString::number(rowCount()).length());
    setColumnSymbolSize(COLUMN_GENERIC_OFFSET, XBinary::getByteSizeFromWidthMode(m_modeOffset) * 2);
    setColumnSymbolSize(COLUMN_GENERIC_ADDRESS, XBinary::getByteSizeFromWidthMode(m_modeAddress) * 2);
    setColumnSymbolSize(COLUMN_GENERIC_REGION, 1);
    setColumnSymbolSize(COLUMN_GENERIC_SIZE, XBinary::getByteSizeFromWidthMode(m_modeOffset) * 2);
    setColumnSymbolSize(COLUMN_GENERIC_METHOD, 10);

    if (pData->emode == XExtractor::EMODE_FORMAT) {
        setColumnSymbolSize(COLUMN_FORMAT_TYPE, 10);
        setColumnSymbolSize(COLUMN_FORMAT_INFO, 10);
        setColumnSymbolSize(COLUMN_FORMAT_NAME, 10);
    } else if (pData->emode == XExtractor::EMODE_RAW) {
        setColumnSymbolSize(COLUMN_RAW_TYPE, 10);
        setColumnSymbolSize(COLUMN_RAW_INFO, 10);
    }

    qint32 nNumberOfRegions = pData->memoryMap.listRecords.count();
    qint32 nMaxRegionNameLength = 4;

    for (qint32 i = 0; i < nNumberOfRegions; i++) {
        nMaxRegionNameLength = qMax(nMaxRegionNameLength, pData->memoryMap.listRecords.at(i).sName.length());
        nMaxRegionNameLength = qMin(50, nMaxRegionNameLength);
    }

    setColumnSymbolSize(COLUMN_GENERIC_REGION, nMaxRegionNameLength);
}

QVariant XModel_Extractor::data(const QModelIndex &index, int nRole) const
{
    QVariant result;

    if (index.isValid()) {
        qint32 nRow = index.row();

        if ((nRow >= 0) && (m_pData->listRecords.count() > nRow)) {
            qint32 nColumn = index.column();

            if (nRole == Qt::DisplayRole) {
                if (nColumn == COLUMN_GENERIC_NUMBER) {
                    result = nRow;
                } else if (nColumn == COLUMN_GENERIC_OFFSET) {
                    result = XBinary::valueToHex(m_modeOffset, m_pData->listRecords.at(nRow).nOffset);
                } else if (nColumn == COLUMN_GENERIC_ADDRESS) {
                    XADDR nAddress = XBinary::offsetToAddress(&(m_pData->memoryMap), m_pData->listRecords.at(nRow).nOffset);
                    if (nAddress != (XADDR)-1) {
                        result = XBinary::valueToHex(m_modeAddress, nAddress);
                    }
                } else if (nColumn == COLUMN_GENERIC_REGION) {
                    result = XBinary::getMemoryRecordByOffset(&(m_pData->memoryMap), m_pData->listRecords.at(nRow).nOffset).sName;
                } else if (nColumn == COLUMN_GENERIC_SIZE) {
                    result = QString::number(m_pData->listRecords.at(nRow).nSize, 16);
                } else if (nColumn == COLUMN_GENERIC_METHOD) {
                    result = XBinary::compressMethodToString((XBinary::COMPRESS_METHOD)(
                        m_pData->listRecords.at(nRow).mapProperties.value(XBinary::FPART_PROP_COMPRESSMETHOD, XBinary::COMPRESS_METHOD_STORE).toInt()));
                } else if (nColumn >= __COLUMN_GENERIC_SIZE) {
                    if (m_pData->emode == XExtractor::EMODE_FORMAT) {
                        if (nColumn == COLUMN_FORMAT_TYPE) {
                            QString sFileType = XBinary::fileTypeIdToString(m_pData->listRecords.at(nRow).fileType);
                            if (m_pData->listRecords.at(nRow).handleMethod != XBinary::HANDLE_METHOD_UNKNOWN)
                                sFileType += " / " + XBinary::handleMethodToString(m_pData->listRecords.at(nRow).handleMethod);
                            result = sFileType;
                        } else if (nColumn == COLUMN_FORMAT_INFO) {
                            result = m_pData->listRecords.at(nRow).sString;
                        } else if (nColumn == COLUMN_FORMAT_NAME) {
                            result = m_pData->listRecords.at(nRow).sName;
                        }
                    } else if (m_pData->emode == XExtractor::EMODE_RAW) {
                        if (nColumn == COLUMN_RAW_TYPE) {
                            QString sFileType = XBinary::fileTypeIdToString(m_pData->listRecords.at(nRow).fileType);
                            if (m_pData->listRecords.at(nRow).handleMethod != XBinary::HANDLE_METHOD_UNKNOWN)
                                sFileType += " / " + XBinary::handleMethodToString(m_pData->listRecords.at(nRow).handleMethod);
                            result = sFileType;
                        } else if (nColumn == COLUMN_RAW_INFO) {
                            result = m_pData->listRecords.at(nRow).sString;
                        }
                    }
                }
            } else if (nRole == Qt::TextAlignmentRole) {
                result = getColumnAlignment(nColumn);
            } else if (nRole == Qt::UserRole + USERROLE_ORIGINDEX) {
                result = nRow;
            } else if (nRole == Qt::UserRole + USERROLE_ADDRESS) {
                result = XBinary::offsetToAddress(&(m_pData->memoryMap), m_pData->listRecords.at(nRow).nOffset);
            } else if (nRole == Qt::UserRole + USERROLE_OFFSET) {
                result = m_pData->listRecords.at(nRow).nOffset;
            } else if (nRole == Qt::UserRole + USERROLE_SIZE) {
                result = m_pData->listRecords.at(nRow).nSize;
            } else if (nRole == Qt::UserRole + USERROLE_STRING1) {
                result = m_pData->listRecords.at(nRow).sString;
            } else if (nRole == Qt::UserRole + USERROLE_STRING2) {
                result = m_pData->listRecords.at(nRow).sExt;
            }
        }
    }

    return result;
}

QVariant XModel_Extractor::headerData(int nSection, Qt::Orientation orientation, int nRole) const
{
    QVariant result;

    if (orientation == Qt::Horizontal) {
        if (nRole == Qt::DisplayRole) {
            if (nSection == COLUMN_GENERIC_NUMBER) {
                result = "#";
            } else if (nSection == COLUMN_GENERIC_OFFSET) {
                result = tr("Offset");
            } else if (nSection == COLUMN_GENERIC_ADDRESS) {
                result = tr("Address");
            } else if (nSection == COLUMN_GENERIC_REGION) {
                result = tr("Region");
            } else if (nSection == COLUMN_GENERIC_SIZE) {
                result = tr("Size");
            } else if (nSection == COLUMN_GENERIC_METHOD) {
                result = tr("Method");
            } else if (nSection >= __COLUMN_GENERIC_SIZE) {
                if (m_pData->emode == XExtractor::EMODE_FORMAT) {
                    if (nSection == COLUMN_FORMAT_TYPE) {
                        result = tr("Type");
                    } else if (nSection == COLUMN_FORMAT_INFO) {
                        result = tr("Info");
                    } else if (nSection == COLUMN_FORMAT_NAME) {
                        result = tr("Name");
                    }
                } else if (m_pData->emode == XExtractor::EMODE_RAW) {
                    if (nSection == COLUMN_RAW_TYPE) {
                        result = tr("Type");
                    } else if (nSection == COLUMN_RAW_INFO) {
                        result = tr("Info");
                    }
                }
            }
        } else if (nRole == Qt::TextAlignmentRole) {
            result = getColumnAlignment(nSection);
        }
    }

    return result;
}

XModel::SORT_METHOD XModel_Extractor::getSortMethod(qint32 nColumn)
{
    SORT_METHOD result = SORT_METHOD_DEFAULT;

    if ((nColumn == COLUMN_GENERIC_OFFSET) || (nColumn == COLUMN_GENERIC_OFFSET) || (nColumn == COLUMN_GENERIC_SIZE)) {
        result = SORT_METHOD_HEX;
    }

    return result;
}
