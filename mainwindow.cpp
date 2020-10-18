#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Метод рассечения-разнесения данных");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::encode(QString TextForEncode, int* ColomnSequence, int* RowSequence){

    /*Инициализируем пременные*/
    QString EncodedText="";
    QList<QCharRef> BlockedText[BLOCKS];

    int TextLen=TextForEncode.length();
    int TableIndexes[TextLen][2];
    int RowIndex;
    int ColomnIndex;

    /*Формируем таблицу, где каждому символу в тексте будет ставиться в соответствие заданный ключ-столбец и ключ-строка*/
    for(int i=0;i<TextLen;i++){
        RowIndex=(i%ROWS_KEYS);
        ColomnIndex=(i%COLOMS_KEYS);
        TableIndexes[i][0]=RowSequence[RowIndex];
        TableIndexes[i][1]=ColomnSequence[ColomnIndex];
    }

    /*Разбиваем текст на блоки символов в соответствии с формулой рассечения-разнесения*/
    int BlockIndex;
    for(int i=0;i<TextLen;i++){
        BlockIndex=COLOMS_KEYS*(TableIndexes[i][0]-1)+TableIndexes[i][1];
        BlockedText[BlockIndex-1].push_back(TextForEncode[i]);
    }

    /*Формируем строку зашифрованного текста для вывода*/
    for(int i=0;i<BLOCKS-1;i++){
        for(QList<QCharRef>::iterator iter=BlockedText[i].begin(); iter!=BlockedText[i].end(); iter++){
            EncodedText+=*iter;
        }
        EncodedText+="|";
    }
    for(QList<QCharRef>::iterator iter=BlockedText[BLOCKS-1].begin(); iter!=BlockedText[BLOCKS-1].end(); iter++){
            EncodedText+=*iter;
    }

    ui->OutputText->setPlainText(EncodedText);
}

void MainWindow::decode(QString TextForDecode, int* ColomnSequence, int* RowSequence){

    /*Инициализируем переменные*/
    QString DecodedText="";
    QList<QCharRef> BlockedText[BLOCKS];

    int TextLen=TextForDecode.length()-(BLOCKS-1);
    int TableIndexes[TextLen][2];
    int RowIndex;
    int ColomnIndex;

    /*Формируем таблицу, где каждому символу блока будет ставиться в соответствие заданный ключ-столбец и ключ-строка*/
    for(int i=0;i<TextLen;i++){
        RowIndex=(i%ROWS_KEYS);
        ColomnIndex=(i%COLOMS_KEYS);
        TableIndexes[i][0]=RowSequence[RowIndex];
        TableIndexes[i][1]=ColomnSequence[ColomnIndex];
    }

    /*Достаём из строки блоки символов и разносим их по соответствующим позициям в массиве BlockedText*/
    int BlockIndex=0;
    for(int i=0;i<TextLen+11;i++){
        if(TextForDecode[i]!='|'){
            BlockedText[BlockIndex].push_back(TextForDecode[i]);
        }else{
            BlockIndex++;
        }
    }

    /*Расшифровываем текст в соответствии с формулой рассечения-разнесения*/
    for(int i=0;i<TextLen;i++){
        BlockIndex=COLOMS_KEYS*(TableIndexes[i][0]-1)+TableIndexes[i][1];
        if(!BlockedText[BlockIndex-1].isEmpty()){
            DecodedText+=BlockedText[BlockIndex-1].front();
            BlockedText[BlockIndex-1].pop_front();
        }
    }

    ui->OutputText->setPlainText(DecodedText);
}

int MainWindow::get_coloms_rows_keys(QString KeysString,int* KeysSequence,bool flag){

    QStringList KeysList;
    KeysList=KeysString.split(" ");

    /*Определяем тип введённой строки*/
    int KeysQuantity;
    if(flag){
        KeysQuantity=COLOMS_KEYS;
    }else{
        KeysQuantity=ROWS_KEYS;
    }

    /*Проверяем: соответствует ли количество ключей требуемому значению*/
    if(KeysList.length()>KeysQuantity || KeysList.length()<KeysQuantity){
        return 1;
    }

    /*Проверяем: являются ли ключи числовыми значениями, а ткаже не выходят ли ключи за границы заданного диапозона значений*/
    for(int i=0;i<KeysList.length();i++){
        if(!KeysList[i][0].isDigit() || KeysList[i].toInt()>KeysQuantity || KeysList[i].toInt()<1){
            return 1;
        }
    }

    /*Записываем значения ключей в числовой массив*/
    for(int i=0;i<KeysList.length();i++){
        KeysSequence[i]=KeysList[i].toInt();
    }

    return 0;
}

int MainWindow::check_for_forbidden_sumbol(QString &Text){

    for(int i=0;i<Text.length();i++){
        if(Text[i]=="|"){
            return 1;
        }
    }

    return 0;
}

int MainWindow::check_for_blocks_quantity(QString &Text){

    int counter=0;

    for(int i=0;i<Text.length();i++){
        if(Text[i]=="|"){
            counter++;
        }
    }

    /*Проверяем:было ли введено требуемое количество блоков символов в соответствии с указанным способом записи*/
    if(counter!=(BLOCKS-1)){
        return 1;
    }

    return 0;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    /*Формируем текстовые поля*/
    if(arg1=="Расшифровать"){
        ui->LabelInputText->setText("Введите зашифрованный текст");

        ui->AttentionTextInputText->clear();
        ui->AttentionTextInputText->insertPlainText("1.нужно ввести 12 блоков рассечённых-разнесённых символов(от 1-ого до 12-ого).\n2.в качестве разделителя блоков использовать символ \"|\".\n3.блоки рассечённых-разнесённых символов не должны содржать символ \"|\".");

        ui->LabelOutputText->setText("Расшифрованный текст");
        ui->AttentionTextOutputText->clear();
        ui->AttentionTextOutputText->insertPlainText("Отсутствуют.");

        ui->pushButton->setText("выполнить расшифровку");

    }else if(arg1=="Зашифровать"){
        ui->LabelInputText->setText("Введите текст для зашифровки");

        ui->AttentionTextInputText->clear();
        ui->AttentionTextInputText->insertPlainText("1.текст не должен содержать символа \"|\", так как он зарезервирован программой в качестве разделителя при выводе блоков зашифрованного текста. ");


        ui->LabelOutputText->setText("Зашифрованный текст");
        ui->AttentionTextOutputText->clear();
        ui->AttentionTextOutputText->insertPlainText("1.будет выведено 12 блоков рассечённых-разнесённых символов.\
                                                     2.блоки выводятся последовательно(от 1-ого до 12-ого).\
                                                     3.в качестве разделителя блоков исползуется символ \"|\".");
        ui->pushButton->setText("выполнить зашифровку");
    }

    ui->OutputText->clear();
    ui->InputText->clear();
    ui->InputRowsKeys->clear();
    ui->InputColomsKeys->clear();
}

void MainWindow::on_pushButton_released()
{
    /*Инициализируем переменные*/
    int ColomsKeys[COLOMS_KEYS];
    int RowsKeys[ROWS_KEYS];
    QString Text=ui->InputText->toPlainText();
    int statusColomsKeys;
    int statusRowsKeys;
    int statusText;

    /*очищаем поля вывода текста*/
    ui->OutputText->clear();

    /*получаем числовые значения ключей столбцов/строк таблицы из соответствующих строк*/
    statusColomsKeys=get_coloms_rows_keys(ui->InputColomsKeys->toPlainText(), ColomsKeys,1);
    statusRowsKeys=get_coloms_rows_keys(ui->InputRowsKeys->toPlainText(), RowsKeys,0);

    /*В случае успешного получения числовых значений ключей*/
    if(statusColomsKeys==0 && statusRowsKeys==0){
        /*очищаем поля ввода ключей*/
        ui->InputColomsKeys->setPlainText("");
        ui->InputRowsKeys->setPlainText("");

        if(ui->comboBox->currentText()=="Зашифровать"){
            /*проверяем введённый текст на наличие запрещённого символа "|"*/
            statusText=check_for_forbidden_sumbol(Text);

            if(statusText==0){
                /*зашифровываем текст*/
                encode(Text,ColomsKeys,RowsKeys);

                ui->InputText->setPlainText("");
                ui->statusbar->showMessage("Зашифровка текста выполнена успешно");
            }else{
                ui->statusbar->showMessage("Ошибка ввода текста: смотри примечания.");
            }

        }else if(ui->comboBox->currentText()=="Расшифровать"){

            /*проверяем введённый зашифрованный текст на наличие требуемого количество блоков символов*/
            statusText=check_for_blocks_quantity(Text);

            if(statusText==0){
                /*расшифровываем текст*/
                decode(Text,ColomsKeys,RowsKeys);

                ui->InputText->setPlainText("");
                ui->statusbar->showMessage("Расшифровка текста выполнена успешно");
            }else{
                ui->statusbar->showMessage("Ошибка ввода зашифрованного текста: смотри примечания.");
            }
        }

    }else{
        ui->statusbar->showMessage("Ошибка ввода ключей: смотри примечания.");
    }
}


