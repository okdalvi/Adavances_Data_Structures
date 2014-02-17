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

	
struct f_rec {	// Financial record structure
   int    num;	// Transaction number
   float  amt;	// Amount of the transaction
   char   type;	// Transaction type (D=debit, C=credit)
   int    acct;	// Account used for transaction
   int    hour;	// Hour of transaction (0-23)
   int    min;	// Minute of transaction (0-59)
   int    day;	// Day of transaction (0-364)
   int    year;	// Year of transaction (0000-9999)
};


	
struct ch_node {	// Chained hash table node
  int   k;	        // Key of record node represents
  long  rec;	    // Offset of corresponding record in database file
  long  next;	    // Offset of next node in this chain (-1 for none)
}; 


int main(int argc, char *argv[] )

{ 

ifstream database_read;
ofstream database_write;
ifstream index_read;
ofstream index_write;


string indexfile,dbfile;
indexfile=argv[1];
dbfile=argv[2];

char* index_char;
char* db_char;

index_char = (char*)indexfile.c_str();
db_char = (char*)dbfile.c_str();

FILE * file;
file = fopen (index_char,"a");

FILE * file1;
file1 = fopen (db_char,"a");

database_write.open(db_char,ios::out|ios::binary);
database_read.open(db_char,ios::in|ios::binary);


index_write.open(index_char, ios::out|ios::binary);
index_read.open(index_char, ios::in|ios::binary);

int i;
ch_node  node;
//  If file empty, append 1001 nodes with a key=-1
long minusone_long=-1;
int minusone_int=-1;


index_read.seekg( 0, ios::end );

int end=index_read.tellg();

if ( end == 0) {

index_write.seekp( 0, ios::beg );
  node.k = minusone_int;
  node.rec= minusone_long;
  node.next= minusone_long;

  for( i = 0; i < 1001; i++ ) 
  {
    index_write.write( (char *) &node, sizeof( ch_node ) );
	index_write.flush();
  }

}



char buffer[1024];
string read_line;

while(!cin.eof())

{   
	
	cin.getline(buffer, 1024);
	read_line = buffer;
	char *command;
    command = strtok (buffer," ");
	string command_string;
	command_string=command;
	int  num;
	float amt;
	char*   type;	// Transaction type (D=debit, C=credit)
	int     acct;	// Account used for transaction
	int    hour;	// Hour of transaction (0-23)
	int     min;	// Minute of transaction (0-59)
	int     day;	// Day of transaction (0-364)
	int     year;	// Year of transaction (0000-9999)





	if(command_string=="add")
	{
	  

		int db_location;

		num = atoi(strtok( NULL, " "));
          amt = atof(strtok( NULL, " "));
          type = strtok( NULL, " ");
       	  acct = atoi(strtok( NULL, " "));
    	  hour = atoi(strtok( NULL, " "));
		  min = atoi(strtok( NULL, " "));
          day = atoi(strtok( NULL, " "));
	      year = atoi(strtok( NULL, " "));
          char type_char=type[0];
	   
	 struct ch_node checker;
	  int hash_value1=num%1001;
	  int position1 = ((hash_value1)* (sizeof(ch_node)));
	  int temp_k1=0;
	  long temp_rec1=0;
	  long temp_next1=0;
	  boolean exists=false;
	  	
    do
	
	{
	
	index_read.seekg(position1,ios::beg); 
	index_read.read((char*)&temp_k1,sizeof(int));
	index_read.read((char*)&temp_rec1,sizeof(long));
	index_read.read((char*)&temp_next1,sizeof(long));
		if(num==temp_k1)

		{
		exists=true;
		break;
		}

		position1=temp_next1;
	
	
	}
	while(position1!=-1);
		  
		  
		 if(exists==false) 
		 {	  
		  
	  struct f_rec* database=new f_rec;
	  database->num=num;
      database->amt=amt;
	  database->type=type_char;
	  database->acct=acct;
	  database->hour=hour;
	  database->min=min;
	  database->day=day;
	  database->year=year;
	//  cout<<"Size of f_rec:"<<sizeof(f_rec)<<'\n';
	  



	  database_write.seekp(0,ios::end);

	  db_location=database_write.tellp();
	  database_write.write((char*)&database,sizeof(f_rec));
	//  cout<<"Record"<<num<<"Written at location:"<<db_location<<'\n';
      database_write.flush();



	  
	  int hash_value=num%1001;
	  int position = ((hash_value)* (sizeof(ch_node)));
	  int temp_k=0;
	  long temp_rec=0;
	  long temp_next=0;

	  struct ch_node temp_index; //temp index to read value at position
	
     index_read.seekg(position,ios::beg); 
	 int pos=index_read.tellg();
	// cout<<pos<<'\n';
	  
index_read.read((char*)&temp_k,sizeof(int));
index_read.read((char*)&temp_rec,sizeof(long));

index_read.read((char*)&temp_next,sizeof(long));


	


	   if(temp_k==-1 & temp_next==-1)

	  {

       index_write.seekp(position,ios::beg);
	   struct ch_node* index=new ch_node; //temp index to write

	   index->k=num;
	   index->rec=db_location;
	   index->next=-1;

	   index_write.write((char*)&num,sizeof(int));
	   index_write.write((char*)&db_location,sizeof(long));
	   index_write.write((char*)&minusone_long,sizeof(long));
	   index_write.flush();

	   }

	   else

	   {
	   
// Append to end of index file

		   index_write.seekp(0,ios::beg);
		      index_write.seekp(0,ios::end);
	   long next_index=index_write.tellp();
	//   next_index=next_index-32;

	  struct ch_node* index=new ch_node;
	  index->k=num;
	  index->rec=db_location;
	  index->next=-1;

	    index_write.write((char*)&num,sizeof(int));
	   index_write.write((char*)&db_location,sizeof(long));
	   index_write.write((char*)&minusone_long,sizeof(long));
	   index_write.flush();

		index_write.seekp(position,ios::beg); 

	  while(temp_next!=-1)
	   {
		   
	  index_read.seekg(temp_next,ios::beg);
	  index_write.seekp(temp_next,ios::beg); 
	  index_read.read((char*)&temp_k,sizeof(int));
      index_read.read((char*)&temp_rec,sizeof(long));

      index_read.read((char*)&temp_next,sizeof(long));



	   }
	  struct ch_node* index2=new ch_node;
	  index2->k=temp_k;
	  index2->rec=temp_rec;
	  index2->next=next_index;

       index_write.write((char*)&temp_k,sizeof(int));
	   index_write.write((char*)&temp_rec,sizeof(long));
	   index_write.write((char*)&next_index,sizeof(long));
	  index_write.flush();
}

		 }

		 else

		 {
		 cout<<"Record "<<num<< " is a duplicate."<<'\n';
		 
		 }





	   

	}


	else if(command_string=="find")


	{
		
		int index = atoi(strtok( NULL, " "));
	
       struct f_rec* temp;
	   struct ch_node temp_index;

	  int hash_value=index%1001;
	  int position = ((hash_value) * (sizeof( ch_node )));
	  boolean flag=false;
	 

	   do
	   {

      index_read.seekg(position,ios::beg);
	  index_read.read((char*)&temp_index,sizeof(ch_node));




	   int k1;
	  long rec1;
	  long next1;


	
	   k1=temp_index.k;
	   rec1=temp_index.rec;
	   next1=temp_index.next;

	//   cout<<"next:"<<next1<<'\n';


	   if(k1==index)
	   {



   int  num1;
   float amt1;
   char   type1;	// Transaction type (D=debit, C=credit)
   int     acct1;	// Account used for transaction
   int    hour1;	// Hour of transaction (0-23)
   int     min1;	// Minute of transaction (0-59)
   int     day1;	// Day of transaction (0-364)
   int     year1;	// Year of transaction (0000-9999)
      
   
   
   
   
   database_read.seekg(rec1,ios::beg);
   database_read.read((char*)&temp,sizeof(f_rec));

      num1=temp->num;
      amt1=temp->amt;
	  type1=temp->type;
	  acct1=temp->acct;
	  hour1=temp->hour;
	  min1=temp->min;
	  day1=temp->day;
	  year1=temp->year;

	  cout<<num1<<" "<<amt1<<" "<<type1<<" "<<acct1<<" "<<hour1<<" "<<min1<<" "<<day1<<" "<<year1<<'\n';
	  flag=true;
	  break;
	
	   }


	   position=temp_index.next;


	   }while(temp_index.next!=-1);
	   
	   if(flag==false)
	   {
	   
	   cout<<"Record "<<index<<" does not exist."<<'\n';
	   
	   }
	   
	   
	   }

	
	else if(command_string=="delete")


	{
	 int index = atoi(strtok( NULL, " "));
	
         struct ch_node checker;
	  int hash_value1=index%1001;
	  int position1 = ((hash_value1)* (sizeof(ch_node)));
	  int temp_k1=0;
	  long temp_rec1=0;
	  long temp_next1=0;
	  boolean exists=false;
	  int previous=-1;
	  int prev_k;
	  long prev_rec;
	  	
    do
	
	{
	
	index_read.seekg(position1,ios::beg); 
	index_read.read((char*)&temp_k1,sizeof(int));
	index_read.read((char*)&temp_rec1,sizeof(long));
	index_read.read((char*)&temp_next1,sizeof(long));

		if(index==temp_k1)

		{
		exists=true;

		if(previous!=-1)

   {
	   index_write.seekp(previous,ios::beg); 
	index_write.write((char*)&prev_k,sizeof(int));
	index_write.write((char*)&prev_rec,sizeof(long));
	index_write.write((char*)&temp_next1,sizeof(long));
	index_write.flush();
	
		
	}


	index_write.seekp(position1,ios::beg); 
	temp_k1=minusone_int;
	temp_rec1=minusone_long;
	

	index_write.write((char*)&temp_k1,sizeof(int));

	index_write.write((char*)&temp_rec1,sizeof(long));
	index_write.write((char*)&temp_next1,sizeof(long));
	index_write.flush();

		break;
		}

		prev_k=temp_k1;
		prev_rec=temp_rec1;
		previous=position1;
		position1=temp_next1;
	
	
	}
	while(position1!=-1);

	   if(exists==false)

	   {
	   
	   cout<<"Record "<<index<<" does not exist."<<'\n';
	   }
	
	
	
	}

		
	else if(command_string=="print")


	{
	
	int level=0;
	index_read.seekg(0,ios::beg);
	int index;
	long record;
	long next;

	int hashvalue1;

	for(hashvalue1=0;hashvalue1<1001;hashvalue1++)  
	
	{	  int position1 = ((hashvalue1)* (sizeof(ch_node)));

	 
	cout<<hashvalue1<<": ";
 while(position1!=-1)
 {
	
     index_read.seekg(position1,ios::beg);
	 index_read.read((char*)&index,sizeof(int));
	 index_read.read((char*)&record,sizeof(long));
	 index_read.read((char*)&next,sizeof(long));

	 if(index==-1 & next==-1)
	 {
	 
	 break;
	 
	 }
	 else if(index==-1 & next!=-1)

	 {
	 
	 position1=next;
	 
	 }
	 else
	 {

		 cout<<index<<"/"<<record<<' ';
	     position1=next;
	 }
 }
	cout<<'\n';
	
	
	}
	}


	else if(command_string=="end")


	{
	
	  database_write.close();
	  database_read.close();
	index_read.close();
	index_write.close();
	
	}

	else
	{
	
	
	cout<<"WRONG COMMAND!!!";
	
	
	}

}


}