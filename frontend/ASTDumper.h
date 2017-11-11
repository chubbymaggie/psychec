/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** Modifications (apply on the LGPL usage):
** Copyright (c) 2016,17 Leandro T. C. Melo (ltcmelo@gmail.com)
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

/////////////////////////////////////////////////////////////////////
/////              Note about copyright/licesing                /////
/////                                                           /////
///// Psyche reuses parts of the C++ parser of the Qt Creator   /////
///// IDE, as explained in 3rdParty.txt. Those sources that we  /////
///// have taken are copyrighed by Roberto Raggi and licensed   /////
///// under BSD (they live in Qt Creator's own 3rd party dir).  /////
///// The only exception to this rule is this AST dumper, which /////
///// is/was copyrighted by NOKIA/Digia/QtCompany and licensed  /////
///// under LGPL (applicable for open-source projects). Still,  /////
///// I have modified it to make it Qt-agnostic.                /////
/////                                                           /////
/////////////////////////////////////////////////////////////////////

#ifndef PSYCHE_ASTDUMPER_H__
#define PSYCHE_ASTDUMPER_H__

#include "FrontendConfig.h"
#include "AST.h"
#include "ASTVisitor.h"
#include <fstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace psyche {

/*!
 * \brief The Dumper class
 *
 * This is an adaptation of Qt Creator's ASTDump class.
 */
class CFE_API ASTDumper: protected psyche::ASTVisitor
{
public:
    ASTDumper(psyche::TranslationUnit *unit)
        : ASTVisitor(unit)
        , os_(nullptr)
        , count_(1)
    {}

    void dump(psyche::AST *ast, const std::string& fileSuffix);
    void dump(psyche::AST *ast, const std::string& fileSuffix, std::ostream& os);

    // AST node dumpers are generated by $cplusplus-update-frontend <frontend-dir> <dumpers-file>
    // in Qt Creator, but we cannot introduce all that infrastructure into our project. So
    // when we add/change AST nodes we must make the necessary adjustments to this file.
    #include "ASTNodeDumpers.inc"

protected:
    void alignTerminals();

    static std::string name(AST *ast);

    std::string terminalId(unsigned token);

    void terminal(unsigned token, AST *node);

    void generateTokens();

    void nonterminal(AST *ast);
    void node(AST *ast);

    bool preVisit(AST *ast) override;
    void postVisit(AST *) override;

private:
    std::unordered_map<psyche::AST *, std::string> id_;
    std::vector<std::pair<std::string, std::string>> connections_;
    std::stack<psyche::AST *> nodes_;
    std::vector<std::string> terminalShapes_;
    std::ostream* os_;
    int count_;
};

}

#endif
