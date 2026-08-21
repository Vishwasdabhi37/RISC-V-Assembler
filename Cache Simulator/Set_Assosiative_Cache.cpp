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
int SET;
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
vector<vector<CacheBlock>> cache;

int getreplacementind(int i)
{
    for (int j = 0; j < cache[i].size(); j++)
    {
        if (cache[i][j].state == 0)
            return j;
    }
    return rand() % cache[i].size();
}
pair<int, bool> read(int address)
{
    int blockadd = address / BLOCK_SIZE;
    int blockoff = address % (BLOCK_SIZE);
    int tag = blockadd / SET;
    int index = blockadd % SET;
    // blockoff /= 4;

    for (auto &&it : cache[index])
    {
        if (it.state == 2 && it.tag == tag)
        {
            return {it.values[blockoff], true};
        }
    }
    int ind = getreplacementind(index);
    if (cache[index][ind].state == 2 && cache[index][ind].dirty)
    {
        int temptag = cache[index][ind].tag;
        int tempadd = temptag;
        for (int i = 0; i < cache[index][ind].values.size(); i++)
            MainMemory[tempadd].values[i] = cache[index][ind].values[i];
        cache[index][ind].dirty = false;
    }
    cache[index][ind].state = 1;
    cache[index][ind].tag = tag;
    for (int i = 0; i < cache[index][ind].values.size(); i++)
        cache[index][ind].values[i] = MainMemory[blockadd].values[i];
    cache[index][ind].state = 2;
    return {cache[index][ind].values[blockoff], false};
}

bool writethrough(int address, int val)
{
    int blockadd = address / BLOCK_SIZE;
    int blockoff = address % BLOCK_SIZE;
    // blockoff /= 4;
    int tag = blockadd / SET;
    int index = blockadd % SET;
    bool flag = false;
    for (auto &&it : cache[index])
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
        ind = getreplacementind(index);
        if (cache[index][ind].state == 2 && cache[index][ind].dirty)
        {
            int temptag = cache[index][ind].tag;
            int tempadd = temptag;
            for (int i = 0; i < cache[index][ind].values.size(); i++)
                MainMemory[tempadd].values[i] = cache[index][ind].values[i];
            cache[index][ind].dirty = false;
        }
        cache[index][ind].state = 1;
        cache[index][ind].tag = tag;
        for (int i = 0; i < cache[index][ind].values.size(); i++)
            cache[index][ind].values[i] = MainMemory[blockadd].values[i];
        cache[index][ind].state = 2;
    }
    return false;
}
bool writeback(int address, int val)
{
    int blockadd = address / BLOCK_SIZE;
    int blockoff = address % BLOCK_SIZE;
    // blockoff /= 4;
    int tag = blockadd / SET;
    int index = blockadd % SET;
    bool flag = false;
    for (auto &&it : cache[index])
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
        ind = getreplacementind(index);
        if (cache[index][ind].state == 2 && cache[index][ind].dirty)
        {
            int temptag = cache[index][ind].tag;
            int tempadd = temptag;
            for (int i = 0; i < cache[index][ind].values.size(); i++)
                MainMemory[tempadd].values[i] = cache[index][ind].values[i];
            cache[index][ind].dirty = false;
        }
        cache[index][ind].state = 1;
        cache[index][ind].tag = tag;
        for (int i = 0; i < cache[index][ind].values.size(); i++)
            cache[index][ind].values[i] = MainMemory[blockadd].values[i];
        cache[index][ind].values[blockoff] = val;
        cache[index][ind].state = 2;
    }
    return false;
}
int main()
{
    freopen("input_file.txt", "r", stdin);
    cin >> CACHE_SIZE >> NUMBER_OF_BLOCKS >> BLOCK_SIZE >> ASSOCIATIVITY;
    SET = NUMBER_OF_BLOCKS / ASSOCIATIVITY;
    cache.resize(SET);
    for (int i = 0; i < SET; i++)
        cache[i].resize(ASSOCIATIVITY);
    BLOCK_SIZE /= 4;
    for (int i = 0; i < SET; i++)
    {
        for (int j = 0; j < ASSOCIATIVITY; j++)
        {
            cache[i][j].state = 0;
            cache[i][j].tag = -1;
            cache[i][j].dirty = false;
            cache[i][j].values.resize(BLOCK_SIZE);
        }
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
    int check, address;
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