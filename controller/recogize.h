#ifndef RECOGIZE_H
#define RECOGIZE_H

#include <QString>

class Recogize
{
public:
    Recogize();
    QString plateStr;//车牌字符串
    QString plateColor;//车牌颜色
    QString plateNum;//车牌号码
    QString sourcePath;//车牌抓拍原图路径
    QString resultPath;//车牌区域截图存储路径
    bool rec(QString dir,QString filename);//识别函数
};

#endif // RECOGIZE_H
