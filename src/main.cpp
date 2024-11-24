/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file main.cpp
 * @brief This file is main of the project.
 *
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QStyleFactory>
#include <string>

#include "./core/configmanager.h"
#include "./ui/mainwindow.h"
#include "version.h"

int main(int argc, char *argv[])
{
    QApplication App(argc, argv);

    App.setOrganizationName(ORGANIZATION_NAME);
    App.setApplicationName(APPLICATION_NAME);
    App.setApplicationVersion(APPLICATION_VERSION);

    /* Parse command line, such as -h, -v*/
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", "The source file to open.");
    /*name, description, default*/
    QCommandLineOption configFileOption("c", "specify configuration file.",
                                        "config.json");
    /*Add extra options*/
    parser.addOption(configFileOption);
    /*Equivalent to QCommandLineParser::process(app.arguments());*/
    parser.process(App);

    /*Get the value of options*/
    QString configFile = parser.value(configFileOption);
    if (configFile.isEmpty()) {
#ifdef Q_OS_WIN // Windows

        /*
        if (QLocale::system().country() == QLocale::China ||
            QLocale::system().language() == QLocale::Chinese)
            App.setFont(QFont("Microsoft Yahei", 9, QFont::Normal, false));
        else */
        App.setFont(QFont("Segoe UI", 9, QFont::Normal, false));

        if (QDir(App.applicationDirPath() + "/config").exists()) {
            configFile = App.applicationDirPath() + "/config/notepanda.json";
        } else {
            QDir configDir = QStandardPaths::writableLocation(
                QStandardPaths::StandardLocation::AppConfigLocation);
            configFile = configDir.absolutePath() + "/config.json";
        }
#else   // Linux
        QDir configDir = QDir::homePath() + "/.config/notepanda";
        configFile = configDir.absolutePath() + "/config.json";
        if (!configDir.exists()) {
            configDir.mkpath(configDir.absolutePath());
        }
#endif
    }
    qDebug() << configFile;

    /*TODO: singleton for ConfigManager*/
    ConfigManager *configManager;
    configManager = new ConfigManager(configFile);

    /*Set style with QT default themes*/
    App.setStyle(QStyleFactory::create(configManager->getStyleTheme()));

    /*Create Main windows*/
    MainWindow notepanda(configManager);
    notepanda.show();

    /*Open file if source file is given*/
    if (parser.positionalArguments().size() == 1)
        notepanda.plainTextEdit->openFile(parser.positionalArguments().at(0));

    qInfo() << QStringLiteral("Welcome to Notepanda!") << "";

    return App.exec();
}
