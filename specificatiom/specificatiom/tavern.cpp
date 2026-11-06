#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>

// ============================================================
//  ТАВЕРНА «У КОДА» — автоматизация кассы
// ------------------------------------------------------------
//  Вход:  количество кружек эля, порций кабанятины, наличие
//         гильдейской карты и размер скидки в процентах.
//  Выход: итоговый чек в текстовом формате.
// ============================================================

// ---------- Константы прейскуранта ----------
const double ALE_PRICE = 35.0;    // кружка эля, медяков
const double BOAR_PRICE = 90.0;   // порция кабанятины, медяков
const int    LINE_WIDTH = 47;     // ширина чека, символов

// ---------- Вспомогательные функции ----------


// Печать горизонтальной линии заданным символом
void printLine(const char symbol) {
    std::cout << std::string(LINE_WIDTH, symbol) << "\n";
}

// Кастомная функция для вывода статуса обслуживания
void showTavernStatus(const std::string& message) {
    std::cout << "\n[ТАВЕРНА] " << message << "\n";
}

/*
Безопасный ввод целого числа: при нечисловом вводе поток
восстанавливается и запрос повторяется 
*/

int readInt(const std::string& prompt) {
    std::string line;

    while (true) {
        std::cout << prompt;

        // Читаем строку целиком — «мусор» не остаётся в буфере
        if (!std::getline(std::cin, line)) {
            std::cout << "\n X [ОШИБКА] Ввод прерван (поток закрыт).\n";
            std::exit(1);
        }

        std::istringstream iss(line);
        int value = 0;
        char extra = '\0';

        /* Ввод корректен, если из строки прочиталось целое число
           и после него нет лишних символов (букв, запятых и т.п.) */
        if (iss >> value && !(iss >> extra)) {
            return value;
        }

        std::cout << "[ОШИБКА] Требуется целое число. Повторите ввод.\n" << std::endl;
    }
}

// Строка заказа: наименование | количество | цена | сумма
void printOrderLine(const std::string& title, int qty, double price) {
    std::cout << std::left << std::setw(18) << title
        << std::right << std::setw(6) << qty
        << std::setw(8) << price
        << std::setw(10) << qty * price << " мед.\n" << std::endl;;
}

// Строка итогов: подпись слева, сумма в денежной колонке чека
void printTotalLine(const std::string& label, double amount) {
    std::cout << std::left << std::setw(32) << label
        << std::right << std::setw(10) << amount << " мед.\n" << std::endl;
}

int main() {
    std::setlocale(0, "");
    std::cout << std::fixed << std::setprecision(1);  // денежный формат: 1 знак после запятой

    // ---------- 0. Шапка ----------
    printLine('=');
    std::cout << "                 ТАВЕРНА «У КОДА»\n";
    std::cout << "              Кассовый модуль, версия 1.0\n";
    printLine('=');

    // ---------- 1. Ввод данных ----------
    std::cout << "\n--- РЕГИСТРАЦИЯ ЗАКАЗА ГОСТЯ ---\n\n";
    const int aleQty = readInt("Кружки эля, шт (35 мед./шт): ");
    const int boarQty = readInt("Порции кабанятины, шт (90 мед./шт): ");
    const int hasCard = readInt("Предъявлена гильдейская карта? (1 - да / 0 - нет): ");

    int discountPercent = 0;
    if (hasCard == 1) {
        discountPercent = readInt("Размер скидки по карте, %: ");
    }

    // ---------- 2. Валидация входных данных ----------
    if (aleQty < 0 || boarQty < 0) {
        std::cout << "\n[ОШИБКА] Количество не может быть отрицательным.\n" << std::endl;
        return 1;
    }
    if (aleQty == 0 && boarQty == 0) {
        std::cout << "\n[ОШИБКА] Заказ пуст: не выбрано ни одной позиции.\n" << std::endl;
        return 1;
    }
    if (hasCard != 0 && hasCard != 1) {
        std::cout << "\n[ОШИБКА] Ответ о карте должен быть 1 (да) или 0 (нет).\n" << std::endl;
        return 1;
    }
    if (discountPercent < 0 || discountPercent > 100) {
        std::cout << "\n[ОШИБКА] Скидка должна быть в диапазоне от 0 до 100 %.\n" << std::endl;
        return 1;
    }

    showTavernStatus("Заказ принят, выполняется расчет стоимости...");

    // ---------- 3. Расчет стоимости ----------
    const double aleTotal = aleQty * ALE_PRICE;
    const double boarTotal = boarQty * BOAR_PRICE;
    const double subtotal = aleTotal + boarTotal;                // сумма без скидки
    const double discount = subtotal * discountPercent / 100.0;  // скидка, медяков
    const double total = subtotal - discount;                 // итог к оплате

    // ---------- 4. Печать чека ----------
    std::cout << "\n";
    printLine('-');
    std::cout << "                  ЧЕК ЗАКАЗА\n";
    printLine('-');
    std::cout << std::left << std::setw(18) << "Наименование"
        << std::right << std::setw(6) << "Кол-во"
        << std::setw(8) << "Цена"
        << std::setw(10) << "Сумма" << "\n";
    printLine('-');

    if (aleQty > 0) printOrderLine("Кружка эля", aleQty, ALE_PRICE);
    if (boarQty > 0) printOrderLine("Порция кабанятины", boarQty, BOAR_PRICE);
    printLine('-');

    printTotalLine("Сумма без скидки:", subtotal);
    if (discount > 0.0) {
        printTotalLine("Скидка гильдии (" + std::to_string(discountPercent) + " %):", -discount);
    }
    printLine('-');
    printTotalLine("ИТОГО К ОПЛАТЕ:", total);
    printLine('=');

    // ---------- 5. Итоговый статус ----------
    if (discount > 0.0) {
        showTavernStatus("ЧЕК СФОРМИРОВАН: гильдейская скидка применена.");
    }
    else {
        showTavernStatus("ЧЕК СФОРМИРОВАН: расчет по стандартному прейскуранту.");
    }
    std::cout << "\nСпасибо за визит, странник! Ещё увидимся!\n";
    return 0;
}