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

    enum COLUMN {
        COLUMN_NUMBER,
        COLUMN_OFFSET,
        COLUMN_ADDRESS,
        COLUMN_REGION,
        COLUMN_SIZE,
        COLUMN_METHOD,
        COLUMN_TYPE,
        COLUMN_INFO,
        __COLUMN_SIZE
    };

public:
    XModel_Extractor(XExtractor::DATA *pData, QObject *pParent = nullptr);

    virtual QVariant data(const QModelIndex &index, int nRole = Qt::DisplayRole) const;
    virtual QVariant headerData(int nSection, Qt::Orientation orientation, int nRole = Qt::DisplayRole) const;
    virtual SORT_METHOD getSortMethod(qint32 nColumn);

private:
    XExtractor::DATA *g_pData;
    XBinary::MODE g_modeAddress;
    XBinary::MODE g_modeOffset;
};

#endif  // XMODEL_EXTRACTOR_H
