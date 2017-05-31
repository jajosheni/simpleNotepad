#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#define NOMINMAX
#define Width 100



//! @ AUTHOR  : SHENI HAMITAJ

//! instructions !//

void instructions()
{
    system("cls");
    puts("\t\t\t\t\t    This is a simple notepad Program");
    puts("\t\t\t\t\tBelow are the commands that you can use:\n\n");
    puts("\t\t\t\tKey\t\t | \t Explanation:");
    puts("\t\t\t\t_________________|_________________________________________________");
    puts("\t\t\t\tBackspace\t | \t Delete the character selected");
    puts("\t\t\t\tCTRL + s\t | \t SAVE the document");
    puts("\t\t\t\tCTRL + o\t | \t OPEN the document");
    puts("\t\t\t\tCTRL + e\t | ----- PRINT THIS MENU -----");
    puts("\t\t\t\tArrow Keys\t | \t Navigate through the text");
    puts("\t\t\t\t_________________|_________________________________________________");
    puts("\tScreen Color changes ->\tShift + 4 \t | \t Start text selecting, press again to stop");
    puts("\t\t\t\tCTRL + X\t | \t Cut Selection");
    puts("\t\t\t\tCTRL + C\t | \t Copy Selection");
    puts("\t\t\t\tCTRL + V\t | \t Paste Selection");
    puts("\t\t\t\t_________________|_________________________________________________");
    puts("\t\t\t\t  Note: mouse selection, copy and paste can be used too ");
    puts("\t\t\t\tMouse Left\t | \t Hold and Drag To select");
    puts("\t\t\t\tEnter\t\t | \t Copy Selection");
    puts("\t\t\t\tMouse Right\t | \t Paste Selection");
    puts("\t\t\t\t___________________________________________________________________");
    puts("\t\t\t\tYou can re-open this menu without effecting your text.");
    puts("\t\t\t\tYou may continue typing and this page will be deleted.");
}

const char * opath() /// get open path
{
  char filename[ MAX_PATH ];
  ZeroMemory( &filename, sizeof( filename ) );

  OPENFILENAME oFilename;

    ZeroMemory( &oFilename,      sizeof( oFilename ) );
    oFilename.lStructSize  = sizeof( oFilename );
    oFilename.lpstrFilter  = "Text File\0*.txt\0Any File\0*.*\0";
    oFilename.lpstrFile    = filename;
    oFilename.nMaxFile     = MAX_PATH;
    oFilename.lpstrTitle   = "Select a File";
    oFilename.Flags        = OFN_FILEMUSTEXIST;

    GetOpenFileNameA( &oFilename ); //! this is the command to get path from opening dialog


  if(strcmp(filename,"")!=0)
    {
        return filename;
    }
    else
    {
        printf(" You cancelled.");
        return "";

    }
}

const char * spath() /// get save path
{
   char filename[MAX_PATH];
   ZeroMemory( &filename, sizeof( filename ) );

   OPENFILENAME sFilename;

    ZeroMemory(&sFilename, sizeof(sFilename));
    sFilename.lStructSize = sizeof(sFilename);
    sFilename.lpstrFilter  = "Text file\0*.txt\0Any File\0*.*\0";
    sFilename.lpstrFile = filename;
    sFilename.nMaxFile = MAX_PATH;
    sFilename.lpstrTitle   = "Select a path to save your file";
    sFilename.lpstrDefExt="txt";

    GetSaveFileName(&sFilename); //! this is the command to get path from saving dialog

    if(strcmp(filename,"")!=0)
    {
        return filename;
    }
    else
    {
        printf(" You cancelled.");
        return "";

    }
}


struct List
{
    struct List *prev;
    char data;
    struct List *next;
};

struct List *First = NULL;
struct List *Last = NULL;

struct List *Global_ptr = NULL; //! determines cursor place

struct List *c_start = NULL; //! starting pointer for copy or cut
struct List *c_end = NULL;   //! ending pointer for copy or cut

struct List *F = NULL;      //!temporary list First
struct List *L = NULL;      //!temporary list Last

void find_direction() //! if - when selection is from end to beginning switch pointers
{
    int reversed=0;
    struct List *temp = (struct List*)malloc(sizeof(struct List));
    struct List *swap = (struct List*)malloc(sizeof(struct List));

    for (temp = c_end; temp!=NULL; temp=temp->next)
    {
        if(temp==c_start)
        {
            reversed ++;
        }

    }
    if(reversed>0)
    {
        swap=c_start;
        c_start=c_end;
        c_end=swap;
    }
}

void cut()
{
     if (c_start==NULL)
    {
        printf("\n\t please press $ where the text starts & press $ again where the text ends");
    }
    else
    {
        int del=0;
        int i;
        struct List *ptr = (struct List*)malloc(sizeof(struct List));
        for(ptr=c_start;ptr!=c_end; ptr=ptr->next)
        {
            temp_add(ptr->data);
            del++;
        }

        Global_ptr=c_end;

        for(i=0 ;i<del; i++)
        {
            end_delete();
            display();
        }
        set_xy(find_x()+1,find_y());
    }

    c_start=NULL;
}

void copy()
{
    if (c_start==NULL)
    {
        printf("\n\t please press $ where the text starts & press $ again where the text ends");
    }
    else
    {
        struct List *ptr = (struct List*)malloc(sizeof(struct List));
        for(ptr=c_start;ptr!=c_end; ptr=ptr->next)
        {
            temp_add(ptr->data);
        }
    }
    c_start=NULL;
}

void paste()
{
    if (F==NULL)
    {
        printf("\t nothing was copied");
    }
    else
    {
        struct List *c_ptr = (struct List*)malloc(sizeof(struct List));
        for(c_ptr = F; c_ptr != NULL; c_ptr=c_ptr->next)
        {
            end_add(c_ptr->data);
            display();
        }
    }
}

void temp_add(char c)  /// temporary Linked List addition
{
    struct List *newList = (struct List*)malloc(sizeof(struct List));
        newList->data = c;

        if(F==NULL)
        {
            F=newList;
            L=newList;

            F->prev=NULL;
            L->prev=F;
            F->next=L;
            L->next=NULL;
        }
        else
        {
            newList->next=NULL;
            newList->prev=L;
            L->next=newList;
            L=newList;
        }
}

void save_this()    //!save function
{
    const char* helper=spath();
        struct List *temp = (struct List*)malloc(sizeof(struct List));
        temp = First;

        char paths[199];
        strcpy(paths,helper);

        if(strcmp(paths,"")!=0)
        {

            FILE *p;
            p = fopen(paths,"w");

            if(p!=NULL)
            {
                printf(" \n\n Successfully saved");
                Sleep(500);
                display();
            }

            while(temp != NULL)
                {
                   fprintf(p,"%c",temp->data);
                   temp = temp->next ;
                }
            fclose(p);
        }
}

void open_this() //! open function
{
    const char* helper=opath();
    char patho[199];
    char read;
    strcpy(patho, helper);

    if(strcmp(patho,"")!=0)
    {
        FILE *fp;
        fp = fopen(patho, "r+");
        if (fp == NULL)
        {
            printf(" Couldn't open file");
        }
        else
        {
            First=NULL;
            while((read=fgetc(fp)) != EOF)
            {
                printf("%s", read);
            }

        }
        fclose(fp);
    }
}

void end_add(char c) //! add a character to the main text file
{
    if (Global_ptr!=Last)
    {
        middle_add(c);
    }
    else
    {
        struct List *newList = (struct List*)malloc(sizeof(struct List));
        newList->data = c;

        if(First==NULL)
        {
            First=newList;
            Last=newList;

            First->prev=NULL;
            Last->prev=First;
            First->next=Last;
            Last->next=NULL;
            Global_ptr=Last;
        }
        else
        {
            newList->next=NULL;
            newList->prev=Last;
            Last->next=newList;
            Last=newList;
            Global_ptr=Last;
        }

    }
}

void display() //! print the text
{
    system("cls");
    struct List *ptr = (struct List*)malloc(sizeof(struct List));
    for(ptr = First; ptr != NULL; ptr=ptr->next)
    {
        printf("%c", ptr->data);
    }
}

void end_delete() //! delete a character
{
    if (Global_ptr!=Last)
    {
        middle_del();
    }
    else
    {

        if(Last==First)
        {
            First=NULL;
            Global_ptr=Last;
        }

        else
        {
            Last->prev->next=NULL;
            Last = Last->prev;
            Global_ptr=Last;
        }
    }
}

void move_cursor(char m) //! move cursor's underline
{
    switch(m)
    {
    case 'l':
        if(find_x()==0)
        {
            if(find_y()>0){
                set_xy(Width,(find_y()-1));
                break;
            }
            else if(find_y()==0)
                break;

        }
        else
        {
            set_xy((find_x()-1),find_y());
            break;
        }
    case 'u':
        if(find_y()==0)
            break;
        set_xy(find_x(),(find_y()-1));
        break;
    case 'r':
        if(find_x()==Width)
        {
            set_xy(0,(find_y()+1));
            break;
        }
        else
        {
            set_xy((find_x()+1),find_y());
            break;
        }

    case 'd':
        set_xy(find_x(),(find_y()+1));
        break;
    }
    goto_ptr(find_x(),find_y());
}

void set_xy(int column, int line) //! set the cursor's underline based on the coordinates received
	{
		COORD coord;
    		coord.X = column;
    		coord.Y = line;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		if (!SetConsoleCursorPosition(hConsole, coord))
        {
            set_xy(0,(find_y()+1));
		}
		else
			SetConsoleCursorPosition(hConsole , coord);
	}

int find_x()  //! return the x coordinate
  {
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      if (!GetConsoleScreenBufferInfo(GetStdHandle( STD_OUTPUT_HANDLE ),&csbi))
        return -1;
      return csbi.dwCursorPosition.X;
  }

int find_y() //! return the y coordinate
  {
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      if (!GetConsoleScreenBufferInfo(GetStdHandle( STD_OUTPUT_HANDLE ),&csbi))
        return -1;
      return csbi.dwCursorPosition.Y;
  }

void middle_add(char m)  //! add character in the middle of the text
{
    struct List * newList = (struct List*)malloc(sizeof(struct List));
    struct List * ptr = (struct List*)malloc(sizeof(struct List));
    newList->data=m;

    ptr=Global_ptr;

        newList->next = ptr;
        newList->prev = ptr->prev;
        ptr->prev->next = newList;
        ptr->prev = newList;
}

void middle_del() //! delete a character from the middle of the text
{
    struct List * ptr = (struct List*)malloc(sizeof(struct List));
    ptr=Global_ptr;


    ptr->prev->next=ptr->next;
    ptr->next->prev=ptr->prev;
    Global_ptr=ptr->prev;
    free(ptr);
}

void goto_ptr(int x, int y)  //! VERY IMPORTANT !!! - Global Pointer always tracks coordinates and
{                                                 //! associates itself with linked lists corresponding to these coordinates
    int xc=0;
    int tempx=0;
    int yc=0;
    struct List * ptr = (struct List*)malloc(sizeof(struct List));
    for(ptr=First;ptr!=NULL;ptr=ptr->next)
    {
        Global_ptr=ptr;
        if(ptr->data!='\n')
        {
            xc++;
        }
        else
        {
            yc++;
            xc=0;
        }
        if(xc==x+1 && yc==y)
        {
            break;
        }
        if(yc==y+1) /// sometimes, when user clicks in empty space, this code
        {           ///             puts the global pointer in the right line
            tempx++;
            if(ptr->data=='\n' && tempx<x)
            {
                Global_ptr=ptr;
                break;
            }
        }
    }
}


int main()
{
    system("color 70");
    Global_ptr=Last;

    char e;
    int andtester;
    instructions();
    andtester=0;
    while(1)
    {
        e = getch();

        switch(e)
        {

        case 5:
            instructions();
            break;
        case 8:
            end_delete();
            display();
            break;
        case 13:
            end_add('\n');
            display();
            break;

    //! open !//
        case 15:
            open_this();
            display();
            break;
    //! save !//
        case 19:
            save_this();
            break;

        case 36: //!pointer to start and stop copying or cutting !//
            if(andtester % 2 == 0)
            {
                c_start = Global_ptr;
                F=NULL;
                andtester++;
                system("color 02");
                break;
            }
            else if(andtester % 2 == 1)
            {
                c_end = Global_ptr;
                andtester++;
                system("color 70");
                find_direction();
                break;
            }
        //! copy or Paste or CUT !//
        case 24:
            cut();
            break;

        case 3:
            copy();
            break;

        case 22:
            paste();
            break;


    //! arrows !//
        case -32:
            break;

        case 75:
                move_cursor('l');
                break;

        case 72:
                move_cursor('u');
                break;

        case 77:
                move_cursor('r');
                break;

        case 80:
                move_cursor('d');
                break;
    //! arrows end! //

        default:
            end_add(e);
            display();
            if (find_x() == Width) //! check if page width is exceeded , if yes put a new line
            {
                end_add('\n');
                display();
                break;
            }
            break;
        }
    }

    return 0;
}
