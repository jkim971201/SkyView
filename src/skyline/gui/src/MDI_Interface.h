#ifndef MDI_INTERFACE_H 
#define MDI_INTERFACE_H 

#include <QMainWindow>
#include <QObject>

namespace gui
{

class MDI_Interface : public QMainWindow
{
    Q_OBJECT
public:
    explicit MDI_Interface(QWidget *parent = nullptr);
};

}

#endif // QMDIMAINWINDOW_H
