#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLockFile lock(QDir::temp().absoluteFilePath("AutoSbor.loc"));

    if (!lock.tryLock(100))
    {
        QMessageBox::critical(0,"Ошибка","Уже есть запущенный экземпляр программы.",QMessageBox::Yes);
        return 1;
    }

    MainWindow w;
    w.name.clear();
    w.pas.clear();
    w.hst.clear();

    if (argc > 1)
    {
        w.name=QString::fromLocal8Bit(argv[1]);
        w.pas=QString::fromLocal8Bit(argv[2]);
        w.hst=QString::fromLocal8Bit(argv[3]);
    }

    w.show();

    return a.exec();
}
