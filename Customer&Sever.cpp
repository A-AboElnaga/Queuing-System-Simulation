#include <iostream>
#include <cassert>

using namespace std;

//Queueing system

void setFree();
void setTransactionTime(int t);
void setCustomerInfo(int customerN = 0, int inTime = 0, int wTime = 0, int tTime = 0);

template <class Type>
class queueADT
{
public:
    virtual bool isEmptyQueue() const = 0;
    virtual bool isFullQueue() const = 0;
    virtual void initializeQueue() = 0;
    virtual Type front() const = 0;
    virtual Type back() const = 0;
    virtual void addQueue(const Type& queueElement) = 0;
    virtual void deleteQueue() = 0;
};


template<class Type>
class queueType: public queueADT<Type>
{
public:
    const queueType<Type>& operator=(const queueType<Type>&);
    bool isEmptyQueue() const
    {
        return (count == 0);
    }

    bool isFullQueue() const
    {
        return(count == maxQueueSize);
    }
    void initializeQueue()
    {
        queueFront = 0;
        queueRear = maxQueueSize - 1;
        count = 0;
    }

    Type front() const
    {
        assert(!isEmptyQueue());
        return list[queueFront];
    }

    Type back() const
    {
        assert(!isEmptyQueue());
        return list[queueRear];
    }

    void addQueue(const Type& queueElement)
    {
        if (!isFullQueue())
        {
            queueRear = (queueRear + 1) % maxQueueSize;
            count++;
            list [queueRear] = queueElement;
        }
        else
        {
            cout << "Cannot add to a full queue." << endl;
        }
    }
    void deleteQueue()
    {
        if (!isEmptyQueue())
        {
            count--;
            queueFront = (queueFront + 1) % maxQueueSize;
        }
        else
        {
            cout << "Cannot remove from an empty queue." << endl;
        }
    }

    queueType(int queueSize = 100)
    {
        if (queueSize <= 0)
        {
            cout << "size of the array to hold the queue must be positive." << endl;
            cout << "Creating an array of size 100." << endl;
            maxQueueSize= 100;
        }
        else{
            maxQueueSize = queueSize;
        }
        queueFront = 0;
        queueRear = maxQueueSize -1;
        count = 0;
        list = new Type[maxQueueSize];
    }
    queueType(const queueType<Type>& otherQueue);

    ~queueType()
    {
        delete [] list;
    }

private:
    int maxQueueSize;
    int count;
    int queueFront;
    int queueRear;
    Type *list;
};



class customerType
{
public:
    customertype(int cN = 0, int arrvTime = 0, int wTime = 0, int tTime = 0)
    {    
         setCustomerInfo(cN, arrvTime, wTime,tTime);`
    }
    void setCustomerInfo(int customerN = 0, int inTime = 0, int wTime = 0, int tTime = 0){
        int customerNumber = customerN;
        int arrivalTime = inTime;
        int waitingTime = wTime;
        int transactionTime = tTime;
    }


    int getWaitingTime() const
        {return waitingTime;}

    void setWaitingTime(int time)
        {waitingTime = time;}

    void incrementWaitingTime()
        {waitingTime++;}

    int getArrivalTime() const
       {return arrivalTime;}

    int getTransactionTime() const
       {return transactionTime;}

    int getCustomerNumber() const
        {return customerNumber;}
private:
    int customerNumber;
    int arrivalTime;
    int waitingTime;
    int transactionTime;
};

class serverType
{
private:
    customerType currentCustomer;
    int transactionTime;
    bool free;
public:
    serverType(int t){

        setFree();
        setTransactionTime(t);
    }

    bool isFree() const{
        if (free)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void setBusy(){
        free = false;
    }
    void setFree(){
        free = true;
    }
    void setTransactionTime(int t){
        transactionTime = t;
    }
    void setTransactionTime()
        {int time = currentCustomer.getTransactionTime();
            transactionTime = time;
        }

    int getRemainingTransactionTime() const
        {return transactionTime;}

    void decreaseTransactionTime()
        {transactionTime--;}

    void setCurrentCustomer(customerType cCustomer)
        {currentCustomer = cCustomer;}

    int getCurrentCustomerNumber() const
        {return currentCustomer.getCustomerNumber();}

    int getCurrentCustomerArrivalTime() const
        {return currentCustomer.getArrivalTime();}

    int getCurrentCustomerWaitingTime() const
        {return currentCustomer.getWaitingTime();}

    int getCurrentCustomerTransactionTime() const
        {return currentCustomer.getTransactionTime();}

};

class serverListType
{
public:
    serverListType(int num = 1)
    {
        numOfServers = num;
        serverType *servers = new serverType(num);
    }
    ~serverListType()
    {
        delete [] servers;
    }
    int getFreeServerID() const
    {
        int serverID = -1;
        for(int i =0; i < numOfServers; i++)
        {
            if(servers[i].isFree())
            {
                serverID = i;
                break;
            }
        }
        return serverID;
    }
    int getNumberOfBusyServers() const
    {
        int numOfBusyServers = 0;
        for(int i = 0; i < numOfServers; i++)
        {
            if(!servers[i].isFree())
            {
                numOfBusyServers ++;
            }
        }
        return numOfBusyServers;
    }
    void setServerBusy(int serverID, customerType cCustomer, int tTime)
    {
        servers[serverID].setBusy();
        servers[serverID].setCurrentCustomer(cCustomer);
        servers[serverID].setTransactionTime(tTime);
    }
    void setServerBusy(int serverID, customerType cCustomer)
    {
        int time = cCustomer.getTransactionTime();
        servers[serverID].setBusy();
        servers[serverID].setCurrentCustomer(cCustomer);
        servers[serverID].setTransactionTime(time);
    }
    void updateServers(ostream& outFile)
    {
        for(int i = 0; i < numOfServers; i++)
        {
            if (!servers[i].isFree())
            {
                servers[i].decreaseTransactionTime();
                if(servers[i].getRemainingTransactionTime() == 0)
                {
                  outFile << "From server number " << (i + 1)
                          << " customer number "
                          << servers[i].getCurrentCustomerNumber()
                          << "\n departed at time unit "
                          << servers[i].getCurrentCustomerArrivalTime() + servers[i].getCurrentCustomerWaitingTime()+ servers[i]. getCurrentCustomerTransactionTime() << endl;
                  servers[i].setFree();
                }
            }

        }
    }
private:
    int numOfServers;
    serverType *servers;
};

class waitingCustomerQueueType: public queueType<customerType>
{
public:
    waitingCustomerQueueType()
    {
       waitingCustomerQueueType  CustomersQueue =  ;

    }
    void updateWaitingQueue()
    {
        customerType cust;
        cust.setWaitingTime(-1);
        int wTime = 0;

        addQueue(cust);

        while (wTime != -1)
        {
            cust = front();
            deleteQueue();
            wTime = cust.getWaitingTime();
            if (wTime == -1)
            {
                break;
            }
            cust.incrementWaitingTime();
            addQueue(cust);
        }
}

};

void setSimulationParameters(int& sTime, int& numOfServers, int& transTime, int& tBetweenCArrival)
{
    cout << "Enter the simulation time: ";
    cin >> sTime;
    cout << endl;
    cout << "Enter the number of servers: ";
    cin >> numOfServers;
    cout << endl;
    cout << "Enter the transaction time: ";
    cin >> transTime;
    cout << endl;
    cout << "Enter the time between customer arrivals: ";
    cin >> tBetweenCArrival;
    cout << endl;
}

void runSim(int& sTime, int& numOfServers, int& transTime, int& tBetweenCArrival,int numOfCust, int clock, int TotalwTime, int AvgwTime, int numCustArrv, int numCustServed, int numOfCustNQueue, int numOfCustLeft,  serverListType *servers)
{
    setSimulationParameters(sTime, numOfServers, transTime, tBetweenCArrival);

    clock = 0;
    TotalwTime = 0;
    numCustArrv = 0;
    numCustServed = 0;
    numCustServed = 0;
    numOfCustLeft = 0;
    numOfCustNQueue = 0;

    waitingCustomerQueueType();

    for(clock = 1; clock <= sTime; clock++)
    {
        for(int i = 0; i < numOfServers; i++)
        {
            if(!servers[i].isFree())
            {
                servers[i].decreaseTransactionTime();
            }
             if (!waitingCustomerQueueType().isEmptyQueue())
            {
                updateWaitingQueue();
            }
            
            if(!servers[i].isBusy())
            {
                updateWaitingQueue();
            }
        }
           
    }
}



int main()
 {

 }
