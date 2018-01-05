/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the tools applications of the Qt Toolkit.
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

#ifndef CLANGCODEPARSER_H
#define CLANGCODEPARSER_H

#include <QTemporaryDir>

#include "cppcodeparser.h"

QT_BEGIN_NAMESPACE

class ClangCodeParser : public CppCodeParser
{
    Q_DECLARE_TR_FUNCTIONS(QDoc::ClangCodeParser)

public:
    ~ClangCodeParser();

    void initializeParser(const Config& config) override;
    void terminateParser() override;
    QString language() override;
    QStringList headerFileNameFilter() override;
    QStringList sourceFileNameFilter() override;
    void parseHeaderFile(const Location& location, const QString& filePath) override;
    void parseSourceFile(const Location& location, const QString& filePath) override;
    void precompileHeaders() override;
    Node *parseFnArg(const Location &location, const QString &fnArg) override;

 private:
    void getDefaultArgs();
    void getMoreArgs();
    void buildPCH();

private:
    QSet<QString> allHeaders_;
    QVector<QByteArray> includePaths_;
    QScopedPointer<QTemporaryDir> pchFileDir_;
    QByteArray pchName_;
    QVector<QByteArray> defines_;
    std::vector<const char *> args_;
    QVector<QByteArray> moreArgs_;
};

QT_END_NAMESPACE

#endif
