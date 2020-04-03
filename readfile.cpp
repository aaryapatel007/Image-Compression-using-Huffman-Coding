#include<bits/stdc++.h>

using namespace std;

int main()
 {
    int no_of_colors=0;
    FILE* f = fopen("Test2.cmpbmp","r");

    fread(&no_of_colors,1,sizeof(int),f);
    int current_hex=0;
    int current_frequency=0;
    cout<<"Header : "<<endl;
    cout<<"No of distinct colors : "<<no_of_colors<<endl;
    cout<<"Hex code "<<"  \tFrequency"<<endl;
    for(int i =0;i<no_of_colors;i++)
    {
        fread(&current_hex,1,sizeof(int),f);
        fread(&current_frequency,1,sizeof(int),f);
        cout<<current_hex<<"\t\t"<<current_frequency<<endl;
    }
    cout<<"Bit Stream : "<<endl;
    while(!feof(f))
    {
        int x = getc(f);
        string b = bitset<8>(x).to_string();
        cout<<b;
    }
    return 0;
 }
