#include <QClipboard>
#include "mainwindow.h"
#include "markdowntohtmlconvertor.h"
#include "./ui_mainwindow.h"


#if 0
//----------------------------------------------------------------------
class MarkdownToHtmlConvertor {
public:
	explicit MarkdownToHtmlConvertor(const QString& markdownText)
        : m_markdownText(markdownText)
        //, m_isParagraphOpen(false)
        //, m_currentUlLevel(0)
    {}
private:
	QString		m_markdownText;
	QString		m_htmlText;
	bool		m_isParagraphOpen = true;
	int			m_currentUlLevel = 0;
};
#endif
//----------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setWindowTitle("QtMDEdit ver 0.001");
}

MainWindow::~MainWindow()
{
	delete ui;
}

#if 0
QString toHtmlText(const QString& plainText) {
	auto lst = plainText.split('\n');
	QString txt;
	bool p_flag = true;
	//bool ul_flag = false;		//	<ul> ブロック内
	int ul_level = 0;
	for(int i = 0; i != lst.size(); ++i) {
		auto line = lst[i];
		int nspc = 0;
	  	while( nspc < line.size() && line[nspc] == ' ' ) ++nspc;
	  	if( nspc != 0 ) line = line.mid(nspc);
		if( !line.isEmpty() ) {
			if( line[0] == '#' ) {
				while( ul_level > 0) {
					txt += "</ul>\n";
					--ul_level;
				}
				int i = 1;
				while( i < line.size() && line[i] == '#' ) ++i;
				int h = i;
				while( i < line.size() && line[i] == ' ' ) ++i;
				QString t = line.mid(i);
				txt += QString("<h%1>").arg(h) + t + QString("</h%1>\n").arg(h);
				p_flag = true;
			} else if( line.left(2) == "- " ) {
				const int lvl = nspc/2 + 1;
				while( ul_level > lvl ) {
					txt += "</ul>\n";
					--ul_level;
				}
				while( ul_level < lvl ) {
					txt += "<ul>\n";
					++ul_level;
				}
				txt += "<li>" + line.mid(2) + "\n";
			} else {
				while( ul_level > 0) {
					txt += "</ul>\n";
					--ul_level;
				}
				if( p_flag ) {
					txt += "<p>";
					p_flag = false;
				}
				txt += line + "\n";
			}
		} else
			p_flag = true;
	}
	while( ul_level > 0) {
		txt += "</ul>\n";
		--ul_level;
	}
	return txt;
}
#endif

void MainWindow::plainTextChanged() {
	qDebug() << "plainTextChanged()\n";
	m_plainText = ui->plainTextEdit->toPlainText();
	//m_htmlText = toHtmlText(m_plainText);
	//MarkdownToHtmlConvertor mhc(m_plainText);
	m_htmlComvertor.setMarkdownText(m_plainText);
	m_htmlText = m_htmlComvertor.convert();
	if( m_htmlMode )
		ui->textEdit->setHtml(m_htmlText);
	else
		ui->textEdit->setPlainText(m_htmlText);
}

void MainWindow::on_action_Source_triggered()
{
	m_htmlMode = false;
	ui->action_HTML->setChecked(false);
	ui->textEdit->setPlainText(m_htmlText);
}

void MainWindow::on_action_HTML_triggered()
{
	m_htmlMode = true;
	ui->action_Source->setChecked(false);
	ui->textEdit->setHtml(m_htmlText);
}


void MainWindow::on_actione_Xit_triggered()
{
    qApp->quit();
}


void MainWindow::on_actionCopy_HTML_Source_triggered()
{
    QString src = m_htmlComvertor.getHtmlText();
    //QString src = ui->textEdit->toPlainText();
    //QString src = ui->plainTextEdit->toPlainText();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(src);
	statusBar()->showMessage("HTML source text is copyed to clipboard.", 3000);
}

