#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void wheelEvent(QWheelEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:
    void on_btmopen_clicked();

    void on_btmsave_clicked();

    void on_btmclose_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_textEdit_cursorPositionChanged();

private:
    Ui::Widget *ui;
    QFile file;
};
#endif // WIDGET_H
