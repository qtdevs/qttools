/****************************************************************************
**
** Copyright (C) 2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Volker Krause <volker.krause@kdab.com>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the tools applications of the Qt Toolkit
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QLibrary>
#include <QPluginLoader>

#include <iostream>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("qplugininfo"));
    QCoreApplication::setApplicationVersion(QStringLiteral(QT_VERSION_STR));

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Qt5 plugin meta-data dumper"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QStringLiteral("plugin"), QStringLiteral("Plug-in of which to read the meta data."), QStringLiteral("<plugin>"));
    parser.process(app);

    if (parser.positionalArguments().isEmpty())
        parser.showHelp(1);

    foreach (const QString &plugin, parser.positionalArguments()) {
        QByteArray pluginNativeName = QFile::encodeName(QDir::toNativeSeparators(plugin));
        if (!QFile::exists(plugin)) {
            std::cerr << "qtplugininfo: " << pluginNativeName.constData() << ": No such file or directory." << std::endl;
            return 1;
        }
        if (!QLibrary::isLibrary(plugin)) {
            std::cerr << "qtplugininfo: " << pluginNativeName.constData() << ": Not a plug-in." << std::endl;
            return 1;
        }
        QPluginLoader loader(plugin);
        if (loader.metaData().isEmpty()) {
            std::cerr << "qtplugininfo: " << pluginNativeName.constData() << ": No plug-in meta-data found: "
                      << qPrintable(loader.errorString()) << std::endl;
            return 1;
        }
        const QJsonDocument doc(loader.metaData());
        std::cout << doc.toJson().constData();
    }

    return 0;
}