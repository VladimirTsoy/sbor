#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLockFile lock(QDir::temp().absoluteFilePath("AutoSort.loc"));

    if (!lock.tryLock(100))
    {
      QMessageBox::critical(0,"Ошибка","Уже есть запущенный экземпляр программы.",QMessageBox::Yes);
      return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
