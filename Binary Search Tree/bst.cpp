#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <conio.h>
#include <string>
#include <cstring>
#include<time.h>
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#pragma warning(disable: 4996)

using std::fstream;
using std::cin;
using std::ios;
using std::string;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::istream;
using std::stringstream;
	
struct bst_node {
  int key;     // Key value
  long l;      // File offset of left child node
  long r;      // File offset of right child node
};

struct queue{  
       long value;  
       struct queue* next;  
       };



int main(int argc, char *argv[] )

{  
ofstream index_file;
ifstream index_readfile;
ifstream index_check;

static long double find_time=0;
static int find_count=0;
time_t start;
time_t end;
clock_t t;

system( "del file.bst" );


FILE * file;
file = fopen ("file.bst","a");
index_file.open("file.bst",ios::out|ios::binary);
index_readfile.open("file.bst",ios::in|ios::binary);

char buffer[1024];
string read_line;

//system( "del file.bst" );

while(!cin.eof())

{   cin.getline(buffer, 1024);
	read_line = buffer;
	char *command;
	char *index=NULL;
	char *index_read=NULL;
	


    command = strtok (buffer," ");
	string command_string;
	string index_string;
	
	command_string=command;


	if(command_string=="add")
	{

		index = strtok( NULL, " ");
       
	    int index_int;
	    index_int=atoi(index);

       	
     index_check.open("file.bst", ios::binary); // open your file
     index_check.seekg(0, ios::end); // put the "cursor" at the end of the file
     int check = index_check.tellg(); // find the position of the cursor
     index_check.close(); // close your file


			
		if((check==0))
		{   
		
		 
		index_file.seekp(0,ios::beg);   
		long pos = index_file.tellp();
		
		char *minusone = new char[2];
		minusone[0]='-';
		minusone[1]='1';
		long minusone_int = -1;

		index_file.write((const char *)&index_int,sizeof index_int);
        index_file.write((const char *)&minusone_int,sizeof minusone_int);
		index_file.write((const char *)&minusone_int,sizeof minusone_int);
	
		index_file.flush();
		}
		

    
		else
		{
      
	    char *minusone = new char[2];
		minusone[0]='-';
		minusone[1]='1';
		long minusone_int = -1;
		char *temp_index=new char[33];
		char *temp_l=new char[33];
		char *temp_r=new char[33];

		index_readfile.seekg(0);
		int pos1=index_readfile.tellg();

		int temp_index_int;

		index_readfile.read((char *)&temp_index_int,sizeof temp_index_int);
		
		 index_readfile.seekg(0, ios::end);
	     long  pos2 = index_readfile.tellg();
		 index_file.seekp(pos2,ios::beg);
		 index_readfile.seekg(4);

	    index_file.write((const char *)&index_int,sizeof index_int);
        index_file.write((const char *)&minusone_int,sizeof minusone_int);
		index_file.write((const char *)&minusone_int,sizeof minusone_int);


		while(true)

		{   
			 long temp_l_int;
		     long temp_r_int;
			
			int left_position=index_readfile.tellg();
			index_readfile.read((char *)&temp_l_int,sizeof temp_l_int);
			int right_position=index_readfile.tellg();
		    index_readfile.read((char *)&temp_r_int,sizeof temp_r_int);
			

			struct bst_node parent;
			parent.key=temp_index_int;
			parent.l=temp_l_int;
			parent.r=temp_r_int;

			if(index_int == temp_index_int)

			{
			
			break;
			
			}



            if(index_int<temp_index_int)
		  {

			  if(temp_l_int == -1)

			  {
						  
			  index_file.seekp(left_position,ios::beg);
			  char temp_pos2[33];
			  itoa(pos2,temp_pos2,10);
			  index_file.write((const char *)&pos2,sizeof pos2);
			  index_file.flush();
			  break;
			  }


		   index_readfile.seekg(temp_l_int, ios::beg);
		   index_readfile.read((char *)&temp_index_int,sizeof temp_index_int);  
	
		  }

		  else if(index_int>temp_index_int)


		  {
		  
			  if(temp_r_int == -1)

			  {
		     index_file.seekp(right_position,ios::beg);
			 
			  char temp_pos2 [33];
			    itoa(pos2,temp_pos2,10);
			    string temp_pos22 = temp_pos2;
			    index_file.write((const char *)&pos2,sizeof pos2);
			    index_file.flush();
			    break;
			  
			  
			  }

		   index_readfile.seekg(temp_r_int, ios::beg);
		   index_readfile.read((char *)&temp_index_int,sizeof temp_index_int);  
				  
		  }
		
		}
	    	
	
		}
	  
	}

	
	else if(command_string == "find")
        
	{
		
		
		
		t= clock();

		index = strtok( NULL, " ");
  
	int index_int=atoi(index);

    start=time(0);
	index_readfile.seekg(0,ios::beg);
	char *temp=new char[31];
	int temp_int;


    index_readfile.read((char *)&temp_int,sizeof(int));
		 long temp_l_int;
		 long temp_r_int;
			
	


	while(true)
	{
		    int left_position=index_readfile.tellg();
			index_readfile.read((char *)&temp_l_int,sizeof temp_l_int);
			int right_position=index_readfile.tellg();
		    index_readfile.read((char *)&temp_r_int,sizeof temp_r_int);
	
	if(temp_int==index_int)
	{
	 cout<<"Record "<<index_int<<" exists."<<'\n';
	 break;
	}

	else if(index_int<temp_int)

	{
		if(temp_l_int == -1)
		{
		
		cout<<"Record "<<index_int<<" does not exist."<<'\n';
		break;
		}

	 index_readfile.seekg(temp_l_int, ios::beg);
     index_readfile.read((char *)&temp_int,sizeof temp_int);  
	
	}
	
	else if(index_int>temp_int)

	{
	
		if(temp_r_int == -1)
		{
		
		cout<<"Record "<<index_int<<" does not exist."<<'\n';
		break;
		}

	 index_readfile.seekg(temp_r_int, ios::beg);
     index_readfile.read((char *)&temp_int,sizeof temp_int);
	
	
	}

            

	}
	      t=clock()-t;
	       find_time=find_time + (((long double)t)/CLOCKS_PER_SEC);
		   find_count++;
	}


         


	else if(command_string == "print")
    {
	    cout<<'\n';
		long temp_index_int=0;
long temp_l_int=0;
long temp_r_int=0;
		index_readfile.seekg(0,ios::beg);

index_readfile.seekg(0,ios::end);
 long temp_end=index_readfile.tellg();
 const long end= temp_end/12;




long* q= new long [end];
int rear=-1,front=-1;
index_readfile.seekg(4,ios::beg);
q[0]=0;
  int nodesInCurrentLevel = 1;
  int nodesInNextLevel = 0;
int i=1;
int prev=1;
while(front<=rear)
{
	
	
	
	
	if(front==-1)
	{
	front=0;
	}

    index_readfile.seekg(q[front],ios::beg);
	index_readfile.read((char *)&temp_index_int,sizeof(long));
	if(nodesInCurrentLevel==prev)
	{cout <<i<<':'<<' ';}
 nodesInCurrentLevel--;
	std::cout<<temp_index_int<<"/"<<q[front]<<" ";
	 
	front++;
                                 
	index_readfile.read((char *)&temp_l_int,sizeof(long));
	index_readfile.read((char *)&temp_r_int,sizeof(long));
  
	if(rear==-1)
	{
	rear=0;
	
	}

	if(temp_l_int!=-1)
	{
	q[++rear]=temp_l_int;
	 nodesInNextLevel++;
	
	
	}

	if(temp_r_int!=-1)
	{
	q[++rear]=temp_r_int;
	nodesInNextLevel++;	
	}

    if (nodesInCurrentLevel == 0) {
      cout << '\n';
      nodesInCurrentLevel = nodesInNextLevel;
      nodesInNextLevel = 0;
	    prev=nodesInCurrentLevel;
	  i++;
    }

}




	 }










    else if(command_string == "end")
    {
	    index_readfile.close();
		index_file.close(); 
double avg=find_time/find_count;
cout<<'\n';
cout<<"Sum:"<<' '<<find_time<<'\n';
cout<<"Avg:"<<' '<<avg<<'\n';


	
	}

	else

	{
	 
	cout<<"Wrong Command!!!";
	}


	
	
	
} 
}