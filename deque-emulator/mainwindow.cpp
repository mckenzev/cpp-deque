#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ApplyModel();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetRandomGen(const std::mt19937& random_gen) {
    random_gen_ = random_gen;
}

void MainWindow::ApplyModel() {
    UpdateOutput();
    ChangeStateBtnsPop();
    UpdateTxtSize();
    ApplyIterator();
}

void MainWindow::UpdateOutput() {
    // Внутри UpdateOutput происходят изменения list_widget, что приведет
    // к ненужному вызову SetCurrentIterator, поэтому устанавливается блокировка сигнала
    ui->list_widget->blockSignals(true); // Блокировка сигнала
    ui->list_widget->clear();
    QStringList output;
    const auto& items = model_.GetItems();
    for (size_t i = 0; i < items.size(); ++i) {
        output.push_back(QString("%1: %2").arg(i)
                                          .arg(QString::fromStdString(items[i])));
    }
    ui->list_widget->addItems(output);
    ui->list_widget->addItem("end");
    ui->list_widget->blockSignals(false); // Разблокировка
}

void MainWindow::ChangeStateBtnsPop() {
    bool state = (model_.Size() != 0);
    ui->btn_pop_back->setEnabled(state);
    ui->btn_pop_front->setEnabled(state);
}

void MainWindow::UpdateTxtSize() {
    auto size = model_.Size();
    ui->txt_size->setText(QString::number(size));
}

void MainWindow::ApplyIterator() {
    SetCurrentIterator();
    ButtonChangesForBegin();
    ButtonChangesForEnd();
    ChangeTxtElemContent();
}

// Устанавливает положение строки list_widget в соответствие со смещением итератора модели оносительно items_.begin()
void MainWindow::SetCurrentIterator() {
    size_t offset = model_.GetCurrentIteratorPos();
    // Блокировка сигнала так как при изменении строки list_widget
    // сработает сигнал об изменении, который изменит положение итератора items_(хоть и на ту же позицию)
    // эффекта никакого не будет, но будет бесполезный вызов функции
    ui->list_widget->blockSignals(true); // блокировка
    ui->list_widget->setCurrentRow(offset);
    ui->list_widget->blockSignals(false); // разблокировка
}

void MainWindow::ButtonChangesForBegin() {
    auto state = model_.IsIteratorBegin();

    ui->btn_decrement->setDisabled(state);
}

void MainWindow::ButtonChangesForEnd() {
    auto state = model_.IsIteratorEnd();

    ui->btn_edit->setDisabled(state);
    ui->btn_erase->setDisabled(state);
    ui->btn_increment->setDisabled(state);
}

void MainWindow::ChangeTxtElemContent() {
    auto text = model_.GetContentByIterator();
    ui->txt_elem_content->setText(QString::fromStdString(text));
}

// Смещает итератор items_ в соответсвие со смещением строки в list_widget
// Метод вызывается при изменении строки list_widget
void MainWindow::ChangeContainerIterator() {
    auto offset = ui->list_widget->currentRow();
    model_.MoveIterator(offset);
    SetCurrentIterator();
}

void MainWindow::SetWorkPiece(const std::initializer_list<std::string>& data) {
    model_.UpdateData(data);
    ApplyModel();
}

// Слоты
void MainWindow::on_btn_pop_back_clicked() {
    model_.PopBack();
    ApplyModel();
}

void MainWindow::on_btn_push_back_clicked() {
    auto str = ui->txt_elem_content->text().toStdString();
    model_.PushBack(str);
    ApplyModel();
}

void MainWindow::on_btn_pop_front_clicked() {
    model_.PopFront();
    ApplyModel();
}

void MainWindow::on_btn_push_front_clicked() {
    auto str = ui->txt_elem_content->text().toStdString();
    model_.PushFront(str);
    ApplyModel();
}

void MainWindow::on_btn_clear_clicked() {
    model_.Clear();
    ApplyModel();
}

void MainWindow::on_btn_edit_clicked() {
    auto text = ui->txt_elem_content->text();
    model_.Edit(text.toStdString());
    ApplyModel();
}

void MainWindow::on_btn_erase_clicked() {
    model_.Erase();
    ApplyModel();
}

void MainWindow::on_btn_insert_clicked() {
    auto text = ui->txt_elem_content->text();
    model_.Insert(text.toStdString());
    ApplyModel();
}

void MainWindow::on_btn_decrement_clicked() {
    model_.Decrement();
    SetCurrentIterator();
    ApplyIterator();
}

void MainWindow::on_btn_increment_clicked() {
    model_.Increment();
    ApplyIterator();
}

void MainWindow::on_btn_begin_clicked() {
    model_.SetIteratorBegin();
    ApplyIterator();
}

void MainWindow::on_btn_end_clicked() {
    model_.SetIteratorEnd();
    ApplyIterator();
}

void MainWindow::on_btn_resize_clicked() {
    auto size = ui->txt_size->text().toULongLong();
    model_.Resize(size);
    ApplyModel();
}

void MainWindow::on_btn_find_clicked() {
    auto text = ui->txt_elem_content->text().toStdString();
    model_.Find(text);
    ApplyIterator();
}

void MainWindow::on_btn_lower_bound_clicked() {
    auto text = ui->txt_elem_content->text().toStdString();
    if (model_.LowerBound(text)) {
        ApplyIterator();
    }
}

void MainWindow::on_btn_upper_bound_clicked() {
    auto text = ui->txt_elem_content->text().toStdString();
    if (model_.UpperBound(text)) {
        ApplyIterator();
    }
}

void MainWindow::on_btn_count_clicked() {
    auto text = ui->le_count->text().toStdString();
    auto count = model_.Count(text);
    ui->lbl_count->setText(QString::number(count));
}

void MainWindow::on_btn_min_element_clicked() {
    model_.MinElement();
    ApplyIterator();
}

void MainWindow::on_btn_max_element_clicked() {
    model_.MaxElement();
    ApplyIterator();
}

void MainWindow::on_btn_merge_sort_clicked() {
    model_.MergeSort();
    ApplyModel();
}

void MainWindow::on_btn_merge_sort_case_free_clicked() {
    model_.MergeSortCaseFree();
    ApplyModel();
}

void MainWindow::on_btn_unique_clicked() {
    model_.Unique();
    ApplyModel();
}

void MainWindow::on_btn_reverse_clicked() {
    model_.Reverse();
    ApplyModel();
}

void MainWindow::on_btn_shuffle_clicked() {
    model_.Shuffle(random_gen_);
    ApplyModel();
}

void MainWindow::on_btn_tea_clicked() {
    SetWorkPiece({
        "Чай Лунцзин",
        "Эрл Грей",
        "Сенча",
        "Пуэр",
        "Дарджилинг",
        "Ассам",
        "Матча",
        "Ганпаудер",
        "Оолонг",
        "Лапсанг Сушонг"
    });
}

void MainWindow::on_btn_cakes_clicked() {
    SetWorkPiece({
        "Красный бархат",
        "Наполеон",
        "Медовик",
        "Тирамису",
        "Прага",
        "Чизкейк",
        "Захер",
        "Эстерхази",
        "Морковный торт",
        "Чёрный лес"
    });
}

void MainWindow::on_list_widget_currentRowChanged() {
    ChangeContainerIterator();
    ApplyIterator();
}

