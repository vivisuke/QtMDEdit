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
            txt += "<p>" + line + "\n";
        }
    }
    return txt;
}

void MainWindow::plainTextChanged() {
    qDebug() << "plainTextChanged()\n";
    m_plainText = ui->plainTextEdit->toPlainText();
    m_htmlText = toHtmlText(m_plainText);
    ui->textEdit->setHtml(m_htmlText);
}
