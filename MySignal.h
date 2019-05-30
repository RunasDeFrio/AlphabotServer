#ifndef SIGNAL_H
#define SIGNAL_H
#include <qglobal.h>
namespace MySignal
{
    enum Signal: quint16
    {
        Nothing = 0,
        Message,
        File
    };
}
#endif // SIGNAL_H
