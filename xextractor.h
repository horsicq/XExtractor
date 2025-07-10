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
#ifndef XEXTRACTOR_H
#define XEXTRACTOR_H

#include "xformats.h"
#include "xthreadobject.h"
#include <QAbstractItemModel>

class XExtractor : public XThreadObject {
    Q_OBJECT

public:
    enum EMODE {
        EMODE_UNKNOWN = 0,
        EMODE_RAW,
        EMODE_FORMAT,
        EMODE_HEURISTIC
    };

    struct OPTIONS {
        XBinary::FT fileType;
        // XBinary::MAPMODE mapMode;
        QList<XBinary::FT> listFileTypes;
        qint32 nLimit;
        bool bDeepScan;
        EMODE emode;  // RAW, FORMAT, HEURISTIC
        // bool bHeuristicScan;
        bool bAnalyze;
        bool bExtract;
        bool bMenu_Hex;
        QString sOutputDirectory;
        bool bShowList;
    };

    struct RECORD {
        qint64 nOffset;
        qint64 nSize;
        XBinary::FT fileType;
        QString sString;
        QString sExt;
    };

    struct DATA {
        OPTIONS options;
        XBinary::_MEMORY_MAP memoryMap;
        QVector<RECORD> listRecords;
    };

    XExtractor(QObject *pParent = nullptr);

    void setData(QIODevice *pDevice, DATA *pData, XBinary::PDSTRUCT *pPdStruct);
    bool processFile(const QString &sFileName, DATA *pData, XBinary::PDSTRUCT *pPdStruct);

    static QList<XBinary::FT> getAvailableFileTypes();
    static XExtractor::OPTIONS getDefaultOptions();
    static QVector<RECORD> scanDevice(QIODevice *pDevice, OPTIONS options, XBinary::PDSTRUCT *pPdStruct);

    static QAbstractItemModel *createModelFromRecords(DATA *pData);

    static QString extractorModeToString(EMODE mode);
    static EMODE ftStringToExtractorMode(QString sString);

    static bool isFormatModeAvailable(XBinary::FT fileType);

private:
    void handleSearch(XBinary *pBinary, XBinary::_MEMORY_MAP *pMemoryMap, XBinary::FT fileType, const QString &sSignature, qint32 nDelta, QVariant varExtra = QVariant(),
                      const QString &sInfo1 = "", const QString &sInfo2 = "");
    void handleRaw();
    void handleFormat();
public slots:
    void process();

private:
    QIODevice *g_pDevice;
    DATA *g_pData;
    XBinary::PDSTRUCT *g_pPdStruct;
    qint32 g_nFreeIndex;
};

#endif  // XEXTRACTOR_H
