#ifndef PAGEPARAM_H
#define PAGEPARAM_H

struct PageSetting
{
    bool reqBSF;
    int valBSF;
    bool reqBackg;
    int valBackg;

    PageSetting() {
        reqBSF = true;
        valBSF = 2;
        reqBackg = false;
        valBackg = 8;
    }
};

#endif // PAGEPARAM_H
