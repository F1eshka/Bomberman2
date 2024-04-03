#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h>

using namespace std;

enum MazeObject { HALL = 0, WALL = 1, WALLTWO = 2, WALLTHREE = 3, ENEMY = 4, BOMB = 5, HEALTH = 6 };
enum Color { DARKGREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15, DARKYELLOW = 6, DARKRED = 4, PURPUR = 13, GREEN = 10 };
enum KeyCode { ENTER = 13, ESCAPE = 27, SPACE = 32, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };

class Maze {
public:
    int width;
    int height;
    int** bomber = nullptr;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    void Generation() {//сделал генерацию для массива bomber
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                bomber[y][x] = rand() % 7;
            }
        }
    }
    Maze(int w, int h) : width(w), height(h) {
        bomber = new int* [height];
        for (int i = 0; i < height; i++) {
            bomber[i] = new int[width];
        }
        Generation();//метод находится в КСП для того чтоб генерация была доступна для всех мктодов класса
    }
    void Options() {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        // размер окна - 130х50 символов
        CONSOLE_CURSOR_INFO ci; // скрытие курсора
        ci.bVisible = false; // курсор не видно
        ci.dwSize = 100; // на самом деле, не важно что сюда писать - подходит любое значение от 1 до 100 (рамер курсора в процентах относительно клетки)
        SetConsoleCursorInfo(h, &ci); // применить настройки
    }
    //очистка памяти
    ~Maze() {
        FreeMemory();
    }
    void KeyBoard(int x, int y) {
        COORD position;
        position.X = x;
        position.Y = y;
    }
    //закрашиваем координаты
    void SetCursor(int x, int y, int color) {
        COORD position;
        position.X = x;
        position.Y = y;
        SetConsoleCursorPosition(h, position);
        SetConsoleTextAttribute(h, color);
    }

    //очистка памяти
    void FreeMemory() {
        for (int y = 0; y < height; y++) {
            delete[] bomber[y];
        }
        delete[] bomber;
    }
};

class Bomberman {
private:
    int x;
    int y;
    int health_person = 100;
    int count_of_bombs = 10;
    int using_bombs = 0;
    bool bomb;
    int bX;
    int bY;
    int** bomber = nullptr;
    int height;
    int width;

public:
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    void Person(int x, int y) {// Рисует персонажа в указанных координатах
        COORD position;
        position.X = x;
        position.Y = y;
        SetCursor(x, y, 12);
        cout << (char)1; // Можно использовать любой другой символ или строку для отображения персонажа

        while (true) {
            if (_kbhit()) {//нажатие было
                int code = _getch(); // Получаем код нажатой клавиши
                if (code == 224) {// Если нажата клавиша стрелки
                    code = _getch(); // Получаем расширенный код клавиши
                }
                // Стираем персонажа в текущей позиции
                SetConsoleCursorPosition(h, position);
                cout << " ";
                if (code == RIGHT && bomber[position.Y][position.X + 1] != WALL && bomber[position.Y][position.X + 1] != WALLTWO && bomber[position.Y][position.X + 1] != WALLTHREE) {// right 
                    position.X++; // изменение позиции ГГ вправо на 1 по иксу
                }
                else if (code == LEFT && bomber[position.Y][position.X - 1] != WALL && bomber[position.Y][position.X - 1] != WALLTWO && bomber[position.Y][position.X - 1] != WALLTHREE) {
                    position.X--;// изменение позиции ГГ влево на 1 по иксу            
                }
                else if (code == UP && bomber[position.Y - 1][position.X] != WALL && bomber[position.Y - 1][position.X] != WALLTWO && bomber[position.Y - 1][position.X] != WALLTHREE) {
                    position.Y--;// изменение позиции ГГ вверх на 1 по иксу          
                }
                else if (code == DOWN && bomber[position.Y + 1][position.X] != WALL && bomber[position.Y + 1][position.X] != WALLTWO && bomber[position.Y + 1][position.X] != WALLTHREE) {
                    position.Y++;// изменение позиции ГГ вниз на 1 по иксу
                }
                // Если нажата клавиша пробела
                else if (code == SPACE && bomber[position.Y][position.X + 1] != WALL && count_of_bombs != 0) {
                    bomb = true;
                    bX = position.X;
                    bY = position.Y;
                }

                else if (code == 13 && count_of_bombs != 0) {
                    if (bomb == true) {
                        SetCursor(bX, bY, PURPUR);
                        cout << (char)254;
                    }
                }

                else if (code == 103 && count_of_bombs != 0) {
                    if (bomb == true) {
                        count_of_bombs--;
                        COORD bomb_position;
                        bomb_position.X = bX;
                        bomb_position.Y = bY;

                        for (int i = -1; i <= 1; ++i) {
                            for (int j = -1; j <= 1; ++j) {
                                int newX = bomb_position.X + i;
                                int newY = bomb_position.Y + j;

                                if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                                    if (bomber[newY][newX] != WALL) {
                                        int a = rand() % 2 + 5;
                                        if (bomber[newY][newX] == WALLTHREE) {
                                            if (a == 5) {
                                                bomber[newY][newX] = BOMB;
                                                SetCursor(newX, newY, PURPUR);
                                                cout << (char)254;
                                            }
                                            if (a == 6) {
                                                bomber[newY][newX] = HEALTH;
                                                SetCursor(newX, newY, 14);
                                                cout << (char)3;
                                            }
                                        }
                                        else {
                                            bomber[newY][newX] = 0;
                                            SetCursor(newX, newY, 0);
                                            cout << ' ';
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                SetCursor(position.X, position.Y, 12);
                cout << (char)1;

                if (position.X == width - 2 and position.Y == height - 2) {
                    MessageBoxA(0, "You found your way out!!!!", "You win!!!", MB_OK);
                    break;
                }

                SetCursor(width + 1, 4, PURPUR);
                cout << "Hit points: ";
                cout << health_person;

                if (bomber[position.Y][position.X] == ENEMY) {
                    health_person -= 20;
                    bomber[position.Y][position.X] = 0;
                    SetCursor(width + 1, 4, PURPUR);
                    cout << "Hit points: ";
                    cout << health_person;
                    cout << " ";
                }

                if (bomber[position.Y][position.X] == HEALTH and health_person < 100) {
                    health_person += 20;
                    bomber[position.Y][position.X] = 0;
                    SetCursor(width + 1, 4, PURPUR);
                    cout << "Hit points: ";
                    cout << health_person;
                    cout << " ";
                }

                SetCursor(width + 1, 6, YELLOW);
                cout << "Number of bombs: ";
                cout << count_of_bombs;
                cout << " ";

                if (code == 103) {
                    SetCursor(width + 1, 6, YELLOW);
                    cout << "Number of bombs: ";
                    cout << count_of_bombs;
                    cout << " ";
                }

                if (bomber[position.Y][position.X] == BOMB and count_of_bombs < 10)
                {
                    count_of_bombs++;
                    bomber[position.Y][position.X] = 0;
                    SetCursor(width + 1, 6, YELLOW);
                    cout << "Number of bombs: ";
                    cout << count_of_bombs;
                    cout << " ";
                }

                if (health_person == 0) {
                    MessageBoxA(0, "You have run out of health!!!!", "You are dead!!!!", MB_OK);
                    break;
                }
            }

            else { // нажатия не было, двигаем врагов
                Sleep(15);
                // движение врагов
                COORD enemy_positions[100]; // массив который хранит координаты врагов
                int enemy_count = 0; // реальное количество врагов на данный момент после шага ГГ
                // перебор всех ячеек которые есть в лабиринте
                for (int y = 0; y < height; y++) {// перебор строк
                    for (int x = 0; x < width; x++) { // перебор столбцов
                        // если очередная ячейка - это враг
                        if (bomber[y][x] == MazeObject::ENEMY) {
                            enemy_positions[enemy_count].X = x;
                            enemy_positions[enemy_count].Y = y;
                            enemy_count++;
                        }
                    }
                }
                // перебор всех врагов
                for (int i = 0; i < enemy_count; i++) {
                    int r = rand() % 100; // 0-left, 1-right, 2-up, 3-down
                    if (r == LEFT &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALL &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALLTWO &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::ENEMY &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::BOMB &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::HEALTH &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALLTHREE) {
                        // стирание врага в старой позиции
                        COORD temp = enemy_positions[i];
                        SetConsoleCursorPosition(h, temp);
                        cout << " ";
                        bomber[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;
                        // перемещаем врага в новую позицию
                        temp.X = enemy_positions[i].X - 1;
                        temp.Y = enemy_positions[i].Y;
                        SetConsoleCursorPosition(h, temp);
                        SetConsoleTextAttribute(h, Color::RED);
                        cout << (char)224;
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] = MazeObject::ENEMY;
                    }

                    else if (r == RIGHT &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::WALL &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::WALLTWO &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::ENEMY &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::BOMB &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::HEALTH &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::WALLTHREE) {
                        // стирание врага в старой позиции
                        COORD temp = enemy_positions[i];
                        SetConsoleCursorPosition(h, temp);
                        cout << " ";
                        bomber[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;
                        // перемещаем врага в новую позицию
                        temp.X = enemy_positions[i].X + 1;
                        temp.Y = enemy_positions[i].Y;
                        SetConsoleCursorPosition(h, temp);
                        SetConsoleTextAttribute(h, Color::RED);
                        cout << (char)224;
                        bomber[enemy_positions[i].Y][enemy_positions[i].X + 1] = MazeObject::ENEMY;
                    }

                    else if (r == UP &&
                        bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::WALL &&
                        bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::WALLTWO &&
                        bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::ENEMY &&
                        bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::BOMB &&
                        bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::HEALTH &&
                        bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::WALLTHREE) {
                        // стирание врага в старой позиции
                        COORD temp = enemy_positions[i];
                        SetConsoleCursorPosition(h, temp);
                        cout << " ";
                        bomber[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;
                        // перемещаем врага в новую позицию
                        temp.X = enemy_positions[i].X;
                        temp.Y = enemy_positions[i].Y - 1;
                        SetConsoleCursorPosition(h, temp);
                        SetConsoleTextAttribute(h, Color::RED);
                        cout << (char)224;
                        bomber[enemy_positions[i].Y - 1][enemy_positions[i].X] = MazeObject::ENEMY;
                    }
                    else if (r == DOWN &&
                        bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::WALL &&
                        bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::WALLTWO &&
                        bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::ENEMY &&
                        bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::BOMB &&
                        bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::HEALTH &&
                        bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::WALLTHREE) {
                        // стирание врага в старой позиции
                        COORD temp = enemy_positions[i];
                        SetConsoleCursorPosition(h, temp);
                        cout << " ";
                        bomber[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;
                        // перемещаем врага в новую позицию
                        temp.X = enemy_positions[i].X;
                        temp.Y = enemy_positions[i].Y + 1;
                        SetConsoleCursorPosition(h, temp);
                        SetConsoleTextAttribute(h, Color::RED);
                        cout << (char)224;
                        bomber[enemy_positions[i].Y + 1][enemy_positions[i].X] = MazeObject::ENEMY;
                    }
                }
            }
        }
    }
};

class Enemy {
private:
    int height;
    int width;
    int** bomber = nullptr;
public:
    void Move() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int r = rand() % 5; // 0...9
                // если выпадает 0 - враг остаётся (10% случаев)
                if (r != 0) {// если выпадает не 0 (а число от 1 до 9) - врага убираем (вероятность 90%)
                    bomber[y][x] = 0; // убираем врага
                }
                if (bomber[y][x] == ENEMY) {
                    SetCursor(x, y, RED);
                    cout << (char)224;
                }
            }
        }
    }
};


 class Wall {
 private:
     int height;
     int width;
     int** bomber = nullptr;

 public:
    void Draw() {
         for (int y = 0; y < height; y++) {
             for (int x = 0; x < width; x++) {
                 if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                     bomber[y][x] = WALL;
                     SetCursor(x, y, 8);
                     cout << (char)178;
                 }
                 if (bomber[y][x] == 0) {//коридор
                     SetCursor(x, y, 0);
                 }
             }
         }
     }
 
        void WallsInsideTheMapOne(int x, int y) {
            HANDLE h = GetStdHandle(-11);
            COORD position;
            for (int j = 0; j < 1; j++) {
                for (int i = 0; i < 1; i++) {
                    position.X = x * 2 + i;
                    position.Y = y * 1 + j;
                    bomber[position.Y][position.X] = WALLTWO;
                    SetConsoleCursorPosition(h, position);
                    SetConsoleTextAttribute(h, 7);
                    cout << (char)178;
                }
            }
        }

        void AutomateWallNumberTwo() {
            for (int i = 2; i <= 28; i += 2) {// идем по каждой второй строке
                for (int j = 2; j <= 14; j += 2) {// идем по каждой второй колонке
                    WallsInsideTheMapOne(i, j); // вызываем метод вывода стены с номером два
                }
            }
        }

        void WallsInsideTheMapTwo() {
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++) {
                    int r = rand() % 10; // 0...9
                    if (r == 1) {// если выпадает не 0 (а число от 1 до 9) - врага убираем (вероятность 90%)
                        bomber[y][x] = 1; // убираем врага
                    }
                    if (bomber[y][x] == 1) {//стена
                        SetCursor(x, y, 3);
                        cout << (char)178;
                    }
                }
            }
        }

        void WallsInsideTheMapThree() {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int r = rand() % 10; // 0...9
                    if (r == 1 && bomber[y][x] != WALL) { // если выпадает не 0 (а число от 1 до 9) - врага убираем (вероятность 90%)
                        bomber[y][x] = WALLTHREE; // убираем врага
                    }
                    if (bomber[y][x] == WALLTHREE) {//стена
                        SetCursor(x, y, 7);
                        cout << (char)178;
                    }
                }
            }
        }
        //очистка памяти
        void FreeMemory() {
            for (int y = 0; y < height; y++) {
                delete[] bomber[y];
            }
            delete[] bomber;
        }
};

        class Menu {
        public:
            void Show() {
                system("cls");
                cout << " Bomberman " << endl;
                cout << "==============" << endl;
                cout << "1. Start game" << endl;
                cout << "2. Exit" << endl;
                cout << "==============" << endl;

                int choice;
                while (true) {
                    cin >> choice;
                    if (choice == 1 || choice == 2) {
                        break;
                    }
                    cout << "Invalid choice. Please enter 1 or 2." << endl;
                }

                if (choice == 1) {
                    // Start game
                }
                else {
                    // Exit
                    exit(0);
                }
            }
        };
};

int main() {
    srand(time(0)); 

  
    Menu menu;
    menu.Show();

   
    Maze maze(30, 16);

   
    Wall walls;
    walls.Draw();
    walls.AutomateWallNumberTwo();
    walls.WallsInsideTheMapTwo();
    walls.WallsInsideTheMapThree();

    Bomberman bomberman;
    bomberman.Person(2, 2);

    Enemy enemy;
    enemy.Move();

    while (true) {
        bomberman.Person(bomberman.position.X, bomberman.position.Y);


    }

    return 0;
}
