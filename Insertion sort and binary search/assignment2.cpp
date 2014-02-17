#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <conio.h>
#include "filereader.h"
#include "str.h"
#include<string.h>
#include <cstring>
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#pragma warning(disable: 4996)


using std::fstream;
using std::cin;
using std::ios;

using std::cout;
using std::ifstream;
using std::istream;


struct index_S {
	int key;     // Key of record
	long off;    // Offset of record in file
};
struct avail_S {
	long off;    // Offset of hole
	int size;    // Size of hole
};

int struct_count=0;
int avail_count=0;
filereader write_student;
filereader write_student1;
long offset_temp=0;
long off_set=0;
long off_global=0;
int MAXSIZE=10;
index_S  * entry = new index_S[MAXSIZE];
avail_S  * avail = new avail_S[MAXSIZE];


int binarysearch(index_S entry[], int n, int value)
{
	int low = 0, mid=0, high = n-1;
	while (low <= high)
	{    mid = (low + high) / 2;
	if (value==entry[mid].key)
	{ break;}

	else if (value < entry[mid].key)

	{ high = mid - 1;}
	else  

	{	 low = mid + 1;}

	}

	if (value==entry[mid].key)
		return mid;
	else
		return -1;
}

char* filename=new char[];
string filename1;


int main(int argc, char *argv[] )

{  
	
filename=argv[1];
filename1=filename;


string indexfilename=filename1+".idx";
string availfilename=filename1+".avail";

int count;
char buffer[ 1024 ];

memset (buffer, 0, sizeof(buffer));


string read_line;



filereader fp_index;
filereader fp_avail;
filereader fp_index_file;
filereader fp_avail_file;



//  Opening all files
FILE * file;
file = fopen (filename,"r");

if (!file == NULL)
{
string indexfilename=filename1+".idx";
string availfilename=filename1+".avail";
fp_index_file.open(indexfilename,'x',1,1);
fp_avail_file.open(availfilename,'x',1,1);
int keyvalue;
long offsetvalue;
int temp;
int temp1; 
string* temp2 = new string[1024];
string temp3;
int temp4;
int temp5;
int temp_counter=0;
int size=0;
int size1=0;
long temp_global=0;

	      
        fp_index_file.read_raw((char *) &temp_global,sizeof(long));	
		off_global=temp_global;
		fp_index_file.read_raw((char *) &size,sizeof(int));	
		fp_avail_file.read_raw((char *) &size1,sizeof(int));	
			
		for(int k=0;k<size;k++)
          {  
			fp_index_file.read_raw((char *) &temp,sizeof(int));
			fp_index_file.read_raw( (char *) &temp1,sizeof(int) );
			entry[temp_counter].key=temp;
			entry[temp_counter].off=temp1;
			temp_counter++;
			struct_count++;
		} 

		fp_index_file.close();


		for(int d=0;d<size1;d++)
          {  
			fp_avail_file.read_raw((char *) &temp4,sizeof(int));
		
			fp_avail_file.read_raw( (char *) &temp5,sizeof(int) );
		
			avail[avail_count].off=temp4;
			avail[avail_count].size=temp5;
			avail_count++;
		} 

		fp_avail_file.close();

}

write_student1.open(filename1,'x',1,1);
fp_index.open(indexfilename,'x',1,1);
fp_avail.open(availfilename,'x',1,1);




while(!cin.eof())

{   cin.getline(buffer, 1024);
	read_line = buffer;
	
    int record_size;
	std::string  current_key;
	std::string  current_key1;
	string* inputline = new string[read_line.len()];
    string* key = new string [1024];
	string* key1 = new string [1024];
	
	
	//reading the command
	read_line.token(inputline,read_line.len());
	string command = inputline[0];

	if(command == "add")
	{  	char key_c2[1024];
		memset (key_c2, 0, sizeof(key_c2));
		inputline[1].token(key1, inputline[1].len(), "|");
		long key_int2;
		current_key1=key1[0];
		key_int2=atoi(key1[0]);
		int value1 =key_int2;
		
		int exist1;
		exist1=binarysearch(entry,struct_count,value1);

		if(exist1!= -1)
		{   
			cout<<"REPEATED VALUE"<<value1;
			cout<<"KEY ALREADY EXISTS!!!"<<'\n';
			break;
		}


	//calculating offset	
    off_set=write_student1.offset();
	inputline[1].token(key, inputline[1].len(), "|");
	
	long key_int;

	current_key=key[0];
	key_int=atoi(key[0]);
		
	char key_c[1024];
	char offset_c[1024];

	char* key_c1=new char[];
	char* offset_c1=new char[];
	memset (key_c, 0, sizeof(key_c));
	int size=sizeof(current_key);
	record_size = strlen(inputline[1]);
	std::size_t length = current_key.copy(key_c,size,0);
		long offset_temp1;
		int size_temp1;
		int w;
		bool flag=false;



		if(avail_count>0)

		{  
			
			for(w=0;w<avail_count;w++)
			{   
				offset_temp1=avail[w].off;
				size_temp1=avail[w].size;
				if(record_size+4<=size_temp1)
					
					
					{   flag=true;
						break;
				    }
	
			}


    if(flag)
	{
	entry[struct_count].key=key_int;
	entry[struct_count].off=offset_temp1;
	struct_count++;
		
	write_student1.seek(offset_temp1 , BEGIN );	
	write_student1.write_raw((char *) &record_size,sizeof( int ) );
	write_student1.write_raw(inputline[1],record_size);
	avail[w].size=avail[w].size-(record_size+4);
	  avail[w].off=offset_temp1+record_size+4;
	}

	else

	{
	entry[struct_count].key=key_int;
	entry[struct_count].off=off_global;
	struct_count++;
	
	write_student1.seek(off_global , BEGIN );	
	write_student1.write_raw((char *) &record_size,sizeof( int ) );
	write_student1.write_raw(inputline[1],record_size);


	off_global=write_student1.offset();
	}
    
	
	
if(flag && avail[w].size==0)
	{   
		
		int avail_size_temp;
        long avail_offset_temp;
	    int z=w;
		while(z!=avail_count)
		{
		avail[z].size=avail[z+1].size;
		avail[z].off=avail[z+1].off;
		z++;		
		}
		
		avail_count--;
	
	}
		
		}


		else

{	
	//MAXSIZE REACHED,increase array size
	if(struct_count>=MAXSIZE)

	{   cout<<"Max size is:"<<MAXSIZE;
		MAXSIZE=MAXSIZE*(1.8);

		index_S *entry_expanded = new index_S[MAXSIZE];
		 cout<<"New Max size is:"<<MAXSIZE;
		for (int v=0; v<struct_count;v++)
		{
		  entry_expanded[v] = entry[v];
		}
		
		entry=NULL;
		delete[] entry;
		entry=entry_expanded;
		
		entry[struct_count].key=key_int;
	    entry[struct_count].off=off_set;
	    struct_count++;
    }

	else
	{
	entry[struct_count].key=key_int;
	entry[struct_count].off=off_set;
	struct_count++;
	
	}
	
	//Writing to the students file
	write_student1.seek(off_global , BEGIN );	
	write_student1.write_raw((char *) &record_size,sizeof( int ) );
	write_student1.write_raw(inputline[1],record_size);


	off_global=write_student1.offset();

		}


			//Insertion sort

	int j=0, key_value; int offset_count=0;
	for (int i=1; i<struct_count; ++i) 
	{
		key_value= entry[i].key;
		offset_temp = entry[i].off;
		
		j = i-1;

		while((j >= 0) && (entry[j].key > key_value))
		{
			entry[j+1].key = entry[j].key;
			entry[j+1].off =entry[j].off;
			j -= 1;
		}
		entry[j+1].key=key_value;
		entry[j+1].off=offset_temp;
		offset_count++;
	}


	//Insertion sort ends here


	}//if ends here

	else if(command == "find")

	{
		char key_c1[1024];
		memset (key_c1, 0, sizeof(key_c1));
		inputline[1].token(key1, inputline[1].len(), "|");
		int record_size1 = strlen(inputline[1]);
		long key_int1;
		current_key1=key1[0];
		key_int1=atoi(key1[0]);
		int value =key_int1;
		char buffer2[ 1024 ];
		memset (buffer2, 0, sizeof(buffer2));

		int exist;
		exist=binarysearch(entry,struct_count,value);

		if(exist==-1)
		{
		cout<<"No Record with SID:"<<value<<" "<<"exists"<<'\n';
		}

		else
		{
			long off=entry[exist].off;
			
			write_student1.seek( off, BEGIN );
            write_student1.read_raw( (char *) &off, sizeof( long ) );
            write_student1.read_raw( buffer2, off );
			for(int z=0;z<off;z++)
			{cout<<buffer2[z];}
			cout<<'\n';
				
        }

	  }


	else if(command == "del")
	

	{
	
	    char key_c1[1024];
		memset (key_c1, 0, sizeof(key_c1));
		inputline[1].token(key1, inputline[1].len());
		int record_size1 = strlen(inputline[1]);
		long key_int1;
		current_key1=key1[0];
		key_int1=atoi(key1[0]);
		int value =key_int1;
		char buffer2[ 1024 ];
		memset (buffer2, 0, sizeof(buffer2));
		int exist;
		int key_temp;
		long off_temp;
		exist=binarysearch(entry,struct_count,value);

		if(exist==-1)
		{
			cout<<"No Record with SID="<<value<<"exists"<<'\n';

		}
		else
		{
		
		//cout<<"KEY FOUND!!!"<<'\n';
		 int del=exist;
		 key_temp=entry[del].key;
		 off_temp=entry[del].off;

		 char buffer3[ 1024 ];
         int count3;
       


// Open file using open() method

		 write_student1.seek( off_temp, BEGIN );
		 write_student1.read_raw( (char *) &count3, sizeof( int ) );
		 write_student1.read_raw( buffer3, count3 );
		avail[avail_count].off=off_temp;
		avail[avail_count].size=count3+4;
	
		 for(int m=del;m<struct_count;m++)
		 {  int temp1=entry[m+1].key;
		    float temp2=entry[m+1].off;

		    entry[m].key=temp1;
			entry[m].off=temp2;

		 }
		 avail_count++;
		 struct_count--;

		}

	}

	else if(command == "end")
	{
	
	      char * key_char =new char[];
		  char * off_char =new char[];
		  fp_index.write_raw((char *)&off_global,sizeof(long));
          fp_index.write_raw((char *)&struct_count,sizeof(int));
	for(int n=0;n<struct_count;n++)
	{
		 		
//Writing the index,offset from structure to the index file
        
		fp_index.write_raw((char *)&entry[n].key,sizeof(int));
		fp_index.write_raw((char *)&entry[n].off,sizeof(long));

	}

	 cout<<"\n";
     cout<<"Index:"<<'\n';
for(int i=0;i<=struct_count-1;i++)
{
	
	cout<<entry[i].key<<":"<<entry[i].off;
   	cout<<"\n";


}

 cout<<"Availibility:"<<'\n';
 fp_avail.write_raw((char *)&avail_count,sizeof(int));

for(int l=0;l<avail_count;l++)
{   
	cout<<avail[l].off<<":";
    cout<<avail[l].size;
	cout<<"\n";
	    fp_avail.write_raw((char *)&avail[l].off,sizeof(long));
		fp_avail.write_raw((char *)&avail[l].size,sizeof(int));
}
	}
	else
	{
	 cout<<"WRONG INPUT";
	
	}




         }//while loop





write_student1.close();
fp_index.close();
fp_avail.close();

_getch();
return 0; 

}	


