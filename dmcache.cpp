#include <fstream>
#include <iostream>
#include<cstdlib>
#include<cstring>
#include <iomanip>

using namespace std;

class Cache
{
public:
  int tag;
  unsigned char data[8];
  int dirty;
};


void update(Cache *cacheLine, int index, unsigned char ram[], int tmptag)
{
// int hit = 1;
int start = (cacheLine[index].tag <<  9) | (index << 3);
      for(int i = 0; i < 8; i++)
        ram[start + i] = cacheLine[index].data[i];

      int next = (tmptag << 9) | (index << 3);//address & ~7;
      for(int i = 0; i < 8; i++)
        cacheLine[index].data[i] = ram[next+i];
}
/*
void read(ofstream &o,int hit, bool dirty, Cache cacheLine[32], int address, int index)
{
  //out << hex << uppercase << setw(4) << setfill('0') << (int) value << ' '; 
  o << hex << uppercase << setw(4) << setfill('0')
        << (((int) (cacheLine[index].data[address & (7)])) & 0xFF);

  o << ' ' << hit << ' ' << dirty << endl; 
  
}
*/
int main(int argc, char** argv)
{
  unsigned long address;
  int operation, val;
  unsigned char ram[65536];
  char* file = argv[1];
  ofstream out;
  ifstream inf(file);
  out.open("dm-out.txt");
  int index, tmptag, offset;
  Cache *cacheLine = new Cache[64];

//initialize the cache
  for(int i = 0; i < 64; i++)
  {
    cacheLine[i].dirty = 0;
    cacheLine[i].tag = 0;
    for(int j = 0; j < 8; j++)
      cacheLine[i].data[j] = 0;
  }

  bool dirty, hit;
  while(inf >> hex >> address >> operation >> val) {
// read entire file, convert to decimal
  
     index = (address >> 3) & 0x3F;
     tmptag = (address >> 9);
     offset = address & 0x7;
    //cout << address << " " << operation << " " << val << endl;
    //cout << index << " " << tmptag << " " << cacheLine[index].tag << endl;

  if(operation)
  {
    //hit = 1;
    if(cacheLine[index].tag == 0)
    {
     //cacheLine[index].tag = tmptag;
     cacheLine[index].data[offset] = val;
     cacheLine[index].tag = tmptag;
     //cacheLine[index].dirty = 1;
    }
    else if(cacheLine[index].tag != tmptag)
    {
      update(cacheLine, index, ram, tmptag);      
/*      unsigned long start = (cacheLine[index].tag << 9) | (index << 3);
      for(int i = 0; i < 8; i++)
      {  ram[start + i] = cacheLine[index].data[i];
}
      unsigned long next = (tmptag << 9) | (index << 3);
      for(int i = 0; i < 8; i++)
 {       cacheLine[index].data[i] = ram[next+i];
}
*/      cacheLine[index].tag = tmptag;
      cacheLine[index].data[offset] = val;
    }
    else
    {
      cacheLine[index].data[offset] = val;
    }

    cacheLine[index].dirty = 1;
  }//  write(cacheLine, address, index, val);
  else //if(operation == 0)
  {
//  read(out,hit, dirty, cacheLine, address, index);   
//  update(cacheLine, index, ram, tmptag); 

    out <<  uppercase <<hex <<setw(4) << setfill('0') <<  address << " "; 
    hit = cacheLine[index].tag == tmptag;
      //update(cacheLine, index, ram, tmptag);
      unsigned long start = (cacheLine[index].tag << 9) | (index << 3);
      for(int i = 0; i < 8; i++)
        ram[start + i] = cacheLine[index].data[i];

      unsigned long next = (tmptag << 9) | (index << 3);// address & ~7;
              //cout << index << " " << tmptag<< " " << next << " " << start << endl;
      for(int i = 0; i < 8; i++) 
      {
        cacheLine[index].data[i] = ram[next+i];
        //cout << address << " " << i << endl;//cacheLine[index].data[i] << endl;
      }
     //cacheLine[index].data[offset] = val; 
      cacheLine[index].tag = tmptag;
      //cacheLine[index].data[next] = val;
        for(int i = 7; i >= 0; i--)
        {
          out <<setfill('0') << setw(2) << uppercase <<  (int) +cacheLine[index].data[i]; 
        }
//(int) (cacheLine[index].data[offset]);

  
  out << ' ' << hit << ' ' << cacheLine[index].dirty << endl;
  

  if(!hit)
    cacheLine[index].dirty = 0;
  }
 //out.close();
  //inf.close();
  //return 0;
  } 
}
