#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <assert.h>
using namespace std;

/*
Name: Ahmad Mongy Saad Aboelnaga
ID  : 120200033

*/

/* ####################################################### */


/// @brief Abstract Data Typre Queue
/// @tparam Type 
template <class Type>
class queueADT
{
public:
    virtual bool isEmpty() const = 0;
    virtual bool isFull() const = 0;
    virtual void initQ() = 0;
    virtual Type front() const = 0;
    virtual Type back() const = 0;
    virtual void addQueue(const Type &queueElement) = 0;
    virtual void deQueue() = 0;
};

/* ####################################################### */
template <class Type>
/// @brief creates a queue of type Type
/// @tparam Type
class queueType : public queueADT<Type>
{
private:
    int maxQ_Size;
    int count;
    int qFront;
    int qRear;
    Type *list;

public:
    const queueType<Type> &operator=(const queueType<Type> &);
    bool isEmpty() const
    {
        return (count == 0);
    }
    bool isFull() const
    {
        return (count == maxQ_Size);
    }
    void initQ()
    {
        qFront = 0;
        qRear = maxQ_Size - 1;
        count = 0;
    }
    Type front() const
    {
        assert(!isEmpty());
        return list[qFront];
    }

    Type back() const
    {
        assert(!isEmpty());
        return list[qRear];
    }

    void addQueue(const Type &queueElement)
    {
        if (!isFull())
        {
            qRear = (qRear + 1) % maxQ_Size;
            count++;
            list[qRear] = queueElement;
        }
        else
        {
            cout << "Queue is Full \n Cannot add to a full queue." << endl;
        }
    }
    void deQueue()
    {
        if (!isEmpty())
        {
            count--;
            qFront = (qFront + 1) % maxQ_Size;
        }
        else
        {
            cout << "Queue is empty \n Cannot remove from an empty queue." << endl;
        }
    }

    queueType(int queueSize = 100)
    {
        if (queueSize <= 0)
        {
            cout << "size of the array to hold the queue must be positive." << endl;
            cout << "Creating an array of size 100." << endl;
            maxQ_Size = 100;
        }
        else
        {
            maxQ_Size = queueSize;
        }
        qFront = 0;
        qRear = maxQ_Size - 1;
        count = 0;
        list = new Type[maxQ_Size];
    }
    ~queueType()
    {
        delete[] list;
    }
};


/* ####################################################### */

class CustomerType
{
public:
    int customerNumber;
    int arrivalTime;
    int waitingTime;
    int transactionTime;

    CustomerType(int cN = 0, int arrvTime = 0, int wTime = 0, int tTime = 0)
    {
        customerNumber = cN;
        arrivalTime = arrvTime;
        waitingTime = wTime;
        transactionTime = tTime;
    }

    void incrementWaitingTime()
    {
        waitingTime++;
    }

    void setCustomerInfo(int customerN = 0, int arrvTime = 0, int wTime = 0, int tTime = 0)
    {
        customerNumber = customerN;
        arrivalTime = arrvTime;
        waitingTime = wTime;
        transactionTime = tTime;
    }
    void setWaitingTime(int time)
    {
        waitingTime = time;
    }

    int getWaitingTime() const
    {
        return waitingTime;
    }
    int getArrivalTime() const
    {
        return arrivalTime;
    }
    int getTransactionTime() const
    {
        return transactionTime;
    }
    int getCustomerNumber() const
    {
        return customerNumber;
    }
};


/* ####################################################### */

class ServerType
{
public:
    CustomerType currentCustomer;
    int transactionTime;
    bool free;
    ServerType(int t = 0)
    {
        /// @brief creates an object of type ServerType with defult transactionTime=0 if not specified
        /// @param t set transactionTime, 0 by defult
        setFree();
        setTransactionTime(t);
    }

    bool isFree() const
    {

        return free;
    }

    void decreaseTransactionTime()
    {
        transactionTime--;
    }

    void setCurrentCustomer(CustomerType cCustomer)
    {
        currentCustomer = cCustomer;
    }
    void setBusy()
    {
        free = false;
    }
    void setFree()
    {
        free = true;
    }
    void setTransactionTime(int t)
    {
        transactionTime = t;
    }
    void setTransactionTime()
    {
        transactionTime = currentCustomer.getTransactionTime();
    }

    int getRemainingTransactionTime() const
    {
        return transactionTime;
    }
    int getCurrentCustomerNumber() const
    {
        return currentCustomer.getCustomerNumber();
    }
    int getCurrentCustomerArrivalTime() const
    {
        return currentCustomer.getArrivalTime();
    }
    int getCurrentCustomerWaitingTime() const
    {
        return currentCustomer.getWaitingTime();
    }
    int getCurrentCustomerTransactionTime() const
    {
        return currentCustomer.getTransactionTime();
    }
};

class ServerListType
{
public:
    int numOfServers;
    ServerType *servers;
    ServerListType(int num = 1)
    {
        numOfServers = num;
        servers = new ServerType[num];
    }

    ~ServerListType()
    {
        delete[] servers;
    }
    

    void updateServers(ostream &outFile)
    {
        for (int i = 0; i < numOfServers; i++)
        {
            if (!servers[i].isFree())
            {
                servers[i].decreaseTransactionTime();
                if (servers[i].getRemainingTransactionTime() == 0)
                {
                    outFile << "From server number " << (i + 1)
                            << " customer number "
                            << servers[i].getCurrentCustomerNumber()
                            << "\n departed at time unit "
                            <<  servers[i].getCurrentCustomerWaitingTime()
                              + servers[i].getCurrentCustomerTransactionTime()
                              + servers[i].getCurrentCustomerArrivalTime() << endl;
                    servers[i].setFree();
                }
            }
        }
    }


    void setServerBusy(int serverID, CustomerType cCustomer, int tTime)
    {
        servers[serverID].setBusy();
        servers[serverID].setCurrentCustomer(cCustomer);
        servers[serverID].setTransactionTime(tTime);
    }

    void setServerBusy(int serverID, CustomerType cCustomer)
    {
        int time = cCustomer.getTransactionTime();
        servers[serverID].setBusy();
        servers[serverID].setCurrentCustomer(cCustomer);
        servers[serverID].setTransactionTime(time);
    }

    int getFreeServerID() const
    {
        int serverID = -1;
        for (int i = 0; i < numOfServers; i++)
        {
            if (servers[i].isFree())
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
        for (int i = 0; i < numOfServers; i++)
        {
            if (!servers[i].isFree())
            {
                numOfBusyServers++;
            }
        }
        return numOfBusyServers;
    }

};

/* ####################################################### */

template <class Type>
class waitingCustomerQueueType : public queueType<Type>
{
public:
    waitingCustomerQueueType(int t = 100) : queueType<Type>(t) {}

    void updateWaitingQueue()
    {
        CustomerType dummyCust;
        dummyCust.setWaitingTime(-1);
        int wTime = 0;
        queueType<Type>::addQueue(dummyCust);
        while (wTime != -1)
        {
            dummyCust = queueType<Type>::front();
            queueType<Type>::deQueue();
            wTime = dummyCust.getWaitingTime();
            if (wTime == -1)
            {
                break;
            }
            dummyCust.incrementWaitingTime();
            queueType<Type>::addQueue(dummyCust);
        }
    }
};

void setSimulationParameters(int &sTime, int &numOfServers, int &transTime,
int &tBetweenCArrival);

bool isCustomerArrived(double arvTimeDiff);
void generateStatistics(ServerListType &serverList,
waitingCustomerQueueType<CustomerType> &CQueue, int numOfCustArrived,
int waitTimeServedCustomers);

void runSim();

void setSimulationParameters(int &sTime, int &numOfServers, int &transTime,
int &tBetweenCArrival)
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

bool isCustomerArrived(double arvTimeDiff)
{
    double value;

    value = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);

    return (value > exp(-1.0 / arvTimeDiff));
}

void runSim()
{
    int simulationTime;
    int numberOfServers;
    int transactionTime;
    int timeBetweenCustomerArrival;
    setSimulationParameters(simulationTime, numberOfServers,
    transactionTime, timeBetweenCustomerArrival);

    int serverID;

    int custNumber = 0;
    int totalWaitTimeServedCustomers = 0;
    int totalWaitTime = 0;
    int numberOfCustomersServed = 0;
    int customersLeftInServers = 0;
    int clock = 0;

    waitingCustomerQueueType<CustomerType> customerQueue;
    CustomerType customer;
    ServerListType serverList(numberOfServers);

    for (clock = 1; clock <= simulationTime; clock++)
    {
        serverList.updateServers(cout);

        if (!customerQueue.isEmpty())
        {
            customerQueue.updateWaitingQueue();
        }

        if (isCustomerArrived(timeBetweenCustomerArrival))
        {
            custNumber++;
            customer.setCustomerInfo(custNumber, clock, 0,
                                     transactionTime);
            customerQueue.addQueue(customer);
            cout << "Customer number " << custNumber
                 << " arrived at time unit " << clock << endl;
        }

        serverID = serverList.getFreeServerID();

        if (serverID != -1 && !customerQueue.isEmpty())
        {
            customer = customerQueue.front();
            customerQueue.deQueue();
            totalWaitTimeServedCustomers = totalWaitTimeServedCustomers + customer.getWaitingTime();
            serverList.setServerBusy(serverID, customer);
        }
    }
    cout << endl;

    cout << "The simulation ran for " 
    << simulationTime << " time units" << endl;

    cout << "Number of servers: " 
    << numberOfServers << endl;

    cout << "Average transaction time: "
     << transactionTime << endl;

    cout << "Average arrival time difference between customers: "
         << timeBetweenCustomerArrival << endl;


    generateStatistics(serverList, customerQueue,
                       custNumber, totalWaitTimeServedCustomers);
}

void generateStatistics(ServerListType &serverList, waitingCustomerQueueType<CustomerType> &CQueue, int numOfCustArrived, int waitTimeServedCustomers)
{
    int customersLeftInQueue = 0;
    int totalWaitTime = waitTimeServedCustomers;
    CustomerType customer;
    while (!CQueue.isEmpty())
    {
        customer = CQueue.front();
        CQueue.deQueue();
        totalWaitTime = totalWaitTime + customer.getWaitingTime();
        customersLeftInQueue++;
    }
    int customersLeftInServers = serverList.getNumberOfBusyServers();
    int numberOfCustomersServed = numOfCustArrived - customersLeftInServers - customersLeftInQueue;

    double averageWaitTime = 0;

    cout << "Total waiting time: " << totalWaitTime << endl;
    cout << "Number of customers that completed a transaction: " << numberOfCustomersServed << endl;
    cout << "Number of customers left in the servers: " << customersLeftInServers << endl;
    cout << "The number of customers left in queue: " << customersLeftInQueue << endl;

    if (numOfCustArrived > 0)
    {
        averageWaitTime = (static_cast<double>(totalWaitTime)) / numOfCustArrived;
        cout << fixed << showpoint;
        cout << setprecision(2);
        cout << "Average waiting time: " << averageWaitTime << endl;
        cout << "************** END SIMULATION *************" << endl;
    }
}

int main()
{
    runSim();
    system("pause");
    return 0;
}