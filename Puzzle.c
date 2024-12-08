#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define EMPTY -1

bool isGameStarted = false;
bool isGameFinished = false;
int difficulty = -1;

bool isValidMove(int boardSize, int **board, int row, int col, int value);
bool checkSolution(int boardSize, int **board);
bool checkUniqueRowsAndColumns(int boardSize, int **board);

// Проверка уникальности строк и столбцов
bool checkUniqueRowsAndColumns(int boardSize, int **board)
{
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = i + 1; j < boardSize; j++)
        {
            bool rowsAreEqual = true;
            for (int k = 0; k < boardSize; k++)
            {
                if (board[i][k] != board[j][k])
                {
                    rowsAreEqual = false;
                    break;
                }
            }
            if (rowsAreEqual) return false;

            bool colsAreEqual = true;
            for (int k = 0; k < boardSize; k++)
            {
                if (board[k][i] != board[k][j])
                {
                    colsAreEqual = false;
                    break;
                }
            }
            if (colsAreEqual) return false;
        }
    }
    return true;
}

// Заполнение поля рандомно по правилам
int** generateSolvableBoard(int boardSize)
{
    int **board = malloc(boardSize * sizeof(int *));
    for (int i = 0; i < boardSize; i++)
    {
        board[i] = malloc(boardSize * sizeof(int));
    }

    bool isValid;
    do
    {
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                board[i][j] = rand() % 2;
            }
        }

        isValid = true;

        for (int i = 0; i < boardSize && isValid; i++)
        {
            for (int j = 2; j < boardSize; j++)
            {
                if (board[i][j] == board[i][j - 1] && board[i][j] == board[i][j - 2])
                {
                    isValid = false;
                    break;
                }
            }
        }

        for (int j = 0; j < boardSize && isValid; j++)
        {
            for (int i = 2; i < boardSize; i++)
            {
                if (board[i][j] == board[i - 1][j] && board[i][j] == board[i - 2][j])
                {
                    isValid = false;
                    break;
                }
            }
        }

        for (int i = 0; i < boardSize && isValid; i++)
        {
            int zeroCount = 0, oneCount = 0;
            for (int j = 0; j < boardSize; j++)
            {
                if (board[i][j] == 0) zeroCount++;
                else oneCount++;
            }
            if (zeroCount != boardSize / 2 || oneCount != boardSize / 2)
            {
                isValid = false;
                break;
            }
        }

        for (int j = 0; j < boardSize && isValid; j++)
        {
            int zeroCount = 0, oneCount = 0;
            for (int i = 0; i < boardSize; i++)
            {
                if (board[i][j] == 0) zeroCount++;
                else oneCount++;
            }
            if (zeroCount != boardSize / 2 || oneCount != boardSize / 2)
            {
                isValid = false;
                break;
            }
        }

        for (int i = 0; i < boardSize && isValid; i++)
        {
            for (int j = i + 1; j < boardSize; j++)
            {
                bool areRowsEqual = true;
                bool areColsEqual = true;

                for (int k = 0; k < boardSize; k++)
                {
                    if (board[i][k] != board[j][k]) areRowsEqual = false;
                    if (board[k][i] != board[k][j]) areColsEqual = false;
                }

                if (areRowsEqual || areColsEqual)
                {
                    isValid = false;
                    break;
                }
            }
        }

    } while (!isValid);

    return board;
}

// Удаление случайных ячеек в зависимости от сложности
bool removeCells(int boardSize, int **board, int difficulty)
{
    int cellsToRemove = (boardSize * boardSize) / 4 * difficulty;

    srand(time(NULL));
    for (int i = 0; i < cellsToRemove; i++)
    {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        if (board[row][col] != EMPTY)
        {
            board[row][col] = EMPTY;
        }
    }

    return true;
}

bool startGame(int *boardSize, int ***board, int difficulty)
{
    if (difficulty == -1)
    {
        printf("Ошибка: перед началом игры выберите сложность.\n");
        return false;
    }
    if (isGameStarted)
    {
        printf("Игра уже начата. Завершите текущую игру перед началом новой.\n");
        return false;
    }

    switch (difficulty)
    {
        case 1: *boardSize = 4; break;
        case 2: *boardSize = 6; break;
        default:
            printf("Ошибка: перед началом игры выберите сложность.\n");
            return false;
    }

    *board = malloc(*boardSize * sizeof(int *));
    if (*board == NULL)
    {
        printf("Ошибка выделения памяти.\n");
        return false;
    }

    for (int i = 0; i < *boardSize; i++)
    {
        (*board)[i] = malloc(*boardSize * sizeof(int));
        if ((*board)[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free((*board)[j]);
            }
            free(*board);
            *board = NULL;
            return false;
        }
    }

    *board = generateSolvableBoard(*boardSize);
    removeCells(*boardSize, *board, difficulty);

    isGameStarted = true;
    isGameFinished = false;
    printf("Игра началась на выбранной сложности!\n");
    return true;
}

bool freeBoard(int boardSize, int **board)
{
    if (board != NULL)
    {
        for (int i = 0; i < boardSize; i++)
        {
            if (board[i] != NULL)
            {
                free(board[i]);
            }
        }
        free(board);
        return true;
    }
    return false;
}

bool finishGame(int boardSize, int **board)
{
    if (board == NULL) return false;
    for (int i = 0; i < boardSize; i++)
    {
        if (board[i] != NULL)
        {
            free(board[i]);
            board[i] = NULL;
        }
    }
    free(board);
    board = NULL;
    isGameStarted = false;
    isGameFinished = true;
    return true;
}

bool initializeBoard(int boardSize, int **board)
{
    srand(time(NULL));

    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (rand() % 3 == 0)
                board[i][j] = EMPTY;
            else
                board[i][j] = rand() % 2;
        }
    }

    return true;
}

bool printBoard(int boardSize, int **board)
{
    if (board == NULL) return false;

    printf("\nТекущее состояние поля:\n");
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (board[i][j] == EMPTY)
                printf(". ");
            else
                printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    return true;
}

// Правила!!!
bool isValidMove(int boardSize, int **board, int row, int col, int value)
{
    // 1. Количество 0 и 1 в строке и столбце
    int countRow[2] = {0, 0};
    int countCol[2] = {0, 0};
    for (int i = 0; i < boardSize; i++)
    {
        if (board[row][i] != EMPTY) countRow[board[row][i]]++;
        if (board[i][col] != EMPTY) countCol[board[i][col]]++;
    }
    countRow[value]++;
    countCol[value]++;
    if (countRow[0] > boardSize / 2 || countRow[1] > boardSize / 2 || 
        countCol[0] > boardSize / 2 || countCol[1] > boardSize / 2)
    {
        return false;
    }

    // 2. Не более двух одинаковых чисел подряд
    if (col > 1 && board[row][col - 1] == value && board[row][col - 2] == value) return false;
    if (col < boardSize - 2 && board[row][col + 1] == value && board[row][col + 2] == value) return false;
    if (row > 1 && board[row - 1][col] == value && board[row - 2][col] == value) return false;
    if (row < boardSize - 2 && board[row + 1][col] == value && board[row + 2][col] == value) return false;

    return true;
}


bool gameLoop(int boardSize, int **board)
{
    int row, col, value;
    while (!isGameFinished)
    {
        if (!printBoard(boardSize, board)) return false;

        printf("Введите строку, столбец и значение (0 или 1) для заполнения (например, '1 2 0'): ");
        scanf("%d %d %d", &row, &col, &value);

        if (row < 1 || row > boardSize || col < 1 || col > boardSize || (value != 0 && value != 1))
        {
            printf("Некорректный ввод. Попробуйте снова.\n");
            continue;
        }

        row--; col--;
        if (board[row][col] != EMPTY)
        {
            printf("Эта ячейка уже заполнена. Выберите другую.\n");
            continue;
        }

        if (isValidMove(boardSize, board, row, col, value))
        {
            board[row][col] = value;
            printf("Ход принят.\n");

            if (checkSolution(boardSize, board))
            {
                printf("Поздравляем, вы решили головоломку!\n");
                isGameFinished = true;
            }
        }
        else
        {
            printf("Ход нарушает правила. Попробуйте снова.\n");
        }
    }
    return true;
}

int main()
{
    int **board = NULL;
    int boardSize = 0;
    bool isRunning = true;
    int difficulty = 0;

    while (isRunning)
    {
        printf("\n--- Меню ---\n");
        printf("1. Начать игру\n");
        printf("2. Выбор сложности\n");
        printf("3. Правила игры\n");
        printf("4. Выход\n");
        printf("Выберите опцию: ");

        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                if (startGame(&boardSize, &board, difficulty))
                {
                    if (!gameLoop(boardSize, board)) break;
                    finishGame(boardSize, board);
                }
                break;
            case 2:
                printf("Выберите сложность:\n");
                printf("1. Легкий уровень (4x4)\n");
                printf("2. Средний уровень (6x6)\n");
                printf("Введите ваш выбор: ");
                int difficultyChoice;
                scanf("%d", &difficultyChoice);

                if (difficultyChoice < 1 || difficultyChoice > 2)
                {
                    printf("Неверный выбор. Попробуйте снова.\n");
                }
                else
                {
                    difficulty = difficultyChoice;
                    printf("Сложность выбрана: ");
                    switch (difficulty)
                    {
                        case 1:
                            printf("Легкий уровень.\n");
                            break;
                        case 2:
                            printf("Средний уровень.\n");
                            break;
                    }
                }
                break;
            case 3:
                printf("Правила игры:\n");
                printf("1. Каждая строка и каждый столбец должны содержать одинаковое количество 1 и 0.\n");
                printf("2. Одна и та же цифра не может находиться более чем в двух ячейках подряд, что предотвращает слишком очевидные последовательности.\n");
                printf("3. Каждая строка и каждый столбец должны быть уникальными, то есть ни одна строка или столбец не могут полностью повторяться в пределах одного уровня.\n");
                printf("\nНажмите Enter для возврата в главное меню...\n");
                getchar();
                getchar();
                break;
            case 4:
                printf("Выход из игры...\n");
                isRunning = false;
                break;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
                break;
        }
    }
}

// Проверка решения
bool checkSolution(int boardSize, int **board)
{
    for (int i = 0; i < boardSize; i++)
    {
        int rowCount[2] = {0, 0};
        int colCount[2] = {0, 0};
        for (int j = 0; j < boardSize; j++)
        {
            if (board[i][j] != EMPTY) {
                if (board[i][j] != 0 && board[i][j] != 1)
                {
                    return false;
                }
                rowCount[board[i][j]]++;
            }
            if (board[j][i] != EMPTY)
            {
                if (board[j][i] != 0 && board[j][i] != 1) 
                {
                    return false;
                }
                colCount[board[j][i]]++;
            }
        }

        if (rowCount[0] != boardSize / 2 || rowCount[1] != boardSize / 2 ||
            colCount[0] != boardSize / 2 || colCount[1] != boardSize / 2)
        {
            return false;
        }
    }

    // Проверка, что нет двух одинаковых чисел подряд в строках и столбцах
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize - 2; j++)
        {
            if (board[i][j] != EMPTY &&
                board[i][j] == board[i][j + 1] &&
                board[i][j] == board[i][j + 2])
            {
                return false;
            }
            if (board[j][i] != EMPTY &&
                board[j][i] == board[j + 1][i] &&
                board[j][i] == board[j + 2][i])
            {
                return false;
            }
        }
    }

    // Проверка на уникальность строк и столбцов
    for (int i = 0; i < boardSize; i++) 
    {
        for (int j = i + 1; j < boardSize; j++) 
        {
            bool rowsEqual = true;
            bool colsEqual = true;
            for (int k = 0; k < boardSize; k++) 
            {
                if (board[i][k] != board[j][k]) 
                {
                    rowsEqual = false;
                }
                if (board[k][i] != board[k][j]) 
                {
                    colsEqual = false;
                }
            }

            if (rowsEqual || colsEqual) 
            {
                return false;
            }
        }
    }

    return true;
}