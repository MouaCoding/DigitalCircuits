#include <iomanip>
#include <fstream>
#include<iostream>

using namespace std;


int main(int argc, char* argv[])
{
  char* file = argv[1];
  ifstream inf(file);
  ofstream out;
  out.open("vm-out.txt");
  int i,j;
  int ram[4];
  int bit[4];

//initialize
  for(int i = 0; i < 4; i++)
  {
    ram[i] = 0;
    bit[i] = 0;
  }
//unsigned ints will hold the data
  unsigned int data, page;  

//count will be the counter of each read
//stop is when the counter stops when there is a match
//match is the index of the matched or already existent
//pointer is the place we insert into our ram[]
//pos is the position where we are writing
//checker checks if there is already existing 
  int count = 1, stop = 0, match = 0, pointer = 1, pos = -1, checker;

//  unsigned long address;

/* READ DATA, CONVERT TO DECIMAL */
  while(inf >> hex >> data)
  {
    //start values
    stop = match = 0;
    pos = -1;
 
    page = (data & 0xFFFFF000) >> 12; //page = 1024
    //cout << page << " " << count << endl;

    if(count == 17) //check first after 16
    {
      ram[0] = page;
      //ram[0] = address-21;
      bit[0] = 1;
      //cout << ram[0] << endl;
      out << hex << ram[0] << "000" << endl;
    }
    if(count > 17) //start storing, checks all after 17 since we stored 17
    {
      for( i = 0; i < 4; i++)
      {
        if(ram[i] == page)
        {
          match = 1; bit[i] = 1;
          if(pointer == 4) //if we reach the end of ram[]. start over and look at 0 index
            pointer = 0;
        }
      }
      if(match == 0) 
      {
        while(stop != 1)//write by clock replacement
	{	
	  if(bit[pointer] == 0)
	  {
     	    pos = pointer; //if stored, then increment pointer
	    pointer++;
	    ram[pos] = page; //ram pos = page
	    bit[pos] = 1;
	    stop = 1;
  	  }
          else
          {
            bit[pointer] = 0;
            pointer++; 
	    checker = 1;
            for( i = 0; i < 4; i++){ //check if it is in RAM
	      if(bit[i] == 0)
	      {
	        checker = 0;
		break; //if it is, then stay at curr ptr
	      }
            }
            if(checker == 1)
            {
              pos = -1; stop = 1;
            }
          }
        if(pointer == 4) //if ptr is at end of ram, start over
          pointer = 0;
        }
      if(pos == -1)
      {
      for( i = 0; i < 4; i++)
        bit[i] = 0;
        ram[0] = page; bit[0] = 1; pointer = 1;
      }
    }
    for( j = 0; j < 4; j++)
    {
      if(ram[j] != 0)
      {
        if(count == 18) {
          if(j!= 1)
            out << hex << ram[j] << "000 ";
          else
            out << hex << ram[j] << "000";
        }
        else if(count == 19)
        {
          if(j!= 2)
            out << hex << ram[j] << "000 ";
          else
            out << hex << ram[j] << "000";
        }
        else
        {
          if(j!= 3)
            out << hex << ram[j] << "000 ";
          else
            out << hex << ram[j] << "000";
        }
      }
     }
   out << endl;
    }
 count++;

    //cout << address << endl;
  }

  out.close();
  inf.close(); 


  return 0;
}
