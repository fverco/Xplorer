#include "src/ui/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

/*!
 * \brief The starting function of the application.
 * \param argc = The amount of arguments passed to the application.
 * \param argv = A list of the arguments.
 * \return An integer that states if the application successfully closed or not.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Xplorer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
