#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <locale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec* codec = QTextCodec::codecForName("cp1251");
    QTextCodec::setCodecForLocale(codec);

    setlocale(LC_ALL, "Russian");
    QLockFile lock(QDir::temp().absoluteFilePath("Sbor.loc"));
    if (!lock.tryLock(100))
    {
      QMessageBox::critical(0,"Ошибка","Уже есть запущенный экземпляр программы.",QMessageBox::Yes);
      return 1;
    }

    a.setQuitOnLastWindowClosed(true);

    MainWindow w;
    w.show();

    return a.exec();
}
