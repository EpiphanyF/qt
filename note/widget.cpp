#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QWheelEvent>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    QShortcut *shortcutopen = new QShortcut(QKeySequence(tr("Ctrl+O", "File|Open")),this);
    QShortcut *shortcutsave = new QShortcut(QKeySequence(tr("Ctrl+S", "File|Save")),this);

    connect(shortcutopen,&QShortcut::activated,[=](){
        on_btmopen_clicked();
    });

    connect(shortcutsave,&QShortcut::activated,[=](){
        on_btmsave_clicked();
    });

}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event){
    QMessageBox msgBox;
      msgBox.setText("当前文件正在被关闭");
      msgBox.setInformativeText("你想要关闭此文件吗?");
      msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Ok | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::Save);
      int ret = msgBox.exec();
      switch (ret) {
          case QMessageBox::Ok:
              event->accept();
              break;
          case QMessageBox::Save:
              on_btmsave_clicked();
              event->accept();
              break;
          case QMessageBox::No:
              event->ignore();
              break;
          default:
              break;
      }
}

void Widget::wheelEvent(QWheelEvent *event){
    if(event->modifiers() & Qt::ControlModifier){
        QFont font = ui->textEdit->font();
        int fontsize = font.pointSize();
        if(fontsize==-1)return;

        if(event->angleDelta().y()>0){
            fontsize++;
        }else if(event->angleDelta().y()<0){
            fontsize--;
            if(fontsize<10)
                fontsize=10;
        }

        font.setPointSize(fontsize);
        ui->textEdit->setFont(font);
        event->accept();
    }else{
        QWidget::wheelEvent(event);
    }
}

//打开
void Widget::on_btmopen_clicked()
{
    //打开文本对话框，选择文件
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),"E:/qt",
                                                    tr("Text(*.txt)"));

    ui->textEdit->clear();
    //创建对象
    file.setFileName(filename);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text)){
        qDebug()<<"open error";
    }

    this->setWindowTitle(filename+"-note");

    //创建文本流对象，用于读取数据
    QTextStream in(&file);
    QString str = ui->comboBox->currentText();
    const char* c_str=str.toStdString().c_str();
    in.setCodec(c_str);
    while(!in.atEnd()){
        QString context = in.readLine();
        ui->textEdit->append(context);
    }

}
//保存
void Widget::on_btmsave_clicked()
{
    //判断是否弹窗
    if(!file.isOpen()){
        QString filename = QFileDialog::getSaveFileName(this,tr("Save File"),"E:/qt",
                                                        tr("Text(*.txt)"));
        file.setFileName(filename);
        if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
            qDebug()<<"open error";
        }
        this->setWindowTitle(filename+"note");

    }

    QTextStream out(&file);
    QString str = ui->comboBox->currentText();
    const char* c_str=str.toStdString().c_str();
    out.setCodec(c_str);
    QString context = ui->textEdit->toPlainText();
    //清空
    file.resize(0);
    out<<context;
}
//关闭
void Widget::on_btmclose_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("关闭");
    msgBox.setInformativeText("你想要保存此次修改吗?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch (ret) {
        case QMessageBox::Save:
            on_btmsave_clicked();
            break;
        case QMessageBox::Discard:
            ui->textEdit->clear();
            if(file.isOpen()){
                file.close();
                this->setWindowTitle("note");
            }
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
      }
}
//编码
void Widget::on_comboBox_currentIndexChanged(int index)
{
    ui->textEdit->clear();
    if(file.isOpen()){
        QTextStream in(&file);
        in.setCodec(ui->comboBox->currentText().toStdString().c_str());
        in.seek(0);
        while(!in.atEnd()){
            QString context = in.readLine();
            ui->textEdit->append(context);
        }
    }
}
//光标
void Widget::on_textEdit_cursorPositionChanged()
{
    QTextCursor cusor = ui->textEdit->textCursor();
    QString blocknum = QString::number(cusor.blockNumber()+1);
    QString columnnum = QString::number(cusor.columnNumber()+1);

    const QString label = "第"+blocknum+"行,第"+columnnum+"列";
    ui->LR->setText(label);


    //当前行强调
    QTextEdit::ExtraSelection ext;
    QList<QTextEdit::ExtraSelection> exts;
    QBrush brush(Qt::lightGray);

    ext.cursor = cusor;
    ext.format.setBackground(brush);
    ext.format.setProperty(QTextFormat::FullWidthSelection,true);
    exts.append(ext);
    ui->textEdit->setExtraSelections(exts);


}


