#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>

#define NOMINMAX
#include <limits> 

#ifdef _WIN32
#include <windows.h>
#else
#define SetConsoleTextAttribute(handle, color)
#endif

using namespace std;
using ushort = unsigned short;

bool IsValueNumber(const string&);
string CheckInputCorrectNumber(const string&);
ushort TryParseUShortInteger(const string&);
short TryParseShortInteger(const string&);
string CheckInputCorrectInteger(const string&);
bool IsIntegerValue(const string&);
void PrintJacquesFresco();
int** CreateMatrix(int, int);
void FillMatrixWithRandom(int**, int, int, int, int);
void PrintMatrix(const int* const*, int, int);
void FindElementsInAreas(const int* const*, ushort, ushort, short&, short&, pair<ushort, ushort>&, pair<ushort, ushort>&, bool&, bool&);
void FreeMatrix(int**, int);

void SwapElements(int**, pair<ushort, ushort>, pair<ushort, ushort>);

enum ConsoleColor {
    White = 7,
    Green = 10,
    Red = 12
};

void SetColor(ConsoleColor color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#else
    switch (color) {
    case Green: cout << "\033[32m"; break; // Green
    case Red: cout << "\033[31m"; break;   // Red
    default: cout << "\033[0m"; break;     // Reset
    }
#endif
}

void ResetColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, White);
#else
    cout << "\033[0m"; // Reset ANSI color
#endif
}

int main()
{
    setlocale(LC_ALL, "rus");

    const string OutputMessageLine = "Введите количество строк";
    const string OutputMessageRaws = "Введите количество столбцов";
    const string OutputMessageMinValueInMatrix = "Введите минимальное значение элемента в матрице";
    const string OutputMessageMaxValueInMatrix = "Введите максимальное значение элемента в матрице";

    string inputNumberForSizeMatrix, inputNumberForValueInMatrix;
    ushort cols, rows;
    short minValueMatrix, maxValueMatrix, maxNeg, minPos;
    bool IsSquareMatrix = false, isFoundMaxNeg = false, isFoundMinPos = false;
    bool IsMinValueLessMaxValue = false;

    do
    {
        do
        {
            inputNumberForSizeMatrix = CheckInputCorrectNumber(OutputMessageLine);

            cols = TryParseUShortInteger(inputNumberForSizeMatrix);

        } while (cols == 0);

        do
        {
            inputNumberForSizeMatrix = CheckInputCorrectNumber(OutputMessageRaws);

            rows = TryParseUShortInteger(inputNumberForSizeMatrix);

        } while (rows == 0);

        if (cols != rows)
            cout << "Матрица не является квадратной, попробуйте снова" << endl;
        else if (cols == rows)
            IsSquareMatrix = true;

    } while (IsSquareMatrix == false);

    do
    {
        do
        {
            inputNumberForValueInMatrix = CheckInputCorrectInteger(OutputMessageMinValueInMatrix);

            minValueMatrix = TryParseShortInteger(inputNumberForValueInMatrix);

        } while (minValueMatrix == 0);

        do
        {
            inputNumberForValueInMatrix = CheckInputCorrectInteger(OutputMessageMaxValueInMatrix);

            maxValueMatrix = TryParseShortInteger(inputNumberForValueInMatrix);

        } while (maxValueMatrix == 0);

        if (minValueMatrix > maxValueMatrix)
        {
            cout << "Загадка от жака фреско, почему минимальное значение больше максимального, на размышления дается 20 секунд" << endl;

            PrintJacquesFresco();
        }
        else
            IsMinValueLessMaxValue = true;

    } while (!IsMinValueLessMaxValue);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int** matrix = CreateMatrix(rows, cols);
    FillMatrixWithRandom(matrix, rows, cols, minValueMatrix, maxValueMatrix);

    PrintMatrix(matrix, rows, cols);

    maxNeg = numeric_limits<short>::min();
    minPos = numeric_limits<short>::max();
    pair<ushort, ushort> posMaxNeg = { -1, -1 };
    pair<ushort, ushort> posMinPos = { -1, -1 };

    FindElementsInAreas((const int* const*)matrix, rows, cols, maxNeg, minPos, posMaxNeg, posMinPos, isFoundMaxNeg, isFoundMinPos);

    if (isFoundMaxNeg) 
    {
        cout << "\nМаксимальный отрицательный элемент в зелёной области: " << maxNeg
             << " на позиции (" << posMaxNeg.first << ", " << posMaxNeg.second << ")" << endl;
    }
    else 
    {
        cout << "\nВ зелёной области нет отрицательных элементов." << endl;
    }

    if (isFoundMinPos) 
    {
        cout << "Минимальный положительный элемент в красной области: " << minPos
             << " на позиции (" << posMinPos.first << ", " << posMinPos.second << ")" << endl;
    }
    else 
    {
        cout << "В красной области нет положительных элементов." << endl;
    }

    if (isFoundMaxNeg && isFoundMinPos) 
    {
        SwapElements(matrix, posMaxNeg, posMinPos);

        cout << "\nЭлементы переставлены местами!" << endl;

        cout << "\nМатрица после перестановки:" << endl;

        PrintMatrix((const int* const*)matrix, rows, cols);
    }
    else {
        cout << "\nПерестановка невозможна, так как один или оба элемента не найдены." << endl;
    }

    FreeMatrix(matrix, rows);
}

void SwapElements(int** matrix, pair<ushort, ushort> pos1, pair<ushort, ushort> pos2)
{
    swap(matrix[pos1.first][pos1.second], matrix[pos2.first][pos2.second]);
}

void FreeMatrix(int** matrix, int size)
{
    for (int i = 0; i < size; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void PrintMatrix(const int* const* matrix, int rows, int cols)
{
    for (int i = 0; i < rows; ++i) 
    {
        for (int j = 0; j < cols; ++j) 
        {
            if (i < j && i + j < rows - 1) 
            {
                SetColor(Green);
                cout << matrix[i][j] << "\t";
                ResetColor();
            }
            else if (j > i && i + j > cols - 1) 
            {
                SetColor(Red);
                cout << matrix[i][j] << "\t";
                ResetColor();
            }
            else 
            {
                cout << matrix[i][j] << "\t";
            }
        }
        cout << endl;
    }
}

void FindElementsInAreas(const int* const* matrix, ushort rows, ushort cols, short& maxNeg, short& minPos, pair<ushort, ushort>& posMaxNeg, pair<ushort, ushort>& posMinPos, bool& foundMaxNeg, bool& foundMinPos)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (i < j && i + j < cols - 1)
            {
                if (matrix[i][j] < 0) {
                    if (!foundMaxNeg || matrix[i][j] > maxNeg) 
                    {
                        maxNeg = matrix[i][j];
                        posMaxNeg = { i, j };
                        foundMaxNeg = true;
                    }
                }
            }
            else if (j > i && i + j > cols - 1)
            {
                if (matrix[i][j] > 0) 
                {
                    if (!foundMinPos || matrix[i][j] < minPos) 
                    {
                        minPos = matrix[i][j];
                        posMinPos = { i, j };
                        foundMinPos = true;
                    }
                }
            }
        }
    }
}

void FillMatrixWithRandom(int** matrix, int rows, int cols, int minValue, int maxValue)
{
    srand(time(0));

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            matrix[i][j] = minValue + rand() % maxValue;
}

int** CreateMatrix(int rows, int cols)
{
    int** matrix = new int* [rows];

    for (int i = 0; i < rows; ++i)
        matrix[i] = new int[cols];

    return matrix;
}

short TryParseShortInteger(const string& value)
{
    try
    {
        int temp = stoi(value);

        if (temp < numeric_limits<short>::min() || temp > numeric_limits<short>::max())
            throw out_of_range("Значение вне диапазона short");

        return static_cast<short>(temp);
    }
    catch (const exception& ex)
    {
        cerr << "Ошибка: " << ex.what() << endl;
    }

    return 0;
}

string CheckInputCorrectInteger(const string& message)
{
    string inputCount;

    do
    {
        cout << message << ": ";

        getline(cin, inputCount);
    } while (!IsIntegerValue(inputCount));

    return inputCount;
}

bool IsIntegerValue(const string& value)
{
    if (value.empty()) return false;

    size_t start = 0;

    if (value[0] == '-') 
    {
        if (value.length() == 1) 
            return false;

        start = 1;
    }

    for (size_t i = start; i < value.length(); ++i)
    {
        if (!isdigit(static_cast<unsigned char>(value[i])))
            return false;
    }

    return true;
}

ushort TryParseUShortInteger(const string& value)
{
    try
    {
        unsigned int temp = stoi(value);

        if (temp > (int)numeric_limits<unsigned short>::max)
            throw out_of_range("Вы ввели слишком большое значение");

        return static_cast<unsigned short>(temp);
    } 
    catch (const exception& ex)
    {
        cerr << "Ошибка: " << ex.what() << endl;
    }

    return 0;
}

string CheckInputCorrectNumber(const string& message)
{
    string inputCount;

    do
    {
        cout << message + ": ";

        getline(cin, inputCount);

    } while (!IsValueNumber(inputCount));


    return inputCount;
}

bool IsValueNumber(const string& value)
{
    for (const auto& subnumbers : value)
    {
        if (!isdigit(subnumbers))
            return false;
    }

    return true;
}

void PrintJacquesFresco()
{
    cout << "          .-\"\"\"\"-." << endl;
    cout << "        .'        '." << endl;
    cout << "       /   O    O   \\" << endl;
    cout << "      :     __     :" << endl;
    cout << "      |   /  \\    |" << endl;
    cout << "      :  /    \\   :" << endl;
    cout << "       \\  `----'  /" << endl;
    cout << "        '.      .'" << endl;
    cout << "          '----'" << endl;
    cout << "          .----." << endl;
    cout << "         /      \\" << endl;
    cout << "        /  ####  \\" << endl;
    cout << "       /  ######  \\" << endl;
    cout << "      /  ########  \\" << endl;
    cout << "     /  ##########  \\" << endl;
    cout << "    /  ############  \\" << endl;
    cout << "   /  ##############  \\" << endl;
    cout << "  /  ################  \\" << endl;
    cout << " /  ##################  \\" << endl;
    cout << "/  ####################  \\" << endl;
    cout << "P.S. Сверху что-то типо Жака Фреско" << endl;
}