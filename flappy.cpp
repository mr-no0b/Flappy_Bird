//2107065
//Constructor, destructor, friend function, opt overloading, inheritance, polymorphism, virtual function, file management, stl, abstract class
//inline parameterized constructor, templates, encapsulation

#include<iostream>
#include<conio.h>
#include<direct.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>
#include <time.h>
#include<fstream>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define N 100
using namespace std;

class pipe;
class bird;
class screencontrol;
class gameControl;

template<typename p>  //Using Template
class myArray{
    p * arr=new p[N];
public:
    myArray(){}
    ~myArray(){
        delete arr;     //Destructor
    }
    p &operator [](int x){ //Operator Overloading
        return *(arr+x);
    }

};
class motherClass{
protected:
    float speeed;
    int High_Speed;
public:
    motherClass(){High_Speed=0;}
    void gotoxy(int x, int y);
    friend void instructions(gameControl*); //Friend Function
    virtual void display()=0; //polymorphism

};
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
void motherClass::gotoxy(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
class pipe:virtual public motherClass{ //Virtual Class
protected:
    myArray<int> pipePos;
    int gapPos[3];
    int pipeFlag[3];
public:
    void genPipe(int);
    void drawPipe(int);
    void erasePipe(int);
    void display(){
        cout<<"Inside Pipe class";
    }
};

void pipe::genPipe(int ind){
	gapPos[ind] = 3 + rand()%14;
}
void pipe::drawPipe(int ind){
	if( pipeFlag[ind] == true ){
        int x=pipePos[ind];
		for(int i=0; i<gapPos[ind]; i++){
			gotoxy(WIN_WIDTH-pipePos[ind],i+1); cout<<"***";
		}
		for(int i=gapPos[ind]+GAP_SIZE; i<SCREEN_HEIGHT-1; i++){
			gotoxy(WIN_WIDTH-pipePos[ind],i+1); cout<<"***";
		}
	}
}
void pipe::erasePipe(int ind){
	if( pipeFlag[ind] == true ){
		for(int i=0; i<gapPos[ind]; i++){
			gotoxy(WIN_WIDTH-pipePos[ind],i+1); cout<<"   ";
		}
		for(int i=gapPos[ind]+GAP_SIZE; i<SCREEN_HEIGHT-1; i++){
			gotoxy(WIN_WIDTH-pipePos[ind],i+1); cout<<"   ";
		}
	}
}


class bird:virtual public motherClass{
protected:

    char bird[2][6] = { '/','-','-','o','\\',' ',
					'|','_','_','_',' ','>' };
    int birdPos = 6;
public:
    void drawBird();
    void eraseBird();
    void display(){
        cout<<"Inside Bird class";
    }
};

void bird::drawBird(){
	for(int i=0; i<2; i++){
		for(int j=0; j<6; j++){
			gotoxy(j+2,i+birdPos); cout<<bird[i][j];

		}
	}
}
void bird::eraseBird(){
	for(int i=0; i<2; i++){
		for(int j=0; j<6; j++){
			gotoxy(j+2,i+birdPos); cout<<" ";
		}
	}
}

class screencontrol:virtual public motherClass{
public:
    screencontrol(){}
    screencontrol(int x){
        speeed=100-x;
    }
    void setcursor(bool visible, size_t size);
    void drawBorder();
    void display(){
        cout<<"Inside ScreenControl class";
    }
};
void screencontrol::setcursor(bool visible, size_t size)
{
	if(size == 0)
		size = 3;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}

void screencontrol::drawBorder(){

	for(int i=0; i<SCREEN_WIDTH; i++){
		gotoxy(i,0); cout<<":";
		gotoxy(i,SCREEN_HEIGHT); cout<<":";
	}

	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(0,i); cout<<":";
		gotoxy(SCREEN_WIDTH,i); cout<<":";
	}
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(WIN_WIDTH,i); cout<<":";
	}
}






class gameControl:public bird, public pipe, public screencontrol{
protected:

    int score;
    int High_Score=0;
public:
    gameControl(){score=0; High_Score=0;}
    gameControl(int x):screencontrol(x){score=0;} //Inline parameterized constructor
    void display(){}
    int collision();
    void gameover();

    void updateScore();
    void play();
};
int gameControl::collision(){
	if( pipePos[0] >= 61  ){
		if( birdPos<gapPos[0] || birdPos >gapPos[0]+GAP_SIZE ){
			return 1;
		}
	}
	return 0;
}

void gameControl::gameover(){
	system("cls");
	cout<<endl;
	cout<<"\t\t--------------------------"<<endl;
	cout<<"\t\t-------- Game Over -------"<<endl;
	cout<<"\t\t--------------------------"<<endl<<endl;
	cout<<"\t\tYour Score: "<<score<<endl;
	cout<<"\t\tPress any key 3 times to go back to menu.";
	speeed=100;
	getch();
	getch();
	getch();

}
void gameControl::updateScore(){
	gotoxy(WIN_WIDTH + 7, 6);cout<<"Score: "<<score<<endl;
	gotoxy(WIN_WIDTH+6, 9); cout<<"----------"<<endl;
	gotoxy(WIN_WIDTH+7, 12); cout<<"Speed: "<<(int)(100-speeed)<<endl;
	gotoxy(WIN_WIDTH+6, 15); cout<<"----------"<<endl;
	High_Score=max(High_Score, score);
	gotoxy(WIN_WIDTH+5, 18); cout<<"High Score: "<<High_Score<<endl;
    //gotoxy(WIN_WIDTH+6, 20); cout<<"----------"<<endl;
}

void instructions(gameControl *ptr){

	system("cls");
	cout<<"Instructions";
	cout<<"\n----------------";
	cout<<"\n Press spacebar to make bird fly";
	cout<<"\n Your primary speed is "<<ptr->speeed;
	cout<<"\n\nPress any key to go back to menu";
	getch();
}

void gameControl::play(){

	birdPos = 6;
	score = 0;
	pipeFlag[0] = 1;
	pipeFlag[1] = 0;
	pipePos[0] = pipePos[1] = 4;

	system("cls");
	drawBorder();
	genPipe(0);
	updateScore();


	gotoxy(10, 5);cout<<"Press any key to start";
	getch();
	gotoxy(10, 5);cout<<"                      ";

    float here=0;
    ofstream outf; outf.open("out.txt", ios::app);
	while(1){

		if(kbhit()){
			char ch = getch();
			if(ch==32){
				if( birdPos > 3 )
					birdPos-=3+here;
			}
			if(ch==27){
				break;
			}
		}

		drawBird();
		drawPipe(0);
		drawPipe(1);
		if( collision() == 1 ){
            outf<<"\nScore: "<<score;
            outf.close();
            gameover();
			return;
		}
		Sleep(speeed);
		eraseBird();
		erasePipe(0);
		erasePipe(1);
		birdPos += 1;

		if( birdPos > SCREEN_HEIGHT - 2 ){
            outf<<"\nScore: "<<score;
            outf.close();
			gameover();
			return;
		}
		if( birdPos < 3 ){
            outf<<"\nScore: "<<score;
            outf.close();
			gameover();
			return;
		}

		if( pipeFlag[0] == 1 )
			pipePos[0] += 2;

		if( pipeFlag[1] == 1 )
			pipePos[1] += 2;

		if( pipePos[0] >= 40 && pipePos[0] < 42 ){
			pipeFlag[1] = 1;
			pipePos[1] = 4;
			genPipe(1);
		}
		if( pipePos[0] > 68 ){
			score++;
			updateScore();
			pipeFlag[1] = 0;
			pipePos[0] = pipePos[1];
			gapPos[0] = gapPos[1];
		}
		if(speeed){
            speeed-=0.2;
            here+=0.01;
		}

	}


}

int main()
{
    gameControl *p=new gameControl(0);
	p->setcursor(0,0);
	srand( (unsigned)time(NULL));

	do{
		system("cls");
		string flappyBird=" |      FLAPPY BIRD       | ";  //STL
		p->gotoxy(10,5); cout<<" -------------------------- ";
		p->gotoxy(10,6); cout<<flappyBird;
		p->gotoxy(10,7); cout<<" --------------------------";
		p->gotoxy(10,9); cout<<"1. Game";
		p->gotoxy(10,10); cout<<"2. Instructions";
		p->gotoxy(10,11); cout<<"3. Quit";
		p->gotoxy(10,13); cout<<"Your option: ";
		char op = getch();

		if( op=='1') p->play();
		else if( op=='2') instructions(p);
		else if( op=='3') exit(0);

	}while(1);

	return 0;
}
