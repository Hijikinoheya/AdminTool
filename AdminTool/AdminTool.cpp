#include <iostream>
#include "admin_tool.h"

int main() {
    int choice;
    while (true) {
        displayMenu();  // メニューを表示
        std::cin >> choice;  // ユーザーの選択を受け取る
        handleMenuSelection(choice);  // 選択に応じて処理を実行

        // 続けるかどうかを確認
        std::cout << "\n次の操作を行いますか？ (y/n): ";
        char continueChoice;
        std::cin >> continueChoice;
        if (continueChoice != 'y' && continueChoice != 'Y') {
            break;  // 'y' または 'Y' 以外なら終了
        }
    }
    return 0;
}
