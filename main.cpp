//
//  main.cpp
//  Tortoise and the Hare Simulation
//
//  Created by Max on 26/04/2021.
//

#include <iostream>
#include <unistd.h>


using namespace std;

//parent class
class Racer {

protected:
    int pos = 0, posBias = 0;
    bool finished = false;
    
    string drawing[4]; //stores racer's drawing

public:
    Racer (){
        
    }
    
    virtual string * getAsText(){
        
        return drawing;
    }
    
    virtual int getDrawingLen(){
        
        return int(drawing[0].length());
    }
    
    int getPos(){
        
        return pos;
    }
    
    /**
     function handles movements. Makes them more smooth.
     */
    virtual void updatePos(){
        
        if(posBias != 0){
                        
            if(posBias < 0){
                
                pos--;
                posBias++;
            }
            
            else{
                
                pos++;
                posBias--;
            }
        }
        
        if(pos < 0)
            pos = 0;
    }
    
    bool won(){ //crossed finish
        
        return pos >= 70;
    }
    
    bool done(){ //reached end of track
        
        return pos >= 90;
    }
};

class Hare : public Racer  {

public:
    Hare (){
        
        drawing[0] = "    \\    ";
        drawing[1] = "     \\_  ";
        drawing[2] = "  .---(')";
        drawing[3] = "o( )_-\\_ ";
    }
    
    /**
     move function does not actually change position directly. It sets position bias that is later handled by updatePos() function.
     */
    virtual void move(){

        if(finished)
            return;
        
        if(won() && pos < 90){
            
            posBias = 90 - pos;
            finished = true;
            return;
        }
        
        int ran = rand() % 10;

        if(ran >= 0 && ran <= 1) //sleep 20%
            posBias += 0;

        else if(ran >= 2 && ran <= 3) //big hop 20%
            posBias += 9;

        else if(ran == 4) //big slip 10%
            posBias += -12;

        else if(ran >= 5 && ran <= 7) // small hop 30%
            posBias += 1;

        else if(ran >= 8 && ran <= 9) // small slip 20%
            posBias += -2;
        
        cout << posBias << endl;
    }
};



class Tortoise : public Racer{
    
public:
    Tortoise (){
        
        drawing[0] = "    ,-\"\"\"-.      ";
        drawing[1] = "  _/ \\___/ \\_.oo ";
        drawing[2] = "(_ /_/___\\__(___)";
        drawing[3] = "  /_//   \\\\_\\    ";
    }
    
    /**
     move function does not actually change position directly. It sets position bias that is later handled by updatePos() function.
     */
    virtual void move(){
        
        if(finished)
            return;
        
        if(won() && pos < 90){
            
            posBias = 90 - pos;
            finished = true;
            return;
        }

        int ran = rand() % 10;

        if(ran >= 0 && ran <= 4) //fast plod 50%
            posBias += 3;

        else if(ran >= 5 && ran <= 6) //slip 20%
            posBias += -6;

        else if(ran >= 7 && ran <= 9) //slow plod 30%
            posBias += 1;
    }
};



void printTrack(Tortoise tort, Hare hare, int timer);

void delay(int amount); //delay in millisecconds
void initializeDigits();

//digits 0-9 and G for printing in console
string *digits[11];


int main(int argc, const char * argv[]) {
    
    initializeDigits();

    //Start condition
    cout << "\n\n\n--------\nSet terminal window size to 127x23\nPress X to start ";
    for(char inp = 'a'; inp != 'X'; cin >> inp);
    
    //Initialize objects
    Tortoise t = Tortoise();
    Hare h = Hare();
    
    //counts time ellapsed
    int timer = 0;

    //run race while no one is done
    while(!t.done() && !h.done()){
        
        for(int i = 0; i < 10; i++){
            
            printTrack(t, h, timer);
            
            t.updatePos(); //updated 10 times a second for more smoothness
            h.updatePos();
            
            if(!t.won() && !h.won())
                timer += 100;
            
            delay(100);
        }
        
        t.move(); //occurs once a second as prescribed
        h.move();
    }

    return 0;
}

/**
 adds string to track
 from line start, to line end
 */
void addSymbol(string * track, string s, int start, int end){
    
    for(int i = start; i < end; i++)
        track[i] += s;
}


/**
 adds text to track.
 spacing parameter can be used to add spaces between label letters
 */
void addLabel(string * track, string label, int spacing){
    
    int letCount = int(label.length());
    int spaceCount = (letCount  - 1) * spacing;
        
    while(letCount + spaceCount > 13 && spacing > 0)
        spaceCount = (letCount  - 1) * --spacing;
    
    string spacedWord;
    
    for(int i = 0; i < letCount; i++){
        
        spacedWord += label.at(i);
        
        if(i + 1 != letCount)
            for(int j = 0; j < spacing; j++)
                spacedWord += " ";
    }
    
    for(int i = 0; spacedWord.length() <= 13; i++){

        if(i % 2)
            spacedWord = " " + spacedWord;
        else
            spacedWord += " ";
    }
        
    for(int i = 0; i < 13; i++)
        track[i + 1] += spacedWord.at(i);
}


/**
 generates and prints track header with timer
 */
void printTimeHeader(int milisecEllapsed){
    
    string header[7];
    
    addSymbol(header, "|", 0, 7);
    
    for(int i = 0; i < 125; i++){
        
        header[0] += "=";
        header[1] += " ";
        header[2] += " ";
        header[3] += " ";
        header[4] += " ";
        header[5] += " ";
        header[6] += " ";
    }
    
    int sec = milisecEllapsed / 1000;
    int deciCes = milisecEllapsed / 100 % 10;

    //decisecond digit (after dot)
    header[2].replace(67, 5, digits[deciCes][0]);
    header[3].replace(67, 5, digits[deciCes][1]);
    header[4].replace(67, 5, digits[deciCes][2]);
    header[5].replace(67, 5, digits[deciCes][3]);

    //dot
    header[5].replace(65, 1, "*");

    //first second digit
    header[2].replace(60, 5, digits[sec % 10][0]);
    header[3].replace(60, 5, digits[sec % 10][1]);
    header[4].replace(60, 5, digits[sec % 10][2]);
    header[5].replace(60, 5, digits[sec % 10][3]);

    //second second digit
    if(sec >= 10){
        
        header[2].replace(54, 5, digits[(sec / 10) % 6][0]);
        header[3].replace(54, 5, digits[(sec / 10) % 6][1]);
        header[4].replace(54, 5, digits[sec / 10 % 6][2]);
        header[5].replace(54, 5, digits[sec / 10 % 6][3]);
    }
    
    //minute digit
    if(sec >= 60){
        
        header[2].replace(46, 5, digits[(sec / 60) % 10][0]);
        header[3].replace(46, 5, digits[(sec / 60) % 10][1]);
        header[4].replace(46, 5, digits[sec / 60 % 10][2]);
        header[5].replace(46, 5, digits[sec / 60 % 10][3]);
    
        header[3].replace(52, 1, "*");
        header[5].replace(52, 1, "*");
    }

    addSymbol(header, "|", 0, 7);

    //print each line to console
    for(int i = 0; i < 7; i++)
        cout << header[i] << endl;
}


/**
 prints track to the console
 */
void displayTrack(string * track, int timer){
    
    printTimeHeader(timer);
    
    //print each line to console
    for(int i = 0; i < 15; i++)
        cout << track[i] << endl;

}


bool countdown = true; //used to initial before race countdown
/**
 this function generates track image and prints in to console
 */
void printTrack(Tortoise tort, Hare hare, int timer){
        
    string * track = new string[15]; //stores track image
        
    //Here happens the creation of track image from left to right
    addSymbol(track, "|", 0, 15);
    addSymbol(track, "                   ", 1, 14);
    addSymbol(track, "|", 1, 14);
    
    addLabel(track, "START", 1); //add start label
    
    //adds 10, 20....60 lines on the track
    for(int i = 1; i < 7; i++){
        
        addSymbol(track, "        ", 1, 14);
        addSymbol(track, "|", 1, 14);
        addLabel(track, to_string(i * 10), 1);
    }
    
    addSymbol(track, "        ", 1, 14);
    addSymbol(track, "|", 1, 14);
    addLabel(track, "FINISH", 1); //add finish label
    addSymbol(track, "|", 1, 14);

    addSymbol(track, "                   ", 1, 14); //add some space after finish line

    for(int i = 0; i < 111; i++){ //add borders
        
        track[0] += "=";
        track[14] += "=";
    }
    
    addSymbol(track, "|", 0, 15);
    
    track[7].replace(23, 67, "-------------------------------------------------------------------"); //add middle line
    

    for(int i = 0; i < 13; i++){ //add space for score counters
        
        track[0] += "=";

        track[1] += " ";
        track[2] += " ";
        track[3] += " ";
        track[4] += " ";
        track[5] += " ";
        track[6] += " ";

        track[7] += "-";

        track[8] += " ";
        track[9] += " ";
        track[10] += " ";
        track[11] += " ";
        track[12] += " ";
        track[13] += " ";

        track[14] += "=";
    }


    addSymbol(track, "|", 0, 15);

    //add hare and turtose
    for(int i = 0; i < 4; i++){
        
        track[i + 2].replace(tort.getPos() + 3, tort.getDrawingLen(), tort.getAsText()[i]);
        track[i + 9].replace(hare.getPos() + 11, hare.getDrawingLen(), hare.getAsText()[i]);
    }
    
    //add score counters
    string * digit1, *digit2;
    
    //if now is before race start, prints 3, 2, 1, GO in every scoreboard
    if(countdown){
        
        for(int i = 0; i < 3; i++){
            
            digit1 = digits[3 - i]; //digits from 3 to 1

            for(int j = 0; j < 4; j++){ //add every line of digit
                
                track[j + 2].replace(117, 5, digit1[j]);
                
                track[j + 9].replace(117, 5, digit1[j]);
            }

            displayTrack(track, timer); //print

            delay(1000);
        }
        
        for(int i = 0; i < 4; i++){ //add "GO"
            
            track[i + 2].replace(114, 5, digits[10][i]);
            track[i + 2].replace(119, 1, " ");
            track[i + 2].replace(120, 5, digits[0][i]);

            
            track[i + 9].replace(114, 5, digits[10][i]);
            track[i + 9].replace(119, 1, " ");
            track[i + 9].replace(120, 5, digits[0][i]);

        }
        
        displayTrack(track, timer);

        delay(1000);
        
        countdown = false;
    }
    
    //add tortoise score
    if(!tort.won()){
        

        digit1 = digits[tort.getPos() / 10];
        digit2 = digits[tort.getPos() % 10];

        if(tort.getPos() && tort.getPos() == hare.getPos()) //if they on the same pos
            track[3].replace(117, 5, "OUCH!");
        
        else
            for(int i = 0; i < 4; i++){

                track[i + 2].replace(114, 5, digit1[i]);
                track[i + 2].replace(120, 5, digit2[i]);
            }

        //add tortoise treat
        track[3].replace(100, 4, "(~~)");
        track[4].replace(98, 8, "(~~)(~~)");
        track[5].replace(100, 4, "(~~)");
    }
    
    //add tortoise score
    if(!hare.won()){
            
        digit1 = digits[hare.getPos() / 10];
        digit2 = digits[hare.getPos() % 10];
        
        if(hare.getPos() && hare.getPos() == tort.getPos()) //if they on the same pos
            track[10].replace(117, 5, "OUCH!");
        
        else
            for(int i = 0; i < 4; i++){
                
                track[i + 9].replace(114, 5, digit1[i]);
                track[i + 9].replace(120, 5, digit2[i]);
            }

        //add hare treat
        track[10].replace(100, 4, "_\\/_");
        track[11].replace(100, 4, "\\  /");
        track[12].replace(100, 4, " \\/ ");
    }
    
    //win/lose/tie handling
    if(hare.won() && tort.won()){
        
        track[3].replace(118, 3, "TIE");
        track[10].replace(118, 3, "TIE");
    }
    
    else if(hare.won()){
        
        track[10].replace(117, 6, "WINNER");
    }
    else if(tort.won()){
        
        track[3].replace(117, 6, "WINNER");
    }
    
    displayTrack(track, timer);
    
    delete [] track; //clear memory!
}


/**
 delay in milliseconds
 */
void delay(int amount){
    
    usleep(amount * 1000);
}

/**
 big digits for console and G letter for printing "GO"
 */
void initializeDigits(){
    
    for(int i = 0; i < 11; i++)
        digits[i] = new string[4];

    digits[0][0] = "  ## ";
    digits[0][1] = "#   #";
    digits[0][2] = "#   #";
    digits[0][3] = " ##  ";

    digits[1][0] = "   ##";
    digits[1][1] = " # # ";
    digits[1][2] = "  #  ";
    digits[1][3] = "#### ";

    digits[2][0] = " ### ";
    digits[2][1] = "   _#";
    digits[2][2] = " #   ";
    digits[2][3] = "#### ";

    digits[3][0] = " ### ";
    digits[3][1] = " ___#";
    digits[3][2] = "   # ";
    digits[3][3] = "###  ";

    digits[4][0] = " #  #";
    digits[4][1] = "#__# ";
    digits[4][2] = "  #  ";
    digits[4][3] = " #   ";

    digits[5][0] = " ####";
    digits[5][1] = "#__  ";
    digits[5][2] = "   # ";
    digits[5][3] = "###  ";

    digits[6][0] = "  ## ";
    digits[6][1] = "#___ ";
    digits[6][2] = "#   #";
    digits[6][3] = " ##  ";

    digits[7][0] = " ####";
    digits[7][1] = "   # ";
    digits[7][2] = "  #  ";
    digits[7][3] = " #   ";

    digits[8][0] = " ####";
    digits[8][1] = "#___#";
    digits[8][2] = "#   #";
    digits[8][3] = "#### ";

    digits[9][0] = " ####";
    digits[9][1] = "#___#";
    digits[9][2] = "   # ";
    digits[9][3] = "###  ";
    
    digits[10][0] = "  ###";
    digits[10][1] = " #  _";
    digits[10][2] = "#   #";
    digits[10][3] = " ### ";
}

