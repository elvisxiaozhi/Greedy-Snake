#include <iostream>
#include <vector>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <conio.h> //kbhit()

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

vector<int> row;
vector<int> col;
vector<int> food_x;
vector<int> food_y;

void welcome_interface();
void set_position(int, int);
void init_snake();
void show_snake();
void show_food();
void show_length();
int get_direction();
void change_direction(int);
bool is_food_eaten();
bool check_fail();
bool play_again();
bool pause_game();
int food_disappear(int);
void moving_speed(int);

int main()
{
    srand(time(NULL));
    welcome_interface();
    system("cls");
    for(int y = 0; y < 21; y++) {
        set_position(y, 60);
        cout << "|";
    }
    for(int x = 0; x < 59; x++) {
        set_position(20, x);
        cout << "--";
    }
    set_position(22, 60);
    system("color E");
    cout << "Created by Theodore";
    init_snake();
    show_snake();
    show_food();
    int dir = get_direction(), count_time = 0, old_size = row.size();
    while(true) {
        if(check_fail() == true) {
            set_position(22, 25);
            cout << "Game Over!" << endl;
            if(play_again() == true) {
                dir = 2; //set default to go down
                set_position(23, 0);
                cout << "                                   ";
            }
            else {
                break;
            }
        }
        if(is_food_eaten() == true) {
            show_food();
        }
        if(kbhit()) {
            dir = get_direction();
        }
        change_direction(dir);
        show_length();
        show_snake();
        count_time = food_disappear(count_time);
        moving_speed(old_size);
    }
    return 0;
}

void welcome_interface()
{
    set_position(8, 25);
    cout << "Welcome to play greedy snake" << endl;
    set_position(10, 25);
    cout << "Created by Theodore" << endl;
    set_position(12, 25);
    cout << "Press any key to continue" << endl;
    getch();
}

void set_position(int y, int x)
{
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void init_snake()
{
    //snake head: row[2] = 3, col[2] = 3;
    for(int i = 1; i < 4; i++) {
        row.push_back(i);
        col.push_back(3);
    }
}

void show_snake()
{
    for(int i = 0; i < row.size(); i++) {
        if(i == row.size() - 1) {
            set_position(row[i], col[i]);
            cout << "@";
        }
        else{
            set_position(row[i], col[i]);
            cout << "*";
        }
    }
}

void show_food()
{
    int flag = 0;
    int size = 0;
    while(flag == 0) {
        int x = rand() % 58 + 1, y = rand() % 19 + 1;
        food_x.push_back(x), food_y.push_back(y);
        for(int i = 0; i < row.size(); i++) {
            if(row[i] != y && col[i] != x) {
                size++;
            }
            if(size == row.size()) {
                set_position(food_y[0], food_x[0]);
                cout << "#";
                flag = 1;
            }
        }
    }
}

void show_length()
{
    set_position(10, 65);
    cout << "Length: " << row.size();
}

int get_direction()
{
    if(kbhit()) {
        switch(getch()) {
        case 224:
            switch(getch()) {
            case 72: //up
                return 1;
                break;
            case 80: //down
                return 2;
                break;
            case 75: //left
                return 3;
                break;
            case 77: //right
                return 4;
                break;
            }
            break;
        case 'p': //pause
            return 5;
            break;
        case 'P': //pause
            return 5;
            break;
        }
    }
    else {
        return 0;
    }
}

void change_direction(int dir)
{
    if(dir == 0) { //default go down
        row.push_back(row[row.size() - 1] + 1);
        col.push_back(col[col.size() - 1]);
        set_position(row[0], col[0]);
        cout << " ";
        row.erase(row.begin());
        col.erase(col.begin());
    }
    if(dir == 1) { //up
        row.push_back(row[row.size() - 1] - 1);
        col.push_back(col[col.size() - 1]);
        set_position(row[0], col[0]);
        cout << " ";
        row.erase(row.begin());
        col.erase(col.begin());
        set_position(21, 27);
        cout << "      " << endl;
        set_position(22, 17);
        cout << "                            " << endl;
    }
    if(dir == 2) { //down
        row.push_back(row[row.size() - 1] + 1);
        col.push_back(col[col.size() - 1]);
        set_position(row[0], col[0]);
        cout << " ";
        row.erase(row.begin());
        col.erase(col.begin());
        set_position(21, 27);
        cout << "      " << endl;
        set_position(22, 17);
        cout << "                            " << endl;
    }
    if(dir == 3) { //left
        row.push_back(row[row.size() - 1]);
        col.push_back(col[col.size() - 1] - 1);
        set_position(row[0], col[0]);
        cout << " ";
        row.erase(row.begin());
        col.erase(col.begin());
        set_position(21, 27);
        cout << "      " << endl;
        set_position(22, 17);
        cout << "                            " << endl;
    }
    if(dir == 4) {//right
        row.push_back(row[row.size() - 1]);
        col.push_back(col[col.size() - 1] + 1);
        set_position(row[0], col[0]);
        cout << " ";
        row.erase(row.begin());
        col.erase(col.begin());
        set_position(21, 27);
        cout << "      " << endl;
        set_position(22, 17);
        cout << "                            " << endl;
    }
    if(dir == 5) {//pause
        set_position(21, 27);
        cout << "Paused" << endl;
        set_position(22, 17);
        cout << "Press any key to continue..." << endl;
    }
}

bool is_food_eaten()
{
    if(row.back() == food_y[0] && col.back() == food_x[0]) {
        row.insert(row.begin(), food_y[0]);
        col.insert(col.begin(), food_x[0]);
        food_x.clear();
        food_y.clear();
        return true;
    }
}

bool check_fail()
{
    for(int i = 0; i < row.size() - 1; i++) {
        if(row.back() == row[i] && col.back() == col[i]) {
            return true;
        }
    }
    if(row.back() == 0 || row.back() == 20 || col.back() == 0 || col.back() == 59) {
        return true;
    }
}

bool play_again()
{
    cout << "Do you want to play it again? (y/n) ";
    if(getch() == 'y' || getch() == 'Y') { //delete snake death position
        for(int i = 0; i < row.size(); i++) {
            if(i == row.size() - 1) {
                set_position(row[i], col[i]);
                cout << " ";
            }
            else{
                set_position(row[i], col[i]);
                cout << " ";
            }
        }
        for(int x = 0; x < 59; x++) { //fix the broken border
            set_position(20, x);
            cout << "--";
        }
        row.clear();
        col.clear();
        init_snake();
        show_snake();
        show_length();
        return true;
    }
}

int food_disappear(int count_time)
{
    count_time++;
    if(count_time == 50) {
        count_time = 0;
        if(row.size() >= 20) {
            set_position(food_y[0], food_x[0]);
            cout << " ";
            food_x.clear();
            food_y.clear();
            show_food();
        }
    }
    return count_time;
}

void moving_speed(int old_size)
{
    int speed = 200, minus_speed = 3;
    if(row.size() >= 50) {
        minus_speed = 150;
    }
    else {
        minus_speed *= row.size() - old_size;
    }
    Sleep(speed - minus_speed);
}
