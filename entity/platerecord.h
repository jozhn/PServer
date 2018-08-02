#ifndef PLATERECORD_H
#define PLATERECORD_H

#include <QString>

class PlateRecord
{
public:
    PlateRecord();
    QString fileName;
    QString plateColor;
    QString plateNum;
    int type;
    int points;
    int fine;
    QString location;
};

#endif // PLATERECORD_H
