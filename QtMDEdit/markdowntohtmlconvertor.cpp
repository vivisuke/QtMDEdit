#include <QList>
#include "markdowntohtmlconvertor.h"

const QString& MarkdownToHtmlConvertor::convert() {
	m_htmlText.clear();
	m_isParagraphOpen = true;
	m_currentUlLevel = 0;
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
		} else {
			do_paragraph(line);
		}
	}
	close_ul();
	return m_htmlText;
}
void MarkdownToHtmlConvertor::close_ul(int lvl) {
	while( m_currentUlLevel > lvl) {
		m_htmlText += "</ul>\n";
		--m_currentUlLevel;
	}
}
void MarkdownToHtmlConvertor::do_heading(const QString& line) {
	close_ul();
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
	while( m_currentUlLevel < lvl ) {
		m_htmlText += "<ul>\n";
		++m_currentUlLevel;
	}
	m_htmlText += "<li>" + line.mid(2) + "\n";
}
void MarkdownToHtmlConvertor::do_paragraph(const QString& line) {
	close_ul();
	if( m_isParagraphOpen ) {
		m_htmlText += "<p>";
		m_isParagraphOpen = false;
	}
	m_htmlText += line + "\n";
}
