#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    buttons_list = {ui->btn_0, ui->btn_1, ui->btn_2, ui->btn_3, ui->btn_4, ui->btn_5, ui->btn_6 ,ui->btn_7 ,ui->btn_8 ,ui->btn_9};

    for (auto &&button: buttons_list) {
        connect(button, &QPushButton::clicked, this, &MainWindow::numClick);
    }

    on_btn_clear_clicked();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::appendLabel(QString str) {
    ui->lbl_number->setText(ui->lbl_number->text() + str);
}

void MainWindow::popLabel() {
    ui->lbl_number->setText(ui->lbl_number->text().left(ui->lbl_number->text().length() - 1));
}

QString MainWindow::peekLabel() {
    return ui->lbl_number->text().right(1);
}

void MainWindow::removeZeros(QString& str) {
    str.remove( QRegExp("0+$") );
    str.remove( QRegExp("\\.$") );
}

void MainWindow::calculateComplex() {

    QString first_sign = "+";
    QString second_sign = "+";

    auto pos_first = first_str.indexOf('+');
    auto pos_second = second_str.indexOf('+');


    if (pos_first == -1) {
        pos_first = first_str.lastIndexOf('-');
        first_sign = "-";
    }

    if (pos_second == -1) {
        pos_second = second_str.lastIndexOf('-');
        second_sign = "-";
    }

    first_complex.real(first_str.left(pos_first).toDouble());
    first_complex.imag(first_str.mid(pos_first + (first_sign=="+"?1:0), first_str.length() - pos_first - 1 - (first_sign=="+"?1:0)).toDouble());

    second_complex.real(second_str.left(pos_second).toDouble());
    second_complex.imag(second_str.mid(pos_second + (second_sign=="+"?1:0), second_str.length() - pos_second - 1 - (second_sign=="+"?1:0)).toDouble());


    switch (op) {
        case PLUS:
            first_complex += second_complex;
            break;

        case MINUS:
            first_complex -= second_complex;
            break;

        case MUL:
            first_complex *= second_complex;
            break;

        case DIV:
            first_complex /= second_complex;
            break;

    default:
        qDebug() << "Unknown operator?";

    }

    if (QString::number(first_complex.real()) == "nan" || QString::number(first_complex.imag()) == "nan"
            || QString::number(first_complex.real()) == "inf" || QString::number(first_complex.imag()) == "inf") {
        state = ERROR;
        ui->lbl_number->setText("ZeroDivision Error!");
        switchButtonsState();
        return;
    }

    QString ans_real = QString::number(first_complex.real(), 'f');
    QString ans_imag = QString::number(first_complex.imag(), 'f');

    removeZeros(ans_real);
    removeZeros(ans_imag);

    first_str = ans_real + (first_complex.imag()<0?"":"+") + ans_imag + "i";

    if (first_str.indexOf("inf") != -1 || first_str.length() > (MAX_LEN)) {
        state = ERROR;
        ui->lbl_number->setText("Limit exceeded!");
        switchButtonsState();
        return;
    }

    ui->lbl_number->setText(first_str);


}

void MainWindow::switchNumbersState(bool state) {
    for (auto &&button: buttons_list) {
        button->setEnabled(state);
    }
}

void MainWindow::switchButtonsState() {
    switch (state) {
        case REAL_NO_NUMBER:
            switchNumbersState(true);
            ui->btn_plus->setEnabled(false);
            ui->btn_minus->setEnabled(true);
            ui->btn_multiply->setEnabled(false);
            ui->btn_divide->setEnabled(false);
            ui->btn_i->setEnabled(false);
            ui->btn_delete->setEnabled(false);
            ui->btn_equal->setEnabled(false);
            ui->btn_dot->setEnabled(false);
            zero_state = false;
            break;

        case ONLY_REAL_SIGN:
            ui->btn_delete->setEnabled(true);
            ui->btn_minus->setEnabled(false);
            ui->btn_plus->setEnabled(false);
            ui->btn_dot->setEnabled(false);
            break;

        case REAL_PART:
            ui->btn_dot->setEnabled(true);
            ui->btn_minus->setEnabled(true);
            ui->btn_plus->setEnabled(true);
            ui->btn_delete->setEnabled(true);
            break;

        case REAL_DOT_NO_NUMBER:
            ui->btn_dot->setEnabled(false);
            ui->btn_minus->setEnabled(false);
            ui->btn_plus->setEnabled(false);
            break;

        case REAL_DOT_PART:
            ui->btn_minus->setEnabled(true);
            ui->btn_plus->setEnabled(true);
            break;

        case IMAG_NO_NUMBER:
            ui->btn_minus->setEnabled(false);
            ui->btn_plus->setEnabled(false);
            ui->btn_dot->setEnabled(false);
            ui->btn_i->setEnabled(false);
            break;

        case IMAG_PART:
            ui->btn_dot->setEnabled(true);
            ui->btn_i->setEnabled(true);
            break;

        case IMAG_DOT_NO_NUMBER:
            ui->btn_dot->setEnabled(false);
            ui->btn_i->setEnabled(false);
            break;

        case IMAG_DOT_PART:
            ui->btn_i->setEnabled(true);

            switchNumbersState(true);
            ui->btn_equal->setEnabled(false);
            ui->btn_minus->setEnabled(false);
            ui->btn_plus->setEnabled(false);
            ui->btn_multiply->setEnabled(false);
            ui->btn_divide->setEnabled(false);
            break;

        case NEXT_COMPLEX_OP:
            switchNumbersState(false);
            ui->btn_i->setEnabled(false);
            ui->btn_equal->setEnabled(false);
            ui->btn_minus->setEnabled(true);
            ui->btn_plus->setEnabled(true);
            ui->btn_multiply->setEnabled(true);
            ui->btn_divide->setEnabled(true);
            break;

        case EQUALITY:
            switchNumbersState(false);
            ui->btn_minus->setEnabled(false);
            ui->btn_plus->setEnabled(false);
            ui->btn_multiply->setEnabled(false);
            ui->btn_divide->setEnabled(false);
            ui->btn_i->setEnabled(false);
            ui->btn_equal->setEnabled(true);
            break;

        case ERROR:
            switchNumbersState(false);
            ui->btn_plus->setEnabled(false);
            ui->btn_minus->setEnabled(false);
            ui->btn_multiply->setEnabled(false);
            ui->btn_divide->setEnabled(false);
            ui->btn_i->setEnabled(false);
            ui->btn_delete->setEnabled(false);
            ui->btn_equal->setEnabled(false);
            ui->btn_dot->setEnabled(false);
            break;

        default:
            qDebug() << "Button switch troubles!";
            break;
    }
}


void MainWindow::nextState() {
    switch (state) {
        case REAL_NO_NUMBER:
            state = ONLY_REAL_SIGN;
            break;

        case ONLY_REAL_SIGN:
            state = REAL_PART;
            break;

        case REAL_PART:
            state = REAL_DOT_NO_NUMBER;
            break;

        case REAL_DOT_NO_NUMBER:
            state = REAL_DOT_PART;
            break;

        case REAL_DOT_PART:
            state = IMAG_NO_NUMBER;
            break;

        case IMAG_NO_NUMBER:
            state = IMAG_PART;
            break;

        case IMAG_PART:
            state = IMAG_DOT_NO_NUMBER;
            break;
        case IMAG_DOT_NO_NUMBER:
            state = IMAG_DOT_PART;
            break;

        case IMAG_DOT_PART:
            state = is_first?NEXT_COMPLEX_OP:EQUALITY;
            break;

        case NEXT_COMPLEX_OP:
            state = REAL_NO_NUMBER;
            is_first = false;
            break;

        case EQUALITY:
            state = NEXT_COMPLEX_OP;
            is_first = true;
            break;

        default:
            qDebug() << "Next state troubles!";
            break;
    }

    switchButtonsState();
}

void MainWindow::prevState() {
    switch (state) {
        case REAL_NO_NUMBER:
            qDebug() << "Trying to go deeper than None!";
            break;

        case ONLY_REAL_SIGN:
            state = REAL_NO_NUMBER;
            break;

        case REAL_PART:
            state = ONLY_REAL_SIGN;
            break;

        case REAL_DOT_NO_NUMBER:
            state = REAL_PART;
            break;

        case REAL_DOT_PART:
            state = REAL_DOT_NO_NUMBER;
            break;

        case IMAG_NO_NUMBER:
            state = REAL_DOT_PART;
            break;

        case IMAG_PART:
            state = IMAG_NO_NUMBER;
            break;

        case IMAG_DOT_NO_NUMBER:
            state = IMAG_PART;
            break;

        case IMAG_DOT_PART:
            state = IMAG_DOT_NO_NUMBER;
            break;

        case NEXT_COMPLEX_OP:
            state = IMAG_DOT_PART;
            break;

        case EQUALITY:
            state = IMAG_DOT_PART;
            break;

        default:
            qDebug() << "Previous state troubles!";
            break;
    }

    switchButtonsState();
}


void MainWindow:: numClick() {
    QPushButton *btn = (QPushButton *) sender();

    if (ui->lbl_number->text().length() == MAX_LEN)
        return;

    switch (state) {
        case REAL_NO_NUMBER:
            ui->lbl_number->setText(btn->text());
            if (btn->text() == "0")
                zero_state = true;
            nextState();
            nextState();
            break;

        case ONLY_REAL_SIGN:
            if (btn->text() == "0")
                zero_state = true;
            nextState();
            appendLabel(btn->text());
            break;

        case REAL_PART:
            if (zero_state && btn->text() == "0")
                break;
            else if (zero_state)
                popLabel();
            appendLabel(btn->text());
            break;

        case REAL_DOT_NO_NUMBER:
            appendLabel(btn->text());
            nextState();
            break;

        case REAL_DOT_PART:
            appendLabel(btn->text());
            break;

        case IMAG_NO_NUMBER:
            zero_state = false;
            if (btn->text() == "0")
                zero_state = true;
            appendLabel(btn->text());
            nextState();
            break;

        case IMAG_PART:
            if (zero_state && btn->text() == "0")
                break;
            else if (zero_state)
                popLabel();
            zero_state = false;
            appendLabel(btn->text());
            break;

        case IMAG_DOT_NO_NUMBER:
            appendLabel(btn->text());
            nextState();
            break;

        case IMAG_DOT_PART:
            appendLabel(btn->text());
            break;

        default:
            qDebug() << "Adding number on wrong state!";
    }
}


void MainWindow::on_btn_plus_clicked() {
    if (state == REAL_PART) {
        if (ui->lbl_number->text().length() == MAX_LEN)
            return;
        nextState();
        nextState();
        nextState();
        appendLabel("+");
    }
    else if (state == REAL_DOT_PART) {
        while (peekLabel() == '0')
            popLabel();
        if (peekLabel() == '.')
            popLabel();
        if (ui->lbl_number->text().length() == MAX_LEN)
            return;
        nextState();
        appendLabel("+");
    }

    else if (state == NEXT_COMPLEX_OP) {
        nextState();
        op = PLUS;
        first_str = ui->lbl_number->text();
        ui->lbl_number->clear();
    }
}

void MainWindow::on_btn_minus_clicked() {
    if (state == REAL_NO_NUMBER) {
        nextState();
        appendLabel("-");
        ui->btn_minus->setEnabled(false);
    }

    else if (state == REAL_PART) {
        if (ui->lbl_number->text().length() == MAX_LEN)
            return;
        nextState();
        nextState();
        nextState();
        appendLabel("-");
    }

    else if (state == REAL_DOT_PART) {
        if (ui->lbl_number->text().length() == MAX_LEN)
            return;
        while (peekLabel() == '0')
            popLabel();
        if (peekLabel() == '.')
            popLabel();
        nextState();
        appendLabel("-");
    }


    else if (state == NEXT_COMPLEX_OP) {
        nextState();
        op = MINUS;
        first_str = ui->lbl_number->text();
        ui->lbl_number->clear();
    }

}

void MainWindow::on_btn_multiply_clicked() {
    if (state == NEXT_COMPLEX_OP) {
        nextState();
        op = MUL;
        first_str = ui->lbl_number->text();
        ui->lbl_number->clear();
    }

}

void MainWindow::on_btn_divide_clicked() {
    if (state == NEXT_COMPLEX_OP) {
        nextState();
        op = DIV;
        first_str = ui->lbl_number->text();
        ui->lbl_number->clear();
    }

}


void MainWindow::on_btn_i_clicked() {
    if (state == IMAG_PART) {
        if (ui->lbl_number->text().length() == MAX_LEN)
            return;
        nextState();
        nextState();
        nextState();
        appendLabel("i");
    }

    else if (state == IMAG_DOT_PART) {
        while (peekLabel() == '0')
            popLabel();
        if (peekLabel() == '.')
            popLabel();
        if (ui->lbl_number->text().length() == MAX_LEN)
            return;
        nextState();
        appendLabel("i");
    }
}

void MainWindow::on_btn_dot_clicked() {
    if (state == REAL_PART) {
        if (ui->lbl_number->text().length() == MAX_LEN)
            return;
        appendLabel(".");
        nextState();
    }

    else if (state == IMAG_PART) {
        if (ui->lbl_number->text().length() == MAX_LEN)
            return;
        appendLabel(".");
        nextState();
    }

}

void MainWindow::on_btn_equal_clicked() {
    if (state == EQUALITY) {
        nextState();
        second_str = ui->lbl_number->text();
        ui->lbl_number->clear();
    }

    calculateComplex();
}


void MainWindow::on_btn_delete_clicked() {
    popLabel();

    switch (state) {
        case REAL_NO_NUMBER:
            qDebug() << "Trying to go deeper than None!";
            break;

        case ONLY_REAL_SIGN:
            prevState();
            break;

        case REAL_PART:
            if (ui->lbl_number->text().length() == 0) {
                prevState();
                prevState();
            }
            else if (peekLabel() == "-") {
                prevState();
            }
            break;

        case REAL_DOT_NO_NUMBER:
            prevState();
            break;

        case REAL_DOT_PART:
            if (ui->lbl_number->text().length() == 0) {
                prevState();
                prevState();
                prevState();
                prevState();
            }
            else if (peekLabel() == ".")
                prevState();
            else if (peekLabel() == "-") {
                prevState();
                prevState();
                prevState();
            }

            break;

        case IMAG_NO_NUMBER:
                prevState();
                if (ui->lbl_number->text().indexOf('.') == -1) {
                    prevState();
                    prevState();
                }
            break;

        case IMAG_PART:
            if (peekLabel() == "-" || peekLabel() == "+")
                prevState();
            break;

        case IMAG_DOT_NO_NUMBER:
                prevState();
            break;

        case IMAG_DOT_PART:
            if (peekLabel() == ".")
                prevState();
            else if (peekLabel() == "-" || peekLabel() == "+") {
                prevState();
                prevState();
                prevState();
            }
            break;

        case NEXT_COMPLEX_OP:
            prevState();
            if (ui->lbl_number->text().indexOf('.') == -1
                     || ((ui->lbl_number->text().count('.') == 1) && ((ui->lbl_number->text().indexOf('+') < ui->lbl_number->text().indexOf('.'))))
                     || ((ui->lbl_number->text().count('.') == 1) && ((ui->lbl_number->text().indexOf('-') < ui->lbl_number->text().indexOf('.'))))) {
                prevState();
                prevState();
            }
            break;

        case EQUALITY:
            prevState();
            if (ui->lbl_number->text().indexOf('.') == -1
                     || ((ui->lbl_number->text().count('.') == 1) && ((ui->lbl_number->text().indexOf('+') < ui->lbl_number->text().indexOf('.'))))
                     || ((ui->lbl_number->text().count('.') == 1) && ((ui->lbl_number->text().indexOf('-') < ui->lbl_number->text().indexOf('.'))))) {
                prevState();
                prevState();
            }
            break;

        default:
            qDebug() << "Unknown state to delete value";
    }

}


void MainWindow::on_btn_clear_clicked() {
    state = REAL_NO_NUMBER;
    is_first = true;
    switchButtonsState();
    ui->lbl_number->clear();
}

void MainWindow::on_btn_help_clicked() {
    QMessageBox::information(this, "Справка", "Ввод числа должен осуществляться всегда в форме:\n{Действительная часть} {Мнимая часть}!");
}



