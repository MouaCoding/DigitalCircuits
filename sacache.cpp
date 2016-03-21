#include <fstream>
#include <iostream>
#include<cstdlib>
#include<cstring>
#include <iomanip>

using namespace std;
/* We need two classes: one for the Cache Line, and the other for Set.
 * This is a 4 way set associative cache, so we will initilaize an array of Cache Lines
 */

class Cache
{
public:
  int tag;
  unsigned char data[8];
  int dirty;
  unsigned char way;
};

class CacheSet
{
public:
  Cache set[4];
  
  Cache *write(unsigned short tag)
  {
    Cache *l = NULL; //create new Cache, we will write to here and return it.
    for(int i = 0; i < 4; i++)
    {
      if(set[i].tag == tag)
        return &set[i];

      if(l == NULL || set[i].way > l->way)
        l = &set[i];
    }
  return l;
  }

};

void update(Cache *cacheLine, int index, unsigned char ram[], int tmptag)
{
int start = (cacheLine->tag << 7) | (index << 3);
      for(int i = 0; i < 8; i++)
        ram[start + i] = cacheLine->data[i];

      //cacheLine[index].tag = tmptag;

      int next = (tmptag << 7) | (index << 3);
      for(int i = 0; i < 8; i++)
        cacheLine->data[i] = ram[next+i];
      cacheLine->tag = tmptag;
      cacheLine->dirty = 0;

}
int main(int argc, char** argv)
{
  int address, operation, val;
  unsigned char ram[65536];
  char* file = argv[1];
  ofstream out;
  ifstream inf(file);
  out.open("sa-out.txt");
  int index, tmptag, offset;
  Cache *cacheLine;
  CacheSet *set = new CacheSet[16];

//initialize set
  for(int i = 0; i < 16; i++)
  {
    for(int j = 0; j < 4; j++)
    {
      set[i].set[j].dirty = 0;
      set[i].set[j].tag = 0;
      set[i].set[j].way = j;
    
    for( int k = 0; k < 8; k++)
            set[i].set[j].data[k] = 0;
  }
  }

  bool dirty, hit;
  while( inf >> hex >> address >> operation >> val) {
     //set = index* 4;
     index = (address >> 3) & 0xF;
     tmptag = (address >> 7);
     offset = address & 0x7;

    //call write(tag), we will write into the new cacheLine.
    cacheLine = set[index].write(tmptag);
    char hit = cacheLine->tag == tmptag;

        
    
//cout << address  << endl;
  if(operation == 255) //if FF
  {//write
    //if(cache[index].tag == tmptag)
      //cacheLine[index].dirty = 0;

   if(!hit)
     update(cacheLine, index, ram, tmptag);

     cacheLine->data[offset] = val;
     cacheLine->tag = tmptag;
     cacheLine->dirty = true;

     for(int i = 0; i < 4; i++)
       set[index].set[i].way++;
     cacheLine->way = 0;
/*

    else if(cacheLine[index].tag != tmptag)
    {
      update(cacheLine, index, ram, tmptag);

      cacheLine[index].tag = tmptag;
      cacheLine[index].data[offset] = val;
    }
    else
    {
      cacheLine[index].data[offset] = val;
    }

    cacheLine[index].dirty = 1;
*/

  }//  write(cacheLine, address, index, val);


  else //if(operation == 0)
  {//read

    //if(address < 4096)
      //out << "0" << uppercase << hex << setw(4) << setfill('0') << (int) address << " ";
    //else
      out <<  uppercase <<hex << setw(4) << setfill('0') << (int) address << " ";
    //hit = cacheLine[index].tag == tmptag;
      char dirt = cacheLine->dirty;

    if(!hit)
    {
      //this is essentially update()
      int start = (cacheLine->tag << 7) | (index << 3);
      for(int i = 0; i < 8; i++)
        ram[start + i] = cacheLine->data[i];

      int next = (tmptag << 7) | (index << 3);
      for(int i = 0; i < 8; i++)
        cacheLine->data[i] = ram[next+i];

      cacheLine->tag = tmptag;
      cacheLine->dirty = 0;
    }
     for(int i = 0; i < 4; i++)
       set[index].set[i].way++;
     cacheLine->way = 0;

//print 
     for(int i = 7; i >= 0; i--)
        {
          out << setw(2) << +cacheLine->data[i];
        }

  //dirty = cacheLine[index].dirty;

  out << ' ' << +hit << ' ' << +dirt << endl;


  //if(!hit)
    //cacheLine[index].dirty = 0;
  }

}

}//main

