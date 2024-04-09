#include <iostream>
#include <ctime>
#include <windows.h>
#include <conio.h> 
using namespace std;

enum MazeObject { HALL = 0, WALL = 1, WALLTWO = 2, WALLTHREE = 3, ENEMY = 4, BOMB = 5, HEALTH = 6 };
enum Color { DARKGREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15, DARKYELLOW = 6, DARKRED = 4, PURPLE = 13, GREEN = 10 };
enum KeyCode { ENTER = 13, ESCAPE = 27, SPACE = 32, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };

// Класс приложения
class Application {
public:
    static HANDLE h;
    static CONSOLE_CURSOR_INFO ci;

    static void Start() {
        MoveWindow(GetConsoleWindow(), 50, 50, 1200, 500, true);
        system("title Bomberman");
        srand(time(NULL));

        h = GetStdHandle(STD_OUTPUT_HANDLE);
        ci.bVisible = false;
        ci.dwSize = 100;
        SetConsoleCursorInfo(h, &ci);
    }
    Application() = delete;
};

HANDLE Application::h;
CONSOLE_CURSOR_INFO Application::ci;

class Menu {
    const int NUM_MENU_ITEMS = 3;
    int ActiveMenuItem = 0; // Выбранный пункт меню
    int ch = 0; // Хранение нажатой клавиши
    bool exit = false; // для выхода из цикла

public:
    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    // Само меню основа
    void Menushka() {
        Application::ci.bVisible = true;
        SetConsoleCursorInfo(Application::h, &Application::ci);

        system("color F0");
        system("cls");

        while (!exit) {
            ShowMenu();
            gotoxy(0, ActiveMenuItem);

            ch = _getch();
            if (ch == 224)
                ch = _getch();

            switch (ch) {
            case 27: // Стрелка вверх
                exit = true;
                break;
            case 72: // Стрелка вверх
                ActiveMenuItem = (ActiveMenuItem - 1 + NUM_MENU_ITEMS) % NUM_MENU_ITEMS;
                break;
            case 80: //Стрелка вниз
                ActiveMenuItem = (ActiveMenuItem + 1) % NUM_MENU_ITEMS;
                break;
            case 13: // Клавиша Энтер
                if (ActiveMenuItem == 0) {
                    system("cls"); // !!!!!!!!!!!!!!!!!!!
                    cout << "NEW GAME\n";
                    Sleep(1000);
                    return;
                }
                else if (ActiveMenuItem == 1) { // Об авторе кнопка
                    AboutAuthors();
                }
                else if (ActiveMenuItem == 2) { // Кнопка выход из игры
                    ::exit(0);
                }
                break;
            }
        }
    }

    // Вывод лого
    void ShowLogo() {
        gotoxy(50, 15);
        cout << "BOMBERMAN!!!" << "\n";
        Sleep(1000);
    }

    // Вывод меню
    void ShowMenu() {
        system("cls");
        cout << "Start game" << "\n";
        cout << "About authors" << "\n";
        cout << "Exit" << "\n";
    }

    // Про нас
    void AboutAuthors() {
        system("cls");
        cout << "Bienoieva Malika" << "\n" << "Lolo Mukhammed\n\n\n\n";
        system("pause");
    }
};


class Maze {
public:
    int width;
    int height;
    int** layout;

    // Конструктор, инициализирующий лабиринт указанными размерами
    Maze(int w, int h) : width(w), height(h) {
        layout = new int* [height];
        for (int i = 0; i < height; i++) {
            layout[i] = new int[width];
        }
    }

    // Деструктор для освобождения памяти
    ~Maze() {
        FreeMemory();
    }

    // Метод для генерации раскладки лабиринта
    void GenerateLayout() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                layout[y][x] = rand() % 7;
            }
        }
    }

    // Метод для освобождения памяти, выделенной под раскладку лабиринта
    void FreeMemory() {
        for (int y = 0; y < height; y++) {
            delete[] layout[y];
        }
        delete[] layout;
    }
};

// Класс, представляющий стены на карте лабиринта
class Wall {
    int width;
    int height;
    int** bomber = nullptr;

public:
    HANDLE h;

    Wall() {}

    // Конструктор, инициализирующий параметры стен
    Wall(HANDLE handle, int width, int height) : h(handle), width(width), height(height) {
        bomber = new int* [height];
        for (int i = 0; i < height; i++) {
            bomber[i] = new int[width];
        }
    }

    // Метод для отрисовки внешних стен лабиринта
    void DrawWalls(int** layout, int width, int height) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                    layout[y][x] = WALL;
                    SetCursor(x, y, 8);
                    cout << (char)178;
                }
                if (layout[y][x] == 0) {
                    SetCursor(x, y, 0);
                }
            }
        }
    }

    // Позиции курсора на экране консоли с цветом
    void SetCursor(int x, int y, int color) {
        COORD position;
        position.X = x;
        position.Y = y;
        SetConsoleCursorPosition(h, position);
        SetConsoleTextAttribute(h, color);
    }

    // Метод для отрисовки стен внутри лабиринта
    void WallsInsideTheMapOne(int x, int y) {
        HANDLE h = GetStdHandle(-11);
        COORD position;
        if (bomber != nullptr) {
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
    }

    // Метод для автоматизации генерации стен
    void AutomateWallNumberTwo() {
        for (int i = 2; i <= 28; i += 2) {
            for (int j = 2; j <= 14; j += 2) {
                WallsInsideTheMapOne(i, j);
            }
        }
    }

    // Метод для отрисовки стен внутри лабиринта
    void WallsInsideTheMapTwo(int** layout, int width, int height) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int r = rand() % 10;
                if (r == 1) {
                    layout[y][x] = 0;
                }
                if (layout[y][x] == 1) {
                    SetCursor(x, y, 3);
                    cout << (char)178;
                }
            }
        }
    }

    // Метод для отрисовки стен внутри лабиринта 
    void WallsInsideTheMapThree(int** layout, int width, int height) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int r = rand() % 10;
                if (r == 1 && layout[y][x] != WALL) {
                    layout[y][x] = WALLTHREE;
                }
                if (layout[y][x] == WALLTHREE) {
                    SetCursor(x, y, 7);
                    cout << (char)178;
                }
            }
        }
    }

    // Метод для освобождения памяти, выделенной под раскладку стен
    void FreeMemory() {
        for (int y = 0; y < height; y++) {
            delete[] bomber[y];
        }
        delete[] bomber;
    }
};

// Класс врагов
class Enemy {
public:
    HANDLE h;

    Enemy(HANDLE handle) : h(handle) {}

    // Метод для генерации врагов на карте
    void GenerateEnemies(int** layout, int width, int height) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int r = rand() % 5;
                if (r != 0) {
                    layout[y][x] = 0;
                }
                if (layout[y][x] == ENEMY) {
                    SetCursor(x, y, RED);
                    cout << (char)224;
                }
            }
        }
    }

    // Метод для установки позиции курсора на экране консоли с заданным цветом
    void SetCursor(int x, int y, int color) {
        COORD position;
        position.X = x;
        position.Y = y;
        SetConsoleCursorPosition(h, position);
        SetConsoleTextAttribute(h, color);
    }
};

// Класс Бомбермен
class BombermanGame {
public:
    HANDLE h;
    Wall wall;
    int** bomber = nullptr;
    int count_of_bombs = 10;
    int using_bombs = 0;
    bool bomb = false;
    int bX = 0;
    int bY = 0;
    int health_person = 100;
    int height;
    int width;

    BombermanGame() {}

    // Метод для генерации начального состояния игры
    void Generation() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                bomber[y][x] = rand() % 7;
            }
        }
    }

    // Конструктор для инициализации игры
    BombermanGame(int w, int h) : width(w), height(h) {
        bomber = new int* [height];
        for (int i = 0; i < height; i++) {
            bomber[i] = new int[width];
        }
        Generation();
    }

    // Метод для перемещения игрока
    void Person(int x, int y) { // Рисует персонажа в указанных координатах
        COORD position;
        position.X = x;
        position.Y = y;
        wall.SetCursor(x, y, 12);
        cout << (char)1; // Можно использовать любой другой символ или строку для отображения персонажа

        while (true) {
            if (_kbhit()) {
                int code = _getch();
                if (code == 224) {
                    code = _getch();
                }
                SetConsoleCursorPosition(h, position);
                cout << " ";
                if (code == RIGHT && position.X < width - 1 && bomber[position.Y][position.X + 1] != WALL && bomber[position.Y][position.X + 1] != WALLTWO && bomber[position.Y][position.X + 1] != WALLTHREE) {
                    position.X++;
                }
                else if (code == LEFT && position.X > 0 && bomber[position.Y][position.X - 1] != WALL && bomber[position.Y][position.X - 1] != WALLTWO && bomber[position.Y][position.X - 1] != WALLTHREE) {
                    position.X--;
                }
                else if (code == UP && position.Y > 0 && bomber[position.Y - 1][position.X] != WALL && bomber[position.Y - 1][position.X] != WALLTWO && bomber[position.Y - 1][position.X] != WALLTHREE) {
                    position.Y--;
                }
                else if (code == DOWN && position.Y < height - 1 && bomber[position.Y + 1][position.X] != WALL && bomber[position.Y + 1][position.X] != WALLTWO && bomber[position.Y + 1][position.X] != WALLTHREE) {
                    position.Y++;
                }
                else if (code == SPACE && bomber[position.Y][position.X + 1] != WALL && count_of_bombs != 0) {
                    bomb = true;
                    bX = position.X;
                    bY = position.Y;
                }
                else if (code == 13 && count_of_bombs != 0) {
                    if (bomb == true) {
                        wall.SetCursor(bX, bY, PURPLE);
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
                                                wall.SetCursor(newX, newY, PURPLE);
                                                cout << (char)254;
                                            }
                                            if (a == 6) {
                                                bomber[newY][newX] = HEALTH;
                                                wall.SetCursor(newX, newY, 14);
                                                cout << (char)3;
                                            }
                                        }
                                        else {
                                            bomber[newY][newX] = 0;
                                            wall.SetCursor(newX, newY, 0);
                                            cout << ' ';
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                wall.SetCursor(position.X, position.Y, 12);
                cout << (char)1;
                if (position.X == width - 2 and position.Y == height - 2) {
                    MessageBoxA(0, "You found your way out!!!!", "You win!!!", MB_OK);
                    break;
                }
                wall.SetCursor(width + 1, 4, PURPLE);
                cout << "Hit points: ";
                cout << health_person;

                if (bomber[position.Y][position.X] == ENEMY) {
                    health_person -= 20;
                    bomber[position.Y][position.X] = 0;
                    wall.SetCursor(width + 1, 4, PURPLE);
                    cout << "Hit points: ";
                    cout << health_person;
                    cout << " ";
                }
                if (bomber[position.Y][position.X] == HEALTH && health_person < 100) {
                    health_person += 20;
                    bomber[position.Y][position.X] = 0;
                    wall.SetCursor(width + 1, 4, PURPLE);
                    cout << "Hit points: ";
                    cout << health_person;
                    cout << " ";
                }
                wall.SetCursor(width + 1, 6, YELLOW);
                cout << "Number of bombs: ";
                cout << count_of_bombs;
                cout << " ";


                if (code == 103) {
                    wall.SetCursor(width + 1, 6, YELLOW);
                    cout << "Number of bombs: ";
                    cout << count_of_bombs;
                    cout << " ";
                }
                if (bomber[position.Y][position.X] == BOMB && count_of_bombs < 10)
                {
                    count_of_bombs++;
                    bomber[position.Y][position.X] = 0;
                    wall.SetCursor(width + 1, 6, YELLOW);
                    cout << "Number of bombs: ";
                    cout << count_of_bombs;
                    cout << " ";
                }
                if (health_person == 0) {
                    MessageBoxA(0, "You have run out of health!!!!", "You are dead!!!!", MB_OK);
                    break;
                }
            }
            else {
                Sleep(15);
                COORD enemy_positions[100];
                int enemy_count = 0;
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        if (bomber[y][x] == MazeObject::ENEMY) {
                            enemy_positions[enemy_count].X = x;
                            enemy_positions[enemy_count].Y = y;
                            enemy_count++;
                        }
                    }
                }
                for (int i = 0; i < enemy_count; i++) {
                    int r = rand() % 100;
                    if (r == LEFT &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALL &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALLTWO &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::ENEMY &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::BOMB &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::HEALTH &&
                        bomber[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALLTHREE) {
                        COORD temp = enemy_positions[i];
                        SetConsoleCursorPosition(h, temp);
                        cout << " ";
                        bomber[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;
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
    void KeyBoard(int x, int y) {
        COORD position;
        position.X = x;
        position.Y = y;
    }
};

int main() {
    Application::Start();

    Menu menu;
    menu.ShowLogo();
    menu.Menushka();

    Maze maze(61, 17);
    maze.GenerateLayout();
    system("title Bomberman");

    Enemy enemy(GetStdHandle(STD_OUTPUT_HANDLE));
    enemy.GenerateEnemies(maze.layout, maze.width, maze.height);

    Wall wall(GetStdHandle(STD_OUTPUT_HANDLE), maze.width, maze.height);
    wall.DrawWalls(maze.layout, maze.width, maze.height);
    wall.AutomateWallNumberTwo();
    wall.WallsInsideTheMapTwo(maze.layout, maze.width, maze.height);
    wall.WallsInsideTheMapThree(maze.layout, maze.width, maze.height);

    BombermanGame bomber;
    bomber.Person(2, 2);
}
