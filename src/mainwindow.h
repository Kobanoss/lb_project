#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <complex>
#include <QDebug>
#include <QVector>
#include <QPushButton>

enum equationState {REAL_NO_NUMBER, ONLY_REAL_SIGN,
                    REAL_PART, REAL_DOT_NO_NUMBER, REAL_DOT_PART,
                    IMAG_NO_NUMBER,
                    IMAG_PART, IMAG_DOT_NO_NUMBER, IMAG_DOT_PART,
                    NEXT_COMPLEX_OP, EQUALITY, ERROR};
enum operation {PLUS, MINUS, MUL, DIV};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_plus_clicked();
    void on_btn_minus_clicked();
    void on_btn_multiply_clicked();
    void on_btn_divide_clicked();

    void on_btn_i_clicked();

    void on_btn_dot_clicked();

    void on_btn_equal_clicked();

    void on_btn_delete_clicked();
    void on_btn_clear_clicked();

    void on_btn_help_clicked();


private:
    Ui::MainWindow *ui;

    QVector<QPushButton*> buttons_list;

    equationState state;
    operation op;

    QString first_str, second_str;
    std::complex<double> first_complex, second_complex;

    bool is_first;
    bool zero_state;

    void appendLabel(QString str);
    void popLabel();
    QString peekLabel();

    void numClick();

    void switchNumbersState(bool state);
    void switchButtonsState();

    void nextState();
    void prevState();

    void calculateComplex();
};
#endif // MAINWINDOW_H
