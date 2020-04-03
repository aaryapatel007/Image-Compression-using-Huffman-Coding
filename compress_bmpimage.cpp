#include <bits/stdc++.h>
using namespace std;
int bit_counter;
char current_byte='\0';
int n;
FILE* compressedFile;
class pixel
{
public:
    int r;
    int g;
    int b;
    int hex;
    vector<int> bitcode;
    int getHex()
    {
        return hex;
    }
    void setHex(int val)
    {
        if(hex>0)
        {
            hex=val;
        }
    }
    pixel()
    {
        r=0;
        g=0;
        b=0;
        hex=0;
    }
    void   createPixel(int a,int b,int c)
    {
        r=a;
        g=b;
        b=c;
        hex=calculateRGB(r,g,b);
    }
    int calculateRGB(int r, int g, int b)
    {
        return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
    }
};

class color
{
   public:
    int hex;
    int frequency;
    vector<int> bit;
    color * left,* right;
    color()
    {
        hex=0;
        frequency=0;
    }

    void setFrequency(int hexcode, int freq)
    {
        hex = hexcode;
        frequency = freq;
    }

    int getHex()
    {
        return hex;
    }

    int getFreq()
    {
        return frequency;
    }
}arr[100];

struct imageInfo
{
    int width;
    int height;
    int sizeim;
    pixel imageMatrix[500];
};

imageInfo readBMP()
{
    char filename[]="Test2.bmp";
    FILE* f = fopen(filename, "r");
    int bmpHeader[54];
    imageInfo bmp;
    for(int i=0;i<54;i++)
    {
        bmpHeader[i]=getc(f);
    }
    bmp.width = *(int*)&bmpHeader[18];
    bmp.height = *(int*)&bmpHeader[22];
    bmp.sizeim = 3 *bmp. width *bmp. height;
    int count=0;
    int r=0;
    int g=0;
    int b=0;
    for(int i=0;i<bmp.sizeim/3;i++)
    {
        b=getc(f);

        g=getc(f);

        r=getc(f);

        bmp.imageMatrix[i].createPixel(r,g,b);

        count++;
    }
    return bmp;
}

void initCompressedFile()
{
    int numberOfColors;
    compressedFile = fopen("Test2.cmpbmp","a");
    current_byte='\0';
}


bool comparePixel(pixel p1, pixel p2)
{
    return p1.getHex() < p2.getHex();
}

bool compareFreq(color f1, color f2)
{
    return f1.getFreq() < f2.getFreq();
}

int getFrequency(imageInfo info, int n, unordered_map<int, int> &freq)
{
    int low = 0, high = n-1, i = 0;

    while(low <= high)
    {
        if(info.imageMatrix[low].getHex() == info.imageMatrix[high].getHex())
        {
            freq[info.imageMatrix[low].getHex()] += high - low + 1;
            low = high + 1;
            high = n-1;
        }
        else
            high = (low + high)/2;
    }

    return freq.size();
}
struct compare
{
    bool operator()(color * a,color * b)
    {
        return(b->frequency < a->frequency);
    }

};
color * makenode(int hex, int frequency)
{
    color * temp = new color;
    temp->hex = hex;
    temp->frequency = frequency;
    temp->left = temp->right = NULL;
    return temp;
}
void generatecode(color * root,int code[],int index = 0)
{
    if(root->left)
    {
        code[index] = 0;
        generatecode(root->left,code,index + 1);
    }
    if(root->right)
    {
        code[index] = 1;
        generatecode(root->right,code,index + 1);
    }
    if(!root->left && !root->right)
    {

        int j = 0;
        for(int i = index - 1;i >= 0;--i)
        {
            root->bit.insert(root->bit.begin(),code[i]);

            //cout << code[i];
        }

        for(int i=0;i<n;i++)
        {
            if(arr[i].getHex() == root->getHex())
                arr[i].bit = root->bit;
        }

        /*cout << root->getFreq() << "\t" << root->getHex() << "\t";
        vector<int> :: iterator itr1;
        for(itr1 = root->bit.begin(); itr1!=root->bit.end();itr1++)
            cout<<*itr1;
        cout<<endl;
        */
    }
}

void huffmancode(color *arr,int size)
{
    color* temp1;
    list<color *>compressedList;
    int colour[size],freq[size];
    for(int i = 0;i < size;++i)
    {
        temp1 = arr+i;
        colour[i] = temp1->getHex();
        freq[i] = temp1->getFreq();
    }
    int code[16];
    color * left, * right;
    priority_queue<color *,vector<color *>,compare>pq;
    for(int i = 0;i < size;++i)
    {
        pq.push(makenode(colour[i],freq[i]));
    }
    while(pq.size() != 1)
    {
        left = pq.top();
        pq.pop();
        right = pq.top();
        pq.pop();
        color * temp = makenode('#',left->frequency + right->frequency);
        temp->left = left;
        temp->right = right;
        pq.push(temp);
    }
    generatecode(pq.top(),code);
}
void writeBitToFile(int a,FILE *f )
{
    char current_bit = (char)a;
    bit_counter++;
    current_byte=current_byte<<1;
    current_byte=current_byte | current_bit;
    if(bit_counter==8)
    {
        fwrite(&current_byte,1,1,f);
        bit_counter=0;
        current_byte='NUL';
    }
}

void writeHeader()
{
    FILE* header = fopen("Test2.cmpbmp","w");
    //fprintf(header,"%d",n);
    fwrite(&n,sizeof(int),1,header);
    for(int i=0;i<n;i++)
    {
        fwrite(&arr[i].hex,sizeof(int),1,header);
        fwrite(&arr[i].frequency,sizeof(int),1,header);

    }
    fclose(header);
}
int main()
{
    int i = 0;
    imageInfo readImage=readBMP();
    unordered_map<int, int> freq;
    unordered_map<int, vector<int>> code;
    unordered_map<int, int>:: iterator itr;
   // cout<<p->getHex();
    initCompressedFile();

    sort(readImage.imageMatrix, readImage.imageMatrix+225, comparePixel);
    n = getFrequency(readImage, 225, freq);
    cout<<"Hex \t Frequency \t bit code"<<endl;
    for(itr = freq.begin(); itr != freq.end(); itr++)
    {
        arr[i].setFrequency(itr->first, itr->second);
        i++;
    }

    sort(arr, arr+n, compareFreq);
    huffmancode(arr,n);
    for(int k=0;k<n;k++)
    {
        if(code.find(arr[k].getHex()) == code.end())
            code[arr[k].getHex()] = arr[k].bit;
    }

    for(int k = 0; k<n; k++)
    {
        cout << arr[k].getHex() << "\t" << arr[k].getFreq() << "\t";
        vector<int> :: iterator itr1;
        for(itr1 = arr[k].bit.begin(); itr1 != arr[k].bit.end(); itr1++)
            {
                //writeBitToFile(*itr1);
                cout << *itr1;
            }
        cout << endl;
    }
    writeHeader();
    cout<<"Bit Stream"<<endl;
    for(int k=0;k<readImage.sizeim/3;k++)
    {
       vector<int> g = code[readImage.imageMatrix[k].getHex()];
       vector<int> :: iterator itr1;
        for(itr1 = g.begin(); itr1 != g.end(); itr1++)
            {
                writeBitToFile(*itr1,compressedFile);
                cout << *itr1;
            }
    }

    fclose(compressedFile);
    fwrite(&current_byte,1,1,compressedFile);
    /*for(i = 0; i<n; i++)
    {
        cout << arr[i].getHex() << "\t" << arr[i].getFreq() << endl;
    }*/
    return 0;
}
