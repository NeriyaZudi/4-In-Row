/* Assignment: final project
Author: Neriya zudi, ID: 207073545
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include <conio.h>

typedef int* Row;//הגדרת קבועים מצביעים
typedef int** Board;

typedef enum { FALSE, TRUE }Boolean;
typedef enum Players {PLAYER_1,PLAYER_2}Players;//הגדרת קבועים לשחקנים

/* הצהרות על פונקציות*/
void free_board(Board board, int rows);
void print_board_index(int cols);
void print_board_recursively(Board board, int rows, int cols);
void print_row_recursively(Row row, int size);

Board build_board(int* rows, int* cols);
void start_game(Board board, int rows, int cols);
Boolean is_legal_position(Board board, int rows, int cols, int position, int* currRow);
void execute_move(Board board, int selected_row, int selected_col, Players player_turn);
Boolean Check_Winner(Board board, int rows, int cols,int RowPosition, int ColPosition);
Boolean check_4_row(Board board, int rows, int cols, int RowPosition, int ColPosition);
Boolean check_4_col(Board board, int rows, int cols, int RowPosition, int ColPosition);
Boolean check_4_MainDiagonal(Board board, int rows, int cols, int RowPosition, int ColPosition);
Boolean check_4_Diagonal(Board board, int rows, int cols, int RowPosition, int ColPosition);
Boolean check_tie(Board board, int rows, int cols);


void players_name_input(char* player_1, char* player_2);
void clean_buffer();


int main()
{
	printf("\n*** HELLO EWLCOME TO 4_IN_ROW GAME ***\n");
	int rows = 0, cols = 0;
	Board board = build_board(&rows, &cols);//בניית לוח משחק לפי גודל רצוי

	start_game(board, rows, cols);//ריצת המשחק

	print_board_recursively(board, rows, cols);//הדפסת לוח סופית
	free_board(board, rows);//שחרור זכרון

	return 0;
}

void free_board(Board board, int rows)
{
	int i = 0;

	for (i = 0; i < rows; i++)//שחרור השורות
		free(board[i]);

	free(board);//שחרור כל הלוח
}

void print_board_index(int cols)//הדפסת אינדקסים לשימוש השחקנים
{
	int i = 0;
	printf("Insert: ");
	for (i = 0; i < cols; i++)
		printf("%d  ", i);

	printf("\n");
}

void print_board_recursively(Board board, int rows, int cols)
{

	if (rows < 1)//תנאי עצירה להדפסה
		return;

	printf("\n\t");
	print_row_recursively(board[rows - 1], cols);//שליחה להדפסה כל השורות
	printf("\n");
	print_board_recursively(board, rows - 1, cols);//הדפסת העמודות ברקורסיה
}

void print_row_recursively(Row row, int size)
{
	if (size < 1)//תנאי עצירה להדפסה
		return;

	printf("%d  ", row[0]);//הדפסת שורה
	print_row_recursively(row + 1, size - 1);//שליחה ברקורסיה להדפסת כל השורות
}


Board build_board(int* rows, int* cols)
{
	int IndexRow = 0,i=0,j=0;
	Board board;//לוח משחק מסוג int**

	printf("\nPlease enter size of board(Rows X Cols)\n");
	while (*rows < 4 || *cols < 4)//כל עוד הגודל לא תקין גודל מינמלי 4 על 4
	{
		scanf("%d%d", rows, cols);//קליטת גודל מהמשתמש
		printf("Your game board contains rows = %d cols = %d\n", *rows, *cols);
		if (*rows < 4 || *cols < 4)//אם הגודל לא תקין
			printf("The minmum size of board is 4x4, please enter size of board again\n");
	}

	board = (Board)malloc((*rows)*sizeof(Row));//הקצאה דימנית עבור השורות
	if(!board)//בדיקת הקצאה
		return NULL;

	for (IndexRow = 0; IndexRow < *rows; IndexRow++)//הקצאה דינמית עבור העמודות
	{
		board[IndexRow] = (Row)malloc((*cols)*sizeof(int));
		if(!board[IndexRow])//בדיקת הקצאה
			return NULL;
	}
	
	for (i = 0; i < *rows; i++)//אתחול הלוח לאפסים
		for (j = 0; j < *cols; j++)
			board[i][j] = 0;

	return board;//החזרת הלוח
}

void start_game(Board board, int rows, int cols)
{
	char player_1[10], player_2[10];
	int row_position = 0, col_position = -1;
	Players player_turn = PLAYER_1;//תור שחקן
    players_name_input(player_1, player_2);
	printf("*****Game Start*****:\n");

	while (1)//ריצת המשחק בלולאה
	{
		system("cls");//ניקוי המסך הקודם
		printf("\n");
		print_board_index(cols);
		print_board_recursively(board, rows, cols);//הדפסת הלוח
		printf("%s (Player %d) turn:\n", !player_turn ? player_1 : player_2, player_turn + 1);//הדפסת הוראה לשחקן בתורו
		printf("Please enter  insert position (0-%d col)\n",cols-1);
		scanf("%d", &col_position);//קליטת מיקום עמודה 

		while (!is_legal_position(board, rows, cols, col_position, &row_position))//בדיקה שהקלט חוקי לא חורג מהגבול או שהשורה לא מלאה
			scanf("%d", &col_position);

		execute_move(board, row_position, col_position, player_turn);//הכנסת הערך של השחקן למיקום הרצוי
		
		if (Check_Winner(board, rows, cols, row_position, col_position))//בדיקת ניצחון
		{
			printf("**GAME OVER!**\nCongratulations!!!\n%s(Player %d) win!!!\n", !player_turn ? player_1 : player_2, player_turn + 1);
			break;//יציאה מהמשחק
		}
		else if (check_tie(board, rows, cols))//בדיקת תיקו
		{
			printf("**GAME OVER!**\nGoog game.Its a tie\n");
			break;//יציאה מהמשחק
		}

		
		player_turn = !player_turn; // החלפת שחקן
	}

}

Boolean is_legal_position(Board board, int rows, int cols, int position, int* currRow)
{//פונקציה לבדיקת חוקיות מיקום
	if (position < 0 || position > cols)//אם המיקום חורג מגבולות הלוח
	{
		printf("Invalid location, Please selected exist Insert\n");
		return FALSE;//החזרת שקר
	}

	for (*currRow = 0; *currRow < rows; (*currRow)++)//בדיקת מיקום הראשון שפנוי ושמירה כמצביע למיקום הזה
	{
		if (board[*currRow][position] == 0)
			break;
	}

	if (*currRow >= rows)//אם השורה מלאה
	{
		printf("FULL, Please selected valid Insert\n");
		return FALSE;
	}

	return TRUE;//אם הכל תקין מחזירה אמת וגם המיקום נשמר כמצביע כדי להכניס אותו למיקום המתאים בלוח
}

void execute_move(Board board, int selected_row, int selected_col, Players player_turn)
{
	board[selected_row][selected_col] = player_turn + 1;//הכנסה למיקום בלוח לפי תור השחקן
}


Boolean Check_Winner(Board board, int rows, int cols, int RowPosition, int ColPosition)
{
	if (check_4_row(board, rows, cols, RowPosition, ColPosition) ||
		check_4_col(board, rows, cols, RowPosition, ColPosition) ||
		check_4_MainDiagonal(board, rows, cols, RowPosition, ColPosition)||
		check_4_Diagonal(board, rows, cols, RowPosition, ColPosition))
		return TRUE;//אם יש 4 בשורה עמודה או אלכסון יש נצחון
	else
		return FALSE;//אם אין נצחון
}

Boolean check_4_row(Board board, int rows, int cols, int RowPosition, int ColPosition)
{
	int counter = 0, Selectedlocation = board[RowPosition][ColPosition];//מונה לספירת כמות הערכים הדומים ומשתנה ששומר את הערך במיקום שהשחקן הכניס
	while ((ColPosition < cols) && (board[RowPosition][ColPosition] == Selectedlocation) && (counter<4))
	{//כל עוד אין חריגה מגבולות הלוח וגם הערך במיקום מתאים לערכים השכנים וגם המונה קטן מרצף של 4
		ColPosition++;//המשך בדיקה במקומות מימין
		counter++;//ספירת ערך דומה
	}
	ColPosition = ColPosition-(counter+1);//החזרה למיקום המקורי
	
	while ((ColPosition >= 0) && (board[RowPosition][ColPosition] == Selectedlocation) && (counter < 4))
	{//כל עוד אין חריגה מגבולות הלוח וגם הערך במיקום מתאים לערכים השכנים וגם המונה קטן מרצף של 4
		ColPosition--;//המשך בדיקה במקומות משמאל
		counter++;//ספירת ערך דומה

	}
	if (counter >= 4)//אם יש לפחות 4 בשורה ברצף
		return TRUE;
	else
		return FALSE;


}
Boolean check_4_col(Board board, int rows, int cols, int RowPosition, int ColPosition)
{
	int counter = 0, Selectedlocation = board[RowPosition][ColPosition];
	while ((RowPosition < rows) && (board[RowPosition][ColPosition] == Selectedlocation) && (counter < 4))
	{//כל עוד אין חריגה מגבולות הלוח וגם הערך במיקום מתאים לערכים השכנים וגם המונה קטן מרצף של 4
		RowPosition++;//המשך בדיקה במקומות מלמעלה
		counter++;//ספירת ערך דומה
	}
	RowPosition = RowPosition - (counter + 1);//החזרה למיקום המקורי

	while ((RowPosition >= 0) && (board[RowPosition][ColPosition] == Selectedlocation) && (counter < 4))
	{//כל עוד אין חריגה מגבולות הלוח וגם הערך במיקום מתאים לערכים השכנים וגם המונה קטן מרצף של 4 
		RowPosition--;//המשך בדיקה במקומות מלמטה
		counter++;//ספירת ערך דומה

	}
	if (counter >= 4)//אם יש לפחות 4 בעמודה ברצף
		return TRUE;
	else
		return FALSE;



}
Boolean check_4_MainDiagonal(Board board, int rows, int cols, int RowPosition, int ColPosition)
{
	int counter = 0, Selectedlocation = board[RowPosition][ColPosition];
	
	while ((RowPosition < rows) && (ColPosition < cols) && (board[RowPosition][ColPosition] == Selectedlocation) && (counter < 4))
	{//כל עוד אין חריגה באלכסון מגבולות הלוח וגם הערך מתאים לערך של השחקן
		RowPosition++;//מעבר על האלכסון הראשי למעלה
		ColPosition++;
		counter++;
	}
	RowPosition = RowPosition - (counter + 1);//החזרה למיקום המקורי
	ColPosition = ColPosition - (counter + 1);
	while ((RowPosition >= 0) && (ColPosition >= 0) && (board[RowPosition][ColPosition] == Selectedlocation) && (counter < 4))
	{//כל עוד אין חריגה באלכסון מגבולות הלוח וגם הערך מתאים לערך של השחקן
		RowPosition--;//מעבר על האלכסון הראשי למטה
		ColPosition--;
		counter++;
	}
	if (counter >= 4)//אם יש 4 ברצף באלכסון
		return TRUE;
	else
		return FALSE;



}
Boolean check_4_Diagonal(Board board, int rows, int cols, int RowPosition, int ColPosition)
{
	int counter = 0, Selectedlocation = board[RowPosition][ColPosition];

	while ((RowPosition >= 0) && (ColPosition < cols) && (board[RowPosition][ColPosition] == Selectedlocation) && (counter < 4))
	{//כל עוד אין חריגה באלכסון מגבולות הלוח וגם הערך מתאים לערך של השחקן
		RowPosition--;
		ColPosition++;// מעבר על שאר האלכסונים הפחתת השורות והגדלת העמודות
		counter++;
	}
	RowPosition = RowPosition + (counter + 1);//החזרה למיקום המקורי
	ColPosition = ColPosition - (counter + 1);
	while ((RowPosition < rows) && (ColPosition >= 0) && (board[RowPosition][ColPosition] == Selectedlocation) && (counter < 4))
	{
		RowPosition++;//מעבר על שאר האלכסונים הגדלת השורות והפחתת העמודות
		ColPosition--;
		counter++;
	}
	if (counter >= 4)//אם יש 4 ברצף באחד מהאלכסונים
		return TRUE;
	else
		return FALSE;

}
Boolean check_tie(Board board, int rows, int cols)
{
	int i;//בדיקת תוצאה תיקו
	for (i = 0; i < cols; i++)
	{
		if (board[rows-1][i] == 0)//מעבר על שורה עליונה בלוח
			return FALSE;
	}
	return TRUE;//אם השורה מלמעלה כולה תפוסה ואין ניצחון אז המשחק נגמר בתיקו
	
}
void players_name_input(char* player_1, char* player_2)
{
	printf("Please enter the name of player 1\n");
	scanf("%s", player_1);//הכנסת שם שחקן 1
	clean_buffer();
	printf("Please enter the name of player 2\n");
	scanf("%s", player_2);//הכנסת שם שחקן 2
	clean_buffer();

	printf("player_1 = %s\n", player_1);
	printf("player_2 = %s\n", player_2);
}

void clean_buffer()//ניקוי זכרון
{
	while (getchar() != '\n');
}








