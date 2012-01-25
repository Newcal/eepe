#ifndef HELPERS_H
#define HELPERS_H

#include <QtGui>
#include <QtXml>
#include "pers.h"

#define TMR_NUM_OPTION  (TMR_VAROFS+2*MAX_DRSWITCH-3)


void populateSwitchCB(QComboBox *b, int value);
void populateCurvesCB(QComboBox *b, int value);
void populateTimerSwitchCB(QComboBox *b, int value);
void populateSourceCB(QComboBox *b, int stickMode=1, int value=0);
void populateCSWCB(QComboBox *b, int value);
QString getSourceStr(int stickMode, int idx);
QString getTimerMode(int tm);
QString getSWName(int val);
QString getCSWFunc(int val);

int  loadiHEX(QWidget *parent, QString fileName, quint8 * data, int datalen, QString header);
bool saveiHEX(QWidget *parent, QString fileName, quint8 * data, int datalen, QString header, int notesIndex=0);


QDomElement getGeneralDataXML(QDomDocument * qdoc, EEGeneral * tgen);   //parse out data to XML format
QDomElement getModelDataXML(QDomDocument * qdoc, ModelData * tmod, int modelNum); //parse out data to XML format

bool loadGeneralDataXML(QDomDocument * qdoc, EEGeneral * tgen); // get data from XML
bool loadModelDataXML(QDomDocument * qdoc, ModelData * tmod); // get data from XML

#endif // HELPERS_H
