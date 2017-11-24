#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
using namespace std;


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
    int get_line_number();
    void set_data(int offset1, string temp);
    void print_data(int offset);
    void set_tag(int tag_no);
    int get_tag();
    string get_data(int offset);
    int get_dirty_bit();
    void print_all_data();

   
    private:
    int dirty_bit; //dirty bit
    int line_number;//line number of cache line
    string data[8]; // data bytes within cache line 
    int tag;
    
};

//cache line constructor

cache_line::cache_line(){
    //initialize values
    dirty_bit = 0;
    line_number=-1;
    tag=-1;
    
    //set all bits within data bytes to 0
    for (int i=0; i<8; i++){
        data[i]="0";
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

int cache_line::get_line_number(){
    return line_number;
}

void cache_line::set_data(int offset1, string temp){
    data[offset1] = temp;
    
}

void cache_line::print_data(int offset){
    cout<<data[offset]<<endl;
}

void cache_line::set_tag(int tag_no){
    tag=tag_no;
    
}

int cache_line::get_tag(){
    return tag;
}

string cache_line::get_data(int offset){
    return data[offset];
}

int cache_line::get_dirty_bit(){
    return dirty_bit;
}

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
    int ram_size=65536;
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
    ofstream fout;
    fout.open("dm-out.txt");
    
    
    //parse address, operation, and data bytes
    //go one line at a time
    while(!fin.eof()){
        string current_line;
        string current_bits;
        string binary_pattern;
        string data_pattern;
        
        char address_hex[4];
        char address_binary[16];
        
        char op_hex[2];
        string operation;
        
        char data_hex[2];
        char data_binary[8];

        unsigned int add_index=0;
        unsigned int op_index=0;
        unsigned int dat_index=0;
        
        unsigned int offset;
        unsigned int line_no;
        int tag_no;
        
        
        //first, read entire line from file
        getline(fin,current_line);
       
        //parse the line and separate into address, operation, and data
        for(int i=0; i<current_line.length(); i++){
            
            if (i<4 and !isspace(current_line[i])){
                address_hex[add_index++] = current_line[i];
            }
            
            else if (i>4 && i<7 && !isspace(current_line[i])){
                op_hex[op_index++]=current_line[i];
            }
            
            else if (i>7 && !isspace(current_line[i])){
                data_hex[dat_index++]=current_line[i];
            }
        }
        //end of parsing line
        
      
        

        //convert address to binary 
        
        for (int i=0; i<4; i++){
            current_bits=hextobinary(address_hex[i]);
            binary_pattern += current_bits;
        }
        
        //store binary pattern into address 
        
        for (int i=0; i<17; i++){
            address_binary[i]=binary_pattern[i];
        }
        
       binary_pattern="";
       current_bits="";
       
       
    //   //convert data to binary
    //   for (int i=0; i<2; i++){
    //       current_bits=hextobinary(data_hex[i]);
    //       binary_pattern += current_bits;
    //   }
    
       data_pattern += data_hex[0];
       data_pattern += data_hex[1];
    
    //   cout<<"data to be writenn or read"<<endl;
    //   cout<<data_pattern<<endl;
       
       //store binary pattern into data
       
       for(int i=0; i<8; i++){
           data_binary[i] = binary_pattern[i];
           
       }
       
       binary_pattern="";
       current_bits="";
       
       //save operation
       operation=op_hex;
       
       
       //get offset
    //   cout<<"address binary is ";
    //   for(int i=0; i<16; i++){
    //       //address_binary[i]=address_binary[i+1];
    //       cout<<address_binary[i];
    //   }
       
       
       
 //      cout<<endl;
       string offset_pattern;
       offset_pattern += address_binary[13];
       offset_pattern += address_binary[14];
       offset_pattern += address_binary[15];
       
  //     cout<<"offset pattern is "<<offset_pattern<<endl;
       
       offset = bintodec(offset_pattern);
       
     
       
       //get line number
       //bits 8 9 10 11 12 
       string line_pattern;
       line_pattern += address_binary[8];
       line_pattern += address_binary[9];
       line_pattern += address_binary[10];
       line_pattern += address_binary[11];
       line_pattern += address_binary[12];
       
       
       line_no = bintodec(line_pattern);
       
       
       //get tag
       string tag_pattern;
       tag_pattern += address_binary[0];
       tag_pattern += address_binary[1];
       tag_pattern += address_binary[2];
       tag_pattern += address_binary[3];
       tag_pattern += address_binary[4];
       tag_pattern += address_binary[5];
       tag_pattern += address_binary[6];
       tag_pattern += address_binary[7];
       
       
       
       tag_no = bintodec(tag_pattern);
       
       
       
       //done converting to bits
       
       //write operation
       if(operation=="FF" && current_line != "" ){
           
           for (int i=0; i<32; i++){
               //cache hits
               //case where tag has not been set for in cache line
               if (cache[i].get_line_number() == line_no && cache[i].get_tag()==-1){
                   //write data to offset of memory block
                //   cout<<"cache write with no tag reached"<<endl;
                //   cout<<"offset is "<<offset<<endl;
                //   cout<<"data to be written is "<<data_pattern<<endl;
                   cache[i].set_tag(tag_no);//set tag number 
                   cache[i].set_data(offset,data_pattern);//set data
                   cache[i].set_dirty_bit(); //set dirty bit
                   operation="";  
                   
                //   cout<<"data written"<<endl;
                //   for(int k=0; k<8; k++){
                //       cout<<cache[i].get_data(k);
                //   }
                //   cout<<endl;
                   
                    //for debugging
                //   cout<<"data that was written"<<endl;
                //   cache[i].print_data(offset);
                   i=33;
               }
               //case where tag is set, checking line number and matching tag
               else if(cache[i].get_line_number() == line_no && cache[i].get_tag()==tag_no){
                    //write data to offset of memory block
                //   cout<<"cache write reached with same tag number"<<endl;
                //   cout<<"offset is "<<offset<<endl;
                //   cout<<"data to be written is "<<data_pattern<<endl;
                   cache[i].set_data(offset,data_pattern);//set data
                   cache[i].set_dirty_bit(); //set dirty bit
                   operation="";  
                   
                   //for debugging
                //   cout<<"data that was written"<<endl;
                //   cache[i].print_data(offset);
                   i=33;
                   
               }
              
               //cache miss, need to evict cache line to RAM, bring in correct data based on tag
               else if(cache[i].get_line_number() == line_no && cache[i].get_tag()!= tag_no){
                    //for debugging
                //   cout<<"cache write miss reached"<<endl;
                //   cout<<"previous tag"<<endl;
                //   cout<<cache[i].get_tag()<<endl;
                //   cout<<"previous data"<<endl;
                //   for (int k=0; k<8;k++){
                //       cout<<cache[i].get_data(k);
                //   }
                   
                   
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
                   operation="";  
                   
                   //for debugging
                //   cout<<"new tag"<<endl;
                //   cout<<cache[i].get_tag()<<endl;
                //   cout<<"data that was written"<<endl;
                //   cache[i].print_data(offset);
                //   cout<<"new data"<<endl;
                //   for (int k=0; k<8;k++){
                //       cout<<cache[i].get_data(k);
                //   }
                   i=33;
               }
               
               
           } //end of for loop
        
       } // end of write operation 
       
       
      //read operation
      else if(operation=="00"  && current_line != "" ){
          //search for correct line number
          for(int i=0; i<32; i++){
              
              //cache hit for read, correct line number and tag 
              if(cache[i].get_line_number()==line_no && cache[i].get_tag() == tag_no){
                //   cout<<"cache hit"<<endl;
                //   cout<<"cache hit read"<<endl;
                //   cache[i].print_all_data();
                //   cout<<"offset is "<<offset<<endl;
                  string data_to_be_read=cache[i].get_data(offset);
                  
                //  cout<<"testing data to be read"<<endl;
                //   char data_output[2];
                //   data_output[0]=data_to_be_read[0];
                //   data_output[1]=data_to_be_read[1];
                  cout<<data_to_be_read<<" "<<"1 "<<cache[i].get_dirty_bit()<<endl;
                  operation="";
                
             //     cout<<"do i get here"<<endl;
                  i=33;
              }
              
              //cache miss for read 
              else if (cache[i].get_line_number()==line_no && cache[i].get_tag() != tag_no){
            //      cout<<"cache miss occurred for read"<<endl;
                  //store previous dirty bit 
                  int previous_dirty_bit = cache[i].get_dirty_bit();
                
              //    cout<<"previous tag"<<endl;
            //      cout<<cache[i].get_tag()<<endl;
                //   cout<<"previous data"<<endl;
                //   for (int k=0; k<8;k++){
                //       cout<<cache[i].get_data(k);
                //   }
                
                
                
                
                
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
                  
                //   cout<<"RAM data at new tag "<<tag_no<<endl;
                //   for (int k=0;k<8; k++){
                //       cout<<RAM[tag_no][k];
                //   }
                  
                  
                //   cout<<"new tag"<<endl;
                //   cout<<cache[i].get_tag()<<endl;
                //   cout<<"new data"<<endl;
                //   for (int k=0; k<8;k++){
                //       cout<<cache[i].get_data(k);
                //   }
                  
                  
                  //print results, data, miss, previous dirty bit 
                //   cout<<"data read"<<endl;
                //   cout<<cache[i].get_data(offset);
               //     cout<<"cache read miss"<<endl;
                  cout<<cache[i].get_data(offset)<<" "<<"0 "<<previous_dirty_bit<<endl;
                  operation="";
             //     cout<<"dirty bit is going to be reset"<<endl;
                  
                  //reset drity bit 
                  cache[i].reset_dirty_bit();
                  
                  i=33;
                  
              }
          }//end of for loop
            
      }//end of read operation 
      
        
    } //end of while loop
    
    
    fin.close();
    fout.close();
    
    
    
    
    
    
    
    return 0;
}
