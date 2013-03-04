#include "mixerdialog.h"
#include "ui_mixerdialog.h"
#include "pers.h"
#include "helpers.h"

MixerDialog::MixerDialog(QWidget *parent, SKYMixData *mixdata, int stickMode, QString * comment) :
    QDialog(parent),
    ui(new Ui::MixerDialog)
{
    ui->setupUi(this);
    md = mixdata;

    this->setWindowTitle(tr("DEST -> CH%1%2").arg(md->destCh/10).arg(md->destCh%10));
    populateSourceCB(ui->sourceCB, stickMode, 0, md->srcRaw);
    ui->sourceCB->removeItem(0);
    ui->weightSB->setValue(md->weight);
    ui->offsetSB->setValue(md->sOffset);
    ui->trimChkB->setChecked(md->carryTrim==0);
    ui->FMtrimChkB->setChecked(md->enableFmTrim);
    populateCurvesCB(ui->curvesCB,md->curve);
    populateSwitchCB(ui->switchesCB,md->swtch);
    ui->warningCB->setCurrentIndex(md->mixWarn);
    ui->mltpxCB->setCurrentIndex(md->mltpx);

    ui->delayDownSB->setValue((double)md->delayDown/10);
    ui->delayUpSB->setValue((double)md->delayUp/10);
    ui->slowDownSB->setValue((double)md->speedDown/10);
    ui->slowUpSB->setValue((double)md->speedUp/10);

    mixCommennt = comment;
    ui->mixerComment->setPlainText(mixCommennt->trimmed());


    valuesChanged();

    connect(ui->sourceCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->weightSB,SIGNAL(editingFinished()),this,SLOT(valuesChanged()));
    connect(ui->offsetSB,SIGNAL(editingFinished()),this,SLOT(valuesChanged()));
    connect(ui->trimChkB,SIGNAL(toggled(bool)),this,SLOT(valuesChanged()));
    connect(ui->curvesCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->switchesCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->warningCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->mltpxCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->delayDownSB,SIGNAL(valueChanged(double)),this,SLOT(valuesChanged()));
    connect(ui->delayUpSB,SIGNAL(valueChanged(double)),this,SLOT(valuesChanged()));
    connect(ui->slowDownSB,SIGNAL(valueChanged(double)),this,SLOT(valuesChanged()));
    connect(ui->slowUpSB,SIGNAL(valueChanged(double)),this,SLOT(valuesChanged()));
    connect(ui->FMtrimChkB,SIGNAL(stateChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->mixerComment,SIGNAL(textChanged()),this,SLOT(valuesChanged()));
}

MixerDialog::~MixerDialog()
{
    delete ui;
}

void MixerDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MixerDialog::valuesChanged()
{
    md->srcRaw       = ui->sourceCB->currentIndex()+1;
    md->weight       = ui->weightSB->value();
    md->sOffset      = ui->offsetSB->value();
    md->carryTrim    = ui->trimChkB->checkState() ? 0 : 1;
    md->curve        = ui->curvesCB->currentIndex();
    md->swtch        = ui->switchesCB->currentIndex()-MAX_DRSWITCH;
    md->mixWarn      = ui->warningCB->currentIndex();
    md->mltpx        = ui->mltpxCB->currentIndex();
    md->delayDown    = ui->delayDownSB->value()*10;
    md->delayUp      = ui->delayUpSB->value()*10;
    md->speedDown    = ui->slowDownSB->value()*10;
    md->speedUp      = ui->slowUpSB->value()*10;
    md->enableFmTrim = ui->FMtrimChkB->checkState() ? 1 : 0;

    if(ui->FMtrimChkB->checkState())
        ui->offset_label->setText("FmTrimVal");
    else
        ui->offset_label->setText("Offset");

    mixCommennt->clear();
    mixCommennt->append(ui->mixerComment->toPlainText());
}
