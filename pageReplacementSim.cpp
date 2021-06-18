/***************************************************************************
*File: pagesComp.cpp
*Author: Kris Ables
*Procedures:
*main       - runs the experiments and prints out the results
*nullArray  - erases values in the array
*fifo       - performs the first in first out procedure on the page stream
*lru        - performs the last recently used procedure on the page stream
*clock      - performs the clock procedure on the page stream

Written in Sublime Text Editor, Tab Size 4
***************************************************************************/

#include <random>
#include <chrono>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <climits>

/**************************************************
*void nullArray(int array[], int n)
*Author: Kristopher Ables
*Date: 3 April 2021

*Description - sets an array to all null values

*Parameters:
*array[] I/P int    -The array to be totaled
*n I/P int          -Starting index
**************************************************/
void nullArray(int array[],int n)
{
    int i=0;        //begining index=0;
    while(i<n)      //while the index of the array exists
    {
        array[i]=0; //set that index to null
        i++;        //increment
    }
}

/**************************************************
*int fifo(int pgStr[],int pgStrLen, int n)
*Author: Kristopher Ables
*Date: 3 April 2021

*Description - performs the FIFO replacement algorithm 
                on the page stream, returning the total
                number of faults that have occured

*Parameters:
*pgStr I/P int[]        -array of page address stream
*pgStrLen I/P int       -length of page address stream
*n I/P int              -number of frames allocated

*totalFaults O/P int    -page faults that occured
**************************************************/
int fifo(int pgStr[],int pgStrLen, int pglen)
{
    int totalFaults=0;                          //initializes total faults

    std::set<int> pages;                        //initializes frames as a set
    std::queue<int> q;                          //initializes queue to indicate
                                                //next page to be replaced

    for(int i=0;i<pgStrLen;i++)                 //loops for the length of the input stream
    {
        if(pages.find(pgStr[i])==pages.end())   //if the current page in the page stream is not found
                                                //in the frames
        {
            if(pages.size()<pglen)              //if there are empty frames present
            {
                pages.insert(pgStr[i]);         //add current page in page stream to the frames
                q.push(pgStr[i]);               //add page to replacement queue
            }
            else                                //if page is not in frames but frames are full
                                                //(page fault occurs)
            {
                int firstIn=q.front();          //grab next object in the replacement queue
                q.pop();                        //remove said object
                pages.erase(firstIn);           //remove replaced page
                pages.insert(pgStr[i]);         //insert current page
                q.push(pgStr[i]);               //add current page to the replacement queue
                totalFaults++;                  //increment total faults
            }
        }
    }
    return totalFaults;                         //return total faults that have occured in the algorithm
}

/**************************************************
*int lru(int pgStr[],int pgStrLen, int n)
*Author: Kristopher Ables
*Date: 10 April 2021

*Description - performs the LRU replacement algorithm 
                on the page stream, returning the total
                number of faults that have occured

*Parameters:
*pgStr I/P int[]        -array of page address stream
*pgStrLen I/P int       -length of page address stream
*nglen I/P int          -number of frames allocated

*totalFaults O/P int    -page faults that occured
**************************************************/
int lru(int pgStr[],int pgStrLen, int pglen)
{
    int totalFaults=0;                                          //initializes total faults

    std::set<int> pages;                                        //initializes frames as a set
    std::map<int,int> indexes;                                  //initializes unordered map to keep the
                                                                //time of pages last accessed, 
                                                                //used to determine last recently used

    for(int i=0;i<pgStrLen;i++)                                 //loops for the length of the input stream
    {
        if(pages.find(pgStr[i])==pages.end())                   //if the current page in page stream is not found
                                                                //within the frames
        {
            if(pages.size()<pglen)                              //if there exist empty frames
            {
                pages.insert(pgStr[i]);                         //insert the page into the frames
                indexes.insert({pgStr[i],i});                   //insert iteration accessed into the map with page as the key
            }
            else                                                //if the frames are full, but the page is not present
            {
                int lastRecentIndex = INT_MAX;                  //initializes the lowest accessed iteration as integer maximum
                int remove;                                     //initializes the value of the page to be replaced
                std::set<int>::iterator it = pages.begin();
                while(it!=pages.end())                          //iterates through frame set
                {
                    if(indexes[*it]<lastRecentIndex)            //if the accessed iteration of page value is less than the current
                                                                //least recently accesseed iteration
                    {
                        lastRecentIndex=indexes[*it];           //last used iteration becomes iteration of current page
                        remove=*it;                             //sets the page that will be replaced
                    }
                    it++;                                       //iterates
                }
                pages.erase(remove);                            //removes page to be replaced from frames
                pages.insert(pgStr[i]);                         //inserts current page stream page into framse
                indexes.erase(remove);                          //removes replaced page from index map
                indexes.insert({pgStr[i],i});                   //inserts current page & iteration into index map
                totalFaults++;                                  //increase total ammount of faults that occured
            }
        }
        else                                                    //if the page is already within the frames
        {
            indexes[pgStr[i]]=i;                                //updates the iteration accessed for current page in page stream
        }
    }
    return totalFaults;                                         //returns total amount of faults that have occured
}

/**************************************************
*int clock(int pgStr[],int pgStrLen, int n)
*Author: Kristopher Ables
*Date: 14 April 2021

*Description - performs the clock replacement algorithm 
                on the page stream, returning the total
                number of faults that have occured

*Parameters:
*pgStr I/P int[]        -array of page address stream
*pgStrLen I/P int       -length of page address stream
*nglen I/P int          -number of frames allocated

*totalFaults O/P int    -page faults that occured
**************************************************/
int clock(int pgStr[],int pgStrLen, int pglen)
{
    int totalFaults = 0;                        //initializes total faults

    std::set<int> pages;                        //initializes frames as a set
    std::list<int> order;                       //initialize order of replacement as a linked list
    std::map<int,int> useBit;                   //initialized use bit as an unordered map, associating
                                                //page with its use bit

    for(int i=0; i<pgStrLen;i++)                //loops for the length of the input stream
    {
        if(pages.find(pgStr[i])==pages.end())   //if the current value of the input stream is not in pages
        {
            
            if(pages.size()<pglen)              //if the frames are not full
            {
                pages.insert(pgStr[i]);         //inserts the page into the frames
                order.push_back(pgStr[i]);      //inserts the page into the back of the order liked list
                useBit.insert({pgStr[i],1});    //inserts the bit into the map, with the page as a key and the bit initialized as 1
            }
            else                                //if the frames are full
            {
                
                auto it = order.begin();        //initialize iterator at the beginning of the order list
                int num = *it;                  //initialize number, used as the value that the iterator is pointing to
                while(useBit[num]==1)           //while the page in order is equal to 1
                {
                    useBit[num]=0;              //sets the page bit to zero, as we we have iterated over the page, exhxausting its second chance
                    it++;                       //increments iterator
                    if(it==order.end())         //if the iterator is at the end of the order list, and all values were equal to 1
                        it=order.begin();       //set iterator to beginning to loop again
                    num=*it;                    //num is equal to the iterator value
                }                     
                pages.erase(num);               //erases page to be replaced
                pages.insert(pgStr[i]);         //insert current page into frames
                useBit.erase(num);              //erases page to be replaced within use bit data structure
                useBit.insert({pgStr[i],1});    //insert use bit with the key set to the current page and initialized to 1
                order.erase(it);                //erases page to be replaced (located at iterator) from the order list
                order.push_back(pgStr[i]);      //inserts current page to the back of the replacement order
                totalFaults++;                  //increments total faults
            }
            
        }
        else                                    //if the current page is already in the frames
            useBit[pgStr[i]]=1;                 //set the current pages use bit to one, as it was accessed recently 
    }
    return totalFaults;                         //returns total amount of faults that have occured
}

/**************************************************
*int main(int argc, char*argv[])
*Author: Kristopher Ables
*Date: 3 April 2021

*Description - 

*Parameters:
*argc I/P int       -Number of arguments on the cmd line
*argv I/P char*[]   -Arguments on the cmd line

*main O/P int       -Status code
**************************************************/
int main()
{
    int fifoArray[20];                                                              //initializes array that stores fifo faults
    int lruArray[20];                                                               //initializes array that stores lru faults
    int clockArray[20];                                                             //initializes array that stores clock faults

    nullArray(fifoArray,20);                                                        //clear garbage in fifo array
    nullArray(lruArray,20);                                                         //clear garbage in lru array
    nullArray(clockArray,20);                                                       //clear garbage in clock array

    const int nrolls = 1000;                                                        //initialize number of rolls per experiment
    const int nexp = 1000;                                                          //initialize number of experiments performed

    printf("\n");                                                                   //print buffer line so console program call line not erased

    for(int a=0;a<nexp;a++)                                                         //iterates over the number of experiments
    {
        if(a%50==0)                                                                 //updates progress bar every 50 experiments
        {
            printf("\033[A\33[2K\rRunning experiments [");                          //clear current line in console output, replace with current progress bar
            for(int b=0;b<a/50;b++)                                                 //for every 50th experiment
            {
                printf("#");                                                        //print a # to indicate complete
            }
            for(int b=a/50;b<19;b++)                                                //for the rest of the progress bar
            {
                printf("-");                                                        //print - for incomplete
            }
            printf("]\n");                                                          //close progress bar, move to new line
        }

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();//creates new random seed, based on the system clock
        std::default_random_engine generator (seed);                                //creates  random generator, based on above seed
        std::poisson_distribution<int> distribution(10);                            //sets the generator distribution to poisson distribution

        int pages[nrolls];                                                          //initialize page stream array
        nullArray(pages,0);                                                         //clear garbage from page stream array

        for (int i=0; i<nrolls; ++i)                                                //for all elements in the stream array
        {
            int number = distribution(generator);                                   //generate random number based on poisson distribution
            pages[i]=number;                                                        //place randomly generated number into page stream array
        }

        int FirstIn,LastRecent,Clock;                                               //initialize intigers that store page faults for fifo, lru and clock
                                                                                    //per experiment
        
        for(int j=2;j<=20;j++)                                                      //loop for the number of conducted frame sizes
        {
            
            FirstIn = fifo(pages,nrolls,j);                                         //perform fifo operation and store number of faults
            fifoArray[j-1]+=FirstIn;                                                //add results to the index of the fifo array that correspond to the current frame number
            
            LastRecent = lru(pages,nrolls,j);                                       //perform lru operation and store number of faults
            lruArray[j-1]+=LastRecent;                                              //add results to the index of the lru array that correspond to the current frame number

            Clock = clock(pages,nrolls,j);                                          //perform clock operation and store number of faults
            clockArray[j-1]+=Clock;                                                 //add results to the index of the clock array that correspond to the current frame number
        }
    }
    
    for(int i=1;i<20;i++)                                                           //for every index of the arrays that has values in it
    {                                                                               //divide by the number of experiments to obtain the average for each frame number
        fifoArray[i]=fifoArray[i]/nexp;
        lruArray[i]=lruArray[i]/nexp;
        clockArray[i]=clockArray[i]/nexp;
    }
    
    //TODO format output better
    printf("Size\tFIFO\tLRU\tClock\n");                                             //print results  header                               
    for(int i=1;i<20;i++)                                                           //loop through array of set sizes
    {
        printf("%d:\t%d\t%d\t%d\n",i+1,fifoArray[i],lruArray[i],clockArray[i]);     //print page fault average for each algorithm
    }


    return 0;                                   //end program
}