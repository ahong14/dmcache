#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
using namespace std;

//functions needed to convert hex to binary, binary to decimal
//cache class created
//cache class member functions defined

//convert hex to binary
string hextobinary(char hex){
    string binary;
    switch(hex){
        case '0':
            binary="0000";
            break;
        case '1':
            binary="0001";
            break;
        case '2':
            binary="0010";
            break;
        case '3':
            binary="0011";
            break;
        case '4':
            binary="0100";
            break;
        case '5':
            binary="0101";
            break;
        case '6':
            binary="0110";
            break;
        case '7':
            binary="0111";
            break;
        case '8':
            binary="1000";
            break;
        case '9':
            binary="1001";
            break;
        case 'A':
            binary="1010";
            break;
        case 'B':
            binary="1011";
            break;
        case 'C':
            binary="1100";
            break;
        case 'D':
            binary="1101";
            break;
        case 'E':
            binary="1110";
            break;
        case 'F':
            binary="1111";
            break;
    }
    
    return binary;
    
}

//convert binary to decimal value
int bintodec(string bits){
    int decimal=0;
    
    for (int i=bits.length()-1; i>=0;i--){
    
        if (bits[i]=='1'){
            int exponent = (bits.length()-1) - i;
            decimal=decimal+pow(2, exponent);
        }
    }
    
    return decimal;
}



//cache line class
class cache_line{
    public:
    cache_line(); //constructor
    void set_dirty_bit(); //set dirty bit
    void reset_dirty_bit(); //reset dirty bit
    void set_line_number(int line_no); //set line number
    int get_line_number();//get line number
    void set_data(int offset1, string temp);//set data at offset
    void print_data(int offset);//print data at offset
    void set_tag(int tag_no);//set tag number
    int get_tag();//get tag number
    string get_data(int offset);//get data at offset
    int get_dirty_bit();//get dirty bit 
    void print_all_data();//print all data bytes

   
    private:
    int dirty_bit; //dirty bit
    int line_number;//line number of cache line
    string data[8]; // data bytes within cache line 
    int tag;//tag number
    
};

//cache line constructor

cache_line::cache_line(){
    //initialize values
    dirty_bit = 0; //set dirty bit to 0
    line_number=-1;//set line number to -1
    tag=-1;//set tag number to -1 
    
    //set all bits within data bytes to 0
    for (int i=0; i<8; i++){
        data[i]="0"; //set all data bytes to 0 initially 
    }
    
    
    
}

//set dirty bit function

void cache_line::set_dirty_bit(){
    dirty_bit=1;
}

//reset dirty bit function

void cache_line::reset_dirty_bit(){
    dirty_bit=0; 
}


//set line number function 
void cache_line::set_line_number(int line_no){
    line_number = line_no;
}

//return line number
int cache_line::get_line_number(){
    return line_number;
}

//set data at offset to temp
void cache_line::set_data(int offset1, string temp){
    data[offset1] = temp;
    
}

//print data at offset
void cache_line::print_data(int offset){
    cout<<data[offset]<<endl;
}

//set tag number
void cache_line::set_tag(int tag_no){
    tag=tag_no;
    
}

//get tag number
int cache_line::get_tag(){
    return tag;
}

//get data at offset
string cache_line::get_data(int offset){
    return data[offset];
}

//get dirty bit
int cache_line::get_dirty_bit(){
    return dirty_bit;
}

//print all data
void cache_line::print_all_data(){
    for(int i=0; i<8; i++){
        cout<<data[i];
    }
    cout<<endl;
}
//end of class functions 

int main(int argc, char**argv){
    
    //initialize 32 cache line objects 
    cache_line cache[32];
    
    //RAM
    string RAM[65536][8];
    
     //initialize RAM
    for (int i=0; i<65536; i++){
        for(int j=0; j<8; j++){
            RAM[i][j]="0";
        }
    }
    
    for (int i=0; i<32;i++){
        //set cache line number 
        cache[i].set_line_number(i);
    }
    
    //open test file to read input
    ifstream fin;
    fin.open(argv[1]);
    if(fin.fail()){
        cout<<"file failed to open"<<endl;
        return -1;
    }
    
    //open output file
    ofstream out;
    out.open("dm-out.txt");
    if(out.fail()){
        cout<<"file output failed to open"<<endl;
        return -1;
    }
  
    
    
    //parse address, operation, and data bytes
    //go one line at a time
    while(!fin.eof()){
        string current_line; //read line from file into this string
        string current_bits; //currents bits that are converted from being converted
        string binary_pattern;  //store bit patterns for hex values being converted
        string data_pattern; //stores binary pattern for data
        
        char address_hex[4]; //store 4 value HEX address pattern 
        char address_binary[16]; //store 16 bit pattern for HEX address
        
        char op_hex[2]; //store 2 value HEX operation pattern
        string operation; //store operation pattern for write or read FF or 00
        
        char data_hex[2]; //store 2 value HEX pattern 
        

        unsigned int add_index=0; //address index to extract each HEX value
        unsigned int op_index=0; //operation index to extract HEX value
        unsigned int dat_index=0; //data index to extract HEX value
        
        unsigned int offset; //offset to store data
        unsigned int line_no; //line number for cache line
        int tag_no; //tag number for cache line
        
        
        //first, read entire line from file
        getline(fin,current_line);
        
        //reached end of file, end program
        if(current_line==""){
            break;
        }
       
        //parse the line and separate into address, operation, and data
        for(int i=0; i<current_line.length(); i++){
            
            //example: 40A4 FF A9
            
            //store first four HEX values
            if (i<4 and !isspace(current_line[i])){
                address_hex[add_index++] = current_line[i];
            }
            
            //store next 2 operation HEX values
            else if (i>4 && i<7 && !isspace(current_line[i])){
                op_hex[op_index++]=current_line[i];
            }
            
            //store next 2 data HEX values
            else if (i>7 && !isspace(current_line[i])){
                data_hex[dat_index++]=current_line[i];
            }
        }
        //end of parsing line
        
        //convert address to binary 
        
        for (int i=0; i<4; i++){
            current_bits=hextobinary(address_hex[i]);//get current bit pattern
            binary_pattern += current_bits;//add bit patterns together to get 16 bit pattern
        }
        
        //store binary pattern into address 
        
        for (int i=0; i<16; i++){
            address_binary[i]=binary_pattern[i];
        }
      
       
       //store data pattern HEX
       data_pattern += data_hex[0];
       data_pattern += data_hex[1];
       
       //store binary pattern into data
      
       //clear strings
       binary_pattern="";
       current_bits="";
       
       //save operation
       operation=op_hex;
        
       //store offset_pattern as bits 13-15       
       string offset_pattern;
       offset_pattern += address_binary[13];
       offset_pattern += address_binary[14];
       offset_pattern += address_binary[15];
       offset = bintodec(offset_pattern);//store decimal value of offset
       
       //get line number
       //bits 8 9 10 11 12 
       string line_pattern;
       line_pattern += address_binary[8];
       line_pattern += address_binary[9];
       line_pattern += address_binary[10];
       line_pattern += address_binary[11];
       line_pattern += address_binary[12];
       
       
       line_no = bintodec(line_pattern);//store decimal value of line number
       
       
       //get tag
       //bits 0-7
       string tag_pattern;
       tag_pattern += address_binary[0];
       tag_pattern += address_binary[1];
       tag_pattern += address_binary[2];
       tag_pattern += address_binary[3];
       tag_pattern += address_binary[4];
       tag_pattern += address_binary[5];
       tag_pattern += address_binary[6];
       tag_pattern += address_binary[7];
       
       tag_no = bintodec(tag_pattern);//store decimal value of tag number
       
       
       //write operation
       if(operation=="FF"){
           //search for correct line number
           for (int i=0; i<32; i++){
               //cache hit for write
               //case where tag has not been set for in cache line
               if (cache[i].get_line_number() == line_no && cache[i].get_tag()==-1){
                   //write data to offset of memory block
                   cache[i].set_tag(tag_no);//set tag number 
                   cache[i].set_data(offset,data_pattern);//set data
                   cache[i].set_dirty_bit(); //set dirty bit
                   operation=""; //clear operation string 
                   i=33; //break out of loop
               }
               
               //case where tag is set, checking line number and matching tag
               else if(cache[i].get_line_number() == line_no && cache[i].get_tag()==tag_no){
                    //write data to offset of memory block
                   cache[i].set_data(offset,data_pattern);//set data
                   cache[i].set_dirty_bit(); //set dirty bit
                   operation="";  //clear operation string
                   i=33; //break out of loop
                   
               }
              
               //cache miss, need to evict cache line to RAM, bring in correct data based on tag
               else if(cache[i].get_line_number() == line_no && cache[i].get_tag()!= tag_no){
                   //evict cache line into RAM
                   //save data of this tag into RAM
                   for (int j=0; j<8; j++){
                       RAM[cache[i].get_tag()][j]=cache[i].get_data(j);
                   }
                   
                   //set this line to target tag
                   cache[i].set_tag(tag_no);
                   
                   //take in data from RAM and place into cache line
                   for (int j=0; j<8; j++){
                       cache[i].set_data(j, RAM[tag_no][j]);
                   }
                   
                   //set appropriate data for target tag
                   cache[i].set_data(offset, data_pattern);
                   cache[i].set_dirty_bit(); //set dirty bit
                   operation=""; //clear operation string 
                   i=33; //break out of loop
               }
           } //end of for loop
       } // end of write operation 
       
       
      //read operation
      else if(operation=="00"){
          //search for correct line number
          for(int i=0; i<32; i++){
              //cache hit for read, correct line number and tag 
              if(cache[i].get_line_number()==line_no && cache[i].get_tag() == tag_no){
                  //cache hit for read, just print data
                  out<<cache[i].get_data(offset)<<" "<<"1 "<<cache[i].get_dirty_bit()<<endl;
                  operation="";//clear operation string
                  i=33;//break out of loop
              }
              
              //cache miss for read 
              else if (cache[i].get_line_number()==line_no && cache[i].get_tag() != tag_no){
                  //store previous dirty bit
                  int previous_dirty_bit = cache[i].get_dirty_bit();
                  
                  //evict cache line into RAM
                  //save data of this tag into RAM
                  for (int j=0; j<8; j++){
                      RAM[cache[i].get_tag()][j]=cache[i].get_data(j);
                  }
                   
                  //set this line to target tag
                  cache[i].set_tag(tag_no);
                  
                  //take in data from RAM and place into cache line
                  for (int j=0; j<8; j++){
                      cache[i].set_data(j, RAM[tag_no][j]);
                  }
                  
                  //print data and previous dirty bit           
                  out<<cache[i].get_data(offset)<<" "<<"0 "<<previous_dirty_bit<<endl;
                  operation="";//clear operation string
                  cache[i].reset_dirty_bit();  //reset drity bit
                  i=33;//break out of loop
              }
          }//end of for loop
            
      }//end of read operation 
      
        
    } //end of while loop
    
    //close files
    fin.close();
    out.close();
    
    return 0;
}
