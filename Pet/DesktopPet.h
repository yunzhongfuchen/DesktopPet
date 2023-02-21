#pragma once

#include <QtWidgets/QWidget>
#include "../Pet/TablePat.h"
#include "../Pet/Action.h"
//#include "ui_DesktopPet.h"

class DesktopPet : public QWidget
{
    Q_OBJECT

public:
    DesktopPet(QWidget *parent = nullptr);
    ~DesktopPet();

private:
    //Ui::DesktopPetClass ui;
};
