#ifndef MARKDOWNTOHTMLCONVERTOR_H
#define MARKDOWNTOHTMLCONVERTOR_H

#include <QString>

class MarkdownToHtmlConvertor
{
public:
	explicit MarkdownToHtmlConvertor(const QString& markdownText)
        : m_markdownText(markdownText)
        //, m_isParagraphOpen(false)
        //, m_currentUlLevel(0)
    {}

    const QString&	convert();

private:
    void	do_heading(const QString&);
    void	do_list(const QString&);
    void	do_paragraph(const QString&);

    void	close_ul(int lvl=0);

private:
	QString		m_markdownText;
	QString		m_htmlText;
	bool		m_isParagraphOpen = true;
	int			m_currentUlLevel = 0;
	int			m_nSpace = 0;
};

#endif // MARKDOWNTOHTMLCONVERTOR_H
