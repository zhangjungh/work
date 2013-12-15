#include <iostream> 
#include <fstream> 
#include <algorithm> 
#include <vector> 
//#include "Winbase.h" 
 
using namespace std; 
 
int main() 
{ 
 
    // init six number of die 
    std::vector<int> vDie; 
    for (int i=1; i<=6; i++)    { 
        vDie.push_back(i); 
    } 
    std::vector<int> vChance; 
    for (int i=1; i<=10; i++)    { 
        vChance.push_back(i); 
    } 
 
    cout << "Welcome to Dice, User start first " << endl; 
    cout << "Input S to START and Q to QUIT" << endl; 
 
    bool bUserFirst = true; 
    int nRound = 1; 
    char c; 
    cin >> c; 
    while ( c != 'Q' ) 
    { 
        int v[4]; 
        for (int i=0; i<4; i++) 
        {     
            std::random_shuffle( vDie.begin(), vDie.end() ); 
            v[i] = (*vDie.begin()); 
        } 
 
        std::random_shuffle( vChance.begin(), vChance.end() ); 
        bool bComputerWin = (*vChance.begin() <= 7); 
         
        // which two are greater 
        int nGreat = -1; // -1 means draw 
        if (v[0] == v[1]) 
        { 
            if (v[2] == v[3]) // both double 
            { 
                if (v[2] == v[0]) // all the same 
                    nGreat = -1; 
                else if (v[2] > v[0]) 
                    nGreat = 2; 
                else 
                    nGreat = 0; 
            } 
            else // first is double 
                nGreat = 0; 
        } 
        else 
        { 
            if (v[2] == v[3]) // second is double 
                nGreat = 2; 
            else // no double for both players 
            { 
                if (v[0]+v[1] == v[2]+v[3]) //same total 
                { 
                    int v0m = v[0] > v[1] ? v[0] : v[1]; 
                    int v2m = v[2] > v[3] ? v[2] : v[3]; 
                     
                    if (v0m == v2m) //the larger in two group are the same 
                        nGreat = -1; 
                    else if (v0m > v2m) //highest number on single die 
                        nGreat = 0; 
                    else 
                        nGreat = 2; 
                } 
                else if (v[0]+v[1] > v[2]+v[3]) 
                    nGreat = 0; 
                else 
                    nGreat = 2; 
            } 
 
        } 
        //end which two are greater 
 
        int vUser[2]; 
        int vComp[2]; 
        if (nGreat == -1) 
        { 
            vUser[0] = v[0], vUser[1] = v[1]; 
            vComp[0] = v[2], vComp[1] = v[3]; 
        } 
        else if (nGreat == 0) 
        { 
            if (bComputerWin) 
            { 
                vComp[0] = v[0], vComp[1] = v[1]; 
                vUser[0] = v[2], vUser[1] = v[3]; 
            } 
            else 
            { 
                vUser[0] = v[0], vUser[1] = v[1]; 
                vComp[0] = v[2], vComp[1] = v[3]; 
            } 
        } 
        else 
        { 
            if (bComputerWin) 
            { 
                vUser[0] = v[0], vUser[1] = v[1]; 
                vComp[0] = v[2], vComp[1] = v[3]; 
            } 
            else 
            { 
                vComp[0] = v[0], vComp[1] = v[1]; 
                vUser[0] = v[2], vUser[1] = v[3]; 
            } 
        } 
 
        //Sleep(5000); 
        if (bUserFirst) 
        {
            cout << "You have a "  << endl;
            cout << "You have a " <<  vUser[0] << " and a " << vUser[1] <<" in round "<< nRound << endl;
            cout << "Computer has a " <<  vComp[0] << " and a " << vComp[1] <<" in round "<< nRound << endl; 
        } 
        else 
        {
            cout << "Computer has a " <<  vComp[0] << " and a " << vComp[1] <<" in round "<< nRound << endl; 
            cout << "You have a " <<  vUser[0] << " and a " << vUser[1] <<" in round "<< nRound << endl;
        }
             
 
 
        if (nGreat == -1) 
            cout << "The Game is Draw" << endl; 
        else if (bComputerWin) 
            cout << "You lose" << endl; 
        else 
            cout << "You win" << endl; 
 
        bUserFirst = !bUserFirst; 
        nRound++; 
 
        cin >> c; 
 
    } 
    system("pause");
    return 0; 
 
}  
