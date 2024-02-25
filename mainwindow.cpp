#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->plainTextEdit1, SIGNAL(textChanged()), this, SLOT(convert()));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(open()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(save()));
}

MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::encode(const QString &input) {
    QString output;

    for (const QChar &c : input) {
        if (lookUpTable1.contains(c.toUpper())) {
            output += lookUpTable1.value(c.toUpper());
        }
        output += ' ';
    }

    return output;
}

QString MainWindow::decode(const QString &input) {
    QString output;

    for (const QString &token : input.split(' ')) {
        if (lookUpTable2.contains(token)) {
            output += lookUpTable2.value(token);
        }
    }

    return output;
}

void MainWindow::convert() {
    QString input = ui->plainTextEdit1->toPlainText();
    if (input.startsWith('.') || input.startsWith('-')) {
        QString output = decode(input);
        ui->plainTextEdit2->setPlainText(output);
    } else {
        QString output = encode(input);
        ui->plainTextEdit2->setPlainText(output);
    }
}

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QTextStream in(&file);
    ui->plainTextEdit1->setPlainText(in.readAll());
    file.close();
}

void MainWindow::save() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QTextStream out(&file);
    out << ui->plainTextEdit2->toPlainText();
    file.close();
}
