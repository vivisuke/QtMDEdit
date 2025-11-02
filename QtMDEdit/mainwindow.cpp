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
    for(int i = 0; i != lst.size(); ++i) {
        const auto &line = lst[i];
        if( !line.isEmpty() ) {
        	if( line[0] == '#' ) {
        		int i = 1;
        		while( i < line.size() && line[i] == '#' ) ++i;
        		int h = i;
        		while( i < line.size() && line[i] == ' ' ) ++i;
                QString t = line.mid(i);
                txt += QString("<h%1>").arg(h) + t + QString("</h%1>\n").arg(h);
        	} else
	            txt += "<p>" + line + "\n";
        }
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
    ui->textEdit->setPlainText(m_htmlText);
}

void MainWindow::on_action_HTML_triggered()
{
    m_htmlMode = true;
    ui->textEdit->setHtml(m_htmlText);
}

