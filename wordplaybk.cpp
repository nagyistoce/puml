#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>
#include <algorithm>

using namespace std;

class word
{
        string WORD;
        string SCRAMBLED;
        public:
         int compare(string answer);
         void scramble();               //scrambles WORD and stores in SCRAMBLED
         void setstring(string str);
         char *getWORD();               //retrieves WORD
         char *getSCRAMBLED();  //retrieves SCRAMBLED
};

class player
{
        int DIFFICULTY;
        int Gamemode;
        public:
         void turn();
         void setgamemode(int mode);
         void freeplay();
         void madminute();
	 void setdifficulty(string input);
         int getdifficulty();
};


class wordlist
{
	vector <word> words_easy;			//holds easy words
	vector <word> words_medium;			//holds medium words
	vector <word> words_hard;			//holds hard words
	public: 
	 void pushword(word *tmp, int difficulty);	//pushes new word into its corresponding difficulty vector
	 int grabword(player &curr_player);			//randomly grabs word out of specified difficulty vector
	 word *getwordclass(int difficulty, int index);
};


class file
{
	public:
	  void readfile(wordlist &list);
};

class menu
{
	public:
	 void display(player &curr_player);
	 void quit();
	 void start(player &curr_player);
 	 void gamemode();
 	 int setdifficulty(string input);
	 void setinput(string input);
	 char *getinput();
	 void setgamemode(int mode);
};

int player::getdifficulty()
{
	return DIFFICULTY;
}

void player::setgamemode(int mode)
{
	Gamemode=mode;
}

void menu::display(player &curr_player)
{
        string input;
	bool valid_input;
        cout<<"\nWelcome to Word Play.\n\n*Type \"$quit\" any time to end game. \n*To return to this screen in game type \"$main\" .\n*To view highscores type \"highscores\"\n*if you give up on a word type \"$giveup\"\n*To start, type \"start\""<<endl;
        do
        {
                valid_input=1;                  //set value to assume correct input, later set to 0 if bad input is detected
                cout<<"\n>";
                cin>>input;
		if(input=="start")
                {
                       start(curr_player);
                }
                else if(input=="$quit")
                {
                       quit();               //quit program
                }
                else if(input=="highscores")
                {
                        //yet to come
                }
                else
                {
                        cout<<"You chose none of the options\n\n"<<endl;
                        valid_input=0;                  //resets loop
                }
        }while(valid_input==0);              //repeats if input was not recognized
	
}

void menu::start(player &curr_player)
{
	bool valid_input;
	string input;
 	do
 	{     
               valid_input=1;		//assumes valid input until proven otherwise
               cout<<"\n\n Pick a game mode:\n\n 1 Mad Minute\n 2 Free play\n\n>"<<endl;
               cin>>input;
               if(input=="1" || input=="Mad Minute")
               {
			curr_player.setgamemode(1);
               }
               else if(input=="2" || input=="Free play")
               {
			curr_player.setgamemode(2);
               }
               else
               {
                     cout<<"\nPlease pick a game mode\n"<<endl;
                     valid_input=0;	//flag for invalid input
               }
               cout<<"\n\nSet difficulty 1-3 (3 being hardest)>";
               cin>>input;
               if(input>="1" && input<="3")
               {
                      curr_player.setdifficulty(input);                  //set difficulty
               }
               else
               {
		     cout<<"\nInvalid input\n"<<endl;
                     valid_input=0;     //flag for invalid input

               }   
        }while(valid_input==0);

}
void player::setdifficulty(string input)
{
	DIFFICULTY=atoi(input.c_str());
}

void menu::quit()
{
	exit(0);
}

int wordlist::grabword(player &curr_player)
{
        int word_index, difficulty;
        string str;
        srand(time(0));
	difficulty=curr_player.getdifficulty();
        if(difficulty==1)
        {
                int vector_size=(int)words_easy.size();				//get vector size
                word_index = rand() % vector_size;                              //randomly indexes word in specified difficulty by moding random with vector size
                words_easy[word_index].scramble();                              //calls scramble on the word chosen
	}
	if(difficulty==2)
        {
                int vector_size=(int)words_medium.size();                         //get vector size
                word_index = rand() % vector_size;                              //randomly indexes word in specified difficulty by moding random with vector size
                words_medium[word_index].scramble();                              //calls scramble on the word chosen
        }
	if(difficulty==3)
        {
                int vector_size=(int)words_hard.size();                         //get vector size
                word_index = rand() % vector_size;                              //randomly indexes word in specified difficulty by moding random with vector size
                words_hard[word_index].scramble();                              //calls scramble on the word chosen
        }

	return word_index;
	
}

void wordlist::pushword(word *tmp, int difficulty)
{
	if(difficulty==1)
	{
		words_easy.push_back(*tmp);
	}
	else if(difficulty==2)
	{
		words_medium.push_back(*tmp);
	}
	else if(difficulty==3)
	{
		words_hard.push_back(*tmp);
	}
}
/*
int player::freeplay(wordlist words, int index, int difficulty)
{
	 time_t start, end;
         double dif;
	 word *tmp=new word;
	 int solved=0;		
         string input, scrambled_word, word;
	 time(&start);
	while(solved==0)			//while word remains unsolved keep excecuting loop
	{ 
		tmp=words.getwordclass(index, difficulty);              //returns pointer to current word
                scrambled_word=tmp->getSCRAMBLED();                     //stores scrambled word to var
                cout<<scrambled_word<<">";
                cin>>input;

		if(input=="$quit")			//quit program
        	 {
        	      exit(0);
       	 	 }
	 	 else if(input=="$main")
	 	 {
	 	     //settings(0);
	 	 }
		else if(input=="$giveup")				//if user gives up then tell them the word and move out of function
		{
			word=tmp->getWORD();
			cout<<"\n\nThe word was \""<<word<<"\"\n"<<endl;
			return 0;		
		}
		solved=tmp->compare(input);				//compare() tests input to word and returns 1 or 0 to solved
		cout<<endl;
	}
	 time(&end);
         dif = difftime (end,start);				//takes time differnce from start and answer 
	 word=tmp->getWORD();					//grab final word
         cout<<"GREAT!\n\n<<Time: "<<dif<<" seconds to solve for \""<<word<<"\""<<endl<<endl;
	
}*/
word *wordlist::getwordclass(int index,int difficulty)
{
	if(difficulty==1)
	{
		return &words_easy[index];
	}
	if(difficulty==2)
        {
                return &words_medium[index];
        }
	if(difficulty==3)
        {
                return &words_hard[index];
        }

}	

int word::compare(string answer)
{
	if(answer==WORD)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

char *word::getWORD()
{
	char *cstr;
	cstr = new char [WORD.size()+1];
        strcpy (cstr, WORD.c_str());
	return cstr;
}

char *word::getSCRAMBLED()
{
        char *cstr;
        cstr = new char [SCRAMBLED.size()+1];
        strcpy (cstr, SCRAMBLED.c_str());
        return cstr;
}

void word::scramble()
{
	int random_index, old_index, word_size;
	char temp;
	string str;
	srand( time(0) );
	word_size=WORD.size();
	for(int swaps=0; swaps<10; swaps++)				//Swaps the elements n number of times
	{
		old_index = random_index = rand() % word_size;		//set random integer that less than or equal to string size
		temp=SCRAMBLED[random_index];				//randomly pick character and store in temp
		random_index = rand() % word_size;			//randomly pick another character
		SCRAMBLED[old_index]=SCRAMBLED[random_index];		//perform swap on both elements...
		SCRAMBLED[random_index]=temp;					
	}
	
}

void file::readfile(wordlist &list)
{
        ifstream infile ("list.txt");
      	string str;
        while(!infile.eof())
        {
	        word *tmp=new word;	
	        infile >> str;
		tmp->setstring(str);						//put newly read string into newly created word

		if(str.size() <= 1)
		{
			//skip words that only have 1 letter or less
		}
	        else if(str.size() < 5)                                             //if size of word is less than 5 then put it in the easy difficulty vector
                {
                         list.pushword(tmp, 1);                                         //stores most recent read word into easy vector
                }
                else if(str.size() >= 5 && str.size() < 8)                 //if size of word is in between 5 and 8 letters then put it in the intermediate difficulty vector
                {
                 	 list.pushword(tmp, 2);						//stores most recent read word into medium vector
                }
                else if(str.size() >= 8)                                            //if size or word is over 8 letters long put it in hard difficulty vector
                {
                    	 list.pushword(tmp, 3);                                         //stores most recent read word into hard vector
                }
	}
}

void word::setstring(string str)
{
	transform(str.begin(), str.end(), str.begin(), ::tolower);			//convert string to lowercase
	WORD=str;
	SCRAMBLED=str;
}

int main()
{
	int difficulty, word_index;
	file wordfile;	
	wordlist words;					//words of "wordlist type" holds 3 vectors (one for each difficulty)
	menu Main;
	player Player_1;
	Main.display(Player_1);
	wordfile.readfile(words);			//read words from file and store in vector 
	while(1)
	{
	
   	        word_index=words.grabword(Player_1);			//returns an index to newly scrambled word
//		Player_1.turn(words, word_index);			//take user answer	
	}
	
}


