/* Copyright (c) 2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
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

XExtractor::XExtractor(QObject *pParent): QObject(pParent)
{
    g_pDevice=nullptr;
    g_pData=nullptr;
    g_pPdStruct=nullptr;
}

void XExtractor::setData(QIODevice *pDevice,DATA *pData,XBinary::PDSTRUCT *pPdStruct)
{
    g_pDevice=pDevice;
    g_pData=pData;
    g_pPdStruct=pPdStruct;
}

void XExtractor::process()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    g_pData->listRecords.clear();

    qint32 _nFreeIndex=XBinary::getFreeIndex(g_pPdStruct);
    XBinary::setPdStructInit(g_pPdStruct,_nFreeIndex,g_pData->options.fileTypes.count());

    XBinary binary(g_pDevice);

    XBinary::_MEMORY_MAP memoryMap=binary.getMemoryMap();

    connect(&binary,SIGNAL(errorMessage(QString)),this,SIGNAL(errorMessage(QString)));

    if(g_pData->options.fileTypes.contains(XBinary::FT_7Z))
    {
        qint64 nOffset=0;

        while(!(g_pPdStruct->bIsStop))
        {
            nOffset=binary.find_signature(&memoryMap,nOffset,-1,"'7z'BCAF271C",nullptr,g_pPdStruct);

            if(nOffset!=-1)
            {
                SubDevice subevice(g_pDevice,nOffset,-1);

                if(subevice.open(QIODevice::ReadOnly))
                {
                    XSevenZip sevenZip(&subevice);

                    if(sevenZip.isValid())
                    {
                        qint64 nFileFormatSize=sevenZip.getFileFormatSize();

                        if(nFileFormatSize)
                        {
                            RECORD record={};

                            record.nOffset=nOffset;
                            record.nSize=nFileFormatSize;
                            record.fileType=XBinary::FT_PDF;

                            g_pData->listRecords.append(record);
                        }
                    }

                    subevice.close();
                }

                nOffset++;
            }
            else
            {
                break;
            }
        }

        XBinary::setPdStructCurrentIncrement(g_pPdStruct,_nFreeIndex);
    }

    // TODO more

    XBinary::setPdStructFinished(g_pPdStruct,_nFreeIndex);

    emit completed(scanTimer.elapsed());
}
