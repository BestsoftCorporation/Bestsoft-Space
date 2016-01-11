#include "highlighter.h"
#include <QMessageBox>
Highlighter::Highlighter(QTextDocument *parent,QString language)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;



    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\#[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::darkCyan);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");

    if (language=="C"){
     keywordFormat.setForeground(QColor(0,51,255));
     keywordFormat.setFontWeight(QFont::Bold);
     QStringList keywordPatterns;

     keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                     << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                     << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                     << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                     << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                     << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                     << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                     << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                     << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                     << "\\bvoid\\b" << "\\bvolatile\\b"<<"if"<<"else"<<"for" << "ret"<<"return";
     foreach (const QString &pattern, keywordPatterns) {
         rule.pattern = QRegExp(pattern);
         rule.format = keywordFormat;
         highlightingRules.append(rule);
     }
   }
    if (language=="ASM"){
     keywordFormat.setForeground(QColor(0,51,255));
     keywordFormat.setFontWeight(QFont::Bold);
     QStringList keywordPatterns;

     keywordPatterns << "\\bmov\\b" << "\\bMOV\\b" << "\\bsub\\b" << "\\bSUB\\b" << "\\badd\\b"
                     << "\\bADD\\b" << "\\bBITS\\b" << "\\bjmp\\b" << "\\bjl\\b" << "\\bjg\\b"
                     << "\\bcmp\\b" << "RET" << "ret" ;
     foreach (const QString &pattern, keywordPatterns) {
         rule.pattern = QRegExp(pattern);
         rule.format = keywordFormat;
         highlightingRules.append(rule);
     }
   }
    if (language=="DASH"){
     keywordFormat.setForeground(QColor(0,51,255));
     keywordFormat.setFontWeight(QFont::Bold);
     QStringList keywordPatterns;

     keywordPatterns << "\\bif\\b" << "Start_Sub" << "End_Start_Sub"
                     << "Import" << "Dash" << "Bestsoft" << "Class" << "Main"
                     << "End_Class" << "kernel" <<"Now";
     foreach (const QString &pattern, keywordPatterns) {
         rule.pattern = QRegExp(pattern);
         rule.format = keywordFormat;
         highlightingRules.append(rule);
     }
   }
}


void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);

        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}

