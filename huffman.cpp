#include<iostream>
#include<string>
#include<vector>
#include<bits/stdc++.h>
#include<fstream>
#include<map>
#include<unordered_map>
using namespace std;
map<char,string>m;
map<string,char>ma;
string strToBinary(string s)
{
    int n = s.length();
    string out;
    for (int i = 0; i <n; i++)
    {
        unsigned char ch=s[i];
        int val=(int)ch;
        string bin;
        while (val > 0)
        {
            (val % 2)? bin.push_back('1') :
                       bin.push_back('0');
            val /= 2;
        }
        int m=8-bin.size();
        for(int j=0;j<m;j++)
            bin=bin+"0";
        out+=bin;
    }
    return out;
}
class node
{
public:
    int freq;
    char symbol;
    string code;
    node *left,*right;
    node(int f,char s,node *l=NULL,node *r=NULL)
    {
        freq=f;
        symbol=s;
        left=l;
        right=r;
    }
};
void getcodes(node * top,string prefix="")
{
    if(top->left==NULL && top->right==NULL)
    {
        top->code=prefix;
        cout<<top->symbol<<"->"<<top->code<<"\n";
        m.insert({top->symbol,prefix});
        ma.insert({prefix,top->symbol});
        return;
    }
    if(top->left!=NULL)
    {
        getcodes(top->left,prefix+"0");
    }
    if(top->right!=NULL)
    {
        getcodes(top->right,prefix+"1");
    }
}
void heapify(vector<node*>&arr, int i)
{
    int n=arr.size();
    int smallest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;
    if (l < n && arr[l]->freq < arr[smallest]->freq)
        smallest = l;
    if (r < n && arr[r]->freq < arr[smallest]->freq)
        smallest = r;
    if (smallest != i)
    {
        node *t=arr[i];
        arr[i]=arr[smallest];
        arr[smallest]=t;
        heapify(arr, smallest);
    }
}
void buildHeap(vector<node*> &arr,int n)
{
    for(int i=n/2;i>0;i--)
        heapify(arr,i);
}
node* extractMin(vector<node*>&arr)
{
    int heap_size=arr.size();
    if (heap_size <= 0)
        return NULL;
    node *root=arr[0];
    arr[0] = arr[heap_size-1];
    arr.pop_back();
    heapify(arr,0);
    return root;
}
void insertKey(vector<node*>&arr,node* x)
{
    arr.push_back(x);
    int i=arr.size()-1;
    while(i>0 && arr[(i-1)/2]>arr[i])
    {
        swap(arr[i],arr[(i-1)/2]);
        i=(i-1)/2;
    }
}
node* huffmancode(vector<char>c, vector<int>f,int n)
{
    vector<node*> nodes;
    for(int i=0;i<n;i++)
        nodes.push_back(new node(f[i],c[i]));
    node *l,*r,*p;

    for(int i=0;i<n-1;i++)
    {
        l=extractMin(nodes);
        r=extractMin(nodes);
        p=new node(l->freq+r->freq,'$',l,r);
        insertKey(nodes,p);
    }
    p=nodes.front();
    return p;
}
void getBackString(string code,int n)
{
    fstream extracted;
    extracted.open("extracted.txt",ios::out);
    cout<<endl<<n<<endl;
    //node*curr=root;
    int i=0;
    cout<<endl;
    string s="";
    while(i<n)
    {
        s+=code[i];

         map<string,char>::iterator it=ma.find(s);
        if(it==ma.end())
        {
            i++;

        }
        else
        {
          extracted<<it->second;
          //cout<<s<<" "<<it->second<<endl;
          //cout<<s<<" "<<it->second<<endl;
          s="";
          i++;
        }

    }

    extracted.close();
    return;
}

int main()
{
    fstream inFile;
    inFile.open("input.txt",ios::in);
    string data,temp;
    while(getline(inFile,temp))
    {

        if(temp.size()==0)
            getline(inFile,temp);
        else
            data+=temp+'\n';
    }
    inFile.close();
    inFile>>data;
    //data="bcada";
    cout<<data<<endl;
    int l=data.size();
    map<char,int>mp;
    for(int i=0;i<l;i++)
    {
        char ch=data[i];
        map<char,int>::iterator it=mp.find(ch);
        if(it==mp.end())
        {
            mp.insert({ch,1});
        }
        else
        {
            it->second++;
        }
    }
    map<char,int>::iterator it;
    vector<char>keys;
    vector<int>values;
    cout<<"frequencies of each character"<<endl;
    for(it=mp.begin();it!=mp.end();it++)
    {
        cout<<it->first<<"->"<<it->second<<endl;
        keys.push_back(it->first);
        values.push_back(it->second);
    }
    cout<<endl<<endl;
    node* root=huffmancode(keys,values,keys.size());
    cout<<"huffmann codes of each character"<<endl;
    getcodes(root);
    ofstream out;
    out.open("out",ios::binary| ios::out);
        bitset<8>f;
        int index=0;
        int msize=0;
    for(int i=0;i<l;i++)
    {
        char ch=data[i];
        int le=m[ch].size();
        msize+=le;
        //cout<<m[ch]<<"";
        for(int j=0;j<le;j++)
        {
            if(m[ch][j]==49)
            {
               // cout<<m[ch][j];
                f[index]=1;

            }
            index++;
            if(index==8)
            {

                unsigned int num=f.to_ulong();
                //cout<<num;
                char ch=(char)num;
                out.write(&ch,sizeof(ch));
                index=0;
                f.reset();
            }
        }
    }
    if(index!=0)
    {
        int num=0;
        int b=1;
        for(int k=index-1;k>=0;k--)
        {
            num+=f[k]*b;
            b*=2;
        }
        char ch=(char)num;
        cout<<endl;
        //cout<<"number="<<ch<<endl;
                out.write(&ch,sizeof(ch));
                index=0;
                f.reset();

    //cout<<"\nnum"<<num;

    }
    cout<<endl;
    out.close();
    fstream compressed;
    compressed.open("out",ios::binary | ios::in);
    string buffer,ans;
    compressed>>buffer;
    cout<<buffer.size()<<endl;
    ans=strToBinary(buffer);
//cout<<ans<<" "<<ans.size();
cout<<ans.size()<<endl;
getBackString(ans,msize);
    return 0;
}
