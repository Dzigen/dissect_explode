#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QList>

#define BLOCKS 12
#define COLOMS_KEYS 4
#define ROWS_KEYS 3

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_released();

private:
    Ui::MainWindow *ui;

    /**
     * @brief check_for_forbidden_sumbol проверка введённого текста для зашифровки
     * на наличие запрещённого символа "|",который уже используется программой.
     *
     * @param Text введённый текст для зашифровки
     * @return 0, если запрещённого символа в тексте обнаружено не было
     *         1, иначе
     */
    int check_for_forbidden_sumbol(QString &Text);

    /**
     * @brief check_for_blocks_quantity проверка введённого текста для расшифровки
     * на наличие требуемых блоков рассечённых-разнесённых символов и установленных разделительных символов.
     * @param Text введённый текст для расшифровки
     * @return 0, если текст для расшифровки был введён верно
     *         1, иначе
     */
    int check_for_blocks_quantity(QString &Text);

    /**
     * @brief get_coloms_rows_keys получить из введённой строки числовые значения ключей столбцов\строк таблицы
     * @param KeysString строка с введёнными ключами строк\столбцов таблицы
     * @param KeysSequence массив, куда нужно будет поместить числовые значения ключей
     * @param flag тип строки, из которой будут доставаться ключи:
     *        1 - строка с ключами столбцов таблицы
     *        0 - строка с ключами строк таблицы
     * @return 0, если удалось успешно достать числовые значения ключей строк\столбцов таблицы
     *         1, иначе
     */
    int get_coloms_rows_keys(QString KeysString,int* KeysSequence,bool flag);

    /**
     * @brief encode зашифровать введённый текст методом рассечения-разнесения.
     * @param TextForEncode текст для зашифровки
     * @param ColomnSequence ключи столбцов таблицы
     * @param RowSequence ключи строк таблицы
     */
    void encode(QString TextForEncode, int* ColomnSequence,int* RowSequence);

    /**
     * @brief decode расшифровать введённый текст методом рассечения-разнесения.
     * @param TextForDecode текст для расшифровки
     * @param ColomnSequence ключи столбцов таблицы
     * @param RowSequence ключи строк таблицы
     */
    void decode(QString TextForDecode, int* ColomnSequence,int* RowSequence);

};
#endif // MAINWINDOW_H
