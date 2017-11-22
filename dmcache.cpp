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
            binary="0010";
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
    void set_data(int offset1, int offset2, char temp);
    void print_data(int offset);

   
    private:
    bool dirty_bit; //dirty bit
    int line_number;//line number of cache line
    char data[8][8]; // data bytes within cache line 
    
};

//cache line constructor

cache_line::cache_line(){
    //initialize values
    dirty_bit = false;
    line_number=-1;
    
    //set all bits within data bytes to 0
    for (int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            data[i][j]=0;
        }
    }
    
    
    
}

//set dirty bit function

void cache_line::set_dirty_bit(){
    dirty_bit=true;
}

//reset dirty bit function

void cache_line::reset_dirty_bit(){
    dirty_bit=false; 
}


//set line number function 
void cache_line::set_line_number(int line_no){
    line_number = line_no;
}

int cache_line::get_line_number(){
    return line_number;
}

void cache_line::set_data(int offset1, int offset2, char temp){
    data[offset1][offset2] = temp;
    
}

void cache_line::print_data(int offset){
    cout<<data[offset]<<endl;
}



int main(int argc, char**argv){
    
    //initialize 32 cache line objects 
    cache_line cache[32];
    
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
        
        for (int i=0; i<16; i++){
            address_binary[i]=binary_pattern[i];
        }
        
       binary_pattern="";
       current_bits="";
       
       
       //convert data to binary
       for (int i=0; i<2; i++){
           current_bits=hextobinary(data_hex[i]);
           binary_pattern += current_bits;
       }
       
       //store binary pattern into data
       
       for(int i=0; i<8; i++){
           data_binary[i] = binary_pattern[i];
           
       }
       
       binary_pattern="";
       current_bits="";
       
       //save operation
       operation=op_hex;
       
       
       //get offset
       string offset_pattern;
       offset_pattern += address_binary[13];
       offset_pattern += address_binary[14];
       offset_pattern += address_binary[15];
       
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
       
       
       
       
       //done converting to bits
       
       
       //write operation
       if(operation=="FF"){
           
           for (int i=0; i<32; i++){
               //found corresponding cache line 
               if (cache[i].get_line_number() == line_no){
                   //write data to offset of memory block
                   for(int j=0; j<8; j++){
                       cache[i].set_data(offset,j,data_binary[j]);
                       //cache[i].data[offset][j]=data_binary[j];
                   }
                   cache[i].set_dirty_bit();
                   cout<<"testing to see if data was written correctly"<<endl;
                   cache[i].print_data(offset);
                 //  cout<<cache[i].data[offset]<<endl;
                   break;
               }
           }
           
       }
       
       break;
       
       
       
       
    //   //read operation
    //   else if(operation=="00"){
           
    //   }
       

        
        // for (int i=0; i<16; i++){
        //     address_binary[i]
        // }
        
    }
    //end of while loop
    
    
    
    
    
    
    
    
    
    
    return 0;
}
