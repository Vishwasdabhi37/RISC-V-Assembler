#include <bits/stdc++.h>
using namespace std;

struct MemBlock
{
    vector<int> values;
};
struct CacheBlock
{
    int state; // 0 - invalid, 1 to misspending, 2 valid
    int tag;
    bool dirty;
    vector<int> values;
};

int CACHE_SIZE;
int NUMBER_OF_BLOCKS;
int BLOCK_SIZE;
int ASSOCIATIVITY;
bool writethroughflag = false;
bool write_no_allocate = false;
int getDecimalU(string &s)
{
    int num = 0;
    reverse(s.begin(), s.end());
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '1')
        {
            num += (1 << i);
        }
    }
    return num;
}
vector<MemBlock> MainMemory(1024);
vector<CacheBlock> cache;

int getreplacementind()
{
    for (int i = 0; i < cache.size(); i++)
    {
        if (cache[i].state == 0)
            return i;
    }
    return rand() % cache.size();
}
pair<int, bool> read(int address)
{
    // int add = getDecimalU(address);
    int add = address;
    int blockadd = add / BLOCK_SIZE;
    int tag = blockadd;
    int blockoff = add % (BLOCK_SIZE);
    // blockoff /= 4;
    for (auto &&it : cache)
    {
        if (it.state == 2 && it.tag == tag)
        {
            return {it.values[blockoff], true};
        }
    }
    int ind = -1;
    for (int i = 0; i < cache.size(); i++)
    {
        if (cache[i].state == 0)
        {
            ind = i;
            break;
        }
    }
    if (ind == -1)
    {
        ind = getreplacementind();
    }
    if (cache[ind].state == 2 && cache[ind].dirty)
    {
        int temptag = cache[ind].tag;
        int tempadd = temptag;
        for (int i = 0; i < cache[ind].values.size(); i++)
            MainMemory[tempadd].values[i] = cache[ind].values[i];
        cache[ind].dirty = false;
    }
    cache[ind].state = 1;
    cache[ind].tag = tag;
    for (int i = 0; i < cache[ind].values.size(); i++)
        cache[ind].values[i] = MainMemory[blockadd].values[i];
    cache[ind].state = 2;
    return {cache[ind].values[blockoff], false};
}

bool writethrough(int address, int val)
{
    // int add = getDecimalU(address);
    int add = address;
    int blockadd = add / BLOCK_SIZE;
    int blockoff = add % (BLOCK_SIZE);
    // blockoff /= 4;
    int tag = blockadd;
    bool flag = false;
    for (auto &&it : cache)
    {
        if (it.state == 2 && it.tag == tag)
        {
            it.values[blockoff] = val;
            flag = true;
            break;
        }
    }
    MainMemory[blockadd].values[blockoff] = val;
    if (flag)
        return true;
    if (!write_no_allocate)
    {
        int ind;
        ind = getreplacementind();
        if (cache[ind].state == 2 && cache[ind].dirty)
        {
            int temptag = cache[ind].tag;
            int tempadd = temptag;
            for (int i = 0; i < cache[ind].values.size(); i++)
                MainMemory[tempadd].values[i] = cache[ind].values[i];
            cache[ind].dirty = false;
        }
        cache[ind].state = 1;
        cache[ind].tag = tag;
        for (int i = 0; i < cache[ind].values.size(); i++)
            cache[ind].values[i] = MainMemory[blockadd].values[i];
        cache[ind].state = 2;
    }
    return false;
}
bool writeback(int address, int val)
{
    // int add = getDecimalU(address);
    int add = address;
    int blockadd = add / BLOCK_SIZE;
    int blockoff = add % (BLOCK_SIZE);
    // blockoff /= 4;
    int tag = blockadd;
    bool flag = false;
    for (auto &&it : cache)
    {
        if (it.state == 2 && it.tag == tag)
        {
            it.values[blockoff] = val;
            it.dirty = true;
            flag = true;
            break;
        }
    }
    if (flag)
        return true;
    if (!write_no_allocate)
    {
        int ind;
        ind = getreplacementind();
        if (cache[ind].state == 2 && cache[ind].dirty)
        {
            int temptag = cache[ind].tag;
            int tempadd = temptag;
            for (int i = 0; i < cache[ind].values.size(); i++)
                MainMemory[tempadd].values[i] = cache[ind].values[i];
            cache[ind].dirty = false;
        }
        cache[ind].state = 1;
        cache[ind].tag = tag;
        for (int i = 0; i < cache[ind].values.size(); i++)
            cache[ind].values[i] = MainMemory[blockadd].values[i];
        cache[ind].values[blockoff] = val;
        cache[ind].state = 2;
    }
    return false;
}
int main()
{
    srand(time(NULL));
    freopen("input_file.txt", "r", stdin);
    cin >> CACHE_SIZE >> NUMBER_OF_BLOCKS >> BLOCK_SIZE >> ASSOCIATIVITY;
    cache.resize(NUMBER_OF_BLOCKS);
    BLOCK_SIZE /= 4;
    for (int i = 0; i < NUMBER_OF_BLOCKS; i++)
    {
        cache[i].state = 0;
        cache[i].tag = -1;
        cache[i].dirty = false;
        cache[i].values.resize(BLOCK_SIZE);
    }
    for (int i = 0; i < MainMemory.size(); i++)
    {
        MainMemory[i].values.resize(BLOCK_SIZE);
        for (int j = 0; j < BLOCK_SIZE; j++)
            MainMemory[i].values[j] = 0;
    }
    // string line;
    int cnt = 1000;
    vector<vector<int>> ins;
    for (int i = 0; i < cnt; i++)
    {
        int address = rand() % (512 * BLOCK_SIZE);
        int val = rand() % 10000;
        ins.push_back({1, address, val});
        address = rand() % (512 * BLOCK_SIZE);
        ins.push_back({0, address});
    }
    int check,address;
    int hitcount = 0, misscount = 0;
    for (int i = 0; i < ins.size(); i++)
    {
        check = ins[i][0];
        address = ins[i][1];
        if (check == 0)
        {
            auto it = read(address);
            int readval = it.first;
            bool hit = it.second;
            if (hit)
            {
                hitcount++;
                // printf("Hit.\t");
            }
            else
            {
                misscount++;
                // printf("Miss.\t");
            }
            // printf("Read %d value from %d address.\n", readval, address);
        }
        else
        {
            int val = ins[i][2];
            bool hit;
            if (writethroughflag)
                hit = writethrough(address, val);
            else
                hit = writeback(address, val);
            if (hit)
            {
                hitcount++;
                // printf("Hit in write.\n");
            }
            else
            {
                misscount++;
                // printf("Miss in write.\n");
            }
        }
    }
    double hitrate = (hitcount) / (1.0 * (hitcount + misscount));
    cout << fixed << setprecision(2) << "Hit Rate = " << hitrate << '.' << endl;
    cout << fixed << setprecision(2) << "Miss Rate = " << 1 - hitrate << '.' << endl;
    return 0;
}