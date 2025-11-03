#include <QList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "markdowntohtmlconvertor.h"

const QString& MarkdownToHtmlConvertor::convert() {
	m_htmlText.clear();
	m_isParagraphOpen = true;
	m_curUlLevel = 0;
	m_curOlLevel = 0;
	auto lst = m_markdownText.split('\n');
	for(int i = 0; i != lst.size(); ++i) {
		auto line = lst[i];
		m_nSpace = 0;
	  	while( m_nSpace < line.size() && line[m_nSpace] == ' ' ) ++m_nSpace;
	  	if( m_nSpace != 0 ) line = line.mid(m_nSpace);
		if( line.isEmpty() ) {
			m_isParagraphOpen = true;
        } else if( line.startsWith('#') ) {
			do_heading(line);
		} else if( line.startsWith("- ") ) {
			do_list(line);
		} else if( line.startsWith("1. ") ) {
			do_olist(line);
		} else {
			do_paragraph(line);
		}
	}
	close_ul();
	close_ol();
	return m_htmlText;
}
void MarkdownToHtmlConvertor::open_ul(int lvl) {
	while( m_curUlLevel < lvl ) {
		m_htmlText += "<ul>\n";
		++m_curUlLevel;
	}
}
void MarkdownToHtmlConvertor::close_ul(int lvl) {
	while( m_curUlLevel > lvl) {
		m_htmlText += "</ul>\n";
		--m_curUlLevel;
	}
}
void MarkdownToHtmlConvertor::open_ol(int lvl) {
	while( m_curOlLevel < lvl ) {
		m_htmlText += "<ol>\n";
		++m_curOlLevel;
	}
}
void MarkdownToHtmlConvertor::close_ol(int lvl) {
	while( m_curOlLevel > lvl) {
		m_htmlText += "</ol>\n";
		--m_curOlLevel;
	}
}
QString MarkdownToHtmlConvertor::parceInline(const QString& line) {
	QString result = line;

	//QRegularExpression italicRe("\\*(.+?)\\*");
	//result.replace(italicRe, "<em>\\1</em>");

    static QRegularExpression re_star2(R"((?<![\\])\*\*)");  // 直前が \ でない ** をマッチ
	QRegularExpressionMatch match = re_star2.match(result);
	while (match.hasMatch()) {
        int s = match.capturedStart();  // 最初のマッチ位置
        match = re_star2.match(result, s+2);
		if( !match.hasMatch() ) break;
        int e = match.capturedStart();  // ２番目のマッチ位置 
		result = result.left(s) + "<b>" + result.mid(s+2, e - s - 2) + "</b>" + result.mid(e+2);
	}
    static QRegularExpression re_star(R"((?<![\\])\*)");  // 直前が \ でない * をマッチ
	match = re_star.match(result);
	while (match.hasMatch()) {
        int s = match.capturedStart();  // 最初のマッチ位置 
		match = re_star.match(result, s+1);
		if( !match.hasMatch() ) break;
        int e = match.capturedStart();  // ２番目のマッチ位置 
        if( e == s + 1 ) break;
		result = result.left(s) + "<i>" + result.mid(s+1, e - s - 1) + "</i>" + result.mid(e+1);
	}
    result.replace("\\*", "*");

	return result;
}
void MarkdownToHtmlConvertor::do_heading(const QString& line) {
	close_ul();
	close_ol();
	int i = 1;
	while( i < line.size() && line[i] == '#' ) ++i;
	int h = i;
	while( i < line.size() && line[i] == ' ' ) ++i;
	QString t = line.mid(i);
	m_htmlText += QString("<h%1>").arg(h) + t + QString("</h%1>\n").arg(h);
	m_isParagraphOpen = true;
}
void MarkdownToHtmlConvertor::do_list(const QString& line) {
    const int lvl = m_nSpace/2 + 1;
    close_ul(lvl);
    open_ul(lvl);
	m_htmlText += "<li>" + line.mid(2) + "\n";
}
void MarkdownToHtmlConvertor::do_olist(const QString& line) {
    const int lvl = m_nSpace/2 + 1;
    close_ol(lvl);
    open_ol(lvl);
	m_htmlText += "<li>" + line.mid(2) + "\n";
}
void MarkdownToHtmlConvertor::do_paragraph(const QString& line) {
	close_ul();
	close_ol();
	if( m_isParagraphOpen ) {
		m_htmlText += "<p>";
		m_isParagraphOpen = false;
	}
	m_htmlText += parceInline(line) + "\n";
}
