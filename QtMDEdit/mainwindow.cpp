#include "mainwindow.h"
#include "./ui_mainwindow.h"

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

QString toHtmlText(const QString& plainText) {
    auto lst = plainText.split('\n');
    QString txt;
    bool p_flag = true;
    bool ul_flag = false;		//	<ul> ブロック内
    for(int i = 0; i != lst.size(); ++i) {
        auto line = lst[i];
        int nspc = 0;
      	while( nspc < line.size() && line[nspc] == ' ' ) ++nspc;
      	if( nspc != 0 ) line = line.mid(nspc);
        if( !line.isEmpty() ) {
        	if( line[0] == '#' ) {
        		if( ul_flag ) {
        			txt += "</ul>\n";
        			ul_flag = false;
        		}
        		int i = 1;
        		while( i < line.size() && line[i] == '#' ) ++i;
        		int h = i;
        		while( i < line.size() && line[i] == ' ' ) ++i;
                QString t = line.mid(i);
                txt += QString("<h%1>").arg(h) + t + QString("</h%1>\n").arg(h);
	        	p_flag = true;
        	} else if( line.left(2) == "- " ) {
        		if( !ul_flag ) {
        			txt += "<ul>\n";
        			ul_flag = true;
        		}
        		txt += "<li>" + line.mid(2) + "\n";
        	} else {
        		if( ul_flag ) {
        			txt += "</ul>\n";
        			ul_flag = false;
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
	if( ul_flag ) {
		txt += "</ul>\n";
		ul_flag = false;
	}
    return txt;
}

void MainWindow::plainTextChanged() {
    qDebug() << "plainTextChanged()\n";
    m_plainText = ui->plainTextEdit->toPlainText();
    m_htmlText = toHtmlText(m_plainText);
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

