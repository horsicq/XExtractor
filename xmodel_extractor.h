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
#ifndef XMODEL_EXTRACTOR_H
#define XMODEL_EXTRACTOR_H

#include "xmodel.h"
#include "xextractor.h"

class XModel_Extractor : public XModel {
    Q_OBJECT

    enum COLUMN_GENERIC {
        COLUMN_GENERIC_NUMBER,
        COLUMN_GENERIC_OFFSET,
        COLUMN_GENERIC_ADDRESS,
        COLUMN_GENERIC_REGION,
        COLUMN_GENERIC_SIZE,
        COLUMN_GENERIC_METHOD,
        __COLUMN_GENERIC_SIZE
    };

    enum COLUMN_RAW {
        COLUMN_RAW_TYPE = __COLUMN_GENERIC_SIZE,
        COLUMN_RAW_INFO,
        __COLUMN_RAW_SIZE
    };

    enum COLUMN_FORMAT {
        COLUMN_FORMAT_TYPE = __COLUMN_GENERIC_SIZE,
        COLUMN_FORMAT_INFO,
        COLUMN_FORMAT_NAME,
        __COLUMN_FORMAT_SIZE
    };

    enum COLUMN_UNPACK {
        COLUMN_UNPACK_NAME = __COLUMN_GENERIC_SIZE,
        __COLUMN_UNPACK_SIZE
    };

public:
    XModel_Extractor(XExtractor::DATA *pData, QObject *pParent = nullptr);

    virtual QVariant data(const QModelIndex &index, int nRole = Qt::DisplayRole) const;
    virtual QVariant headerData(int nSection, Qt::Orientation orientation, int nRole = Qt::DisplayRole) const;
    virtual SORT_METHOD getSortMethod(qint32 nColumn);

private:
    XExtractor::DATA *m_pData;
    XBinary::MODE m_modeAddress;
    XBinary::MODE m_modeOffset;
};

#endif  // XMODEL_EXTRACTOR_H
